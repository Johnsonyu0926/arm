//arch/arc/core/fpu.c
#include <zephyr/kernel.h>
#include <zephyr/kernel_structs.h>
#include <kernel_arch_interface.h>
#include <zephyr/arch/cpu.h>
#include <zephyr/sys/barrier.h>
#include <zephyr/sys/atomic.h>

/* External declarations for FPU save and restore functions */
extern void z_arm64_fpu_save(struct z_arm64_fp_context *saved_fp_context);
extern void z_arm64_fpu_restore(struct z_arm64_fp_context *saved_fp_context);

#define FPU_DEBUG 0

#if FPU_DEBUG

#include <string.h>

/**
 * @brief Debug function to print FPU context information.
 *
 * @param msg Debug message.
 * @param th Pointer to the thread.
 */
static void DBG(char *msg, struct k_thread *th)
{
	char buf[80], *p;
	unsigned int v;

	strcpy(buf, "CPU# exc# ");
	buf[3] = '0' + _current_cpu->id;
	buf[8] = '0' + arch_exception_depth();
	strcat(buf, _current->name);
	strcat(buf, ": ");
	strcat(buf, msg);
	strcat(buf, " ");
	strcat(buf, th->name);

	v = *(unsigned char *)&th->arch.saved_fp_context;
	p = buf + strlen(buf);
	*p++ = ' ';
	*p++ = ((v >> 4) < 10) ? ((v >> 4) + '0') : ((v >> 4) - 10 + 'a');
	*p++ = ((v & 15) < 10) ? ((v & 15) + '0') : ((v & 15) - 10 + 'a');
	*p++ = '\n';
	*p = 0;

	k_str_out(buf, p - buf);
}

#else

static inline void DBG(char *msg, struct k_thread *t) { }

#endif /* FPU_DEBUG */

/**
 * @brief Flush FPU content and disable access.
 *
 * This is called locally and also from flush_fpu_ipi_handler().
 */
void arch_flush_local_fpu(void)
{
	__ASSERT(read_daif() & DAIF_IRQ_BIT, "must be called with IRQs disabled");

	struct k_thread *owner = atomic_ptr_get(&_current_cpu->arch.fpu_owner);

	if (owner != NULL) {
		uint64_t cpacr = read_cpacr_el1();

		/* Turn on FPU access */
		write_cpacr_el1(cpacr | CPACR_EL1_FPEN_NOTRAP);
		barrier_isync_fence_full();

		/* Save current owner's content */
		z_arm64_fpu_save(&owner->arch.saved_fp_context);
		barrier_dsync_fence_full();
		atomic_ptr_clear(&_current_cpu->arch.fpu_owner);
		DBG("disable", owner);

		/* Disable FPU access */
		write_cpacr_el1(cpacr & ~CPACR_EL1_FPEN_NOTRAP);
		barrier_isync_fence_full();
	}
}

#ifdef CONFIG_SMP
/**
 * @brief Flush the FPU context of a specific thread.
 *
 * @param thread Pointer to the thread.
 */
static void flush_owned_fpu(struct k_thread *thread)
{
	__ASSERT(read_daif() & DAIF_IRQ_BIT, "must be called with IRQs disabled");

	unsigned int num_cpus = arch_num_cpus();

	for (int i = 0; i < num_cpus; i++) {
		if (atomic_ptr_get(&_kernel.cpus[i].arch.fpu_owner) != thread) {
			continue;
		}
		if (i == _current_cpu->id) {
			arch_flush_local_fpu();
		} else {
			arch_flush_fpu_ipi(i);
			if (thread == _current) {
				arch_flush_local_fpu();
				while (atomic_ptr_get(&_kernel.cpus[i].arch.fpu_owner) == thread) {
					barrier_dsync_fence_full();
				}
			}
		}
		break;
	}
}
#endif

/**
 * @brief Enter exception context for FPU.
 */
void z_arm64_fpu_enter_exc(void)
{
	__ASSERT(read_daif() & DAIF_IRQ_BIT, "must be called with IRQs disabled");

	/* Always deny FPU access whenever an exception is entered */
	write_cpacr_el1(read_cpacr_el1() & ~CPACR_EL1_FPEN_NOTRAP);
	barrier_isync_fence_full();
}

