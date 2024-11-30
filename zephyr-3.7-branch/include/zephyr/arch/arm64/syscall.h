// zephyr-3.7-branch/include/zephyr/arch/arm64/syscall.h

#ifndef ZEPHYR_INCLUDE_ARCH_ARM64_SYSCALL_H_
#define ZEPHYR_INCLUDE_ARCH_ARM64_SYSCALL_H_

#define _SVC_CALL_IRQ_OFFLOAD        1
#define _SVC_CALL_RUNTIME_EXCEPT     2
#define _SVC_CALL_SYSTEM_CALL        3

#ifdef CONFIG_USERSPACE
#ifndef _ASMLANGUAGE

#include <zephyr/types.h>
#include <stdbool.h>
#include <zephyr/arch/arm64/lib_helpers.h>
#include <zephyr/arch/arm64/tpidrro_el0.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Syscall invocation macros. arm-specific machine constraints used to ensure
 * args land in the proper registers.
 */

/**
 * @brief Invoke a syscall with 6 arguments
 *
 * This function invokes a syscall with 6 arguments.
 *
 * @param arg1 The first argument
 * @param arg2 The second argument
 * @param arg3 The third argument
 * @param arg4 The fourth argument
 * @param arg5 The fifth argument
 * @param arg6 The sixth argument
 * @param call_id The syscall ID
 * @return The result of the syscall
 */
static inline uintptr_t arch_syscall_invoke6(uintptr_t arg1, uintptr_t arg2,
                                             uintptr_t arg3, uintptr_t arg4,
                                             uintptr_t arg5, uintptr_t arg6,
                                             uintptr_t call_id)
{
    register uint64_t ret __asm__("x0") = arg1;
    register uint64_t r1 __asm__("x1") = arg2;
    register uint64_t r2 __asm__("x2") = arg3;
    register uint64_t r3 __asm__("x3") = arg4;
    register uint64_t r4 __asm__("x4") = arg5;
    register uint64_t r5 __asm__("x5") = arg6;
    register uint64_t r8 __asm__("x8") = call_id;

    __asm__ volatile("svc %[svid]\n"
                     : "=r"(ret)
                     : [svid] "i" (_SVC_CALL_SYSTEM_CALL),
                       "r" (ret), "r" (r1), "r" (r2), "r" (r3),
                       "r" (r4), "r" (r5), "r" (r8)
                     : "memory");

    return ret;
}

/**
 * @brief Invoke a syscall with 5 arguments
 *
 * This function invokes a syscall with 5 arguments.
 *
 * @param arg1 The first argument
 * @param arg2 The second argument
 * @param arg3 The third argument
 * @param arg4 The fourth argument
 * @param arg5 The fifth argument
 * @param call_id The syscall ID
 * @return The result of the syscall
 */
static inline uintptr_t arch_syscall_invoke5(uintptr_t arg1, uintptr_t arg2,
                                             uintptr_t arg3, uintptr_t arg4,
                                             uintptr_t arg5,
                                             uintptr_t call_id)
{
    register uint64_t ret __asm__("x0") = arg1;
    register uint64_t r1 __asm__("x1") = arg2;
    register uint64_t r2 __asm__("x2") = arg3;
    register uint64_t r3 __asm__("x3") = arg4;
    register uint64_t r4 __asm__("x4") = arg5;
    register uint64_t r8 __asm__("x8") = call_id;

    __asm__ volatile("svc %[svid]\n"
                     : "=r"(ret)
                     : [svid] "i" (_SVC_CALL_SYSTEM_CALL),
                       "r" (ret), "r" (r1), "r" (r2), "r" (r3),
                       "r" (r4), "r" (r8)
                     : "memory");

    return ret;
}

/**
 * @brief Invoke a syscall with 4 arguments
 *
 * This function invokes a syscall with 4 arguments.
 *
 * @param arg1 The first argument
 * @param arg2 The second argument
 * @param arg3 The third argument
 * @param arg4 The fourth argument
 * @param call_id The syscall ID
 * @return The result of the syscall
 */
