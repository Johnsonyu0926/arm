// zephyr-3.7-branch/include/zephyr/net/mdio.h

/**
 * @file
 * @brief IEEE 802.3管理接口定义
 */

#ifndef ZEPHYR_INCLUDE_NET_MDIO_H_
#define ZEPHYR_INCLUDE_NET_MDIO_H_

/**
 * @brief IEEE 802.3管理接口定义
 * @defgroup ethernet_mdio IEEE 802.3管理接口
 * @ingroup ethernet
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/** MDIO事务操作码 */
enum mdio_opcode {
	/** IEEE 802.3 22.2.4.5.4 写操作 */
	MDIO_OP_C22_WRITE = 1,

	/** IEEE 802.3 22.2.4.5.4 读操作 */
	MDIO_OP_C22_READ = 2,

	/** IEEE 802.3 45.3.4 地址操作 */
	MDIO_OP_C45_ADDRESS = 0,

	/** IEEE 802.3 45.3.4 写操作 */
	MDIO_OP_C45_WRITE = 1,

	/** IEEE 802.3 45.3.4 读后递增地址操作 */
	MDIO_OP_C45_READ_INC = 2,

	/** IEEE 802.3 45.3.4 读操作 */
	MDIO_OP_C45_READ = 3
};

/* MDIO可管理设备地址 */
/** 物理介质附件/物理介质依赖 */
#define MDIO_MMD_PMAPMD			0x01U
/** 广域网接口子层 */
#define MDIO_MMD_WIS			0x02U
/** 物理编码子层 */
#define MDIO_MMD_PCS			0x03U
/** PHY扩展子层 */
#define MDIO_MMD_PHYXS			0x04U
/** DTE扩展子层 */
#define MDIO_MMD_DTEXS			0x05U
/** 传输汇聚 */
#define MDIO_MMD_TC			0x06U
/** 自动协商 */
#define MDIO_MMD_AN			0x07U
/** 分离的PMA（1） */
#define MDIO_MMD_SEPARATED_PMA1		0x08U
/** 分离的PMA（2） */
#define MDIO_MMD_SEPARATED_PMA2		0x09U
/** 分离的PMA（3） */
#define MDIO_MMD_SEPARATED_PMA3		0x0AU
/** 分离的PMA（4） */
#define MDIO_MMD_SEPARATED_PMA4		0x0BU
/** 第22条扩展 */
#define MDIO_MMD_C22EXT			0x1DU
/** 供应商特定1 */
#define MDIO_MMD_VENDOR_SPECIFIC1	0x1EU
/** 供应商特定2 */
#define MDIO_MMD_VENDOR_SPECIFIC2	0x1FU

/* MDIO通用寄存器 */
/** 控制1 */
#define MDIO_CTRL1			0x0000U
/** 状态1 */
#define MDIO_STAT1			0x0001U
/** 设备标识符（1） */
#define MDIO_DEVID1			0x0002U
/** 设备标识符（2） */
#define MDIO_DEVID2			0x0003U
/** 速度能力 */
#define MDIO_SPEED			0x0004U
/** 包中的设备（1） */
#define MDIO_DEVS1			0x0005U
/** 包中的设备（2） */
#define MDIO_DEVS2			0x0006U
/** 控制2 */
#define MDIO_CTRL2			0x0007U
/** 状态2 */
#define MDIO_STAT2			0x0008U
/** 包标识符（1） */
#define MDIO_PKGID1			0x000EU
/** 包标识符（2） */
#define MDIO_PKGID2			0x000FU

/* BASE-T1寄存器 */
/** BASE-T1自动协商控制 */
#define MDIO_AN_T1_CTRL			0x0200U
/** BASE-T1自动协商状态 */
#define MDIO_AN_T1_STAT			0x0201U
/** BASE-T1自动协商广告寄存器[15:0] */
#define MDIO_AN_T1_ADV_L		0x0202U
/** BASE-T1自动协商广告寄存器[31:16] */
#define MDIO_AN_T1_ADV_M		0x0203U
/** BASE-T1自动协商广告寄存器[47:32] */
#define MDIO_AN_T1_ADV_H		0x0204U
/** BASE-T1 PMA/PMD控制寄存器 */
#define MDIO_PMA_PMD_BT1_CTRL		0x0834U

/* BASE-T1自动协商控制寄存器 */
/** 自动协商重启 */
#define MDIO_AN_T1_CTRL_RESTART		BIT(9)
/** 自动协商启用 */
#define MDIO_AN_T1_CTRL_EN		BIT(12)

/* BASE-T1自动协商状态寄存器 */
/** 链路状态 */
#define MDIO_AN_T1_STAT_LINK_STATUS	BIT(2)
/** 自动协商能力 */
#define MDIO_AN_T1_STAT_ABLE		BIT(3)
/** 自动协商远程故障 */
#define MDIO_AN_T1_STAT_REMOTE_FAULT	BIT(4)
/** 自动协商完成 */
#define MDIO_AN_T1_STAT_COMPLETE	BIT(5)
/** 页面接收 */
#define MDIO_AN_T1_STAT_PAGE_RX		BIT(6)

/* BASE-T1自动协商广告寄存器[15:0] */
/** 暂停能力 */
#define MDIO_AN_T1_ADV_L_PAUSE_CAP	BIT(10)
/** 暂停能力 */
#define MDIO_AN_T1_ADV_L_PAUSE_ASYM	BIT(11)
/** 强制主/从配置 */
#define MDIO_AN_T1_ADV_L_FORCE_MS	BIT(12)
/** 远程故障 */
#define MDIO_AN_T1_ADV_L_REMOTE_FAULT	BIT(13)
/** 确认（ACK） */
#define MDIO_AN_T1_ADV_L_ACK		BIT(14)
/** 下一页请求 */
#define MDIO_AN_T1_ADV_L_NEXT_PAGE_REQ	BIT(15)

