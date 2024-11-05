typedef uint32_t lws_conmon_interval_us_t;

typedef enum lws_conmon_pcol {
    LWSCONMON_PCOL_NONE,
    LWSCONMON_PCOL_HTTP,
} lws_conmon_pcol_t;

typedef enum lws_conmon_dns_disposition {
    LWSCONMON_DNS_NONE,
    LWSCONMON_DNS_OK = 1,
    LWSCONMON_DNS_SERVER_UNREACHABLE = 2,
    LWSCONMON_DNS_NO_RESULT = 3
} lws_conmon_dns_disposition_t;

struct lws_conmon {
    lws_sockaddr46 peer46;
    union {
        struct {
            int response;
        } http;
    } protocol_specific;
    struct addrinfo *dns_results_copy;
    lws_conmon_interval_us_t ciu_dns;
    lws_conmon_interval_us_t ciu_sockconn;
    lws_conmon_interval_us_t ciu_tls;
    lws_conmon_interval_us_t ciu_txn_resp;
    lws_conmon_pcol_t pcol;
    lws_conmon_dns_disposition_t dns_disposition;
};

/**
 * lws_conmon_wsi_take() - create a connection latency object from client wsi
 *
 * \param context: lws wsi
 * \param dest: conmon struct to fill
 *
 * Copies wsi conmon data into the caller's struct.  Passes ownership of
 * any allocations in the addrinfo list to the caller, lws will not delete that
 * any more on wsi close after this call.  The caller must call
 * lws_conmon_release() on the struct to destroy any addrinfo in the struct
 * that is prepared by this eventually but it can defer it as long as it wants.
 *
 * Other than the addrinfo list, the contents of the returned object are
 * completely selfcontained and don't point outside of the object itself, ie,
 * everything else in there remains in scope while the object itself does.
 */
LWS_VISIBLE LWS_EXTERN void
lws_conmon_wsi_take(struct lws *wsi, struct lws_conmon *dest);

/**
 * lws_conmon_release() - free any allocations in the conmon struct
 *
 * \param conmon: pointer to conmon struct
 *
 * Destroys any allocations in the conmon struct so it can go out of scope.
 * It doesn't free \p dest itself, it's designed to clean out a struct that
 * is on the stack or embedded in another object.
 */
LWS_VISIBLE LWS_EXTERN void
lws_conmon_release(struct lws_conmon *conmon);

///@}
// By GST @Date