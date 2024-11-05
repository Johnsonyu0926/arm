typedef struct lws_pwm_map {
    _lws_plat_gpio_t gpio;
    uint8_t index;
    uint8_t active_level;
} lws_pwm_map_t;

typedef struct lws_pwm_ops {
    int (*init)(const struct lws_pwm_ops *lo);
    void (*intensity)(const struct lws_pwm_ops *lo, _lws_plat_gpio_t gpio, lws_led_intensity_t inten);
    const lws_pwm_map_t *pwm_map;
    uint8_t count_pwm_map;
} lws_pwm_ops_t;

LWS_VISIBLE LWS_EXTERN int lws_pwm_plat_init(const struct lws_pwm_ops *lo);

LWS_VISIBLE LWS_EXTERN void lws_pwm_plat_intensity(const struct lws_pwm_ops *lo, _lws_plat_gpio_t gpio, lws_led_intensity_t inten);

#define lws_pwm_plat_ops \
    .init = lws_pwm_plat_init, \
    .intensity = lws_pwm_plat_intensity

LWS_VISIBLE LWS_EXTERN lws_led_intensity_t lws_led_func_linear(lws_led_seq_phase_t n);
LWS_VISIBLE LWS_EXTERN lws_led_intensity_t lws_led_func_sine(lws_led_seq_phase_t n);

extern const lws_led_sequence_def_t lws_pwmseq_sine_endless_slow, lws_pwmseq_sine_endless_fast, lws_pwmseq_linear_wipe, lws_pwmseq_sine_up, lws_pwmseq_sine_down, lws_pwmseq_static_on, lws_pwmseq_static_half, lws_pwmseq_static_off;
// By GST @Date