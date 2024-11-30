// zephyr-3.7-branch/include/zephyr/arch/arm64/error.h

#ifndef ZEPHYR_INCLUDE_ARCH_ARM64_ERROR_H_
#define ZEPHYR_INCLUDE_ARCH_ARM64_ERROR_H_

#include <zephyr/arch/arm64/syscall.h>
#include <zephyr/arch/arm64/exception.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Trigger an architecture-specific exception
 *
 * This macro triggers an architecture-specific exception with the given reason.
 *
 * @param reason_p The reason for the exception
 */
#define ARCH_EXCEPT(reason_p)                       \
do {                                                \
    register uint64_t x8 __asm__("x8") = reason_p;  \
                                                    \
    __asm__ volatile("svc %[id]\n"                  \
                     :                              \
                     : [id] "i" (_SVC_CALL_RUNTIME_EXCEPT), \
                        "r" (x8)                    \
                     : "memory");                   \
} while (false)

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ARCH_ARM64_ERROR_H_ */
//GST