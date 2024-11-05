typedef uint16_t lws_led_intensity_t;
typedef uint16_t lws_led_seq_phase_t;

#define LWS_LED_MAX_INTENSITY			(0xffff)
#define LWS_LED_FUNC_PHASE			65536
#define LWS_SEQ_LEDPHASE_TOTAL_ENDLESS		(-1)
#define LWS_LED_SEQUENCER_UPDATE_INTERVAL_MS	33

struct lws_led_state;
struct lws_pwm_ops;

typedef lws_led_intensity_t (*lws_led_lookup_t)(lws_led_seq_phase_t ph);

typedef struct lws_led_sequence_def_t {
	lws_led_lookup_t		func;
	lws_led_seq_phase_t		ledphase_offset;
	int				ledphase_total;
	uint16_t			ms;
	uint8_t				flags;
} lws_led_sequence_def_t;

enum {
	LLSI_CURR,
	LLSI_NEXT,
	LLSI_TRANS
};

typedef struct lws_led_state_ch {
	const lws_led_sequence_def_t		*seq;
	lws_led_seq_phase_t			ph;
	lws_led_seq_phase_t			step;
	int					phase_budget;
	lws_led_intensity_t			last;
} lws_led_state_ch_t;

typedef struct lws_led_state_chs {
	lws_led_state_ch_t			seqs[3];
} lws_led_state_chs_t;

typedef struct lws_led_ops {
	void (*intensity)(const struct lws_led_ops *lo, const char *name, lws_led_intensity_t inten);
	struct lws_led_state * (*create)(const struct lws_led_ops *led_ops);
	void (*destroy)(struct lws_led_state *);
} lws_led_ops_t;

typedef struct lws_led_gpio_map {
	const char			*name;
	_lws_plat_gpio_t		gpio;
	lws_led_lookup_t		intensity_correction;
	const struct lws_pwm_ops	*pwm_ops;
	uint8_t				active_level;
} lws_led_gpio_map_t;

typedef struct lws_led_gpio_controller {
	const lws_led_ops_t		led_ops;
	const lws_gpio_ops_t		*gpio_ops;
	const lws_led_gpio_map_t	*led_map;
	uint8_t				count_leds;
} lws_led_gpio_controller_t;

LWS_VISIBLE LWS_EXTERN struct lws_led_state * lws_led_gpio_create(const lws_led_ops_t *led_ops);
LWS_VISIBLE LWS_EXTERN void lws_led_gpio_destroy(struct lws_led_state *lcs);
LWS_VISIBLE LWS_EXTERN void lws_led_gpio_intensity(const struct lws_led_ops *lo, const char *name, lws_led_intensity_t inten);
LWS_VISIBLE LWS_EXTERN int lws_led_transition(struct lws_led_state *lcs, const char *name, const lws_led_sequence_def_t *next, const lws_led_sequence_def_t *trans);

#define lws_led_gpio_ops \
	{ \
		.create		= lws_led_gpio_create, \
		.destroy	= lws_led_gpio_destroy, \
		.intensity	= lws_led_gpio_intensity, \
	}
// By GST @Date