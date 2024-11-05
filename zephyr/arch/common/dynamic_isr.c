// Filename: dynamic_isr.c
// 评分: 90

#include "sw_isr_common.h"
#include <zephyr/sw_isr_table.h>
#include <zephyr/irq.h>
#include <zephyr/sys/__assert.h>

void __weak z_isr_install(unsigned int irq, void (*routine)(const void *), const void *param)
{
    unsigned int table_idx;
#ifndef CONFIG_GIC
    __ASSERT(!irq_is_enabled(irq), "IRQ %d is enabled", irq);
#endif
    table_idx = z_get_sw_isr_table_idx(irq);
    _sw_isr_table[table_idx].arg = param;
    _sw_isr_table[table_idx].isr = routine;
}

int __weak arch_irq_connect_dynamic(unsigned int irq, unsigned int priority, void (*routine)(const void *), const void *parameter, uint32_t flags)
{
    ARG_UNUSED(flags);
    ARG_UNUSED(priority);
    z_isr_install(irq, routine, parameter);
    return irq;
}
// By GST @Date