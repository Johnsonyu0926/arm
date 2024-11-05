#define LLL_ERR     (1 << 0)
#define LLL_WARN    (1 << 1)
#define LLL_NOTICE    (1 << 2)
#define LLL_INFO    (1 << 3)
#define LLL_DEBUG   (1 << 4)
#define LLL_PARSER    (1 << 5)
#define LLL_HEADER    (1 << 6)
#define LLL_EXT     (1 << 7)
#define LLL_CLIENT    (1 << 8)
#define LLL_LATENCY   (1 << 9)
#define LLL_USER    (1 << 10)
#define LLL_THREAD    (1 << 11)

#define LLL_COUNT   (12)

#define LLLF_SECRECY_PII  (1 << 16)
#define LLLF_SECRECY_BEARER (1 << 17)
#define LLLF_LOG_TIMESTAMP  (1 << 18)
#define LLLF_LOG_CONTEXT_AWARE  (1 << 30)

struct lws_log_cx;

typedef void (*lws_log_emit_t)(int level, const char *line);
typedef void (*lws_log_emit_cx_t)(struct lws_log_cx *cx, int level, const char *line, size_t len);
typedef void (*lws_log_prepend_cx_t)(struct lws_log_cx *cx, void *obj, char **p, char *e);
typedef void (*lws_log_use_cx_t)(struct lws_log_cx *cx, int _new);

typedef struct lws_log_cx {
  union {
    lws_log_emit_t    emit;
    lws_log_emit_cx_t emit_cx;
  } u;
  lws_log_use_cx_t    refcount_cb;
  lws_log_prepend_cx_t    prepend;
  struct lws_log_cx   *parent;
  void        *opaque;
  void        *stg;
  uint32_t      lll_flags;
  int32_t       refcount;
} lws_log_cx_t;

LWS_VISIBLE LWS_EXTERN int lwsl_timestamp(int level, char *p, size_t len);

#if defined(LWS_PLAT_OPTEE) && !defined(LWS_WITH_NETWORK)
#define _lws_log(aaa, ...) SMSG(__VA_ARGS__)
#else
LWS_VISIBLE LWS_EXTERN void _lws_log(int filter, const char *format, ...) LWS_FORMAT(2);
LWS_VISIBLE LWS_EXTERN void _lws_logv(int filter, const char *format, va_list vl);
#endif

struct lws_vhost;
struct lws;

LWS_VISIBLE LWS_EXTERN struct lws_log_cx * lwsl_context_get_cx(struct lws_context *cx);
LWS_VISIBLE LWS_EXTERN struct lws_log_cx * lwsl_vhost_get_cx(struct lws_vhost *vh);
LWS_VISIBLE LWS_EXTERN struct lws_log_cx * lwsl_wsi_get_cx(struct lws *wsi);
#if defined(LWS_WITH_SECURE_STREAMS)
struct lws_ss_handle;
struct lws_sspc_handle;
LWS_VISIBLE LWS_EXTERN struct lws_log_cx * lwsl_ss_get_cx(struct lws_ss_handle *ss);
LWS_VISIBLE LWS_EXTERN struct lws_log_cx * lwsl_sspc_get_cx(struct lws_sspc_handle *ss);
#endif

LWS_VISIBLE LWS_EXTERN void lws_log_emit_cx_file(struct lws_log_cx *cx, int level, const char *line, size_t len);
LWS_VISIBLE LWS_EXTERN void lws_log_use_cx_file(struct lws_log_cx *cx, int _new);
LWS_VISIBLE LWS_EXTERN void lws_log_prepend_context(struct lws_log_cx *cx, void *obj, char **p, char *e);
LWS_VISIBLE LWS_EXTERN void lws_log_prepend_vhost(struct lws_log_cx *cx, void *obj, char **p, char *e);
LWS_VISIBLE LWS_EXTERN void lws_log_prepend_wsi(struct lws_log_cx *cx, void *obj, char **p, char *e);
#if defined(LWS_WITH_SECURE_STREAMS)
LWS_VISIBLE LWS_EXTERN void lws_log_prepend_ss(struct lws_log_cx *cx, void *obj, char **p, char *e);
LWS_VISIBLE LWS_EXTERN void lws_log_prepend_sspc(struct lws_log_cx *cx, void *obj, char **p, char *e);
#endif

