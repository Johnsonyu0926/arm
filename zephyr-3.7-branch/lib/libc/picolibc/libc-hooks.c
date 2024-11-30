//zephyr-3.7-branch/lib/libc/picolibc/libc-hooks.c

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <zephyr/posix/sys/stat.h>
#include <sys/time.h>
#include <zephyr/arch/cpu.h>
#include <zephyr/linker/linker-defs.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/errno_private.h>
#include <zephyr/sys/libc-hooks.h>
#include <zephyr/internal/syscall_handler.h>
#include <zephyr/app_memory/app_memdomain.h>
#include <zephyr/init.h>
#include <zephyr/sys/sem.h>
#include <zephyr/logging/log.h>
#ifdef CONFIG_MMU
#include <zephyr/kernel/mm.h>
#endif

#define LIBC_BSS K_APP_BMEM(z_libc_partition)
#define LIBC_DATA K_APP_DMEM(z_libc_partition)

static LIBC_DATA int (*_stdout_hook)(int);

/**
 * @brief Implementation of zephyr_fputc
 *
 * @param a Character to output
 * @param out Output stream
 * @return 0 on success
 */
int z_impl_zephyr_fputc(int a, FILE *out)
{
	(*_stdout_hook)(a);
	return 0;
}

#ifdef CONFIG_USERSPACE
/**
 * @brief Verify and implement zephyr_fputc for userspace
 *
 * @param c Character to output
 * @param stream Output stream
 * @return 0 on success
 */
static inline int z_vrfy_zephyr_fputc(int c, FILE *stream)
{
	return z_impl_zephyr_fputc(c, stream);
}
#include <zephyr/syscalls/zephyr_fputc_mrsh.c>
#endif

/**
 * @brief Picolibc put character function
 *
 * @param a Character to output
 * @param f Output stream
 * @return 0 on success
 */
static int picolibc_put(char a, FILE *f)
{
	zephyr_fputc(a, f);
	return 0;
}

static LIBC_DATA FILE __stdout = FDEV_SETUP_STREAM(picolibc_put, NULL, NULL, 0);
static LIBC_DATA FILE __stdin = FDEV_SETUP_STREAM(NULL, NULL, NULL, 0);

#ifdef __strong_reference
#define STDIO_ALIAS(x) __strong_reference(stdout, x);
#else
#define STDIO_ALIAS(x) FILE *const x = &__stdout;
#endif

FILE *const stdin = &__stdin;
FILE *const stdout = &__stdout;
STDIO_ALIAS(stderr);

/**
 * @brief Install a custom stdout hook function
 *
 * @param hook Pointer to the custom hook function
 */
void __stdout_hook_install(int (*hook)(int))
{
	_stdout_hook = hook;
	__stdout.flags |= _FDEV_SETUP_WRITE;
}

/**
 * @brief Install a custom stdin hook function
 *
 * @param hook Pointer to the custom hook function
 */
void __stdin_hook_install(unsigned char (*hook)(void))
{
	__stdin.get = (int (*)(FILE *)) hook;
	__stdin.flags |= _FDEV_SETUP_READ;
}

#include <zephyr/sys/cbprintf.h>

struct cb_bits {
	FILE f;
	cbprintf_cb out;
	void *ctx;
};

/**
 * @brief Callback put character function
 *
 * @param c Character to output
 * @param _s Pointer to the cb_bits structure
 * @return 0 on success
 */
static int cbputc(char c, FILE *_s)
{
	struct cb_bits *s = (struct cb_bits *) _s;

	(*s->out)(c, s->ctx);
	return 0;
}

/**
 * @brief Callback vprintf function
 *
 * @param out Callback function
 * @param ctx Context
 * @param fp Format string
 * @param ap Argument list
 * @return Number of characters written
 */
int cbvprintf(cbprintf_cb out, void *ctx, const char *fp, va_list ap)
{
	struct cb_bits s = {
		.f = FDEV_SETUP_STREAM(cbputc, NULL, NULL, _FDEV_SETUP_WRITE),
		.out = out,
		.ctx = ctx,
	};
	return vfprintf(&s.f, fp, ap);
}

/**
 * @brief Exit the program
 *
 * @param status Exit status
 */
__weak void _exit(int status)
{
	printk("exit\n");
	while (1) {
		Z_SPIN_DELAY(100);
	}
}

#ifdef CONFIG_MULTITHREADING
#define _LOCK_T void *
K_MUTEX_DEFINE(__lock___libc_recursive_mutex);

#ifdef CONFIG_USERSPACE
/**
 * @brief Grant public access to picolibc lock after boot
 *
 * @return 0 on success
 */
static int picolibc_locks_prepare(void)
{
	/* Initialise recursive locks */
	k_object_access_all_grant(&__lock___libc_recursive_mutex);

	return 0;
}

