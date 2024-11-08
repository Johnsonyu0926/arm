struct lws_ssl_info {
	int where;
	int ret;
};

enum lws_cert_update_state {
	LWS_CUS_IDLE,
	LWS_CUS_STARTING,
	LWS_CUS_SUCCESS,
	LWS_CUS_FAILED,
	LWS_CUS_CREATE_KEYS,
	LWS_CUS_REG,
	LWS_CUS_AUTH,
	LWS_CUS_CHALLENGE,
	LWS_CUS_CREATE_REQ,
	LWS_CUS_REQ,
	LWS_CUS_CONFIRM,
	LWS_CUS_ISSUE,
};

enum {
	LWS_TLS_REQ_ELEMENT_COUNTRY,
	LWS_TLS_REQ_ELEMENT_STATE,
	LWS_TLS_REQ_ELEMENT_LOCALITY,
	LWS_TLS_REQ_ELEMENT_ORGANIZATION,
	LWS_TLS_REQ_ELEMENT_COMMON_NAME,
	LWS_TLS_REQ_ELEMENT_SUBJECT_ALT_NAME,
	LWS_TLS_REQ_ELEMENT_EMAIL,
	LWS_TLS_REQ_ELEMENT_COUNT,
	LWS_TLS_SET_DIR_URL = LWS_TLS_REQ_ELEMENT_COUNT,
	LWS_TLS_SET_AUTH_PATH,
	LWS_TLS_SET_CERT_PATH,
	LWS_TLS_SET_KEY_PATH,
	LWS_TLS_TOTAL_COUNT
};

struct lws_acme_cert_aging_args {
	struct lws_vhost *vh;
	const char *element_overrides[LWS_TLS_TOTAL_COUNT];
};

struct lws_filter_network_conn_args {
	struct sockaddr_storage		cli_addr;
	socklen_t			clilen;
	lws_sockfd_type			accept_fd;
};

