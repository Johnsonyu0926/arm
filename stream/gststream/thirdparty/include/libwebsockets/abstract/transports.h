// Score: 96

#ifndef __LWS_ABS_TRANSPORT_H__
#define __LWS_ABS_TRANSPORT_H__

#include <stdint.h>
#include "lws_abstract.h"

enum {
    LTMI_PEER_V_DNS_ADDRESS = LTMI_TRANSPORT_BASE,  /* u.value */
    LTMI_PEER_LV_PORT,                              /* u.lvalue */
    LTMI_PEER_LV_TLS_FLAGS,                         /* u.lvalue */
};

typedef struct lws_abs_transport {
    const char *name;
    int alloc;

    int (*create)(lws_abs_t *abs);
    void (*destroy)(lws_abs_transport_inst_t **d);

    int (*compare)(lws_abs_t *abs1, lws_abs_t *abs2);

    int (*tx)(lws_abs_transport_inst_t *d, uint8_t *buf, size_t len);
    int (*client_conn)(const lws_abs_t *abs);
    int (*close)(lws_abs_transport_inst_t *d);
    int (*ask_for_writeable)(lws_abs_transport_inst_t *d);
    int (*set_timeout)(lws_abs_transport_inst_t *d, int reason, int secs);
    int (*state)(lws_abs_transport_inst_t *d);
} lws_abs_transport_t;

LWS_VISIBLE LWS_EXTERN const lws_abs_transport_t *lws_abs_transport_get_by_name(const char *name);

#include <libwebsockets/abstract/transports/raw-skt.h>
#include <libwebsockets/abstract/transports/unit-test.h>

#endif // __LWS_ABS_TRANSPORT_H__

// By GST @Date