// zephyr-3.7-branch/include/zephyr/arch/arm64/mm.h

#ifndef ZEPHYR_INCLUDE_ARCH_ARM64_MM_H_
#define ZEPHYR_INCLUDE_ARCH_ARM64_MM_H_

#if defined(CONFIG_ARM_MMU)
#include <zephyr/arch/arm64/arm_mmu.h>
/*
 * When MMU is enabled, some section addresses need to be aligned with
 * page size which is CONFIG_MMU_PAGE_SIZE
 */
#define MEM_DOMAIN_ALIGN_AND_SIZE CONFIG_MMU_PAGE_SIZE
#elif defined(CONFIG_ARM_MPU)
#include <zephyr/arch/arm64/cortex_r/arm_mpu.h>
/*
 * When MPU is enabled, some section addresses need to be aligned with
 * MPU region minimum alignment size which is
 * CONFIG_ARM_MPU_REGION_MIN_ALIGN_AND_SIZE
 */
#define MEM_DOMAIN_ALIGN_AND_SIZE CONFIG_ARM_MPU_REGION_MIN_ALIGN_AND_SIZE
#endif

#ifndef _ASMLANGUAGE

/**
 * @brief Initialize memory domains for a thread
 *
 * This function initializes the memory domains for the specified thread.
 *
 * @param thread The thread to initialize memory domains for
 */
void z_arm64_thread_mem_domains_init(struct k_thread *thread);

/**
 * @brief Swap memory domains for a thread
 *
 * This function swaps the memory domains for the specified thread.
 *
 * @param thread The thread to swap memory domains for
 */
void z_arm64_swap_mem_domains(struct k_thread *thread);

#endif /* _ASMLANGUAGE */

#endif /* ZEPHYR_INCLUDE_ARCH_ARM64_MM_H_ */
//GST