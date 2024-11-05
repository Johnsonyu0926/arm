//GST
#define LWS_SS_MTU 1540

struct lws_ss_handle;
typedef uint32_t lws_ss_tx_ordinal_t;

typedef enum {
    LWSSSCS_CREATING = 1,
    LWSSSCS_DISCONNECTED,
    LWSSSCS_UNREACHABLE,
    LWSSSCS_AUTH_FAILED,
    LWSSSCS_CONNECTED,
    LWSSSCS_CONNECTING,
    LWSSSCS_DESTROYING,
    LWSSSCS_POLL,
    LWSSSCS_ALL_RETRIES_FAILED,
    LWSSSCS_QOS_ACK_REMOTE,
    LWSSSCS_QOS_NACK_REMOTE,
    LWSSSCS_QOS_ACK_LOCAL,
    LWSSSCS_QOS_NACK_LOCAL,
    LWSSSCS_TIMEOUT,
    LWSSSCS_SERVER_TXN,
    LWSSSCS_SERVER_UPGRADE,
    LWSSSCS_EVENT_WAIT_CANCELLED,
    LWSSSCS_UPSTREAM_LINK_RETRY,
    LWSSSCS_SINK_JOIN,
    LWSSSCS_SINK_PART,
    LWSSSCS_USER_BASE = 1000
} lws_ss_constate_t;

enum {
    LWSSS_FLAG_SOM = (1 << 0),
    LWSSS_FLAG_EOM = (1 << 1),
    LWSSS_FLAG_POLL = (1 << 2),
    LWSSS_FLAG_RELATED_START = (1 << 3),
    LWSSS_FLAG_RELATED_END = (1 << 4),
    LWSSS_FLAG_RIDESHARE = (1 << 5),
    LWSSS_FLAG_PERF_JSON = (1 << 6),

    LWSSS_SER_RXPRE_RX_PAYLOAD = 0x55,
    LWSSS_SER_RXPRE_CREATE_RESULT,
    LWSSS_SER_RXPRE_CONNSTATE,
    LWSSS_SER_RXPRE_TXCR_UPDATE,
    LWSSS_SER_RXPRE_METADATA,
    LWSSS_SER_RXPRE_TLSNEG_ENCLAVE_SIGN,
    LWSSS_SER_RXPRE_PERF,

    LWSSS_SER_TXPRE_STREAMTYPE = 0xaa,
    LWSSS_SER_TXPRE_ONWARD_CONNECT,
    LWSSS_SER_TXPRE_DESTROYING,
    LWSSS_SER_TXPRE_TX_PAYLOAD,
    LWSSS_SER_TXPRE_METADATA,
    LWSSS_SER_TXPRE_TXCR_UPDATE,
    LWSSS_SER_TXPRE_TIMEOUT_UPDATE,
    LWSSS_SER_TXPRE_PAYLOAD_LENGTH_HINT,
    LWSSS_SER_TXPRE_TLSNEG_ENCLAVE_SIGNED,
};

typedef enum {
    LPCSPROX_WAIT_INITIAL_TX = 1,
    LPCSPROX_REPORTING_FAIL,
    LPCSPROX_REPORTING_OK,
    LPCSPROX_OPERATIONAL,
    LPCSPROX_DESTROYED,

    LPCSCLI_SENDING_INITIAL_TX,
    LPCSCLI_WAITING_CREATE_RESULT,
    LPCSCLI_LOCAL_CONNECTED,
    LPCSCLI_ONWARD_CONNECT,
    LPCSCLI_OPERATIONAL,
} lws_ss_conn_states_t;

typedef enum lws_ss_state_return {
    LWSSSSRET_TX_DONT_SEND = 1,
    LWSSSSRET_OK = 0,
    LWSSSSRET_DISCONNECT_ME = -1,
    LWSSSSRET_DESTROY_ME = -2,
} lws_ss_state_return_t;

enum {
    LWSSSINFLAGS_REGISTER_SINK = (1 << 0),
    LWSSSINFLAGS_PROXIED = (1 << 1),
    LWSSSINFLAGS_SERVER = (1 << 2),
    LWSSSINFLAGS_ACCEPTED = (1 << 3),
};

typedef lws_ss_state_return_t (*lws_sscb_rx)(void *userobj, const uint8_t *buf, size_t len, int flags);
typedef lws_ss_state_return_t (*lws_sscb_tx)(void *userobj, lws_ss_tx_ordinal_t ord, uint8_t *buf, size_t *len, int *flags);
typedef lws_ss_state_return_t (*lws_sscb_state)(void *userobj, void *h_src, lws_ss_constate_t state, lws_ss_tx_ordinal_t ack);

#if defined(LWS_WITH_SECURE_STREAMS_BUFFER_DUMP)
typedef void (*lws_ss_buffer_dump_cb)(void *userobj, const uint8_t *buf, size_t len, int done);
#endif

struct lws_ss_policy;

