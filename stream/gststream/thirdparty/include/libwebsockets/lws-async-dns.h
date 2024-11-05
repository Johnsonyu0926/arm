

#if defined(LWS_WITH_UDP)

typedef enum dns_query_type {
    LWS_ADNS_RECORD_A                   = 0x01,
    LWS_ADNS_RECORD_CNAME                   = 0x05,
    LWS_ADNS_RECORD_MX                  = 0x0f,
    LWS_ADNS_RECORD_AAAA                    = 0x1c,
} adns_query_type_t;

typedef enum {
    LADNS_RET_FAILED_WSI_CLOSED             = -4,
    LADNS_RET_NXDOMAIN                  = -3,
    LADNS_RET_TIMEDOUT                  = -2,
    LADNS_RET_FAILED                    = -1,
    LADNS_RET_FOUND,
    LADNS_RET_CONTINUING
} lws_async_dns_retcode_t;

struct addrinfo;

typedef struct lws * (*lws_async_dns_cb_t)(struct lws *wsi, const char *ads,
        const struct addrinfo *result, int n, void *opaque);

LWS_VISIBLE LWS_EXTERN lws_async_dns_retcode_t
lws_async_dns_query(struct lws_context *context, int tsi, const char *name,
            adns_query_type_t qtype, lws_async_dns_cb_t cb,
            struct lws *wsi, void *opaque);

LWS_VISIBLE LWS_EXTERN void
lws_async_dns_freeaddrinfo(const struct addrinfo **ai);

#endif
// By GST @Date
