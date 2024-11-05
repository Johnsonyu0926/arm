//GST
typedef int (*plugin_auth_status_cb)(struct lws_ss_handle *ss, int status);

#if defined(LWS_WITH_SSPLUGINS)
typedef struct lws_ss_plugin {
    struct lws_ss_plugin *next;
    const char *name;
    size_t alloc;
    int (*create)(struct lws_ss_handle *ss, void *info, plugin_auth_status_cb status);
    int (*destroy)(struct lws_ss_handle *ss);
    int (*munge)(struct lws_ss_handle *ss, char *path, size_t path_len);
} lws_ss_plugin_t;
#endif

typedef struct lws_metric_policy {
    const struct lws_metric_policy *next;
    const char *name;
    const char *report;
    uint64_t us_schedule;
    uint32_t us_decay_unit;
    uint8_t min_contributors;
} lws_metric_policy_t;

typedef struct lws_ss_x509 {
    struct lws_ss_x509 *next;
    const char *vhost_name;
    const uint8_t *ca_der;
    size_t ca_der_len;
    uint8_t keep:1;
} lws_ss_x509_t;

enum {
    LWSSSPOLF_OPPORTUNISTIC = (1 << 0),
    LWSSSPOLF_NAILED_UP = (1 << 1),
    LWSSSPOLF_URGENT_TX = (1 << 2),
    LWSSSPOLF_URGENT_RX = (1 << 3),
    LWSSSPOLF_TLS = (1 << 4),
    LWSSSPOLF_LONG_POLL = (1 << 5),
    LWSSSPOLF_AUTH_BEARER = (1 << 6),
    LWSSSPOLF_HTTP_NO_CONTENT_LENGTH = (1 << 7),
    LWSSSPOLF_QUIRK_NGHTTP2_END_STREAM = (1 << 8),
    LWSSSPOLF_H2_QUIRK_OVERFLOWS_TXCR = (1 << 9),
    LWSSSPOLF_H2_QUIRK_UNCLEAN_HPACK_STATE = (1 << 10),
    LWSSSPOLF_HTTP_MULTIPART = (1 << 11),
    LWSSSPOLF_HTTP_X_WWW_FORM_URLENCODED = (1 << 12),
    LWSSSPOLF_LOCAL_SINK = (1 << 13),
    LWSSSPOLF_WAKE_SUSPEND__VALIDITY = (1 << 14),
    LWSSSPOLF_SERVER = (1 << 15),
    LWSSSPOLF_ALLOW_REDIRECTS = (1 << 16),
    LWSSSPOLF_HTTP_MULTIPART_IN = (1 << 17),
    LWSSSPOLF_ATTR_LOW_LATENCY = (1 << 18),
    LWSSSPOLF_ATTR_HIGH_THROUGHPUT = (1 << 19),
    LWSSSPOLF_ATTR_HIGH_RELIABILITY = (1 << 20),
    LWSSSPOLF_ATTR_LOW_COST = (1 << 21),
    LWSSSPOLF_PERF = (1 << 22),
    LWSSSPOLF_DIRECT_PROTO_STR = (1 << 23),
    LWSSSPOLF_HTTP_CACHE_COOKIES = (1 << 24),
    LWSSSPOLF_PRIORITIZE_READS = (1 << 25),
};

typedef struct lws_ss_trust_store {
    struct lws_ss_trust_store *next;
    const char *name;
    const lws_ss_x509_t *ssx509[6];
    int count;
} lws_ss_trust_store_t;

enum {
    LWSSSP_H1,
    LWSSSP_H2,
    LWSSSP_WS,
    LWSSSP_MQTT,
    LWSSSP_RAW,
    LWSSS_HBI_AUTH = 0,
    LWSSS_HBI_DSN,
    LWSSS_HBI_FWV,
    LWSSS_HBI_TYPE,
    _LWSSS_HBI_COUNT
};

