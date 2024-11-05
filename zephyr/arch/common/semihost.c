// Filename: semihost.c
// 评分: 90

#include <zephyr/kernel.h>
#include <zephyr/init.h>
#include <zephyr/arch/cpu.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>

#define SEMIHOSTING_SYS_WRITE0 0x04

static inline void semihosting_call(uint32_t operation, void *arg)
{
    __asm__ volatile (
        "mov r0, %[op]\n"
        "mov r1, %[arg]\n"
        "bkpt 0xab\n"
        :
        : [op] "r" (operation), [arg] "r" (arg)
        : "r0", "r1", "memory"
    );
}

void semihost_write0(const char *str)
{
    semihosting_call(SEMIHOSTING_SYS_WRITE0, (void *)str);
}

static int semihost_init(const struct device *dev)
{
    ARG_UNUSED(dev);
    semihost_write0("Semihosting initialized\n");
    return 0;
}

SYS_INIT(semihost_init, PRE_KERNEL_1, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT);
// By GST @Date