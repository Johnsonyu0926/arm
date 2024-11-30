// zephyr-3.7-branch/include/zephyr/net/ethernet.h

#ifndef ZEPHYR_INCLUDE_NET_ETHERNET_H_
#define ZEPHYR_INCLUDE_NET_ETHERNET_H_

#include <zephyr/kernel.h>
#include <zephyr/types.h>
#include <stdbool.h>
#include <zephyr/sys/atomic.h>

#include <zephyr/net/net_ip.h>
#include <zephyr/net/net_pkt.h>
#include <zephyr/net/lldp.h>
#include <zephyr/sys/util.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/ethernet_vlan.h>
#include <zephyr/net/ptp_time.h>

#if defined(CONFIG_NET_DSA)
#include <zephyr/net/dsa.h>
#endif

#if defined(CONFIG_NET_ETHERNET_BRIDGE)
#include <zephyr/net/ethernet_bridge.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Ethernet支持函数
 * @defgroup ethernet Ethernet支持函数
 * @ingroup networking
 * @{
 */

#define NET_ETH_ADDR_LEN		6U /**< Ethernet MAC地址长度 */

/** Ethernet地址 */
struct net_eth_addr {
	uint8_t addr[NET_ETH_ADDR_LEN]; /**< 存储地址的缓冲区 */
};

/** @cond INTERNAL_HIDDEN */

#define NET_ETH_HDR(pkt) ((struct net_eth_hdr *)net_pkt_data(pkt))

#define NET_ETH_PTYPE_CAN		0x000C /* CAN: 控制器局域网 */
#define NET_ETH_PTYPE_CANFD		0x000D /* CANFD: CAN灵活数据速率 */
#define NET_ETH_PTYPE_HDLC		0x0019 /* HDLC帧（如在PPP中） */
#define NET_ETH_PTYPE_ARP		0x0806
#define NET_ETH_PTYPE_IP		0x0800
#define NET_ETH_PTYPE_TSN		0x22f0 /* TSN（IEEE 1722）数据包 */
#define NET_ETH_PTYPE_IPV6		0x86dd
#define NET_ETH_PTYPE_VLAN		0x8100
#define NET_ETH_PTYPE_PTP		0x88f7
#define NET_ETH_PTYPE_LLDP		0x88cc
#define NET_ETH_PTYPE_ALL               0x0003 /* 来自linux/if_ether.h */
#define NET_ETH_PTYPE_ECAT		0x88a4
#define NET_ETH_PTYPE_EAPOL		0x888e
#define NET_ETH_PTYPE_IEEE802154	0x00F6 /* 来自linux/if_ether.h: IEEE802.15.4帧 */

#if !defined(ETH_P_ALL)
#define ETH_P_ALL	NET_ETH_PTYPE_ALL
#endif
#if !defined(ETH_P_IP)
#define ETH_P_IP	NET_ETH_PTYPE_IP
#endif
#if !defined(ETH_P_ARP)
#define ETH_P_ARP	NET_ETH_PTYPE_ARP
#endif
#if !defined(ETH_P_IPV6)
#define ETH_P_IPV6	NET_ETH_PTYPE_IPV6
#endif
#if !defined(ETH_P_8021Q)
#define ETH_P_8021Q	NET_ETH_PTYPE_VLAN
#endif
#if !defined(ETH_P_TSN)
#define ETH_P_TSN	NET_ETH_PTYPE_TSN
#endif
#if !defined(ETH_P_ECAT)
#define  ETH_P_ECAT	NET_ETH_PTYPE_ECAT
#endif
#if !defined(ETH_P_EAPOL)
#define ETH_P_EAPOL	NET_ETH_PTYPE_EAPOL
#endif
#if !defined(ETH_P_IEEE802154)
#define  ETH_P_IEEE802154 NET_ETH_PTYPE_IEEE802154
#endif
#if !defined(ETH_P_CAN)
#define ETH_P_CAN	NET_ETH_PTYPE_CAN
#endif
#if !defined(ETH_P_CANFD)
#define ETH_P_CANFD	NET_ETH_PTYPE_CANFD
#endif
#if !defined(ETH_P_HDLC)
#define ETH_P_HDLC	NET_ETH_PTYPE_HDLC
#endif

/** @endcond */

#define NET_ETH_MINIMAL_FRAME_SIZE	60   /**< 最小Ethernet帧大小 */
#define NET_ETH_MTU			1500 /**< Ethernet MTU大小 */

/** @cond INTERNAL_HIDDEN */

#if defined(CONFIG_NET_VLAN)
#define _NET_ETH_MAX_HDR_SIZE		(sizeof(struct net_eth_vlan_hdr))
#else
#define _NET_ETH_MAX_HDR_SIZE		(sizeof(struct net_eth_hdr))
#endif

#define _NET_ETH_MAX_FRAME_SIZE	(NET_ETH_MTU + _NET_ETH_MAX_HDR_SIZE)
/*
 * 将DSA（KSZ8794）的最大帧大小扩展一个字节（到1519），以存储尾标记。
 */
#if defined(CONFIG_NET_DSA)
#define NET_ETH_MAX_FRAME_SIZE (_NET_ETH_MAX_FRAME_SIZE + DSA_TAG_SIZE)
#define NET_ETH_MAX_HDR_SIZE (_NET_ETH_MAX_HDR_SIZE + DSA_TAG_SIZE)
#else
#define NET_ETH_MAX_FRAME_SIZE (_NET_ETH_MAX_FRAME_SIZE)
#define NET_ETH_MAX_HDR_SIZE (_NET_ETH_MAX_HDR_SIZE)
#endif

#define NET_ETH_VLAN_HDR_SIZE	4

/** @endcond */

/** @brief Ethernet硬件功能 */
enum ethernet_hw_caps {
	/** 支持所有IPv4、UDP、TCP的TX校验和卸载 */
	ETHERNET_HW_TX_CHKSUM_OFFLOAD	= BIT(0),

	/** 支持所有IPv4、UDP、TCP的RX校验和卸载 */
	ETHERNET_HW_RX_CHKSUM_OFFLOAD	= BIT(1),

	/** 支持VLAN */
	ETHERNET_HW_VLAN		= BIT(2),

	/** 支持启用/禁用自动协商 */
	ETHERNET_AUTO_NEGOTIATION_SET	= BIT(3),

	/** 支持10 Mbits链路 */
	ETHERNET_LINK_10BASE_T		= BIT(4),

	/** 支持100 Mbits链路 */
	ETHERNET_LINK_100BASE_T		= BIT(5),

	/** 支持1 Gbits链路 */
	ETHERNET_LINK_1000BASE_T	= BIT(6),

	/** 支持更改双工（半双工/全双工） */
	ETHERNET_DUPLEX_SET		= BIT(7),

	/** 支持IEEE 802.1AS（gPTP）时钟 */
	ETHERNET_PTP			= BIT(8),

	/** 支持IEEE 802.1Qav（基于信用的整形） */
	ETHERNET_QAV			= BIT(9),

	/** 支持混杂模式 */
	ETHERNET_PROMISC_MODE		= BIT(10),

	/** 可用优先级队列 */
	ETHERNET_PRIORITY_QUEUES	= BIT(11),

	/** 支持MAC地址过滤 */
	ETHERNET_HW_FILTERING		= BIT(12),

	/** 支持链路层发现协议 */
	ETHERNET_LLDP			= BIT(13),

	/** VLAN标签剥离 */
	ETHERNET_HW_VLAN_TAG_STRIP	= BIT(14),

	/** DSA交换机从端口 */
	ETHERNET_DSA_SLAVE_PORT		= BIT(15),

	/** DSA交换机主端口 */
	ETHERNET_DSA_MASTER_PORT	= BIT(16),

	/** 支持IEEE 802.1Qbv（计划流量） */
	ETHERNET_QBV			= BIT(17),

	/** 支持IEEE 802.1Qbu（帧抢占） */
	ETHERNET_QBU			= BIT(18),

	/** 支持TXTIME */
	ETHERNET_TXTIME			= BIT(19),

	/** 支持TX-Injection */
	ETHERNET_TXINJECTION_MODE	= BIT(20),
};

/** @cond INTERNAL_HIDDEN */

enum ethernet_config_type {
	ETHERNET_CONFIG_TYPE_AUTO_NEG,
	ETHERNET_CONFIG_TYPE_LINK,
	ETHERNET_CONFIG_TYPE_DUPLEX,
	ETHERNET_CONFIG_TYPE_MAC_ADDRESS,
	ETHERNET_CONFIG_TYPE_QAV_PARAM,
	ETHERNET_CONFIG_TYPE_QBV_PARAM,
	ETHERNET_CONFIG_TYPE_QBU_PARAM,
	ETHERNET_CONFIG_TYPE_TXTIME_PARAM,
	ETHERNET_CONFIG_TYPE_PROMISC_MODE,
	ETHERNET_CONFIG_TYPE_PRIORITY_QUEUES_NUM,
	ETHERNET_CONFIG_TYPE_FILTER,
	ETHERNET_CONFIG_TYPE_PORTS_NUM,
	ETHERNET_CONFIG_TYPE_T1S_PARAM,
	ETHERNET_CONFIG_TYPE_TXINJECTION_MODE,
	ETHERNET_CONFIG_TYPE_RX_CHECKSUM_SUPPORT,
	ETHERNET_CONFIG_TYPE_TX_CHECKSUM_SUPPORT
};

enum ethernet_qav_param_type {
	ETHERNET_QAV_PARAM_TYPE_DELTA_BANDWIDTH,
	ETHERNET_QAV_PARAM_TYPE_IDLE_SLOPE,
	ETHERNET_QAV_PARAM_TYPE_OPER_IDLE_SLOPE,
	ETHERNET_QAV_PARAM_TYPE_TRAFFIC_CLASS,
	ETHERNET_QAV_PARAM_TYPE_STATUS,
};

enum ethernet_t1s_param_type {
	ETHERNET_T1S_PARAM_TYPE_PLCA_CONFIG,
};

/** @endcond */

/** Ethernet T1S特定参数 */
struct ethernet_t1s_param {
	/** T1S参数类型 */
	enum ethernet_t1s_param_type type;
	union {
		/**
		 * PLCA是与多点10Base-T1S标准一起使用的物理层（PHY）碰撞避免技术。
		 *
		 * PLCA参数在标准[1]中描述为内存映射4（MMS = 4）中的寄存器（第9.6点）。
		 *
		 * IDVER	（PLCA ID版本）
		 * CTRL0	（PLCA控制0）
		 * CTRL1	（PLCA控制1）
		 * STATUS	（PLCA状态）
		 * TOTMR	（PLCA TO控制）
		 * BURST	（PLCA突发控制）
		 *
		 * 这些寄存器由每个OA TC6兼容供应商实现（例如LAN865x - 例如[2]）。
		 *
		 * 文档：
		 * [1] - "OPEN Alliance 10BASE-T1x MAC-PHY串行接口"（版本1.1）
		 * [2] - "DS60001734C" - LAN865x数据表
		 */
		struct {
			/** 启用T1S PLCA */
			bool enable;
			/** T1S PLCA节点ID范围：0到254 */
			uint8_t node_id;
			/** T1S PLCA节点计数范围：1到255 */
			uint8_t node_count;
			/** T1S PLCA突发计数范围：0x0到0xFF */
			uint8_t burst_count;
			/** T1S PLCA突发计时器 */
			uint8_t burst_timer;
			/** T1S PLCA TO值 */
			uint8_t to_timer;
		} plca;
	};
};

/** Ethernet Qav特定参数 */
struct ethernet_qav_param {
	/** 要使用的优先级队列ID */
	int queue_id;
	/** Qav参数类型 */
	enum ethernet_qav_param_type type;
	union {
		/** 如果为true，则表示队列启用了Qav */
		bool enabled;
		/** Delta带宽（带宽百分比） */
		unsigned int delta_bandwidth;
		/** 空闲斜率（每秒比特数） */
		unsigned int idle_slope;
		/** 操作空闲斜率（每秒比特数） */
		unsigned int oper_idle_slope;
		/** 队列绑定的流量类别 */
		unsigned int traffic_class;
	};
};

/** @cond INTERNAL_HIDDEN */

enum ethernet_qbv_param_type {
	ETHERNET_QBV_PARAM_TYPE_STATUS,
	ETHERNET_QBV_PARAM_TYPE_GATE_CONTROL_LIST,
	ETHERNET_QBV_PARAM_TYPE_GATE_CONTROL_LIST_LEN,
	ETHERNET_QBV_PARAM_TYPE_TIME,
};

enum ethernet_qbv_state_type {
	ETHERNET_QBV_STATE_TYPE_ADMIN,
	ETHERNET_QBV_STATE_TYPE_OPER,
};

enum ethernet_gate_state_operation {
	ETHERNET_SET_GATE_STATE,
	ETHERNET_SET_AND_HOLD_MAC_STATE,
	ETHERNET_SET_AND_RELEASE_MAC_STATE,
};

/** @endcond */

/** Ethernet Qbv特定参数 */
struct ethernet_qbv_param {
	/** 端口ID */
	int port_id;
	/** Qbv参数类型 */
	enum ethernet_qbv_param_type type;
	/** 这些参数是哪个状态（Admin/Oper） */
	enum ethernet_qbv_state_type state;
	union {
		/** 如果为true，则表示启用了Qbv */
		bool enabled;

		/** 门控信息 */
		struct {
			/** true = 打开，false = 关闭 */
			bool gate_status[NET_TC_TX_COUNT];

			/** GateState操作 */
			enum ethernet_gate_state_operation operation;

			/** 时间间隔（纳秒） */
			uint32_t time_interval;

			/** 门控列表行 */
			uint16_t row;
		} gate_control;

		/** 门控列表中的条目数 */
		uint32_t gate_control_list_len;

		/**
		 * 当类型设置为ETHERNET_QBV_PARAM_TYPE_TIME时，时间值一次性设置
		 */
		struct {
			/** 基准时间 */
			struct net_ptp_extended_time base_time;

			/** 周期时间 */
			struct net_ptp_time cycle_time;

			/** 扩展时间（纳秒） */
			uint32_t extension_time;
		};
	};
};

/** @cond INTERNAL_HIDDEN */

enum ethernet_qbu_param_type {
	ETHERNET_QBU_PARAM_TYPE_STATUS,
	ETHERNET_QBU_PARAM_TYPE_RELEASE_ADVANCE,
	ETHERNET_QBU_PARAM_TYPE_HOLD_ADVANCE,
	ETHERNET_QBU_PARAM_TYPE_PREEMPTION_STATUS_TABLE,

	/* 一些抢占设置来自Qbr规范。 */
	ETHERNET_QBR_PARAM_TYPE_LINK_PARTNER_STATUS,
	ETHERNET_QBR_PARAM_TYPE_ADDITIONAL_FRAGMENT_SIZE,
};

enum ethernet_qbu_preempt_status {
	ETHERNET_QBU_STATUS_EXPRESS,
	ETHERNET_QBU_STATUS_PREEMPTABLE
} __packed;

/** @endcond */

/** Ethernet Qbu特定参数 */
struct ethernet_qbu_param {
	/** 端口ID */
	int port_id;
	/** Qbu参数类型 */
	enum ethernet_qbu_param_type type;
	union {
		/** 保持提前量（纳秒） */
		uint32_t hold_advance;

		/** 释放提前量（纳秒） */
		uint32_t release_advance;

		/** framePreemptionAdminStatus值的序列 */
		enum ethernet_qbu_preempt_status
				frame_preempt_statuses[NET_TC_TX_COUNT];

		/** 如果为true，则表示启用了Qbu */
		bool enabled;

		/** 链路伙伴状态（来自Qbr） */
		bool link_partner_status;

		/**
		 * 额外的片段大小（来自Qbr）。最小的非最终片段大小为（additional_fragment_size + 1）* 64字节
		 */
		uint8_t additional_fragment_size : 2;
	};
};

/** @cond INTERNAL_HIDDEN */

enum ethernet_filter_type {
	ETHERNET_FILTER_TYPE_SRC_MAC_ADDRESS,
	ETHERNET_FILTER_TYPE_DST_MAC_ADDRESS,
};

/** @endcond */

/** Ethernet L2类型 */
enum ethernet_if_types {
	/** IEEE 802.3 Ethernet（默认） */
	L2_ETH_IF_TYPE_ETHERNET,

	/** IEEE 802.11 Wi-Fi */
	L2_ETH_IF_TYPE_WIFI,
} __packed;

/** Ethernet过滤器描述 */
struct ethernet_filter {
	/** 过滤器类型 */
	enum ethernet_filter_type type;
	/** 要过滤的MAC地址 */
	struct net_eth_addr mac_address;
	/** 设置（true）或取消设置（false）过滤器 */
	bool set;
};

/** @cond INTERNAL_HIDDEN */

enum ethernet_txtime_param_type {
	ETHERNET_TXTIME_PARAM_TYPE_ENABLE_QUEUES,
};

/** @endcond */

/** Ethernet TXTIME特定参数 */
struct ethernet_txtime_param {
	/** TXTIME参数类型 */
	enum ethernet_txtime_param_type type;
	/** 配置TXTIME的队列号 */
	int queue_id;
	/** 启用或禁用每个队列的TXTIME */
	bool enable_txtime;
};

/** 支持校验和卸载的协议 */
enum ethernet_checksum_support {
	/** 设备不支持任何L3/L4校验和卸载 */
	ETHERNET_CHECKSUM_SUPPORT_NONE			= NET_IF_CHECKSUM_NONE_BIT,
	/** 设备支持IPv4头的校验和卸载 */
	ETHERNET_CHECKSUM_SUPPORT_IPV4_HEADER		= NET_IF_CHECKSUM_IPV4_HEADER_BIT,
	/** 设备支持ICMPv4有效负载的校验和卸载（隐含IPv4头） */
	ETHERNET_CHECKSUM_SUPPORT_IPV4_ICMP		= NET_IF_CHECKSUM_IPV4_ICMP_BIT,
	/** 设备支持IPv6头的校验和卸载 */
	ETHERNET_CHECKSUM_SUPPORT_IPV6_HEADER		= NET_IF_CHECKSUM_IPV6_HEADER_BIT,
	/** 设备支持ICMPv6有效负载的校验和卸载（隐含IPv6头） */
	ETHERNET_CHECKSUM_SUPPORT_IPV6_ICMP		= NET_IF_CHECKSUM_IPV6_ICMP_BIT,
	/** 设备支持所有支持的IP协议的TCP校验和卸载 */
	ETHERNET_CHECKSUM_SUPPORT_TCP			= NET_IF_CHECKSUM_TCP_BIT,
	/** 设备支持所有支持的IP协议的UDP校验和卸载 */
	ETHERNET_CHECKSUM_SUPPORT_UDP			= NET_IF_CHECKSUM_UDP_BIT,
};

/** @cond INTERNAL_HIDDEN */

struct ethernet_config {
	union {
		bool auto_negotiation;
		bool full_duplex;
		bool promisc_mode;
		bool txinjection_mode;

		struct {
			bool link_10bt;
			bool link_100bt;
			bool link_1000bt;
		} l;

		struct net_eth_addr mac_address;

		struct ethernet_t1s_param t1s_param;
		struct ethernet_qav_param qav_param;
		struct ethernet_qbv_param qbv_param;
		struct ethernet_qbu_param qbu_param;
		struct ethernet_txtime_param txtime_param;

		int priority_queues_num;
		int ports_num;

		enum ethernet_checksum_support chksum_support;

		struct ethernet_filter filter;
	};
};

/** @endcond */

/** Ethernet L2 API操作。 */
struct ethernet_api {
	/**
	 * net_if_api必须放在此结构的第一个位置，以便与网络接口API兼容。
	 */
	struct net_if_api iface_api;

	/** 收集可选的以太网特定统计信息。如果需要为该驱动程序收集统计信息，则驱动程序应设置此指针。 */
#if defined(CONFIG_NET_STATISTICS_ETHERNET)
	struct net_stats_eth *(*get_stats)(const struct device *dev);
#endif

	/** 启动设备 */
	int (*start)(const struct device *dev);

	/** 停止设备 */
	int (*stop)(const struct device *dev);

	/** 获取设备功能 */
	enum ethernet_hw_caps (*get_capabilities)(const struct device *dev);

	/** 设置特定的硬件配置 */
	int (*set_config)(const struct device *dev,
			  enum ethernet_config_type type,
			  const struct ethernet_config *config);

	/** 获取特定的硬件配置 */
	int (*get_config)(const struct device *dev,
			  enum ethernet_config_type type,
			  struct ethernet_config *config);

	/** 当启用或禁用VLAN标签时，IP堆栈将调用此函数。如果enable设置为true，则添加了VLAN标签；如果为false，则删除了标签。驱动程序可以根据需要利用此信息。 */
#if defined(CONFIG_NET_VLAN)
	int (*vlan_setup)(const struct device *dev, struct net_if *iface,
			  uint16_t tag, bool enable);
#endif /* CONFIG_NET_VLAN */

	/** 返回与此以太网设备绑定的ptp_clock设备 */
#if defined(CONFIG_PTP_CLOCK)
	const struct device *(*get_ptp_clock)(const struct device *dev);
#endif /* CONFIG_PTP_CLOCK */

	/** 发送网络数据包 */
	int (*send)(const struct device *dev, struct net_pkt *pkt);
};

/** @cond INTERNAL_HIDDEN */

/* 确保网络接口API在以太网API结构内正确设置（它是第一个）。 */
BUILD_ASSERT(offsetof(struct ethernet_api, iface_api) == 0);

struct net_eth_hdr {
	struct net_eth_addr dst;
	struct net_eth_addr src;
	uint16_t type;
} __packed;

struct ethernet_vlan {
	/** 启用了VLAN的网络接口 */
	struct net_if *iface;

	/** VLAN标签 */
	uint16_t tag;
};

#if defined(CONFIG_NET_VLAN_COUNT)
#define NET_VLAN_MAX_COUNT CONFIG_NET_VLAN_COUNT
#else
/* 即使没有VLAN支持，最小计数也必须设置为1。 */
#define NET_VLAN_MAX_COUNT 1
#endif

/** @endcond */

/** Ethernet LLDP特定参数 */
struct ethernet_lldp {
	/** 用于跟踪计时器 */
	sys_snode_t node;

	/** 接口的LLDP数据单元强制TLV。 */
	const struct net_lldpdu *lldpdu;

	/** 接口的LLDP数据单元可选TLV */
	const uint8_t *optional_du;

	/** 可选数据单元TLV的长度 */
	size_t optional_len;

	/** 支持LLDP的网络接口。 */
	struct net_if *iface;

	/** LLDP TX计时器启动时间 */
	int64_t tx_timer_start;

	/** LLDP TX超时 */
	uint32_t tx_timer_timeout;

	/** LLDP RX回调函数 */
	net_lldp_recv_cb_t cb;
};

/** @cond INTERNAL_HIDDEN */

enum ethernet_flags {
	ETH_CARRIER_UP,
};

/** VLAN所需的以太网L2上下文 */
struct ethernet_context {
	/** 表示以太网状态的标志，从多个线程访问。 */
	atomic_t flags;

#if defined(CONFIG_NET_ETHERNET_BRIDGE)
	struct eth_bridge_iface_context bridge;
#endif

	/** 载波ON/OFF处理程序工作。以太网L2驱动程序注意到载波ON/OFF情况时，用于创建网络接口UP/DOWN事件。我们不能从管理处理程序内部创建另一个网络管理事件，因此我们使用工作线程触发UP/DOWN事件。 */
	struct k_work carrier_work;

	/** 网络接口。 */
	struct net_if *iface;

#if defined(CONFIG_NET_LLDP)
	struct ethernet_lldp lldp[NET_VLAN_MAX_COUNT];
#endif

	/** 这告诉以太网支持哪些L2功能。 */
	enum net_l2_flags ethernet_l2_flags;

#if defined(CONFIG_NET_L2_PTP)
	/** 此网络设备的PTP端口号。我们需要在此处存储端口号，以便不需要为每个传入的PTP数据包获取它。 */
	int port;
#endif

#if defined(CONFIG_NET_DSA)
	/** DSA RX回调函数 - 用于自定义处理 - 例如捕获MAC地址时重定向数据包 */
	dsa_net_recv_cb_t dsa_recv_cb;

	/** 交换机物理端口号 */
	uint8_t dsa_port_idx;

	/** DSA上下文指针 */
	struct dsa_context *dsa_ctx;

	/** 通过DSA主端口发送网络数据包 */
	dsa_send_t dsa_send;
#endif

	/** 网络载波是否已启动 */
	bool is_net_carrier_up : 1;

	/** 此上下文是否已初始化 */
	bool is_init : 1;

	/** 以太网网络接口类型 */
	enum ethernet_if_types eth_if_type;
};

/**
 * @brief 初始化给定接口的以太网L2堆栈
 *
 * @param iface 有效的网络接口指针
 */
void ethernet_init(struct net_if *iface);

#define ETHERNET_L2_CTX_TYPE	struct ethernet_context

/* VLAN的单独头文件，因为某些设备接口可能不支持VLAN。 */
struct net_eth_vlan_hdr {
	struct net_eth_addr dst;
	struct net_eth_addr src;
	struct {
		uint16_t tpid; /* 标签协议ID */
		uint16_t tci;  /* 标签控制信息 */
	} vlan;
	uint16_t type;
} __packed;

/** @endcond */

/**
 * @brief 检查以太网MAC地址是否为广播地址。
 *
 * @param addr 有效的以太网MAC地址指针。
 *
 * @return 如果地址是广播地址，则返回true，否则返回false
 */
static inline bool net_eth_is_addr_broadcast(struct net_eth_addr *addr)
{
	if (addr->addr[0] == 0xff &&
	    addr->addr[1] == 0xff &&
	    addr->addr[2] == 0xff &&
	    addr->addr[3] == 0xff &&
	    addr->addr[4] == 0xff &&
	    addr->addr[5] == 0xff) {
		return true;
	}

	return false;
}

/**
 * @brief 检查以太网MAC地址是否为全零地址。
 *
 * @param addr 有效的以太网MAC地址指针。
 *
 * @return 如果地址是全零地址，则返回true，否则返回false
 */
static inline bool net_eth_is_addr_all_zeroes(struct net_eth_addr *addr)
{
	if (addr->addr[0] == 0x00 &&
	    addr->addr[1] == 0x00 &&
	    addr->addr[2] == 0x00 &&
	    addr->addr[3] == 0x00 &&
	    addr->addr[4] == 0x00 &&
	    addr->addr[5] == 0x00) {
		return true;
	}

	return false;
}

/**
 * @brief 检查以太网MAC地址是否未指定。
 *
 * @param addr 有效的以太网MAC地址指针。
 *
 * @return 如果地址未指定，则返回true，否则返回false
 */
static inline bool net_eth_is_addr_unspecified(struct net_eth_addr *addr)
{
	if (addr->addr[0] == 0x00 &&
	    addr->addr[1] == 0x00 &&
	    addr->addr[2] == 0x00 &&
	    addr->addr[3] == 0x00 &&
	    addr->addr[4] == 0x00 &&
	    addr->addr[5] == 0x00) {
		return true;
	}

	return false;
}

/**
 * @brief 检查以太网MAC地址是否为多播地址。
 *
 * @param addr 有效的以太网MAC地址指针。
 *
 * @return 如果地址是多播地址，则返回true，否则返回false
 */
static inline bool net_eth_is_addr_multicast(struct net_eth_addr *addr)
{
#if defined(CONFIG_NET_IPV6)
	if (addr->addr[0] == 0x33 &&
	    addr->addr[1] == 0x33) {
		return true;
	}
#endif

#if defined(CONFIG_NET_IPV4)
	if (addr->addr[0] == 0x01 &&
	    addr->addr[1] == 0x00 &&
	    addr->addr[2] == 0x5e) {
		return true;
	}
#endif

	return false;
}

/**
 * @brief 检查以太网MAC地址是否为组地址。
 *
 * @param addr 有效的以太网MAC地址指针。
 *
 * @return 如果地址是组地址，则返回true，否则返回false
 */
static inline bool net_eth_is_addr_group(struct net_eth_addr *addr)
{
	return addr->addr[0] & 0x01;
}

/**
 * @brief 检查以太网MAC地址是否有效。
 *
 * @param addr 有效的以太网MAC地址指针。
 *
 * @return 如果地址有效，则返回true，否则返回false
 */
static inline bool net_eth_is_addr_valid(struct net_eth_addr *addr)
{
	return !net_eth_is_addr_unspecified(addr) && !net_eth_is_addr_group(addr);
}

/**
 * @brief 检查以太网MAC地址是否为LLDP多播地址。
 *
 * @param addr 有效的以太网MAC地址指针。
 *
 * @return 如果地址是LLDP多播地址，则返回true，否则返回false
 */
static inline bool net_eth_is_addr_lldp_multicast(struct net_eth_addr *addr)
{
#if defined(CONFIG_NET_GPTP) || defined(CONFIG_NET_LLDP)
	if (addr->addr[0] == 0x01 &&
	    addr->addr[1] == 0x80 &&
	    addr->addr[2] == 0xc2 &&
	    addr->addr[3] == 0x00 &&
	    addr->addr[4] == 0x00 &&
	    addr->addr[5] == 0x0e) {
		return true;
	}
#else
	ARG_UNUSED(addr);
#endif

	return false;
}

/**
 * @brief 检查以太网MAC地址是否为PTP多播地址。
 *
 * @param addr 有效的以太网MAC地址指针。
 *
 * @return 如果地址是PTP多播地址，则返回true，否则返回false
 */
static inline bool net_eth_is_addr_ptp_multicast(struct net_eth_addr *addr)
{
#if defined(CONFIG_NET_GPTP)
	if (addr->addr[0] == 0x01 &&
	    addr->addr[1] == 0x1b &&
	    addr->addr[2] == 0x19 &&
	    addr->addr[3] == 0x00 &&
	    addr->addr[4] == 0x00 &&
	    addr->addr[5] == 0x00) {
		return true;
	}
#else
	ARG_UNUSED(addr);
#endif

	return false;
}

/**
 * @brief 返回以太网广播地址。
 *
 * @return 以太网广播地址。
 */
const struct net_eth_addr *net_eth_broadcast_addr(void);

/**
 * @brief 将IPv4多播地址转换为以太网地址。
 *
 * @param ipv4_addr IPv4多播地址
 * @param mac_addr 以太网地址的输出缓冲区
 */
void net_eth_ipv4_mcast_to_mac_addr(const struct in_addr *ipv4_addr,
				    struct net_eth_addr *mac_addr);

/**
 * @brief 将IPv6多播地址转换为以太网地址。
 *
 * @param ipv6_addr IPv6多播地址
 * @param mac_addr 以太网地址的输出缓冲区
 */
void net_eth_ipv6_mcast_to_mac_addr(const struct in6_addr *ipv6_addr,
				    struct net_eth_addr *mac_addr);

/**
 * @brief 返回以太网设备硬件功能信息。
 *
 * @param iface 网络接口
 *
 * @return 硬件功能
 */
static inline
enum ethernet_hw_caps net_eth_get_hw_capabilities(struct net_if *iface)
{
	const struct ethernet_api *eth =
		(struct ethernet_api *)net_if_get_device(iface)->api;

	if (!eth->get_capabilities) {
		return (enum ethernet_hw_caps)0;
	}

	return eth->get_capabilities(net_if_get_device(iface));
}

/**
 * @brief 返回以太网设备硬件配置信息。
 *
 * @param iface 网络接口
 * @param type 配置类型
 * @param config 以太网配置
 *
 * @return 如果成功则返回0，如果错误则返回<0
 */
static inline
int net_eth_get_hw_config(struct net_if *iface, enum ethernet_config_type type,
			 struct ethernet_config *config)
{
	const struct ethernet_api *eth =
		(struct ethernet_api *)net_if_get_device(iface)->api;

	if (!eth->get_config) {
		return -ENOTSUP;
	}

	return eth->get_config(net_if_get_device(iface), type, config);
}


/**
 * @brief 向接口添加VLAN标签。
 *
 * @param iface 要使用的接口。
 * @param tag 要添加的VLAN标签
 *
 * @return 如果成功则返回0，如果错误则返回<0
 */
#if defined(CONFIG_NET_VLAN)
int net_eth_vlan_enable(struct net_if *iface, uint16_t tag);
#else
static inline int net_eth_vlan_enable(struct net_if *iface, uint16_t tag)
{
	ARG_UNUSED(iface);
	ARG_UNUSED(tag);

	return -EINVAL;
}
#endif

/**
 * @brief 从接口中删除VLAN标签。
 *
 * @param iface 要使用的接口。
 * @param tag 要删除的VLAN标签
 *
 * @return 如果成功则返回0，如果错误则返回<0
 */
#if defined(CONFIG_NET_VLAN)
int net_eth_vlan_disable(struct net_if *iface, uint16_t tag);
#else
static inline int net_eth_vlan_disable(struct net_if *iface, uint16_t tag)
{
	ARG_UNUSED(iface);
	ARG_UNUSED(tag);

	return -EINVAL;
}
#endif

/**
 * @brief 返回指定网络接口的VLAN标签。
 *
 * 请注意，接口参数必须是VLAN接口，而不是以太网接口。
 *
 * @param iface VLAN网络接口。
 *
 * @return 此接口的VLAN标签，如果未为该接口配置VLAN，则返回NET_VLAN_TAG_UNSPEC。
 */
#if defined(CONFIG_NET_VLAN)
uint16_t net_eth_get_vlan_tag(struct net_if *iface);
#else
static inline uint16_t net_eth_get_vlan_tag(struct net_if *iface)
{
	ARG_UNUSED(iface);

	return NET_VLAN_TAG_UNSPEC;
}
#endif

/**
 * @brief 返回与此VLAN标签相关的网络接口
 *
 * @param iface 主网络接口（不是VLAN接口）。
 * @param tag VLAN标签
 *
 * @return 与此标签相关的网络接口，如果不存在此类接口，则返回NULL。
 */
#if defined(CONFIG_NET_VLAN)
struct net_if *net_eth_get_vlan_iface(struct net_if *iface, uint16_t tag);
#else
static inline
struct net_if *net_eth_get_vlan_iface(struct net_if *iface, uint16_t tag)
{
	ARG_UNUSED(iface);
	ARG_UNUSED(tag);

	return NULL;
}
#endif

/**
 * @brief 返回与此VLAN标签关联的主网络接口。
 *
 * @param iface VLAN网络接口。这用于获取以太网L2上下文的指针
 *
 * @return 与此标签相关的网络接口，如果不存在此类接口，则返回NULL。
 */
#if defined(CONFIG_NET_VLAN)
struct net_if *net_eth_get_vlan_main(struct net_if *iface);
#else
static inline
struct net_if *net_eth_get_vlan_main(struct net_if *iface)
{
	ARG_UNUSED(iface);

	return NULL;
}
#endif

/**
 * @brief 检查是否有任何VLAN接口启用到此特定的以太网网络接口。
 *
 * 请注意，iface必须是实际的以太网接口，而不是虚拟VLAN接口。
 *
 * @param ctx 以太网上下文
 * @param iface 以太网网络接口
 *
 * @return 如果此网络接口启用了VLAN，则返回true，否则返回false。
 */
#if defined(CONFIG_NET_VLAN)
bool net_eth_is_vlan_enabled(struct ethernet_context *ctx,
			     struct net_if *iface);
#else
static inline bool net_eth_is_vlan_enabled(struct ethernet_context *ctx,
					   struct net_if *iface)
{
	ARG_UNUSED(ctx);
	ARG_UNUSED(iface);

	return false;
}
#endif

/**
 * @brief 获取给定网络接口的VLAN状态（启用或未启用）。
 *
 * @param iface 网络接口
 *
 * @return 如果此网络接口启用了VLAN，则返回true，否则返回false。
 */
#if defined(CONFIG_NET_VLAN)
bool net_eth_get_vlan_status(struct net_if *iface);
#else
static inline bool net_eth_get_vlan_status(struct net_if *iface)
{
	ARG_UNUSED(iface);

	return false;
}
#endif

/**
 * @brief 检查给定接口是否为VLAN接口。
 *
 * @param iface 网络接口
 *
 * @return 如果此网络接口为VLAN接口，则返回true，否则返回false。
 */
#if defined(CONFIG_NET_VLAN)
bool net_eth_is_vlan_interface(struct net_if *iface);
#else
static inline bool net_eth_is_vlan_interface(struct net_if *iface)
{
	ARG_UNUSED(iface);

	return false;
}
#endif

/** @cond INTERNAL_HIDDEN */

#if !defined(CONFIG_ETH_DRIVER_RAW_MODE)

#define Z_ETH_NET_DEVICE_INIT_INSTANCE(node_id, dev_id, name, instance,	\
				       init_fn, pm, data, config, prio,	\
				       api, mtu)			\
	Z_NET_DEVICE_INIT_INSTANCE(node_id, dev_id, name, instance,	\
				   init_fn, pm, data, config, prio,	\
				   api, ETHERNET_L2,			\
				   NET_L2_GET_CTX_TYPE(ETHERNET_L2), mtu)

