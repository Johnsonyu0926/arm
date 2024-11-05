


#define LWS_SERVER_OPTION_REQUIRE_VALID_OPENSSL_CLIENT_CERT	 ((1ll << 1) | \
								  (1ll << 12))
	/**< (VH) Don't allow the connection unless the client has a
	 * client cert that we recognize; provides
	 * LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT */
#define LWS_SERVER_OPTION_SKIP_SERVER_CANONICAL_NAME		  (1ll << 2)
	/**< (CTX) Don't try to get the server's hostname */
#define LWS_SERVER_OPTION_ALLOW_NON_SSL_ON_SSL_PORT		 ((1ll << 3) | \
								  (1ll << 12))

#define LWS_SERVER_OPTION_LIBEV					 (1ll << 4)
	/**< (CTX) Use libev event loop */
#define LWS_SERVER_OPTION_DISABLE_IPV6				 (1ll << 5)
	/**< (VH) Disable IPV6 support */
#define LWS_SERVER_OPTION_DISABLE_OS_CA_CERTS			 (1ll << 6)
	/**< (VH) Don't load OS CA certs, you will need to load your
	 * own CA cert(s) */
#define LWS_SERVER_OPTION_PEER_CERT_NOT_REQUIRED		 (1ll << 7)
	/**< (VH) Accept connections with no valid Cert (eg, selfsigned) */
#define LWS_SERVER_OPTION_VALIDATE_UTF8				 (1ll << 8)
	/**< (VH) Check UT-8 correctness */
#define LWS_SERVER_OPTION_SSL_ECDH				 ((1ll << 9) | \
								  (1ll << 12))
	/**< (VH)  initialize ECDH ciphers */
#define LWS_SERVER_OPTION_LIBUV					(1ll << 10)
	/**< (CTX)  Use libuv event loop */
#define LWS_SERVER_OPTION_REDIRECT_HTTP_TO_HTTPS		((1ll << 11) |\
								 (1ll << 12))

#define LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT			 (1ll << 12)
	/**< (CTX) Initialize the SSL library at all */
#define LWS_SERVER_OPTION_EXPLICIT_VHOSTS			 (1ll << 13)
	/**< (CTX) Only create the context when calling context
	 * create api, implies user code will create its own vhosts */
#define LWS_SERVER_OPTION_UNIX_SOCK				 (1ll << 14)
	/**< (VH) Use Unix socket */
#define LWS_SERVER_OPTION_STS					 (1ll << 15)
	/**< (VH) Send Strict Transport Security header, making
	 * clients subsequently go to https even if user asked for http */
#define LWS_SERVER_OPTION_IPV6_V6ONLY_MODIFY			 (1ll << 16)
	/**< (VH) Enable LWS_SERVER_OPTION_IPV6_V6ONLY_VALUE to take effect */
#define LWS_SERVER_OPTION_IPV6_V6ONLY_VALUE			 (1ll << 17)
	/**< (VH) if set, only ipv6 allowed on the vhost */
#define LWS_SERVER_OPTION_UV_NO_SIGSEGV_SIGFPE_SPIN		 (1ll << 18)

#define LWS_SERVER_OPTION_JUST_USE_RAW_ORIGIN			 (1ll << 19)

#define LWS_SERVER_OPTION_FALLBACK_TO_RAW /* use below name */	 (1ll << 20)
#define LWS_SERVER_OPTION_FALLBACK_TO_APPLY_LISTEN_ACCEPT_CONFIG (1ll << 20)

#define LWS_SERVER_OPTION_LIBEVENT				(1ll << 21)
	/**< (CTX) Use libevent event loop */

#define LWS_SERVER_OPTION_ONLY_RAW /* Use below name instead */	(1ll << 22)
#define LWS_SERVER_OPTION_ADOPT_APPLY_LISTEN_ACCEPT_CONFIG	(1ll << 22)

#define LWS_SERVER_OPTION_ALLOW_LISTEN_SHARE			(1ll << 23)

#define LWS_SERVER_OPTION_CREATE_VHOST_SSL_CTX			(1ll << 24)

#define LWS_SERVER_OPTION_SKIP_PROTOCOL_INIT			(1ll << 25)

#define LWS_SERVER_OPTION_IGNORE_MISSING_CERT			(1ll << 26)

#define LWS_SERVER_OPTION_VHOST_UPG_STRICT_HOST_CHECK		(1ll << 27)

#define LWS_SERVER_OPTION_HTTP_HEADERS_SECURITY_BEST_PRACTICES_ENFORCE (1ll << 28)

#define LWS_SERVER_OPTION_ALLOW_HTTP_ON_HTTPS_LISTENER		 (1ll << 29)

#define LWS_SERVER_OPTION_FAIL_UPON_UNABLE_TO_BIND		 (1ll << 30)


#define LWS_SERVER_OPTION_H2_JUST_FIX_WINDOW_UPDATE_OVERFLOW	 (1ll << 31)