static inline uintptr_t arch_syscall_invoke4(uintptr_t arg1, uintptr_t arg2,
                                             uintptr_t arg3, uintptr_t arg4,
                                             uintptr_t call_id)
{
    register uint64_t ret __asm__("x0") = arg1;
    register uint64_t r1 __asm__("x1") = arg2;
    register uint64_t r2 __asm__("x2") = arg3;
    register uint64_t r3 __asm__("x3") = arg4;
    register uint64_t r8 __asm__("x8") = call_id;

    __asm__ volatile("svc %[svid]\n"
                     : "=r"(ret)
                     : [svid] "i" (_SVC_CALL_SYSTEM_CALL),
                       "r" (ret), "r" (r1), "r" (r2), "r" (r3),
                       "r" (r8)
                     : "memory");

    return ret;
}

/**
 * @brief Invoke a syscall with 3 arguments
 *
 * This function invokes a syscall with 3 arguments.
 *
 * @param arg1 The first argument
 * @param arg2 The second argument
 * @param arg3 The third argument
 * @param call_id The syscall ID
 * @return The result of the syscall
 */
static inline uintptr_t arch_syscall_invoke3(uintptr_t arg1, uintptr_t arg2,
                                             uintptr_t arg3,
                                             uintptr_t call_id)
{
    register uint64_t ret __asm__("x0") = arg1;
    register uint64_t r1 __asm__("x1") = arg2;
    register uint64_t r2 __asm__("x2") = arg3;
    register uint64_t r8 __asm__("x8") = call_id;

    __asm__ volatile("svc %[svid]\n"
                     : "=r"(ret)
                     : [svid] "i" (_SVC_CALL_SYSTEM_CALL),
                       "r" (ret), "r" (r1), "r" (r2), "r" (r8)
                     : "memory");

    return ret;
}

/**
 * @brief Invoke a syscall with 2 arguments
 *
 * This function invokes a syscall with 2 arguments.
 *
 * @param arg1 The first argument
 * @param arg2 The second argument
 * @param call_id The syscall ID
 * @return The result of the syscall
 */
static inline uintptr_t arch_syscall_invoke2(uintptr_t arg1, uintptr_t arg2,
                                             uintptr_t call_id)
{
    register uint64_t ret __asm__("x0") = arg1;
    register uint64_t r1 __asm__("x1") = arg2;
    register uint64_t r8 __asm__("x8") = call_id;

    __asm__ volatile("svc %[svid]\n"
                     : "=r"(ret)
                     : [svid] "i" (_SVC_CALL_SYSTEM_CALL),
                       "r" (ret), "r" (r1), "r" (r8)
                     : "memory");

    return ret;
}

/**
 * @brief Invoke a syscall with 1 argument
 *
 * This function invokes a syscall with 1 argument.
 *
 * @param arg1 The first argument
 * @param call_id The syscall ID
 * @return The result of the syscall
 */
static inline uintptr_t arch_syscall_invoke1(uintptr_t arg1,
                                             uintptr_t call_id)
{
    register uint64_t ret __asm__("x0") = arg1;
    register uint64_t r8 __asm__("x8") = call_id;

    __asm__ volatile("svc %[svid]\n"
                     : "=r"(ret)
                     : [svid] "i" (_SVC_CALL_SYSTEM_CALL),
                       "r" (ret), "r" (r8)
                     : "memory");
    return ret;
}

/**
 * @brief Invoke a syscall with no arguments
 *
 * This function invokes a syscall with no arguments.
 *
 * @param call_id The syscall ID
 * @return The result of the syscall
 */
static inline uintptr_t arch_syscall_invoke0(uintptr_t call_id)
{
    register uint64_t ret __asm__("x0");
    register uint64_t r8 __asm__("x8") = call_id;

    __asm__ volatile("svc %[svid]\n"
                     : "=r"(ret)
                     : [svid] "i" (_SVC_CALL_SYSTEM_CALL),
                       "r" (ret), "r" (r8)
                     : "memory");

    return ret;
}

/**
 * @brief Check if the current context is user context
 *
 * This function checks if the current context is user context.
 *
 * @return True if the current context is user context, false otherwise
 */
static inline bool arch_is_user_context(void)
{
    return (read_tpidrro_el0() & TPIDRROEL0_IN_EL0) != 0;
}

#ifdef __cplusplus
}
#endif

#endif /* _ASMLANGUAGE */
#endif /* CONFIG_USERSPACE */

#endif /* ZEPHYR_INCLUDE_ARCH_ARM64_SYSCALL_H_ */
//GST