// zephyr-3.7-branch/include/zephyr/net/ieee802154.h

/**
 * @file
 * @brief IEEE 802.15.4原生L2栈公共头文件
 *
 * @note 本文件中的所有标准引用均指IEEE 802.15.4-2020。
 */

#ifndef ZEPHYR_INCLUDE_NET_IEEE802154_H_
#define ZEPHYR_INCLUDE_NET_IEEE802154_H_

#include <limits.h>
#include <zephyr/net/net_l2.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/crypto/cipher.h>
#include <zephyr/net/ieee802154_radio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup ieee802154 IEEE 802.15.4和Thread API
 * @since 1.0
 * @version 0.8.0
 * @ingroup connectivity
 *
 * @brief IEEE 802.15.4原生和OpenThread L2、配置、管理和驱动程序API
 *
 * @details IEEE 802.15.4和Thread子系统包括OpenThread L2子系统、原生IEEE 802.15.4 L2子系统（“软”MAC）、OpenThread和原生L2栈共享的主要供应商和协议无关的驱动程序API（“硬”MAC和PHY），以及多个配置子系统的API（shell、网络管理、Kconfig、设备树等）。
 *
 * **OpenThread子系统API**将外部<a href="https://openthread.io">OpenThread</a>栈集成到Zephyr中。它基于Zephyr的原生IEEE 802.15.4驱动程序API。
 *
 * **原生IEEE 802.15.4子系统API**在不同级别上公开，面向多个受众：
 *  - shell（终端用户、应用程序开发人员）：
 *    - 一组IEEE 802.15.4 shell命令（参见`shell> ieee802154 help`）
 *  - 应用程序API（应用程序开发人员）：
 *    - IPv6、DGRAM和RAW套接字，用于节点之间的实际点对点、多播和广播数据交换，包括连接特定配置（示例即将推出，参见https://github.com/linux-wpan/wpan-tools/tree/master/examples，灵感来自我们的API，因此具有类似的套接字API），
 *    - Kconfig和设备树配置选项（网络配置库扩展、子系统范围的MAC和PHY Kconfig/DT选项、驱动程序/供应商特定的Kconfig/DT选项，注意以IEEE802154/ieee802154开头的选项），
 *    - 网络管理：在MAC（L2）和PHY（L1）级别运行时配置IEEE 802.15.4协议栈（参见@ref ieee802154_mgmt），
 *  - L2集成（子系统贡献者）：
 *    - 参见@ref ieee802154_l2
 *    - 实现Zephyr的内部L2级别套接字和网络上下文抽象（上下文/套接字操作，参见@ref net_l2），
 *    - 接口结构的协议特定扩展（参见@ref net_if）
 *    - 网络数据包结构的协议特定扩展（参见@ref net_pkt），
 *
 *  - OpenThread和原生IEEE 802.15.4共享一个**驱动程序API**（驱动程序维护者/贡献者）：
 *    - 参见@ref ieee802154_driver
 *    - 所有驱动程序都必须实现的基本、主要PHY级别的驱动程序API，
 *    - 多个“硬MAC”（硬件/固件卸载）扩展点，用于协议的性能关键或时间敏感方面
 */

/**
 * @defgroup ieee802154_l2 IEEE 802.15.4 L2
 * @since 1.0
 * @version 0.8.0
 * @ingroup ieee802154
 *
 * @brief IEEE 802.15.4 L2 API
 *
 * @details 此API提供与Zephyr的套接字和网络上下文的集成。**应用程序和驱动程序开发人员不应直接与此API接口。** 它仅对子系统维护者感兴趣。
 *
 * 该API实现并扩展了以下结构：
 *    - 实现Zephyr的内部L2级别套接字和网络上下文抽象（上下文/套接字操作，参见@ref net_l2），
 *    - 接口结构的协议特定扩展（参见@ref net_if）
 *    - 网络数据包结构的协议特定扩展（参见@ref net_pkt），
 *
 * @note 本文件中的所有章节、表格和图形引用均指IEEE 802.15.4-2020标准。
 *
 * @{
 */

/**
 * @brief 表示PHY常量aMaxPhyPacketSize，参见第11.3节。
 *
 * @note 目前仅支持127字节大小的数据包，尽管某些PHY（例如SUN、MSK、LECIM等）支持更大的数据包大小。一旦这些PHY应得到完全支持，需要进行更改。
 */
#define IEEE802154_MAX_PHY_PACKET_SIZE	127

/**
 * @brief 表示帧校验序列长度，参见第7.2.1.1节。
 *
 * @note 目前仅支持2字节FCS，尽管某些PHY（例如SUN、TVWS等）可选支持4字节FCS。一旦这些PHY应得到完全支持，需要进行更改。
 */
#define IEEE802154_FCS_LENGTH		2