#define LWS_SERVER_OPTION_VH_H2_HALF_CLOSED_LONG_POLL		 (1ll << 32)


#define LWS_SERVER_OPTION_GLIB					 (1ll << 33)
	/**< (CTX) Use glib event loop */

#define LWS_SERVER_OPTION_H2_PRIOR_KNOWLEDGE			 (1ll << 34)
	/**< (VH) Tell the vhost to treat plain text http connections as
	 * H2 with prior knowledge (no upgrade request involved)
	 */

#define LWS_SERVER_OPTION_NO_LWS_SYSTEM_STATES			 (1ll << 35)
	/**< (CTX) Disable lws_system state, eg, because we are a secure streams
	 * proxy client that is not trying to track system state by itself. */

#define LWS_SERVER_OPTION_SS_PROXY				 (1ll << 36)
	/**< (VH) We are being a SS Proxy listen socket for the vhost */

#define LWS_SERVER_OPTION_SDEVENT			 	 (1ll << 37)
	/**< (CTX) Use sd-event loop */

#define LWS_SERVER_OPTION_ULOOP					 (1ll << 38)
	/**< (CTX) Use libubox / uloop event loop */

#define LWS_SERVER_OPTION_DISABLE_TLS_SESSION_CACHE		 (1ll << 39)
	/**< (VHOST) Disallow use of client tls caching (on by default) */


	/****** add new things just above ---^ ******/


#define lws_check_opt(c, f) ((((uint64_t)c) & ((uint64_t)f)) == ((uint64_t)f))

struct lws_plat_file_ops;
struct lws_ss_policy;
struct lws_ss_plugin;
struct lws_metric_policy;

typedef int (*lws_context_ready_cb_t)(struct lws_context *context);

typedef int (*lws_peer_limits_notify_t)(struct lws_context *ctx,
					lws_sockfd_type sockfd,
					lws_sockaddr46 *sa46);


struct lws_context_creation_info {
#if defined(LWS_WITH_NETWORK)
	const char *iface;

	const struct lws_protocols *protocols;

#if defined(LWS_ROLE_WS)
	const struct lws_extension *extensions;
	/**< VHOST: NULL or array of lws_extension structs listing the
	 * extensions this context supports. */
#endif
#if defined(LWS_ROLE_H1) || defined(LWS_ROLE_H2)
	const struct lws_token_limits *token_limits;
	/**< CONTEXT: NULL or struct lws_token_limits pointer which is
	 * initialized with a token length limit for each possible WSI_TOKEN_ */
	const char *http_proxy_address;
	/**< VHOST: If non-NULL, attempts to proxy via the given address.
	 * If proxy auth is required, use format
	 * "username:password\@server:port" */
	const struct lws_protocol_vhost_options *headers;
		/**< VHOST: pointer to optional linked list of per-vhost
		 * canned headers that are added to server responses */

	const struct lws_protocol_vhost_options *reject_service_keywords;
	/**< CONTEXT: Optional list of keywords and rejection codes + text.
	 *
	 * The keywords are checked for existing in the user agent string.
	 *
	 * Eg, "badrobot" "404 Not Found"
	 */
	const struct lws_protocol_vhost_options *pvo;
	/**< VHOST: pointer to optional linked list of per-vhost
	 * options made accessible to protocols */
	const char *log_filepath;
	/**< VHOST: filepath to append logs to... this is opened before
	 *		any dropping of initial privileges */
	const struct lws_http_mount *mounts;
	/**< VHOST: optional linked list of mounts for this vhost */
	const char *server_string;
	/**< CONTEXT: string used in HTTP headers to identify server
	 * software, if NULL, "libwebsockets". */

	const char *error_document_404;
	/**< VHOST: If non-NULL, when asked to serve a non-existent file,
	 *          lws attempts to server this url path instead.  Eg,
	 *          "/404.html" */
	int port;
	/**< VHOST: Port to listen on. Use CONTEXT_PORT_NO_LISTEN to suppress
	 * listening for a client. Use CONTEXT_PORT_NO_LISTEN_SERVER if you are
	 * writing a server but you are using \ref sock-adopt instead of the
	 * built-in listener.
	 *
	 * You can also set port to 0, in which case the kernel will pick
	 * a random port that is not already in use.  You can find out what
	 * port the vhost is listening on using lws_get_vhost_listen_port() */

	unsigned int http_proxy_port;
	/**< VHOST: If http_proxy_address was non-NULL, uses this port */
	unsigned int max_http_header_data2;
	/**< CONTEXT: if max_http_header_data is 0 and this
	 * is nonzero, this will be used in place of the default.  It's
	 * like this for compatibility with the original short version,
	 * this is unsigned int length. */
	unsigned int max_http_header_pool2;
	/**< CONTEXT: if max_http_header_pool is 0 and this
	 * is nonzero, this will be used in place of the default.  It's
	 * like this for compatibility with the original short version:
	 * this is unsigned int length. */

	int keepalive_timeout;

	uint32_t	http2_settings[7];


