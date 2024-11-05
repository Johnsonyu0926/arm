// Score: 96

#ifndef __LWS_ABSTRACT_H__
#define __LWS_ABSTRACT_H__

#include <stdint.h>
#include "lws_vhost.h"
#include "lws_sequencer.h"

typedef struct lws_token_map {
    union {
        const char *value;
        uint8_t *bvalue;
        unsigned long lvalue;
    } u;
    short name_index;
    short length_or_zero;
} lws_token_map_t;

enum {
    LTMI_END_OF_ARRAY,
    LTMI_PROTOCOL_BASE = 2048,
    LTMI_TRANSPORT_BASE = 4096
};

struct lws_abs_transport;
struct lws_abs_protocol;
typedef struct lws_abs lws_abs_t;

LWS_VISIBLE LWS_EXTERN const lws_token_map_t *lws_abs_get_token(const lws_token_map_t *token_map, short name_index);

typedef void lws_abs_transport_inst_t;
typedef void lws_abs_protocol_inst_t;

LWS_VISIBLE LWS_EXTERN lws_abs_t *lws_abstract_alloc(struct lws_vhost *vhost, void *user, const char *abstract_path, const lws_token_map_t *ap_tokens, const lws_token_map_t *at_tokens, struct lws_sequencer *seq, void *opaque_user_data);

LWS_VISIBLE LWS_EXTERN void lws_abstract_free(lws_abs_t **pabs);

LWS_VISIBLE LWS_EXTERN lws_abs_t *lws_abs_bind_and_create_instance(const lws_abs_t *ai);

LWS_VISIBLE LWS_EXTERN void lws_abs_destroy_instance(lws_abs_t **ai);

#include <libwebsockets/abstract/protocols.h>
#include <libwebsockets/abstract/transports.h>

#endif // __LWS_ABSTRACT_H__

// By GST @Date