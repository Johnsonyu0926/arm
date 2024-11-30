//kernel/timeslicing.c
#include <zephyr/kernel.h>
#include <kswap.h>
#include <ksched.h>
#include <ipi.h>

/* Time slice duration in ticks */
static int slice_ticks = DIV_ROUND_UP(CONFIG_TIMESLICE_SIZE * Z_HZ_ticks, Z_HZ_ms);
/* Maximum priority for time slicing */
static int slice_max_prio = CONFIG_TIMESLICE_PRIORITY;
/* Timeouts for each CPU */
static struct _timeout slice_timeouts[CONFIG_MP_MAX_NUM_CPUS];
/* Flags indicating if the time slice has expired for each CPU */
static bool slice_expired[CONFIG_MP_MAX_NUM_CPUS];

#ifdef CONFIG_SWAP_NONATOMIC
/* If z_swap() isn't atomic, then it's possible for a timer interrupt
 * to try to timeslice away _current after it has already pended
 * itself but before the corresponding context switch. Treat that as
 * a noop condition in z_time_slice().
 */
struct k_thread *pending_current;
#endif

/**
 * @brief Get the time slice duration for a thread
 *
 * This function returns the time slice duration for the specified thread.
 *
 * @param thread Pointer to the thread
 * @return Time slice duration in ticks
 */
static inline int slice_time(struct k_thread *thread)
{
	int ret = slice_ticks;

#ifdef CONFIG_TIMESLICE_PER_THREAD
	if (thread->base.slice_ticks != 0) {
		ret = thread->base.slice_ticks;
	}
#else
	ARG_UNUSED(thread);
#endif
	return ret;
}

/**
 * @brief Check if a thread is sliceable
 *
 * This function checks if the specified thread is eligible for time slicing.
 *
 * @param thread Pointer to the thread
 * @return true if the thread is sliceable, false otherwise
 */
bool thread_is_sliceable(struct k_thread *thread)
{
	bool ret = thread_is_preemptible(thread)
		&& slice_time(thread) != 0
		&& !z_is_prio_higher(thread->base.prio, slice_max_prio)
		&& !z_is_thread_prevented_from_running(thread)
		&& !z_is_idle_thread_object(thread);

#ifdef CONFIG_TIMESLICE_PER_THREAD
	ret |= thread->base.slice_ticks != 0;
#endif

	return ret;
}

/**
 * @brief Timeout handler for time slicing
 *
 * This function is called when the time slice duration expires.
 *
 * @param timeout Pointer to the timeout structure
 */
static void slice_timeout(struct _timeout *timeout)
{
	int cpu = ARRAY_INDEX(slice_timeouts, timeout);

	slice_expired[cpu] = true;

	/* We need an IPI if we just handled a timeslice expiration
	 * for a different CPU.
	 */
	if (cpu != _current_cpu->id) {
		flag_ipi(IPI_CPU_MASK(cpu));
	}
}

/**
 * @brief Reset the time slice for a thread
 *
 * This function resets the time slice for the specified thread.
 *
 * @param thread Pointer to the thread
 */
void z_reset_time_slice(struct k_thread *thread)
{
	int cpu = _current_cpu->id;

	z_abort_timeout(&slice_timeouts[cpu]);
	slice_expired[cpu] = false;
	if (thread_is_sliceable(thread)) {
		z_add_timeout(&slice_timeouts[cpu], slice_timeout,
			      K_TICKS(slice_time(thread) - 1));
	}
}

/**
 * @brief Set the global time slice duration and priority
 *
 * This function sets the global time slice duration and priority.
 *
 * @param slice Time slice duration in milliseconds
 * @param prio Maximum priority for time slicing
 */
void k_sched_time_slice_set(int32_t slice, int prio)
{
	K_SPINLOCK(&_sched_spinlock) {
		slice_ticks = k_ms_to_ticks_ceil32(slice);
		slice_max_prio = prio;
		z_reset_time_slice(_current);
	}
}

#ifdef CONFIG_TIMESLICE_PER_THREAD
/**
 * @brief Set the time slice duration for a specific thread
 *
 * This function sets the time slice duration for the specified thread.
 *
 * @param thread Pointer to the thread
 * @param thread_slice_ticks Time slice duration in ticks
 * @param expired Callback function to call when the time slice expires
 * @param data Data to pass to the callback function
 */
void k_thread_time_slice_set(struct k_thread *thread, int32_t thread_slice_ticks,
			     k_thread_timeslice_fn_t expired, void *data)
{
	K_SPINLOCK(&_sched_spinlock) {
		thread->base.slice_ticks = thread_slice_ticks;
		thread->base.slice_expired = expired;
		thread->base.slice_data = data;
		z_reset_time_slice(thread);
	}
}
#endif

/**
 * @brief Handle time slice expiration
 *
 * This function is called from each timer interrupt to handle time slice
 * expiration.
 */
void z_time_slice(void)
{
	k_spinlock_key_t key = k_spin_lock(&_sched_spinlock);
	struct k_thread *curr = _current;

#ifdef CONFIG_SWAP_NONATOMIC
	if (pending_current == curr) {
		z_reset_time_slice(curr);
		k_spin_unlock(&_sched_spinlock, key);
		return;
	}
	pending_current = NULL;
#endif

	if (slice_expired[_current_cpu->id] && thread_is_sliceable(curr)) {
#ifdef CONFIG_TIMESLICE_PER_THREAD
		if (curr->base.slice_expired) {
			k_spin_unlock(&_sched_spinlock, key);
			curr->base.slice_expired(curr, curr->base.slice_data);
			key = k_spin_lock(&_sched_spinlock);
		}
#endif
		if (!z_is_thread_prevented_from_running(curr)) {
			move_thread_to_end_of_prio_q(curr);
		}
		z_reset_time_slice(curr);
	}
	k_spin_unlock(&_sched_spinlock, key);
}
//GST