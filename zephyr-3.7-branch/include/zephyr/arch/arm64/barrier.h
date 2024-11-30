// zephyr-3.7-branch/include/zephyr/arch/arm64/barrier.h

#ifndef ZEPHYR_INCLUDE_BARRIER_ARM64_H_
#define ZEPHYR_INCLUDE_BARRIER_ARM64_H_

#ifndef ZEPHYR_INCLUDE_SYS_BARRIER_H_
#error Please include <zephyr/sys/barrier.h>
#endif

#include <zephyr/toolchain.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Data Memory Barrier (DMB) full system fence
 *
 * This function issues a Data Memory Barrier (DMB) instruction with full system scope.
 */
static ALWAYS_INLINE void z_barrier_dmem_fence_full(void)
{
	__asm__ volatile ("dmb sy" ::: "memory");
}

/**
 * @brief Data Synchronization Barrier (DSB) full system fence
 *
 * This function issues a Data Synchronization Barrier (DSB) instruction with full system scope.
 */
static ALWAYS_INLINE void z_barrier_dsync_fence_full(void)
{
	__asm__ volatile ("dsb sy" ::: "memory");
}

/**
 * @brief Instruction Synchronization Barrier (ISB) full system fence
 *
 * This function issues an Instruction Synchronization Barrier (ISB) instruction with full system scope.
 */
static ALWAYS_INLINE void z_barrier_isync_fence_full(void)
{
	__asm__ volatile ("isb" ::: "memory");
}

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_BARRIER_ARM64_H_ */
//GST