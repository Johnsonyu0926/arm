/*
 * Copyright (c) 2019 Carlo Caione <ccaione@baylibre.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_ARCH_ARM64_LIB_HELPERS_H_
#define ZEPHYR_INCLUDE_ARCH_ARM64_LIB_HELPERS_H_

#include <zephyr/types.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

static ALWAYS_INLINE uint64_t read_mpidr_el1(void)
{
    uint64_t val;

    __asm__ volatile("mrs %0, mpidr_el1" : "=r" (val));

    return val;
}

static ALWAYS_INLINE uint64_t read_daif(void)
{
    uint64_t val;

    __asm__ volatile("mrs %0, daif" : "=r" (val));

    return val;
}

static ALWAYS_INLINE void write_daif(uint64_t val)
{
    __asm__ volatile("msr daif, %0" : : "r" (val) : "memory");
}

static ALWAYS_INLINE void disable_irq(void)
{
    __asm__ volatile("msr daifset, %0" : : "i" (DAIFSET_IRQ_BIT) : "memory");
}

static ALWAYS_INLINE void enable_irq(void)
{
    __asm__ volatile("msr daifclr, %0" : : "i" (DAIFSET_IRQ_BIT) : "memory");
}

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ARCH_ARM64_LIB_HELPERS_H_ */
// By GST @Date