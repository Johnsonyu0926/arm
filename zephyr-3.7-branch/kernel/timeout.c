//kernel/timeout.c
#include <zephyr/kernel.h>
#include <zephyr/spinlock.h>
#include <ksched.h>
#include <timeout_q.h>
#include <zephyr/internal/syscall_handler.h>
#include <zephyr/drivers/timer/system_timer.h>
#include <zephyr/sys_clock.h>

static uint64_t curr_tick;

static sys_dlist_t timeout_list = SYS_DLIST_STATIC_INIT(&timeout_list);

static struct k_spinlock timeout_lock;

#define MAX_WAIT (IS_ENABLED(CONFIG_SYSTEM_CLOCK_SLOPPY_IDLE) \
		  ? K_TICKS_FOREVER : INT_MAX)

/* Ticks left to process in the currently-executing sys_clock_announce() */
static int announce_remaining;

#if defined(CONFIG_TIMER_READS_ITS_FREQUENCY_AT_RUNTIME)
int z_clock_hw_cycles_per_sec = CONFIG_SYS_CLOCK_HW_CYCLES_PER_SEC;

#ifdef CONFIG_USERSPACE
static inline int z_vrfy_sys_clock_hw_cycles_per_sec_runtime_get(void)
{
	return z_impl_sys_clock_hw_cycles_per_sec_runtime_get();
}
#include <zephyr/syscalls/sys_clock_hw_cycles_per_sec_runtime_get_mrsh.c>
#endif /* CONFIG_USERSPACE */
#endif /* CONFIG_TIMER_READS_ITS_FREQUENCY_AT_RUNTIME */

/**
 * @brief Get the first timeout in the list
 *
 * @return Pointer to the first timeout, or NULL if the list is empty
 */
static struct _timeout *first(void)
{
	sys_dnode_t *t = sys_dlist_peek_head(&timeout_list);

	return (t == NULL) ? NULL : CONTAINER_OF(t, struct _timeout, node);
}

/**
 * @brief Get the next timeout in the list
 *
 * @param t Pointer to the current timeout
 * @return Pointer to the next timeout, or NULL if there is no next timeout
 */
static struct _timeout *next(struct _timeout *t)
{
	sys_dnode_t *n = sys_dlist_peek_next(&timeout_list, &t->node);

	return (n == NULL) ? NULL : CONTAINER_OF(n, struct _timeout, node);
}

/**
 * @brief Remove a timeout from the list
 *
 * @param t Pointer to the timeout to be removed
 */
static void remove_timeout(struct _timeout *t)
{
	if (next(t) != NULL) {
		next(t)->dticks += t->dticks;
	}

	sys_dlist_remove(&t->node);
}

/**
 * @brief Get the elapsed ticks
 *
 * @return Number of elapsed ticks
 */
static int32_t elapsed(void)
{
	return announce_remaining == 0 ? sys_clock_elapsed() : 0U;
}

/**
 * @brief Get the next timeout value
 *
 * @return Number of ticks until the next timeout
 */
static int32_t next_timeout(void)
{
	struct _timeout *to = first();
	int32_t ticks_elapsed = elapsed();
	int32_t ret;

	if ((to == NULL) ||
	    ((int64_t)(to->dticks - ticks_elapsed) > (int64_t)INT_MAX)) {
		ret = MAX_WAIT;
	} else {
		ret = MAX(0, to->dticks - ticks_elapsed);
	}

	return ret;
}

/**
 * @brief Add a timeout to the list
 *
 * @param to Pointer to the timeout
 * @param fn Timeout callback function
 * @param timeout Timeout value
 */
void z_add_timeout(struct _timeout *to, _timeout_func_t fn,
		   k_timeout_t timeout)
{
	if (K_TIMEOUT_EQ(timeout, K_FOREVER)) {
		return;
	}

#ifdef CONFIG_KERNEL_COHERENCE
	__ASSERT_NO_MSG(arch_mem_coherent(to));
#endif /* CONFIG_KERNEL_COHERENCE */

	__ASSERT(!sys_dnode_is_linked(&to->node), "");
	to->fn = fn;

	K_SPINLOCK(&timeout_lock) {
		struct _timeout *t;

		if (IS_ENABLED(CONFIG_TIMEOUT_64BIT) &&
		    (Z_TICK_ABS(timeout.ticks) >= 0)) {
			k_ticks_t ticks = Z_TICK_ABS(timeout.ticks) - curr_tick;

			to->dticks = MAX(1, ticks);
		} else {
			to->dticks = timeout.ticks + 1 + elapsed();
		}

		for (t = first(); t != NULL; t = next(t)) {
			if (t->dticks > to->dticks) {
				t->dticks -= to->dticks;
				sys_dlist_insert(&t->node, &to->node);
				break;
			}
			to->dticks -= t->dticks;
		}

		if (t == NULL) {
			sys_dlist_append(&timeout_list, &to->node);
		}

		if (to == first() && announce_remaining == 0) {
			sys_clock_set_timeout(next_timeout(), false);
		}
	}
}

/**
 * @brief Abort a timeout
 *
 * @param to Pointer to the timeout to be aborted
 * @return 0 on success, or -EINVAL if the timeout is not found
 */
int z_abort_timeout(struct _timeout *to)
{
	int ret = -EINVAL;

	K_SPINLOCK(&timeout_lock) {
		if (sys_dnode_is_linked(&to->node)) {
			remove_timeout(to);
			ret = 0;
		}
	}

	return ret;
}

/**
 * @brief Get the remaining ticks for a timeout
 *
 * @param timeout Pointer to the timeout
 * @return Number of remaining ticks
 */