/**
 * @brief IEEE 802.15.4“硬件”MTU（不要与L3/IP MTU混淆），即实际可用于下一个更高层的有效负载。
 *
 * @details 这相当于IEEE 802.15.4 MAC帧长度减去校验和字节，这又相当于PHY有效负载即PSDU长度减去校验和字节。此定义存在于与Linux和Zephyr的L3相同的概念兼容。它不是IEEE 802.15.4标准中的概念。
 *
 * @note 目前仅支持2006标准版本及更早版本的原始帧大小。2015+标准引入了具有更大PHY有效负载的PHY。这些尚未在Zephyr中得到支持。
 */
#define IEEE802154_MTU			(IEEE802154_MAX_PHY_PACKET_SIZE - IEEE802154_FCS_LENGTH)

/* TODO: 支持IEEE 802.15.4-2015ff的灵活MTU和FCS长度 */

/** IEEE 802.15.4短地址长度。 */
#define IEEE802154_SHORT_ADDR_LENGTH	2

/** IEEE 802.15.4扩展地址长度。 */
#define IEEE802154_EXT_ADDR_LENGTH	8

/** IEEE 802.15.4最大地址长度。 */
#define IEEE802154_MAX_ADDR_LENGTH	IEEE802154_EXT_ADDR_LENGTH

/**
 * 表示“所有”频道或“任何”频道的特殊频道值 - 取决于上下文。
 */
#define IEEE802154_NO_CHANNEL		USHRT_MAX

/**
 * 表示IEEE 802.15.4广播短地址，参见第6.1节和第8.4.3节，表8-94，macShortAddress。
 */
#define IEEE802154_BROADCAST_ADDRESS	     0xffff

/**
 * 表示一个特殊的IEEE 802.15.4短地址，表示设备已与协调器关联但未收到短地址，参见第6.4.1节和第8.4.3节，表8-94，macShortAddress。
 */
#define IEEE802154_NO_SHORT_ADDRESS_ASSIGNED 0xfffe

/** 表示IEEE 802.15.4广播PAN ID，参见第6.1节。 */
#define IEEE802154_BROADCAST_PAN_ID 0xffff

/**
 * 表示macShortAddress MAC PIB属性的特殊值，而设备未关联，参见第8.4.3节，表8-94。
 */
#define IEEE802154_SHORT_ADDRESS_NOT_ASSOCIATED IEEE802154_BROADCAST_ADDRESS

/**
 * 表示macPanId MAC PIB属性的特殊值，而设备未关联，参见第8.4.3节，表8-94。
 */
#define IEEE802154_PAN_ID_NOT_ASSOCIATED	IEEE802154_BROADCAST_PAN_ID

/** 接口级别的安全属性，参见第9.5节。 */
struct ieee802154_security_ctx {
	/**
	 * 接口级别的传出帧计数器，第9.5节，表9-8，secFrameCounter。
	 *
	 * 仅在驱动程序未实现特定于密钥的帧计数器时使用。
	 */
	uint32_t frame_counter;

	/** @cond INTERNAL_HIDDEN */
	struct cipher_ctx enc;
	struct cipher_ctx dec;
	/** INTERNAL_HIDDEN @endcond */

	/**
	 * @brief 接口级别的帧加密安全密钥材料
	 *
	 * @details 目前原生L2仅支持单个secKeySource，参见第9.5节，表9-9，结合secKeyMode零（隐式密钥模式），参见第9.4.2.3节，表9-7。
	 *
	 * @warning 这不再符合2015+版本的标准，需要在未来扩展以实现完整的安全程序合规性。
	 */
	uint8_t key[16];

	/** 接口级别的安全密钥材料的长度（以字节为单位）。 */
	uint8_t key_len;

	/**
	 * @brief 帧安全级别，可能的值在第9.4.2.2节，表9-6中定义。
	 *
	 * @warning 目前原生L2允许为所有帧类型、命令和信息元素配置一个通用的安全级别。这不再符合2015+版本的标准，需要在未来扩展以实现完整的安全程序合规性。
	 */
	uint8_t level	: 3;

	/**
	 * @brief 帧安全密钥模式
	 *
	 * @details 目前仅部分支持隐式密钥模式，参见第9.4.2.3节，表9-7，secKeyMode。
	 *
	 * @warning 这不再符合2015+版本的标准，需要在未来扩展以实现完整的安全程序合规性。
	 */
	uint8_t key_mode	: 2;

	/** @cond INTERNAL_HIDDEN */
	uint8_t _unused	: 3;
	/** INTERNAL_HIDDEN @endcond */
};

/** @brief IEEE 802.15.4设备角色 */
enum ieee802154_device_role {
	IEEE802154_DEVICE_ROLE_ENDDEVICE,       /**< 终端设备 */
	IEEE802154_DEVICE_ROLE_COORDINATOR,     /**< 协调器 */
	IEEE802154_DEVICE_ROLE_PAN_COORDINATOR, /**< PAN协调器 */
};