	unsigned short max_http_header_data;
	/**< CONTEXT: The max amount of header payload that can be handled
	 * in an http request (unrecognized header payload is dropped) */
	unsigned short max_http_header_pool;


#endif

#if defined(LWS_WITH_TLS)
	const char *ssl_private_key_password;

	const char *ssl_cert_filepath;

	const char *ssl_private_key_filepath;

	const char *ssl_ca_filepath;

	const char *ssl_cipher_list;

	const char *ecdh_curve;
	/**< VHOST: if NULL, defaults to initializing server with
	 *   "prime256v1" */
	const char *tls1_3_plus_cipher_list;


	const void *server_ssl_cert_mem;

	const void *server_ssl_private_key_mem;

	const void *server_ssl_ca_mem;

	long ssl_options_set;
	/**< VHOST: Any bits set here will be set as server SSL options */
	long ssl_options_clear;
	/**< VHOST: Any bits set here will be cleared as server SSL options */
	int simultaneous_ssl_restriction;
	/**< CONTEXT: 0 (no limit) or limit of simultaneous SSL sessions
	 * possible.*/
	int simultaneous_ssl_handshake_restriction;
	/**< CONTEXT: 0 (no limit) or limit of simultaneous SSL handshakes ongoing */
	int ssl_info_event_mask;
	/**< VHOST: mask of ssl events to be reported on LWS_CALLBACK_SSL_INFO
	 * callback for connections on this vhost.  The mask values are of
	 * the form SSL_CB_ALERT, defined in openssl/ssl.h.  The default of
	 * 0 means no info events will be reported.
	 */
	unsigned int server_ssl_cert_mem_len;
	/**< VHOST: Server SSL context init: length of server_ssl_cert_mem in
	 * bytes */
	unsigned int server_ssl_private_key_mem_len;
	/**< VHOST: length of \p server_ssl_private_key_mem in memory */
	unsigned int server_ssl_ca_mem_len;
	/**< VHOST: length of \p server_ssl_ca_mem in memory */

	const char *alpn;


#if defined(LWS_WITH_CLIENT)
	const char *client_ssl_private_key_password;
	/**< VHOST: Client SSL context init: NULL or the passphrase needed
	 * for the private key */
	const char *client_ssl_cert_filepath;
	/**< VHOST: Client SSL context init: The certificate the client
	 * should present to the peer on connection */
	const void *client_ssl_cert_mem;
	/**< VHOST: Client SSL context init: client certificate memory buffer or
	 * NULL... use this to load client cert from memory instead of file */
	unsigned int client_ssl_cert_mem_len;
	/**< VHOST: Client SSL context init: length of client_ssl_cert_mem in
	 * bytes */
	const char *client_ssl_private_key_filepath;

	const void *client_ssl_key_mem;
	/**< VHOST: Client SSL context init: client key memory buffer or
	 * NULL... use this to load client key from memory instead of file */
	const char *client_ssl_ca_filepath;
	/**< VHOST: Client SSL context init: CA certificate filepath or NULL */
	const void *client_ssl_ca_mem;
	/**< VHOST: Client SSL context init: CA certificate memory buffer or
	 * NULL... use this to load CA cert from memory instead of file */

	const char *client_ssl_cipher_list;
	/**< VHOST: Client SSL context init: List of valid ciphers to use (eg,
	* "RC4-MD5:RC4-SHA:AES128-SHA:AES256-SHA:HIGH:!DSS:!aNULL"
	* or you can leave it as NULL to get "DEFAULT" */
	const char *client_tls_1_3_plus_cipher_list;


	long ssl_client_options_set;
	/**< VHOST: Any bits set here will be set as CLIENT SSL options */
	long ssl_client_options_clear;
	/**< VHOST: Any bits set here will be cleared as CLIENT SSL options */


	unsigned int client_ssl_ca_mem_len;
	/**< VHOST: Client SSL context init: length of client_ssl_ca_mem in
	 * bytes */
	unsigned int client_ssl_key_mem_len;
	/**< VHOST: Client SSL context init: length of client_ssl_key_mem in
	 * bytes */

#endif

#if !defined(LWS_WITH_MBEDTLS)
	SSL_CTX *provided_client_ssl_ctx;

#else /* WITH_MBEDTLS */
	const char *mbedtls_client_preload_filepath;

#endif
#endif

	int ka_time;
	/**< CONTEXT: 0 for no TCP keepalive, otherwise apply this keepalive
	 * timeout to all libwebsocket sockets, client or server */
	int ka_probes;
	/**< CONTEXT: if ka_time was nonzero, after the timeout expires how many
	 * times to try to get a response from the peer before giving up
	 * and killing the connection */
	int ka_interval;
	/**< CONTEXT: if ka_time was nonzero, how long to wait before each ka_probes
	 * attempt */
	unsigned int timeout_secs;

	unsigned int connect_timeout_secs;
	/**< VHOST: client connections have this long to find a working server
	 * from the DNS results, or the whole connection times out.  If zero,
	 * a default timeout is used */
	int bind_iface;

