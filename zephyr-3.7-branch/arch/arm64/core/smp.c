//arch/arc/core/smp.c
/**
 * @file
 * @brief Codes required for AArch64 multicore and Zephyr SMP support
 */

#include <zephyr/cache.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/kernel.h>
#include <zephyr/kernel_structs.h>
#include <ksched.h>
#include <ipi.h>
#include <zephyr/init.h>
#include <zephyr/arch/arm64/mm.h>
#include <zephyr/arch/cpu.h>
#include <zephyr/drivers/interrupt_controller/gic.h>
#include <zephyr/drivers/pm_cpu_ops.h>
#include <zephyr/arch/arch_interface.h>
#include <zephyr/sys/barrier.h>
#include <zephyr/irq.h>
#include "boot.h"

#define INV_MPID UINT64_MAX

#define SGI_SCHED_IPI 0
#define SGI_MMCFG_IPI 1
#define SGI_FPU_IPI 2

struct boot_params {
	uint64_t mpid;
	char *sp;
	uint8_t voting[CONFIG_MP_MAX_NUM_CPUS];
	arch_cpustart_t fn;
	void *arg;
	int cpu_num;
};

/* Offsets used in reset.S */
BUILD_ASSERT(offsetof(struct boot_params, mpid) == BOOT_PARAM_MPID_OFFSET);
BUILD_ASSERT(offsetof(struct boot_params, sp) == BOOT_PARAM_SP_OFFSET);
BUILD_ASSERT(offsetof(struct boot_params, voting) == BOOT_PARAM_VOTING_OFFSET);

volatile struct boot_params __aligned(L1_CACHE_BYTES) arm64_cpu_boot_params = {
	.mpid = -1,
};

const uint64_t cpu_node_list[] = {
	DT_FOREACH_CHILD_STATUS_OKAY_SEP(DT_PATH(cpus), DT_REG_ADDR, (,))
};

/* cpu_map saves the mapping of core id and mpid */
static uint64_t cpu_map[CONFIG_MP_MAX_NUM_CPUS] = {
	[0 ... (CONFIG_MP_MAX_NUM_CPUS - 1)] = INV_MPID
};

extern void z_arm64_mm_init(bool is_primary_core);

/**
 * @brief Start a CPU
 *
 * @param cpu_num CPU number
 * @param stack Stack pointer
 * @param sz Stack size
 * @param fn Function to execute
 * @param arg Argument to pass to the function
 */
void arch_cpu_start(int cpu_num, k_thread_stack_t *stack, int sz,
		    arch_cpustart_t fn, void *arg)
{
	int cpu_count;
	static int i;
	uint64_t cpu_mpid = 0;
	uint64_t master_core_mpid;

	/* Now it is on master core */
	__ASSERT(arch_curr_cpu()->id == 0, "");
	master_core_mpid = MPIDR_TO_CORE(GET_MPIDR());

	cpu_count = ARRAY_SIZE(cpu_node_list);

#ifdef CONFIG_ARM64_FALLBACK_ON_RESERVED_CORES
	__ASSERT(cpu_count >= CONFIG_MP_MAX_NUM_CPUS,
		"The count of CPU Core nodes in dts is not greater or equal to CONFIG_MP_MAX_NUM_CPUS\n");
#else
	__ASSERT(cpu_count == CONFIG_MP_MAX_NUM_CPUS,
		"The count of CPU Cores nodes in dts is not equal to CONFIG_MP_MAX_NUM_CPUS\n");
#endif

	arm64_cpu_boot_params.sp = K_KERNEL_STACK_BUFFER(stack) + sz;
	arm64_cpu_boot_params.fn = fn;
	arm64_cpu_boot_params.arg = arg;
	arm64_cpu_boot_params.cpu_num = cpu_num;

	for (; i < cpu_count; i++) {
		if (cpu_node_list[i] == master_core_mpid) {
			continue;
		}

		cpu_mpid = cpu_node_list[i];

		barrier_dsync_fence_full();

		/* store mpid last as this is our synchronization point */
		arm64_cpu_boot_params.mpid = cpu_mpid;

		sys_cache_data_flush_range((void *)&arm64_cpu_boot_params,
					  sizeof(arm64_cpu_boot_params));

		if (pm_cpu_on(cpu_mpid, (uint64_t)&__start)) {
			printk("Failed to boot secondary CPU core %d (MPID:%#llx)\n",
			       cpu_num, cpu_mpid);
#ifdef CONFIG_ARM64_FALLBACK_ON_RESERVED_CORES
			printk("Falling back on reserved cores\n");
			continue;
#else
			k_panic();
#endif
		}

		break;
	}
	if (i++ == cpu_count) {
		printk("Can't find CPU Core %d from dts and failed to boot it\n", cpu_num);
		k_panic();
	}

	/* Wait secondary cores up, see arch_secondary_cpu_init */
	while (arm64_cpu_boot_params.fn) {
		wfe();
	}

	cpu_map[cpu_num] = cpu_mpid;

	printk("Secondary CPU core %d (MPID:%#llx) is up\n", cpu_num, cpu_mpid);
}

