/*
 * Copyright (c) 2019 Carlo Caione <ccaione@baylibre.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_ARCH_ARM64_TIMER_H_
#define ZEPHYR_INCLUDE_ARCH_ARM64_TIMER_H_

#include <zephyr/types.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ARM64_TIMER_IRQ    30

static ALWAYS_INLINE void arch_timer_enable(void)
{
    __asm__ volatile("msr cntp_ctl_el0, %0" : : "r" (1) : "memory");
}

static ALWAYS_INLINE void arch_timer_disable(void)
{
    __asm__ volatile("msr cntp_ctl_el0, %0" : : "r" (0) : "memory");
}

static ALWAYS_INLINE void arch_timer_set_compare(uint64_t compare)
{
    __asm__ volatile("msr cntp_cval_el0, %0" : : "r" (compare) : "memory");
}

static ALWAYS_INLINE uint64_t arch_timer_get_count(void)
{
    uint64_t count;

    __asm__ volatile("mrs %0, cntvct_el0" : "=r" (count));

    return count;
}

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ARCH_ARM64_TIMER_H_ */
// By GST @Date