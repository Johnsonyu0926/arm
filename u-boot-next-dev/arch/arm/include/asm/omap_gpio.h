// Filename: omap_gpio.h

#ifndef _GPIO_H
#define _GPIO_H

#include <asm/arch/cpu.h>

#ifdef CONFIG_DM_GPIO

/* Information about a GPIO bank */
struct omap_gpio_platdata {
    int bank_index;
    ulong base;    /* address of registers in physical memory */
    const char *port_name;
};

#else

struct gpio_bank {
    void *base;
};

extern const struct gpio_bank *const omap_gpio_bank;

/**
 * Check if gpio is valid.
 *
 * @param gpio    GPIO number
 * @return 1 if ok, 0 on error
 */
int gpio_is_valid(int gpio);
#endif

#endif /* _GPIO_H */

// By GST @Date