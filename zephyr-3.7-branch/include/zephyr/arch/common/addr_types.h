// zephyr-3.7-branch/include/zephyr/arch/common/addr_types.h

#ifndef ZEPHYR_INCLUDE_ARCH_X86_ADDR_TYPES_H_
#define ZEPHYR_INCLUDE_ARCH_X86_ADDR_TYPES_H_

#ifndef _ASMLANGUAGE

/**
 * @brief Physical address type
 *
 * This type represents a physical address in memory.
 */
typedef uintptr_t paddr_t;

/**
 * @brief Virtual address type
 *
 * This type represents a virtual address in memory.
 */
typedef void *vaddr_t;

#endif /* _ASMLANGUAGE */

#endif /* ZEPHYR_INCLUDE_ARCH_X86_ADDR_TYPES_H_ */
//gst