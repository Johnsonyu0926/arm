//arch/arc/core/tls.c
#include <zephyr/kernel.h>
#include <zephyr/kernel_structs.h>
#include <kernel_internal.h>
#include <kernel_tls.h>
#include <zephyr/app_memory/app_memdomain.h>
#include <zephyr/sys/util.h>

/**
 * @brief Set up the TLS (Thread Local Storage) area for a new thread
 *
 * This function sets up the TLS area for a new thread. The TLS area for ARM
 * has some data fields followed by thread data and bss. These fields are
 * supposed to be used by the toolchain and OS TLS code to aid in locating
 * the TLS data/bss. Zephyr currently has no use for this so we can simply
 * skip these. However, since GCC is generating code assuming these fields
 * are there, we simply skip them when setting the TLS pointer.
 *
 * @param new_thread Pointer to the new thread
 * @param stack_ptr Pointer to the stack
 * @return The size of the TLS area
 */
size_t arch_tls_stack_setup(struct k_thread *new_thread, char *stack_ptr)
{
	/*
	 * Since we are populating things backwards,
	 * setup the TLS data/bss area first.
	 */
	stack_ptr -= z_tls_data_size();
	z_tls_copy(stack_ptr);

	/* Skip two pointers due to toolchain */
	stack_ptr -= sizeof(uintptr_t) * 2;

	/*
	 * Set thread TLS pointer which is used in
	 * context switch to point to TLS area.
	 */
	new_thread->tls = POINTER_TO_UINT(stack_ptr);

	return (z_tls_data_size() + (sizeof(uintptr_t) * 2));
}
//GST