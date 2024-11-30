//arch/arc/common/include/sw_isr_common.h
#ifndef ZEPHYR_ARCH_COMMON_INCLUDE_SW_ISR_COMMON_H_
#define ZEPHYR_ARCH_COMMON_INCLUDE_SW_ISR_COMMON_H_

#if !defined(_ASMLANGUAGE)
#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Compute the index in _sw_isr_table based on the passed IRQ
 *
 * This helper function computes the index in the _sw_isr_table based on the
 * passed IRQ number.
 *
 * @param irq IRQ number in its Zephyr format
 * @return Corresponding index in _sw_isr_table
 */
unsigned int z_get_sw_isr_table_idx(unsigned int irq);

/**
 * @brief Get the parent interrupt controller device based on the passed IRQ
 *
 * This helper function retrieves the parent interrupt controller device based
 * on the passed IRQ number.
 *
 * @param irq IRQ number in its Zephyr format
 * @return Corresponding interrupt controller device in _sw_isr_table
 */
const struct device *z_get_sw_isr_device_from_irq(unsigned int irq);

/**
 * @brief Get the IRQ number of the parent interrupt controller device
 *
 * This helper function retrieves the IRQ number of the passed parent interrupt
 * controller device.
 *
 * @param dev Parent interrupt controller device
 * @return IRQ number of the interrupt controller
 */
unsigned int z_get_sw_isr_irq_from_device(const struct device *dev);

#ifdef __cplusplus
}
#endif

#endif /* _ASMLANGUAGE */

#endif /* ZEPHYR_ARCH_COMMON_INCLUDE_SW_ISR_COMMON_H_ */
//GST