static k_ticks_t timeout_rem(const struct _timeout *timeout)
{
	k_ticks_t ticks = 0;

	for (struct _timeout *t = first(); t != NULL; t = next(t)) {
		ticks += t->dticks;
		if (timeout == t) {
			break;
		}
	}

	return ticks;
}

/**
 * @brief Get the remaining ticks for a timeout
 *
 * @param timeout Pointer to the timeout
 * @return Number of remaining ticks
 */
k_ticks_t z_timeout_remaining(const struct _timeout *timeout)
{
	k_ticks_t ticks = 0;

	K_SPINLOCK(&timeout_lock) {
		if (!z_is_inactive_timeout(timeout)) {
			ticks = timeout_rem(timeout) - elapsed();
		}
	}

	return ticks;
}

/**
 * @brief Get the expiration tick for a timeout
 *
 * @param timeout Pointer to the timeout
 * @return Expiration tick
 */
k_ticks_t z_timeout_expires(const struct _timeout *timeout)
{
	k_ticks_t ticks = 0;

	K_SPINLOCK(&timeout_lock) {
		ticks = curr_tick;
		if (!z_is_inactive_timeout(timeout)) {
			ticks += timeout_rem(timeout);
		}
	}

	return ticks;
}

/**
 * @brief Get the next timeout expiry
 *
 * @return Number of ticks until the next timeout expiry
 */
int32_t z_get_next_timeout_expiry(void)
{
	int32_t ret = (int32_t) K_TICKS_FOREVER;

	K_SPINLOCK(&timeout_lock) {
		ret = next_timeout();
	}
	return ret;
}

/**
 * @brief Announce the system clock
 *
 * @param ticks Number of ticks to announce
 */
void sys_clock_announce(int32_t ticks)
{
	k_spinlock_key_t key = k_spin_lock(&timeout_lock);

	if (IS_ENABLED(CONFIG_SMP) && (announce_remaining != 0)) {
		announce_remaining += ticks;
		k_spin_unlock(&timeout_lock, key);
		return;
	}

	announce_remaining = ticks;

	struct _timeout *t;

	for (t = first();
	     (t != NULL) && (t->dticks <= announce_remaining);
	     t = first()) {
		int dt = t->dticks;

		curr_tick += dt;
		t->dticks = 0;
		remove_timeout(t);

		k_spin_unlock(&timeout_lock, key);
		t->fn(t);
		key = k_spin_lock(&timeout_lock);
		announce_remaining -= dt;
	}

	if (t != NULL) {
		t->dticks -= announce_remaining;
	}

	curr_tick += announce_remaining;
	announce_remaining = 0;

	sys_clock_set_timeout(next_timeout(), false);

	k_spin_unlock(&timeout_lock, key);

#ifdef CONFIG_TIMESLICING
	z_time_slice();
#endif /* CONFIG_TIMESLICING */
}

/**
 * @brief Get the current system tick
 *
 * @return Current system tick
 */
int64_t sys_clock_tick_get(void)
{
	uint64_t t = 0U;

	K_SPINLOCK(&timeout_lock) {
		t = curr_tick + elapsed();
	}
	return t;
}

/**
 * @brief Get the current system tick (32-bit)
 *
 * @return Current system tick (32-bit)
 */
uint32_t sys_clock_tick_get_32(void)
{
#ifdef CONFIG_TICKLESS_KERNEL
	return (uint32_t)sys_clock_tick_get();
#else
	return (uint32_t)curr_tick;
#endif /* CONFIG_TICKLESS_KERNEL */
}

/**
 * @brief Get the current uptime in ticks
 *
 * @return Current uptime in ticks
 */
int64_t z_impl_k_uptime_ticks(void)
{
	return sys_clock_tick_get();
}

#ifdef CONFIG_USERSPACE
static inline int64_t z_vrfy_k_uptime_ticks(void)
{
	return z_impl_k_uptime_ticks();
}
#include <zephyr/syscalls/k_uptime_ticks_mrsh.c>
#endif /* CONFIG_USERSPACE */

/**
 * @brief Calculate the timepoint for a timeout
 *
 * @param timeout Timeout value
 * @return Calculated timepoint
 */
k_timepoint_t sys_timepoint_calc(k_timeout_t timeout)
{
	k_timepoint_t timepoint;

	if (K_TIMEOUT_EQ(timeout, K_FOREVER)) {
		timepoint.tick = UINT64_MAX;
	} else if (K_TIMEOUT_EQ(timeout, K_NO_WAIT)) {
		timepoint.tick = 0;
	} else {
		k_ticks_t dt = timeout.ticks;

		if (IS_ENABLED(CONFIG_TIMEOUT_64BIT) && Z_TICK_ABS(dt) >= 0) {
			timepoint.tick = Z_TICK_ABS(dt);
		} else {
			timepoint.tick = sys_clock_tick_get() + MAX(1, dt);
		}
	}

	return timepoint;
}

/**
 * @brief Calculate the timeout for a timepoint
 *
 * @param timepoint Timepoint value
 * @return Calculated timeout
 */
k_timeout_t sys_timepoint_timeout(k_timepoint_t timepoint)
{
	uint64_t now, remaining;

	if (timepoint.tick == UINT64_MAX) {
		return K_FOREVER;
	}
	if (timepoint.tick == 0) {
		return K_NO_WAIT;
	}

	now = sys_clock_tick_get();
	remaining = (timepoint.tick > now) ? (timepoint.tick - now) : 0;
	return K_TICKS(remaining);
}

#ifdef CONFIG_ZTEST
void z_impl_sys_clock_tick_set(uint64_t tick)
{
	curr_tick = tick;
}

void z_vrfy_sys_clock_tick_set(uint64_t tick)
{
	z_impl_sys_clock_tick_set(tick);
}
#endif /* CONFIG_ZTEST */
//GST