	unsigned int timeout_secs_ah_idle;
	/**< VHOST: seconds to allow a client to hold an ah without using it.
	 * 0 defaults to 10s. */
#endif /* WITH_NETWORK */

#if defined(LWS_WITH_TLS_SESSIONS)
	uint32_t			tls_session_timeout;
	/**< VHOST: seconds until timeout/ttl for newly created sessions.
	 * 0 means default timeout (defined per protocol, usually 300s). */
	uint32_t			tls_session_cache_max;
	/**< VHOST: 0 for default limit of 10, or the maximum number of
	 * client tls sessions we are willing to cache */
#endif

	gid_t gid;
	/**< CONTEXT: group id to change to after setting listen socket,
	 *   or -1. See also .username below. */
	uid_t uid;
	/**< CONTEXT: user id to change to after setting listen socket,
	 *   or -1.  See also .groupname below. */
	uint64_t options;
	/**< VHOST + CONTEXT: 0, or LWS_SERVER_OPTION_... bitfields */
	void *user;

	unsigned int count_threads;
	/**< CONTEXT: how many contexts to create in an array, 0 = 1 */
	unsigned int fd_limit_per_thread;

	const char *vhost_name;

#if defined(LWS_WITH_PLUGINS)
	const char * const *plugin_dirs;
	/**< CONTEXT: NULL, or NULL-terminated array of directories to
	 * scan for lws protocol plugins at context creation time */
#endif
	void *external_baggage_free_on_destroy;

	unsigned int pt_serv_buf_size;

#if defined(LWS_WITH_FILE_OPS)
	const struct lws_plat_file_ops *fops;

#endif

#if defined(LWS_WITH_SOCKS5)
	const char *socks_proxy_address;
	
	unsigned int socks_proxy_port;

#endif

#if defined(LWS_HAVE_SYS_CAPABILITY_H) && defined(LWS_HAVE_LIBCAP)
	cap_value_t caps[4];

	char count_caps;
	/**< CONTEXT: count of Linux capabilities in .caps[].  0 means
	 * no capabilities will be inherited from root (the default) */
#endif
	void **foreign_loops;

	void (*signal_cb)(void *event_lib_handle, int signum);

	struct lws_context **pcontext;

	void (*finalize)(struct lws_vhost *vh, void *arg);

	void *finalize_arg;
	/**< VHOST: opaque pointer lws ignores but passes to the finalize
	 *	    callback.  If you don't care, leave it NULL.
	 */
	const char *listen_accept_role;
	/**< VHOST: NULL for default, or force accepted incoming connections to
	 * bind to this role.  Uses the role names from their ops struct, eg,
	 * "raw-skt".
	 */
	const char *listen_accept_protocol;
	/**< VHOST: NULL for default, or force accepted incoming connections to
	 * bind to this vhost protocol name.
	 */
	const struct lws_protocols **pprotocols;

	const char *username; /**< CONTEXT: string username for post-init
	 * permissions.  Like .uid but takes a string username. */
	const char *groupname; /**< CONTEXT: string groupname for post-init
	 * permissions.  Like .gid but takes a string groupname. */
	const char *unix_socket_perms; /**< VHOST: if your vhost is listening
	 * on a unix socket, you can give a "username:groupname" string here
	 * to control the owner:group it's created with.  It's always created
	 * with 0660 mode. */
	const lws_system_ops_t *system_ops;
	/**< CONTEXT: hook up lws_system_ apis to system-specific
	 * implementations */
	const lws_retry_bo_t *retry_and_idle_policy;

#if defined(LWS_WITH_SYS_STATE)
	lws_state_notify_link_t * const *register_notifier_list;

#endif
#if defined(LWS_WITH_SECURE_STREAMS)
#if defined(LWS_WITH_SECURE_STREAMS_STATIC_POLICY_ONLY)
	const struct lws_ss_policy *pss_policies; /**< CONTEXT: point to first
	 * in a linked-list of streamtype policies prepared by user code */
#else
	const char *pss_policies_json; /**< CONTEXT: point to a string

#endif
	const struct lws_ss_plugin **pss_plugins; /**< CONTEXT: point to an array
	 * of pointers to plugin structs here, terminated with a NULL ptr.
	 * Set to NULL if not using Secure Streams. */
	const char *ss_proxy_bind; /**< CONTEXT: NULL, or: ss_proxy_port == 0:
	 * point to a string giving the Unix Domain Socket address to use (start

	const char *ss_proxy_address; /**< CONTEXT: NULL, or if ss_proxy_port
	 * nonzero: the tcp address of the ss proxy to connect to */
	uint16_t ss_proxy_port; /* 0 = if connecting to ss proxy, do it via a

#endif

	int rlimit_nofile;

#if defined(LWS_WITH_PEER_LIMITS)
	lws_peer_limits_notify_t pl_notify_cb;

	unsigned short ip_limit_ah;

	unsigned short ip_limit_wsi;


#endif /* PEER_LIMITS */

#if defined(LWS_WITH_SYS_FAULT_INJECTION)
	lws_fi_ctx_t				fic;

#endif

#if defined(LWS_WITH_SYS_SMD)
	lws_smd_notification_cb_t		early_smd_cb;