SYS_INIT(picolibc_locks_prepare, POST_KERNEL,
	 CONFIG_KERNEL_INIT_PRIORITY_DEFAULT);
#endif /* CONFIG_USERSPACE */

/**
 * @brief Create a new dynamic recursive lock
 *
 * @param lock Pointer to the lock
 */
void __retarget_lock_init_recursive(_LOCK_T *lock)
{
	__ASSERT_NO_MSG(lock != NULL);

	/* Allocate mutex object */
#ifndef CONFIG_USERSPACE
	*lock = malloc(sizeof(struct k_mutex));
#else
	*lock = k_object_alloc(K_OBJ_MUTEX);
#endif /* !CONFIG_USERSPACE */
	__ASSERT(*lock != NULL, "recursive lock allocation failed");

	k_mutex_init((struct k_mutex *)*lock);
}

/**
 * @brief Create a new dynamic non-recursive lock
 *
 * @param lock Pointer to the lock
 */
void __retarget_lock_init(_LOCK_T *lock)
{
	__retarget_lock_init_recursive(lock);
}

/**
 * @brief Close dynamic recursive lock
 *
 * @param lock Pointer to the lock
 */
void __retarget_lock_close_recursive(_LOCK_T lock)
{
	__ASSERT_NO_MSG(lock != NULL);
#ifndef CONFIG_USERSPACE
	free(lock);
#else
	k_object_release(lock);
#endif /* !CONFIG_USERSPACE */
}

/**
 * @brief Close dynamic non-recursive lock
 *
 * @param lock Pointer to the lock
 */
void __retarget_lock_close(_LOCK_T lock)
{
	__retarget_lock_close_recursive(lock);
}

/**
 * @brief Acquire recursive lock
 *
 * @param lock Pointer to the lock
 */
void __retarget_lock_acquire_recursive(_LOCK_T lock)
{
	__ASSERT_NO_MSG(lock != NULL);
	k_mutex_lock((struct k_mutex *)lock, K_FOREVER);
}

/**
 * @brief Acquire non-recursive lock
 *
 * @param lock Pointer to the lock
 */
void __retarget_lock_acquire(_LOCK_T lock)
{
	__retarget_lock_acquire_recursive(lock);
}

/**
 * @brief Try acquiring recursive lock
 *
 * @param lock Pointer to the lock
 * @return 1 if the lock was acquired, 0 otherwise
 */
int __retarget_lock_try_acquire_recursive(_LOCK_T lock)
{
	__ASSERT_NO_MSG(lock != NULL);
	return !k_mutex_lock((struct k_mutex *)lock, K_NO_WAIT);
}

/**
 * @brief Try acquiring non-recursive lock
 *
 * @param lock Pointer to the lock
 * @return 1 if the lock was acquired, 0 otherwise
 */
int __retarget_lock_try_acquire(_LOCK_T lock)
{
	return __retarget_lock_try_acquire_recursive(lock);
}

/**
 * @brief Release recursive lock
 *
 * @param lock Pointer to the lock
 */
void __retarget_lock_release_recursive(_LOCK_T lock)
{
	__ASSERT_NO_MSG(lock != NULL);
	k_mutex_unlock((struct k_mutex *)lock);
}

/**
 * @brief Release non-recursive lock
 *
 * @param lock Pointer to the lock
 */
void __retarget_lock_release(_LOCK_T lock)
{
	__retarget_lock_release_recursive(lock);
}

#endif /* CONFIG_MULTITHREADING */

#ifdef CONFIG_PICOLIBC_ASSERT_VERBOSE

/**
 * @brief Handle assertion failure with verbose output
 *
 * @param file Source file name
 * @param line Line number
 * @param function Function name
 * @param expression Failed expression
 */
FUNC_NORETURN void __assert_func(const char *file, int line,
				 const char *function, const char *expression)
{
	__ASSERT(0, "assertion \"%s\" failed: file \"%s\", line %d%s%s\n",
		 expression, file, line,
		 function ? ", function: " : "", function ? function : "");
	CODE_UNREACHABLE;
}

#else

/**
 * @brief Handle assertion failure without arguments
 */
FUNC_NORETURN void __assert_no_args(void)
{
	__ASSERT_NO_MSG(0);
	CODE_UNREACHABLE;
}

#endif

/**
 * @brief Handle buffer overflow detection
 */
__weak FUNC_NORETURN void __chk_fail(void)
{
	printk("* buffer overflow detected *\n");
	z_except_reason(K_ERR_STACK_CHK_FAIL);
	CODE_UNREACHABLE;
}

#ifndef CONFIG_LIBC_ERRNO

/**
 * @brief Get the address of the errno variable
 *
 * @return Pointer to the errno variable
 */
int *z_errno_wrap(void)
{
	return z_errno();
}

#endif
//GST