/**
 * @brief Initialize secondary CPU
 *
 * @param cpu_num CPU number
 */
void arch_secondary_cpu_init(int cpu_num)
{
	cpu_num = arm64_cpu_boot_params.cpu_num;
	arch_cpustart_t fn;
	void *arg;

	__ASSERT(arm64_cpu_boot_params.mpid == MPIDR_TO_CORE(GET_MPIDR()), "");

	/* Initialize tpidrro_el0 with our struct _cpu instance address */
	write_tpidrro_el0((uintptr_t)&_kernel.cpus[cpu_num]);

	z_arm64_mm_init(false);

#ifdef CONFIG_ARM64_SAFE_EXCEPTION_STACK
	z_arm64_safe_exception_stack_init();
#endif

#ifdef CONFIG_SMP
	arm_gic_secondary_init();

	irq_enable(SGI_SCHED_IPI);
#ifdef CONFIG_USERSPACE
	irq_enable(SGI_MMCFG_IPI);
#endif
#ifdef CONFIG_FPU_SHARING
	irq_enable(SGI_FPU_IPI);
#endif
#endif

	fn = arm64_cpu_boot_params.fn;
	arg = arm64_cpu_boot_params.arg;
	barrier_dsync_fence_full();

	/*
	 * Secondary core clears .fn to announce its presence.
	 * Primary core is polling for this. We no longer own
	 * arm64_cpu_boot_params afterwards.
	 */
	arm64_cpu_boot_params.fn = NULL;
	barrier_dsync_fence_full();
	sev();

	fn(arg);
}

#ifdef CONFIG_SMP

/**
 * @brief Send an IPI to the specified CPUs
 *
 * @param ipi IPI number
 * @param cpu_bitmap Bitmap of CPUs to send the IPI to
 */
static void send_ipi(unsigned int ipi, uint32_t cpu_bitmap)
{
	uint64_t mpidr = MPIDR_TO_CORE(GET_MPIDR());

	/*
	 * Send SGI to all cores except itself
	 */
	unsigned int num_cpus = arch_num_cpus();

	for (int i = 0; i < num_cpus; i++) {
		if ((cpu_bitmap & BIT(i)) == 0) {
			continue;
		}

		uint64_t target_mpidr = cpu_map[i];
		uint8_t aff0;

		if (mpidr == target_mpidr || target_mpidr == INV_MPID) {
			continue;
		}

		aff0 = MPIDR_AFFLVL(target_mpidr, 0);
		gic_raise_sgi(ipi, target_mpidr, 1 << aff0);
	}
}

/**
 * @brief Handler for scheduling IPI
 *
 * @param unused Unused parameter
 */
void sched_ipi_handler(const void *unused)
{
	ARG_UNUSED(unused);

	z_sched_ipi();
}

