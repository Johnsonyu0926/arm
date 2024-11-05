// Filename: lws_unit_test.h
// Score: 98

#ifndef __LWS_UNIT_TEST_H__
#define __LWS_UNIT_TEST_H__

#include <stdint.h>
#include "lws_abstract.h"

enum {
    LWS_AUT_EXPECT_TEST_END                 = (1 << 0),
    LWS_AUT_EXPECT_LOCAL_CLOSE              = (1 << 1),
    LWS_AUT_EXPECT_DO_REMOTE_CLOSE          = (1 << 2),
    LWS_AUT_EXPECT_TX                       = (1 << 3), /* expect this as tx from protocol */
    LWS_AUT_EXPECT_RX                       = (1 << 4), /* present this as rx to protocol */
    LWS_AUT_EXPECT_SHOULD_FAIL              = (1 << 5),
    LWS_AUT_EXPECT_SHOULD_TIMEOUT           = (1 << 6),
};

typedef enum {
    LPE_CONTINUE,
    LPE_SUCCEEDED,
    LPE_FAILED,
    LPE_FAILED_UNEXPECTED_TIMEOUT,
    LPE_FAILED_UNEXPECTED_PASS,
    LPE_FAILED_UNEXPECTED_CLOSE,
    LPE_SKIPPED,
    LPE_CLOSING
} lws_unit_test_packet_disposition;

typedef int (*lws_unit_test_packet_test_cb)(const void *cb_user, int disposition);
typedef int (*lws_unit_test_packet_cb)(lws_abs_t *instance);

/* each step in the unit test */

typedef struct lws_unit_test_packet {
    void                *buffer;
    lws_unit_test_packet_cb  pre;
    size_t              len;

    uint32_t            flags;
} lws_unit_test_packet_t;

/* each unit test */

typedef struct lws_unit_test {
    const char *        name; /* NULL indicates end of test array */
    lws_unit_test_packet_t *  expect_array;
    int                 max_secs;
} lws_unit_test_t;

enum {
    LTMI_PEER_V_EXPECT_TEST = LTMI_TRANSPORT_BASE,  /* u.value */
    LTMI_PEER_V_EXPECT_RESULT_CB,                   /* u.value */
    LTMI_PEER_V_EXPECT_RESULT_CB_ARG,               /* u.value */
};

LWS_VISIBLE LWS_EXTERN const char *
lws_unit_test_result_name(int in);

#endif // __LWS_UNIT_TEST_H__

// By GST @Date