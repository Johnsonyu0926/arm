typedef struct lws_retry_bo {
    const uint32_t *retry_ms_table;
    uint16_t retry_ms_table_count;
    uint16_t conceal_count;
    uint16_t secs_since_valid_ping;
    uint16_t secs_since_valid_hangup;
    uint8_t jitter_percent;
} lws_retry_bo_t;

#define LWS_RETRY_CONCEAL_ALWAYS (0xffff)

LWS_VISIBLE LWS_EXTERN unsigned int lws_retry_get_delay_ms(struct lws_context *context, const lws_retry_bo_t *retry, uint16_t *ctry, char *conceal);

LWS_VISIBLE LWS_EXTERN int lws_retry_sul_schedule(struct lws_context *context, int tid, lws_sorted_usec_list_t *sul, const lws_retry_bo_t *retry, sul_cb_t cb, uint16_t *ctry);

LWS_VISIBLE LWS_EXTERN int lws_retry_sul_schedule_retry_wsi(struct lws *wsi, lws_sorted_usec_list_t *sul, sul_cb_t cb, uint16_t *ctry);
// By GST @Date