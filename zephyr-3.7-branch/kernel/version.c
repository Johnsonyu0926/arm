//kernel/version.c
#include <zephyr/types.h>
#include <zephyr/version.h> /* generated by MAKE, at compile time */

/**
 * @brief Return the kernel version of the present build
 *
 * The kernel version is a four-byte value, whose format is described in the
 * file "kernel_version.h".
 *
 * @return Kernel version
 */
uint32_t sys_kernel_version_get(void)
{
	return KERNELVERSION;
}
//GST