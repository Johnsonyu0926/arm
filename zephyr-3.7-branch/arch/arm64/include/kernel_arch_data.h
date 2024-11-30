//arch/arc/include/kernel_arch_data.h
#ifndef ZEPHYR_ARCH_ARM64_INCLUDE_KERNEL_ARCH_DATA_H_
#define ZEPHYR_ARCH_ARM64_INCLUDE_KERNEL_ARCH_DATA_H_

#include <zephyr/toolchain.h>
#include <zephyr/linker/sections.h>
#include <zephyr/arch/cpu.h>
#include <exception.h>

#ifndef _ASMLANGUAGE
#include <zephyr/kernel.h>
#include <zephyr/types.h>
#include <zephyr/sys/dlist.h>
#include <zephyr/sys/atomic.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Exception Stack Frame (ESF) type
 *
 * This type represents the exception stack frame used in ARM64 architecture.
 */
typedef struct arch_esf _esf_t;

/**
 * @brief Basic Stack Frame (BSF) type
 *
 * This type represents the basic stack frame used in ARM64 architecture.
 */
typedef struct __basic_sf _basic_sf_t;

#ifdef __cplusplus
}
#endif

#endif /* _ASMLANGUAGE */

#endif /* ZEPHYR_ARCH_ARM64_INCLUDE_KERNEL_ARCH_DATA_H_ */
//GST