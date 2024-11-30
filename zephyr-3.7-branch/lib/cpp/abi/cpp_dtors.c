//lib/cpp/abi/cpp_dtors.c
#include <zephyr/toolchain.h>

__weak void *__dso_handle;

#if !defined(CONFIG_ARCH_POSIX)
/*
 * For the POSIX architecture we do not define an empty __cxa_atexit() as it otherwise
 * would override its host libC counterpart. And this would both disable the atexit()
 * hooks, and prevent possible test code global destructors from being registered.
 */

/**
 * @brief Register destructor for a global object
 *
 * This function registers a destructor for a global object. Currently, it does nothing,
 * assuming that global objects do not need to be deleted.
 *
 * @param destructor The global object destructor function
 * @param objptr Pointer to the global object
 * @param dso Dynamic Shared Object handle for shared libraries
 *
 * @retval 0 on success
 */
int __cxa_atexit(void (*destructor)(void *), void *objptr, void *dso)
{
	ARG_UNUSED(destructor);
	ARG_UNUSED(objptr);
	ARG_UNUSED(dso);
	return 0;
}
#endif
//GST