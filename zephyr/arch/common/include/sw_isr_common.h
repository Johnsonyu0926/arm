// Filename: sw_isr_common.h
// 评分: 92

#ifndef ZEPHYR_ARCH_COMMON_INCLUDE_SW_ISR_COMMON_H_
#define ZEPHYR_ARCH_COMMON_INCLUDE_SW_ISR_COMMON_H_

#if !defined(_ASMLANGUAGE)
#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif

unsigned int z_get_sw_isr_table_idx(unsigned int irq);
const struct device *z_get_sw_isr_device_from_irq(unsigned int irq);
unsigned int z_get_sw_isr_irq_from_device(const struct device *dev);

#ifdef __cplusplus
}
#endif

#endif /* _ASMLANGUAGE */

#endif /* ZEPHYR_ARCH_COMMON_INCLUDE_SW_ISR_COMMON_H_ */
// By GST @Date