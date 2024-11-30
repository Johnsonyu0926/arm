// zephyr-3.7-branch/include/zephyr/kernel/sTATS.h

#ifndef ZEPHYR_INCLUDE_KERNEL_SMP_H_
#define ZEPHYR_INCLUDE_KERNEL_SMP_H_

#include <stdbool.h>

typedef void (*smp_init_fn)(void *arg);

/**
 * @brief Start a CPU.
 *
 * This routine is used to manually start the CPU specified
 * by @a id. It may be called to restart a CPU that had been
 * stopped or powered down, as well as some other scenario.
 * After the CPU has finished initialization, the CPU will be
 * ready to participate in thread scheduling and execution.
 *
 * @note This function must not be used on currently running
 *       CPU. The target CPU must be in off state, or in
 *       certain architectural state(s) where the CPU is
 *       permitted to go through the power up process.
 *       Detection of such state(s) must be provided by
 *       the platform layers.
 *
 * @note This initializes per-CPU kernel structs and also
 *       initializes timers needed for MP operations.
 *       Use @ref k_smp_cpu_resume if these are not
 *       desired.
 *
 * @param id ID of target CPU.
 * @param fn Function to be called before letting scheduler
 *           run.
 * @param arg Argument to @a fn.
 */
void k_smp_cpu_start(int id, smp_init_fn fn, void *arg);

/**
 * @brief Resume a previously suspended CPU.
 *
 * This function works like @ref k_smp_cpu_start, but does not
 * re-initialize the kernel's internal tracking data for
 * the target CPU. Therefore, @ref k_smp_cpu_start must have
 * previously been called for the target CPU, and it must have
 * verifiably reached an idle/off state (detection of which
 * must be provided by the platform layers). It may be used
 * in cases where platform layers require, for example, that
 * data on the interrupt or idle stack be preserved.
 *
 * @note This function must not be used on currently running
 *       CPU. The target CPU must be in suspended state, or
 *       in certain architectural state(s) where the CPU is
 *       permitted to go through the resume process.
 *       Detection of such state(s) must be provided by
 *       the platform layers.
 *
 * @param id ID of target CPU.
 * @param fn Function to be called before resuming context.
 * @param arg Argument to @a fn.
 * @param reinit_timer True if timer needs to be re-initialized.
 * @param invoke_sched True if scheduler is invoked after the CPU
 *                     has started.
 */
void k_smp_cpu_resume(int id, smp_init_fn fn, void *arg,
		      bool reinit_timer, bool invoke_sched);

#endif /* ZEPHYR_INCLUDE_KERNEL_SMP_H_ */

// zephyr-3.7-branch/include/zephyr/kernel/stats.h

#ifndef ZEPHYR_INCLUDE_KERNEL_STATS_H_
#define ZEPHYR_INCLUDE_KERNEL_STATS_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Structure used to track internal statistics about both thread
 * and CPU usage.
 */
struct k_cycle_stats {
	uint64_t  total;        /**< Total usage in cycles */
#if defined(CONFIG_SCHED_THREAD_USAGE_ANALYSIS) || defined(__DOXYGEN__)
	/**
	 * @name Fields available when CONFIG_SCHED_THREAD_USAGE_ANALYSIS is selected.
	 * @{
	 */
	uint64_t  current;      /**< Number of cycles in current usage window */
	uint64_t  longest;      /**< Number of cycles in longest usage window */
	uint32_t  num_windows;  /**< Number of usage windows */
	/** @} */
#endif /* CONFIG_SCHED_THREAD_USAGE_ANALYSIS */
	bool      track_usage;  /**< True if gathering usage stats */
};

#endif /* ZEPHYR_INCLUDE_KERNEL_STATS_H_ */
//GST