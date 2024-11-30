// zephyr-3.7-branch/include/zephyr/arch/arm64/asm_inline.h

#ifndef ZEPHYR_INCLUDE_ARCH_ARM64_ASM_INLINE_H_
#define ZEPHYR_INCLUDE_ARCH_ARM64_ASM_INLINE_H_

/*
 * The file must not be included directly
 * Include kernel.h instead
 */

#if defined(__GNUC__)
#include <zephyr/arch/arm64/asm_inline_gcc.h>
#else
#include <arch/arm/asm_inline_other.h>
#endif

#endif /* ZEPHYR_INCLUDE_ARCH_ARM64_ASM_INLINE_H_ */
//GST