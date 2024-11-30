// zephyr-3.7-branch/include/zephyr/net/mii.h

/** @file
 * @brief IEEE 802.3第2节MII兼容PHY收发器的定义
 */

#ifndef ZEPHYR_INCLUDE_NET_MII_H_
#define ZEPHYR_INCLUDE_NET_MII_H_

/**
 * @brief 以太网MII（媒体独立接口）功能
 * @defgroup ethernet_mii 以太网MII支持功能
 * @ingroup ethernet
 * @{
 */

/* MII管理寄存器 */
/** 基本模式控制寄存器 */
#define MII_BMCR       0x0
/** 基本模式状态寄存器 */
#define MII_BMSR       0x1
/** PHY ID 1寄存器 */
#define MII_PHYID1R    0x2
/** PHY ID 2寄存器 */
#define MII_PHYID2R    0x3
/** 自动协商广告寄存器 */
#define MII_ANAR       0x4
/** 自动协商链接伙伴能力寄存器 */
#define MII_ANLPAR     0x5
/** 自动协商扩展寄存器 */
#define MII_ANER       0x6
/** 自动协商下一页传输寄存器 */
#define MII_ANNPTR     0x7
/** 自动协商链接伙伴接收的下一页寄存器 */
#define MII_ANLPRNPR   0x8
/** 1000BASE-T控制寄存器 */
#define MII_1KTCR 0x9
/** 1000BASE-T状态寄存器 */
#define MII_1KSTSR 0xa
/** MMD访问控制寄存器 */
#define MII_MMD_ACR    0xd
/** MMD访问地址数据寄存器 */
#define MII_MMD_AADR   0xe
/** 扩展状态寄存器 */
#define MII_ESTAT      0xf

/* 基本模式控制寄存器（BMCR）位定义 */
/** PHY复位 */
#define MII_BMCR_RESET             (1 << 15)
/** 启用环回模式 */
#define MII_BMCR_LOOPBACK          (1 << 14)
/** 10=1000Mbps 01=100Mbps; 00=10Mbps */
#define MII_BMCR_SPEED_LSB         (1 << 13)
/** 启用自动协商 */
#define MII_BMCR_AUTONEG_ENABLE    (1 << 12)
/** 省电模式 */
#define MII_BMCR_POWER_DOWN        (1 << 11)
/** 电气隔离PHY与MII */
#define MII_BMCR_ISOLATE           (1 << 10)
/** 重新启动自动协商 */
#define MII_BMCR_AUTONEG_RESTART   (1 << 9)
/** 全双工模式 */
#define MII_BMCR_DUPLEX_MODE       (1 << 8)
/** 10=1000Mbps 01=100Mbps; 00=10Mbps */
#define MII_BMCR_SPEED_MSB         (1 << 6)
/** 链接速度字段 */
#define   MII_BMCR_SPEED_MASK      (1 << 6 | 1 << 13)
/** 选择速度10 Mb/s */
#define   MII_BMCR_SPEED_10        (0 << 6 | 0 << 13)
/** 选择速度100 Mb/s */
#define   MII_BMCR_SPEED_100       (0 << 6 | 1 << 13)
/** 选择速度1000 Mb/s */
#define   MII_BMCR_SPEED_1000      (1 << 6 | 0 << 13)

