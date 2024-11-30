// zephyr-3.7-branch/include/zephyr/net/coap.h

#ifndef ZEPHYR_INCLUDE_NET_COAP_H_
#define ZEPHYR_INCLUDE_NET_COAP_H_

/**
 * @brief COAP库
 * @defgroup coap COAP库
 * @since 1.10
 * @version 0.8.0
 * @ingroup networking
 * @{
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <stdbool.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/slist.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 我们知道的一组CoAP数据包选项。
 *
 * 用户可以将这些选项以外的选项添加到他们的数据包中，前提是他们知道如何正确格式化它们。唯一的限制是所有选项必须按数字顺序添加到数据包中。
 *
 * 有关更多信息，请参阅RFC 7252，第12.2节。
 */
enum coap_option_num {
	COAP_OPTION_IF_MATCH = 1,        /**< If-Match */
	COAP_OPTION_URI_HOST = 3,        /**< Uri-Host */
	COAP_OPTION_ETAG = 4,            /**< ETag */
	COAP_OPTION_IF_NONE_MATCH = 5,   /**< If-None-Match */
	COAP_OPTION_OBSERVE = 6,         /**< Observe (RFC 7641) */
	COAP_OPTION_URI_PORT = 7,        /**< Uri-Port */
	COAP_OPTION_LOCATION_PATH = 8,   /**< Location-Path */
	COAP_OPTION_URI_PATH = 11,       /**< Uri-Path */
	COAP_OPTION_CONTENT_FORMAT = 12, /**< Content-Format */
	COAP_OPTION_MAX_AGE = 14,        /**< Max-Age */
	COAP_OPTION_URI_QUERY = 15,      /**< Uri-Query */
	COAP_OPTION_ACCEPT = 17,         /**< Accept */
	COAP_OPTION_LOCATION_QUERY = 20, /**< Location-Query */
	COAP_OPTION_BLOCK2 = 23,         /**< Block2 (RFC 7959) */
	COAP_OPTION_BLOCK1 = 27,         /**< Block1 (RFC 7959) */
	COAP_OPTION_SIZE2 = 28,          /**< Size2 (RFC 7959) */
	COAP_OPTION_PROXY_URI = 35,      /**< Proxy-Uri */
	COAP_OPTION_PROXY_SCHEME = 39,   /**< Proxy-Scheme */
	COAP_OPTION_SIZE1 = 60,          /**< Size1 */
	COAP_OPTION_ECHO = 252,          /**< Echo (RFC 9175) */
	COAP_OPTION_REQUEST_TAG = 292    /**< Request-Tag (RFC 9175) */
};

/**
 * @brief 可用的请求方法。
 *
 * 在创建请求或响应时使用。
 */
enum coap_method {
	COAP_METHOD_GET = 1,     /**< GET */
	COAP_METHOD_POST = 2,    /**< POST */
	COAP_METHOD_PUT = 3,     /**< PUT */
	COAP_METHOD_DELETE = 4,  /**< DELETE */
	COAP_METHOD_FETCH = 5,   /**< FETCH */
	COAP_METHOD_PATCH = 6,   /**< PATCH */
	COAP_METHOD_IPATCH = 7,  /**< IPATCH */
};

/** @cond INTERNAL_HIDDEN */

#define COAP_REQUEST_MASK 0x07

#define COAP_VERSION_1 1U

#define COAP_OBSERVE_MAX_AGE 0xFFFFFF

/** @endcond */

/**
 * @brief CoAP数据包可能是以下类型之一。
 */
enum coap_msgtype {
	/**
	 * 确认消息。
	 *
	 * 数据包是请求或响应，目标端点必须确认。
	 */
	COAP_TYPE_CON = 0,
	/**
	 * 非确认消息。
	 *
	 * 数据包是请求或响应，不需要确认。
	 */
	COAP_TYPE_NON_CON = 1,
	/**
	 * 确认。
	 *
	 * 对确认消息的响应。
	 */
	COAP_TYPE_ACK = 2,
	/**
	 * 重置。
	 *
	 * 出于任何原因拒绝数据包是通过发送此类型的消息来完成的。
	 */
	COAP_TYPE_RESET = 3
};

/**
 * 实用宏，用于创建CoAP响应代码。
 * @param class 响应代码的类（例如2、4、5等）
 * @param det 响应代码的详细信息
 * @return 响应代码字面量
 */
#define COAP_MAKE_RESPONSE_CODE(class, det) ((class << 5) | (det))

/**
 * @brief 可用于响应数据包的响应代码集。
 *
 * 在创建响应时使用。
 */