/**
 * @brief Simulate FPU store instructions.
 *
 * @param esf Pointer to the exception stack frame.
 * @return true if simulation was successful, false otherwise.
 */
static bool simulate_str_q_insn(struct arch_esf *esf)
{
	if (arch_exception_depth() <= 1) {
		return false;
	}

	uint32_t *pc = (uint32_t *)esf->elr;
	uintptr_t sp = (uintptr_t)esf + sizeof(*esf);

	for (;;) {
		uint32_t insn = *pc;

		if ((insn & 0xffc003f8) != 0x3d8003e0) {
			break;
		}

		uint32_t pimm = (insn >> 10) & 0xfff;
		*(__int128 *)(sp + pimm * 16) = 0;
		pc++;
	}

	if (pc != (uint32_t *)esf->elr) {
		esf->elr = (uintptr_t)pc;
		return true;
	}

	return false;
}

/**
 * @brief Process the FPU trap.
 *
 * @param esf Pointer to the exception stack frame.
 */
void z_arm64_fpu_trap(struct arch_esf *esf)
{
	__ASSERT(read_daif() & DAIF_IRQ_BIT, "must be called with IRQs disabled");

	if (simulate_str_q_insn(esf)) {
		return;
	}

	write_cpacr_el1(read_cpacr_el1() | CPACR_EL1_FPEN_NOTRAP);
	barrier_isync_fence_full();

	struct k_thread *owner = atomic_ptr_get(&_current_cpu->arch.fpu_owner);

	if (owner) {
		z_arm64_fpu_save(&owner->arch.saved_fp_context);
		barrier_dsync_fence_full();
		atomic_ptr_clear(&_current_cpu->arch.fpu_owner);
		DBG("save", owner);
	}

	if (arch_exception_depth() > 1) {
		esf->spsr |= DAIF_IRQ_BIT;
		return;
	}

#ifdef CONFIG_SMP
	flush_owned_fpu(_current);
#endif

	atomic_ptr_set(&_current_cpu->arch.fpu_owner, _current);
	z_arm64_fpu_restore(&_current->arch.saved_fp_context);
	DBG("restore", _current);
}

/**
 * @brief Update FPU access based on ownership.
 *
 * @param exc_update_level Exception update level.
 */
static void fpu_access_update(unsigned int exc_update_level)
{
	__ASSERT(read_daif() & DAIF_IRQ_BIT, "must be called with IRQs disabled");

	uint64_t cpacr = read_cpacr_el1();

	if (arch_exception_depth() == exc_update_level) {
		if (atomic_ptr_get(&_current_cpu->arch.fpu_owner) == _current) {
			write_cpacr_el1(cpacr | CPACR_EL1_FPEN_NOTRAP);
		} else {
			write_cpacr_el1(cpacr & ~CPACR_EL1_FPEN_NOTRAP);
		}
	} else {
		write_cpacr_el1(cpacr & ~CPACR_EL1_FPEN_NOTRAP);
	}
	barrier_isync_fence_full();
}

/**
 * @brief Handle FPU access on exception exit.
 */
void z_arm64_fpu_exit_exc(void)
{
	fpu_access_update(1);
}

/**
 * @brief Handle FPU access during thread context switch.
 */
void z_arm64_fpu_thread_context_switch(void)
{
	fpu_access_update(0);
}

/**
 * @brief Disable FPU for a specific thread.
 *
 * @param thread Pointer to the thread.
 * @return 0 on success.
 */
int arch_float_disable(struct k_thread *thread)
{
	if (thread != NULL) {
		unsigned int key = arch_irq_lock();

#ifdef CONFIG_SMP
		flush_owned_fpu(thread);
#else
		if (thread == atomic_ptr_get(&_current_cpu->arch.fpu_owner)) {
			arch_flush_local_fpu();
		}
#endif

		arch_irq_unlock(key);
	}

	return 0;
}

/**
 * @brief Enable FPU for a specific thread.
 *
 * @param thread Pointer to the thread.
 * @param options Options for enabling FPU.
 * @return 0 on success.
 */
int arch_float_enable(struct k_thread *thread, unsigned int options)
{
	return 0;
}
//GST