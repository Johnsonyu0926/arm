/*
 * Copyright (c) 2019 Carlo Caione <ccaione@baylibre.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_ARCH_ARM64_SYSCALL_H_
#define ZEPHYR_INCLUDE_ARCH_ARM64_SYSCALL_H_

#include <zephyr/types.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ARM64_SYSCALL_NR    0

static ALWAYS_INLINE void arch_syscall(unsigned long nr)
{
    __asm__ volatile("svc %0" : : "r" (nr) : "memory");
}

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ARCH_ARM64_SYSCALL_H_ */
// By GST @Date