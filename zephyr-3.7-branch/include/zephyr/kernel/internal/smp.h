// zephyr-3.7-branch/include/zephyr/kernel/internal/smp.h

#ifndef ZEPHYR_INCLUDE_KERNEL_INTERNAL_SMP_H_
#define ZEPHYR_INCLUDE_KERNEL_INTERNAL_SMP_H_

/**
 * @brief Trigger a scheduler IPI (Inter-Processor Interrupt).
 *
 * This function is used to send an IPI to other processors in the system
 * to trigger a scheduling operation.
 */
void z_sched_ipi(void);

#endif /* ZEPHYR_INCLUDE_KERNEL_INTERNAL_SMP_H_ */
//GST