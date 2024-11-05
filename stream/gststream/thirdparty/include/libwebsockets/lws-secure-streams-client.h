//GST
struct lws_sspc_handle;

#if defined(LWS_SS_USE_SSPC)
#define lws_ss_handle lws_sspc_handle
#define lws_ss_create lws_sspc_create
#define lws_ss_destroy lws_sspc_destroy
#define lws_ss_request_tx lws_sspc_request_tx
#define lws_ss_request_tx_len lws_sspc_request_tx_len
#define lws_ss_client_connect lws_sspc_client_connect
#define lws_ss_get_sequencer lws_sspc_get_sequencer
#define lws_ss_proxy_create lws_sspc_proxy_create
#define lws_ss_get_context lws_sspc_get_context
#define lws_ss_rideshare lws_sspc_rideshare
#define lws_ss_set_metadata lws_sspc_set_metadata
#define lws_ss_get_metadata lws_sspc_get_metadata
#define lws_ss_add_peer_tx_credit lws_sspc_add_peer_tx_credit
#define lws_ss_get_est_peer_tx_credit lws_sspc_get_est_peer_tx_credit
#define lws_ss_start_timeout lws_sspc_start_timeout
#define lws_ss_cancel_timeout lws_sspc_cancel_timeout
#define lws_ss_to_user_object lws_sspc_to_user_object
#define lws_ss_change_handlers lws_sspc_change_handlers
#define lws_smd_ss_rx_forward lws_smd_sspc_rx_forward
#define lws_ss_tag lws_sspc_tag
#define _lws_fi_user_ss_fi _lws_fi_user_sspc_fi
#define lwsl_ss_get_cx lwsl_sspc_get_cx

LWS_VISIBLE LWS_EXTERN void lws_log_prepend_sspc(struct lws_log_cx *cx, void *obj, char **p, char *e);

LWS_VISIBLE LWS_EXTERN struct lws_log_cx *lwsl_sspc_get_cx(struct lws_sspc_handle *ss);

#undef lwsl_ss
#define lwsl_ss lwsl_sspc

#undef lwsl_hexdump_ss
#define lwsl_hexdump_ss lwsl_hexdump_sspc
#endif

#define lwsl_sspc(_h, _fil, ...) _lws_log_cx(lwsl_sspc_get_cx(_h), lws_log_prepend_sspc, _h, _fil, __func__, __VA_ARGS__)

#define lwsl_hexdump_sspc(_h, _fil, _buf, _len) lwsl_hexdump_level_cx(lwsl_sspc_get_cx(_h), lws_log_prepend_sspc, _h, _fil, _buf, _len)

#if (_LWS_ENABLED_LOGS & LLL_ERR)
#define lwsl_sspc_err(_w, ...) lwsl_sspc(_w, LLL_ERR, __VA_ARGS__)
#else
#define lwsl_sspc_err(_w, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_WARN)
#define lwsl_sspc_warn(_w, ...) lwsl_sspc(_w, LLL_WARN, __VA_ARGS__)
#else
#define lwsl_sspc_warn(_w, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_NOTICE)
#define lwsl_sspc_notice(_w, ...) lwsl_sspc(_w, LLL_NOTICE, __VA_ARGS__)
#else
#define lwsl_sspc_notice(_w, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_INFO)
#define lwsl_sspc_info(_w, ...) lwsl_sspc(_w, LLL_INFO, __VA_ARGS__)
#else
#define lwsl_sspc_info(_w, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_DEBUG)
#define lwsl_sspc_debug(_w, ...) lwsl_sspc(_w, LLL_DEBUG, __VA_ARGS__)
#else
#define lwsl_sspc_debug(_w, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_PARSER)
#define lwsl_sspc_parser(_w, ...) lwsl_sspc(_w, LLL_PARSER, __VA_ARGS__)
#else
#define lwsl_sspc_parser(_w, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_HEADER)
#define lwsl_sspc_header(_w, ...) lwsl_sspc(_w, LLL_HEADER, __VA_ARGS__)
#else
#define lwsl_sspc_header(_w, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_EXT)
#define lwsl_sspc_ext(_w, ...) lwsl_sspc(_w, LLL_EXT, __VA_ARGS__)
#else
#define lwsl_sspc_ext(_w, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_CLIENT)
#define lwsl_sspc_client(_w, ...) lwsl_sspc(_w, LLL_CLIENT, __VA_ARGS__)
#else
#define lwsl_sspc_client(_w, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_LATENCY)
#define lwsl_sspc_latency(_w, ...) lwsl_sspc(_w, LLL_LATENCY, __VA_ARGS__)
#else
#define lwsl_sspc_latency(_w, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_THREAD)
#define lwsl_sspc_thread(_w, ...) lwsl_sspc(_w, LLL_THREAD, __VA_ARGS__)
#else
#define lwsl_sspc_thread(_w, ...) do {} while(0)
#endif

