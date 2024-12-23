// Filename: stack.c

#include <common.h>

DECLARE_GLOBAL_DATA_PTR;

int arch_reserve_stacks(void)
{
#ifdef CONFIG_SPL_BUILD
    gd->start_addr_sp -= 128;    /* leave 32 words for abort-stack */
    gd->irq_sp = gd->start_addr_sp;
#else
    /* setup stack pointer for exceptions */
    gd->irq_sp = gd->start_addr_sp;

# if !defined(CONFIG_ARM64)
#if CONFIG_IS_ENABLED(IRQ)
#ifndef CONFIG_IRQ_STACK_SIZE
#define CONFIG_IRQ_STACK_SIZE    8192
#endif
    gd->start_addr_sp -= CONFIG_IRQ_STACK_SIZE;

#else
    /* leave 3 words for abort-stack, plus 1 for alignment */
    gd->start_addr_sp -= 16;
#endif
# endif
#endif

    return 0;
}

// By GST @Date