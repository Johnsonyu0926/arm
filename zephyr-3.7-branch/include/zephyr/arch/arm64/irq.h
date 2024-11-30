// zephyr-3.7-branch/include/zephyr/arch/arm64/irq.h

#ifndef ZEPHYR_INCLUDE_ARCH_ARM64_IRQ_H_
#define ZEPHYR_INCLUDE_ARCH_ARM64_IRQ_H_

#include <zephyr/irq.h>
#include <zephyr/sw_isr_table.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _ASMLANGUAGE
GTEXT(arch_irq_enable)
GTEXT(arch_irq_disable)
GTEXT(arch_irq_is_enabled)
#if defined(CONFIG_ARM_CUSTOM_INTERRUPT_CONTROLLER)
GTEXT(z_soc_irq_get_active)
GTEXT(z_soc_irq_eoi)
#endif /* CONFIG_ARM_CUSTOM_INTERRUPT_CONTROLLER */
#else

#if !defined(CONFIG_ARM_CUSTOM_INTERRUPT_CONTROLLER)

/**
 * @brief Enable an IRQ
 *
 * This function enables the specified IRQ.
 *
 * @param irq The IRQ to enable
 */
extern void arch_irq_enable(unsigned int irq);

/**
 * @brief Disable an IRQ
 *
 * This function disables the specified IRQ.
 *
 * @param irq The IRQ to disable
 */
extern void arch_irq_disable(unsigned int irq);

/**
 * @brief Check if an IRQ is enabled
 *
 * This function checks if the specified IRQ is enabled.
 *
 * @param irq The IRQ to check
 * @return 1 if the IRQ is enabled, 0 otherwise
 */
extern int arch_irq_is_enabled(unsigned int irq);

/**
 * @brief Set the priority of an IRQ
 *
 * This function sets the priority of the specified IRQ.
 *
 * @param irq The IRQ to set the priority for
 * @param prio The priority to set
 * @param flags Additional flags for the IRQ
 */
extern void z_arm64_irq_priority_set(unsigned int irq, unsigned int prio, uint32_t flags);

#else

/*
 * When a custom interrupt controller is specified, map the architecture
 * interrupt control functions to the SoC layer interrupt control functions.
 */

/**
 * @brief Initialize the SoC IRQs
 *
 * This function initializes the SoC IRQs.
 */
void z_soc_irq_init(void);

/**
 * @brief Enable an IRQ
 *
 * This function enables the specified IRQ.
 *
 * @param irq The IRQ to enable
 */
void z_soc_irq_enable(unsigned int irq);

/**
 * @brief Disable an IRQ
 *
 * This function disables the specified IRQ.
 *
 * @param irq The IRQ to disable
 */
void z_soc_irq_disable(unsigned int irq);

/**
 * @brief Check if an IRQ is enabled
 *
 * This function checks if the specified IRQ is enabled.
 *
 * @param irq The IRQ to check
 * @return 1 if the IRQ is enabled, 0 otherwise
 */
int z_soc_irq_is_enabled(unsigned int irq);

/**
 * @brief Set the priority of an IRQ
 *
 * This function sets the priority of the specified IRQ.
 *
 * @param irq The IRQ to set the priority for
 * @param prio The priority to set
 * @param flags Additional flags for the IRQ
 */
void z_soc_irq_priority_set(unsigned int irq, unsigned int prio, unsigned int flags);

/**
 * @brief Get the active IRQ
 *
 * This function returns the active IRQ.
 *
 * @return The active IRQ
 */
unsigned int z_soc_irq_get_active(void);

/**
 * @brief End of interrupt
 *
 * This function signals the end of the specified IRQ.
 *
 * @param irq The IRQ to signal the end of
 */
void z_soc_irq_eoi(unsigned int irq);

#define arch_irq_enable(irq)        z_soc_irq_enable(irq)
#define arch_irq_disable(irq)       z_soc_irq_disable(irq)
#define arch_irq_is_enabled(irq)    z_soc_irq_is_enabled(irq)

#define z_arm64_irq_priority_set(irq, prio, flags) \
    z_soc_irq_priority_set(irq, prio, flags)

#endif /* !CONFIG_ARM_CUSTOM_INTERRUPT_CONTROLLER */

/**
 * @brief Initialize the ARM64 interrupts
 *
 * This function initializes the ARM64 interrupts.
 */
extern void z_arm64_interrupt_init(void);

/**
 * @brief Connect an IRQ
 *
 * This macro connects an IRQ to an ISR with the specified priority and flags.
 *
 * @param irq_p The IRQ to connect
 * @param priority_p The priority of the IRQ
 * @param isr_p The ISR to connect to the IRQ
 * @param isr_param_p The parameter to pass to the ISR
 * @param flags_p Additional flags for the IRQ
 */
#define ARCH_IRQ_CONNECT(irq_p, priority_p, isr_p, isr_param_p, flags_p) \
{ \
    Z_ISR_DECLARE(irq_p, 0, isr_p, isr_param_p); \
    z_arm64_irq_priority_set(irq_p, priority_p, flags_p); \
}

#endif /* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ARCH_ARM64_IRQ_H_ */
//GST