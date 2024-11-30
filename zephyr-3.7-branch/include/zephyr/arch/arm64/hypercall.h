// zephyr-3.7-branch/include/zephyr/arch/arm64/hypercall.h

#ifndef ZEPHYR_INCLUDE_ARCH_ARM64_HYPERCALL_H_
#define ZEPHYR_INCLUDE_ARCH_ARM64_HYPERCALL_H_

/**
 * @brief Hypervisor console I/O operation
 *
 * This function performs a console I/O operation in the hypervisor.
 *
 * @param op The operation code
 * @param cnt The count of characters
 * @param str The string to be written
 * @return The result of the hypercall
 */
int HYPERVISOR_console_io(int op, int cnt, char *str);

/**
 * @brief Hypervisor scheduler operation
 *
 * This function performs a scheduler operation in the hypervisor.
 *
 * @param op The operation code
 * @param param The parameter for the operation
 * @return The result of the hypercall
 */
int HYPERVISOR_sched_op(int op, void *param);

/**
 * @brief Hypervisor event channel operation
 *
 * This function performs an event channel operation in the hypervisor.
 *
 * @param op The operation code
 * @param param The parameter for the operation
 * @return The result of the hypercall
 */
int HYPERVISOR_event_channel_op(int op, void *param);

/**
 * @brief Hypervisor HVM operation
 *
 * This function performs an HVM operation in the hypervisor.
 *
 * @param op The operation code
 * @param param The parameter for the operation
 * @return The result of the hypercall
 */
int HYPERVISOR_hvm_op(int op, void *param);

/**
 * @brief Hypervisor memory operation
 *
 * This function performs a memory operation in the hypervisor.
 *
 * @param op The operation code
 * @param param The parameter for the operation
 * @return The result of the hypercall
 */
int HYPERVISOR_memory_op(int op, void *param);

/**
 * @brief Hypervisor grant table operation
 *
 * This function performs a grant table operation in the hypervisor.
 *
 * @param op The operation code
 * @param uop The user operation
 * @param count The count of operations
 * @return The result of the hypercall
 */
int HYPERVISOR_grant_table_op(int op, void *uop, unsigned int count);

#ifdef CONFIG_XEN_DOM0
/**
 * @brief Hypervisor domain control operation
 *
 * This function performs a domain control operation in the hypervisor.
 *
 * @param param The parameter for the operation
 * @return The result of the hypercall
 */
int HYPERVISOR_domctl(void *param);
#endif

#endif /* ZEPHYR_INCLUDE_ARCH_ARM64_HYPERCALL_H_ */
//GST