enum coap_response_code {
	/** 2.00 - OK */
	COAP_RESPONSE_CODE_OK = COAP_MAKE_RESPONSE_CODE(2, 0),
	/** 2.01 - Created */
	COAP_RESPONSE_CODE_CREATED = COAP_MAKE_RESPONSE_CODE(2, 1),
	/** 2.02 - Deleted */
	COAP_RESPONSE_CODE_DELETED = COAP_MAKE_RESPONSE_CODE(2, 2),
	/** 2.03 - Valid */
	COAP_RESPONSE_CODE_VALID = COAP_MAKE_RESPONSE_CODE(2, 3),
	/** 2.04 - Changed */
	COAP_RESPONSE_CODE_CHANGED = COAP_MAKE_RESPONSE_CODE(2, 4),
	/** 2.05 - Content */
	COAP_RESPONSE_CODE_CONTENT = COAP_MAKE_RESPONSE_CODE(2, 5),
	/** 2.31 - Continue */
	COAP_RESPONSE_CODE_CONTINUE = COAP_MAKE_RESPONSE_CODE(2, 31),
	/** 4.00 - Bad Request */
	COAP_RESPONSE_CODE_BAD_REQUEST = COAP_MAKE_RESPONSE_CODE(4, 0),
	/** 4.01 - Unauthorized */
	COAP_RESPONSE_CODE_UNAUTHORIZED = COAP_MAKE_RESPONSE_CODE(4, 1),
	/** 4.02 - Bad Option */
	COAP_RESPONSE_CODE_BAD_OPTION = COAP_MAKE_RESPONSE_CODE(4, 2),
	/** 4.03 - Forbidden */
	COAP_RESPONSE_CODE_FORBIDDEN = COAP_MAKE_RESPONSE_CODE(4, 3),
	/** 4.04 - Not Found */
	COAP_RESPONSE_CODE_NOT_FOUND = COAP_MAKE_RESPONSE_CODE(4, 4),
	/** 4.05 - Method Not Allowed */
	COAP_RESPONSE_CODE_NOT_ALLOWED = COAP_MAKE_RESPONSE_CODE(4, 5),
	/** 4.06 - Not Acceptable */
	COAP_RESPONSE_CODE_NOT_ACCEPTABLE = COAP_MAKE_RESPONSE_CODE(4, 6),
	/** 4.08 - Request Entity Incomplete */
	COAP_RESPONSE_CODE_INCOMPLETE = COAP_MAKE_RESPONSE_CODE(4, 8),
	/** 4.12 - Precondition Failed */
	COAP_RESPONSE_CODE_CONFLICT = COAP_MAKE_RESPONSE_CODE(4, 9),
	/** 4.12 - Precondition Failed */
	COAP_RESPONSE_CODE_PRECONDITION_FAILED = COAP_MAKE_RESPONSE_CODE(4, 12),
	/** 4.13 - Request Entity Too Large */
	COAP_RESPONSE_CODE_REQUEST_TOO_LARGE = COAP_MAKE_RESPONSE_CODE(4, 13),
	/** 4.15 - Unsupported Content-Format */
	COAP_RESPONSE_CODE_UNSUPPORTED_CONTENT_FORMAT =
						COAP_MAKE_RESPONSE_CODE(4, 15),
	/** 4.22 - Unprocessable Entity */
	COAP_RESPONSE_CODE_UNPROCESSABLE_ENTITY = COAP_MAKE_RESPONSE_CODE(4, 22),
	/** 4.29 - Too Many Requests */
	COAP_RESPONSE_CODE_TOO_MANY_REQUESTS = COAP_MAKE_RESPONSE_CODE(4, 29),
	/** 5.00 - Internal Server Error */
	COAP_RESPONSE_CODE_INTERNAL_ERROR = COAP_MAKE_RESPONSE_CODE(5, 0),
	/** 5.01 - Not Implemented */
	COAP_RESPONSE_CODE_NOT_IMPLEMENTED = COAP_MAKE_RESPONSE_CODE(5, 1),
	/** 5.02 - Bad Gateway */
	COAP_RESPONSE_CODE_BAD_GATEWAY = COAP_MAKE_RESPONSE_CODE(5, 2),
	/** 5.03 - Service Unavailable */
	COAP_RESPONSE_CODE_SERVICE_UNAVAILABLE = COAP_MAKE_RESPONSE_CODE(5, 3),
	/** 5.04 - Gateway Timeout */
	COAP_RESPONSE_CODE_GATEWAY_TIMEOUT = COAP_MAKE_RESPONSE_CODE(5, 4),
	/** 5.05 - Proxying Not Supported */
	COAP_RESPONSE_CODE_PROXYING_NOT_SUPPORTED =
						COAP_MAKE_RESPONSE_CODE(5, 5)
};

/** @cond INTERNAL_HIDDEN */

#define COAP_CODE_EMPTY (0)

#define COAP_TOKEN_MAX_LEN 8UL

/** @endcond */

/**
 * @brief CoAP的Content-Format选项值集。
 *
 * 在编码或解码Content-Format选项时使用。
 */
enum coap_content_format {
	COAP_CONTENT_FORMAT_TEXT_PLAIN = 0,             /**< text/plain;charset=utf-8 */
	COAP_CONTENT_FORMAT_APP_LINK_FORMAT = 40,       /**< application/link-format */
	COAP_CONTENT_FORMAT_APP_XML = 41,               /**< application/xml */
	COAP_CONTENT_FORMAT_APP_OCTET_STREAM = 42,      /**< application/octet-stream */
	COAP_CONTENT_FORMAT_APP_EXI = 47,               /**< application/exi */
	COAP_CONTENT_FORMAT_APP_JSON = 50,              /**< application/json */
	COAP_CONTENT_FORMAT_APP_JSON_PATCH_JSON = 51,   /**< application/json-patch+json */
	COAP_CONTENT_FORMAT_APP_MERGE_PATCH_JSON = 52,  /**< application/merge-patch+json */
	COAP_CONTENT_FORMAT_APP_CBOR = 60               /**< application/cbor */
};

