// zephyr-3.7-branch/include/zephyr/arch/arm64/misc.h

#ifndef ZEPHYR_INCLUDE_ARCH_ARM64_MISC_H_
#define ZEPHYR_INCLUDE_ARCH_ARM64_MISC_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _ASMLANGUAGE

/**
 * @brief Get the current system clock cycle count (32-bit)
 *
 * This function returns the current system clock cycle count as a 32-bit value.
 *
 * @return The current system clock cycle count (32-bit)
 */
extern uint32_t sys_clock_cycle_get_32(void);

/**
 * @brief Get the current architecture-specific clock cycle count (32-bit)
 *
 * This function returns the current architecture-specific clock cycle count as a 32-bit value.
 *
 * @return The current architecture-specific clock cycle count (32-bit)
 */
static inline uint32_t arch_k_cycle_get_32(void)
{
	return sys_clock_cycle_get_32();
}

/**
 * @brief Get the current system clock cycle count (64-bit)
 *
 * This function returns the current system clock cycle count as a 64-bit value.
 *
 * @return The current system clock cycle count (64-bit)
 */
extern uint64_t sys_clock_cycle_get_64(void);

/**
 * @brief Get the current architecture-specific clock cycle count (64-bit)
 *
 * This function returns the current architecture-specific clock cycle count as a 64-bit value.
 *
 * @return The current architecture-specific clock cycle count (64-bit)
 */
static inline uint64_t arch_k_cycle_get_64(void)
{
	return sys_clock_cycle_get_64();
}

/**
 * @brief Execute a no-operation instruction
 *
 * This function executes a no-operation (NOP) instruction.
 */
static ALWAYS_INLINE void arch_nop(void)
{
	__asm__ volatile("nop");
}

#endif /* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ARCH_ARM64_MISC_H_ */
//GST