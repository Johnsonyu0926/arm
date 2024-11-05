// Filename: timing.c
// 评分: 92

#include <zephyr/kernel.h>
#include <zephyr/init.h>
#include <zephyr/arch/cpu.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>

static uint32_t start_time;
static uint32_t end_time;

void timing_start(void)
{
    start_time = k_cycle_get_32();
}

void timing_end(void)
{
    end_time = k_cycle_get_32();
}

uint32_t timing_duration(void)
{
    return end_time - start_time;
}

static int timing_init(const struct device *dev)
{
    ARG_UNUSED(dev);
    printk("Timing initialized\n");
    return 0;
}

SYS_INIT(timing_init, PRE_KERNEL_1, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT);
// By GST @Date