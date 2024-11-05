/*
 * Copyright (c) 2019 Carlo Caione <ccaione@baylibre.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_ARCH_ARM64_TPIDRRO_EL0_H_
#define ZEPHYR_INCLUDE_ARCH_ARM64_TPIDRRO_EL0_H_

#include <zephyr/types.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

static ALWAYS_INLINE uint64_t read_tpidrro_el0(void)
{
    uint64_t val;

    __asm__ volatile("mrs %0, tpidrro_el0" : "=r" (val));

    return val;
}

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ARCH_ARM64_TPIDRRO_EL0_H_ */
// By GST @Date