/** @cond INTERNAL_HIDDEN */

/* 块选项助手 */
#define GET_BLOCK_NUM(v)        ((v) >> 4)
#define GET_BLOCK_SIZE(v)       (((v) & 0x7))
#define GET_MORE(v)             (!!((v) & 0x08))

/** @endcond */

struct coap_observer;
struct coap_packet;
struct coap_pending;
struct coap_reply;
struct coap_resource;

/**
 * @typedef coap_method_t
 * @brief 当远程实体调用资源的方法时调用的回调类型。
 */
typedef int (*coap_method_t)(struct coap_resource *resource,
			     struct coap_packet *request,
			     struct sockaddr *addr, socklen_t addr_len);

/**
 * @typedef coap_notify_t
 * @brief 当资源有观察者需要在更新时通知时调用的回调类型。
 */
typedef void (*coap_notify_t)(struct coap_resource *resource,
			      struct coap_observer *observer);

/**
 * @brief CoAP资源的描述。
 *
 * CoAP服务器通常希望注册资源，以便客户端可以对其进行操作，通过获取其状态或请求更新。
 */
struct coap_resource {
	/** 为每个CoAP方法调用的函数 */
	coap_method_t get, post, put, del, fetch, patch, ipatch;
	/** 通知函数 */
	coap_notify_t notify;
	/** 资源路径 */
	const char * const *path;
	/** 用户特定的不透明数据 */
	void *user_data;
	/** 资源观察者列表 */
	sys_slist_t observers;
	/** 资源年龄 */
	int age;
};

/**
 * @brief 表示观察本地资源的远程设备。
 */
struct coap_observer {
	/** 观察者列表节点 */
	sys_snode_t list;
	/** 观察者连接端点信息 */
	struct sockaddr addr;
	/** 观察者令牌 */
	uint8_t token[8];
	/** 扩展令牌长度 */
	uint8_t tkl;
};

/**
 * @brief CoAP数据包的表示。
 */
struct coap_packet {
	uint8_t *data;    /**< 用户分配的缓冲区 */
	uint16_t offset;  /**< CoAP库在添加数据时维护的偏移量 */
	uint16_t max_len; /**< CoAP数据包数据的最大长度 */
	uint8_t hdr_len;  /**< CoAP头部长度 */
	uint16_t opt_len; /**< 总选项长度（delta + len + value） */
	uint16_t delta;   /**< 用于在CoAP数据包中计算delta */
#if defined(CONFIG_COAP_KEEP_USER_DATA) || defined(DOXYGEN)
	/**
	 * 应用程序特定的用户数据。
	 * 仅在启用@kconfig{CONFIG_COAP_KEEP_USER_DATA}时可用。
	 */
	void *user_data;
#endif
};

/**
 * @brief CoAP选项的表示。
 */
struct coap_option {
	uint16_t delta;     /**< 选项delta */
#if defined(CONFIG_COAP_EXTENDED_OPTIONS_LEN)
	uint16_t len;
	uint8_t value[CONFIG_COAP_EXTENDED_OPTIONS_LEN_VALUE];
#else
	uint8_t len;        /**< 选项长度 */
	uint8_t value[12];  /**< 选项值 */
#endif
};

/**
 * @typedef coap_reply_t
 * @brief 当响应与挂起请求匹配时调用的助手函数。
 * 发送块时，仅在接收到最后一个块的回复时执行回调。
 * 即，当回复代码为“继续”（2.31）时，不会调用它。
 */
typedef int (*coap_reply_t)(const struct coap_packet *response,
			    struct coap_reply *reply,
			    const struct sockaddr *from);

/**
 * @brief CoAP传输参数。
 */
struct coap_transmission_parameters {
	/** 初始ACK超时。值用作重试挂起的CoAP数据包的基值。 */
	uint32_t ack_timeout;
	/** 设置CoAP重试回退因子。值为200表示因子为2.0。 */
	uint16_t coap_backoff_percent;
	/** 最大重传次数。 */
	uint8_t max_retransmission;
};

/**
 * @brief 表示等待确认（ACK）的请求。
 */
struct coap_pending {
	struct sockaddr addr; /**< 远程地址 */
	int64_t t0;           /**< 发送请求的时间 */
	uint32_t timeout;     /**< 超时时间（毫秒） */
	uint16_t id;          /**< 消息ID */
	uint8_t *data;        /**< 用户分配的缓冲区 */
	uint16_t len;         /**< CoAP数据包的长度 */
	uint8_t retries;      /**< 请求已发送的次数 */
	struct coap_transmission_parameters params; /**< 传输参数 */
};

/**
 * @brief 表示请求的回复处理程序，也用于观察资源时。
 */
struct coap_reply {
	/** CoAP回复回调 */
	coap_reply_t reply;
	/** 用户特定的不透明数据 */
	void *user_data;
	/** 回复年龄 */
	int age;
	/** 回复ID */
	uint16_t id;
	/** 回复令牌 */
	uint8_t token[8];
	/** 扩展令牌长度 */
	uint8_t tkl;
};

/**
 * @brief 返回CoAP数据包中的版本。
 *
 * @param cpkt CoAP数据包表示
 *
 * @return 数据包中的CoAP版本
 */
uint8_t coap_header_get_version(const struct coap_packet *cpkt);

