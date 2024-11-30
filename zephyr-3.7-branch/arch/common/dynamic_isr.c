//arch/arc/common/include/dynamic_isr.c
#include "sw_isr_common.h"
#include <zephyr/sw_isr_table.h>
#include <zephyr/irq.h>
#include <zephyr/sys/__assert.h>

/**
 * @brief Install an ISR for the specified IRQ
 *
 * This function installs an ISR for the specified IRQ. If dynamic IRQs are
 * enabled, the ISR table is in RAM and can be modified.
 *
 * @param irq IRQ number
 * @param routine ISR routine to install
 * @param param Parameter to pass to the ISR routine
 */
void __weak z_isr_install(unsigned int irq, void (*routine)(const void *),
			  const void *param)
{
	unsigned int table_idx;

	/*
	 * Do not assert on the IRQ enable status for ARM GIC since the SGI
	 * type interrupts are always enabled and attempting to install an ISR
	 * for them will cause the assertion to fail.
	 */
#ifndef CONFIG_GIC
	__ASSERT(!irq_is_enabled(irq), "IRQ %d is enabled", irq);
#endif /* !CONFIG_GIC */

	table_idx = z_get_sw_isr_table_idx(irq);

	/* If dynamic IRQs are enabled, then the _sw_isr_table is in RAM and
	 * can be modified
	 */
	_sw_isr_table[table_idx].arg = param;
	_sw_isr_table[table_idx].isr = routine;
}

/**
 * @brief Connect a dynamic ISR for the specified IRQ
 *
 * This function connects a dynamic ISR for the specified IRQ. Some
 * architectures do not interpret flags or priority and have no more processing
 * to do than this. This function provides a generic fallback.
 *
 * @param irq IRQ number
 * @param priority IRQ priority
 * @param routine ISR routine to connect
 * @param parameter Parameter to pass to the ISR routine
 * @param flags IRQ flags
 * @return IRQ number
 */
int __weak arch_irq_connect_dynamic(unsigned int irq,
				    unsigned int priority,
				    void (*routine)(const void *),
				    const void *parameter,
				    uint32_t flags)
{
	ARG_UNUSED(flags);
	ARG_UNUSED(priority);

	z_isr_install(irq, routine, parameter);
	return irq;
}
//GST