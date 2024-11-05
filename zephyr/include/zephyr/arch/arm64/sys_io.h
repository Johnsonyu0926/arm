/*
 * Copyright (c) 2019 Carlo Caione <ccaione@baylibre.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_ARCH_ARM64_SYS_IO_H_
#define ZEPHYR_INCLUDE_ARCH_ARM64_SYS_IO_H_

#include <zephyr/types.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

static ALWAYS_INLINE void sys_write32(uint32_t data, mm_reg_t addr)
{
    __asm__ volatile("str %w0, [%1]" : : "r" (data), "r" (addr) : "memory");
}

static ALWAYS_INLINE uint32_t sys_read32(mm_reg_t addr)
{
    uint32_t data;

    __asm__ volatile("ldr %w0, [%1]" : "=r" (data) : "r" (addr) : "memory");

    return data;
}

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ARCH_ARM64_SYS_IO_H_ */
// By GST @Date