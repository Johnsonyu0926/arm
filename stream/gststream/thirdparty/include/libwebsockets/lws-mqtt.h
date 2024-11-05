#ifndef _LWS_MQTT_H
#define _LWS_MQTT_H 1

struct _lws_mqtt_related;
typedef struct _lws_mqtt_related lws_mqtt_related_t;
struct lws_mqtt_str_st;
typedef struct lws_mqtt_str_st lws_mqtt_str_t;

#define MQTT_VER_3_1_1 4
#define LWS_MQTT_FINAL_PART 1
#define LWS_MQTT_MAX_AWSIOT_TOPICLEN  256
#define LWS_MQTT_MAX_TOPICLEN  65535
#define LWS_MQTT_MAX_CIDLEN    128
#define LWS_MQTT_RANDOM_CIDLEN 23

typedef enum {
	QOS0,
	QOS1,
	QOS2,
	RESERVED_QOS_LEVEL,
	FAILURE_QOS_LEVEL = 0x80
} lws_mqtt_qos_levels_t;

typedef union {
	struct {
		uint8_t		retain:1;
		uint8_t 	qos:2;
		uint8_t 	dup:1;
		uint8_t 	ctrl_pkt_type:4;
	} flags;
	uint8_t bits;
} lws_mqtt_fixed_hdr_t;

typedef struct lws_mqtt_client_connect_param_s {
	const char *client_id;
	uint16_t keep_alive;
	uint8_t clean_start:1;
	uint8_t client_id_nofree:1;
	struct {
		const char *topic;
		const char *message;
		lws_mqtt_qos_levels_t qos;
		uint8_t retain;
	} will_param;
	const char *username;
	const char *password;
	uint8_t aws_iot;
} lws_mqtt_client_connect_param_t;

typedef struct lws_mqtt_publish_param_s {
	char *topic;
	uint16_t topic_len;
	const void *payload;
	uint32_t payload_len;
	uint32_t payload_pos;
	lws_mqtt_qos_levels_t qos;
	uint16_t packet_id;
	uint8_t dup:1;
} lws_mqtt_publish_param_t;

typedef struct topic_elem {
	const char *name;
	lws_mqtt_qos_levels_t qos;
	uint8_t acked;
} lws_mqtt_topic_elem_t;

typedef struct lws_mqtt_subscribe_param_s {
	uint32_t num_topics;
	lws_mqtt_topic_elem_t *topic;
	uint16_t packet_id;
} lws_mqtt_subscribe_param_t;

typedef enum {
	LMQCP_RESERVED,
	LMQCP_CTOS_CONNECT,
	LMQCP_STOC_CONNACK,
	LMQCP_PUBLISH,
	LMQCP_PUBACK,
	LMQCP_PUBREC,
	LMQCP_PUBREL,
	LMQCP_PUBCOMP,
	LMQCP_CTOS_SUBSCRIBE,
	LMQCP_STOC_SUBACK,
	LMQCP_CTOS_UNSUBSCRIBE,
	LMQCP_STOC_UNSUBACK,
	LMQCP_CTOS_PINGREQ,
	LMQCP_STOC_PINGRESP,
	LMQCP_DISCONNECT,
	LMQCP_AUTH
} lws_mqtt_control_packet_t;

typedef enum {
	LMQCFT_CLIENT_ID_NOFREE = (1 << 8),
	LMQCFT_USERNAME = (1 << 7),
	LMQCFT_PASSWORD = (1 << 6),
	LMQCFT_WILL_RETAIN = (1 << 5),
	LMQCFT_WILL_QOS = (1 << 3),
	LMQCFT_WILL_FLAG = (1 << 2),
	LMQCFT_CLEAN_START = (1 << 1),
	LMQCFT_RESERVED = (1 << 0),
	LMQCFT_WILL_QOS_MASK = (3 << 3),
} lws_mqtt_connect_flags_t;

typedef enum {
	LMQCFT_SESSION_PRESENT = (1 << 0),
} lws_mqtt_connack_flags_t;