LWS_VISIBLE LWS_EXTERN void _lws_log_cx(lws_log_cx_t *cx, lws_log_prepend_cx_t prep, void *obj, int filter, const char *_fun, const char *format, ...) LWS_FORMAT(6);

#define lwsl_cx(_c, _fil, ...) _lws_log_cx(lwsl_context_get_cx(_c), lws_log_prepend_context, _c, _fil, __func__, __VA_ARGS__)
#define lwsl_vhost(_v, _fil, ...) _lws_log_cx(lwsl_vhost_get_cx(_v), lws_log_prepend_vhost, _v, _fil, __func__, __VA_ARGS__)
#define lwsl_wsi(_w, _fil, ...) _lws_log_cx(lwsl_wsi_get_cx(_w), lws_log_prepend_wsi, _w, _fil, __func__, __VA_ARGS__)
#define lwsl_ss(_h, _fil, ...) _lws_log_cx(lwsl_ss_get_cx(_h), lws_log_prepend_ss, _h, _fil, __func__, __VA_ARGS__)

#define lwsl_hexdump_context(_c, _fil, _buf, _len) lwsl_hexdump_level_cx(lwsl_context_get_cx(_c), lws_log_prepend_context, _c, _fil, _buf, _len)
#define lwsl_hexdump_vhost(_v, _fil, _buf, _len) lwsl_hexdump_level_cx(lwsl_vhost_get_cx(_v), lws_log_prepend_vhost, _v, _fil, _buf, _len)
#define lwsl_hexdump_wsi(_w, _fil, _buf, _len) lwsl_hexdump_level_cx(lwsl_wsi_get_cx(_w), lws_log_prepend_wsi, _w, _fil, _buf, _len)
#define lwsl_hexdump_ss(_h, _fil, _buf, _len) lwsl_hexdump_level_cx(lwsl_ss_get_cx(_h), lws_log_prepend_ss, _h, _fil, _buf, _len)

#if defined(_DEBUG)
 #if defined(LWS_WITH_NO_LOGS)
  #define _LWS_LINIT (LLL_ERR | LLL_USER)
 #else
   #define _LWS_LINIT ((1 << LLL_COUNT) - 1)
 #endif
#else
#if defined(LWS_WITH_NO_LOGS)
#define _LWS_LINIT (LLL_ERR | LLL_USER)
#else
 #define _LWS_LINIT (LLL_ERR | LLL_USER | LLL_WARN | LLL_NOTICE)
#endif
#endif

#if defined(LWS_LOGGING_BITFIELD_SET)
 #define _LWS_LBS (LWS_LOGGING_BITFIELD_SET)
#else
 #define _LWS_LBS 0
#endif

#if defined(LWS_LOGGING_BITFIELD_CLEAR)
 #define _LWS_LBC (LWS_LOGGING_BITFIELD_CLEAR)
#else
 #define _LWS_LBC 0
#endif

#define _LWS_ENABLED_LOGS (((_LWS_LINIT) | (_LWS_LBS)) & ~(_LWS_LBC))

