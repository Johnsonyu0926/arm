// zephyr-3.7-branch/include/zephyr/arch/syscall.h

#ifndef ZEPHYR_INCLUDE_ARCH_SYSCALL_H_
#define ZEPHYR_INCLUDE_ARCH_SYSCALL_H_

#if defined(CONFIG_X86_64)
#include <zephyr/arch/x86/intel64/syscall.h>
#elif defined(CONFIG_X86)
#include <zephyr/arch/x86/ia32/syscall.h>
#elif defined(CONFIG_ARM64)
#include <zephyr/arch/arm64/syscall.h>
#elif defined(CONFIG_ARM)
#include <zephyr/arch/arm/syscall.h>
#elif defined(CONFIG_ARC)
#include <zephyr/arch/arc/syscall.h>
#elif defined(CONFIG_RISCV)
#include <zephyr/arch/riscv/syscall.h>
#elif defined(CONFIG_XTENSA)
#include <zephyr/arch/xtensa/syscall.h>
#endif

#endif /* ZEPHYR_INCLUDE_ARCH_SYSCALL_H_ */
//GST