#else /* CONFIG_ETH_DRIVER_RAW_MODE */

#define Z_ETH_NET_DEVICE_INIT_INSTANCE(node_id, dev_id, name, instance,	\
				       init_fn, pm, data, config, prio,	\
				       api, mtu)			\
	Z_DEVICE_STATE_DEFINE(dev_id);					\
	Z_DEVICE_DEFINE(node_id, dev_id, name, init_fn, pm, data,	\
			config, POST_KERNEL, prio, api,			\
			&Z_DEVICE_STATE_NAME(dev_id));

#endif /* CONFIG_ETH_DRIVER_RAW_MODE */

#define Z_ETH_NET_DEVICE_INIT(node_id, dev_id, name, init_fn, pm, data,	\
			      config, prio, api, mtu)			\
	Z_ETH_NET_DEVICE_INIT_INSTANCE(node_id, dev_id, name, 0,	\
				       init_fn, pm, data, config, prio,	\
				       api, mtu)

/** @endcond */

/**
 * @brief 创建以太网网络接口并将其绑定到网络设备。
 *
 * @param dev_id 网络设备ID。
 * @param name 驱动程序向系统公开的名称。
 * @param init_fn 驱动程序的初始化函数地址。
 * @param pm 与设备关联的pm_device结构的引用。（可选）。
 * @param data* 指向设备私有数据的指针。
 * @param config 包含此驱动程序实例的配置信息的结构地址。
 * @param prio 配置发生的初始化级别。
 * @param api 提供指向驱动程序使用的API函数结构的初始指针。可以为NULL。
 * @param mtu 此网络接口的最大传输单元（字节）。
 */
