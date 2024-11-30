// zephyr-3.7-branch/include/zephyr/arch/arm64/exception.h

#ifndef ZEPHYR_INCLUDE_ARCH_ARM64_EXCEPTION_H_
#define ZEPHYR_INCLUDE_ARCH_ARM64_EXCEPTION_H_

/* for assembler, only works with constants */

#ifdef _ASMLANGUAGE
#else
#include <zephyr/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Architecture-specific exception stack frame
 *
 * This structure represents the architecture-specific exception stack frame.
 */
struct arch_esf {
	uint64_t x0;  /**< General-purpose register x0 */
	uint64_t x1;  /**< General-purpose register x1 */
	uint64_t x2;  /**< General-purpose register x2 */
	uint64_t x3;  /**< General-purpose register x3 */
	uint64_t x4;  /**< General-purpose register x4 */
	uint64_t x5;  /**< General-purpose register x5 */
	uint64_t x6;  /**< General-purpose register x6 */
	uint64_t x7;  /**< General-purpose register x7 */
	uint64_t x8;  /**< General-purpose register x8 */
	uint64_t x9;  /**< General-purpose register x9 */
	uint64_t x10; /**< General-purpose register x10 */
	uint64_t x11; /**< General-purpose register x11 */
	uint64_t x12; /**< General-purpose register x12 */
	uint64_t x13; /**< General-purpose register x13 */
	uint64_t x14; /**< General-purpose register x14 */
	uint64_t x15; /**< General-purpose register x15 */
	uint64_t x16; /**< General-purpose register x16 */
	uint64_t x17; /**< General-purpose register x17 */
	uint64_t x18; /**< General-purpose register x18 */
	uint64_t lr;  /**< Link register */
	uint64_t spsr; /**< Saved Program Status Register */
	uint64_t elr; /**< Exception Link Register */
#ifdef CONFIG_FRAME_POINTER
	uint64_t fp; /**< Frame pointer */
#endif
#ifdef CONFIG_ARM64_SAFE_EXCEPTION_STACK
	uint64_t sp; /**< Stack pointer */
#endif
} __aligned(16);

#ifdef __cplusplus
}
#endif

#endif /* _ASMLANGUAGE */

#endif /* ZEPHYR_INCLUDE_ARCH_ARM64_EXCEPTION_H_ */
//GST