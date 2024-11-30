// zephyr-3.7-branch/include/zephyr/arch/arm64/thread.h

#ifndef ZEPHYR_INCLUDE_ARCH_ARM64_THREAD_H_
#define ZEPHYR_INCLUDE_ARCH_ARM64_THREAD_H_

#ifndef _ASMLANGUAGE
#include <zephyr/types.h>
#include <zephyr/arch/arm64/mm.h>

/**
 * @brief Callee-saved registers structure
 *
 * This structure represents the callee-saved registers.
 */
struct _callee_saved {
	uint64_t x19; /**< Callee-saved register x19 */
	uint64_t x20; /**< Callee-saved register x20 */
	uint64_t x21; /**< Callee-saved register x21 */
	uint64_t x22; /**< Callee-saved register x22 */
	uint64_t x23; /**< Callee-saved register x23 */
	uint64_t x24; /**< Callee-saved register x24 */
	uint64_t x25; /**< Callee-saved register x25 */
	uint64_t x26; /**< Callee-saved register x26 */
	uint64_t x27; /**< Callee-saved register x27 */
	uint64_t x28; /**< Callee-saved register x28 */
	uint64_t x29; /**< Callee-saved register x29 */
	uint64_t sp_el0; /**< Stack pointer for EL0 */
	uint64_t sp_elx; /**< Stack pointer for ELx */
	uint64_t lr; /**< Link register */
};

typedef struct _callee_saved _callee_saved_t;

/**
 * @brief Floating-point context structure
 *
 * This structure represents the floating-point context.
 */
struct z_arm64_fp_context {
	__int128 q0,  q1,  q2,  q3,  q4,  q5,  q6,  q7;
	__int128 q8,  q9,  q10, q11, q12, q13, q14, q15;
	__int128 q16, q17, q18, q19, q20, q21, q22, q23;
	__int128 q24, q25, q26, q27, q28, q29, q30, q31;
	uint32_t fpsr, fpcr; /**< Floating-point status and control registers */
};

/**
 * @brief Thread architecture-specific structure
 *
 * This structure represents the architecture-specific information for a thread.
 */
struct _thread_arch {
#if defined(CONFIG_USERSPACE) || defined(CONFIG_ARM64_STACK_PROTECTION)
#if defined(CONFIG_ARM_MMU)
	struct arm_mmu_ptables *ptables; /**< MMU page tables */
#endif
#if defined(CONFIG_ARM_MPU)
	struct dynamic_region_info regions[ARM64_MPU_MAX_DYNAMIC_REGIONS]; /**< MPU regions */
	uint8_t region_num; /**< Number of MPU regions */
	atomic_t flushing; /**< Flushing state */
#endif
#endif
#ifdef CONFIG_ARM64_SAFE_EXCEPTION_STACK
	uint64_t stack_limit; /**< Stack limit */
#endif
#ifdef CONFIG_FPU_SHARING
	struct z_arm64_fp_context saved_fp_context; /**< Saved floating-point context */
#endif
	uint8_t exception_depth; /**< Exception depth */
};

typedef struct _thread_arch _thread_arch_t;

#endif /* _ASMLANGUAGE */

#endif /* ZEPHYR_INCLUDE_ARCH_ARM64_THREAD_H_ */
//GST