	void					*early_smd_opaque;
	lws_smd_class_t				early_smd_class_filter;
	lws_usec_t				smd_ttl_us;

	uint16_t				smd_queue_depth;

#endif

#if defined(LWS_WITH_SYS_METRICS)
	const struct lws_metric_policy		*metrics_policies;
	/**< CONTEXT: non-SS policy metrics policies */
	const char				*metrics_prefix;

#endif

	int					fo_listen_queue;

	const struct lws_plugin_evlib		*event_lib_custom;


#if defined(LWS_WITH_TLS_JIT_TRUST)
	size_t					jitt_cache_max_footprint;

	int					vh_idle_grace_ms;

#endif

	lws_log_cx_t				*log_cx;

#if defined(LWS_WITH_CACHE_NSCOOKIEJAR) && defined(LWS_WITH_CLIENT)
	const char				*http_nsc_filepath;

	size_t					http_nsc_heap_max_footprint;

	size_t					http_nsc_heap_max_items;

	size_t					http_nsc_heap_max_payload;

#endif

	void *_unused[2]; /**< dummy */
};


LWS_VISIBLE LWS_EXTERN struct lws_context *
lws_create_context(const struct lws_context_creation_info *info);

LWS_VISIBLE LWS_EXTERN void
lws_context_destroy(struct lws_context *context);

typedef int (*lws_reload_func)(void);

LWS_VISIBLE LWS_EXTERN void
lws_context_deprecate(struct lws_context *context, lws_reload_func cb);

LWS_VISIBLE LWS_EXTERN int
lws_context_is_deprecated(struct lws_context *context);


LWS_VISIBLE LWS_EXTERN int
lws_set_proxy(struct lws_vhost *vhost, const char *proxy);


LWS_VISIBLE LWS_EXTERN int
lws_set_socks(struct lws_vhost *vhost, const char *socks);

struct lws_vhost;

LWS_VISIBLE LWS_EXTERN struct lws_vhost *
lws_create_vhost(struct lws_context *context,
		 const struct lws_context_creation_info *info);

LWS_VISIBLE LWS_EXTERN void
lws_vhost_destroy(struct lws_vhost *vh);


LWS_VISIBLE LWS_EXTERN int
lwsws_get_config_globals(struct lws_context_creation_info *info, const char *d,
			 char **config_strings, int *len);


LWS_VISIBLE LWS_EXTERN int
lwsws_get_config_vhosts(struct lws_context *context,
			struct lws_context_creation_info *info, const char *d,
			char **config_strings, int *len);
SIBLE LWS_EXTERN struct lws_vhost *
lws_get_vhost(struct lws *wsi);

LWS_VISIBLE LWS_EXTERN const char *
lws_get_vhost_name(struct lws_vhost *vhost);


LWS_VISIBLE LWS_EXTERN struct lws_vhost *
lws_get_vhost_by_name(struct lws_context *context, const char *name);


LWS_VISIBLE LWS_EXTERN int
lws_get_vhost_port(struct lws_vhost *vhost);


LWS_VISIBLE LWS_EXTERN void *
lws_get_vhost_user(struct lws_vhost *vhost);

LWS_VISIBLE LWS_EXTERN const char *
lws_get_vhost_iface(struct lws_vhost *vhost);


LWS_VISIBLE LWS_EXTERN int
lws_json_dump_vhost(const struct lws_vhost *vh, char *buf, int len);


LWS_VISIBLE LWS_EXTERN int
lws_json_dump_context(const struct lws_context *context, char *buf, int len,
		      int hide_vhosts);


LWS_VISIBLE LWS_EXTERN void *
lws_vhost_user(struct lws_vhost *vhost);


LWS_VISIBLE LWS_EXTERN void *
lws_context_user(struct lws_context *context);

LWS_VISIBLE LWS_EXTERN const char *
lws_vh_tag(struct lws_vhost *vh);

LWS_VISIBLE LWS_EXTERN int
lws_context_is_being_destroyed(struct lws_context *context);


struct lws_protocol_vhost_options {
	const struct lws_protocol_vhost_options *next; /**< linked list */
	const struct lws_protocol_vhost_options *options; /**< child linked-list of more options for this node */
	const char *name; /**< name of name=value pair */
	const char *value; /**< value of name=value pair */
};

