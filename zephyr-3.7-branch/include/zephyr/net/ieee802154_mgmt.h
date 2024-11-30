// zephyr-3.7-branch/include/zephyr/net/ieee802154_mgmt.h

/**
 * @file
 * @brief IEEE 802.15.4管理接口公共头文件
 *
 * @note 本文件中的所有标准引用均指IEEE 802.15.4-2020。
 */

#ifndef ZEPHYR_INCLUDE_NET_IEEE802154_MGMT_H_
#define ZEPHYR_INCLUDE_NET_IEEE802154_MGMT_H_

#include <zephyr/net/ieee802154.h>
#include <zephyr/net/net_mgmt.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup ieee802154_mgmt IEEE 802.15.4网络管理
 * @since 1.0
 * @version 0.8.0
 * @ingroup ieee802154
 *
 * @brief IEEE 802.15.4网络管理库
 *
 * @details IEEE 802.15.4网络管理库提供了应用程序可以直接接口的运行时配置功能。
 *
 * 这些命令中的大多数也可以通过shell命令访问。请参阅shell的帮助功能（`shell> ieee802154 help`）。
 *
 * @note 本文件中的所有章节、表格和图形引用均指IEEE 802.15.4-2020标准。
 *
 * @{
 */

/**
 * @cond INTERNAL_HIDDEN
 */

#define _NET_IEEE802154_LAYER	NET_MGMT_LAYER_L2
#define _NET_IEEE802154_CODE	0x154
#define _NET_IEEE802154_BASE	(NET_MGMT_IFACE_BIT |			\
				 NET_MGMT_LAYER(_NET_IEEE802154_LAYER) |\
				 NET_MGMT_LAYER_CODE(_NET_IEEE802154_CODE))
#define _NET_IEEE802154_EVENT	(_NET_IEEE802154_BASE | NET_MGMT_EVENT_BIT)

enum net_request_ieee802154_cmd {
	NET_REQUEST_IEEE802154_CMD_SET_ACK = 1,
	NET_REQUEST_IEEE802154_CMD_UNSET_ACK,
	NET_REQUEST_IEEE802154_CMD_PASSIVE_SCAN,
	NET_REQUEST_IEEE802154_CMD_ACTIVE_SCAN,
	NET_REQUEST_IEEE802154_CMD_CANCEL_SCAN,
	NET_REQUEST_IEEE802154_CMD_ASSOCIATE,
	NET_REQUEST_IEEE802154_CMD_DISASSOCIATE,
	NET_REQUEST_IEEE802154_CMD_SET_CHANNEL,
	NET_REQUEST_IEEE802154_CMD_GET_CHANNEL,
	NET_REQUEST_IEEE802154_CMD_SET_PAN_ID,
	NET_REQUEST_IEEE802154_CMD_GET_PAN_ID,
	NET_REQUEST_IEEE802154_CMD_SET_EXT_ADDR,
	NET_REQUEST_IEEE802154_CMD_GET_EXT_ADDR,
	NET_REQUEST_IEEE802154_CMD_SET_SHORT_ADDR,
	NET_REQUEST_IEEE802154_CMD_GET_SHORT_ADDR,
	NET_REQUEST_IEEE802154_CMD_GET_TX_POWER,
	NET_REQUEST_IEEE802154_CMD_SET_TX_POWER,
	NET_REQUEST_IEEE802154_CMD_SET_SECURITY_SETTINGS,
	NET_REQUEST_IEEE802154_CMD_GET_SECURITY_SETTINGS,
};

/**
 * INTERNAL_HIDDEN @endcond
 */

