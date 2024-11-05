/*
 * Copyright (c) 2019 Carlo Caione <ccaione@baylibre.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_ARCH_ARM64_EXCEPTION_H_
#define ZEPHYR_INCLUDE_ARCH_ARM64_EXCEPTION_H_

#include <zephyr/types.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ARM64_EXC_SYNC_SP0    0
#define ARM64_EXC_IRQ_SP0    1
#define ARM64_EXC_FIQ_SP0    2
#define ARM64_EXC_SERR_SP0    3
#define ARM64_EXC_SYNC_SPX    4
#define ARM64_EXC_IRQ_SPX    5
#define ARM64_EXC_FIQ_SPX    6
#define ARM64_EXC_SERR_SPX    7
#define ARM64_EXC_SYNC_EL0    8
#define ARM64_EXC_IRQ_EL0    9
#define ARM64_EXC_FIQ_EL0    10
#define ARM64_EXC_SERR_EL0    11
#define ARM64_EXC_SYNC_ELX    12
#define ARM64_EXC_IRQ_ELX    13
#define ARM64_EXC_FIQ_ELX    14
#define ARM64_EXC_SERR_ELX    15

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ARCH_ARM64_EXCEPTION_H_ */
// By GST @Date