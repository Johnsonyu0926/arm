typedef struct lws_bb_i2c {
    lws_i2c_ops_t       bb_ops; /* init to lws_bb_i2c_ops */

    /* implementation-specific members */

    _lws_plat_gpio_t    scl;
    _lws_plat_gpio_t    sda;

    const lws_gpio_ops_t    *gpio;
    void (*delay)(void);
} lws_bb_i2c_t;

#define lws_bb_i2c_ops \
    { \
        .init = lws_bb_i2c_init, \
        .start = lws_bb_i2c_start, \
        .stop = lws_bb_i2c_stop, \
        .write = lws_bb_i2c_write, \
        .read = lws_bb_i2c_read, \
        .set_ack = lws_bb_i2c_set_ack, \
    }

int
lws_bb_i2c_init(const lws_i2c_ops_t *octx);

int
lws_bb_i2c_start(const lws_i2c_ops_t *octx);

void
lws_bb_i2c_stop(const lws_i2c_ops_t *octx);

int
lws_bb_i2c_write(const lws_i2c_ops_t *octx, uint8_t data);

int
lws_bb_i2c_read(const lws_i2c_ops_t *octx);

void
lws_bb_i2c_set_ack(const lws_i2c_ops_t *octx, int ack);

// By GST @Date