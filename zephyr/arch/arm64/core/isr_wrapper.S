/*
 * Copyright (c) 2019 Carlo Caione <ccaione@baylibre.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * ARM64 Cortex-A ISRs wrapper
 */

#include <zephyr/toolchain.h>
#include <zephyr/linker/sections.h>
#include <offsets_short.h>
#include <zephyr/arch/cpu.h>
#include <zephyr/sw_isr_table.h>
#include <zephyr/drivers/interrupt_controller/gic.h>
#include "macro_priv.inc"

_ASM_FILE_PROLOGUE

GDATA(_sw_isr_table)

/*
 * Wrapper around ISRs when inserted in software ISR table
 *
 * When inserted in the vector table, _isr_wrapper() demuxes the ISR table
 * using the running interrupt number as the index, and invokes the registered
 * ISR with its corresponding argument. When returning from the ISR, it
 * determines if a context switch needs to happen.
 */

GTEXT(_isr_wrapper)
SECTION_FUNC(TEXT, _isr_wrapper)

    /* ++_current_cpu->nested to be checked by arch_is_in_isr() */
    get_cpu    x0
    ldr    w1, [x0, #___cpu_t_nested_OFFSET]
    add    w2, w1, #1
    str    w2, [x0, #___cpu_t_nested_OFFSET]

    /* If not nested: switch to IRQ stack and save current sp on it. */
    cbnz    w1, 1f
    ldr    x1, [x0, #___cpu_t_irq_stack_OFFSET]
    mov    x2, sp
    mov    sp, x1
    str    x2, [sp, #-16]!
#if defined(CONFIG_ARM64_SAFE_EXCEPTION_STACK)
    sub    x1, x1, #CONFIG_ISR_STACK_SIZE
    str    x1, [x0, #_cpu_offset_to_current_stack_limit]
#endif
1:
#ifdef CONFIG_SCHED_THREAD_USAGE
    bl    z_sched_usage_stop
#endif

#ifdef CONFIG_TRACING
    bl    sys_trace_isr_enter
#endif

    /* Get active IRQ number from the GIC */
    mrs x0, ICC_IAR1_EL1
    and x0, x0, #0x3ff

    /* Get ISR table entry */
    ldr x1, =_sw_isr_table
    add x1, x1, x0, lsl #3

    /* Load ISR and parameter */
    ldr x2, [x1]
    ldr x3, [x1, #8]

    /* Call ISR */
    blr x2

#ifdef CONFIG_TRACING
    bl sys_trace_isr_exit
#endif

#ifdef CONFIG_SCHED_THREAD_USAGE
    bl z_sched_usage_start
#endif

    /* --_current_cpu->nested */
    get_cpu x0
    ldr w1, [x0, #___cpu_t_nested_OFFSET]
    sub w1, w1, #1
    str w1, [x0, #___cpu_t_nested_OFFSET]

    /* If not nested: restore sp from IRQ stack */
    cbnz w1, 1f
    ldr x1, [sp], #16
    mov sp, x1
1:
    /* Signal end of interrupt to the GIC */
    msr ICC_EOIR1_EL1, x0

    /* Check if context switch is needed */
    bl z_arm64_exit_exc

    ret

// By GST @Date