#define ETH_NET_DEVICE_INIT(dev_id, name, init_fn, pm, data, config,	\
			    prio, api, mtu)				\
	Z_ETH_NET_DEVICE_INIT(DT_INVALID_NODE, dev_id, name, init_fn,	\
			      pm, data, config, prio, api, mtu)
/**
 * @brief 创建多个以太网网络接口并将其绑定到网络设备。
 * 如果您的网络设备需要多个网络接口实例，请使用以下宏并每次提供不同的实例后缀（0、1、2...或a、b、c...任何适合您的）。
 *
 * @param dev_id 网络设备ID。
 * @param name 驱动程序向系统公开的名称。
 * @param instance 实例标识符。
 * @param init_fn 驱动程序的初始化函数地址。
 * @param pm 与设备关联的pm_device结构的引用。（可选）。
 * @param data 指向设备私有数据的指针。
 * @param config 包含此驱动程序实例的配置信息的结构地址。
 * @param prio 配置发生的初始化级别。
 * @param api 提供指向驱动程序使用的API函数结构的初始指针。可以为NULL。
 * @param mtu 此网络接口的最大传输单元（字节）。
 */
#define ETH_NET_DEVICE_INIT_INSTANCE(dev_id, name, instance, init_fn,	\
				     pm, data, config, prio, api, mtu)	\
	Z_ETH_NET_DEVICE_INIT_INSTANCE(DT_INVALID_NODE, dev_id, name,	\
				       instance, init_fn, pm, data,	\
				       config, prio, api, mtu)
