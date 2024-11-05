/*
 * Copyright (c) 2020 Arm Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_ARCH_ARM64_CORTEX_R_ARM_MPU_H_
#define ZEPHYR_INCLUDE_ARCH_ARM64_CORTEX_R_ARM_MPU_H_

#include <zephyr/types.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ARM_MPU_REGION_SIZE_32B    0x04U
#define ARM_MPU_REGION_SIZE_64B    0x05U
#define ARM_MPU_REGION_SIZE_128B   0x06U
#define ARM_MPU_REGION_SIZE_256B   0x07U
#define ARM_MPU_REGION_SIZE_512B   0x08U
#define ARM_MPU_REGION_SIZE_1KB    0x09U
#define ARM_MPU_REGION_SIZE_2KB    0x0AU
#define ARM_MPU_REGION_SIZE_4KB    0x0BU
#define ARM_MPU_REGION_SIZE_8KB    0x0CU
#define ARM_MPU_REGION_SIZE_16KB   0x0DU
#define ARM_MPU_REGION_SIZE_32KB   0x0EU
#define ARM_MPU_REGION_SIZE_64KB   0x0FU
#define ARM_MPU_REGION_SIZE_128KB  0x10U
#define ARM_MPU_REGION_SIZE_256KB  0x11U
#define ARM_MPU_REGION_SIZE_512KB  0x12U
#define ARM_MPU_REGION_SIZE_1MB    0x13U
#define ARM_MPU_REGION_SIZE_2MB    0x14U
#define ARM_MPU_REGION_SIZE_4MB    0x15U
#define ARM_MPU_REGION_SIZE_8MB    0x16U
#define ARM_MPU_REGION_SIZE_16MB   0x17U
#define ARM_MPU_REGION_SIZE_32MB   0x18U
#define ARM_MPU_REGION_SIZE_64MB   0x19U
#define ARM_MPU_REGION_SIZE_128MB  0x1AU
#define ARM_MPU_REGION_SIZE_256MB  0x1BU
#define ARM_MPU_REGION_SIZE_512MB  0x1CU
#define ARM_MPU_REGION_SIZE_1GB    0x1DU
#define ARM_MPU_REGION_SIZE_2GB    0x1EU
#define ARM_MPU_REGION_SIZE_4GB    0x1FU

#define ARM_MPU_REGION_SIZE(size)  ((size) << 1U)

struct arm_mpu_region {
    uint32_t base;
    uint32_t size;
    uint32_t attr;
};

void arm_mpu_configure(const struct arm_mpu_region *regions, uint32_t region_count);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ARCH_ARM64_CORTEX_R_ARM_MPU_H_ */
// By GST @Date