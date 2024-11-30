// zephyr-3.7-branch/include/zephyr/arch/arm64/arch.h

#ifndef ZEPHYR_INCLUDE_ARCH_ARM64_ARCH_H_
#define ZEPHYR_INCLUDE_ARCH_ARM64_ARCH_H_

/* Add include for DTS generated information */
#include <zephyr/devicetree.h>

#include <zephyr/arch/arm64/thread.h>
#include <zephyr/arch/arm64/exception.h>
#include <zephyr/arch/arm64/irq.h>
#include <zephyr/arch/arm64/misc.h>
#include <zephyr/arch/arm64/asm_inline.h>
#include <zephyr/arch/arm64/cpu.h>
#include <zephyr/arch/arm64/macro.inc>
#include <zephyr/arch/arm64/sys_io.h>
#include <zephyr/arch/arm64/timer.h>
#include <zephyr/arch/arm64/error.h>
#include <zephyr/arch/arm64/mm.h>
#include <zephyr/arch/arm64/thread_stack.h>
#include <zephyr/arch/common/addr_types.h>
#include <zephyr/arch/common/sys_bitops.h>
#include <zephyr/arch/common/ffs.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _ASMLANGUAGE

#include <zephyr/sys/slist.h>

/**
 * @brief Architecture-specific memory domain structure
 *
 * This structure represents the architecture-specific memory domain.
 */
struct arch_mem_domain {
#ifdef CONFIG_ARM_MMU
	struct arm_mmu_ptables ptables; /**< Page tables for ARM MMU */
#endif
	sys_snode_t node; /**< System node */
};

#endif /* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ARCH_ARM64_ARCH_H_ */

// zephyr-3.7-branch/include/zephyr/arch/xtensa/arch_inlines.h

#ifndef ZEPHYR_INCLUDE_ARCH_XTENSA_ARCH_INLINES_H_
#define ZEPHYR_INCLUDE_ARCH_XTENSA_ARCH_INLINES_H_

#ifndef _ASMLANGUAGE

#include <zephyr/kernel_structs.h>
#include <zephyr/zsr.h>

/**
 * @brief Read a special register.
 *
 * @param sr Name of special register.
 *
 * @return Value of special register.
 */
#define XTENSA_RSR(sr) \
	({uint32_t v; \
	 __asm__ volatile ("rsr." sr " %0" : "=a"(v)); \
	 v; })

/**
 * @brief Write to a special register.
 *
 * @param sr Name of special register.
 * @param v Value to be written to special register.
 */
#define XTENSA_WSR(sr, v) \
	do { \
		__asm__ volatile ("wsr." sr " %0" : : "r"(v)); \
	} while (false)

/**
 * @brief Read a user register.
 *
 * @param ur Name of user register.
 *
 * @return Value of user register.
 */
#define XTENSA_RUR(ur) \
	({uint32_t v; \
	 __asm__ volatile ("rur." ur " %0" : "=a"(v)); \
	 v; })

/**
 * @brief Write to a user register.
 *
 * @param ur Name of user register.
 * @param v Value to be written to user register.
 */
#define XTENSA_WUR(ur, v) \
	do { \
		__asm__ volatile ("wur." ur " %0" : : "r"(v)); \
	} while (false)

/** Implementation of @ref arch_curr_cpu. */
static ALWAYS_INLINE _cpu_t *arch_curr_cpu(void)
{
	_cpu_t *cpu;

	cpu = (_cpu_t *)XTENSA_RSR(ZSR_CPU_STR);

	return cpu;
}

/** Implementation of @ref arch_proc_id. */
static ALWAYS_INLINE uint32_t arch_proc_id(void)
{
	uint32_t prid;

	__asm__ volatile("rsr %0, PRID" : "=r"(prid));
	return prid;
}

#ifdef CONFIG_SOC_HAS_RUNTIME_NUM_CPUS
extern unsigned int soc_num_cpus;
#endif

/** Implementation of @ref arch_num_cpus. */
static ALWAYS_INLINE unsigned int arch_num_cpus(void)
{
#ifdef CONFIG_SOC_HAS_RUNTIME_NUM_CPUS
	return soc_num_cpus;
#else
	return CONFIG_MP_MAX_NUM_CPUS;
#endif
}

#endif /* !_ASMLANGUAGE */

#endif /* ZEPHYR_INCLUDE_ARCH_XTENSA_ARCH_INLINES_H_ */
//GST