// zephyr-3.7-branch/include/zephyr/net/lldp.h

#ifndef ZEPHYR_INCLUDE_NET_LLDP_H_
#define ZEPHYR_INCLUDE_NET_LLDP_H_

/**
 * @brief LLDP定义和助手
 * @defgroup lldp 链路层发现协议定义和助手
 * @ingroup networking
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @cond INTERNAL_HIDDEN */

#define LLDP_TLV_GET_LENGTH(type_length)	(type_length & BIT_MASK(9))
#define LLDP_TLV_GET_TYPE(type_length)		((uint8_t)(type_length >> 9))

/* LLDP定义 */

/* 根据规范，LLDPDU结束TLV值是常量。 */
#define NET_LLDP_END_LLDPDU_VALUE 0x0000

/*
 * 对于机箱ID TLV值，如果子类型是MAC地址，则必须使用CONFIG_NET_LLDP_CHASSIS_ID_MAC0到CONFIG_NET_LLDP_CHASSIS_ID_MAC5的值。
 * 如果不是，则使用CONFIG_NET_LLDP_CHASSIS_ID。
 *
 * FIXME: 为子类型5（网络地址）实现类似的方案。
 */
#if defined(CONFIG_NET_LLDP_CHASSIS_ID_SUBTYPE)
#if (CONFIG_NET_LLDP_CHASSIS_ID_SUBTYPE == 4)
#define NET_LLDP_CHASSIS_ID_VALUE		\
	{					\
	  CONFIG_NET_LLDP_CHASSIS_ID_MAC0,	\
	  CONFIG_NET_LLDP_CHASSIS_ID_MAC1,	\
	  CONFIG_NET_LLDP_CHASSIS_ID_MAC2,	\
	  CONFIG_NET_LLDP_CHASSIS_ID_MAC3,	\
	  CONFIG_NET_LLDP_CHASSIS_ID_MAC4,	\
	  CONFIG_NET_LLDP_CHASSIS_ID_MAC5 	\
	}

#define NET_LLDP_CHASSIS_ID_VALUE_LEN (6)
#else
#define NET_LLDP_CHASSIS_ID_VALUE CONFIG_NET_LLDP_CHASSIS_ID
#define NET_LLDP_CHASSIS_ID_VALUE_LEN (sizeof(CONFIG_NET_LLDP_CHASSIS_ID) - 1)
#endif
#else
#define NET_LLDP_CHASSIS_ID_VALUE 0
#define NET_LLDP_CHASSIS_ID_VALUE_LEN 0
#endif

/*
 * 对于端口ID TLV值，如果子类型是MAC地址，则必须使用CONFIG_NET_LLDP_PORT_ID_MAC0到CONFIG_NET_LLDP_PORT_ID_MAC5的值。
 * 如果不是，则使用CONFIG_NET_LLDP_PORT_ID。
 *
 * FIXME: 为子类型4（网络地址）实现类似的方案。
 */
#if defined(CONFIG_NET_LLDP_PORT_ID_SUBTYPE)
#if (CONFIG_NET_LLDP_PORT_ID_SUBTYPE == 3)
#define NET_LLDP_PORT_ID_VALUE		\
	{				\
	  CONFIG_NET_LLDP_PORT_ID_MAC0,	\
	  CONFIG_NET_LLDP_PORT_ID_MAC1, \
	  CONFIG_NET_LLDP_PORT_ID_MAC2, \
	  CONFIG_NET_LLDP_PORT_ID_MAC3, \
	  CONFIG_NET_LLDP_PORT_ID_MAC4, \
	  CONFIG_NET_LLDP_PORT_ID_MAC5  \
	}

#define NET_LLDP_PORT_ID_VALUE_LEN (6)
#else
#define NET_LLDP_PORT_ID_VALUE CONFIG_NET_LLDP_PORT_ID
#define NET_LLDP_PORT_ID_VALUE_LEN (sizeof(CONFIG_NET_LLDP_PORT_ID) - 1)
#endif
#else
#define NET_LLDP_PORT_ID_VALUE 0
#define NET_LLDP_PORT_ID_VALUE_LEN 0
#endif

/*
 * TLV长度。
 * 请注意，具有子类型的TLV必须在其长度上加一个字节。
 */
#define NET_LLDP_CHASSIS_ID_TLV_LEN (NET_LLDP_CHASSIS_ID_VALUE_LEN + 1)
#define NET_LLDP_PORT_ID_TLV_LEN (NET_LLDP_PORT_ID_VALUE_LEN + 1)
#define NET_LLDP_TTL_TLV_LEN (2)

/*
 * 生存时间值。
 * 根据LLDP规范第9.2.5.22节计算。
 *
 * FIXME: 当网络接口即将“禁用”时，TTL应设置为零，以便LLDP Rx代理可以使与此节点相关的条目无效。
 */
#if defined(CONFIG_NET_LLDP_TX_INTERVAL) && defined(CONFIG_NET_LLDP_TX_HOLD)
#define NET_LLDP_TTL \
	MIN((CONFIG_NET_LLDP_TX_INTERVAL * CONFIG_NET_LLDP_TX_HOLD) + 1, 65535)
#endif

struct net_if;

/** @endcond */