/**
 * @brief 类似于ETH_NET_DEVICE_INIT，但从设备树中获取元数据。
 * 创建以太网网络接口并将其绑定到网络设备。
 *
 * @param node_id 设备树节点标识符。
 * @param init_fn 驱动程序的初始化函数地址。
 * @param pm 与设备关联的pm_device结构的引用。（可选）。
 * @param data 指向设备私有数据的指针。
 * @param config 包含此驱动程序实例的配置信息的结构地址。
 * @param prio 配置发生的初始化级别。
 * @param api 提供指向驱动程序使用的API函数结构的初始指针。可以为NULL。
 * @param mtu 此网络接口的最大传输单元（字节）。
 */
#define ETH_NET_DEVICE_DT_DEFINE(node_id, init_fn, pm, data, config,	\
				 prio, api, mtu)			\
	Z_ETH_NET_DEVICE_INIT(node_id, Z_DEVICE_DT_DEV_ID(node_id),	\
			      DEVICE_DT_NAME(node_id), init_fn, pm,	\
			      data, config, prio, api, mtu)
/**
 * @brief 类似于ETH_NET_DEVICE_DT_DEFINE，用于DT_DRV_COMPAT兼容的实例
 *
 * @param inst 实例编号。这在调用ETH_NET_DEVICE_DT_DEFINE时替换为<tt>DT_DRV_COMPAT(inst)</tt>。
 *
 * @param ... ETH_NET_DEVICE_DT_DEFINE预期的其他参数。
 */
