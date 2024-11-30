// zephyr-3.7-branch/include/zephyr/arch/arm64/arch_inlines.h

#ifndef ZEPHYR_INCLUDE_ARCH_ARM64_ARCH_INLINES_H
#define ZEPHYR_INCLUDE_ARCH_ARM64_ARCH_INLINES_H

#ifndef _ASMLANGUAGE

#include <zephyr/kernel_structs.h>
#include <zephyr/arch/arm64/lib_helpers.h>
#include <zephyr/arch/arm64/tpidrro_el0.h>
#include <zephyr/sys/__assert.h>

/**
 * @brief Get the current CPU structure pointer.
 *
 * This function returns a pointer to the current CPU structure.
 *
 * @return Pointer to the current CPU structure.
 */
static ALWAYS_INLINE _cpu_t *arch_curr_cpu(void)
{
	return (_cpu_t *)(read_tpidrro_el0() & TPIDRROEL0_CURR_CPU);
}

/**
 * @brief Get the current exception depth.
 *
 * This function returns the current exception depth.
 *
 * @return Current exception depth.
 */
static ALWAYS_INLINE int arch_exception_depth(void)
{
	return (read_tpidrro_el0() & TPIDRROEL0_EXC_DEPTH) / TPIDRROEL0_EXC_UNIT;
}

/**
 * @brief Get the processor ID.
 *
 * This function returns the processor ID.
 *
 * @return Processor ID.
 */
static ALWAYS_INLINE uint32_t arch_proc_id(void)
{
	uint64_t cpu_mpid = read_mpidr_el1();

	__ASSERT(cpu_mpid == (uint32_t)cpu_mpid, "mpid extends past 32 bits");

	return (uint32_t)cpu_mpid;
}

/**
 * @brief Get the number of CPUs.
 *
 * This function returns the number of CPUs.
 *
 * @return Number of CPUs.
 */
static ALWAYS_INLINE unsigned int arch_num_cpus(void)
{
	return CONFIG_MP_MAX_NUM_CPUS;
}

#endif /* !_ASMLANGUAGE */
#endif /* ZEPHYR_INCLUDE_ARCH_ARM64_ARCH_INLINES_H */
//gst