#if (_LWS_ENABLED_LOGS & LLL_USER)
#define lwsl_sspc_user(_w, ...) lwsl_sspc(_w, LLL_USER, __VA_ARGS__)
#else
#define lwsl_sspc_user(_w, ...) do {} while(0)
#endif

#define lwsl_hexdump_sspc_err(_v, ...) lwsl_hexdump_sspc(_v, LLL_ERR, __VA_ARGS__)
#define lwsl_hexdump_sspc_warn(_v, ...) lwsl_hexdump_sspc(_v, LLL_WARN, __VA_ARGS__)
#define lwsl_hexdump_sspc_notice(_v, ...) lwsl_hexdump_sspc(_v, LLL_NOTICE, __VA_ARGS__)
#define lwsl_hexdump_sspc_info(_v, ...) lwsl_hexdump_sspc(_v, LLL_INFO, __VA_ARGS__)
#define lwsl_hexdump_sspc_debug(_v, ...) lwsl_hexdump_sspc(_v, LLL_DEBUG, __VA_ARGS__)

LWS_VISIBLE LWS_EXTERN int lws_sspc_create(struct lws_context *context, int tsi, const lws_ss_info_t *ssi, void *opaque_user_data, struct lws_sspc_handle **ppss, struct lws_sequencer *seq_owner, const char **ppayload_fmt);

LWS_VISIBLE LWS_EXTERN void lws_sspc_destroy(struct lws_sspc_handle **ppss);

LWS_VISIBLE LWS_EXTERN lws_ss_state_return_t lws_sspc_request_tx(struct lws_sspc_handle *pss);

LWS_VISIBLE LWS_EXTERN lws_ss_state_return_t lws_sspc_request_tx_len(struct lws_sspc_handle *h, unsigned long len);

LWS_VISIBLE LWS_EXTERN lws_ss_state_return_t lws_sspc_client_connect(struct lws_sspc_handle *h);

LWS_VISIBLE LWS_EXTERN struct lws_sequencer *lws_sspc_get_sequencer(struct lws_sspc_handle *h);

LWS_VISIBLE LWS_EXTERN int lws_sspc_proxy_create(struct lws_context *context);

LWS_VISIBLE LWS_EXTERN struct lws_context *lws_sspc_get_context(struct lws_sspc_handle *h);

LWS_VISIBLE extern const struct lws_protocols lws_sspc_protocols[2];

LWS_VISIBLE LWS_EXTERN const char *lws_sspc_rideshare(struct lws_sspc_handle *h);

LWS_VISIBLE LWS_EXTERN int lws_sspc_set_metadata(struct lws_sspc_handle *h, const char *name, const void *value, size_t len);

LWS_VISIBLE LWS_EXTERN int lws_sspc_get_metadata(struct lws_sspc_handle *h, const char *name, const void **value, size_t *len);

LWS_VISIBLE LWS_EXTERN int lws_sspc_add_peer_tx_credit(struct lws_sspc_handle *h, int32_t add);

LWS_VISIBLE LWS_EXTERN int lws_sspc_get_est_peer_tx_credit(struct lws_sspc_handle *h);

LWS_VISIBLE LWS_EXTERN void lws_sspc_start_timeout(struct lws_sspc_handle *h, unsigned int timeout_ms);

LWS_VISIBLE LWS_EXTERN void lws_sspc_cancel_timeout(struct lws_sspc_handle *h);

LWS_VISIBLE LWS_EXTERN void *lws_sspc_to_user_object(struct lws_sspc_handle *h);

LWS_VISIBLE LWS_EXTERN void lws_sspc_change_handlers(struct lws_sspc_handle *h, lws_ss_state_return_t (*rx)(void *userobj, const uint8_t *buf, size_t len, int flags), lws_ss_state_return_t (*tx)(void *userobj, lws_ss_tx_ordinal_t ord, uint8_t *buf, size_t *len, int *flags), lws_ss_state_return_t (*state)(void *userobj, void *h_src, lws_ss_constate_t state, lws_ss_tx_ordinal_t ack));

const char *lws_sspc_tag(struct lws_sspc_handle *h);
// By GST @Date