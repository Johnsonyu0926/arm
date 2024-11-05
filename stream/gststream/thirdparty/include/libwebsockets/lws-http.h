# LWS HTTP API
#define LWS_RECOMMENDED_MIN_HEADER_SPACE 2048

LWS_VISIBLE LWS_EXTERN const char *
lws_get_mimetype(const char *file, const struct lws_http_mount *m);

LWS_VISIBLE LWS_EXTERN int
lws_serve_http_file(struct lws *wsi, const char *file, const char *content_type,
		    const char *other_headers, int other_headers_len);

LWS_VISIBLE LWS_EXTERN int
lws_serve_http_file_fragment(struct lws *wsi);

enum http_status {
	HTTP_STATUS_CONTINUE                    = 100,

	HTTP_STATUS_OK                          = 200,
	HTTP_STATUS_NO_CONTENT                  = 204,
	HTTP_STATUS_PARTIAL_CONTENT             = 206,

	HTTP_STATUS_MOVED_PERMANENTLY           = 301,
	HTTP_STATUS_FOUND                       = 302,
	HTTP_STATUS_SEE_OTHER                   = 303,
	HTTP_STATUS_NOT_MODIFIED                = 304,

	HTTP_STATUS_BAD_REQUEST                 = 400,
	HTTP_STATUS_UNAUTHORIZED,
	HTTP_STATUS_PAYMENT_REQUIRED,
	HTTP_STATUS_FORBIDDEN,
	HTTP_STATUS_NOT_FOUND,
	HTTP_STATUS_METHOD_NOT_ALLOWED,
	HTTP_STATUS_NOT_ACCEPTABLE,
	HTTP_STATUS_PROXY_AUTH_REQUIRED,
	HTTP_STATUS_REQUEST_TIMEOUT,
	HTTP_STATUS_CONFLICT,
	HTTP_STATUS_GONE,
	HTTP_STATUS_LENGTH_REQUIRED,
	HTTP_STATUS_PRECONDITION_FAILED,
	HTTP_STATUS_REQ_ENTITY_TOO_LARGE,
	HTTP_STATUS_REQ_URI_TOO_LONG,
	HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE,
	HTTP_STATUS_REQ_RANGE_NOT_SATISFIABLE,
	HTTP_STATUS_EXPECTATION_FAILED,

	HTTP_STATUS_INTERNAL_SERVER_ERROR       = 500,
	HTTP_STATUS_NOT_IMPLEMENTED,
	HTTP_STATUS_BAD_GATEWAY,
	HTTP_STATUS_SERVICE_UNAVAILABLE,
	HTTP_STATUS_GATEWAY_TIMEOUT,
	HTTP_STATUS_HTTP_VERSION_NOT_SUPPORTED,
};

struct lws_process_html_args {
	char *p; /**< pointer to the buffer containing the data */
	int len; /**< length of the original data at p */
	int max_len; /**< maximum length we can grow the data to */
	int final; /**< set if this is the last chunk of the file */
	int chunked; /**< 0 == unchunked, 1 == produce chunk headers
			(incompatible with HTTP/2) */
};

typedef const char *(*lws_process_html_state_cb)(void *data, int index);

struct lws_process_html_state {
	char *start; /**< pointer to start of match */
	char swallow[16]; /**< matched character buffer */
	int pos; /**< position in match */
	void *data; /**< opaque pointer */
	const char * const *vars; /**< list of variable names */
	int count_vars; /**< count of variable names */

	lws_process_html_state_cb replace;
		/**< called on match to perform substitution */
};

/*! lws_chunked_html_process() - generic chunked substitution
 * \param args: buffer to process using chunked encoding
 * \param s: current processing state
 */
LWS_VISIBLE LWS_EXTERN int
lws_chunked_html_process(struct lws_process_html_args *args,
			 struct lws_process_html_state *s);

struct lws_tokens {
	unsigned char *token; /**< pointer to start of the token */
	int len; /**< length of the token's value */
};

