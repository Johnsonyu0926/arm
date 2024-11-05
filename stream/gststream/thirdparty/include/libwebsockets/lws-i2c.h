#if !defined(__LWS_I2C_H__)
#define __LWS_I2C_H__

#include <stdint.h>
#include <stddef.h>

typedef struct lws_i2c_ops {
    int  (*init)(const struct lws_i2c_ops *ctx);
    int  (*start)(const struct lws_i2c_ops *ctx);
    void (*stop)(const struct lws_i2c_ops *ctx);
    int  (*write)(const struct lws_i2c_ops *ctx, uint8_t data);
    int  (*read)(const struct lws_i2c_ops *ctx);
    void (*set_ack)(const struct lws_i2c_ops *octx, int ack);
} lws_i2c_ops_t;

/*
 * These are implemented by calling the ops above, and so are generic
 */

LWS_VISIBLE LWS_EXTERN int
lws_i2c_command(const lws_i2c_ops_t *ctx, uint8_t ads7, uint8_t c);

LWS_VISIBLE LWS_EXTERN int
lws_i2c_command_list(const lws_i2c_ops_t *ctx, uint8_t ads7, const uint8_t *buf,
             size_t len);

#endif
// By GST @Date