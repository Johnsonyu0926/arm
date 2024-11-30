//zephyr-3.7-branch/lib/libc/common/source/thrd/tss.c

#include <errno.h>
#include <threads.h>
#include <zephyr/kernel.h>
#include <zephyr/posix/pthread.h>

/**
 * @brief Create a thread-specific storage key
 *
 * @param key Pointer to the key
 * @param destructor Destructor function for the key
 * @return thrd_success on success, thrd_busy if out of resources, thrd_nomem if out of memory, or thrd_error on failure
 */
int tss_create(tss_t *key, tss_dtor_t destructor)
{
	switch (pthread_key_create(key, destructor)) {
	case 0:
		return thrd_success;
	case EAGAIN:
		return thrd_busy;
	case ENOMEM:
		return thrd_nomem;
	default:
		return thrd_error;
	}
}

/**
 * @brief Get the value of a thread-specific storage key
 *
 * @param key The key
 * @return The value associated with the key
 */
void *tss_get(tss_t key)
{
	return pthread_getspecific(key);
}

/**
 * @brief Set the value of a thread-specific storage key
 *
 * @param key The key
 * @param val The value to set
 * @return thrd_success on success, thrd_nomem if out of memory, or thrd_error on failure
 */
int tss_set(tss_t key, void *val)
{
	switch (pthread_setspecific(key, val)) {
	case 0:
		return thrd_success;
	case ENOMEM:
		return thrd_nomem;
	default:
		return thrd_error;
	}
}

/**
 * @brief Delete a thread-specific storage key
 *
 * @param key The key
 */
void tss_delete(tss_t key)
{
	(void)pthread_key_delete(key);
}

//GST
