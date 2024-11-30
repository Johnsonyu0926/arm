//zephyr-3.7-branch/lib/libc/common/source/thrd/cnd.c

#include <errno.h>
#include <threads.h>
#include <zephyr/posix/pthread.h>

/**
 * @brief Broadcast a condition variable
 *
 * @param cond Pointer to the condition variable
 * @return thrd_success on success, or thrd_error on failure
 */
int cnd_broadcast(cnd_t *cond)
{
	switch (pthread_cond_broadcast(cond)) {
	case 0:
		return thrd_success;
	default:
		return thrd_error;
	}
}

/**
 * @brief Destroy a condition variable
 *
 * @param cond Pointer to the condition variable
 */
void cnd_destroy(cnd_t *cond)
{
	(void)pthread_cond_destroy(cond);
}

/**
 * @brief Initialize a condition variable
 *
 * @param cond Pointer to the condition variable
 * @return thrd_success on success, thrd_nomem if out of memory, or thrd_error on failure
 */
int cnd_init(cnd_t *cond)
{
	switch (pthread_cond_init(cond, NULL)) {
	case 0:
		return thrd_success;
	case ENOMEM:
		return thrd_nomem;
	default:
		return thrd_error;
	}
}

/**
 * @brief Signal a condition variable
 *
 * @param cond Pointer to the condition variable
 * @return thrd_success on success, thrd_nomem if out of memory, or thrd_error on failure
 */
int cnd_signal(cnd_t *cond)
{
	switch (pthread_cond_signal(cond)) {
	case 0:
		return thrd_success;
	case ENOMEM:
		return thrd_nomem;
	default:
		return thrd_error;
	}
}

/**
 * @brief Wait on a condition variable with a timeout
 *
 * @param cond Pointer to the condition variable
 * @param mtx Pointer to the mutex
 * @param ts Pointer to the timeout duration
 * @return thrd_success on success, thrd_timedout if timed out, or thrd_error on failure
 */
int cnd_timedwait(cnd_t *restrict cond, mtx_t *restrict mtx, const struct timespec *restrict ts)
{
	switch (pthread_cond_timedwait(cond, mtx, ts)) {
	case 0:
		return thrd_success;
	case ETIMEDOUT:
		return thrd_timedout;
	default:
		return thrd_error;
	}
}

/**
 * @brief Wait on a condition variable
 *
 * @param cond Pointer to the condition variable
 * @param mtx Pointer to the mutex
 * @return thrd_success on success, or thrd_error on failure
 */
int cnd_wait(cnd_t *cond, mtx_t *mtx)
{
	switch (pthread_cond_wait(cond, mtx)) {
	case 0:
		return thrd_success;
	default:
		return thrd_error;
	}
}
//GST