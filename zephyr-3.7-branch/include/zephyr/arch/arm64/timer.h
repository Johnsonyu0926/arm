// zephyr-3.7-branch/include/zephyr/arch/arm64/timer.h

#ifndef ZEPHYR_INCLUDE_ARCH_ARM64_TIMER_H_
#define ZEPHYR_INCLUDE_ARCH_ARM64_TIMER_H_

#ifndef _ASMLANGUAGE

#include <limits.h>
#include <zephyr/drivers/timer/arm_arch_timer.h>
#include <zephyr/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ARM_ARCH_TIMER_IRQ    ARM_TIMER_VIRTUAL_IRQ
#define ARM_ARCH_TIMER_PRIO   ARM_TIMER_VIRTUAL_PRIO
#define ARM_ARCH_TIMER_FLAGS  ARM_TIMER_VIRTUAL_FLAGS

/**
 * @brief Initialize the ARM architecture timer
 *
 * This function initializes the ARM architecture timer.
 */
static ALWAYS_INLINE void arm_arch_timer_init(void)
{
#ifdef CONFIG_TIMER_READS_ITS_FREQUENCY_AT_RUNTIME
	extern int z_clock_hw_cycles_per_sec;
	uint64_t cntfrq_el0 = read_cntfrq_el0();

	__ASSERT(cntfrq_el0 < INT_MAX, "cntfrq_el0 cannot fit in system 'int'");
	z_clock_hw_cycles_per_sec = (int) cntfrq_el0;
#endif
}

/**
 * @brief Set the compare value for the ARM architecture timer
 *
 * This function sets the compare value for the ARM architecture timer.
 *
 * @param val The compare value to set
 */
static ALWAYS_INLINE void arm_arch_timer_set_compare(uint64_t val)
{
	write_cntv_cval_el0(val);
}

/**
 * @brief Enable or disable the ARM architecture timer
 *
 * This function enables or disables the ARM architecture timer.
 *
 * @param enable True to enable the timer, false to disable it
 */
static ALWAYS_INLINE void arm_arch_timer_enable(unsigned char enable)
{
	uint64_t cntv_ctl;

	cntv_ctl = read_cntv_ctl_el0();

	if (enable) {
		cntv_ctl |= CNTV_CTL_ENABLE_BIT;
	} else {
		cntv_ctl &= ~CNTV_CTL_ENABLE_BIT;
	}

	write_cntv_ctl_el0(cntv_ctl);
}

/**
 * @brief Set the IRQ mask for the ARM architecture timer
 *
 * This function sets the IRQ mask for the ARM architecture timer.
 *
 * @param mask True to mask the IRQ, false to unmask it
 */
static ALWAYS_INLINE void arm_arch_timer_set_irq_mask(bool mask)
{
	uint64_t cntv_ctl;

	cntv_ctl = read_cntv_ctl_el0();

	if (mask) {
		cntv_ctl |= CNTV_CTL_IMASK_BIT;
	} else {
		cntv_ctl &= ~CNTV_CTL_IMASK_BIT;
	}

	write_cntv_ctl_el0(cntv_ctl);
}

/**
 * @brief Get the current count value of the ARM architecture timer
 *
 * This function returns the current count value of the ARM architecture timer.
 *
 * @return The current count value
 */
static ALWAYS_INLINE uint64_t arm_arch_timer_count(void)
{
	return read_cntvct_el0();
}

#ifdef __cplusplus
}
#endif

#endif /* _ASMLANGUAGE */

#endif /* ZEPHYR_INCLUDE_ARCH_ARM64_TIMER_H_ */
//GST