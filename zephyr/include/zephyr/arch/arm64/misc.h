/*
 * Copyright (c) 2019 Carlo Caione <ccaione@baylibre.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_ARCH_ARM64_MISC_H_
#define ZEPHYR_INCLUDE_ARCH_ARM64_MISC_H_

#include <zephyr/types.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ARM64_ERR_NONE        0
#define ARM64_ERR_UNKNOWN    -1
#define ARM64_ERR_UNSUPPORTED    -2
#define ARM64_ERR_INVALID    -3
#define ARM64_ERR_NO_MEMORY    -4

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ARCH_ARM64_MISC_H_ */
// By GST @Date