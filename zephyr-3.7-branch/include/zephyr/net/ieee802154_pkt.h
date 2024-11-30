// zephyr-3.7-branch/include/zephyr/net/ieee802154_pkt.h

/**
 * @file
 * @brief IEEE 802.15.4 L2层通用数据包数据
 *
 * 本文件中的所有规范引用均指IEEE 802.15.4-2020。
 */

#ifndef ZEPHYR_INCLUDE_NET_IEEE802154_PKT_H_
#define ZEPHYR_INCLUDE_NET_IEEE802154_PKT_H_

#include <string.h>
#include <zephyr/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @cond ignore */

#ifndef NET_PKT_HAS_CONTROL_BLOCK
#define NET_PKT_HAS_CONTROL_BLOCK
#endif

/* 参见第6.16.2.8节 - 接收信号强度指示器（RSSI） */
#define IEEE802154_MAC_RSSI_MIN       0U   /* 对应于-174 dBm */
#define IEEE802154_MAC_RSSI_MAX       254U /* 对应于80 dBm */
#define IEEE802154_MAC_RSSI_UNDEFINED 255U /* 我们用来表示未定义的RSSI值 */

#define IEEE802154_MAC_RSSI_DBM_MIN       -174      /* 以dBm为单位 */
#define IEEE802154_MAC_RSSI_DBM_MAX       80        /* 以dBm为单位 */
#define IEEE802154_MAC_RSSI_DBM_UNDEFINED INT16_MIN /* 表示未定义的RSSI值 */

struct net_pkt_cb_ieee802154 {
#if defined(CONFIG_NET_L2_OPENTHREAD)
	uint32_t ack_fc;   /* ACK中设置的帧计数器 */
	uint8_t ack_keyid; /* ACK中设置的密钥索引 */
#endif
	union {
		/* 接收数据包 */
		struct {
			uint8_t lqi;  /* 链路质量指示器 */
			/* 参见第6.16.2.8节 - 接收信号强度指示器（RSSI）
			 * "RSSI表示为一个八位整数[...]；因此，最小值和最大值分别为0（-174 dBm）和254（80 dBm），
			 * 255保留。"（MAC PIB属性macRssi，参见第8.4.3.10节，表8-108）
			 *
			 * 发送数据包将显示此值为零。驱动程序可以将该字段设置为保留值255（0xff），以表示该数据包没有可用的RSSI值。
			 */
			uint8_t rssi;
		};
	};

	/* 标志 */
	uint8_t ack_fpb : 1;	   /* ACK中设置的帧挂起位 */
	uint8_t frame_secured : 1; /* 帧根据其辅助安全头进行认证和加密 */
	uint8_t mac_hdr_rdy : 1;   /* 指示帧的MAC头是否准备好传输，或者是否需要进一步修改，例如帧计数器注入。 */
#if defined(CONFIG_NET_L2_OPENTHREAD)
	uint8_t ack_seb : 1; /* ACK中设置的安全启用位 */
#endif
};

struct net_pkt;
static inline void *net_pkt_cb(struct net_pkt *pkt);

static inline struct net_pkt_cb_ieee802154 *net_pkt_cb_ieee802154(struct net_pkt *pkt)
{
	return (struct net_pkt_cb_ieee802154 *)net_pkt_cb(pkt);
}

static inline uint8_t net_pkt_ieee802154_lqi(struct net_pkt *pkt)
{
	return net_pkt_cb_ieee802154(pkt)->lqi;
}

static inline void net_pkt_set_ieee802154_lqi(struct net_pkt *pkt, uint8_t lqi)
{
	net_pkt_cb_ieee802154(pkt)->lqi = lqi;
}

/**
 * @brief 获取无符号RSSI值，如第6.16.2.8节所定义，
 *        接收信号强度指示器（RSSI）
 *
 * @param pkt 指向数据包的指针。
 *
 * @returns RSSI表示为无符号字节值，范围从
 *          0（-174 dBm）到254（80 dBm）。
 *          特殊值255（IEEE802154_MAC_RSSI_UNDEFINED）
 *          表示该数据包没有可用的RSSI值。对于TX路径上的数据包，将返回零。
 */
static inline uint8_t net_pkt_ieee802154_rssi(struct net_pkt *pkt)
{
	return net_pkt_cb_ieee802154(pkt)->rssi;
}

/**
 * @brief 设置无符号RSSI值，如第6.16.2.8节所定义，
 *        接收信号强度指示器（RSSI）。
 *
 * @param pkt 指向接收到给定RSSI的数据包的指针。
 * @param rssi RSSI表示为无符号字节值，范围从
 *             0（-174 dBm）到254（80 dBm）。
 *             特殊值255（IEEE802154_MAC_RSSI_UNDEFINED）
 *             表示该数据包没有可用的RSSI值。
 */
static inline void net_pkt_set_ieee802154_rssi(struct net_pkt *pkt, uint8_t rssi)
{
	net_pkt_cb_ieee802154(pkt)->rssi = rssi;
}