#define LWS_SERVER_OPTION_REQUIRE_VALID_OPENSSL_CLIENT_CERT	 ((1ll << 1) | (1ll << 12))
#define LWS_SERVER_OPTION_SKIP_SERVER_CANONICAL_NAME		  (1ll << 2)
#define LWS_SERVER_OPTION_ALLOW_NON_SSL_ON_SSL_PORT		 ((1ll << 3) | (1ll << 12))
#define LWS_SERVER_OPTION_LIBEV					 (1ll << 4)
#define LWS_SERVER_OPTION_DISABLE_IPV6				 (1ll << 5)
#define LWS_SERVER_OPTION_DISABLE_OS_CA_CERTS			 (1ll << 6)
#define LWS_SERVER_OPTION_PEER_CERT_NOT_REQUIRED		 (1ll << 7)
#define LWS_SERVER_OPTION_VALIDATE_UTF8				 (1ll << 8)
#define LWS_SERVER_OPTION_SSL_ECDH				 ((1ll << 9) | (1ll << 12))
#define LWS_SERVER_OPTION_LIBUV					(1ll << 10)
#define LWS_SERVER_OPTION_REDIRECT_HTTP_TO_HTTPS		((1ll << 11) | (1ll << 12))
#define LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT			 (1ll << 12)
#define LWS_SERVER_OPTION_EXPLICIT_VHOSTS			 (1ll << 13)
#define LWS_SERVER_OPTION_UNIX_SOCK				 (1ll << 14)
#define LWS_SERVER_OPTION_STS					 (1ll << 15)
#define LWS_SERVER_OPTION_IPV6_V6ONLY_MODIFY			 (1ll << 16)
#define LWS_SERVER_OPTION_IPV6_V6ONLY_VALUE			 (1ll << 17)
#define LWS_SERVER_OPTION_UV_NO_SIGSEGV_SIGFPE_SPIN		 (1ll << 18)
#define LWS_SERVER_OPTION_JUST_USE_RAW_ORIGIN			 (1ll << 19)
#define LWS_SERVER_OPTION_FALLBACK_TO_RAW			 (1ll << 20)
#define LWS_SERVER_OPTION_LIBEVENT				(1ll << 21)
#define LWS_SERVER_OPTION_ONLY_RAW					(1ll << 22)
#define LWS_SERVER_OPTION_ALLOW_LISTEN_SHARE			(1ll << 23)
#define LWS_SERVER_OPTION_CREATE_VHOST_SSL_CTX			(1ll << 24)
#define LWS_SERVER_OPTION_SKIP_PROTOCOL_INIT			(1ll << 25)
#define LWS_SERVER_OPTION_IGNORE_MISSING_CERT			(1ll << 26)
#define LWS_SERVER_OPTION_VHOST_UPG_STRICT_HOST_CHECK		(1ll << 27)
#define LWS_SERVER_OPTION_HTTP_HEADERS_SECURITY_BEST_PRACTICES_ENFORCE (1ll << 28)
#define LWS_SERVER_OPTION_ALLOW_HTTP_ON_HTTPS_LISTENER		 (1ll << 29)
#define LWS_SERVER_OPTION_FAIL_UPON_UNABLE_TO_BIND		 (1ll << 30)
#define LWS_SERVER_OPTION_H2_JUST_FIX_WINDOW_UPDATE_OVERFLOW	 (1ll << 31)
#define LWS_SERVER_OPTION_VH_H2_HALF_CLOSED_LONG_POLL		 (1ll << 32)
#define LWS_SERVER_OPTION_GLIB					 (1ll << 33)
#define LWS_SERVER_OPTION_H2_PRIOR_KNOWLEDGE			 (1ll << 34)
#define LWS_SERVER_OPTION_NO_LWS_SYSTEM_STATES			 (1ll << 35)
#define LWS_SERVER_OPTION_SS_PROXY				 (1ll << 36)
#define LWS_SERVER_OPTION_SDEVENT			 	 (1ll << 37)
#define LWS_SERVER_OPTION_ULOOP					 (1ll << 38)
#define LWS_SERVER_OPTION_DISABLE_TLS_SESSION_CACHE		 (1ll << 39)

#define lws_check_opt(c, f) ((((uint64_t)c) & ((uint64_t)f)) == ((uint64_t)f))

struct lws_plat_file_ops;
struct lws_ss_policy;
struct lws_ss_plugin;
struct lws_metric_policy;

typedef int (*lws_context_ready_cb_t)(struct lws_context *context);
typedef int (*lws_peer_limits_notify_t)(struct lws_context *ctx, lws_sockfd_type sockfd, lws_sockaddr46 *sa46);

