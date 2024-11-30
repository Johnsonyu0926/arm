// zephyr-3.7-branch/include/zephyr/net/ieee802154_radio_openthread.h

/**
 * @file
 * @brief 扩展ieee802154_radio.h接口以支持OpenThread。
 */

#ifndef ZEPHYR_INCLUDE_NET_IEEE802154_RADIO_OPENTHREAD_H_
#define ZEPHYR_INCLUDE_NET_IEEE802154_RADIO_OPENTHREAD_H_

#include <zephyr/net/ieee802154_radio.h>

/**
 *  OpenThread特定的ieee802154驱动程序功能。
 *  此类型扩展了@ref ieee802154_hw_caps。
 */
enum ieee802154_openthread_hw_caps {
	/** 支持使用@ref IEEE802154_OPENTHREAD_TX_MODE_TXTIME_MULTIPLE_CCA模式进行传输的功能。 */
	IEEE802154_OPENTHREAD_HW_MULTIPLE_CCA = BIT(IEEE802154_HW_CAPS_BITS_PRIV_START),
};

/** @brief 传输模式 */
enum ieee802154_openthread_tx_mode {
	/**
	 * @ref IEEE802154_OPENTHREAD_TX_MODE_TXTIME_MULTIPLE_CCA模式允许在最多1 + max_extra_cca_attempts次连续CCA报告空闲后发送计划的数据包。
	 *
	 * 该模式是一个非标准的实验性OpenThread功能。它允许在特定时间窗口内传输数据包。
	 * 最早的传输时间与其他TXTIME模式相同：当第一个CCA报告空闲信道时，数据包的PHR的第一个符号应在计划的TX时间戳（以下简称T_tx）表示的时间出现在本地天线上。
	 *
	 * 如果第一个CCA报告信道忙，则将进行最多max_extra_cca_attempts次额外的CCA，直到其中一个报告空闲信道并发送数据包，或者达到最大尝试次数，此时传输失败。
	 *
	 * 这些额外CCA的时间取决于驱动程序的能力，驱动程序在T_recca和T_ccatx驱动程序属性中报告这些能力（参见@ref IEEE802154_OPENTHREAD_ATTR_T_RECCA和@ref IEEE802154_OPENTHREAD_ATTR_T_CCATX）。
	 * 基于这些属性，上层可以计算计划数据包的PHR的第一个符号应在本地天线上出现的最晚时间点（T_txmax）：
	 *
	 * T_maxtxdelay = max_extra_cca_attempts * (aCcaTime + T_recca) - T_recca + T_ccatx
	 * T_txmax = T_tx + T_maxtxdelay
	 *
	 * 参见IEEE 802.15.4-2020，第11.3节，表11-1，了解aCcaTime的定义。
	 *
	 * 实现此传输模式的驱动程序应尽可能缩短T_recca和T_ccatx。T_ccatx应小于或等于aTurnaroundTime，定义见前述标准，第11.3节，表11-1。
	 *
	 * CCA应按phyCcaMode PHY PIB属性定义执行（参见前述标准，第11.3节，表11-2）。
	 *
	 * 需要IEEE802154_OPENTHREAD_HW_MULTIPLE_CCA功能。
	 */
	IEEE802154_OPENTHREAD_TX_MODE_TXTIME_MULTIPLE_CCA = IEEE802154_TX_MODE_PRIV_START
};

/**
 *  OpenThread特定的ieee802154驱动程序配置类型。
 *  此类型扩展了@ref ieee802154_config_type。
 */
enum ieee802154_openthread_config_type {
	/** 允许配置请求使用@ref IEEE802154_OPENTHREAD_TX_MODE_TXTIME_MULTIPLE_CCA模式传输的额外CCA。
	 *  需要IEEE802154_OPENTHREAD_HW_MULTIPLE_CCA功能。
	 */
	IEEE802154_OPENTHREAD_CONFIG_MAX_EXTRA_CCA_ATTEMPTS  = IEEE802154_CONFIG_PRIV_START
};

/**
 * 供应商特定头部或嵌套信息元素的Thread供应商OUI，
 * 参见IEEE 802.15.4-2020，第7.4.2.2节和第7.4.4.30节。
 *
 * 小端序
 */
#define IEEE802154_OPENTHREAD_THREAD_IE_VENDOR_OUI { 0x9b, 0xb8, 0xea }

/** IEEE 802.15.4-2020供应商OUI的长度 */
#define IEEE802154_OPENTHREAD_VENDOR_OUI_LEN 3

/** OpenThread特定的ieee802154驱动程序配置数据。 */
struct ieee802154_openthread_config {
	union {
		/** 通用配置 */
		struct ieee802154_config common;

		/** ``IEEE802154_OPENTHREAD_CONFIG_MAX_EXTRA_CCA_ATTEMPTS``
		 *
		 *  请求使用@ref IEEE802154_OPENTHREAD_TX_MODE_TXTIME_MULTIPLE_CCA模式传输时要执行的最大额外CCA次数。
		 */
		uint8_t max_extra_cca_attempts;
	};
};

/**
 *  OpenThread特定的ieee802154驱动程序属性。
 *  此类型扩展了@ref ieee802154_attr
 */
enum ieee802154_openthread_attr {

	/** 属性：连续CCA之间的最大时间。
	 *
	 *  这是@ref IEEE802154_OPENTHREAD_TX_MODE_TXTIME_MULTIPLE_CCA提到的T_recca参数的属性。
	 *  时间以微秒为单位表示。
	 */
	IEEE802154_OPENTHREAD_ATTR_T_RECCA = IEEE802154_ATTR_PRIV_START,

	/** 属性：检测到CCA空闲信道和SHR在本地天线上开始之间的最大时间。
	 *
	 *  这是@ref IEEE802154_OPENTHREAD_TX_MODE_TXTIME_MULTIPLE_CCA提到的T_ccatx参数的属性。
	 *  时间以微秒为单位表示。
	 */
	IEEE802154_OPENTHREAD_ATTR_T_CCATX
};

/**
 *  OpenThread特定的ieee802154驱动程序属性值数据。
 *  此类型扩展了@ref ieee802154_attr_value
 */
struct ieee802154_openthread_attr_value {
	union {
		/** 通用属性值 */
		struct ieee802154_attr_value common;

		/** @brief @ref IEEE802154_OPENTHREAD_ATTR_T_RECCA的属性值 */
		uint16_t t_recca;

		/** @brief @ref IEEE802154_OPENTHREAD_ATTR_T_CCATX的属性值 */
		uint16_t t_ccatx;

	};
};

#endif /* ZEPHYR_INCLUDE_NET_IEEE802154_RADIO_OPENTHREAD_H_ */

// By GST @Data