#if (_LWS_ENABLED_LOGS & LLL_ERR)
#define lwsl_err(...) _lws_log(LLL_ERR, __VA_ARGS__)
#else
#define lwsl_err(...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_WARN)
#define lwsl_warn(...) _lws_log(LLL_WARN, __VA_ARGS__)
#else
#define lwsl_warn(...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_NOTICE)
#define lwsl_notice(...) _lws_log(LLL_NOTICE, __VA_ARGS__)
#else
#define lwsl_notice(...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_INFO)
#define lwsl_info(...) _lws_log(LLL_INFO, __VA_ARGS__)
#else
#define lwsl_info(...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_DEBUG)
#define lwsl_debug(...) _lws_log(LLL_DEBUG, __VA_ARGS__)
#else
#define lwsl_debug(...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_PARSER)
#define lwsl_parser(...) _lws_log(LLL_PARSER, __VA_ARGS__)
#else
#define lwsl_parser(...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_HEADER)
#define lwsl_header(...) _lws_log(LLL_HEADER, __VA_ARGS__)
#else
#define lwsl_header(...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_EXT)
#define lwsl_ext(...) _lws_log(LLL_EXT, __VA_ARGS__)
#else
#define lwsl_ext(...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_CLIENT)
#define lwsl_client(...) _lws_log(LLL_CLIENT, __VA_ARGS__)
#else
#define lwsl_client(...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_LATENCY)
#define lwsl_latency(...) _lws_log(LLL_LATENCY, __VA_ARGS__)
#else
#define lwsl_latency(...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_THREAD)
#define lwsl_thread(...) _lws_log(LLL_THREAD, __VA_ARGS__)
#else
#define lwsl_thread(...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_USER)
#define lwsl_user(...) _lws_log(LLL_USER, __VA_ARGS__)
#else
#define lwsl_user(...) do {} while(0)
#endif

#define lwsl_hexdump_err(...) lwsl_hexdump_level(LLL_ERR, __VA_ARGS__)
#define lwsl_hexdump_warn(...) lwsl_hexdump_level(LLL_WARN, __VA_ARGS__)
#define lwsl_hexdump_notice(...) lwsl_hexdump_level(LLL_NOTICE, __VA_ARGS__)
#define lwsl_hexdump_info(...) lwsl_hexdump_level(LLL_INFO, __VA_ARGS__)
#define lwsl_hexdump_debug(...) lwsl_hexdump_level(LLL_DEBUG, __VA_ARGS__)

#if (_LWS_ENABLED_LOGS & LLL_ERR)
#define lwsl_cx_err(_c, ...) lwsl_cx(_c, LLL_ERR, __VA_ARGS__)
#else
#define lwsl_cx_err(_c, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_WARN)
#define lwsl_cx_warn(_c, ...) lwsl_cx(_c, LLL_WARN, __VA_ARGS__)
#else
#define lwsl_cx_warn(_c, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_NOTICE)
#define lwsl_cx_notice(_c, ...) lwsl_cx(_c, LLL_NOTICE, __VA_ARGS__)
#else
#define lwsl_cx_notice(_c, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_INFO)
#define lwsl_cx_info(_c, ...) lwsl_cx(_c, LLL_INFO, __VA_ARGS__)
#else
#define lwsl_cx_info(_c, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_DEBUG)
#define lwsl_cx_debug(_c, ...) lwsl_cx(_c, LLL_DEBUG, __VA_ARGS__)
#else
#define lwsl_cx_debug(_c, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_PARSER)
#define lwsl_cx_parser(_c, ...) lwsl_cx(_c, LLL_PARSER, __VA_ARGS__)
#else
#define lwsl_cx_parser(_c, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_HEADER)
#define lwsl_cx_header(_c, ...) lwsl_cx(_c, LLL_HEADER, __VA_ARGS__)
#else
#define lwsl_cx_header(_c, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_EXT)
#define lwsl_cx_ext(_c, ...) lwsl_cx(_c, LLL_EXT, __VA_ARGS__)
#else
#define lwsl_cx_ext(_c, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_CLIENT)
#define lwsl_cx_client(_c, ...) lwsl_cx(_c, LLL_CLIENT, __VA_ARGS__)
#else
#define lwsl_cx_client(_c, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_LATENCY)
#define lwsl_cx_latency(_c, ...) lwsl_cx(_c, LLL_LATENCY, __VA_ARGS__)
#else
#define lwsl_cx_latency(_c, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_THREAD)
#define lwsl_cx_thread(_c, ...) lwsl_cx(_c, LLL_THREAD, __VA_ARGS__)
#else
#define lwsl_cx_thread(_c, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_USER)
#define lwsl_cx_user(_c, ...) lwsl_cx(_c, LLL_USER, __VA_ARGS__)
#else
#define lwsl_cx_user(_c, ...) do {} while(0)
#endif

