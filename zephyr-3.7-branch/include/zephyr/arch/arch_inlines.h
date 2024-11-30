//zephyr-3.7-branch/include/zephyr/arch/arch_inlines.h

#ifndef ZEPHYR_INCLUDE_ARCH_INLINES_H_
#define ZEPHYR_INCLUDE_ARCH_INLINES_H_

#if defined(CONFIG_X86)
#include <zephyr/arch/x86/arch_inlines.h>
#elif defined(CONFIG_ARM)
#include <zephyr/arch/arm/arch_inlines.h>
#elif defined(CONFIG_ARM64)
#include <zephyr/arch/arm64/arch_inlines.h>
#elif defined(CONFIG_ARC)
#include <zephyr/arch/arc/arch_inlines.h>
#elif defined(CONFIG_XTENSA)
#include <zephyr/arch/xtensa/arch_inlines.h>
#elif defined(CONFIG_RISCV)
#include <zephyr/arch/riscv/arch_inlines.h>
#elif defined(CONFIG_NIOS2)
#include <zephyr/arch/nios2/arch_inlines.h>
#elif defined(CONFIG_MIPS)
#include <zephyr/arch/mips/arch_inlines.h>
#elif defined(CONFIG_ARCH_POSIX)
#include <zephyr/arch/posix/arch_inlines.h>
#elif defined(CONFIG_SPARC)
#include <zephyr/arch/sparc/arch_inlines.h>
#endif

#endif /* ZEPHYR_INCLUDE_ARCH_INLINES_H_ */

//GST
