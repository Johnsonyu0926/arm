//zephyr-3.7-branch/lib/libc/armstdc/include/sys/_timespec.h

#ifndef ZEPHYR_LIB_LIBC_COMMON_INCLUDE_THREADS_H_
#define ZEPHYR_LIB_LIBC_COMMON_INCLUDE_THREADS_H_

#include <time.h>
#include <machine/_threads.h>
#include <zephyr/toolchain.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*thrd_start_t)(void *arg);

enum {
	thrd_success,
#define thrd_success thrd_success
	thrd_nomem,
#define thrd_nomem thrd_nomem
	thrd_timedout,
#define thrd_timedout thrd_timedout
	thrd_busy,
#define thrd_busy thrd_busy
	thrd_error,
#define thrd_error thrd_error
};

/**
 * @brief Create a new thread
 *
 * @param thr Pointer to the thread handle
 * @param func Function to be executed by the thread
 * @param arg Argument to be passed to the thread function
 * @return thrd_success on success, or an error code on failure
 */
int thrd_create(thrd_t *thr, thrd_start_t func, void *arg);

/**
 * @brief Compare two thread handles
 *
 * @param lhs First thread handle
 * @param rhs Second thread handle
 * @return Non-zero if the thread handles are equal, zero otherwise
 */
int thrd_equal(thrd_t lhs, thrd_t rhs);

/**
 * @brief Get the current thread handle
 *
 * @return Current thread handle
 */
thrd_t thrd_current(void);

/**
 * @brief Sleep for a specified duration
 *
 * @param duration Pointer to the duration to sleep
 * @param remaining Pointer to store the remaining time if interrupted
 * @return thrd_success on success, or an error code on failure
 */
int thrd_sleep(const struct timespec *duration, struct timespec *remaining);

/**
 * @brief Yield the processor to another thread
 */
void thrd_yield(void);

/**
 * @brief Exit the current thread
 *
 * @param res Exit code
 */
_Noreturn void thrd_exit(int res);

/**
 * @brief Detach a thread
 *
 * @param thr Thread handle
 * @return thrd_success on success, or an error code on failure
 */
int thrd_detach(thrd_t thr);

/**
 * @brief Join a thread
 *
 * @param thr Thread handle
 * @param res Pointer to store the exit code of the joined thread
 * @return thrd_success on success, or an error code on failure
 */
int thrd_join(thrd_t thr, int *res);

enum {
	mtx_plain,
#define mtx_plain mtx_plain
	mtx_timed,
#define mtx_timed mtx_timed
	mtx_recursive,
#define mtx_recursive mtx_recursive
};

/**
 * @brief Initialize a mutex
 *
 * @param mutex Pointer to the mutex handle
 * @param type Type of the mutex
 * @return thrd_success on success, or an error code on failure
 */
int mtx_init(mtx_t *mutex, int type);

/**
 * @brief Destroy a mutex
 *
 * @param mutex Pointer to the mutex handle
 */
void mtx_destroy(mtx_t *mutex);

/**
 * @brief Lock a mutex
 *
 * @param mutex Pointer to the mutex handle
 * @return thrd_success on success, or an error code on failure
 */
int mtx_lock(mtx_t *mutex);

/**
 * @brief Lock a mutex with a timeout
 *
 * @param mutex Pointer to the mutex handle
 * @param time_point Pointer to the timeout duration
 * @return thrd_success on success, or an error code on failure
 */
int mtx_timedlock(mtx_t *ZRESTRICT mutex, const struct timespec *ZRESTRICT time_point);

/**
 * @brief Try to lock a mutex
 *
 * @param mutex Pointer to the mutex handle
 * @return thrd_success on success, or an error code on failure
 */
int mtx_trylock(mtx_t *mutex);

/**
 * @brief Unlock a mutex
 *
 * @param mutex Pointer to the mutex handle
 * @return thrd_success on success, or an error code on failure
 */
int mtx_unlock(mtx_t *mutex);

/**
 * @brief Initialize a condition variable
 *
 * @param cond Pointer to the condition variable handle
 * @return thrd_success on success, or an error code on failure
 */
int cnd_init(cnd_t *cond);

/**
 * @brief Wait on a condition variable
 *
 * @param cond Pointer to the condition variable handle
 * @param mtx Pointer to the mutex handle
 * @return thrd_success on success, or an error code on failure
 */
int cnd_wait(cnd_t *cond, mtx_t *mtx);

/**
 * @brief Wait on a condition variable with a timeout
 *
 * @param cond Pointer to the condition variable handle
 * @param mtx Pointer to the mutex handle
 * @param ts Pointer to the timeout duration
 * @return thrd_success on success, or an error code on failure
 */
int cnd_timedwait(cnd_t *ZRESTRICT cond, mtx_t *ZRESTRICT mtx, const struct timespec *ZRESTRICT ts);

/**
 * @brief Signal a condition variable
 *
 * @param cond Pointer to the condition variable handle
 * @return thrd_success on success, or an error code on failure
 */
int cnd_signal(cnd_t *cond);

/**
 * @brief Broadcast a condition variable
 *
 * @param cond Pointer to the condition variable handle
 * @return thrd_success on success, or an error code on failure
 */
int cnd_broadcast(cnd_t *cond);

/**
 * @brief Destroy a condition variable
 *
 * @param cond Pointer to the condition variable handle
 */
void cnd_destroy(cnd_t *cond);

#ifndef thread_local
#define	thread_local _Thread_local
#endif

typedef void (*tss_dtor_t)(void *val);

/**
 * @brief Create a thread-specific storage key
 *
 * @param key Pointer to the TSS key handle
 * @param destructor Destructor function for the TSS key
 * @return thrd_success on success, or an error code on failure
 */
int tss_create(tss_t *key, tss_dtor_t destructor);

/**
 * @brief Get the value of a thread-specific storage key
 *
 * @param key TSS key handle
 * @return Value of the TSS key
 */
void *tss_get(tss_t key);

/**
 * @brief Set the value of a thread-specific storage key
 *
 * @param key TSS key handle
 * @param val Value to set
 * @return thrd_success on success, or an error code on failure
 */
int tss_set(tss_t key, void *val);

/**
 * @brief Delete a thread-specific storage key
 *
 * @param key TSS key handle
 */
void tss_delete(tss_t key);

/**
 * @brief Call a function once
 *
 * @param flag Pointer to the once flag
 * @param func Function to call
 */
void call_once(once_flag *flag, void (*func)(void));

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_LIB_LIBC_COMMON_INCLUDE_THREADS_H_ */
//GST