#define lwsl_hexdump_cx_err(_c, ...)    lwsl_hexdump_context(_c, LLL_ERR, __VA_ARGS__)
#define lwsl_hexdump_cx_warn(_c, ...)   lwsl_hexdump_context(_c, LLL_WARN, __VA_ARGS__)
#define lwsl_hexdump_cx_notice(_c, ...) lwsl_hexdump_context(_c, LLL_NOTICE, __VA_ARGS__)
#define lwsl_hexdump_cx_info(_c, ...)   lwsl_hexdump_context(_c, LLL_INFO, __VA_ARGS__)
#define lwsl_hexdump_cx_debug(_c, ...)  lwsl_hexdump_context(_c, LLL_DEBUG, __VA_ARGS__)

#if (_LWS_ENABLED_LOGS & LLL_ERR)
#define lwsl_vhost_err(_v, ...) lwsl_vhost(_v, LLL_ERR, __VA_ARGS__)
#else
#define lwsl_vhost_err(_v, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_WARN)
#define lwsl_vhost_warn(_v, ...) lwsl_vhost(_v, LLL_WARN, __VA_ARGS__)
#else
#define lwsl_vhost_warn(_v, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_NOTICE)
#define lwsl_vhost_notice(_v, ...) lwsl_vhost(_v, LLL_NOTICE, __VA_ARGS__)
#else
#define lwsl_vhost_notice(_v, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_INFO)
#define lwsl_vhost_info(_v, ...) lwsl_vhost(_v, LLL_INFO, __VA_ARGS__)
#else
#define lwsl_vhost_info(_v, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_DEBUG)
#define lwsl_vhost_debug(_v, ...) lwsl_vhost(_v, LLL_DEBUG, __VA_ARGS__)
#else
#define lwsl_vhost_debug(_v, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_PARSER)
#define lwsl_vhost_parser(_v, ...) lwsl_vhost(_v, LLL_PARSER, __VA_ARGS__)
#else
#define lwsl_vhost_parser(_v, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_HEADER)
#define lwsl_vhost_header(_v, ...) lwsl_vhost(_v, LLL_HEADER, __VA_ARGS__)
#else
#define lwsl_vhost_header(_v, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_EXT)
#define lwsl_vhost_ext(_v, ...) lwsl_vhost(_v, LLL_EXT, __VA_ARGS__)
#else
#define lwsl_vhost_ext(_v, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_CLIENT)
#define lwsl_vhost_client(_v, ...) lwsl_vhost(_v, LLL_CLIENT, __VA_ARGS__)
#else
#define lwsl_vhost_client(_v, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_LATENCY)
#define lwsl_vhost_latency(_v, ...) lwsl_vhost(_v, LLL_LATENCY, __VA_ARGS__)
#else
#define lwsl_vhost_latency(_v, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_THREAD)
#define lwsl_vhost_thread(_v, ...) lwsl_vhost(_v, LLL_THREAD, __VA_ARGS__)
#else
#define lwsl_vhost_thread(_v, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_USER)
#define lwsl_vhost_user(_v, ...) lwsl_vhost(_v, LLL_USER, __VA_ARGS__)
#else
#define lwsl_vhost_user(_v, ...) do {} while(0)
#endif

#define lwsl_hexdump_vhost_err(_v, ...)    lwsl_hexdump_vhost(_v, LLL_ERR, __VA_ARGS__)
#define lwsl_hexdump_vhost_warn(_v, ...)   lwsl_hexdump_vhost(_v, LLL_WARN, __VA_ARGS__)
#define lwsl_hexdump_vhost_notice(_v, ...) lwsl_hexdump_vhost(_v, LLL_NOTICE, __VA_ARGS__)
#define lwsl_hexdump_vhost_info(_v, ...)   lwsl_hexdump_vhost(_v, LLL_INFO, __VA_ARGS__)
#define lwsl_hexdump_vhost_debug(_v, ...)  lwsl_hexdump_vhost(_v, LLL_DEBUG, __VA_ARGS__)

