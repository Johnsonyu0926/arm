//zephyr-3.7-branch/lib/os/thread_entry.c

#include <zephyr/kernel.h>
#ifdef CONFIG_CURRENT_THREAD_USE_TLS
#include <zephyr/random/random.h>

__thread k_tid_t z_tls_current;
#endif

#ifdef CONFIG_STACK_CANARIES_TLS
extern __thread volatile uintptr_t __stack_chk_guard;
#endif /* CONFIG_STACK_CANARIES_TLS */

/**
 * @brief Common thread entry point function
 *
 * This routine invokes the actual thread entry point function and passes
 * it three arguments. It also handles graceful termination of the thread
 * if the entry point function ever returns.
 *
 * This routine does not return, and is marked as such so the compiler won't
 * generate preamble code that is only used by functions that actually return.
 *
 * @param entry Thread entry point function
 * @param p1 First argument to the entry point function
 * @param p2 Second argument to the entry point function
 * @param p3 Third argument to the entry point function
 */
FUNC_NORETURN void z_thread_entry(k_thread_entry_t entry,
				 void *p1, void *p2, void *p3)
{
#ifdef CONFIG_CURRENT_THREAD_USE_TLS
	z_tls_current = k_sched_current_thread_query();
#endif
#ifdef CONFIG_STACK_CANARIES_TLS
	uintptr_t stack_guard;

	sys_rand_get((uint8_t *)&stack_guard, sizeof(stack_guard));
	__stack_chk_guard = stack_guard;
	__stack_chk_guard <<= 8;
#endif	/* CONFIG_STACK_CANARIES_TLS */
	entry(p1, p2, p3);

	k_thread_abort(k_current_get());

	/*
	 * Compiler can't tell that k_thread_abort() won't return and issues a
	 * warning unless we tell it that control never gets this far.
	 */

	CODE_UNREACHABLE; /* LCOV_EXCL_LINE */
}
//GST