/**
 * @brief 返回CoAP数据包的类型。
 *
 * @param cpkt CoAP数据包表示
 *
 * @return 数据包的类型
 */
uint8_t coap_header_get_type(const struct coap_packet *cpkt);

/**
 * @brief 返回CoAP数据包中的令牌（如果有）。
 *
 * @param cpkt CoAP数据包表示
 * @param token 存储令牌的位置，必须指向包含至少COAP_TOKEN_MAX_LEN字节的缓冲区
 *
 * @return CoAP数据包中的令牌长度（0 - COAP_TOKEN_MAX_LEN）。
 */
uint8_t coap_header_get_token(const struct coap_packet *cpkt, uint8_t *token);

/**
 * @brief 返回CoAP数据包的代码。
 *
 * @param cpkt CoAP数据包表示
 *
 * @return 数据包中的代码
 */
uint8_t coap_header_get_code(const struct coap_packet *cpkt);

/**
 * @brief 修改CoAP数据包的代码。
 *
 * @param cpkt CoAP数据包表示
 * @param code CoAP代码
 * @return 成功返回0，失败返回-EINVAL
 */
int coap_header_set_code(const struct coap_packet *cpkt, uint8_t code);

/**
 * @brief 返回与CoAP数据包关联的消息ID。
 *
 * @param cpkt CoAP数据包表示
 *
 * @return 数据包中的消息ID
 */
uint16_t coap_header_get_id(const struct coap_packet *cpkt);

/**
 * @brief 返回CoAP数据包的数据指针和长度。
 *
 * @param cpkt CoAP数据包表示
 * @param len CoAP有效负载的总长度
 *
 * @return 如果存在有效负载，则返回数据指针和长度
 *         如果没有有效负载，则返回NULL指针并将长度设置为0
 */
const uint8_t *coap_packet_get_payload(const struct coap_packet *cpkt,
				       uint16_t *len);

/**
 * @brief 验证CoAP URI路径是否与提供的选项匹配。
 *
 * @param path 以空结尾的字符串数组。
 * @param options 从coap_packet_parse()解析的选项
 * @param opt_num 选项数量
 *
 * @return 如果CoAP URI路径匹配，则返回true，
 *         否则返回false。
 */
bool coap_uri_path_match(const char * const *path,
			 struct coap_option *options,
			 uint8_t opt_num);

/**
 * @brief 解析数据中的CoAP数据包，验证并初始化@a cpkt。@a data在使用@a cpkt时必须保持有效。
 *
 * @param cpkt 从接收到的数据初始化的数据包。
 * @param data 包含CoAP数据包的数据，其@a data指针定位在CoAP数据包的起始位置。
 * @param len 数据的长度
 * @param options 解析选项并缓存其详细信息。
 * @param opt_num 选项数量
 *
 * @retval 成功返回0。
 * @retval 输入参数无效返回-EINVAL。
 * @retval CoAP数据包头格式错误返回-EBADMSG。
 * @retval CoAP选项格式错误返回-EILSEQ。
 */
int coap_packet_parse(struct coap_packet *cpkt, uint8_t *data, uint16_t len,
		      struct coap_option *options, uint8_t opt_num);

/**
 * @brief 解析提供的CoAP路径（带或不带查询）或查询，并将其作为选项附加到@a cpkt。
 *
 * @param cpkt 要附加路径和查询选项的数据包。
 * @param path 以空结尾的CoAP路径、查询
 * 或两者的字符串。
 *
 * @retval 成功返回0，错误返回负值。
 */
int coap_packet_set_path(struct coap_packet *cpkt, const char *path);
/**
 * @brief 从输入数据创建一个新的CoAP数据包。
 *
 * @param cpkt 使用@a data中的存储初始化的新数据包。
 * @param data 将包含CoAP数据包信息的数据
 * @param max_len 数据的最大允许长度
 * @param ver CoAP头版本
 * @param type CoAP头类型
 * @param token_len CoAP头令牌长度
 * @param token CoAP头令牌
 * @param code CoAP头代码
 * @param id CoAP头消息ID
 *
 * @return 成功返回0，错误返回负值。
 */
int coap_packet_init(struct coap_packet *cpkt, uint8_t *data, uint16_t max_len,
		     uint8_t ver, uint8_t type, uint8_t token_len,
		     const uint8_t *token, uint8_t code, uint16_t id);
/**
 * @brief 为给定请求创建一个新的CoAP确认消息。
 *
 * 此函数类似于@ref coap_packet_init，填充CoAP头类型、CoAP头令牌和CoAP头消息ID字段，符合确认规则。
 *
 * @param cpkt 使用@a data中的存储初始化的新数据包。
 * @param req 正在确认的CoAP请求数据包
 * @param data 将包含CoAP数据包信息的数据
 * @param max_len 数据的最大允许长度
 * @param code CoAP头代码
 *
 * @return 成功返回0，错误返回负值。
 */
int coap_ack_init(struct coap_packet *cpkt, const struct coap_packet *req,
		  uint8_t *data, uint16_t max_len, uint8_t code);
/**
 * @brief 返回一个随机生成的8字节数组，可用作消息的令牌。
 *
 * @return 一个8字节的伪随机令牌。
 */
uint8_t *coap_next_token(void);
/**
 * @brief 生成消息ID的助手
 *
 * @return 一个新的消息ID
 */