typedef enum {
	LMQCP_REASON_SUCCESS = 0x00,
	LMQCP_REASON_NORMAL_DISCONNECTION = 0x00,
	LMQCP_REASON_GRANTED_QOS0 = 0x00,
	LMQCP_REASON_GRANTED_QOS1 = 0x01,
	LMQCP_REASON_GRANTED_QOS2 = 0x02,
	LMQCP_REASON_DISCONNECT_WILL = 0x04,
	LMQCP_REASON_NO_MATCHING_SUBSCRIBER = 0x10,
	LMQCP_REASON_NO_SUBSCRIPTION_EXISTED = 0x11,
	LMQCP_REASON_CONTINUE_AUTHENTICATION = 0x18,
	LMQCP_REASON_RE_AUTHENTICATE = 0x19,
	LMQCP_REASON_UNSPECIFIED_ERROR = 0x80,
	LMQCP_REASON_MALFORMED_PACKET = 0x81,
	LMQCP_REASON_PROTOCOL_ERROR = 0x82,
	LMQCP_REASON_IMPLEMENTATION_SPECIFIC_ERROR = 0x83,
	LMQCP_REASON_UNSUPPORTED_PROTOCOL = 0x84,
	LMQCP_REASON_CLIENT_ID_INVALID = 0x85,
	LMQCP_REASON_BAD_CREDENTIALS = 0x86,
	LMQCP_REASON_NOT_AUTHORIZED = 0x87,
	LMQCP_REASON_SERVER_UNAVAILABLE = 0x88,
	LMQCP_REASON_SERVER_BUSY = 0x89,
	LMQCP_REASON_BANNED = 0x8a,
	LMQCP_REASON_SERVER_SHUTTING_DOWN = 0x8b,
	LMQCP_REASON_BAD_AUTHENTICATION_METHOD = 0x8c,
	LMQCP_REASON_KEEPALIVE_TIMEOUT = 0x8d,
	LMQCP_REASON_SESSION_TAKEN_OVER = 0x8e,
	LMQCP_REASON_TOPIC_FILTER_INVALID = 0x8f,
	LMQCP_REASON_TOPIC_NAME_INVALID = 0x90,
	LMQCP_REASON_PACKET_ID_IN_USE = 0x91,
	LMQCP_REASON_PACKET_ID_NOT_FOUND = 0x92,
	LMQCP_REASON_MAX_RX_EXCEEDED = 0x93,
	LMQCP_REASON_TOPIC_ALIAS_INVALID = 0x94,
	LMQCP_REASON_PACKET_TOO_LARGE = 0x95,
	LMQCP_REASON_RATELIMIT = 0x96,
	LMQCP_REASON_QUOTA_EXCEEDED = 0x97,
	LMQCP_REASON_ADMINISTRATIVE_ACTION = 0x98,
	LMQCP_REASON_PAYLOAD_FORMAT_INVALID = 0x99,
	LMQCP_REASON_RETAIN_NOT_SUPPORTED = 0x9a,
	LMQCP_REASON_QOS_NOT_SUPPORTED = 0x9b,
	LMQCP_REASON_USE_ANOTHER_SERVER = 0x9c,
	LMQCP_REASON_SERVER_MOVED = 0x9d,
	LMQCP_REASON_SHARED_SUBSCRIPTIONS_NOT_SUPPORTED = 0x9e,
	LMQCP_REASON_CONNECTION_RATE_EXCEEDED = 0x9f,
	LMQCP_REASON_MAXIMUM_CONNECT_TIME = 0xa0,
	LMQCP_REASON_SUBSCRIPTION_IDS_NOT_SUPPORTED = 0xa1,
	LMQCP_REASON_WILDCARD_SUBSCRIPTIONS_NOT_SUPPORTED = 0xa2,
} lws_mqtt_reason_t;