enum lws_token_indexes {
	WSI_TOKEN_GET_URI, /* 0 */
	WSI_TOKEN_POST_URI,
#if defined(LWS_WITH_HTTP_UNCOMMON_HEADERS) || defined(LWS_HTTP_HEADERS_ALL)
	WSI_TOKEN_OPTIONS_URI,
#endif
	WSI_TOKEN_HOST,
	WSI_TOKEN_CONNECTION,
	WSI_TOKEN_UPGRADE, /* 5 */
	WSI_TOKEN_ORIGIN,
#if defined(LWS_ROLE_WS) || defined(LWS_HTTP_HEADERS_ALL)
	WSI_TOKEN_DRAFT,
#endif
	WSI_TOKEN_CHALLENGE,
#if defined(LWS_ROLE_WS) || defined(LWS_HTTP_HEADERS_ALL)
	WSI_TOKEN_EXTENSIONS,
	WSI_TOKEN_KEY1, /* 10 */
	WSI_TOKEN_KEY2,
	WSI_TOKEN_PROTOCOL,
	WSI_TOKEN_ACCEPT,
	WSI_TOKEN_NONCE,
#endif
	WSI_TOKEN_HTTP,
#if defined(LWS_ROLE_H2) || defined(LWS_HTTP_HEADERS_ALL)
	WSI_TOKEN_HTTP2_SETTINGS, /* 16 */
#endif
	WSI_TOKEN_HTTP_ACCEPT,
#if defined(LWS_WITH_HTTP_UNCOMMON_HEADERS) || defined(LWS_HTTP_HEADERS_ALL)
	WSI_TOKEN_HTTP_AC_REQUEST_HEADERS,
#endif
	WSI_TOKEN_HTTP_IF_MODIFIED_SINCE,
	WSI_TOKEN_HTTP_IF_NONE_MATCH, /* 20 */
	WSI_TOKEN_HTTP_ACCEPT_ENCODING,
	WSI_TOKEN_HTTP_ACCEPT_LANGUAGE,
	WSI_TOKEN_HTTP_PRAGMA,
	WSI_TOKEN_HTTP_CACHE_CONTROL,
	WSI_TOKEN_HTTP_AUTHORIZATION,
	WSI_TOKEN_HTTP_COOKIE,
	WSI_TOKEN_HTTP_CONTENT_LENGTH, /* 27 */
	WSI_TOKEN_HTTP_CONTENT_TYPE,
	WSI_TOKEN_HTTP_DATE,
	WSI_TOKEN_HTTP_RANGE,
#if defined(LWS_WITH_HTTP_UNCOMMON_HEADERS) || defined(LWS_ROLE_H2) || defined(LWS_HTTP_HEADERS_ALL)
	WSI_TOKEN_HTTP_REFERER,
#endif
#if defined(LWS_ROLE_WS) || defined(LWS_HTTP_HEADERS_ALL)
	WSI_TOKEN_KEY,
	WSI_TOKEN_VERSION,
	WSI_TOKEN_SWORIGIN,
#endif
#if defined(LWS_ROLE_H2) || defined(LWS_HTTP_HEADERS_ALL)
	WSI_TOKEN_HTTP_COLON_AUTHORITY,
	WSI_TOKEN_HTTP_COLON_METHOD,
	WSI_TOKEN_HTTP_COLON_PATH,
	WSI_TOKEN_HTTP_COLON_SCHEME,
	WSI_TOKEN_HTTP_COLON_STATUS,
#endif

#if defined(LWS_WITH_HTTP_UNCOMMON_HEADERS) || defined(LWS_ROLE_H2) || defined(LWS_HTTP_HEADERS_ALL)
	WSI_TOKEN_HTTP_ACCEPT_CHARSET,
#endif
	WSI_TOKEN_HTTP_ACCEPT_RANGES,
#if defined(LWS_WITH_HTTP_UNCOMMON_HEADERS) || defined(LWS_ROLE_H2) || defined(LWS_HTTP_HEADERS_ALL)
	WSI_TOKEN_HTTP_ACCESS_CONTROL_ALLOW_ORIGIN,
#endif
	WSI_TOKEN_HTTP_AGE,
	WSI_TOKEN_HTTP_ALLOW,
	WSI_TOKEN_HTTP_CONTENT_DISPOSITION,
	WSI_TOKEN_HTTP_CONTENT_ENCODING,
	WSI_TOKEN_HTTP_CONTENT_LANGUAGE,
	WSI_TOKEN_HTTP_CONTENT_LOCATION,
	WSI_TOKEN_HTTP_CONTENT_RANGE,
	WSI_TOKEN_HTTP_ETAG,
	WSI_TOKEN_HTTP_EXPECT,
	WSI_TOKEN_HTTP_EXPIRES,
	WSI_TOKEN_HTTP_FROM,
	WSI_TOKEN_HTTP_IF_MATCH,
	WSI_TOKEN_HTTP_IF_RANGE,
	WSI_TOKEN_HTTP_IF_UNMODIFIED_SINCE,
	WSI_TOKEN_HTTP_LAST_MODIFIED,
	WSI_TOKEN_HTTP_LINK,
	WSI_TOKEN_HTTP_LOCATION,
#if defined(LWS_WITH_HTTP_UNCOMMON_HEADERS) || defined(LWS_ROLE_H2) || defined(LWS_HTTP_HEADERS_ALL)
	WSI_TOKEN_HTTP_MAX_FORWARDS,
	WSI_TOKEN_HTTP_PROXY_AUTHENTICATE,
	WSI_TOKEN_HTTP_PROXY_AUTHORIZATION,
#endif
	WSI_TOKEN_HTTP_REFRESH,
	WSI_TOKEN_HTTP_RETRY_AFTER,
	WSI_TOKEN_HTTP_SERVER,
	WSI_TOKEN_HTTP_SET_COOKIE,
#if defined(LWS_WITH_HTTP_UNCOMMON_HEADERS) || defined(LWS_ROLE_H2) || defined(LWS_HTTP_HEADERS_ALL)
	WSI_TOKEN_HTTP_STRICT_TRANSPORT_SECURITY,
#endif
	WSI_TOKEN_HTTP_TRANSFER_ENCODING,
#if defined(LWS_WITH_HTTP_UNCOMMON_HEADERS) || defined(LWS_ROLE_H2) || defined(LWS_HTTP_HEADERS_ALL)
	WSI_TOKEN_HTTP_USER_AGENT,
	WSI_TOKEN_HTTP_VARY,
	WSI_TOKEN_HTTP_VIA,
	WSI_TOKEN_HTTP_WWW_AUTHENTICATE,
#endif
#if defined(LWS_WITH_HTTP_UNCOMMON_HEADERS) || defined(LWS_HTTP_HEADERS_ALL)
	WSI_TOKEN_PATCH_URI,
	WSI_TOKEN_PUT_URI,
	WSI_TOKEN_DELETE_URI,
#endif

