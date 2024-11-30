// kernel/compiler_stack_protect.c

#include <zephyr/toolchain.h> /* compiler specific configurations */
#include <zephyr/kernel_structs.h>
#include <zephyr/toolchain.h>
#include <zephyr/linker/sections.h>
#include <zephyr/kernel.h>
#include <zephyr/app_memory/app_memdomain.h>

/**
 * @brief Stack canary error handler
 *
 * This function is invoked when a stack canary error is detected.
 *
 * @return Does not return
 */
void _StackCheckHandler(void)
{
	/* Stack canary error is a software fatal condition; treat it as such. */
	z_except_reason(K_ERR_STACK_CHK_FAIL);
	CODE_UNREACHABLE; /* LCOV_EXCL_LINE */
}

/* Global variable */

/*
 * Symbol referenced by GCC compiler generated code for canary value.
 * The canary value gets initialized in z_cstart().
 */
#ifdef CONFIG_STACK_CANARIES_TLS
__thread volatile uintptr_t __stack_chk_guard;
#elif CONFIG_USERSPACE
K_APP_DMEM(z_libc_partition) volatile uintptr_t __stack_chk_guard;
#else
__noinit volatile uintptr_t __stack_chk_guard;
#endif

/**
 * @brief Referenced by GCC compiler generated code
 *
 * This routine is invoked when a stack canary error is detected, indicating
 * a buffer overflow or stack corruption problem.
 */
FUNC_ALIAS(_StackCheckHandler, __stack_chk_fail, void);
//GST