struct lws_context_creation_info {
#if defined(LWS_WITH_NETWORK)
	const char *iface;
	const struct lws_protocols *protocols;
#if defined(LWS_ROLE_WS)
	const struct lws_extension *extensions;
#endif
#if defined(LWS_ROLE_H1) || defined(LWS_ROLE_H2)
	const struct lws_token_limits *token_limits;
	const char *http_proxy_address;
	const struct lws_protocol_vhost_options *headers;
	const struct lws_protocol_vhost_options *reject_service_keywords;
	const struct lws_protocol_vhost_options *pvo;
	const char *log_filepath;
	const struct lws_http_mount *mounts;
	const char *server_string;
	const char *error_document_404;
	int port;
	unsigned int http_proxy_port;
	unsigned int max_http_header_data2;
	unsigned int max_http_header_pool2;
	int keepalive_timeout;
	uint32_t http2_settings[7];
	unsigned short max_http_header_data;
	unsigned short max_http_header_pool;
#endif
#if defined(LWS_WITH_TLS)
	const char *ssl_private_key_password;
	const char *ssl_cert_filepath;
	const char *ssl_private_key_filepath;
	const char *ssl_ca_filepath;
	const char *ssl_cipher_list;
	const char *ecdh_curve;
	const char *tls1_3_plus_cipher_list;
	const void *server_ssl_cert_mem;
	const void *server_ssl_private_key_mem;
	const void *server_ssl_ca_mem;
	long ssl_options_set;
	long ssl_options_clear;
	int simultaneous_ssl_restriction;
	int simultaneous_ssl_handshake_restriction;
	int ssl_info_event_mask;
	unsigned int server_ssl_cert_mem_len;
	unsigned int server_ssl_private_key_mem_len;
	unsigned int server_ssl_ca_mem_len;
	const char *alpn;
#if defined(LWS_WITH_CLIENT)
	const char *client_ssl_private_key_password;
	const char *client_ssl_cert_filepath;
	const void *client_ssl_cert_mem;
	unsigned int client_ssl_cert_mem_len;
	const char *client_ssl_private_key_filepath;
	const void *client_ssl_key_mem;
	const char *client_ssl_ca_filepath;
	const void *client_ssl_ca_mem;
	const char *client_ssl_cipher_list;
	const char *client_tls_1_3_plus_cipher_list;
	long ssl_client_options_set;
	long ssl_client_options_clear;
	unsigned int client_ssl_ca_mem_len;
	unsigned int client_ssl_key_mem_len;
#endif
#if !defined(LWS_WITH_MBEDTLS)
	SSL_CTX *provided_client_ssl_ctx;
#else
	const char *mbedtls_client_preload_filepath;
#endif
#endif
	int ka_time;
	int ka_probes;
	int ka_interval;
	unsigned int timeout_secs;
	unsigned int connect_timeout_secs;
	int bind_iface;
	unsigned int timeout_secs_ah_idle;
#endif
#if defined(LWS_WITH_TLS_SESSIONS)
	uint32_t tls_session_timeout;
	uint32_t tls_session_cache_max;
#endif
	gid_t gid;
	uid_t uid;
	uint64_t options;
	void *user;
	unsigned int count_threads;
	unsigned int fd_limit_per_thread;
	const char *vhost_name;
#if defined(LWS_WITH_PLUGINS)
	const char * const *plugin_dirs;
#endif
	void *external_baggage_free_on_destroy;
	unsigned int pt_serv_buf_size;
#if defined(LWS_WITH_FILE_OPS)
	const struct lws_plat_file_ops *fops;
#endif
#if defined(LWS_WITH_SOCKS5)
	const char *socks_proxy_address;
	unsigned int socks_proxy_port;
#endif
#if defined(LWS_HAVE_SYS_CAPABILITY_H) && defined(LWS_HAVE_LIBCAP)
	cap_value_t caps[4];
	char count_caps;
#endif
	void **foreign_loops;
	void (*signal_cb)(void *event_lib_handle, int signum);
	struct lws_context **pcontext;
	void (*finalize)(struct lws_vhost *vh, void *arg);
	void *finalize_arg;
	const char *listen_accept_role;
	const char *listen_accept_protocol;
	const struct lws_protocols **pprotocols;
	const char *username;
	const char *groupname;
	const char *unix_socket_perms;
	const lws_system_ops_t *system_ops;
	const lws_retry_bo_t *retry_and_idle_policy;
#if defined(LWS_WITH_SYS_STATE)
	lws_state_notify_link_t * const *register_notifier_list;
#endif
#if defined(LWS_WITH_SECURE_STREAMS)
#if defined(LWS_WITH_SECURE_STREAMS_STATIC_POLICY_ONLY)
	const struct lws_ss_policy *pss_policies;
#else
	const char *pss_policies_json;
#endif
	const struct lws_ss_plugin **pss_plugins;
	const char *ss_proxy_bind;
	const char *ss_proxy_address;
	uint16_t ss_proxy_port;
#endif
	int rlimit_nofile;
#if defined(LWS_WITH_PEER_LIMITS)
	lws_peer_limits_notify_t pl_notify_cb;
	unsigned short ip_limit_ah;
	unsigned short ip_limit_wsi;
#endif
#if defined(LWS_WITH_SYS_FAULT_INJECTION)
	lws_fi_ctx_t fic;
#endif
#if defined(LWS_WITH_SYS_SMD)
	lws_smd_notification_cb_t early_smd_cb;
	void *early_smd_opaque;
	lws_smd_class_t early_smd_class_filter;
	lws_usec_t smd_ttl_us;
	uint16_t smd_queue_depth;
#endif
#if defined(LWS_WITH_SYS_METRICS)
	const struct lws_metric_policy *metrics_policies;
	const char *metrics_prefix;
#endif
	int fo_listen_queue;
	const struct lws_plugin_evlib *event_lib_custom;
#if defined(LWS_WITH_TLS_JIT_TRUST)
	size_t jitt_cache_max_footprint;
	int vh_idle_grace_ms;
#endif
	lws_log_cx_t *log_cx;
#if defined(LWS_WITH_CACHE_NSCOOKIEJAR) && defined(LWS_WITH_CLIENT)
	const char *http_nsc_filepath;
	size_t http_nsc_heap_max_footprint;
	size_t http_nsc_heap_max_items;
	size_t http_nsc_heap_max_payload;
#endif
	void *_unused[2];
};