	WSI_TOKEN_HTTP_URI_ARGS,
#if defined(LWS_WITH_HTTP_UNCOMMON_HEADERS) || defined(LWS_HTTP_HEADERS_ALL)
	WSI_TOKEN_PROXY,
	WSI_TOKEN_HTTP_X_REAL_IP,
#endif
	WSI_TOKEN_HTTP1_0,
	WSI_TOKEN_X_FORWARDED_FOR,
	WSI_TOKEN_CONNECT,
	WSI_TOKEN_HEAD_URI,
#if defined(LWS_WITH_HTTP_UNCOMMON_HEADERS) || defined(LWS_ROLE_H2) || defined(LWS_HTTP_HEADERS_ALL)
	WSI_TOKEN_TE,
	WSI_TOKEN_REPLAY_NONCE, /* ACME */
#endif
#if defined(LWS_ROLE_H2) || defined(LWS_HTTP_HEADERS_ALL)
	WSI_TOKEN_COLON_PROTOCOL,
#endif
	WSI_TOKEN_X_AUTH_TOKEN,
	WSI_TOKEN_DSS_SIGNATURE,

	/****** add new things just above ---^ ******/

	/* use token storage to stash these internally, not for
	 * user use */

	_WSI_TOKEN_CLIENT_SENT_PROTOCOLS,
	_WSI_TOKEN_CLIENT_PEER_ADDRESS,
	_WSI_TOKEN_CLIENT_URI,
	_WSI_TOKEN_CLIENT_HOST,
	_WSI_TOKEN_CLIENT_ORIGIN,
	_WSI_TOKEN_CLIENT_METHOD,
	_WSI_TOKEN_CLIENT_IFACE,
	_WSI_TOKEN_CLIENT_ALPN,