typedef enum {
	LMQPROP_INVALID,
	LMQPROP_PAYLOAD_FORMAT_INDICATOR = 0x01,
	LMQPROP_MESSAGE_EXPIRY_INTERVAL = 0x02,
	LMQPROP_CONTENT_TYPE = 0x03,
	LMQPROP_RESPONSE_TOPIC = 0x08,
	LMQPROP_CORRELATION_DATA = 0x09,
	LMQPROP_SUBSCRIPTION_IDENTIFIER = 0x0b,
	LMQPROP_SESSION_EXPIRY_INTERVAL = 0x11,
	LMQPROP_ASSIGNED_CLIENT_IDENTIFIER = 0x12,
	LMQPROP_SERVER_KEEP_ALIVE = 0x13,
	LMQPROP_AUTHENTICATION_METHOD = 0x15,
	LMQPROP_AUTHENTICATION_DATA = 0x16,
	LMQPROP_REQUEST_PROBLEM_INFORMATION = 0x17,
	LMQPROP_WILL_DELAY_INTERVAL = 0x18,
	LMQPROP_REQUEST_RESPONSE_INFORMATION = 0x19,
	LMQPROP_RESPONSE_INFORMATION = 0x1a,
	LMQPROP_SERVER_REFERENCE = 0x1c,
	LMQPROP_REASON_STRING = 0x1f,
	LMQPROP_RECEIVE_MAXIMUM = 0x21,
	LMQPROP_TOPIC_ALIAS_MAXIMUM = 0x22,
	LMQPROP_TOPIC_ALIAS = 0x23,
	LMQPROP_MAXIMUM_QOS = 0x24,
	LMQPROP_RETAIN_AVAILABLE = 0x25,
	LMQPROP_USER_PROPERTY = 0x26,
	LMQPROP_MAXIMUM_PACKET_SIZE = 0x27,
	LMQPROP_WILDCARD_SUBSCRIPTION_AVAIL = 0x28,
	LMQPROP_SUBSCRIPTION_IDENTIFIER_AVAIL = 0x29,
	LMQPROP_SHARED_SUBSCRIPTION_AVAIL = 0x2a
} lws_mqtt_property;

int lws_read_mqtt(struct lws *wsi, unsigned char *buf, lws_filepos_t len);

LWS_VISIBLE LWS_EXTERN int lws_mqtt_bindata_cmp(const lws_mqtt_str_t *bd1, const lws_mqtt_str_t *bd2);
LWS_VISIBLE LWS_EXTERN void lws_mqtt_str_init(lws_mqtt_str_t *s, uint8_t *buf, uint16_t lim, char nf);
LWS_VISIBLE LWS_EXTERN lws_mqtt_str_t * lws_mqtt_str_create(uint16_t lim);
LWS_VISIBLE LWS_EXTERN lws_mqtt_str_t * lws_mqtt_str_create_init(uint8_t *buf, uint16_t len, uint16_t lim);
LWS_VISIBLE LWS_EXTERN lws_mqtt_str_t * lws_mqtt_str_create_cstr_dup(const char *buf, uint16_t lim);
LWS_VISIBLE LWS_EXTERN uint8_t * lws_mqtt_str_next(lws_mqtt_str_t *s, uint16_t *budget);
LWS_VISIBLE LWS_EXTERN int lws_mqtt_str_advance(lws_mqtt_str_t *s, int n);
LWS_VISIBLE LWS_EXTERN void lws_mqtt_str_free(lws_mqtt_str_t **s);

LWS_VISIBLE LWS_EXTERN int lws_mqtt_client_send_publish(struct lws *wsi, lws_mqtt_publish_param_t *pub, const void *buf, uint32_t len, int final);
LWS_VISIBLE LWS_EXTERN int lws_mqtt_client_send_subcribe(struct lws *wsi, lws_mqtt_subscribe_param_t *sub);
LWS_VISIBLE LWS_EXTERN int LWS_WARN_UNUSED_RESULT lws_mqtt_client_send_unsubcribe(struct lws *wsi, const lws_mqtt_subscribe_param_t *unsub);

#endif /* _LWS_MQTT_H */
// By GST @Date