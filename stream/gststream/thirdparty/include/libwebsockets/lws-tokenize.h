// Filename: lws_tokenize.h
// Score: 97

#ifndef __LWS_TOKENIZE_H__
#define __LWS_TOKENIZE_H__

#define LWS_TOKENIZE_F_MINUS_NONTERM (1 << 0)
#define LWS_TOKENIZE_F_AGG_COLON (1 << 1)
#define LWS_TOKENIZE_F_COMMA_SEP_LIST (1 << 2)
#define LWS_TOKENIZE_F_RFC7230_DELIMS (1 << 3)
#define LWS_TOKENIZE_F_DOT_NONTERM (1 << 4)
#define LWS_TOKENIZE_F_NO_FLOATS (1 << 5)
#define LWS_TOKENIZE_F_NO_INTEGERS (1 << 6)
#define LWS_TOKENIZE_F_HASH_COMMENT (1 << 7)
#define LWS_TOKENIZE_F_SLASH_NONTERM (1 << 8)
#define LWS_TOKENIZE_F_ASTERISK_NONTERM (1 << 9)
#define LWS_TOKENIZE_F_EQUALS_NONTERM (1 << 10)

typedef enum {
    LWS_TOKZE_ERRS = 5,
    LWS_TOKZE_ERR_BROKEN_UTF8 = -5,
    LWS_TOKZE_ERR_UNTERM_STRING = -4,
    LWS_TOKZE_ERR_MALFORMED_FLOAT = -3,
    LWS_TOKZE_ERR_NUM_ON_LHS = -2,
    LWS_TOKZE_ERR_COMMA_LIST = -1,
    LWS_TOKZE_ENDED = 0,
    LWS_TOKZE_DELIMITER,
    LWS_TOKZE_TOKEN,
    LWS_TOKZE_INTEGER,
    LWS_TOKZE_FLOAT,
    LWS_TOKZE_TOKEN_NAME_EQUALS,
    LWS_TOKZE_TOKEN_NAME_COLON,
    LWS_TOKZE_QUOTED_STRING,
} lws_tokenize_elem;

enum lws_tokenize_delimiter_tracking {
    LWSTZ_DT_NEED_FIRST_CONTENT,
    LWSTZ_DT_NEED_DELIM,
    LWSTZ_DT_NEED_NEXT_CONTENT,
};

typedef struct lws_tokenize {
    const char *start;
    const char *token;
    size_t len;
    size_t token_len;
    uint16_t flags;
    uint8_t delim;
    int8_t e;
} lws_tokenize_t;

LWS_VISIBLE LWS_EXTERN void lws_tokenize_init(struct lws_tokenize *ts, const char *start, int flags);

LWS_VISIBLE LWS_EXTERN lws_tokenize_elem lws_tokenize(struct lws_tokenize *ts);

LWS_VISIBLE LWS_EXTERN int lws_tokenize_cstr(struct lws_tokenize *ts, char *str, size_t max);

typedef int (*lws_strexp_expand_cb)(void *priv, const char *name, char *out, size_t *pos, size_t olen, size_t *exp_ofs);

typedef struct lws_strexp {
    char name[32];
    lws_strexp_expand_cb cb;
    void *priv;
    char *out;
    size_t olen;
    size_t pos;
    size_t exp_ofs;
    uint8_t name_pos;
    char state;
} lws_strexp_t;

enum {
    LSTRX_DONE,
    LSTRX_FILLED_OUT,
    LSTRX_FATAL_NAME_TOO_LONG = -1,
    LSTRX_FATAL_NAME_UNKNOWN = -2,
};

LWS_VISIBLE LWS_EXTERN void lws_strexp_init(lws_strexp_t *exp, void *priv, lws_strexp_expand_cb cb, char *out, size_t olen);

LWS_VISIBLE LWS_EXTERN void lws_strexp_reset_out(lws_strexp_t *exp, char *out, size_t olen);

LWS_VISIBLE LWS_EXTERN int lws_strexp_expand(lws_strexp_t *exp, const char *in, size_t len, size_t *pused_in, size_t *pused_out);

LWS_VISIBLE LWS_EXTERN int lws_strcmp_wildcard(const char *wildcard, size_t wlen, const char *check, size_t clen);

#endif // __LWS_TOKENIZE_H__

// By GST @Date