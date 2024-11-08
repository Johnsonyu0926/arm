// Filename: z_arm64_paging.h
/*
 * Copyright (c) 2024 BayLibre SAS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef Z_ARM64_PAGING_H
#define Z_ARM64_PAGING_H

#include <stdint.h>
#include <stdbool.h>

struct arch_esf;

bool z_arm64_do_demand_paging(struct arch_esf *esf, uint64_t esr, uint64_t far);

#endif /* Z_ARM64_PAGING_H */

// By GST @Date