	/* always last real token index*/
	WSI_TOKEN_COUNT,

	/* parser state additions, no storage associated */
	WSI_TOKEN_NAME_PART,
#if defined(LWS_WITH_CUSTOM_HEADERS) || defined(LWS_HTTP_HEADERS_ALL)
	WSI_TOKEN_UNKNOWN_VALUE_PART,
#endif
	WSI_TOKEN_SKIPPING,
	WSI_TOKEN_SKIPPING_SAW_CR,
	WSI_PARSING_COMPLETE,
	WSI_INIT_TOKEN_MUXURL,
};

struct lws_token_limits {
	unsigned short token_limit[WSI_TOKEN_COUNT]; /**< max chars for this token */
};

enum lws_h2_settings {
	H2SET_HEADER_TABLE_SIZE = 1,
	H2SET_ENABLE_PUSH,
	H2SET_MAX_CONCURRENT_STREAMS,
	H2SET_INITIAL_WINDOW_SIZE,
	H2SET_MAX_FRAME_SIZE,
	H2SET_MAX_HEADER_LIST_SIZE,
	H2SET_RESERVED7,
	H2SET_ENABLE_CONNECT_PROTOCOL, /* defined in mcmanus-httpbis-h2-ws-02 */

	H2SET_COUNT /* always last */
};

LWS_VISIBLE LWS_EXTERN const unsigned char *
lws_token_to_string(enum lws_token_indexes token);

LWS_VISIBLE LWS_EXTERN int LWS_WARN_UNUSED_RESULT
lws_hdr_total_length(struct lws *wsi, enum lws_token_indexes h);

LWS_VISIBLE LWS_EXTERN int LWS_WARN_UNUSED_RESULT
lws_hdr_fragment_length(struct lws *wsi, enum lws_token_indexes h,
			int frag_idx);

LWS_VISIBLE LWS_EXTERN int
lws_hdr_copy(struct lws *wsi, char *dest, int len, enum lws_token_indexes h);

LWS_VISIBLE LWS_EXTERN int
lws_hdr_copy_fragment(struct lws *wsi, char *dest, int len,
		      enum lws_token_indexes h, int frag_idx);

LWS_VISIBLE LWS_EXTERN int
lws_hdr_custom_length(struct lws *wsi, const char *name, int nlen);

LWS_VISIBLE LWS_EXTERN int
lws_hdr_custom_copy(struct lws *wsi, char *dst, int len, const char *name,
		    int nlen);

typedef void (*lws_hdr_custom_fe_cb_t)(const char *name, int nlen, void *opaque);

LWS_VISIBLE LWS_EXTERN int
lws_hdr_custom_name_foreach(struct lws *wsi, lws_hdr_custom_fe_cb_t cb, void *opaque);

LWS_VISIBLE LWS_EXTERN int
lws_get_urlarg_by_name_safe(struct lws *wsi, const char *name, char *buf, int len);

LWS_VISIBLE LWS_EXTERN const char *
lws_get_urlarg_by_name(struct lws *wsi, const char *name, char *buf, int len);

#define LWSAHH_CODE_MASK            ((1 << 16) - 1)
#define LWSAHH_FLAG_NO_SERVER_NAME  (1 << 30)

LWS_VISIBLE LWS_EXTERN int LWS_WARN_UNUSED_RESULT
lws_add_http_header_status(struct lws *wsi,
			   unsigned int code, unsigned char **p,
			   unsigned char *end);

LWS_VISIBLE LWS_EXTERN int LWS_WARN_UNUSED_RESULT
lws_add_http_header_by_name(struct lws *wsi, const unsigned char *name,
			    const unsigned char *value, int length,
			    unsigned char **p, unsigned char *end);

LWS_VISIBLE LWS_EXTERN int LWS_WARN_UNUSED_RESULT
lws_add_http_header_by_token(struct lws *wsi, enum lws_token_indexes token,
			     const unsigned char *value, int length,
			     unsigned char **p, unsigned char *end);

