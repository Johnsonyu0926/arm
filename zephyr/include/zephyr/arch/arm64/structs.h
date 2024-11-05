/*
 * Copyright (c) 2019 Carlo Caione <ccaione@baylibre.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_ARCH_ARM64_STRUCTS_H_
#define ZEPHYR_INCLUDE_ARCH_ARM64_STRUCTS_H_

#include <zephyr/types.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct arm64_cpu {
    uint64_t mpidr;
    uint64_t elr_el1;
    uint64_t spsr_el1;
    uint64_t sp_el0;
    uint64_t sp_el1;
    uint64_t tpidr_el0;
    uint64_t tpidrro_el0;
    uint64_t tpidr_el1;
};

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ARCH_ARM64_STRUCTS_H_ */
// By GST @Date