enum lws_callback_reasons {
	LWS_CALLBACK_PROTOCOL_INIT				= 27,
	LWS_CALLBACK_PROTOCOL_DESTROY				= 28,
	LWS_CALLBACK_WSI_CREATE					= 29,
	LWS_CALLBACK_WSI_DESTROY				= 30,
	LWS_CALLBACK_WSI_TX_CREDIT_GET				= 103,
	LWS_CALLBACK_OPENSSL_LOAD_EXTRA_CLIENT_VERIFY_CERTS	= 21,
	LWS_CALLBACK_OPENSSL_LOAD_EXTRA_SERVER_VERIFY_CERTS	= 22,
	LWS_CALLBACK_OPENSSL_PERFORM_CLIENT_CERT_VERIFICATION	= 23,
	LWS_CALLBACK_OPENSSL_CONTEXT_REQUIRES_PRIVATE_KEY	= 37,
	LWS_CALLBACK_SSL_INFO					= 67,
	LWS_CALLBACK_OPENSSL_PERFORM_SERVER_CERT_VERIFICATION = 58,
	LWS_CALLBACK_SERVER_NEW_CLIENT_INSTANTIATED		= 19,
	LWS_CALLBACK_HTTP					= 12,
	LWS_CALLBACK_HTTP_BODY					= 13,
	LWS_CALLBACK_HTTP_BODY_COMPLETION			= 14,
	LWS_CALLBACK_HTTP_FILE_COMPLETION			= 15,
	LWS_CALLBACK_HTTP_WRITEABLE				= 16,
	LWS_CALLBACK_CLOSED_HTTP				=  5,
	LWS_CALLBACK_FILTER_HTTP_CONNECTION			= 18,
	LWS_CALLBACK_ADD_HEADERS				= 53,
	LWS_CALLBACK_VERIFY_BASIC_AUTHORIZATION = 102,
	LWS_CALLBACK_CHECK_ACCESS_RIGHTS			= 51,
	LWS_CALLBACK_PROCESS_HTML				= 52,
	LWS_CALLBACK_HTTP_BIND_PROTOCOL				= 49,
	LWS_CALLBACK_HTTP_DROP_PROTOCOL				= 50,
	LWS_CALLBACK_HTTP_CONFIRM_UPGRADE			= 86,
	LWS_CALLBACK_ESTABLISHED_CLIENT_HTTP			= 44,
	LWS_CALLBACK_CLOSED_CLIENT_HTTP				= 45,
	LWS_CALLBACK_RECEIVE_CLIENT_HTTP_READ			= 48,
	LWS_CALLBACK_RECEIVE_CLIENT_HTTP			= 46,
	LWS_CALLBACK_COMPLETED_CLIENT_HTTP			= 47,
	LWS_CALLBACK_CLIENT_HTTP_WRITEABLE			= 57,
	LWS_CALLBACK_CLIENT_HTTP_REDIRECT			= 104,
	LWS_CALLBACK_CLIENT_HTTP_BIND_PROTOCOL			= 85,
	LWS_CALLBACK_CLIENT_HTTP_DROP_PROTOCOL			= 76,
	LWS_CALLBACK_ESTABLISHED				=  0,
	LWS_CALLBACK_CLOSED					=  4,
	LWS_CALLBACK_SERVER_WRITEABLE				= 11,
	LWS_CALLBACK_RECEIVE					=  6,
	LWS_CALLBACK_RECEIVE_PONG				=  7,
	LWS_CALLBACK_WS_PEER_INITIATED_CLOSE			= 38,
	LWS_CALLBACK_FILTER_PROTOCOL_CONNECTION			= 20,
	LWS_CALLBACK_CONFIRM_EXTENSION_OKAY			= 25,
	LWS_CALLBACK_WS_SERVER_BIND_PROTOCOL			= 77,
	LWS_CALLBACK_WS_SERVER_DROP_PROTOCOL			= 78,
	LWS_CALLBACK_CLIENT_CONNECTION_ERROR			=  1,
	LWS_CALLBACK_CLIENT_FILTER_PRE_ESTABLISH		=  2,
	LWS_CALLBACK_CLIENT_ESTABLISHED				=  3,
	LWS_CALLBACK_CLIENT_CLOSED				= 75,
	LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER		= 24,
	LWS_CALLBACK_CLIENT_RECEIVE				=  8,
	LWS_CALLBACK_CLIENT_RECEIVE_PONG			=  9,
	LWS_CALLBACK_CLIENT_WRITEABLE				= 10,
	LWS_CALLBACK_CLIENT_CONFIRM_EXTENSION_SUPPORTED		= 26,
	LWS_CALLBACK_WS_EXT_DEFAULTS				= 39,
	LWS_CALLBACK_FILTER_NETWORK_CONNECTION			= 17,
	LWS_CALLBACK_WS_CLIENT_BIND_PROTOCOL			= 79,
	LWS_CALLBACK_WS_CLIENT_DROP_PROTOCOL			= 80,
	LWS_CALLBACK_GET_THREAD_ID				= 31,
	LWS_CALLBACK_ADD_POLL_FD				= 32,
	LWS_CALLBACK_DEL_POLL_FD				= 33,
	LWS_CALLBACK_CHANGE_MODE_POLL_FD			= 34,
	LWS_CALLBACK_LOCK_POLL					= 35,
	LWS_CALLBACK_UNLOCK_POLL				= 36,
	LWS_CALLBACK_CGI					= 40,
	LWS_CALLBACK_CGI_TERMINATED				= 41,
	LWS_CALLBACK_CGI_STDIN_DATA				= 42,
	LWS_CALLBACK_CGI_STDIN_COMPLETED			= 43,
	LWS_CALLBACK_CGI_PROCESS_ATTACH				= 70,
	LWS_CALLBACK_SESSION_INFO				= 54,
	LWS_CALLBACK_GS_EVENT					= 55,
	LWS_CALLBACK_HTTP_PMO					= 56,
	LWS_CALLBACK_RAW_PROXY_CLI_RX				= 89,
	LWS_CALLBACK_RAW_PROXY_SRV_RX				= 90,
	LWS_CALLBACK_RAW_PROXY_CLI_CLOSE			= 91,
	LWS_CALLBACK_RAW_PROXY_SRV_CLOSE			= 92,
	LWS_CALLBACK_RAW_PROXY_CLI_WRITEABLE			= 93,
	LWS_CALLBACK_RAW_PROXY_SRV_WRITEABLE			= 94,
	LWS_CALLBACK_RAW_PROXY_CLI_ADOPT			= 95,
	LWS_CALLBACK_RAW_PROXY_SRV_ADOPT			= 96,
	LWS_CALLBACK_RAW_PROXY_CLI_BIND_PROTOCOL		= 97,
	LWS_CALLBACK_RAW_PROXY_SRV_BIND_PROTOCOL		= 98,
	LWS_CALLBACK_RAW_PROXY_CLI_DROP_PROTOCOL		= 99,
	LWS_CALLBACK_RAW_PROXY_SRV_DROP_PROTOCOL		= 100,
	LWS_CALLBACK_RAW_RX					= 59,
	LWS_CALLBACK_RAW_CLOSE					= 60,
	LWS_CALLBACK_RAW_WRITEABLE				= 61,
	LWS_CALLBACK_RAW_ADOPT					= 62,
	LWS_CALLBACK_RAW_CONNECTED				= 101,
	LWS_CALLBACK_RAW_SKT_BIND_PROTOCOL			= 81,
	LWS_CALLBACK_RAW_SKT_DROP_PROTOCOL			= 82,
	LWS_CALLBACK_RAW_ADOPT_FILE				= 63,
	LWS_CALLBACK_RAW_RX_FILE				= 64,
	LWS_CALLBACK_RAW_WRITEABLE_FILE				= 65,
	LWS_CALLBACK_RAW_CLOSE_FILE				= 66,
	LWS_CALLBACK_RAW_FILE_BIND_PROTOCOL			= 83,
	LWS_CALLBACK_RAW_FILE_DROP_PROTOCOL			= 84,
	LWS_CALLBACK_TIMER					= 73,
	LWS_CALLBACK_EVENT_WAIT_CANCELLED			= 71,
	LWS_CALLBACK_CHILD_CLOSING				= 69,
	LWS_CALLBACK_CONNECTING					= 105,
	LWS_CALLBACK_VHOST_CERT_AGING				= 72,
	LWS_CALLBACK_VHOST_CERT_UPDATE				= 74,
	LWS_CALLBACK_MQTT_NEW_CLIENT_INSTANTIATED		= 200,
	LWS_CALLBACK_MQTT_IDLE					= 201,
	LWS_CALLBACK_MQTT_CLIENT_ESTABLISHED			= 202,
	LWS_CALLBACK_MQTT_SUBSCRIBED				= 203,
	LWS_CALLBACK_MQTT_CLIENT_WRITEABLE			= 204,
	LWS_CALLBACK_MQTT_CLIENT_RX				= 205,
	LWS_CALLBACK_MQTT_UNSUBSCRIBED				= 206,
	LWS_CALLBACK_MQTT_DROP_PROTOCOL				= 207,
	LWS_CALLBACK_MQTT_CLIENT_CLOSED				= 208,
	LWS_CALLBACK_MQTT_ACK					= 209,
	LWS_CALLBACK_MQTT_RESEND				= 210,
	LWS_CALLBACK_MQTT_UNSUBSCRIBE_TIMEOUT			= 211,
	LWS_CALLBACK_USER = 1000,
};

typedef int
lws_callback_function(struct lws *wsi, enum lws_callback_reasons reason,
		    void *user, void *in, size_t len);

#define LWS_CB_REASON_AUX_BF__CGI		1
#define LWS_CB_REASON_AUX_BF__PROXY		2
#define LWS_CB_REASON_AUX_BF__CGI_CHUNK_END	4
#define LWS_CB_REASON_AUX_BF__CGI_HEADERS	8
#define LWS_CB_REASON_AUX_BF__PROXY_TRANS_END	16
#define LWS_CB_REASON_AUX_BF__PROXY_HEADERS	32

// By GST @Date