uint16_t coap_next_id(void);
/**
 * @brief 返回与值@a code的选项相关联的值。
 *
 * @param cpkt CoAP数据包表示
 * @param code 要查找的选项编号
 * @param options 存储找到的选项值的#coap_option数组
 * @param veclen options数组中的元素数量
 *
 * @return 数据包中匹配code的选项数量，错误时返回负值。
 */
int coap_find_options(const struct coap_packet *cpkt, uint16_t code,
		      struct coap_option *options, uint16_t veclen);
/**
 * @brief 将选项附加到数据包。
 *
 * 注意：选项可以按其代码的数字顺序添加。但按顺序添加更有效。
 *
 * @param cpkt 要更新的数据包
 * @param code 要添加到数据包的选项代码，请参见#coap_option_num
 * @param value 选项的值指针，将被复制到数据包中
 * @param len 要添加的数据大小
 *
 * @return 成功返回0，错误返回负值。
 */
int coap_packet_append_option(struct coap_packet *cpkt, uint16_t code,
			      const uint8_t *value, uint16_t len);
/**
 * @brief 从数据包中删除选项。
 *
 * @param cpkt 要更新的数据包
 * @param code 要从数据包中删除的选项代码，请参见#coap_option_num
 *
 * @return 成功返回0，错误返回负值。
 */
int coap_packet_remove_option(struct coap_packet *cpkt, uint16_t code);
/**
 * @brief 将选项转换为其整数表示形式。
 *
 * 假设数字以网络字节顺序编码在选项中。
 *
 * @param option 选项值的指针，由coap_find_options()检索
 *
 * @return 选项的整数表示形式
 */
unsigned int coap_option_value_to_int(const struct coap_option *option);
/**
 * @brief 将整数值选项附加到数据包。
 *
 * 选项必须按其代码的数字顺序添加，并且将使用最少的字节数来编码值。
 *
 * @param cpkt 要更新的数据包
 * @param code 要添加到数据包的选项代码，请参见#coap_option_num
 * @param val 要添加的整数值
 *
 * @return 成功返回0，错误返回负值。
 */
int coap_append_option_int(struct coap_packet *cpkt, uint16_t code,
			   unsigned int val);
/**
 * @brief 将有效负载标记附加到CoAP数据包
 *
 * @param cpkt 要附加有效负载标记（0xFF）的数据包
 *
 * @return 成功返回0，错误返回负值。
 */
int coap_packet_append_payload_marker(struct coap_packet *cpkt);
/**
 * @brief 将有效负载附加到CoAP数据包
 *
 * @param cpkt 要附加有效负载的数据包
 * @param payload CoAP数据包有效负载
 * @param payload_len CoAP数据包有效负载长度
 *
 * @return 成功返回0，错误返回负值。
 */
int coap_packet_append_payload(struct coap_packet *cpkt, const uint8_t *payload,
			       uint16_t payload_len);
/**
 * @brief 检查CoAP数据包是否为CoAP请求。
 *
 * @param cpkt 要检查的数据包。
 *
 * @return 如果数据包是请求，则返回true，
 *         否则返回false。
 */
bool coap_packet_is_request(const struct coap_packet *cpkt);
/**
 * @brief 当接收到请求时，调用匹配资源的适当方法。
 *
 * @param cpkt 接收到的数据包
 * @param resources 已知资源数组
 * @param resources_len 数组中的资源数量
 * @param options 从coap_packet_parse()解析的选项
 * @param opt_num 选项数量
 * @param addr 对等地址
 * @param addr_len 对等地址长度
 *
 * @retval 成功返回>= 0。
 * @retval 无效请求代码返回-ENOTSUP。
 * @retval 资源处理程序未实现返回-EPERM。
 * @retval 未找到资源返回-ENOENT。
 */
int coap_handle_request_len(struct coap_packet *cpkt,
			    struct coap_resource *resources,
			    size_t resources_len,
			    struct coap_option *options,
			    uint8_t opt_num,
			    struct sockaddr *addr, socklen_t addr_len);
/**
 * @brief 当接收到请求时，调用匹配资源的适当方法。
 *
 * @param cpkt 接收到的数据包
 * @param resources 已知资源数组（以空资源终止）
 * @param options 从coap_packet_parse()解析的选项
 * @param opt_num 选项数量
 * @param addr 对等地址
 * @param addr_len 对等地址长度
 *
 * @retval 成功返回>= 0。
 * @retval 无效请求代码返回-ENOTSUP。
 * @retval 资源处理程序未实现返回-EPERM。
 * @retval 未找到资源返回-ENOENT。
 */
int coap_handle_request(struct coap_packet *cpkt,
			struct coap_resource *resources,
			struct coap_option *options,
			uint8_t opt_num,
			struct sockaddr *addr, socklen_t addr_len);
/**
 * 表示将使用块传输[RFC7959]传输的每个块的大小：
 *
 * 每个条目直接映射到在线使用的值。
 *
 * https://tools.ietf.org/html/rfc7959
 */
enum coap_block_size {
	COAP_BLOCK_16,   /**< 16字节块大小 */
	COAP_BLOCK_32,   /**< 32字节块大小 */
	COAP_BLOCK_64,   /**< 64字节块大小 */
	COAP_BLOCK_128,  /**< 128字节块大小 */
	COAP_BLOCK_256,  /**< 256字节块大小 */
	COAP_BLOCK_512,  /**< 512字节块大小 */
	COAP_BLOCK_1024, /**< 1024字节块大小 */
};
/**
 * @brief 将枚举转换为以字节表示的大小的助手。
 *
 * @param block_size 要转换的块大小
 *
 * @return block_size表示的字节大小
 */
