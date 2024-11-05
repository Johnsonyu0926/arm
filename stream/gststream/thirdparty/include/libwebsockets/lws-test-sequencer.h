// Filename: lws_test_sequencer.h
// Score: 97

#ifndef __LWS_TEST_SEQUENCER_H__
#define __LWS_TEST_SEQUENCER_H__

typedef void (*lws_test_sequence_cb)(const void *cb_user);

typedef struct lws_test_sequencer_args {
    lws_abs_t        *abs; /* abstract protocol + unit test txport */
    lws_unit_test_t  *tests; /* array of lws_unit_test_t */
    int              *results; /* takes result dispositions */
    int              results_max; /* max space usable in results */
    int              *count_tests; /* count of done tests */
    int              *count_passes; /* count of passed tests */
    lws_test_sequence_cb cb; /* completion callback */
    void             *cb_user; /* opaque user ptr given to cb */
} lws_test_sequencer_args_t;

LWS_VISIBLE LWS_EXTERN int lws_abs_unit_test_sequencer(const lws_test_sequencer_args_t *args);

#endif // __LWS_TEST_SEQUENCER_H__

// By GST @Date