#define ETH_NET_DEVICE_DT_INST_DEFINE(inst, ...) \
	ETH_NET_DEVICE_DT_DEFINE(DT_DRV_INST(inst), __VA_ARGS__)
/**
 * @brief 通知以太网L2驱动程序检测到以太网载波。这发生在连接电缆时。
 *
 * @param iface 网络接口
 */
void net_eth_carrier_on(struct net_if *iface);
/**
 * @brief 通知以太网L2驱动程序以太网载波丢失。这发生在断开电缆时。
 *
 * @param iface 网络接口
 */
void net_eth_carrier_off(struct net_if *iface);
/**
 * @brief 设置混杂模式为ON或OFF。
 *
 * @param iface 网络接口
 * @param enable 开（true）或关（false）
 *
 * @return 如果模式设置或取消设置成功，则返回0，否则返回<0。
 */
int net_eth_promisc_mode(struct net_if *iface, bool enable);
/**
 * @brief 设置TX-Injection模式为ON或OFF。
 *
 * @param iface 网络接口
 * @param enable 开（true）或关（false）
 *
 * @return 如果模式设置或取消设置成功，则返回0，否则返回<0。
 */
int net_eth_txinjection_mode(struct net_if *iface, bool enable);
/**
 * @brief 为MAC地址@p mac设置或取消设置HW过滤。
 *
 * @param iface 网络接口
 * @param mac 指向以太网MAC地址的指针
 * @param type 过滤器类型，源或目标
 * @param enable 设置（true）或取消设置（false）
 *
 * @return 如果过滤器设置或取消设置成功，则返回0，否则返回<0。
 */
