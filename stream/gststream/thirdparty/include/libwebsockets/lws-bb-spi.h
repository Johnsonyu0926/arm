#define LWSBBSPI_FLAG_USE_NCMD3     (1 << 7)
#define LWSBBSPI_FLAG_USE_NCMD2     (1 << 6)
#define LWSBBSPI_FLAG_USE_NCMD1     (1 << 5)
#define LWSBBSPI_FLAG_USE_NCMD0     (1 << 4)
#define LWSBBSPI_FLAG_USE_NCS3      (1 << 3)
#define LWSBBSPI_FLAG_USE_NCS2      (1 << 2)
#define LWSBBSPI_FLAG_USE_NCS1      (1 << 1)
#define LWSBBSPI_FLAG_USE_NCS0      (1 << 0)

#define LWS_SPI_BB_MAX_CH       4

typedef struct lws_bb_spi {
    lws_spi_ops_t       bb_ops; /* init to lws_bb_spi_ops */

    /* implementation-specific members */
    const lws_gpio_ops_t    *gpio;

    _lws_plat_gpio_t    clk;
    _lws_plat_gpio_t    ncs[LWS_SPI_BB_MAX_CH];
    _lws_plat_gpio_t    ncmd[LWS_SPI_BB_MAX_CH];
    _lws_plat_gpio_t    mosi;
    _lws_plat_gpio_t    miso;

    uint8_t         flags;
} lws_bb_spi_t;

#define lws_bb_spi_ops \
        .init       = lws_bb_spi_init, \
        .queue      = lws_bb_spi_queue

int
lws_bb_spi_init(const lws_spi_ops_t *octx);

int
lws_bb_spi_queue(const lws_spi_ops_t *octx, const lws_spi_desc_t *desc);

// By GST @Date