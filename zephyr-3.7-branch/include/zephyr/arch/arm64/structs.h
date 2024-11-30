// zephyr-3.7-branch/include/zephyr/arch/arm64/structs.h

#ifndef ZEPHYR_INCLUDE_ARM64_STRUCTS_H_
#define ZEPHYR_INCLUDE_ARM64_STRUCTS_H_

/**
 * @brief Per CPU architecture specifics
 *
 * This structure holds architecture-specific information for each CPU.
 */
struct _cpu_arch {
#ifdef CONFIG_FPU_SHARING
	atomic_ptr_val_t fpu_owner; /**< Owner of the FPU */
#endif
#ifdef CONFIG_ARM64_SAFE_EXCEPTION_STACK
	uint64_t safe_exception_stack; /**< Safe exception stack */
	uint64_t current_stack_limit; /**< Current stack limit */
	/**< Saved the corrupted stack pointer when stack overflow, else 0 */
	uint64_t corrupted_sp;
#endif
};

#endif /* ZEPHYR_INCLUDE_ARM64_STRUCTS_H_ */
//GST