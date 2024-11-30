//arch/arc/core/irq_init.c

/**
 * @file
 * @brief ARM64 Cortex-A interrupt initialization
 */

#include <zephyr/arch/cpu.h>
#include <zephyr/drivers/interrupt_controller/gic.h>

/**
 * @brief Initialize interrupts
 *
 * This function invokes the ARM Generic Interrupt Controller (GIC) driver to
 * initialize the interrupt system on the SoCs that use the GIC as the primary
 * interrupt controller.
 *
 * When a custom interrupt controller is used, however, the SoC layer function
 * is invoked for SoC-specific interrupt system initialization.
 */
void z_arm64_interrupt_init(void)
{
#ifdef CONFIG_ARM_CUSTOM_INTERRUPT_CONTROLLER
	/* Invoke SoC-specific interrupt controller initialization */
	z_soc_irq_init();
#else
	/* Initialize the ARM Generic Interrupt Controller (GIC) */
	gic_init();
#endif
}
//GST