/** TLV类型。请参阅IEEE 802.1AB标准的表8-1。 */
enum net_lldp_tlv_type {
	LLDP_TLV_END_LLDPDU          = 0, /**< LLDPDU结束（可选） */
	LLDP_TLV_CHASSIS_ID          = 1, /**< 机箱ID（必选） */
	LLDP_TLV_PORT_ID             = 2, /**< 端口ID（必选） */
	LLDP_TLV_TTL                 = 3, /**< 生存时间（必选） */
	LLDP_TLV_PORT_DESC           = 4, /**< 端口描述（可选） */
	LLDP_TLV_SYSTEM_NAME         = 5, /**< 系统名称（可选） */
	LLDP_TLV_SYSTEM_DESC         = 6, /**< 系统描述（可选） */
	LLDP_TLV_SYSTEM_CAPABILITIES = 7, /**< 系统能力（可选） */
	LLDP_TLV_MANAGEMENT_ADDR     = 8, /**< 管理地址（可选） */
	/* 类型9 - 126保留。 */
	LLDP_TLV_ORG_SPECIFIC       = 127, /**< 组织特定TLV（可选） */
};

/** 机箱ID TLV，参见IEEE 802.1AB第8.5.2章 */
struct net_lldp_chassis_tlv {
	/** 7位类型，9位长度 */
	uint16_t type_length;
	/** ID子类型 */
	uint8_t subtype;
	/** 机箱ID值 */
	uint8_t value[NET_LLDP_CHASSIS_ID_VALUE_LEN];
} __packed;

/** 端口ID TLV，参见IEEE 802.1AB第8.5.3章 */
struct net_lldp_port_tlv {
	/** 7位类型，9位长度 */
	uint16_t type_length;
	/** ID子类型 */
	uint8_t subtype;
	/** 端口ID值 */
	uint8_t value[NET_LLDP_PORT_ID_VALUE_LEN];
} __packed;

/** 生存时间TLV，参见IEEE 802.1AB第8.5.4章 */
struct net_lldp_time_to_live_tlv {
	/** 7位类型，9位长度 */
	uint16_t type_length;
	/** 生存时间（TTL）值 */
	uint16_t ttl;
} __packed;

/**
 * LLDP数据单元（LLDPDU）应包含以下有序TLV，
 * 如IEEE 802.1AB的“8.2 LLDPDU格式”中所述
 */
struct net_lldpdu {
	struct net_lldp_chassis_tlv chassis_id;	/**< 必选机箱TLV */
	struct net_lldp_port_tlv port_id;	/**< 必选端口TLV */
	struct net_lldp_time_to_live_tlv ttl;	/**< 必选TTL TLV */
} __packed;

/**
 * @brief 为网络接口设置LLDP数据单元。
 *
 * @param iface 网络接口
 * @param lldpdu LLDP数据单元结构
 *
 * @return 如果成功则返回0，否则返回<0
 */
int net_lldp_config(struct net_if *iface, const struct net_lldpdu *lldpdu);

/**
 * @brief 为网络接口设置可选LLDP TLV。
 *
 * @param iface 网络接口
 * @param tlv 跟随必选部分的LLDP可选TLV
 * @param len 可选TLV的长度
 *
 * @return 如果成功则返回0，否则返回<0
 */
int net_lldp_config_optional(struct net_if *iface, const uint8_t *tlv,
			     size_t len);

/**
 * @brief 初始化LLDP引擎。
 */
void net_lldp_init(void);

/**
 * @brief LLDP接收数据包回调
 *
 * 回调在接收到数据包时调用。它负责释放数据包或指示堆栈需要释放数据包
 * 通过返回正确的net_verdict。
 *
 * 返回：
 *  - NET_DROP，如果数据包无效、被拒绝或我们希望堆栈释放它。
 *    在这种情况下，核心堆栈将释放数据包。
 *  - NET_OK，如果数据包被接受，在这种情况下，net_pkt的所有权将转移到回调，核心网络堆栈将忘记它。
 */
typedef enum net_verdict (*net_lldp_recv_cb_t)(struct net_if *iface,
					       struct net_pkt *pkt);

/**
 * @brief 注册LLDP Rx回调函数
 *
 * @param iface 网络接口
 * @param cb 回调函数
 *
 * @return 如果成功则返回0，否则返回<0
 */
int net_lldp_register_callback(struct net_if *iface, net_lldp_recv_cb_t cb);

/**
 * @brief 解析LLDP数据包
 *
 * @param iface 网络接口
 * @param pkt 网络数据包
 *
 * @return 返回网络缓冲区的策略
 */
enum net_verdict net_lldp_recv(struct net_if *iface, struct net_pkt *pkt);

/**
 * @brief 为网络接口设置LLDP协议数据单元（LLDPDU）。
 *
 * @param iface 网络接口
 *
 * @return 如果错误则返回<0，如果在lldp数组中找到iface则返回索引
 */
#if defined(CONFIG_NET_LLDP)
int net_lldp_set_lldpdu(struct net_if *iface);
#else
#define net_lldp_set_lldpdu(iface)
#endif

/**
 * @brief 为网络接口取消设置LLDP协议数据单元（LLDPDU）。
 *
 * @param iface 网络接口
 */
#if defined(CONFIG_NET_LLDP)
void net_lldp_unset_lldpdu(struct net_if *iface);
#else
#define net_lldp_unset_lldpdu(iface)
#endif

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* ZEPHYR_INCLUDE_NET_LLDP_H_ */

// By GST @Data