/**
 * @name 命令宏
 *
 * @brief IEEE 802.15.4网络管理命令。
 *
 * @details 这些IEEE 802.15.4子系统网络管理命令可以通过@ref net_mgmt宏由应用程序调用。
 *
 * 除非另有说明，所有属性和参数均以CPU字节顺序（标量）或大端序（字节数组）给出。
 *
 * 本枚举中引用了以下IEEE 802.15.4 MAC管理服务原语：
 *  - MLME-ASSOCIATE.request，参见第8.2.3节
 *  - MLME-DISASSOCIATE.request，参见第8.2.4节
 *  - MLME-SET/GET.request，参见第8.2.6节
 *  - MLME-SCAN.request，参见第8.2.11节
 *
 * 本枚举中引用了以下IEEE 802.15.4 MAC数据服务原语：
 *  - MLME-DATA.request，参见第8.3.2节
 *
 * MAC PIB属性（mac.../sec...）：参见第8.4.3和9.5节。
 * PHY PIB属性（phy...）：参见第11.3节。
 * 通过MLME-SET/GET原语访问这两者。
 *
 * @{
 */

/** 为所有后续的MLME-DATA（即TX）请求设置AckTx。 */
#define NET_REQUEST_IEEE802154_SET_ACK (_NET_IEEE802154_BASE | NET_REQUEST_IEEE802154_CMD_SET_ACK)

NET_MGMT_DEFINE_REQUEST_HANDLER(NET_REQUEST_IEEE802154_SET_ACK);

/** 为所有后续的MLME-DATA请求取消设置AckTx。 */
#define NET_REQUEST_IEEE802154_UNSET_ACK                                                           \
	(_NET_IEEE802154_BASE | NET_REQUEST_IEEE802154_CMD_UNSET_ACK)

NET_MGMT_DEFINE_REQUEST_HANDLER(NET_REQUEST_IEEE802154_UNSET_ACK);

/**
 * MLME-SCAN（被动，...）请求
 *
 * 参见@ref ieee802154_req_params以获取相关命令参数。
 */
#define NET_REQUEST_IEEE802154_PASSIVE_SCAN                                                        \
	(_NET_IEEE802154_BASE | NET_REQUEST_IEEE802154_CMD_PASSIVE_SCAN)

NET_MGMT_DEFINE_REQUEST_HANDLER(NET_REQUEST_IEEE802154_PASSIVE_SCAN);

/**
 * MLME-SCAN（主动，...）请求
 *
 * 参见@ref ieee802154_req_params以获取相关命令参数。
 */
#define NET_REQUEST_IEEE802154_ACTIVE_SCAN                                                         \
	(_NET_IEEE802154_BASE | NET_REQUEST_IEEE802154_CMD_ACTIVE_SCAN)

NET_MGMT_DEFINE_REQUEST_HANDLER(NET_REQUEST_IEEE802154_ACTIVE_SCAN);

/** 取消正在进行的MLME-SCAN（...）命令（非标准）。 */
#define NET_REQUEST_IEEE802154_CANCEL_SCAN                                                         \
	(_NET_IEEE802154_BASE | NET_REQUEST_IEEE802154_CMD_CANCEL_SCAN)

NET_MGMT_DEFINE_REQUEST_HANDLER(NET_REQUEST_IEEE802154_CANCEL_SCAN);

/** MLME-ASSOCIATE（...）请求 */
#define NET_REQUEST_IEEE802154_ASSOCIATE                                                           \
	(_NET_IEEE802154_BASE | NET_REQUEST_IEEE802154_CMD_ASSOCIATE)

NET_MGMT_DEFINE_REQUEST_HANDLER(NET_REQUEST_IEEE802154_ASSOCIATE);

/** MLME-DISASSOCIATE（...）请求 */
#define NET_REQUEST_IEEE802154_DISASSOCIATE                                                        \
	(_NET_IEEE802154_BASE | NET_REQUEST_IEEE802154_CMD_DISASSOCIATE)

NET_MGMT_DEFINE_REQUEST_HANDLER(NET_REQUEST_IEEE802154_DISASSOCIATE);

/** MLME-SET（phyCurrentChannel）请求 */
#define NET_REQUEST_IEEE802154_SET_CHANNEL                                                         \
	(_NET_IEEE802154_BASE | NET_REQUEST_IEEE802154_CMD_SET_CHANNEL)

NET_MGMT_DEFINE_REQUEST_HANDLER(NET_REQUEST_IEEE802154_SET_CHANNEL);

/** MLME-GET（phyCurrentChannel）请求 */
#define NET_REQUEST_IEEE802154_GET_CHANNEL                                                         \
	(_NET_IEEE802154_BASE | NET_REQUEST_IEEE802154_CMD_GET_CHANNEL)

