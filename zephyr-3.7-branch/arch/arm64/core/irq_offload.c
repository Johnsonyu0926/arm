//arch/arc/core/irq_offload.c
/**
 * @file
 * @brief Software interrupts utility code - ARM64 implementation
 */

#include <zephyr/kernel.h>
#include <zephyr/irq_offload.h>
#include <exception.h>

/**
 * @brief Offload an interrupt to a specific routine
 *
 * This function uses a software interrupt (SVC) to offload an interrupt to a
 * specific routine.
 *
 * @param routine The routine to execute in the interrupt context
 * @param parameter The parameter to pass to the routine
 */
void arch_irq_offload(irq_offload_routine_t routine, const void *parameter)
{
	register const void *x0 __asm__("x0") = routine;
	register const void *x1 __asm__("x1") = parameter;

	__asm__ volatile ("svc %[svid]"
			  :
			  : [svid] "i" (_SVC_CALL_IRQ_OFFLOAD),
			    "r" (x0), "r" (x1));
}
//gst