typedef struct lws_ss_metadata {
    struct lws_ss_metadata *next;
    const char *name;
    void *value__may_own_heap;
    size_t length;
    uint8_t value_length;
    uint8_t value_is_http_token;
#if defined(LWS_WITH_SS_DIRECT_PROTOCOL_STR)
    uint8_t name_on_lws_heap:1;
#endif
    uint8_t value_on_lws_heap:1;
#if defined(LWS_WITH_SECURE_STREAMS_PROXY_API)
    uint8_t pending_onward:1;
#endif
} lws_ss_metadata_t;

typedef struct lws_ss_http_respmap {
    uint16_t resp;
    uint16_t state;
} lws_ss_http_respmap_t;

typedef struct lws_ss_auth {
    struct lws_ss_auth *next;
    const char *name;
    const char *type;
    const char *streamtype;
    uint8_t blob_index;
} lws_ss_auth_t;

typedef struct lws_ss_policy {
    struct lws_ss_policy *next;
    const char *streamtype;
    const char *endpoint;
    const char *rideshare_streamtype;
    const char *payload_fmt;
    const char *socks5_proxy;
    lws_ss_metadata_t *metadata;
    const lws_metric_policy_t *metrics;
    const lws_ss_auth_t *auth;
    union {
#if defined(LWS_ROLE_H1) || defined(LWS_ROLE_H2) || defined(LWS_ROLE_WS)
        struct {
            const char *method;
            const char *url;
            const char *multipart_name;
            const char *multipart_filename;
            const char *multipart_content_type;
            const char *blob_header[_LWSSS_HBI_COUNT];
            const char *auth_preamble;
            const lws_ss_http_respmap_t *respmap;
            union {
                struct {
                    const char *subprotocol;
                    uint8_t binary;
                } ws;
            } u;
            uint16_t resp_expect;
            uint8_t count_respmap;
            uint8_t fail_redirect:1;
        } http;
#endif
#if defined(LWS_ROLE_MQTT)
        struct {
            const char *topic;
            const char *subscribe;
            const char *will_topic;
            const char *will_message;
            uint16_t keep_alive;
            uint8_t qos;
            uint8_t clean_start;
            uint8_t will_qos;
            uint8_t will_retain;
            uint8_t aws_iot;
        } mqtt;
#endif
    } u;

#if defined(LWS_WITH_SSPLUGINS)
    const struct lws_ss_plugin *plugins[2];
    const void *plugins_info[2];
#endif

#if defined(LWS_WITH_SECURE_STREAMS_AUTH_SIGV4)
    const char *aws_region;
    const char *aws_service;
#endif

    union {
        const lws_ss_trust_store_t *store;
        struct {
            const lws_ss_x509_t *cert;
            const lws_ss_x509_t *key;
        } server;
    } trust;

    const lws_retry_bo_t *retry_bo;

    uint32_t proxy_buflen;
    uint32_t proxy_buflen_rxflow_on_above;
    uint32_t proxy_buflen_rxflow_off_below;
    uint32_t client_buflen;
    uint32_t client_buflen_rxflow_on_above;
    uint32_t client_buflen_rxflow_off_below;
    uint32_t timeout_ms;
    uint32_t flags;

    uint16_t port;

    uint8_t metadata_count;
    uint8_t protocol;
    uint8_t client_cert;
    uint8_t priority;
} lws_ss_policy_t;

#if !defined(LWS_WITH_SECURE_STREAMs_STATIC_POLICY_ONLY)
LWS_VISIBLE LWS_EXTERN int lws_ss_policy_parse_begin(struct lws_context *context, int overlay);

LWS_VISIBLE LWS_EXTERN int lws_ss_policy_parse_abandon(struct lws_context *context);

LWS_VISIBLE LWS_EXTERN int lws_ss_policy_parse(struct lws_context *context, const uint8_t *buf, size_t len);

LWS_VISIBLE LWS_EXTERN int lws_ss_policy_overlay(struct lws_context *context, const char *overlay);

LWS_VISIBLE LWS_EXTERN const lws_ss_policy_t *lws_ss_policy_get(struct lws_context *context);

LWS_VISIBLE LWS_EXTERN const lws_ss_auth_t *lws_ss_auth_get(struct lws_context *context);
#endif
// By GST @Date