static inline uint16_t coap_block_size_to_bytes(
	enum coap_block_size block_size)
{
	return (1 << (block_size + 4));
}
/**
 * @brief 将字节中的块大小转换为枚举的助手。
 *
 * 注意：只有有效的CoAP块大小才能正确映射。
 *
 * @param bytes CoAP块大小（字节）。
 * @return enum coap_block_size
 */
static inline enum coap_block_size coap_bytes_to_block_size(uint16_t bytes)
{
	int sz = u32_count_trailing_zeros(bytes) - 4;
	if (sz < COAP_BLOCK_16) {
		return COAP_BLOCK_16;
	}
	if (sz > COAP_BLOCK_1024) {
		return COAP_BLOCK_1024;
	}
	return (enum coap_block_size)sz;
}
/**
 * @brief 表示块传输的当前状态。
 */
struct coap_block_context {
	/** 块传输的总大小 */
	size_t total_size;
	/** 块传输的当前大小 */
	size_t current;
	/** 块大小 */
	enum coap_block_size block_size;
};
/**
 * @brief 初始化块传输的上下文。
 *
 * @param ctx 要初始化的上下文
 * @param block_size 块的大小
 * @param total_size 传输的总大小（如果已知）
 *
 * @return 成功返回0，错误返回负值。
 */
int coap_block_transfer_init(struct coap_block_context *ctx,
			     enum coap_block_size block_size,
			     size_t total_size);
/**
 * @brief 将BLOCK1或BLOCK2选项附加到数据包。
 *
 * 如果CoAP数据包是请求，则附加BLOCK1，否则附加BLOCK2。
 *
 * @param cpkt 要更新的数据包
 * @param ctx 用于检索块选项信息的块上下文
 *
 * @return 成功返回0，错误返回负值。
 */
int coap_append_descriptive_block_option(struct coap_packet *cpkt, struct coap_block_context *ctx);
/**
 * @brief 检查数据包中是否设置了描述性块选项。
 *
 * 如果CoAP数据包是请求，则检查可用的BLOCK1选项，否则检查BLOCK2选项。
 *
 * @param cpkt 要检查的数据包。
 *
 * @return 如果设置了相应的块选项，则返回true，
 *         否则返回false。
 */
bool coap_has_descriptive_block_option(struct coap_packet *cpkt);
/**
 * @brief 从数据包中删除BLOCK1或BLOCK2选项。
 *
 * 如果CoAP数据包是请求，则删除BLOCK1，否则删除BLOCK2。
 *
 * @param cpkt 要更新的数据包。
 *
 * @return 成功返回0，错误返回负值。
 */
int coap_remove_descriptive_block_option(struct coap_packet *cpkt);
/**
 * @brief 检查BLOCK1或BLOCK2选项是否设置了更多标志
 *
 * @param cpkt 要检查的数据包。
 * @return 如果在BLOCK1或BLOCK2中设置了更多标志，则返回true
 *         如果未设置更多标志或未找到BLOCK标头，则返回false。
 */
bool coap_block_has_more(struct coap_packet *cpkt);
/**
 * @brief 将BLOCK1选项附加到数据包。
 *
 * @param cpkt 要更新的数据包
 * @param ctx 用于检索Block1选项信息的块上下文
 *
 * @return 成功返回0，错误返回负值。
 */
int coap_append_block1_option(struct coap_packet *cpkt,
			      struct coap_block_context *ctx);
/**
 * @brief 将BLOCK2选项附加到数据包。
 *
 * @param cpkt 要更新的数据包
 * @param ctx 用于检索Block2选项信息的块上下文
 *
 * @return 成功返回0，错误返回负值。
 */
int coap_append_block2_option(struct coap_packet *cpkt,
			      struct coap_block_context *ctx);
/**
 * @brief 将SIZE1选项附加到数据包。
 *
 * @param cpkt 要更新的数据包
 * @param ctx 用于检索Size1选项信息的块上下文
 *
 * @return 成功返回0，错误返回负值。
 */
int coap_append_size1_option(struct coap_packet *cpkt,
			     struct coap_block_context *ctx);
/**
 * @brief 将SIZE2选项附加到数据包。
 *
 * @param cpkt 要更新的数据包
 * @param ctx 用于检索Size2选项信息的块上下文
 *
 * @return 成功返回0，错误返回负值。
 */
int coap_append_size2_option(struct coap_packet *cpkt,
			     struct coap_block_context *ctx);
/**
 * @brief 获取CoAP选项的整数表示形式。
 *
 * @param cpkt 要检查的数据包
 * @param code CoAP选项代码
 *
 * @return 成功返回整数值>= 0，错误返回负值。
 */
int coap_get_option_int(const struct coap_packet *cpkt, uint16_t code);
/**
 * @brief 从CoAP block1选项中获取块大小、更多标志和块编号。
 *
 * @param cpkt 要检查的数据包
 * @param has_more 设置为更多标志的值
 * @param block_number 设置为块编号
 *
 * @return 成功返回块大小的整数值，错误返回负值。
 */
