#if !defined(__LWS_GPIO_H__)
#define __LWS_GPIO_H__

typedef int _lws_plat_gpio_t;

typedef enum {
    LWSGGPIO_IRQ_NONE,
    LWSGGPIO_IRQ_RISING,
    LWSGGPIO_IRQ_FALLING,
    LWSGGPIO_IRQ_CHANGE,
    LWSGGPIO_IRQ_LOW,
    LWSGGPIO_IRQ_HIGH
} lws_gpio_irq_t;

enum {
    LWSGGPIO_FL_READ            = (1 << 0),
    LWSGGPIO_FL_WRITE           = (1 << 1),
    LWSGGPIO_FL_PULLUP          = (1 << 2),
    LWSGGPIO_FL_PULLDOWN        = (1 << 3),
    LWSGGPIO_FL_START_LOW       = (1 << 4),
};

typedef void (*lws_gpio_irq_cb_t)(void *arg);

typedef struct lws_gpio_ops {
    void (*mode)(_lws_plat_gpio_t gpio, int flags);
    int (*read)(_lws_plat_gpio_t gpio);
    void (*set)(_lws_plat_gpio_t gpio, int val);
    int (*irq_mode)(_lws_plat_gpio_t gpio, lws_gpio_irq_t irq,
            lws_gpio_irq_cb_t cb, void *arg);
} lws_gpio_ops_t;

#endif
// By GST @Date