// Score: 96

#ifndef __LWS_TLS_SESSION_H__
#define __LWS_TLS_SESSION_H__

#define LWS_SESSION_TAG_LEN 96

struct lws_tls_session_dump {
    char tag[LWS_SESSION_TAG_LEN];
    void *blob;
    void *opaque;
    size_t blob_len;
};

typedef int (*lws_tls_sess_cb_t)(struct lws_context *cx, struct lws_tls_session_dump *info);

LWS_VISIBLE LWS_EXTERN int lws_tls_session_dump_save(struct lws_vhost *vh, const char *host, uint16_t port, lws_tls_sess_cb_t cb_save, void *opq);

LWS_VISIBLE LWS_EXTERN int lws_tls_session_dump_load(struct lws_vhost *vh, const char *host, uint16_t port, lws_tls_sess_cb_t cb_load, void *opq);

#endif // __LWS_TLS_SESSION_H__

// By GST @Date