/** IEEE 802.15.4 L2上下文。 */
struct ieee802154_context {
	/**
	 * @brief PAN ID
	 *
	 * @details 设备正在操作的PAN的标识符。如果此值为0xffff，则设备未关联。参见第8.4.3.1节，表8-94，macPanId。
	 *
	 * 以CPU字节顺序
	 */
	uint16_t pan_id;

	/**
	 * @brief 频道号
	 *
	 * @details 用于所有传输和接收的RF频道，参见第11.3节，表11-2，phyCurrentChannel。允许的值范围取决于PHY，如第10.1.3节所定义。
	 *
	 * 以CPU字节顺序
	 */
	uint16_t channel;

	/**
	 * @brief 短地址（以CPU字节顺序）
	 *
	 * @details 范围：
	 *  * 0x0000–0xfffd：已关联，已分配短地址
	 *  * 0xfffe：已关联但未分配短地址
	 *  * 0xffff：未关联（默认），
	 *
	 * 参见第6.4.1节，表6-4（短地址的使用）和第8.4.3.1节，表8-94，macShortAddress。
	 */
	uint16_t short_addr;

	/**
	 * @brief 扩展地址（小端序）
	 *
	 * @details 扩展地址是设备特定的，通常永久存储在设备上且不可变。
	 *
	 * 参见第8.4.3.1节，表8-94，macExtendedAddress。
	 */
	uint8_t ext_addr[IEEE802154_MAX_ADDR_LENGTH];

	/** 链路层地址（大端序） */
	struct net_linkaddr_storage linkaddr;

#ifdef CONFIG_NET_L2_IEEE802154_SECURITY
	/** 安全上下文 */
	struct ieee802154_security_ctx sec_ctx;
#endif

#ifdef CONFIG_NET_L2_IEEE802154_MGMT
	/** 指向扫描参数和结果的指针，由scan_ctx_lock保护 */
	struct ieee802154_req_params *scan_ctx;

	/**
	 * 用于维护此结构中所有字段的数据完整性，除非在字段级别另有说明。
	 */
	struct k_sem scan_ctx_lock;

	/**
	 * @brief 协调器扩展地址
	 *
	 * @details 参见第8.4.3.1节，表8-94，macCoordExtendedAddress，设备通过其关联的协调器的地址。
	 *
	 * 值为零表示协调器扩展地址未知（默认）。
	 *
	 * 小端序
	 */
	uint8_t coord_ext_addr[IEEE802154_MAX_ADDR_LENGTH];

	/**
	 * @brief 协调器短地址
	 *
	 * @details 参见第8.4.3.1节，表8-94，macCoordShortAddress，设备通过其关联的协调器分配的短地址。
	 *
	 * 值为0xfffe表示协调器仅使用其扩展地址。值为0xffff表示此值未知。
	 *
	 * 以CPU字节顺序
	 */
	uint16_t coord_short_addr;
#endif

	/** 传输功率，以dBm为单位。 */
	int16_t tx_power;

	/** L2标志 */
	enum net_l2_flags flags;

	/**
	 * @brief 数据序列号
	 *
	 * @details 添加到传输的数据帧或MAC命令的序列号，参见第8.4.3.1节，表8-94，macDsn。
	 */
	uint8_t sequence;

	/**
	 * @brief 设备角色
	 *
	 * @details 参见第6.1节：设备可以作为终端设备（0）、协调器（1）或PAN协调器（2）操作。如果未显式配置设备角色，则设备将被视为终端设备。
	 *
	 * 值为3是未定义的。
	 *
	 * 可以通过@ref ieee802154_device_role读取/设置。
	 */
	uint8_t device_role : 2;

	/** @cond INTERNAL_HIDDEN */
	uint8_t _unused : 5;
	/** INTERNAL_HIDDEN @endcond */

	/**
	 * 请求ACK标志，由ack_lock保护
	 */
	uint8_t ack_requested: 1;

	/** 预期的ACK序列号，由ack_lock保护 */
	uint8_t ack_seq;

	/** ACK锁，保护ack_*字段 */
	struct k_sem ack_lock;

	/**
	 * @brief 上下文锁
	 *
	 * @details 此锁保护所有可变上下文属性，除非在属性级别另有说明。
	 */
	struct k_sem ctx_lock;
};

/** @cond INTERNAL_HIDDEN */

/* 用于NET_L2_GET_CTX_TYPE的L2上下文类型 */
#define IEEE802154_L2_CTX_TYPE	struct ieee802154_context

/** INTERNAL_HIDDEN @endcond */

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* ZEPHYR_INCLUDE_NET_IEEE802154_H_ */

// By GST @Data