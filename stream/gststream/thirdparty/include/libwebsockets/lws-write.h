// Filename: lws_write.h
// Score: 95

#ifndef __LWS_WRITE_H__
#define __LWS_WRITE_H__

#if !defined(LWS_SIZEOFPTR)
#define LWS_SIZEOFPTR ((int)sizeof(void *))
#endif

#if defined(__x86_64__)
#define _LWS_PAD_SIZE 16
#else
#define _LWS_PAD_SIZE LWS_SIZEOFPTR
#endif

#define _LWS_PAD(n) (((n) % _LWS_PAD_SIZE) ? ((n) + (_LWS_PAD_SIZE - ((n) % _LWS_PAD_SIZE))) : (n))
#define LWS_PRE _LWS_PAD(4 + 10 + 2)
#define LWS_SEND_BUFFER_PRE_PADDING LWS_PRE
#define LWS_SEND_BUFFER_POST_PADDING 0

#define LWS_WRITE_RAW LWS_WRITE_HTTP

enum lws_write_protocol {
    LWS_WRITE_TEXT = 0,
    LWS_WRITE_BINARY = 1,
    LWS_WRITE_CONTINUATION = 2,
    LWS_WRITE_HTTP = 3,
    LWS_WRITE_PING = 5,
    LWS_WRITE_PONG = 6,
    LWS_WRITE_HTTP_FINAL = 7,
    LWS_WRITE_HTTP_HEADERS = 8,
    LWS_WRITE_HTTP_HEADERS_CONTINUATION = 9,
    LWS_WRITE_BUFLIST = 0x20,
    LWS_WRITE_NO_FIN = 0x40,
    LWS_WRITE_H2_STREAM_END = 0x80,
    LWS_WRITE_CLIENT_IGNORE_XOR_MASK = 0x80
};

struct lws_write_passthru {
    struct lws *wsi;
    unsigned char *buf;
    size_t len;
    enum lws_write_protocol wp;
};

LWS_VISIBLE LWS_EXTERN int lws_write(struct lws *wsi, unsigned char *buf, size_t len, enum lws_write_protocol protocol);

#define lws_write_http(wsi, buf, len) lws_write(wsi, (unsigned char *)(buf), len, LWS_WRITE_HTTP)

static LWS_INLINE int lws_write_ws_flags(int initial, int is_start, int is_end) {
    int r;
    if (is_start) r = initial;
    else r = LWS_WRITE_CONTINUATION;
    if (!is_end) r |= LWS_WRITE_NO_FIN;
    return r;
}

LWS_VISIBLE LWS_EXTERN int LWS_WARN_UNUSED_RESULT lws_raw_transaction_completed(struct lws *wsi);

#endif // __LWS_WRITE_H__

// By GST @Date