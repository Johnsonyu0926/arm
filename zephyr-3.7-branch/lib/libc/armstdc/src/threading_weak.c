
//zephyr-3.7-branch/lib/libc/armstdc/src/threading_weak.c

#include <zephyr/kernel.h>

/**
 * @brief Initialize a mutex
 *
 * @param mutex Pointer to the mutex
 * @return Always returns 0
 */
int __weak z_impl_k_mutex_init(struct k_mutex *mutex)
{
	return 0;
}

/**
 * @brief Lock a mutex
 *
 * @param mutex Pointer to the mutex
 * @param timeout Timeout for locking the mutex
 * @return Always returns 0
 */
int __weak z_impl_k_mutex_lock(struct k_mutex *mutex, k_timeout_t timeout)
{
	return 0;
}

/**
 * @brief Unlock a mutex
 *
 * @param mutex Pointer to the mutex
 * @return Always returns 0
 */
int __weak z_impl_k_mutex_unlock(struct k_mutex *mutex)
{
	return 0;
}

/**
 * @brief Give a semaphore
 *
 * @param sem Pointer to the semaphore
 */
void __weak z_impl_k_sem_give(struct k_sem *sem)
{
}

/**
 * @brief Initialize a semaphore
 *
 * @param sem Pointer to the semaphore
 * @param initial_count Initial count of the semaphore
 * @param limit Maximum count of the semaphore
 * @return Always returns 0
 */
int __weak z_impl_k_sem_init(struct k_sem *sem, unsigned int initial_count,
		      unsigned int limit)
{
	return 0;
}

/**
 * @brief Take a semaphore
 *
 * @param sem Pointer to the semaphore
 * @param timeout Timeout for taking the semaphore
 * @return Always returns 0
 */
int __weak z_impl_k_sem_take(struct k_sem *sem, k_timeout_t timeout)
{
	return 0;
}

/**
 * @brief Query the current thread
 *
 * @return Always returns 0
 */
k_tid_t __weak z_impl_k_sched_current_thread_query(void)
{
	return 0;
}

/**
 * @brief Sleep for a specified number of microseconds
 *
 * @param us Number of microseconds to sleep
 * @return Always returns 0
 */
int32_t __weak z_impl_k_usleep(int us)
{
	return 0;
}

/**
 * @brief Abort a thread
 *
 * @param thread Pointer to the thread
 */
void __weak z_thread_abort(struct k_thread *thread)
{
}

/**
 * @brief Lock the scheduler
 */
void __weak k_sched_lock(void)
{
}

/**
 * @brief Unlock the scheduler
 */
void __weak k_sched_unlock(void)
{
}
//@GST