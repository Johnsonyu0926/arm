// Filename: lws_sequencer.h
// Score: 98

typedef enum {
    LWSSEQ_CREATED,
    LWSSEQ_DESTROYED,
    LWSSEQ_TIMED_OUT,
    LWSSEQ_HEARTBEAT,
    LWSSEQ_WSI_CONNECTED,
    LWSSEQ_WSI_CONN_FAIL,
    LWSSEQ_WSI_CONN_CLOSE,
    LWSSEQ_SS_STATE_BASE,
    LWSSEQ_USER_BASE = 100
} lws_seq_events_t;

typedef enum lws_seq_cb_return {
    LWSSEQ_RET_CONTINUE,
    LWSSEQ_RET_DESTROY
} lws_seq_cb_return_t;

typedef lws_seq_cb_return_t (*lws_seq_event_cb)(struct lws_sequencer *seq, void *user, int event, void *data, void *aux);

typedef struct lws_seq_info {
    struct lws_context *context;
    int tsi;
    size_t user_size;
    void **puser;
    lws_seq_event_cb cb;
    const char *name;
    const lws_retry_bo_t *retry;
    uint8_t wakesuspend:1;
} lws_seq_info_t;

LWS_VISIBLE LWS_EXTERN struct lws_sequencer *lws_seq_create(lws_seq_info_t *info);

LWS_VISIBLE LWS_EXTERN void lws_seq_destroy(struct lws_sequencer **seq);

LWS_VISIBLE LWS_EXTERN int lws_seq_queue_event(struct lws_sequencer *seq, lws_seq_events_t e, void *data, void *aux);

LWS_VISIBLE LWS_EXTERN int lws_seq_check_wsi(struct lws_sequencer *seq, struct lws *wsi);

#define LWSSEQTO_NONE 0

LWS_VISIBLE LWS_EXTERN int lws_seq_timeout_us(struct lws_sequencer *seq, lws_usec_t us);

LWS_VISIBLE LWS_EXTERN struct lws_sequencer *lws_seq_from_user(void *u);

LWS_VISIBLE LWS_EXTERN lws_usec_t lws_seq_us_since_creation(struct lws_sequencer *seq);

LWS_VISIBLE LWS_EXTERN const char *lws_seq_name(struct lws_sequencer *seq);

LWS_VISIBLE LWS_EXTERN struct lws_context *lws_seq_get_context(struct lws_sequencer *seq);

// By GST @Date