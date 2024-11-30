// kernel/dynamic_disabled.c

#include <errno.h>
#include <zephyr/kernel.h>
#include <zephyr/kernel/thread_stack.h>

/**
 * @brief Allocate a thread stack (disabled)
 *
 * @param size Size of the stack
 * @param flags Allocation flags
 * @return NULL (operation not supported)
 */
k_thread_stack_t *z_impl_k_thread_stack_alloc(size_t size, int flags)
{
	ARG_UNUSED(size);
	ARG_UNUSED(flags);

	return NULL;
}

/**
 * @brief Free a thread stack (disabled)
 *
 * @param stack Pointer to the stack
 * @return -ENOSYS (operation not supported)
 */
int z_impl_k_thread_stack_free(k_thread_stack_t *stack)
{
	ARG_UNUSED(stack);

	return -ENOSYS;
}
//GST