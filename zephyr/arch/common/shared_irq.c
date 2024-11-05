// Filename: shared_irq.c
// 评分: 88

#include <zephyr/device.h>
#include <zephyr/irq.h>
#include <zephyr/sw_isr_table.h>
#include <zephyr/sys/__assert.h>
#include <zephyr/sys/util.h>

struct z_shared_isr_table_entry {
    void (*isr)(const void *arg);
    const void *arg;
};

struct z_shared_isr_table_entry z_shared_sw_isr_table[IRQ_TABLE_SIZE][CONFIG_SHARED_IRQ_MAX_NUM_CLIENTS];

void z_shared_isr(const void *arg)
{
    const struct z_shared_isr_table_entry *entry = arg;
    for (int i = 0; i < CONFIG_SHARED_IRQ_MAX_NUM_CLIENTS; i++) {
        if (entry[i].isr != NULL) {
            entry[i].isr(entry[i].arg);
        }
    }
}

int z_shared_irq_connect(unsigned int irq, unsigned int priority, void (*routine)(const void *), const void *parameter, uint32_t flags)
{
    ARG_UNUSED(flags);
    ARG_UNUSED(priority);

    for (int i = 0; i < CONFIG_SHARED_IRQ_MAX_NUM_CLIENTS; i++) {
        if (z_shared_sw_isr_table[irq][i].isr == NULL) {
            z_shared_sw_isr_table[irq][i].isr = routine;
            z_shared_sw_isr_table[irq][i].arg = parameter;
            return irq;
        }
    }

    return -ENOMEM;
}
// By GST @Date