NET_MGMT_DEFINE_REQUEST_HANDLER(NET_REQUEST_IEEE802154_GET_CHANNEL);

/** MLME-SET（macPanId）请求 */
#define NET_REQUEST_IEEE802154_SET_PAN_ID                                                          \
	(_NET_IEEE802154_BASE | NET_REQUEST_IEEE802154_CMD_SET_PAN_ID)

NET_MGMT_DEFINE_REQUEST_HANDLER(NET_REQUEST_IEEE802154_SET_PAN_ID);

/** MLME-GET（macPanId）请求 */
#define NET_REQUEST_IEEE802154_GET_PAN_ID                                                          \
	(_NET_IEEE802154_BASE | NET_REQUEST_IEEE802154_CMD_GET_PAN_ID)

NET_MGMT_DEFINE_REQUEST_HANDLER(NET_REQUEST_IEEE802154_GET_PAN_ID);

/**
 * 设置扩展接口地址（非标准），参见第7.1和8.4.3.1节，以大端序字节顺序
 */
#define NET_REQUEST_IEEE802154_SET_EXT_ADDR                                                        \
	(_NET_IEEE802154_BASE | NET_REQUEST_IEEE802154_CMD_SET_EXT_ADDR)

NET_MGMT_DEFINE_REQUEST_HANDLER(NET_REQUEST_IEEE802154_SET_EXT_ADDR);

/** 类似于MLME-GET（macExtendedAddress），但以大端序字节顺序 */
#define NET_REQUEST_IEEE802154_GET_EXT_ADDR                                                        \
	(_NET_IEEE802154_BASE | NET_REQUEST_IEEE802154_CMD_GET_EXT_ADDR)

NET_MGMT_DEFINE_REQUEST_HANDLER(NET_REQUEST_IEEE802154_GET_EXT_ADDR);

/** MLME-SET（macShortAddress）请求，仅允许协调器使用 */
#define NET_REQUEST_IEEE802154_SET_SHORT_ADDR                                                      \
	(_NET_IEEE802154_BASE | NET_REQUEST_IEEE802154_CMD_SET_SHORT_ADDR)

NET_MGMT_DEFINE_REQUEST_HANDLER(NET_REQUEST_IEEE802154_SET_SHORT_ADDR);

/** MLME-GET（macShortAddress）请求 */
#define NET_REQUEST_IEEE802154_GET_SHORT_ADDR                                                      \
	(_NET_IEEE802154_BASE | NET_REQUEST_IEEE802154_CMD_GET_SHORT_ADDR)

NET_MGMT_DEFINE_REQUEST_HANDLER(NET_REQUEST_IEEE802154_GET_SHORT_ADDR);

/**
 * MLME-SET（phyUnicastTxPower/phyBroadcastTxPower）请求（目前未区分）
 */
#define NET_REQUEST_IEEE802154_GET_TX_POWER                                                        \
	(_NET_IEEE802154_BASE | NET_REQUEST_IEEE802154_CMD_GET_TX_POWER)

NET_MGMT_DEFINE_REQUEST_HANDLER(NET_REQUEST_IEEE802154_GET_TX_POWER);

/** MLME-GET（phyUnicastTxPower/phyBroadcastTxPower）请求 */
#define NET_REQUEST_IEEE802154_SET_TX_POWER                                                        \
	(_NET_IEEE802154_BASE | NET_REQUEST_IEEE802154_CMD_SET_TX_POWER)

NET_MGMT_DEFINE_REQUEST_HANDLER(NET_REQUEST_IEEE802154_SET_TX_POWER);

#ifdef CONFIG_NET_L2_IEEE802154_SECURITY

/**
 * 配置基本的sec* MAC PIB属性，意味着macSecurityEnabled=true。
 *
 * 参见@ref ieee802154_security_params以获取相关命令参数。
 */
#define NET_REQUEST_IEEE802154_SET_SECURITY_SETTINGS                                               \
	(_NET_IEEE802154_BASE | NET_REQUEST_IEEE802154_CMD_SET_SECURITY_SETTINGS)

