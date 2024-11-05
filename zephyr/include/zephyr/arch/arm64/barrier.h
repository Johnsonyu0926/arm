/**
 * Copyright (c) 2023 Carlo Caione <ccaione@baylibre.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef ZEPHYR_INCLUDE_BARRIER_ARM64_H_
#define ZEPHYR_INCLUDE_BARRIER_ARM64_H_

#ifndef ZEPHYR_INCLUDE_SYS_BARRIER_H_
#error Please include <zephyr/sys/barrier.h> instead of this file directly
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Memory barrier functions for ARM64 architecture */

static ALWAYS_INLINE void sys_barrier_dsync(void)
{
    __asm__ volatile("dsb sy" : : : "memory");
}

static ALWAYS_INLINE void sys_barrier_isync(void)
{
    __asm__ volatile("isb" : : : "memory");
}

static ALWAYS_INLINE void sys_barrier_dmb(void)
{
    __asm__ volatile("dmb sy" : : : "memory");
}

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_BARRIER_ARM64_H_ */
// By GST @Date