/* 基本模式状态寄存器（BMSR）位定义 */
/** 支持100BASE-T4 */
#define MII_BMSR_100BASE_T4        (1 << 15)
/** 支持100BASE-X全双工 */
#define MII_BMSR_100BASE_X_FULL    (1 << 14)
/** 支持100BASE-X半双工 */
#define MII_BMSR_100BASE_X_HALF    (1 << 13)
/** 支持10 Mb/s全双工 */
#define MII_BMSR_10_FULL           (1 << 12)
/** 支持10 Mb/s半双工 */
#define MII_BMSR_10_HALF           (1 << 11)
/** 支持100BASE-T2全双工 */
#define MII_BMSR_100BASE_T2_FULL   (1 << 10)
/** 支持100BASE-T2半双工 */
#define MII_BMSR_100BASE_T2_HALF   (1 << 9)
/** 在寄存器15中扩展状态信息 */
#define MII_BMSR_EXTEND_STATUS     (1 << 8)
/** PHY接受管理帧时抑制前导码 */
#define MII_BMSR_MF_PREAMB_SUPPR   (1 << 6)
/** 自动协商过程完成 */
#define MII_BMSR_AUTONEG_COMPLETE  (1 << 5)
/** 检测到远程故障 */
#define MII_BMSR_REMOTE_FAULT      (1 << 4)
/** PHY能够执行自动协商 */
#define MII_BMSR_AUTONEG_ABILITY   (1 << 3)
/** 链接已建立 */
#define MII_BMSR_LINK_STATUS       (1 << 2)
/** 检测到跳变条件 */
#define MII_BMSR_JABBER_DETECT     (1 << 1)
/** 扩展寄存器功能 */
#define MII_BMSR_EXTEND_CAPAB      (1 << 0)

/* 自动协商广告寄存器（ANAR）位定义 */
/* 自动协商链接伙伴能力寄存器（ANLPAR）位定义 */
/** 下一页 */
#define MII_ADVERTISE_NEXT_PAGE    (1 << 15)
/** 链接伙伴确认响应 */
#define MII_ADVERTISE_LPACK        (1 << 14)
/** 远程故障 */
#define MII_ADVERTISE_REMOTE_FAULT (1 << 13)
/** 尝试非对称暂停 */
#define MII_ADVERTISE_ASYM_PAUSE   (1 << 11)
/** 尝试暂停 */
#define MII_ADVERTISE_PAUSE        (1 << 10)
/** 尝试支持100BASE-T4 */
#define MII_ADVERTISE_100BASE_T4   (1 << 9)
/** 尝试支持100BASE-X全双工 */
#define MII_ADVERTISE_100_FULL     (1 << 8)
/** 尝试支持100BASE-X */
#define MII_ADVERTISE_100_HALF     (1 << 7)
/** 尝试支持10 Mb/s全双工 */
#define MII_ADVERTISE_10_FULL      (1 << 6)
/** 尝试支持10 Mb/s半双工 */
#define MII_ADVERTISE_10_HALF      (1 << 5)
/** 选择字段掩码 */
#define MII_ADVERTISE_SEL_MASK     (0x1F << 0)
/** 选择字段 */
#define MII_ADVERTISE_SEL_IEEE_802_3   0x01

/* 1000BASE-T控制寄存器位定义 */
/** 尝试支持1000BASE-T全双工 */
#define MII_ADVERTISE_1000_FULL    (1 << 9)
/** 尝试支持1000BASE-T半双工 */
#define MII_ADVERTISE_1000_HALF    (1 << 8)

/** 广告所有速度 */
#define MII_ADVERTISE_ALL (MII_ADVERTISE_10_HALF | MII_ADVERTISE_10_FULL |\
			   MII_ADVERTISE_100_HALF | MII_ADVERTISE_100_FULL |\
			   MII_ADVERTISE_SEL_IEEE_802_3)

/* 扩展状态寄存器位定义 */
/** 支持1000BASE-X全双工 */
#define MII_ESTAT_1000BASE_X_FULL  (1 << 15)
/** 支持1000BASE-X半双工 */
#define MII_ESTAT_1000BASE_X_HALF  (1 << 14)
/** 支持1000BASE-T全双工 */
#define MII_ESTAT_1000BASE_T_FULL  (1 << 13)
/** 支持1000BASE-T半双工 */
#define MII_ESTAT_1000BASE_T_HALF  (1 << 12)

/**
 * @}
 */

#endif /* ZEPHYR_INCLUDE_NET_MII_H_ */

// By GST @Data