/**
 * @brief Broadcast a scheduling IPI to all CPUs
 */
void arch_sched_broadcast_ipi(void)
{
	send_ipi(SGI_SCHED_IPI, IPI_ALL_CPUS_MASK);
}

/**
 * @brief Send a directed scheduling IPI to specified CPUs
 *
 * @param cpu_bitmap Bitmap of CPUs to send the IPI to
 */
void arch_sched_directed_ipi(uint32_t cpu_bitmap)
{
	send_ipi(SGI_SCHED_IPI, cpu_bitmap);
}

#ifdef CONFIG_USERSPACE
/**
 * @brief Handler for memory configuration IPI
 *
 * @param unused Unused parameter
 */
void mem_cfg_ipi_handler(const void *unused)
{
	ARG_UNUSED(unused);
	unsigned int key = arch_irq_lock();

	/*
	 * Make sure a domain switch by another CPU is effective on this CPU.
	 * This is a no-op if the page table is already the right one.
	 * Lock irq to prevent the interrupt during mem region switch.
	 */
	z_arm64_swap_mem_domains(_current);
	arch_irq_unlock(key);
}

/**
 * @brief Send a memory configuration IPI to all CPUs
 */
void z_arm64_mem_cfg_ipi(void)
{
	send_ipi(SGI_MMCFG_IPI, IPI_ALL_CPUS_MASK);
}
#endif

#ifdef CONFIG_FPU_SHARING
/**
 * @brief Handler for FPU flush IPI
 *
 * @param unused Unused parameter
 */
void flush_fpu_ipi_handler(const void *unused)
{
	ARG_UNUSED(unused);

	disable_irq();
	arch_flush_local_fpu();
	/* no need to re-enable IRQs here */
}

/**
 * @brief Send an FPU flush IPI to a specified CPU
 *
 * @param cpu CPU number
 */
void arch_flush_fpu_ipi(unsigned int cpu)
{
	const uint64_t mpidr = cpu_map[cpu];
	uint8_t aff0;

	if (mpidr == INV_MPID) {
		return;
	}

	aff0 = MPIDR_AFFLVL(mpidr, 0);
	gic_raise_sgi(SGI_FPU_IPI, mpidr, 1 << aff0);
}

/**
 * @brief Relax the spinlock and handle pending FPU flush requests
 */
void arch_spin_relax(void)
{
	if (arm_gic_irq_is_pending(SGI_FPU_IPI)) {
		arm_gic_irq_clear_pending(SGI_FPU_IPI);
		/*
		 * We may not be in IRQ context here hence cannot use
		 * arch_flush_local_fpu() directly.
		 */
		arch_float_disable(_current_cpu->arch.fpu_owner);
	}
}
#endif

/**
 * @brief Initialize SMP support
 *
 * @return 0 on success
 */
int arch_smp_init(void)
{
	cpu_map[0] = MPIDR_TO_CORE(GET_MPIDR());

	/*
	 * SGI0 is used for sched IPI, this might be changed to use Kconfig
	 * option
	 */
	IRQ_CONNECT(SGI_SCHED_IPI, IRQ_DEFAULT_PRIORITY, sched_ipi_handler, NULL, 0);
	irq_enable(SGI_SCHED_IPI);

#ifdef CONFIG_USERSPACE
	IRQ_CONNECT(SGI_MMCFG_IPI, IRQ_DEFAULT_PRIORITY,
			mem_cfg_ipi_handler, NULL, 0);
	irq_enable(SGI_MMCFG_IPI);
#endif
#ifdef CONFIG_FPU_SHARING
	IRQ_CONNECT(SGI_FPU_IPI, IRQ_DEFAULT_PRIORITY, flush_fpu_ipi_handler, NULL, 0);
	irq_enable(SGI_FPU_IPI);
#endif

	return 0;
}

#endif /* CONFIG_SMP */
//GST