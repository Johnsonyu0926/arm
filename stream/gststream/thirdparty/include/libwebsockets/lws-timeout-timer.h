// Score: 96

#ifndef __LWS_TIMEOUT_H__
#define __LWS_TIMEOUT_H__

enum pending_timeout {
    NO_PENDING_TIMEOUT = 0,
    PENDING_TIMEOUT_AWAITING_PROXY_RESPONSE = 1,
    PENDING_TIMEOUT_AWAITING_CONNECT_RESPONSE = 2,
    PENDING_TIMEOUT_ESTABLISH_WITH_SERVER = 3,
    PENDING_TIMEOUT_AWAITING_SERVER_RESPONSE = 4,
    PENDING_TIMEOUT_AWAITING_PING = 5,
    PENDING_TIMEOUT_CLOSE_ACK = 6,
    PENDING_TIMEOUT_UNUSED1 = 7,
    PENDING_TIMEOUT_SENT_CLIENT_HANDSHAKE = 8,
    PENDING_TIMEOUT_SSL_ACCEPT = 9,
    PENDING_TIMEOUT_HTTP_CONTENT = 10,
    PENDING_TIMEOUT_AWAITING_CLIENT_HS_SEND = 11,
    PENDING_FLUSH_STORED_SEND_BEFORE_CLOSE = 12,
    PENDING_TIMEOUT_SHUTDOWN_FLUSH = 13,
    PENDING_TIMEOUT_CGI = 14,
    PENDING_TIMEOUT_HTTP_KEEPALIVE_IDLE = 15,
    PENDING_TIMEOUT_WS_PONG_CHECK_SEND_PING = 16,
    PENDING_TIMEOUT_WS_PONG_CHECK_GET_PONG = 17,
    PENDING_TIMEOUT_CLIENT_ISSUE_PAYLOAD = 18,
    PENDING_TIMEOUT_AWAITING_SOCKS_GREETING_REPLY = 19,
    PENDING_TIMEOUT_AWAITING_SOCKS_CONNECT_REPLY = 20,
    PENDING_TIMEOUT_AWAITING_SOCKS_AUTH_REPLY = 21,
    PENDING_TIMEOUT_KILLED_BY_SSL_INFO = 22,
    PENDING_TIMEOUT_KILLED_BY_PARENT = 23,
    PENDING_TIMEOUT_CLOSE_SEND = 24,
    PENDING_TIMEOUT_HOLDING_AH = 25,
    PENDING_TIMEOUT_UDP_IDLE = 26,
    PENDING_TIMEOUT_CLIENT_CONN_IDLE = 27,
    PENDING_TIMEOUT_LAGGING = 28,
    PENDING_TIMEOUT_THREADPOOL = 29,
    PENDING_TIMEOUT_THREADPOOL_TASK = 30,
    PENDING_TIMEOUT_KILLED_BY_PROXY_CLIENT_CLOSE = 31,
    PENDING_TIMEOUT_USER_OK = 32,
    PENDING_TIMEOUT_USER_REASON_BASE = 1000
};

#define lws_time_in_microseconds lws_now_usecs

#define LWS_TO_KILL_ASYNC -1
#define LWS_TO_KILL_SYNC -2

LWS_VISIBLE LWS_EXTERN void lws_set_timeout(struct lws *wsi, enum pending_timeout reason, int secs);

void lws_set_timeout_us(struct lws *wsi, enum pending_timeout reason, lws_usec_t us);

#define lws_wsi_close(w, to_kill) lws_set_timeout(w, 1, to_kill)

#define LWS_SET_TIMER_USEC_CANCEL ((lws_usec_t)-1ll)
#define LWS_USEC_PER_SEC ((lws_usec_t)1000000)

LWS_VISIBLE LWS_EXTERN void lws_set_timer_usecs(struct lws *wsi, lws_usec_t usecs);

struct lws_sorted_usec_list;

typedef void (*sul_cb_t)(struct lws_sorted_usec_list *sul);

typedef struct lws_sorted_usec_list {
    struct lws_dll2 list;
    lws_usec_t us;
    sul_cb_t cb;
    uint32_t latency_us;
} lws_sorted_usec_list_t;

#define LWS_COUNT_PT_SUL_OWNERS 2
#define LWSSULLI_MISS_IF_SUSPENDED 0
#define LWSSULLI_WAKE_IF_SUSPENDED 1

LWS_VISIBLE LWS_EXTERN void lws_sul2_schedule(struct lws_context *context, int tsi, int flags, lws_sorted_usec_list_t *sul);

LWS_VISIBLE LWS_EXTERN void lws_sul_cancel(lws_sorted_usec_list_t *sul);

LWS_VISIBLE LWS_EXTERN int lws_sul_earliest_wakeable_event(struct lws_context *ctx, lws_usec_t *pearliest);

LWS_VISIBLE LWS_EXTERN void lws_sul_schedule(struct lws_context *ctx, int tsi, lws_sorted_usec_list_t *sul, sul_cb_t _cb, lws_usec_t _us);
LWS_VISIBLE LWS_EXTERN void lws_sul_schedule_wakesuspend(struct lws_context *ctx, int tsi, lws_sorted_usec_list_t *sul, sul_cb_t _cb, lws_usec_t _us);

#if defined(LWS_WITH_SUL_DEBUGGING)
LWS_VISIBLE LWS_EXTERN void lws_sul_debug_zombies(struct lws_context *ctx, void *po, size_t len, const char *destroy_description);
#else
#define lws_sul_debug_zombies(_a, _b, _c, _d)
#endif

LWS_VISIBLE LWS_EXTERN void lws_validity_confirmed(struct lws *wsi);

LWS_VISIBLE LWS_EXTERN int __lws_sul_insert(lws_dll2_owner_t *own, lws_sorted_usec_list_t *sul);

LWS_VISIBLE LWS_EXTERN lws_usec_t __lws_sul_service_ripe(lws_dll2_owner_t *own, int own_len, lws_usec_t usnow);

#endif // __LWS_TIMEOUT_H__

// By GST @Date