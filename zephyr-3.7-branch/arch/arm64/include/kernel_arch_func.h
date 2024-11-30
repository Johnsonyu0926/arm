//arch/arc/include/kernel_arch_func.h
#ifndef ZEPHYR_ARCH_ARM64_INCLUDE_KERNEL_ARCH_FUNC_H_
#define ZEPHYR_ARCH_ARM64_INCLUDE_KERNEL_ARCH_FUNC_H_

#include <kernel_arch_data.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _ASMLANGUAGE

/**
 * @brief Initialize the kernel architecture
 *
 * This function initializes the kernel architecture. It is called during
 * system initialization.
 */
static ALWAYS_INLINE void arch_kernel_init(void)
{
}

/**
 * @brief Switch context between threads
 *
 * This function switches the context from the current thread to the specified
 * new thread.
 *
 * @param switch_to Pointer to the new thread to switch to
 * @param switched_from Pointer to the current thread being switched from
 */
static inline void arch_switch(void *switch_to, void **switched_from)
{
	extern void z_arm64_context_switch(struct k_thread *new,
					   struct k_thread *old);
	struct k_thread *new = switch_to;
	struct k_thread *old = CONTAINER_OF(switched_from, struct k_thread,
					    switch_handle);

	z_arm64_context_switch(new, old);
}

/**
 * @brief Handle a fatal error
 *
 * This function handles a fatal error by performing necessary cleanup and
 * system shutdown.
 *
 * @param reason The reason for the fatal error
 * @param esf Pointer to the exception stack frame
 */
extern void z_arm64_fatal_error(unsigned int reason, struct arch_esf *esf);

/**
 * @brief Set the Translation Table Base Register 0 (TTBR0)
 *
 * This function sets the TTBR0 register to the specified value.
 *
 * @param ttbr0 The value to set in the TTBR0 register
 */
extern void z_arm64_set_ttbr0(uint64_t ttbr0);

/**
 * @brief Send a memory configuration IPI
 *
 * This function sends a memory configuration IPI to all CPUs.
 */
extern void z_arm64_mem_cfg_ipi(void);

#ifdef CONFIG_FPU_SHARING
/**
 * @brief Flush the local FPU
 *
 * This function flushes the local FPU state.
 */
void arch_flush_local_fpu(void);

/**
 * @brief Send an FPU flush IPI
 *
 * This function sends an FPU flush IPI to the specified CPU.
 *
 * @param cpu The CPU to send the FPU flush IPI to
 */
void arch_flush_fpu_ipi(unsigned int cpu);
#endif

#ifdef CONFIG_ARM64_SAFE_EXCEPTION_STACK
/**
 * @brief Initialize the safe exception stack
 *
 * This function initializes the safe exception stack for the current CPU.
 */
void z_arm64_safe_exception_stack_init(void);
#endif

#endif /* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_ARCH_ARM64_INCLUDE_KERNEL_ARCH_FUNC_H_ */
//GST