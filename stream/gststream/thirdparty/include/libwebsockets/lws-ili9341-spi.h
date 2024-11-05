#if !defined(__LWS_DISPLAY_ILI9341_SPI_H__)
#define __LWS_DISPLAY_ILI9341_SPI_H__

typedef struct lws_display_ili9341 {
    lws_display_t       disp; /* use lws_display_ili9341_ops to set */
    const lws_spi_ops_t *spi;         /* spi ops */
    const lws_gpio_ops_t    *gpio;        /* NULL or gpio ops */
    _lws_plat_gpio_t    reset_gpio;   /* if gpio ops, nReset gpio # */
    uint8_t         spi_index; /* cs index starting from 0 */
} lws_display_ili9341_t;

int
lws_display_ili9341_spi_init(const struct lws_display *disp);

int
lws_display_ili9341_spi_blit(const struct lws_display *disp, const uint8_t *src,
                             lws_display_scalar x, lws_display_scalar y,
                             lws_display_scalar w, lws_display_scalar h);

int
lws_display_ili9341_spi_power(const struct lws_display *disp, int state);

#define lws_display_ili9341_ops \
    .init = lws_display_ili9341_spi_init, \
    .blit = lws_display_ili9341_spi_blit, \
    .power = lws_display_ili9341_spi_power

#endif
// By GST @Date