#if (_LWS_ENABLED_LOGS & LLL_ERR)
#define lwsl_wsi_err(_w, ...) lwsl_wsi(_w, LLL_ERR, __VA_ARGS__)
#else
#define lwsl_wsi_err(_w, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_WARN)
#define lwsl_wsi_warn(_w, ...) lwsl_wsi(_w, LLL_WARN, __VA_ARGS__)
#else
#define lwsl_wsi_warn(_w, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_NOTICE)
#define lwsl_wsi_notice(_w, ...) lwsl_wsi(_w, LLL_NOTICE, __VA_ARGS__)
#else
#define lwsl_wsi_notice(_w, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_INFO)
#define lwsl_wsi_info(_w, ...) lwsl_wsi(_w, LLL_INFO, __VA_ARGS__)
#else
#define lwsl_wsi_info(_w, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_DEBUG)
#define lwsl_wsi_debug(_w, ...) lwsl_wsi(_w, LLL_DEBUG, __VA_ARGS__)
#else
#define lwsl_wsi_debug(_w, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_PARSER)
#define lwsl_wsi_parser(_w, ...) lwsl_wsi(_w, LLL_PARSER, __VA_ARGS__)
#else
#define lwsl_wsi_parser(_w, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_HEADER)
#define lwsl_wsi_header(_w, ...) lwsl_wsi(_w, LLL_HEADER, __VA_ARGS__)
#else
#define lwsl_wsi_header(_w, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_EXT)
#define lwsl_wsi_ext(_w, ...) lwsl_wsi(_w, LLL_EXT, __VA_ARGS__)
#else
#define lwsl_wsi_ext(_w, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_CLIENT)
#define lwsl_wsi_client(_w, ...) lwsl_wsi(_w, LLL_CLIENT, __VA_ARGS__)
#else
#define lwsl_wsi_client(_w, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_LATENCY)
#define lwsl_wsi_latency(_w, ...) lwsl_wsi(_w, LLL_LATENCY, __VA_ARGS__)
#else
#define lwsl_wsi_latency(_w, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_THREAD)
#define lwsl_wsi_thread(_w, ...) lwsl_wsi(_w, LLL_THREAD, __VA_ARGS__)
#else
#define lwsl_wsi_thread(_w, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_USER)
#define lwsl_wsi_user(_w, ...) lwsl_wsi(_w, LLL_USER, __VA_ARGS__)
#else
#define lwsl_wsi_user(_w, ...) do {} while(0)
#endif

#define lwsl_hexdump_wsi_err(_v, ...)    lwsl_hexdump_wsi(_v, LLL_ERR, __VA_ARGS__)
#define lwsl_hexdump_wsi_warn(_v, ...)   lwsl_hexdump_wsi(_v, LLL_WARN, __VA_ARGS__)
#define lwsl_hexdump_wsi_notice(_v, ...) lwsl_hexdump_wsi(_v, LLL_NOTICE, __VA_ARGS__)
#define lwsl_hexdump_wsi_info(_v, ...)   lwsl_hexdump_wsi(_v, LLL_INFO, __VA_ARGS__)
#define lwsl_hexdump_wsi_debug(_v, ...)  lwsl_hexdump_wsi(_v, LLL_DEBUG, __VA_ARGS__)