/**
 * @brief 获取以dBm为单位的有符号RSSI值。
 *
 * @param pkt 指向数据包的指针。
 *
 * @returns RSSI表示为dBm。如果该数据包没有可用的RSSI值，则返回特殊值
 *          IEEE802154_MAC_RSSI_DBM_UNDEFINED。TX路径上的数据包将始终显示-174 dBm（对应于无符号零的内部值）。
 */
static inline int16_t net_pkt_ieee802154_rssi_dbm(struct net_pkt *pkt)
{
	int16_t rssi = net_pkt_cb_ieee802154(pkt)->rssi;
	return rssi == IEEE802154_MAC_RSSI_UNDEFINED ? IEEE802154_MAC_RSSI_DBM_UNDEFINED
						     : rssi + IEEE802154_MAC_RSSI_DBM_MIN;
}

/**
 * @brief 设置以dBm为单位的有符号RSSI值。
 *
 * @param pkt 指向接收到给定RSSI的数据包的指针。
 * @param rssi 表示为dBm。设置为特殊值
 *             IEEE802154_MAC_RSSI_DBM_UNDEFINED，如果该数据包没有可用的RSSI值。值高于80 dBm将映射到80 dBm，值低于-174 dBm将映射到-174 dBm。
 */
static inline void net_pkt_set_ieee802154_rssi_dbm(struct net_pkt *pkt, int16_t rssi)
{
	if (likely(rssi >= IEEE802154_MAC_RSSI_DBM_MIN && rssi <= IEEE802154_MAC_RSSI_DBM_MAX)) {
		int16_t unsigned_rssi = rssi - IEEE802154_MAC_RSSI_DBM_MIN;

		net_pkt_cb_ieee802154(pkt)->rssi = unsigned_rssi;
		return;
	} else if (rssi == IEEE802154_MAC_RSSI_DBM_UNDEFINED) {
		net_pkt_cb_ieee802154(pkt)->rssi = IEEE802154_MAC_RSSI_UNDEFINED;
		return;
	} else if (rssi < IEEE802154_MAC_RSSI_DBM_MIN) {
		net_pkt_cb_ieee802154(pkt)->rssi = IEEE802154_MAC_RSSI_MIN;
		return;
	} else if (rssi > IEEE802154_MAC_RSSI_DBM_MAX) {
		net_pkt_cb_ieee802154(pkt)->rssi = IEEE802154_MAC_RSSI_MAX;
		return;
	}

	CODE_UNREACHABLE;
}

static inline bool net_pkt_ieee802154_ack_fpb(struct net_pkt *pkt)
{
	return net_pkt_cb_ieee802154(pkt)->ack_fpb;
}

static inline void net_pkt_set_ieee802154_ack_fpb(struct net_pkt *pkt, bool fpb)
{
	net_pkt_cb_ieee802154(pkt)->ack_fpb = fpb;
}

static inline bool net_pkt_ieee802154_frame_secured(struct net_pkt *pkt)
{
	return net_pkt_cb_ieee802154(pkt)->frame_secured;
}

static inline void net_pkt_set_ieee802154_frame_secured(struct net_pkt *pkt, bool secured)
{
	net_pkt_cb_ieee802154(pkt)->frame_secured = secured;
}

static inline bool net_pkt_ieee802154_mac_hdr_rdy(struct net_pkt *pkt)
{
	return net_pkt_cb_ieee802154(pkt)->mac_hdr_rdy;
}

static inline void net_pkt_set_ieee802154_mac_hdr_rdy(struct net_pkt *pkt, bool rdy)
{
	net_pkt_cb_ieee802154(pkt)->mac_hdr_rdy = rdy;
}

#if defined(CONFIG_NET_L2_OPENTHREAD)
static inline uint32_t net_pkt_ieee802154_ack_fc(struct net_pkt *pkt)
{
	return net_pkt_cb_ieee802154(pkt)->ack_fc;
}

static inline void net_pkt_set_ieee802154_ack_fc(struct net_pkt *pkt, uint32_t fc)
{
	net_pkt_cb_ieee802154(pkt)->ack_fc = fc;
}

static inline uint8_t net_pkt_ieee802154_ack_keyid(struct net_pkt *pkt)
{
	return net_pkt_cb_ieee802154(pkt)->ack_keyid;
}

static inline void net_pkt_set_ieee802154_ack_keyid(struct net_pkt *pkt, uint8_t keyid)
{
	net_pkt_cb_ieee802154(pkt)->ack_keyid = keyid;
}

static inline bool net_pkt_ieee802154_ack_seb(struct net_pkt *pkt)
{
	return net_pkt_cb_ieee802154(pkt)->ack_seb;
}

static inline void net_pkt_set_ieee802154_ack_seb(struct net_pkt *pkt, bool seb)
{
	net_pkt_cb_ieee802154(pkt)->ack_seb = seb;
}
#endif /* CONFIG_NET_L2_OPENTHREAD */

/** @endcond */

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_NET_IEEE802154_PKT_H_ */

// By GST @Data