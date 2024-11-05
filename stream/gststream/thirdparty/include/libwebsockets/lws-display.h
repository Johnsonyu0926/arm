#if !defined(__LWS_DISPLAY_H__)
#define __LWS_DISPLAY_H__

#include <stdint.h>

typedef uint16_t lws_display_scalar;

typedef struct lws_display {
    int (*init)(const struct lws_display *disp);
    const lws_pwm_ops_t *bl_pwm_ops;
    int (*contrast)(const struct lws_display *disp, uint8_t contrast);
    int (*blit)(const struct lws_display *disp, const uint8_t *src,
            lws_display_scalar x, lws_display_scalar y,
            lws_display_scalar w, lws_display_scalar h);
    int (*power)(const struct lws_display *disp, int state);

    const lws_led_sequence_def_t *bl_active;
    const lws_led_sequence_def_t *bl_dim;
    const lws_led_sequence_def_t *bl_transition;

    void *variant;

    int bl_index;

    lws_display_scalar w;
    /**< display surface width in pixels */
    lws_display_scalar h;
    /**< display surface height in pixels */

    uint8_t latency_wake_ms;
    /**< ms required after wake from sleep before display usable again...
     * delay bringing up the backlight for this amount of time on wake.
     * This is managed via a sul on the event loop, not blocking. */
} lws_display_t;

enum lws_display_controller_state {
    LWSDISPS_OFF,
    LWSDISPS_AUTODIMMED,
    LWSDISPS_BECOMING_ACTIVE,
    LWSDISPS_ACTIVE,
    LWSDISPS_GOING_OFF
};

typedef struct lws_display_state {
    lws_sorted_usec_list_t sul_autodim;
    const lws_display_t *disp;
    struct lws_context *ctx;

    int autodim_ms;
    int off_ms;

    struct lws_led_state *bl_lcs;

    lws_led_state_chs_t chs;
    /* set of sequencer transition channels */

    enum lws_display_controller_state state;

} lws_display_state_t;

/**
 * lws_display_state_init() - initialize display states
 *
 * \param lds: the display state object
 * \param ctx: the lws context
 * \param autodim_ms: ms since last active report to dim display (<0 = never)
 * \param off_ms: ms since dim to turn display off (<0 = never)
 * \param bl_lcs: the led controller instance that has the backlight
 * \param disp: generic display object we belong to
 *
 * This initializes a display's state, and sets up the optional screen auto-dim
 * and blanking on inactive, and gradual brightness change timer.
 *
 *  - auto-dim then off: set autodim to some ms and off_ms to some ms
 *  - auto-dim only: set autodim to some ms and off_ms to -1
 *  - off-only: set autodim to some ms and off_ms to 0
 *  - neither: set both autodim and off_ms to -1
 */
LWS_VISIBLE LWS_EXTERN void
lws_display_state_init(lws_display_state_t *lds, struct lws_context *ctx,
               int autodim_ms, int off_ms, struct lws_led_state *bl_lcs,
               const lws_display_t *disp);

/**
 * lws_display_state_set_brightness() - gradually change the brightness
 *
 * \param lds: the display state we are changing
 * \param target: the target brightness to transition to
 *
 * Adjusts the brightness gradually towards the target at 20Hz
 */
LWS_VISIBLE LWS_EXTERN void
lws_display_state_set_brightness(lws_display_state_t *lds,
                 const lws_led_sequence_def_t *pwmseq);

/**
 * lws_display_state_active() - inform the system the display is active
 *
 * \param lds: the display state we are marking as active
 *
 * Resets the auto-dim and auto-off timers and makes sure the display is on and
 * at the active brightness level
 */
LWS_VISIBLE LWS_EXTERN void
lws_display_state_active(lws_display_state_t *lds);

/**
 * lws_display_state_off() - turns off the related display
 *
 * \param lds: the display state we are turning off
 *
 * Turns the display to least power mode or completely off if possible.
 * Disables the timers related to dimming and blanking.
 */
LWS_VISIBLE LWS_EXTERN void
lws_display_state_off(lws_display_state_t *lds);

#endif
// By GST @Date