// Filename: lws_spi.h
// Score: 95

#ifndef __LWS_SPI_H__
#define __LWS_SPI_H__

#include <stdint.h>
#include <stddef.h>

typedef int (*lws_spi_cb_t)(void *opaque);

enum {
    LWSSPIMODE_CPOL                 = (1 << 0),
    LWSSPIMODE_CPHA                 = (1 << 1),

    LWS_SPI_BUSMODE_CLK_IDLE_LOW_SAMP_RISING    = 0,
    LWS_SPI_BUSMODE_CLK_IDLE_HIGH_SAMP_RISING   = LWSSPIMODE_CPOL,
    LWS_SPI_BUSMODE_CLK_IDLE_LOW_SAMP_FALLING   = LWSSPIMODE_CPHA,
    LWS_SPI_BUSMODE_CLK_IDLE_HIGH_SAMP_FALLING  = LWSSPIMODE_CPHA | LWSSPIMODE_CPOL,

    LWS_SPI_TXN_HALF_DUPLEX_DISCRETE    = 0,
    /**< separate MISO and MOSI, but only either MISO or MOSI has data at
     * one time... i2c style in SPI */
};

typedef struct lws_spi_desc {
    const uint8_t        *src;
    const uint8_t        *data;
    uint8_t              *dest;
    void                 *opaque;
    lws_spi_cb_t         completion_cb;
    uint16_t             count_cmd;
    uint16_t             count_write;
    uint16_t             count_read;
    uint8_t              txn_type;
    uint8_t              channel;
} lws_spi_desc_t;

typedef struct lws_spi_ops {
    int  (*init)(const struct lws_spi_ops *ctx);
    int  (*queue)(const struct lws_spi_ops *ctx, const lws_spi_desc_t *desc);
    uint8_t bus_mode;
} lws_spi_ops_t;

#endif // __LWS_SPI_H__

// By GST @Date