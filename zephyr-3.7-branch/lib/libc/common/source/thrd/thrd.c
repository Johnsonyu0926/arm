//zephyr-3.7-branch/lib/libc/common/source/thrd/thrd.c

#include <errno.h>
#include <threads.h>
#include <zephyr/kernel.h>
#include <zephyr/posix/pthread.h>
#include <zephyr/posix/sched.h>

/**
 * @brief Argument structure for thread trampoline
 */
struct thrd_trampoline_arg {
	thrd_start_t func;
	void *arg;
};

/**
 * @brief Create a new thread
 *
 * @param thr Pointer to the thread handle
 * @param func Function to be executed by the thread
 * @param arg Argument to be passed to the thread function
 * @return thrd_success on success, thrd_nomem if out of memory, or thrd_error on failure
 */
int thrd_create(thrd_t *thr, thrd_start_t func, void *arg)
{
	typedef void *(*pthread_func_t)(void *arg);

	pthread_func_t pfunc = (pthread_func_t)func;

	switch (pthread_create(thr, NULL, pfunc, arg)) {
	case 0:
		return thrd_success;
	case EAGAIN:
		return thrd_nomem;
	default:
		return thrd_error;
	}
}

/**
 * @brief Compare two thread handles
 *
 * @param lhs First thread handle
 * @param rhs Second thread handle
 * @return Non-zero if the thread handles are equal, zero otherwise
 */
int thrd_equal(thrd_t lhs, thrd_t rhs)
{
	return pthread_equal(lhs, rhs);
}

/**
 * @brief Get the current thread handle
 *
 * @return The current thread handle
 */
thrd_t thrd_current(void)
{
	return pthread_self();
}

/**
 * @brief Sleep for a specified duration
 *
 * @param duration Pointer to the duration to sleep
 * @param remaining Pointer to store the remaining time if interrupted
 * @return 0 on success, or -1 on failure
 */
int thrd_sleep(const struct timespec *duration, struct timespec *remaining)
{
	return nanosleep(duration, remaining);
}

/**
 * @brief Yield the processor to another thread
 */
void thrd_yield(void)
{
	(void)sched_yield();
}

/**
 * @brief Exit the current thread
 *
 * @param res Exit code
 */
FUNC_NORETURN void thrd_exit(int res)
{
	pthread_exit(INT_TO_POINTER(res));

	CODE_UNREACHABLE;
}

/**
 * @brief Detach a thread
 *
 * @param thr Thread handle
 * @return thrd_success on success, or thrd_error on failure
 */
int thrd_detach(thrd_t thr)
{
	switch (pthread_detach(thr)) {
	case 0:
		return thrd_success;
	default:
		return thrd_error;
	}
}

/**
 * @brief Join a thread
 *
 * @param thr Thread handle
 * @param res Pointer to store the exit code of the thread
 * @return thrd_success on success, or thrd_error on failure
 */
int thrd_join(thrd_t thr, int *res)
{
	void *ret;

	switch (pthread_join(thr, &ret)) {
	case 0:
		if (res != NULL) {
			*res = POINTER_TO_INT(ret);
		}
		return thrd_success;
	default:
		return thrd_error;
	}
}
//GST