int net_eth_mac_filter(struct net_if *iface, struct net_eth_addr *mac,
		       enum ethernet_filter_type type, bool enable);
/**
 * @brief 返回与此以太网网络接口绑定的PTP时钟。
 *
 * @param iface 网络接口
 *
 * @return 如果找到PTP时钟，则返回指向PTP时钟的指针，如果未找到或此以太网接口不支持PTP，则返回NULL。
 */
#if defined(CONFIG_PTP_CLOCK)
const struct device *net_eth_get_ptp_clock(struct net_if *iface);
#else
static inline const struct device *net_eth_get_ptp_clock(struct net_if *iface)
{
	ARG_UNUSED(iface);
	return NULL;
}
#endif
/**
 * @brief 返回与此以太网网络接口索引绑定的PTP时钟。
 *
 * @param index 网络接口索引
 *
 * @return 如果找到PTP时钟，则返回指向PTP时钟的指针，如果未找到或此以太网接口索引不支持PTP，则返回NULL。
 */
__syscall const struct device *net_eth_get_ptp_clock_by_index(int index);
/**
 * @brief 返回与此接口关联的PTP端口号。
 *
 * @param iface 网络接口
 *
 * @return 端口号，如果<0则表示没有此类端口
 */
#if defined(CONFIG_NET_L2_PTP)
int net_eth_get_ptp_port(struct net_if *iface);
#else
static inline int net_eth_get_ptp_port(struct net_if *iface)
{
	ARG_UNUSED(iface);
	return -ENODEV;
}
#endif /* CONFIG_NET_L2_PTP */
/**
 * @brief 设置与此接口关联的PTP端口号。
 *
 * @param iface 网络接口
 * @param port 要设置的端口号
 */
#if defined(CONFIG_NET_L2_PTP)
void net_eth_set_ptp_port(struct net_if *iface, int port);
#else
static inline void net_eth_set_ptp_port(struct net_if *iface, int port)
{
	ARG_UNUSED(iface);
	ARG_UNUSED(port);
}
#endif /* CONFIG_NET_L2_PTP */
/**
 * @brief 检查以太网L2网络接口是否可以执行Wi-Fi。
 *
 * @param iface 指向网络接口的指针
 *
 * @return 如果接口支持Wi-Fi，则返回true，否则返回false。
 */
static inline bool net_eth_type_is_wifi(struct net_if *iface)
{
	const struct ethernet_context *ctx = (struct ethernet_context *)
		net_if_l2_data(iface);
	return ctx->eth_if_type == L2_ETH_IF_TYPE_WIFI;
}
/**
 * @}
 */
#ifdef __cplusplus
}
#endif
#include <zephyr/syscalls/ethernet.h>
#endif /* ZEPHYR_INCLUDE_NET_ETHERNET_H_ */
// By GST @Data