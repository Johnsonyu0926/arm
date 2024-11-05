/*
 * Copyright (c) 2019 Carlo Caione <ccaione@baylibre.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_ARCH_ARM64_CPU_H_
#define ZEPHYR_INCLUDE_ARCH_ARM64_CPU_H_

#include <zephyr/types.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DAIFSET_IRQ_BIT    (1 << 7)
#define DAIFSET_FIQ_BIT    (1 << 6)
#define DAIFSET_ABT_BIT    (1 << 5)
#define DAIFSET_DBG_BIT    (1 << 4)

#define DAIFSET_IRQ_MASK    (DAIFSET_IRQ_BIT | DAIFSET_FIQ_BIT | DAIFSET_ABT_BIT | DAIFSET_DBG_BIT)

static ALWAYS_INLINE void arch_irq_enable(unsigned int irq)
{
    __asm__ volatile("msr daifclr, %0" : : "i" (DAIFSET_IRQ_BIT) : "memory");
}

static ALWAYS_INLINE void arch_irq_disable(unsigned int irq)
{
    __asm__ volatile("msr daifset, %0" : : "i" (DAIFSET_IRQ_BIT) : "memory");
}

static ALWAYS_INLINE bool arch_irq_is_enabled(unsigned int irq)
{
    uint32_t daif;

    __asm__ volatile("mrs %0, daif" : "=r" (daif));

    return !(daif & DAIFSET_IRQ_BIT);
}

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ARCH_ARM64_CPU_H_ */
// By GST @Date