LWS_VISIBLE LWS_EXTERN int LWS_WARN_UNUSED_RESULT
lws_add_http_header_content_length(struct lws *wsi,
				   lws_filepos_t content_length,
				   unsigned char **p, unsigned char *end);

LWS_VISIBLE LWS_EXTERN int LWS_WARN_UNUSED_RESULT
lws_finalize_http_header(struct lws *wsi, unsigned char **p,
			 unsigned char *end);

LWS_VISIBLE LWS_EXTERN int LWS_WARN_UNUSED_RESULT
lws_finalize_write_http_header(struct lws *wsi, unsigned char *start,
			       unsigned char **p, unsigned char *end);

#define LWS_ILLEGAL_HTTP_CONTENT_LEN ((lws_filepos_t)-1ll)

LWS_VISIBLE LWS_EXTERN int LWS_WARN_UNUSED_RESULT
lws_add_http_common_headers(struct lws *wsi, unsigned int code,
			    const char *content_type, lws_filepos_t content_len,
			    unsigned char **p, unsigned char *end);

enum {
	LWSHUMETH_GET,
	LWSHUMETH_POST,
	LWSHUMETH_OPTIONS,
	LWSHUMETH_PUT,
	LWSHUMETH_PATCH,
	LWSHUMETH_DELETE,
	LWSHUMETH_CONNECT,
	LWSHUMETH_HEAD,
	LWSHUMETH_COLON_PATH,
};

LWS_VISIBLE LWS_EXTERN int LWS_WARN_UNUSED_RESULT
lws_http_get_uri_and_method(struct lws *wsi, char **puri_ptr, int *puri_len);

LWS_VISIBLE LWS_EXTERN const char *
lws_urlencode(char *escaped, const char *string, int len);

LWS_VISIBLE LWS_EXTERN int
lws_urldecode(char *string, const char *escaped, int len);

LWS_VISIBLE LWS_EXTERN int
lws_http_date_render_from_unix(char *buf, size_t len, const time_t *t);

LWS_VISIBLE LWS_EXTERN int
lws_http_date_parse_unix(const char *b, size_t len, time_t *t);

LWS_VISIBLE LWS_EXTERN int
lws_http_check_retry_after(struct lws *wsi, lws_usec_t *us_interval_in_out);

LWS_VISIBLE LWS_EXTERN int
lws_return_http_status(struct lws *wsi, unsigned int code,
		       const char *html_body);

LWS_VISIBLE LWS_EXTERN int LWS_WARN_UNUSED_RESULT
lws_http_redirect(struct lws *wsi, int code, const unsigned char *loc, int len,
		  unsigned char **p, unsigned char *end);

LWS_VISIBLE LWS_EXTERN int LWS_WARN_UNUSED_RESULT
lws_http_transaction_completed(struct lws *wsi);

LWS_VISIBLE LWS_EXTERN int
lws_http_headers_detach(struct lws *wsi);

LWS_VISIBLE LWS_EXTERN int
lws_http_mark_sse(struct lws *wsi);

LWS_VISIBLE LWS_EXTERN int
lws_h2_client_stream_long_poll_rxonly(struct lws *wsi);

LWS_VISIBLE LWS_EXTERN int
lws_http_compression_apply(struct lws *wsi, const char *name,
			   unsigned char **p, unsigned char *end, char decomp);

LWS_VISIBLE LWS_EXTERN int
lws_http_is_redirected_to_get(struct lws *wsi);

LWS_VISIBLE LWS_EXTERN int
lws_http_cookie_get(struct lws *wsi, const char *name, char *buf, size_t *max);

#define lws_http_client_http_resp_is_error(code) (!(code < 400))

#define LWS_H2_STREAM_SID -1
LWS_VISIBLE LWS_EXTERN int
lws_h2_update_peer_txcredit(struct lws *wsi, unsigned int sid, int bump);

LWS_VISIBLE LWS_EXTERN int
lws_h2_get_peer_txcredit_estimate(struct lws *wsi);

///@}
// By GST @Date