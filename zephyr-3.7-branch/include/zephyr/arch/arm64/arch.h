// zephyr-3.7-branch/include/zephyr/arch/arm64/arch.h

#ifndef ZEPHYR_INCLUDE_ARCH_ARM64_ARCH_H_
#define ZEPHYR_INCLUDE_ARCH_ARM64_ARCH_H_

/* Add include for DTS generated information */
#include <zephyr/devicetree.h>

#include <zephyr/arch/arm64/thread.h>
#include <zephyr/arch/arm64/exception.h>
#include <zephyr/arch/arm64/irq.h>
#include <zephyr/arch/arm64/misc.h>
#include <zephyr/arch/arm64/asm_inline.h>
#include <zephyr/arch/arm64/cpu.h>
#include <zephyr/arch/arm64/macro.inc>
#include <zephyr/arch/arm64/sys_io.h>
#include <zephyr/arch/arm64/timer.h>
#include <zephyr/arch/arm64/error.h>
#include <zephyr/arch/arm64/mm.h>
#include <zephyr/arch/arm64/thread_stack.h>
#include <zephyr/arch/common/addr_types.h>
#include <zephyr/arch/common/sys_bitops.h>
#include <zephyr/arch/common/ffs.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _ASMLANGUAGE

#include <zephyr/sys/slist.h>

/**
 * @brief Architecture-specific memory domain structure
 *
 * This structure represents the architecture-specific memory domain.
 */
struct arch_mem_domain {
#ifdef CONFIG_ARM_MMU
	struct arm_mmu_ptables ptables; /**< Page tables for ARM MMU */
#endif
	sys_snode_t node; /**< System node */
};

#endif /* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ARCH_ARM64_ARCH_H_ */
//GST