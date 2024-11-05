/*
 * syscall.h - automatically selects the correct syscall.h file to include
 *   based on the architecture being built.
 */

/*
 * Copyright (c) 2010-2011, 2013-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_ARCH_SYSCALL_H_
#define ZEPHYR_INCLUDE_ARCH_SYSCALL_H_

#if defined(CONFIG_X86)
#include <zephyr/arch/x86/syscall.h>
#elif defined(CONFIG_ARM)
#include <zephyr/arch/arm/aarch32/syscall.h>
#elif defined(CONFIG_ARC)
#include <zephyr/arch/arc/syscall.h>
#elif defined(CONFIG_NIOS2)
#include <zephyr/arch/nios2/syscall.h>
#elif defined(CONFIG_RISCV)
#include <zephyr/arch/riscv/syscall.h>
#elif defined(CONFIG_XTENSA)
#include <zephyr/arch/xtensa/syscall.h>
#elif defined(CONFIG_SPARC)
#include <zephyr/arch/sparc/syscall.h>
#elif defined(CONFIG_MIPS)
#include <zephyr/arch/mips/syscall.h>
#elif defined(CONFIG_ARCH_POSIX)
#include <zephyr/arch/posix/syscall.h>
#elif defined(CONFIG_ARCH_ISA_ARM)
#include <zephyr/arch/arm/aarch64/syscall.h>
#else
#error "Unknown Architecture"
#endif

#endif /* ZEPHYR_INCLUDE_ARCH_SYSCALL_H_ */
// By GST @Date