// Score: 95

#ifndef __LWS_ABS_PROTOCOL_H__
#define __LWS_ABS_PROTOCOL_H__

#include <stdint.h>
#include "lws_abstract.h"

enum {
    LWS_AP_FLAG_PIPELINE_TRANSACTIONS = (1 << 0),
    LWS_AP_FLAG_MUXABLE_STREAM = (1 << 1),
};

typedef struct lws_abs_protocol {
    const char *name;
    int alloc;
    int flags;

    int (*create)(const struct lws_abs *ai);
    void (*destroy)(lws_abs_protocol_inst_t **d);
    int (*compare)(lws_abs_t *abs1, lws_abs_t *abs2);

    int (*accept)(lws_abs_protocol_inst_t *d);
    int (*rx)(lws_abs_protocol_inst_t *d, const uint8_t *b, size_t l);
    int (*writeable)(lws_abs_protocol_inst_t *d, size_t budget);
    int (*closed)(lws_abs_protocol_inst_t *d);
    int (*heartbeat)(lws_abs_protocol_inst_t *d);

    int (*child_bind)(lws_abs_t *abs);
} lws_abs_protocol_t;

LWS_VISIBLE LWS_EXTERN const lws_abs_protocol_t *lws_abs_protocol_get_by_name(const char *name);

#include <libwebsockets/abstract/protocols/smtp.h>

#endif // __LWS_ABS_PROTOCOL_H__

// By GST @Date