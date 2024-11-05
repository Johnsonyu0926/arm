enum lws_client_connect_ssl_connection_flags {
	LCCSCF_USE_SSL 				= (1 << 0),
	LCCSCF_ALLOW_SELFSIGNED			= (1 << 1),
	LCCSCF_SKIP_SERVER_CERT_HOSTNAME_CHECK	= (1 << 2),
	LCCSCF_ALLOW_EXPIRED			= (1 << 3),
	LCCSCF_ALLOW_INSECURE			= (1 << 4),
	LCCSCF_H2_QUIRK_NGHTTP2_END_STREAM	= (1 << 5),
	LCCSCF_H2_QUIRK_OVERFLOWS_TXCR		= (1 << 6),
	LCCSCF_H2_AUTH_BEARER			= (1 << 7),
	LCCSCF_H2_HEXIFY_AUTH_TOKEN		= (1 << 8),
	LCCSCF_H2_MANUAL_RXFLOW			= (1 << 9),
	LCCSCF_HTTP_MULTIPART_MIME		= (1 << 10),
	LCCSCF_HTTP_X_WWW_FORM_URLENCODED	= (1 << 11),
	LCCSCF_HTTP_NO_FOLLOW_REDIRECT		= (1 << 12),
	LCCSCF_PIPELINE				= (1 << 16),
	LCCSCF_MUXABLE_STREAM			= (1 << 17),
	LCCSCF_H2_PRIOR_KNOWLEDGE		= (1 << 18),
	LCCSCF_WAKE_SUSPEND__VALIDITY		= (1 << 19),
	LCCSCF_PRIORITIZE_READS			= (1 << 20),
	LCCSCF_SECSTREAM_CLIENT			= (1 << 21),
	LCCSCF_SECSTREAM_PROXY_LINK		= (1 << 22),
	LCCSCF_SECSTREAM_PROXY_ONWARD		= (1 << 23),
	LCCSCF_IP_LOW_LATENCY			= (1 << 24),
	LCCSCF_IP_HIGH_THROUGHPUT		= (1 << 25),
	LCCSCF_IP_HIGH_RELIABILITY		= (1 << 26),
	LCCSCF_IP_LOW_COST			= (1 << 27),
	LCCSCF_CONMON				= (1 << 28),
	LCCSCF_ACCEPT_TLS_DOWNGRADE_REDIRECTS	= (1 << 29),
	LCCSCF_CACHE_COOKIES			= (1 << 30),
};

struct lws_client_connect_info {
	struct lws_context *context;
	const char *address;
	int port;
	int ssl_connection;
	const char *path;
	const char *host;
	const char *origin;
	const char *protocol;
	int ietf_version_or_minus_one;
	void *userdata;
	const void *client_exts;
	const char *method;
	struct lws *parent_wsi;
	const char *uri_replace_from;
	const char *uri_replace_to;
	struct lws_vhost *vhost;
	struct lws **pwsi;
	const char *iface;
	const char *local_protocol_name;
	const char *alpn;
	struct lws_sequencer *seq;
	void *opaque_user_data;
	const lws_retry_bo_t *retry_and_idle_policy;
	int manual_initial_tx_credit;
	uint8_t sys_tls_client_cert;
	uint8_t priority;
#if defined(LWS_ROLE_MQTT)
	const lws_mqtt_client_connect_param_t *mqtt_cp;
#else
	void *mqtt_cp;
#endif
#if defined(LWS_WITH_SYS_FAULT_INJECTION)
	lws_fi_ctx_t fic;
#endif
	const char *fi_wsi_name;
	uint16_t keep_warm_secs;
	lws_log_cx_t *log_cx;
	void *_unused[4];
};

LWS_VISIBLE LWS_EXTERN struct lws *
lws_client_connect_via_info(const struct lws_client_connect_info *ccinfo);

LWS_VISIBLE LWS_EXTERN int
lws_init_vhost_client_ssl(const struct lws_context_creation_info *info,
			  struct lws_vhost *vhost);

LWS_VISIBLE LWS_EXTERN int
lws_http_client_read(struct lws *wsi, char **buf, int *len);

LWS_VISIBLE LWS_EXTERN unsigned int
lws_http_client_http_response(struct lws *wsi);

LWS_VISIBLE LWS_EXTERN int
lws_tls_client_vhost_extra_cert_mem(struct lws_vhost *vh,
		const uint8_t *der, size_t der_len);

LWS_VISIBLE LWS_EXTERN void
lws_client_http_body_pending(struct lws *wsi, int something_left_to_send);

LWS_VISIBLE LWS_EXTERN int
lws_client_http_multipart(struct lws *wsi, const char *name,
			  const char *filename, const char *content_type,
			  char **p, char *end);

LWS_VISIBLE LWS_EXTERN int
lws_http_basic_auth_gen(const char *user, const char *pw, char *buf, size_t len);

LWS_VISIBLE LWS_EXTERN int
lws_tls_session_is_reused(struct lws *wsi);

// By GST @Date