NET_MGMT_DEFINE_REQUEST_HANDLER(NET_REQUEST_IEEE802154_SET_SECURITY_SETTINGS);

/**
 * 获取配置的sec*属性。
 *
 * 参见@ref ieee802154_security_params以获取相关命令参数。
 */
#define NET_REQUEST_IEEE802154_GET_SECURITY_SETTINGS                                               \
	(_NET_IEEE802154_BASE | NET_REQUEST_IEEE802154_CMD_GET_SECURITY_SETTINGS)

NET_MGMT_DEFINE_REQUEST_HANDLER(NET_REQUEST_IEEE802154_GET_SECURITY_SETTINGS);

#endif /* CONFIG_NET_L2_IEEE802154_SECURITY */

/**
 * @}
 */

/**
 * @cond INTERNAL_HIDDEN
 */

enum net_event_ieee802154_cmd {
	NET_EVENT_IEEE802154_CMD_SCAN_RESULT = 1,
};

/**
 * INTERNAL_HIDDEN @endcond
 */

/**
 * @name 事件宏
 *
 * @brief IEEE 802.15.4网络管理事件。
 *
 * @details 这些IEEE 802.15.4子系统网络管理事件可以通过@ref net_mgmt_init_event_callback、@ref net_mgmt_add_event_callback和@ref net_mgmt_del_event_callback由应用程序订阅。
 *
 * @{
 */

/**
 * 表示@ref NET_REQUEST_IEEE802154_ACTIVE_SCAN或@ref NET_REQUEST_IEEE802154_PASSIVE_SCAN网络管理命令的结果。
 *
 * 参见@ref ieee802154_req_params以获取相关事件参数。
 */
#define NET_EVENT_IEEE802154_SCAN_RESULT				\
	(_NET_IEEE802154_EVENT | NET_EVENT_IEEE802154_CMD_SCAN_RESULT)

/**
 * @}
 */

/**
 * @cond INTERNAL_HIDDEN
 */

#define IEEE802154_IS_CHAN_SCANNED(_channel_set, _chan)	\
	(_channel_set & BIT(_chan - 1))
#define IEEE802154_IS_CHAN_UNSCANNED(_channel_set, _chan)	\
	(!IEEE802154_IS_CHAN_SCANNED(_channel_set, _chan))

#define IEEE802154_ALL_CHANNELS	UINT32_MAX

/**
 * INTERNAL_HIDDEN @endcond
 */

/**
 * @brief 扫描参数
 *
 * 用于请求扫描并获取结果，参见第8.2.11.2节
 */
struct ieee802154_req_params {
	/** 要扫描的频道集，使用上述宏进行管理 */
	uint32_t channel_set;

	/** 扫描持续时间，每个频道，以毫秒为单位 */
	uint32_t duration;

	/** 结果中使用的当前频道 */
	uint16_t channel; /* 以CPU字节顺序 */
	/** 结果中使用的当前pan_id */
	uint16_t pan_id; /* 以CPU字节顺序 */

	/** 结果地址 */
	union {
		uint16_t short_addr;			  /**< 以CPU字节顺序 */
		uint8_t addr[IEEE802154_MAX_ADDR_LENGTH]; /**< 以大端序 */
	};

	/** 地址长度 */
	uint8_t len;
	/** 链路质量信息，介于0和255之间 */
	uint8_t lqi;
};

/**
 * @brief 安全参数
 *
 * 用于设置链路层安全设置，
 * 参见第9.5节中的表9-9和9-10。
 */
struct ieee802154_security_params {
	uint8_t key[16];      /**< secKeyDescriptor.secKey */
	uint8_t key_len;      /**< 16字节的密钥长度是标准合规性的强制要求 */
	uint8_t key_mode : 2; /**< secKeyIdMode */
	uint8_t level : 3;    /**< 构建辅助安全头时使用，而不是帧特定的SecurityLevel参数 */
	uint8_t _unused : 3;  /**< 未使用的值（忽略） */
};

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* ZEPHYR_INCLUDE_NET_IEEE802154_MGMT_H_ */

// By GST @Data