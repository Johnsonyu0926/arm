//arch/arc/include/exception.h
#ifndef ZEPHYR_ARCH_ARM64_INCLUDE_EXCEPTION_H_
#define ZEPHYR_ARCH_ARM64_INCLUDE_EXCEPTION_H_

#include <zephyr/arch/cpu.h>

#ifdef _ASMLANGUAGE

/* Nothing to include for assembly language */

#else

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Check if the current context is an ISR
 *
 * This function checks if the current context is an ISR (Interrupt Service Routine).
 *
 * @return true if the current context is an ISR, false otherwise
 */
static ALWAYS_INLINE bool arch_is_in_isr(void)
{
	return arch_curr_cpu()->nested != 0U;
}

#ifdef __cplusplus
}
#endif

#endif /* _ASMLANGUAGE */

#endif /* ZEPHYR_ARCH_ARM64_INCLUDE_EXCEPTION_H_ */
//GST
