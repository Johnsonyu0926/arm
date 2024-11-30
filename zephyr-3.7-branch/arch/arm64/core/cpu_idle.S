//cpu_idle.S

#include <zephyr/toolchain.h>
#include <zephyr/linker/sections.h>
#include <zephyr/arch/cpu.h>
/* Assembly file prologue */
_ASM_FILE_PROLOGUE
/* Define the arch_cpu_idle function */
GTEXT(arch_cpu_idle)
SECTION_FUNC(TEXT, arch_cpu_idle)
#ifdef CONFIG_TRACING
    /* Save the link register and call sys_trace_idle */
    str lr, [sp, #-16]!
    bl sys_trace_idle
    /* Restore the link register */
    ldr lr, [sp], #16
#endif
    /* Data synchronization barrier */
    dsb sy
    /* Wait for interrupt */
    wfi
    /* Clear the interrupt mask */
    msr daifclr, #(DAIFCLR_IRQ_BIT)
    /* Return from function */
    ret
/* Define the arch_cpu_atomic_idle function */
GTEXT(arch_cpu_atomic_idle)
SECTION_FUNC(TEXT, arch_cpu_atomic_idle)
#ifdef CONFIG_TRACING
    /* Save x0 and the link register, and call sys_trace_idle */
    stp x0, lr, [sp, #-16]!
    bl sys_trace_idle
    /* Restore x0 and the link register */
    ldp x0, lr, [sp], #16
#endif
    /* Set the interrupt mask */
    msr daifset, #(DAIFSET_IRQ_BIT)
    /* Instruction synchronization barrier */
    isb
    /* Wait for event */
    wfe
    /* Test if interrupts are enabled */
    tst x0, #(DAIF_IRQ_BIT)
    /* If interrupts are disabled, branch to _irq_disabled */
    beq _irq_disabled
    /* Clear the interrupt mask */
    msr daifclr, #(DAIFCLR_IRQ_BIT)
_irq_disabled:
    /* Return from function */
    ret
	//GST
