// zephyr-3.7-branch/include/zephyr/arch/arm64/asm_inline_gcc.h

#ifndef ZEPHYR_INCLUDE_ARCH_ARM64_ASM_INLINE_GCC_H_
#define ZEPHYR_INCLUDE_ARCH_ARM64_ASM_INLINE_GCC_H_

/*
 * The file must not be included directly
 * Include arch/cpu.h instead
 */

#ifndef _ASMLANGUAGE

#include <zephyr/arch/arm64/lib_helpers.h>
#include <zephyr/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Lock IRQs and return the previous state
 *
 * This function locks IRQs and returns the previous state of the DAIF register.
 *
 * @return The previous state of the DAIF register
 */
static ALWAYS_INLINE unsigned int arch_irq_lock(void)
{
	unsigned int key;

	/*
	 * Return the whole DAIF register as key but use DAIFSET to disable
	 * IRQs.
	 */
	key = read_daif();
	disable_irq();

	return key;
}

/**
 * @brief Unlock IRQs and restore the previous state
 *
 * This function unlocks IRQs and restores the previous state of the DAIF register.
 *
 * @param key The previous state of the DAIF register
 */
static ALWAYS_INLINE void arch_irq_unlock(unsigned int key)
{
	write_daif(key);
}

/**
 * @brief Check if IRQs are unlocked
 *
 * This function checks if IRQs are unlocked by examining the IRQ bit in the DAIF register.
 *
 * @param key The state of the DAIF register
 * @return True if IRQs are unlocked, false otherwise
 */
static ALWAYS_INLINE bool arch_irq_unlocked(unsigned int key)
{
	/* We only check the (I)RQ bit on the DAIF register */
	return (key & DAIF_IRQ_BIT) == 0;
}

#ifdef __cplusplus
}
#endif

#endif /* _ASMLANGUAGE */

#endif /* ZEPHYR_INCLUDE_ARCH_ARM64_ASM_INLINE_GCC_H_ */
//GST