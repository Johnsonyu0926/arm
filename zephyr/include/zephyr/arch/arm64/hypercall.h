/*
 * Copyright (c) 2020 Arm Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_ARCH_ARM64_HYPERCALL_H_
#define ZEPHYR_INCLUDE_ARCH_ARM64_HYPERCALL_H_

#include <zephyr/types.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HVC_CALL_NR    0

static ALWAYS_INLINE void arch_hypercall(unsigned long nr)
{
    __asm__ volatile("hvc %0" : : "r" (nr) : "memory");
}

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ARCH_ARM64_HYPERCALL_H_ */
// By GST @Date