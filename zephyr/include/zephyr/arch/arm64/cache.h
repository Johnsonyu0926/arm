/*
 * Copyright (c) 2019 Carlo Caione <ccaione@baylibre.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_ARCH_ARM64_CACHE_H_
#define ZEPHYR_INCLUDE_ARCH_ARM64_CACHE_H_

#include <zephyr/types.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define L1_CACHE_BYTES 64

static ALWAYS_INLINE void arch_dcache_flush(void *addr, size_t size)
{
    uintptr_t start = (uintptr_t)addr & ~(L1_CACHE_BYTES - 1);
    uintptr_t end = (uintptr_t)addr + size;

    while (start < end) {
        __asm__ volatile("dc cvac, %0" : : "r" (start) : "memory");
        start += L1_CACHE_BYTES;
    }

    __asm__ volatile("dsb sy" : : : "memory");
}

static ALWAYS_INLINE void arch_dcache_invd(void *addr, size_t size)
{
    uintptr_t start = (uintptr_t)addr & ~(L1_CACHE_BYTES - 1);
    uintptr_t end = (uintptr_t)addr + size;

    while (start < end) {
        __asm__ volatile("dc ivac, %0" : : "r" (start) : "memory");
        start += L1_CACHE_BYTES;
    }

    __asm__ volatile("dsb sy" : : : "memory");
}

static ALWAYS_INLINE void arch_dcache_flush_all(void)
{
    __asm__ volatile("dc cisw, xzr" : : : "memory");
    __asm__ volatile("dsb sy" : : : "memory");
}

static ALWAYS_INLINE void arch_dcache_invd_all(void)
{
    __asm__ volatile("dc isw, xzr" : : : "memory");
    __asm__ volatile("dsb sy" : : : "memory");
}

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ARCH_ARM64_CACHE_H_ */
// By GST @Date