#if (_LWS_ENABLED_LOGS & LLL_ERR)
#define lwsl_ss_err(_w, ...) lwsl_ss(_w, LLL_ERR, __VA_ARGS__)
#else
#define lwsl_ss_err(_w, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_WARN)
#define lwsl_ss_warn(_w, ...) lwsl_ss(_w, LLL_WARN, __VA_ARGS__)
#else
#define lwsl_ss_warn(_w, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_NOTICE)
#define lwsl_ss_notice(_w, ...) lwsl_ss(_w, LLL_NOTICE, __VA_ARGS__)
#else
#define lwsl_ss_notice(_w, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_INFO)
#define lwsl_ss_info(_w, ...) lwsl_ss(_w, LLL_INFO, __VA_ARGS__)
#else
#define lwsl_ss_info(_w, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_DEBUG)
#define lwsl_ss_debug(_w, ...) lwsl_ss(_w, LLL_DEBUG, __VA_ARGS__)
#else
#define lwsl_ss_debug(_w, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_PARSER)
#define lwsl_ss_parser(_w, ...) lwsl_ss(_w, LLL_PARSER, __VA_ARGS__)
#else
#define lwsl_ss_parser(_w, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_HEADER)
#define lwsl_ss_header(_w, ...) lwsl_ss(_w, LLL_HEADER, __VA_ARGS__)
#else
#define lwsl_ss_header(_w, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_EXT)
#define lwsl_ss_ext(_w, ...) lwsl_ss(_w, LLL_EXT, __VA_ARGS__)
#else
#define lwsl_ss_ext(_w, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_CLIENT)
#define lwsl_ss_client(_w, ...) lwsl_ss(_w, LLL_CLIENT, __VA_ARGS__)
#else
#define lwsl_ss_client(_w, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_LATENCY)
#define lwsl_ss_latency(_w, ...) lwsl_ss(_w, LLL_LATENCY, __VA_ARGS__)
#else
#define lwsl_ss_latency(_w, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_THREAD)
#define lwsl_ss_thread(_w, ...) lwsl_ss(_w, LLL_THREAD, __VA_ARGS__)
#else
#define lwsl_ss_thread(_w, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_USER)
#define lwsl_ss_user(_w, ...) lwsl_ss(_w, LLL_USER, __VA_ARGS__)
#else
#define lwsl_ss_user(_w, ...) do {} while(0)
#endif

#define lwsl_hexdump_ss_err(_v, ...)    lwsl_hexdump_ss(_v, LLL_ERR, __VA_ARGS__)
#define lwsl_hexdump_ss_warn(_v, ...)   lwsl_hexdump_ss(_v, LLL_WARN, __VA_ARGS__)
#define lwsl_hexdump_ss_notice(_v, ...) lwsl_hexdump_ss(_v, LLL_NOTICE, __VA_ARGS__)
#define lwsl_hexdump_ss_info(_v, ...)   lwsl_hexdump_ss(_v, LLL_INFO, __VA_ARGS__)
#define lwsl_hexdump_ss_debug(_v, ...)  lwsl_hexdump_ss(_v, LLL_DEBUG, __VA_ARGS__)

LWS_VISIBLE LWS_EXTERN void lwsl_hexdump_level(int level, const void *vbuf, size_t len);
LWS_VISIBLE LWS_EXTERN void lwsl_hexdump_level_cx(lws_log_cx_t *cx, lws_log_prepend_cx_t prep, void *obj, int hexdump_level, const void *vbuf, size_t len);
LWS_VISIBLE LWS_EXTERN void lwsl_hexdump(const void *buf, size_t len);

static LWS_INLINE int lws_is_be(void) {
  const int probe = ~0xff;
  return *(const char *)&probe;
}

LWS_VISIBLE LWS_EXTERN void lws_set_log_level(int level, lws_log_emit_t log_emit_function);
LWS_VISIBLE LWS_EXTERN void lwsl_emit_syslog(int level, const char *line);
LWS_VISIBLE LWS_EXTERN void lwsl_emit_stderr(int level, const char *line);
LWS_VISIBLE LWS_EXTERN void lwsl_emit_stderr_notimestamp(int level, const char *line);
LWS_VISIBLE LWS_EXTERN int lwsl_visible(int level);

struct lws;

LWS_VISIBLE LWS_EXTERN const char * lws_wsi_tag(struct lws *wsi);
LWS_VISIBLE LWS_EXTERN void lwsl_refcount_cx(lws_log_cx_t *cx, int _new);
// By GST @Date