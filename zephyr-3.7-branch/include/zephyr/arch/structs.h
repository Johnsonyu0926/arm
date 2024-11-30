// zephyr-3.7-branch/include/zephyr/arch/structs.h

#ifndef ZEPHYR_INCLUDE_ARCH_STRUCTS_H_
#define ZEPHYR_INCLUDE_ARCH_STRUCTS_H_

#if !defined(_ASMLANGUAGE)

#if defined(CONFIG_ARM64)
#include <zephyr/arch/arm64/structs.h>
#elif defined(CONFIG_RISCV)
#include <zephyr/arch/riscv/structs.h>
#elif defined(CONFIG_ARM)
#include <zephyr/arch/arm/structs.h>
#else

/* Default definitions when no architecture specific definitions exist. */

/**
 * @brief Per CPU architecture specifics (empty)
 */
struct _cpu_arch {
#ifdef __cplusplus
	/* This struct will have a size 0 in C which is not allowed in C++ (it'll have a size 1). To
	 * prevent this, we add a 1 byte dummy variable.
	 */
	uint8_t dummy;
#endif
};

#endif

/* typedefs to be used with GEN_OFFSET_SYM(), etc. */
typedef struct _cpu_arch _cpu_arch_t;

#endif /* _ASMLANGUAGE */

#endif /* ZEPHYR_INCLUDE_ARCH_STRUCTS_H_ */
//GST