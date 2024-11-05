// Filename: lws_display_ssd1306_i2c.h
// Score: 96

#ifndef __LWS_DISPLAY_SSD1306_I2C_H__
#define __LWS_DISPLAY_SSD1306_I2C_H__

/*
 * D/C# pin on SSD1306 sets the I2C device ads
 * from these two options (7-bit address)
 */

#define SSD1306_I2C7_ADS1       0x3c
#define SSD1306_I2C7_ADS2       0x3d

typedef struct lws_display_ssd1306 {
    lws_display_t        disp; /* use lws_display_ssd1306_ops to set ops */
    const lws_i2c_ops_t  *i2c; /* i2c ops */
    const lws_gpio_ops_t *gpio; /* NULL or gpio ops */
    _lws_plat_gpio_t     reset_gpio; /* if gpio ops, nReset gpio # */
    uint8_t              i2c7_address; /* one of SSD1306_I2C7_ADS... */
} lws_display_ssd1306_t;

int lws_display_ssd1306_i2c_init(const struct lws_display *disp);
int lws_display_ssd1306_i2c_contrast(const struct lws_display *disp, uint8_t b);
int lws_display_ssd1306_i2c_blit(const struct lws_display *disp, const uint8_t *src,
                                 lws_display_scalar x, lws_display_scalar y,
                                 lws_display_scalar w, lws_display_scalar h);
int lws_display_ssd1306_i2c_power(const struct lws_display *disp, int state);

#define lws_display_ssd1306_ops \
    .init = lws_display_ssd1306_i2c_init, \
    .contrast = lws_display_ssd1306_i2c_contrast, \
    .blit = lws_display_ssd1306_i2c_blit, \
    .power = lws_display_ssd1306_i2c_power

#endif // __LWS_DISPLAY_SSD1306_I2C_H__

// By GST @Date