/* BASE-T1自动协商广告寄存器[31:16] */
/** 10BASE-T1L能力 */
#define MDIO_AN_T1_ADV_M_B10L		BIT(14)
/** 主/从配置 */
#define MDIO_AN_T1_ADV_M_MST		BIT(4)

/* BASE-T1自动协商广告寄存器[47:32] */
/** 10BASE-T1L高电平传输操作模式请求 */
#define MDIO_AN_T1_ADV_H_10L_TX_HI_REQ	BIT(12)
/** 10BASE-T1L高电平传输操作模式能力 */
#define MDIO_AN_T1_ADV_H_10L_TX_HI	BIT(13)

/* BASE-T1 PMA/PMD控制寄存器 */
/** BASE-T1主/从配置 */
#define MDIO_PMA_PMD_BT1_CTRL_CFG_MST	BIT(14)

/* 10BASE-T1L寄存器 */
/** 10BASE-T1L PMA控制 */
#define MDIO_PMA_B10L_CTRL		0x08F6U
/** 10BASE-T1L PMA状态 */
#define MDIO_PMA_B10L_STAT		0x08F7U
/** 10BASE-T1L PMA链路状态 */
#define MDIO_PMA_B10L_LINK_STAT		0x8302U
/** 10BASE-T1L PCS控制 */
#define MDIO_PCS_B10L_CTRL		0x08E6U
/** 10BASE-T1L PCS状态 */
#define MDIO_PCS_B10L_STAT		0x08E7U

/* 10BASE-T1L PMA控制寄存器 */
/** 10BASE-T1L传输禁用模式 */
#define MDIO_PMA_B10L_CTRL_TX_DIS_MODE_EN		BIT(14)
/** 10BASE-T1L传输电压幅度控制 */
#define MDIO_PMA_B10L_CTRL_TX_LVL_HI			BIT(12)
/** 10BASE-T1L EEE启用 */
#define MDIO_PMA_B10L_CTRL_EEE				BIT(10)
/** 10BASE-T1L PMA环回 */
#define MDIO_PMA_B10L_CTRL_LB_PMA_LOC_EN		BIT(0)

/* 10BASE-T1L PMA状态寄存器 */
/** 10BASE-T1L PMA接收链路正常 */
#define MDIO_PMA_B10L_STAT_LINK				BIT(0)
/** 10BASE-T1L检测到故障条件 */
#define MDIO_PMA_B10L_STAT_FAULT			BIT(1)
/** 10BASE-T1L接收极性反转 */
#define MDIO_PMA_B10L_STAT_POLARITY			BIT(2)
/** 10BASE-T1L能够检测接收路径上的故障 */
#define MDIO_PMA_B10L_STAT_RECV_FAULT			BIT(9)
/** 10BASE-T1L PHY具有EEE能力 */
#define MDIO_PMA_B10L_STAT_EEE				BIT(10)
/** 10BASE-T1L PMA具有低功耗能力 */
#define MDIO_PMA_B10L_STAT_LOW_POWER			BIT(11)
/** 10BASE-T1L PHY具有2.4 Vpp操作模式能力 */
#define MDIO_PMA_B10L_STAT_2V4_ABLE			BIT(12)
/** 10BASE-T1L PHY具有环回能力 */
#define MDIO_PMA_B10L_STAT_LB_ABLE			BIT(13)

/* 10BASE-T1L PMA链路状态 */
/** 10BASE-T1L远程接收器状态正常锁存低 */
#define MDIO_PMA_B10L_LINK_STAT_REM_RCVR_STAT_OK_LL	BIT(9)
/** 10BASE-T1L远程接收器状态正常 */
#define MDIO_PMA_B10L_LINK_STAT_REM_RCVR_STAT_OK	BIT(8)
/** 10BASE-T1L本地接收器状态正常 */
#define MDIO_PMA_B10L_LINK_STAT_LOC_RCVR_STAT_OK_LL	BIT(7)
/** 10BASE-T1L本地接收器状态正常 */
#define MDIO_PMA_B10L_LINK_STAT_LOC_RCVR_STAT_OK	BIT(6)
/** 10BASE-T1L解扰器状态正常锁存低 */
#define MDIO_PMA_B10L_LINK_STAT_DSCR_STAT_OK_LL		BIT(5)
/** 10BASE-T1L解扰器状态正常 */
#define MDIO_PMA_B10L_LINK_STAT_DSCR_STAT_OK		BIT(4)
/** 10BASE-T1L链路状态正常锁存低 */
#define MDIO_PMA_B10L_LINK_STAT_LINK_STAT_OK_LL		BIT(1)
/** 10BASE-T1L链路状态正常 */
#define MDIO_PMA_B10L_LINK_STAT_LINK_STAT_OK		BIT(0)

/* 10BASE-T1L PCS控制 */
/** 10BASE-T1L PCS环回启用 */
#define MDIO_PCS_B10L_CTRL_LB_PCS_EN			BIT(14)

/* 10BASE-T1L PCS状态 */
/** 10BASE-T1L PCS解扰器状态 */
#define MDIO_PCS_B10L_STAT_DSCR_STAT_OK_LL		BIT(2)

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* ZEPHYR_INCLUDE_NET_MDIO_H_ */

// By GST @Data