int coap_get_block1_option(const struct coap_packet *cpkt, bool *has_more, uint32_t *block_number);
/**
 * @brief 从CoAP block2选项中获取值。
 *
 * 从选项中解码块编号、更多标志和块大小。
 *
 * @param cpkt 要检查的数据包
 * @param has_more 设置为更多标志的值
 * @param block_number 设置为块编号
 *
 * @return 成功返回块大小的整数值，错误返回负值。
 */
int coap_get_block2_option(const struct coap_packet *cpkt, bool *has_more,
			   uint32_t *block_number);
/**
 * @brief 从@a cpkt中检索BLOCK{1,2}和SIZE{1,2}并相应地更新@a ctx。
 *
 * @param cpkt 要查找块传输选项的数据包
 * @param ctx 要更新的块上下文
 *
 * @return 成功返回0，错误返回负值。
 */
int coap_update_from_block(const struct coap_packet *cpkt,
			   struct coap_block_context *ctx);
/**
 * @brief 根据@a cpkt中的@a选项更新@a ctx，因此在调用此函数后，当前条目指示正在传输的数据体中的正确偏移量。
 *
 * @param cpkt 要查找块传输选项的数据包
 * @param ctx 要更新的块上下文
 * @param option COAP_OPTION_BLOCK1或COAP_OPTION_BLOCK2
 *
 * @return 块传输中的偏移量，传输完成时为0，错误时为负值。
 */
int coap_next_block_for_option(const struct coap_packet *cpkt,
			       struct coap_block_context *ctx,
			       enum coap_option_num option);
/**
 * @brief 更新@a ctx，因此在调用此函数后，当前条目指示正在传输的数据体中的正确偏移量。
 *
 * @param cpkt 要查找块传输选项的数据包
 * @param ctx 要更新的块上下文
 *
 * @return 块传输中的偏移量，传输完成时为0。
 */
size_t coap_next_block(const struct coap_packet *cpkt,
		       struct coap_block_context *ctx);
/**
 * @brief 表示由@a addr引用的远程设备希望观察资源。
 *
 * @param observer 要初始化的观察者
 * @param request 观察者将基于的请求
 * @param addr 远程设备的地址
 */
void coap_observer_init(struct coap_observer *observer,
			const struct coap_packet *request,
			const struct sockaddr *addr);
/**
 * @brief 观察者初始化后，将观察者与资源关联。
 *
 * @param resource 要添加观察者的资源
 * @param observer 要添加的观察者
 *
 * @return 如果这是添加到此资源的第一个观察者，则返回true。
 */
bool coap_register_observer(struct coap_resource *resource,
			    struct coap_observer *observer);
/**
 * @brief 从该资源的已注册观察者列表中删除此观察者。
 *
 * @param resource 要从中删除观察者的资源
 * @param observer 要删除的观察者
 *
 * @return 如果找到并删除了观察者，则返回true。
 */
bool coap_remove_observer(struct coap_resource *resource,
			  struct coap_observer *observer);
/**
 * @brief 返回与地址@a addr匹配并具有令牌@a token的观察者。
 *
 * @param observers 观察者数组的指针
 * @param len 观察者数组的大小
 * @param addr 观察资源的端点地址
 * @param token 令牌的指针
 * @param token_len 令牌中有效字节的长度
 *
 * @return 如果找到匹配的观察者，则返回观察者的指针，否则返回NULL。
 */
struct coap_observer *coap_find_observer(
	struct coap_observer *observers, size_t len,
	const struct sockaddr *addr,
	const uint8_t *token, uint8_t token_len);
/**
 * @brief 返回与地址@a addr匹配的观察者。
 *
 * @param observers 观察者数组的指针
 * @param len 观察者数组的大小
 * @param addr 观察资源的端点地址
 *
 * @note 如果已知观察者的地址和令牌，则应优先使用coap_find_observer()函数。
 *
 * @return 如果找到匹配的观察者，则返回观察者的指针，否则返回NULL。
 */
struct coap_observer *coap_find_observer_by_addr(
	struct coap_observer *observers, size_t len,
	const struct sockaddr *addr);
/**
 * @brief 返回具有令牌
 * @a token的观察者。
 *
 * @param observers 观察者数组的指针
 * @param len 观察者数组的大小
 * @param token 令牌的指针
 * @param token_len 令牌中有效字节的长度
 *
 * @note 如果已知观察者的地址和令牌，则应优先使用coap_find_observer()函数。
 *
 * @return 如果找到匹配的观察者，则返回观察者的指针，否则返回NULL。
 */
struct coap_observer *coap_find_observer_by_token(
	struct coap_observer *observers, size_t len,
	const uint8_t *token, uint8_t token_len);
/**
 * @brief 返回下一个可用的观察者表示。
 *
 * @param observers 观察者数组的指针
 * @param len 观察者数组的大小
 *
 * @return 如果有可用的观察者，则返回观察者的指针，否则返回NULL。
 */
struct coap_observer *coap_observer_next_unused(
	struct coap_observer *observers, size_t len);
/**
 * @brief 表示预期@a request的回复。
 *
 * @param reply 要初始化的回复结构
 * @param request 回复将基于的请求
 */
void coap_reply_init(struct coap_reply *reply,
		     const struct coap_packet *request);