typedef struct lws_ss_info {
    const char *streamtype;
    size_t user_alloc;
    size_t handle_offset;
    size_t opaque_user_data_offset;
#if defined(LWS_WITH_SECURE_STREAMS_CPP)
    const struct lws_ss_policy *policy;
#endif
#if defined(LWS_WITH_SYS_FAULT_INJECTION)
    lws_fi_ctx_t fic;
#endif
    lws_sscb_rx rx;
    lws_sscb_tx tx;
    lws_sscb_state state;
#if defined(LWS_WITH_SECURE_STREAMS_BUFFER_DUMP)
    lws_ss_buffer_dump_cb dump;
#endif
    int manual_initial_tx_credit;
    uint32_t client_pid;
    uint8_t flags;
    uint8_t sss_protocol_version;
} lws_ss_info_t;

LWS_VISIBLE LWS_EXTERN int LWS_WARN_UNUSED_RESULT lws_ss_create(struct lws_context *context, int tsi, const lws_ss_info_t *ssi, void *opaque_user_data, struct lws_ss_handle **ppss, struct lws_sequencer *seq_owner, const char **ppayload_fmt);

LWS_VISIBLE LWS_EXTERN void lws_ss_destroy(struct lws_ss_handle **ppss);

LWS_VISIBLE LWS_EXTERN lws_ss_state_return_t LWS_WARN_UNUSED_RESULT lws_ss_request_tx(struct lws_ss_handle *pss);

LWS_VISIBLE LWS_EXTERN lws_ss_state_return_t LWS_WARN_UNUSED_RESULT lws_ss_request_tx_len(struct lws_ss_handle *pss, unsigned long len);

LWS_VISIBLE LWS_EXTERN lws_ss_state_return_t LWS_WARN_UNUSED_RESULT lws_ss_client_connect(struct lws_ss_handle *h);

LWS_VISIBLE LWS_EXTERN struct lws_sequencer *lws_ss_get_sequencer(struct lws_ss_handle *h);

LWS_VISIBLE LWS_EXTERN int lws_ss_proxy_create(struct lws_context *context, const char *bind, int port);

LWS_VISIBLE LWS_EXTERN const char *lws_ss_state_name(int state);

LWS_VISIBLE LWS_EXTERN struct lws_context *lws_ss_get_context(struct lws_ss_handle *h);

#define LWSSS_TIMEOUT_FROM_POLICY 0

LWS_VISIBLE LWS_EXTERN void lws_ss_start_timeout(struct lws_ss_handle *h, unsigned int timeout_ms);

LWS_VISIBLE LWS_EXTERN void lws_ss_cancel_timeout(struct lws_ss_handle *h);

LWS_VISIBLE LWS_EXTERN void *lws_ss_to_user_object(struct lws_ss_handle *h);

LWS_VISIBLE LWS_EXTERN const char *lws_ss_rideshare(struct lws_ss_handle *h);

LWS_VISIBLE LWS_EXTERN int LWS_WARN_UNUSED_RESULT lws_ss_set_metadata(struct lws_ss_handle *h, const char *name, const void *value, size_t len);

LWS_VISIBLE LWS_EXTERN int LWS_WARN_UNUSED_RESULT lws_ss_alloc_set_metadata(struct lws_ss_handle *h, const char *name, const void *value, size_t len);

LWS_VISIBLE LWS_EXTERN int lws_ss_get_metadata(struct lws_ss_handle *h, const char *name, const void **value, size_t *len);

LWS_VISIBLE LWS_EXTERN void lws_ss_server_ack(struct lws_ss_handle *h, int nack);

typedef void (*lws_sssfec_cb)(struct lws_ss_handle *h, void *arg);

LWS_VISIBLE LWS_EXTERN void lws_ss_server_foreach_client(struct lws_ss_handle *h, lws_sssfec_cb cb, void *arg);

LWS_VISIBLE LWS_EXTERN void lws_ss_change_handlers(struct lws_ss_handle *h, lws_sscb_rx rx, lws_sscb_tx tx, lws_sscb_state state);

LWS_VISIBLE LWS_EXTERN int lws_ss_add_peer_tx_credit(struct lws_ss_handle *h, int32_t add);

LWS_VISIBLE LWS_EXTERN int lws_ss_get_est_peer_tx_credit(struct lws_ss_handle *h);

LWS_VISIBLE LWS_EXTERN const char *lws_ss_tag(struct lws_ss_handle *h);

#if defined(LWS_WITH_SECURE_STREAMs_AUTH_SIGV4)
LWS_VISIBLE LWS_EXTERN int lws_ss_sigv4_set_aws_key(struct lws_context* context, uint8_t idx, const char *keyid, const char *key);

LWS_VISIBLE LWS_EXTERN int lws_aws_filesystem_credentials_helper(const char *path, const char *kid, const char *ak, char **aws_keyid, char **aws_key);
#endif
// By GST @Date