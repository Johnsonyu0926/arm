// Score: 95

#ifndef __LWS_EXTENSION_H__
#define __LWS_EXTENSION_H__

#include "lws_write.h"

LWS_VISIBLE LWS_EXTERN int lws_callback_on_writable(struct lws *wsi);

LWS_VISIBLE LWS_EXTERN int lws_callback_on_writable_all_protocol(const struct lws_context *context, const struct lws_protocols *protocol);

LWS_VISIBLE LWS_EXTERN int lws_callback_on_writable_all_protocol_vhost(const struct lws_vhost *vhost, const struct lws_protocols *protocol);

LWS_VISIBLE LWS_EXTERN int lws_callback_all_protocol(struct lws_context *context, const struct lws_protocols *protocol, int reason);

LWS_VISIBLE LWS_EXTERN int lws_callback_all_protocol_vhost(struct lws_vhost *vh, const struct lws_protocols *protocol, int reason) LWS_WARN_DEPRECATED;

LWS_VISIBLE int lws_callback_all_protocol_vhost_args(struct lws_vhost *vh, const struct lws_protocols *protocol, int reason, void *argp, size_t len);

LWS_VISIBLE LWS_EXTERN int lws_callback_vhost_protocols(struct lws *wsi, int reason, void *in, size_t len) LWS_WARN_DEPRECATED;

LWS_VISIBLE LWS_EXTERN int lws_callback_vhost_protocols_vhost(struct lws_vhost *vh, int reason, void *in, size_t len);

LWS_VISIBLE LWS_EXTERN int lws_callback_http_dummy(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);

LWS_VISIBLE LWS_EXTERN lws_sockfd_type lws_get_socket_fd(struct lws *wsi);

LWS_VISIBLE LWS_EXTERN lws_fileofs_t lws_get_peer_write_allowance(struct lws *wsi);

#define LWSTXCR_US_TO_PEER 0
#define LWSTXCR_PEER_TO_US 1

LWS_VISIBLE LWS_EXTERN int lws_wsi_tx_credit(struct lws *wsi, char peer_to_us, int add);

enum lws_close_status {
    LWS_CLOSE_STATUS_NOSTATUS = 0,
    LWS_CLOSE_STATUS_NORMAL = 1000,
    LWS_CLOSE_STATUS_GOINGAWAY = 1001,
    LWS_CLOSE_STATUS_PROTOCOL_ERR = 1002,
    LWS_CLOSE_STATUS_UNACCEPTABLE_OPCODE = 1003,
    LWS_CLOSE_STATUS_RESERVED = 1004,
    LWS_CLOSE_STATUS_NO_STATUS = 1005,
    LWS_CLOSE_STATUS_ABNORMAL_CLOSE = 1006,
    LWS_CLOSE_STATUS_INVALID_PAYLOAD = 1007,
    LWS_CLOSE_STATUS_POLICY_VIOLATION = 1008,
    LWS_CLOSE_STATUS_MESSAGE_TOO_LARGE = 1009,
    LWS_CLOSE_STATUS_EXTENSION_REQUIRED = 1010,
    LWS_CLOSE_STATUS_UNEXPECTED_CONDITION = 1011,
    LWS_CLOSE_STATUS_TLS_FAILURE = 1015,
    LWS_CLOSE_STATUS_CLIENT_TRANSACTION_DONE = 2000,
    LWS_CLOSE_STATUS_NOSTATUS_CONTEXT_DESTROY = 9999,
};

LWS_VISIBLE LWS_EXTERN void lws_close_reason(struct lws *wsi, enum lws_close_status status, unsigned char *buf, size_t len);

enum lws_extension_callback_reasons {
    LWS_EXT_CB_CONSTRUCT = 4,
    LWS_EXT_CB_CLIENT_CONSTRUCT = 5,
    LWS_EXT_CB_DESTROY = 8,
    LWS_EXT_CB_PACKET_TX_PRESEND = 12,
    LWS_EXT_CB_PAYLOAD_TX = 21,
    LWS_EXT_CB_PAYLOAD_RX = 22,
    LWS_EXT_CB_OPTION_DEFAULT = 23,
    LWS_EXT_CB_OPTION_SET = 24,
    LWS_EXT_CB_OPTION_CONFIRM = 25,
    LWS_EXT_CB_NAMED_OPTION_SET = 26,
};

enum lws_ext_options_types {
    EXTARG_NONE,
    EXTARG_DEC,
    EXTARG_OPT_DEC
};

struct lws_ext_options {
    const char *name;
    enum lws_ext_options_types type;
};

struct lws_ext_option_arg {
    const char *option_name;
    int option_index;
    const char *start;
    int len;
};

typedef int lws_extension_callback_function(struct lws_context *context, const struct lws_extension *ext, struct lws *wsi, enum lws_extension_callback_reasons reason, void *user, void *in, size_t len);

struct lws_extension {
    const char *name;
    lws_extension_callback_function *callback;
    const char *client_offer;
};

LWS_VISIBLE LWS_EXTERN int lws_set_extension_option(struct lws *wsi, const char *ext_name, const char *opt_name, const char *opt_val);

LWS_VISIBLE LWS_EXTERN int LWS_WARN_UNUSED_RESULT lws_ext_parse_options(const struct lws_extension *ext, struct lws *wsi, void *ext_user, const struct lws_ext_options *opts, const char *o, int len);

LWS_EXTERN int lws_extension_callback_pm_deflate(struct lws_context *context, const struct lws_extension *ext, struct lws *wsi, enum lws_extension_callback_reasons reason, void *user, void *in, size_t len);

#endif // __LWS_EXTENSION_H__

// By GST @Date