/**
 * @brief 使用请求初始化挂起请求。
 *
 * 请求的字段被复制到挂起结构中，因此@a request不必与挂起结构一样长时间存在，但“data”需要至少存在那么长时间。
 *
 * @param pending 表示等待确认消息的结构，使用@a request的数据初始化
 * @param request 等待确认的消息
 * @param addr 发送重传的地址
 * @param params 指向CoAP传输参数结构的指针，或NULL以使用默认值
 *
 * @return 成功返回0，错误返回负值。
 */
int coap_pending_init(struct coap_pending *pending,
		      const struct coap_packet *request,
		      const struct sockaddr *addr,
		      const struct coap_transmission_parameters *params);
/**
 * @brief 返回下一个可用的挂起结构，可用于跟踪请求的重传状态。
 *
 * @param pendings #coap_pending结构数组的指针
 * @param len #coap_pending结构数组的大小
 *
 * @return 指向空闲#coap_pending结构的指针，如果找不到则返回NULL。
 */
struct coap_pending *coap_pending_next_unused(
	struct coap_pending *pendings, size_t len);
/**
 * @brief 返回下一个可用的回复结构，以便可以用于跟踪接收到的回复和通知。
 *
 * @param replies #coap_reply结构数组的指针
 * @param len #coap_reply结构数组的大小
 *
 * @return 指向空闲#coap_reply结构的指针，如果找不到则返回NULL。
 */
struct coap_reply *coap_reply_next_unused(
	struct coap_reply *replies, size_t len);
/**
 * @brief 接收到响应后，返回是否存在任何匹配的挂起请求。用户必须通过调用coap_pending_clear()清除与该响应相关的所有挂起重传。
 *
 * @param response 接收到的响应
 * @param pendings #coap_reply结构数组的指针
 * @param len #coap_reply结构数组的大小
 *
 * @return 指向关联的#coap_pending结构的指针，如果找不到则返回NULL。
 */
struct coap_pending *coap_pending_received(
	const struct coap_packet *response,
	struct coap_pending *pendings, size_t len);
/**
 * @brief 接收到响应后，调用在#coap_reply结构中注册的coap_reply_t处理程序
 *
 * @param response 接收到的响应
 * @param from 接收到响应的地址
 * @param replies #coap_reply结构数组的指针
 * @param len #coap_reply结构数组的大小
 *
 * @return 指向匹配接收到的数据包的回复的指针，如果找不到则返回NULL。
 */
struct coap_reply *coap_response_received(
	const struct coap_packet *response,
	const struct sockaddr *from,
	struct coap_reply *replies, size_t len);
/**
 * @brief 返回即将到期的下一个挂起请求，pending->timeout指示到下一个到期的毫秒数。
 *
 * @param pendings #coap_pending结构数组的指针
 * @param len #coap_pending结构数组的大小
 *
 * @return 即将到期的#coap_pending，如果没有即将到期的则返回NULL。
 */
struct coap_pending *coap_pending_next_to_expire(
	struct coap_pending *pendings, size_t len);
/**
 * @brief 发送请求后，用户可能希望循环挂起重传，以便更新超时。
 *
 * @param pending 要更新超时的挂起表示
 *
 * @return 如果这是最后一次重传，则返回false。
 */
bool coap_pending_cycle(struct coap_pending *pending);
/**
 * @brief 取消挂起重传，使其再次可用。
 *
 * @param pending 要取消的挂起表示
 */
void coap_pending_clear(struct coap_pending *pending);
/**
 * @brief 取消所有挂起重传，使其再次可用。
 *
 * @param pendings #coap_pending结构数组的指针
 * @param len #coap_pending结构数组的大小
 */
void coap_pendings_clear(struct coap_pending *pendings, size_t len);
/**
 * @brief 计算挂起请求的数量。
 *
 * @param len 数组中的元素数量。
 * @param pendings 挂起请求数组。
 * @return 超时不为零的元素计数。
 */
size_t coap_pendings_count(struct coap_pending *pendings, size_t len);
/**
 * @brief 取消等待此回复，使其再次可用。用户有责任释放与数据关联的内存。
 *
 * @param reply 要取消的回复
 */
void coap_reply_clear(struct coap_reply *reply);
/**
 * @brief 取消所有回复，使其再次可用。
 *
 * @param replies #coap_reply结构数组的指针
 * @param len #coap_reply结构数组的大小
 */
void coap_replies_clear(struct coap_reply *replies, size_t len);
/**
 * @brief 表示此资源已更新，并且应为每个注册的观察者调用@a notify回调。
 *
 * @param resource 已更新的资源
 *
 * @return 成功返回0，错误返回负值。
 */
int coap_resource_notify(struct coap_resource *resource);
/**
 * @brief 返回此请求是否启用观察资源。
 *
 * @param request 要检查的请求
 *
 * @return 如果请求启用观察资源，则返回true，否则返回false。
 */
bool coap_request_is_observe(const struct coap_packet *request);
/**
 * @brief 获取当前活动的CoAP传输参数。
 *
 * @return CoAP传输参数结构。
 */
struct coap_transmission_parameters coap_get_transmission_parameters(void);
/**
 * @brief 设置CoAP传输参数。
 *
 * @param params 指向传输参数结构的指针。
 */
void coap_set_transmission_parameters(const struct coap_transmission_parameters *params);
#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif /* ZEPHYR_INCLUDE_NET_COAP_H_ */
// By GST @Data