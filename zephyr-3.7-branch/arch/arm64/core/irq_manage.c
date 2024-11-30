//arch/arc/core/irq_manage.c
/**
 * @file
 * @brief ARM64 Cortex-A interrupt management
 */

#include <zephyr/kernel.h>
#include <zephyr/arch/cpu.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/irq.h>
#include <zephyr/toolchain.h>
#include <zephyr/linker/sections.h>
#include <zephyr/sw_isr_table.h>
#include <zephyr/drivers/interrupt_controller/gic.h>

void z_arm64_fatal_error(unsigned int reason, struct arch_esf *esf);

#if !defined(CONFIG_ARM_CUSTOM_INTERRUPT_CONTROLLER)
/*
 * The default interrupt controller for AArch64 is the ARM Generic Interrupt
 * Controller (GIC) and therefore the architecture interrupt control functions
 * are mapped to the GIC driver interface.
 *
 * When a custom interrupt controller is used (i.e.
 * CONFIG_ARM_CUSTOM_INTERRUPT_CONTROLLER is enabled), the architecture
 * interrupt control functions are mapped to the SoC layer in
 * `include/arch/arm64/irq.h`.
 */

/**
 * @brief Enable an interrupt
 *
 * @param irq The interrupt number to enable
 */
void arch_irq_enable(unsigned int irq)
{
	arm_gic_irq_enable(irq);
}

/**
 * @brief Disable an interrupt
 *
 * @param irq The interrupt number to disable
 */
void arch_irq_disable(unsigned int irq)
{
	arm_gic_irq_disable(irq);
}

/**
 * @brief Check if an interrupt is enabled
 *
 * @param irq The interrupt number to check
 * @return 1 if the interrupt is enabled, 0 otherwise
 */
int arch_irq_is_enabled(unsigned int irq)
{
	return arm_gic_irq_is_enabled(irq);
}

/**
 * @brief Set the priority of an interrupt
 *
 * @param irq The interrupt number
 * @param prio The priority to set
 * @param flags Additional flags for the interrupt
 */
void z_arm64_irq_priority_set(unsigned int irq, unsigned int prio, uint32_t flags)
{
	arm_gic_irq_set_priority(irq, prio, flags);
}
#endif /* !CONFIG_ARM_CUSTOM_INTERRUPT_CONTROLLER */

#ifdef CONFIG_DYNAMIC_INTERRUPTS
/**
 * @brief Connect a dynamic interrupt
 *
 * @param irq The interrupt number
 * @param priority The priority of the interrupt
 * @param routine The ISR routine to connect
 * @param parameter The parameter to pass to the ISR
 * @param flags Additional flags for the interrupt
 * @return The interrupt number
 */
int arch_irq_connect_dynamic(unsigned int irq, unsigned int priority,
			     void (*routine)(const void *parameter),
			     const void *parameter, uint32_t flags)
{
	z_isr_install(irq, routine, parameter);
	z_arm64_irq_priority_set(irq, priority, flags);
	return irq;
}
#endif

/**
 * @brief Handle spurious interrupts
 *
 * @param unused Unused parameter
 */
void z_irq_spurious(const void *unused)
{
	ARG_UNUSED(unused);

	z_arm64_fatal_error(K_ERR_SPURIOUS_IRQ, NULL);
}
//GST