LWS_VISIBLE LWS_EXTERN struct lws_context *
lws_create_context(const struct lws_context_creation_info *info);

LWS_VISIBLE LWS_EXTERN void
lws_context_destroy(struct lws_context *context);

typedef int (*lws_reload_func)(void);

LWS_VISIBLE LWS_EXTERN void
lws_context_deprecate(struct lws_context *context, lws_reload_func cb);

LWS_VISIBLE LWS_EXTERN int
lws_context_is_deprecated(struct lws_context *context);

LWS_VISIBLE LWS_EXTERN int
lws_set_proxy(struct lws_vhost *vhost, const char *proxy);

LWS_VISIBLE LWS_EXTERN int
lws_set_socks(struct lws_vhost *vhost, const char *socks);

LWS_VISIBLE LWS_EXTERN struct lws_vhost *
lws_create_vhost(struct lws_context *context, const struct lws_context_creation_info *info);

LWS_VISIBLE LWS_EXTERN void
lws_vhost_destroy(struct lws_vhost *vh);

LWS_VISIBLE LWS_EXTERN int
lwsws_get_config_globals(struct lws_context_creation_info *info, const char *d, char **config_strings, int *len);

LWS_VISIBLE LWS_EXTERN int
lwsws_get_config_vhosts(struct lws_context *context, struct lws_context_creation_info *info, const char *d, char **config_strings, int *len);

LWS_VISIBLE LWS_EXTERN struct lws_vhost *
lws_get_vhost(struct lws *wsi);

LWS_VISIBLE LWS_EXTERN const char *
lws_get_vhost_name(struct lws_vhost *vhost);

LWS_VISIBLE LWS_EXTERN struct lws_vhost *
lws_get_vhost_by_name(struct lws_context *context, const char *name);

LWS_VISIBLE LWS_EXTERN int
lws_get_vhost_port(struct lws_vhost *vhost);

LWS_VISIBLE LWS_EXTERN void *
lws_get_vhost_user(struct lws_vhost *vhost);

LWS_VISIBLE LWS_EXTERN const char *
lws_get_vhost_iface(struct lws_vhost *vhost);

LWS_VISIBLE LWS_EXTERN int
lws_json_dump_vhost(const struct lws_vhost *vh, char *buf, int len);

LWS_VISIBLE LWS_EXTERN int
lws_json_dump_context(const struct lws_context *context, char *buf, int len, int hide_vhosts);

LWS_VISIBLE LWS_EXTERN void *
lws_vhost_user(struct lws_vhost *vhost);

LWS_VISIBLE LWS_EXTERN void *
lws_context_user(struct lws_context *context);

LWS_VISIBLE LWS_EXTERN const char *
lws_vh_tag(struct lws_vhost *vh);

LWS_VISIBLE LWS_EXTERN int
lws_context_is_being_destroyed(struct lws_context *context);

struct lws_protocol_vhost_options {
	const struct lws_protocol_vhost_options *next;
	const struct lws_protocol_vhost_options *options;
	const char *name;
	const char *value;
};

enum lws_mount_protocols {
	LWSMPRO_HTTP = 0,
	LWSMPRO_HTTPS = 1,
	LWSMPRO_FILE = 2,
	LWSMPRO_CGI = 3,
	LWSMPRO_REDIR_HTTP = 4,
	LWSMPRO_REDIR_HTTPS = 5,
	LWSMPRO_CALLBACK = 6,
};

enum lws_authentication_mode {
	LWSAUTHM_DEFAULT = 0,
	LWSAUTHM_BASIC_AUTH_CALLBACK = 1 << 28
};

#define AUTH_MODE_MASK 0xF0000000

struct lws_http_mount {
	const struct lws_http_mount *mount_next;
	const char *mountpoint;
	const char *origin;
	const char *def;
	const char *protocol;
	const struct lws_protocol_vhost_options *cgienv;
	const struct lws_protocol_vhost_options *extra_mimetypes;
	const struct lws_protocol_vhost_options *interpret;
	int cgi_timeout;
	int cache_max_age;
	unsigned int auth_mask;
	unsigned int cache_reusable:1;
	unsigned int cache_revalidate:1;
	unsigned int cache_intermediaries:1;
	unsigned char origin_protocol;
	unsigned char mountpoint_len;
	const char *basic_auth_login_file;
};

///@}
// By GST @Date