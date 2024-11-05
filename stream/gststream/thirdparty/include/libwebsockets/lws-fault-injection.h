typedef struct lws_xos {
    uint64_t s[4];
} lws_xos_t;

LWS_VISIBLE LWS_EXTERN void
lws_xos_init(struct lws_xos *xos, uint64_t seed);

LWS_VISIBLE LWS_EXTERN uint64_t LWS_WARN_UNUSED_RESULT
lws_xos(struct lws_xos *xos);

LWS_VISIBLE LWS_EXTERN int LWS_WARN_UNUSED_RESULT
lws_xos_percent(struct lws_xos *xos, int percent);

#if defined(LWS_WITH_SYS_FAULT_INJECTION)

enum {
    LWSFI_ALWAYS,
    LWSFI_DETERMINISTIC,
    LWSFI_PROBABILISTIC,
    LWSFI_PATTERN,
    LWSFI_PATTERN_ALLOC,
    LWSFI_RANGE
};

typedef struct lws_fi {
    const char *name;
    const uint8_t *pattern;
    uint64_t pre;
    uint64_t count;
    uint64_t times;
    char type;
} lws_fi_t;

typedef struct lws_fi_ctx {
    lws_dll2_owner_t fi_owner;
    struct lws_xos xos;
    const char *name;
} lws_fi_ctx_t;

LWS_VISIBLE LWS_EXTERN int
lws_fi(const lws_fi_ctx_t *fic, const char *fi_name);

LWS_VISIBLE LWS_EXTERN int
lws_fi_range(const lws_fi_ctx_t *fic, const char *name, uint64_t *result);

LWS_VISIBLE LWS_EXTERN int
lws_fi_add(lws_fi_ctx_t *fic, const lws_fi_t *fi);

LWS_VISIBLE LWS_EXTERN void
lws_fi_remove(lws_fi_ctx_t *fic, const char *name);

LWS_VISIBLE LWS_EXTERN void
lws_fi_import(lws_fi_ctx_t *fic_dest, const lws_fi_ctx_t *fic_src);

LWS_VISIBLE LWS_EXTERN void
lws_fi_inherit_copy(lws_fi_ctx_t *fic_dest, const lws_fi_ctx_t *fic_src,
            const char *scope, const char *value);

LWS_VISIBLE LWS_EXTERN void
lws_fi_destroy(const lws_fi_ctx_t *fic);

LWS_VISIBLE LWS_EXTERN void
lws_fi_deserialize(lws_fi_ctx_t *fic, const char *sers);

LWS_VISIBLE LWS_EXTERN int
_lws_fi_user_wsi_fi(struct lws *wsi, const char *name);
LWS_VISIBLE LWS_EXTERN int
_lws_fi_user_context_fi(struct lws_context *ctx, const char *name);

#if defined(LWS_WITH_SECURE_STREAMS)
struct lws_ss_handle;
LWS_VISIBLE LWS_EXTERN int
_lws_fi_user_ss_fi(struct lws_ss_handle *h, const char *name);
#if defined(LWS_WITH_SECURE_STREAMS_PROXY_API)
struct lws_sspc_handle;
LWS_VISIBLE LWS_EXTERN int
_lws_fi_user_sspc_fi(struct lws_sspc_handle *h, const char *name);
#endif
#endif

#define lws_fi_user_wsi_fi(_wsi, _name) _lws_fi_user_wsi_fi(_wsi, _name)
#define lws_fi_user_context_fi(_ctx, _name) _lws_fi_user_context_fi(_ctx, _name)
#define lws_fi_user_ss_fi(_h, _name) _lws_fi_user_ss_fi(_h, _name)
#define lws_fi_user_sspc_fi(_h, _name) _lws_fi_user_sspc_fi(_h, _name)

#else

#define lws_fi(_fi_name, _fic) (0)
#define lws_fi_destroy(_x)
#define lws_fi_inherit_copy(_a, _b, _c, _d)
#define lws_fi_deserialize(_x, _y)
#define lws_fi_user_wsi_fi(_wsi, _name) (0)
#define lws_fi_user_context_fi(_wsi, _name) (0)
#define lws_fi_user_ss_fi(_h, _name) (0)
#define lws_fi_user_sspc_fi(_h, _name) (0)

#endif
// By GST @Date