/* SPDX-License-Identifier: GPL-2.0 */
/*
 * (C) Copyright 2024 Rockchip Electronics Co., Ltd
 */

#ifndef _RK628_H_
#define _RK628_H_

#include <asm-generic/gpio.h>
#include <errno.h>
#include <common.h>
#include <dm.h>
#include <i2c.h>
#include <dm/uclass.h>
#include <dm/uclass-id.h>
#include <power/regulator.h>
#include <linux/bitfield.h>
#include <linux/math64.h>
#include <drm_modes.h>

#include "../rockchip_phy.h"

#define DRIVER_VERSION				"0.1.0"
#define UPDATE(x, h, l)		(((x) << (l)) & GENMASK((h), (l)))
#define HIWORD_UPDATE(v, h, l)	((((v) << (l)) & GENMASK((h), (l))) | \
				 (GENMASK((h), (l)) << 16))

#define GRF_SYSTEM_CON0			0x0000
#define SW_VSYNC_POL_MASK		BIT(26)
#define SW_VSYNC_POL(x)			UPDATE(x, 26, 26)
#define SW_HSYNC_POL_MASK		BIT(25)
#define SW_HSYNC_POL(x)			UPDATE(x, 25, 25)
#define SW_ADAPTER_I2CSLADR_MASK	GENMASK(24, 22)
#define SW_ADAPTER_I2CSLADR(x)		UPDATE(x, 24, 22)
#define SW_EDID_MODE_MASK		BIT(21)
#define SW_EDID_MODE(x)			UPDATE(x, 21, 21)
#define SW_I2S_DATA_OEN_MASK		BIT(10)
#define SW_I2S_DATA_OEN(x)		UPDATE(x, 10, 10)
#define SW_BT_DATA_OEN_MASK		BIT(9)
#define SW_BT_DATA_OEN			BIT(9)
#define SW_EFUSE_HDCP_EN_MASK		BIT(8)
#define SW_EFUSE_HDCP_EN(x)		UPDATE(x, 8, 8)
#define SW_OUTPUT_MODE_MASK		GENMASK(5, 3)
#define SW_OUTPUT_MODE(x)		UPDATE(x, 5, 3)
/* compatible with rk628f */
#define SW_OUTPUT_RGB_MODE_MASK		GENMASK(7, 6)
#define SW_OUTPUT_RGB_MODE(x)		UPDATE(x, 7, 6)
#define SW_HDMITX_EN_MASK		BIT(5)
#define SW_HDMITX_EN(x)			UPDATE(x, 5, 5)
#define SW_OUTPUT_COMBTX_MODE_MASK	GENMASK(4, 3)
#define SW_OUTPUT_COMBTX_MODE(x)	UPDATE(x, 4, 3)

#define SW_INPUT_MODE_MASK		GENMASK(2, 0)
#define SW_INPUT_MODE(x)		UPDATE(x, 2, 0)
#define GRF_SYSTEM_CON1			0x0004
#define GRF_SYSTEM_CON2			0x0008
#define GRF_SYSTEM_CON3			0x000c
#define GRF_GPIO_RX_CEC_SEL_MASK	BIT(7)
#define GRF_GPIO_RX_CEC_SEL(x)		UPDATE(x, 7, 7)
#define GRF_GPIO_RXDDC_SDA_SEL_MASK	BIT(6)
#define GRF_GPIO_RXDDC_SDA_SEL(x)	UPDATE(x, 6, 6)
#define GRF_GPIO_RXDDC_SCL_SEL_MASK	BIT(5)
#define GRF_GPIO_RXDDC_SCL_SEL(x)	UPDATE(x, 5, 5)
#define GRF_DPHY_CH1_EN_MASK		BIT(1)
#define GRF_DPHY_CH1_EN(x)		UPDATE(x, 1, 1)
#define GRF_AS_DSIPHY_MASK		BIT(0)
#define GRF_AS_DSIPHY(x)		UPDATE(x, 0, 0)
#define GRF_SCALER_CON0			0x0010
#define SCL_8_PIXEL_ALIGN(x)		HIWORD_UPDATE(x, 12, 12)
#define SCL_COLOR_VER_EN(x)		HIWORD_UPDATE(x, 10, 10)
#define SCL_COLOR_BAR_EN(x)		HIWORD_UPDATE(x, 9, 9)
#define SCL_VER_DOWN_MODE(x)		HIWORD_UPDATE(x, 8, 8)
#define SCL_HOR_DOWN_MODE(x)		HIWORD_UPDATE(x, 7, 7)
#define SCL_BIC_COE_SEL(x)		HIWORD_UPDATE(x, 6, 5)
#define SCL_VER_MODE(x)			HIWORD_UPDATE(x, 4, 3)
#define SCL_HOR_MODE(x)			HIWORD_UPDATE(x, 2, 1)
#define SCL_EN(x)			HIWORD_UPDATE(x, 0, 0)
#define GRF_SCALER_CON1			0x0014
#define SCL_V_FACTOR(x)			UPDATE(x, 31, 16)
#define SCL_H_FACTOR(x)			UPDATE(x, 15, 0)
#define GRF_SCALER_CON2			0x0018
#define DSP_FRAME_VST(x)		UPDATE(x, 28, 16)
#define DSP_FRAME_HST(x)		UPDATE(x, 12, 0)
#define GRF_SCALER_CON3			0x001c
#define DSP_HS_END(x)			UPDATE(x, 23, 16)
#define DSP_HTOTAL(x)			UPDATE(x, 12, 0)
#define GRF_SCALER_CON4			0x0020
#define DSP_HACT_ST(x)			UPDATE(x, 28, 16)
#define DSP_HACT_END(x)			UPDATE(x, 12, 0)
#define GRF_SCALER_CON5			0x0024
#define DSP_VS_END(x)			UPDATE(x, 23, 16)
#define DSP_VTOTAL(x)			UPDATE(x, 12, 0)
#define GRF_SCALER_CON6			0x0028
#define DSP_VACT_ST(x)			UPDATE(x, 28, 16)
#define DSP_VACT_END(x)			UPDATE(x, 12, 0)
#define GRF_SCALER_CON7			0x002c
#define DSP_HBOR_ST(x)			UPDATE(x, 28, 16)
#define DSP_HBOR_END(x)			UPDATE(x, 12, 0)
#define GRF_SCALER_CON8			0x0030
#define DSP_VBOR_ST(x)			UPDATE(x, 28, 16)
#define DSP_VBOR_END(x)			UPDATE(x, 12, 0)
#define GRF_POST_PROC_CON		0x0034
#define SW_HDMITX_VSYNC_POL		BIT(17)
#define SW_HDMITX_HSYNC_POL		BIT(16)
#define SW_DCLK_OUT_INV_EN		BIT(9)
#define SW_DCLK_IN_INV_EN		BIT(8)
#define SW_TXPHY_REFCLK_SEL_MASK	GENMASK(6, 5)
#define SW_TXPHY_REFCLK_SEL(x)		UPDATE(x, 6, 5)
#define SW_HDMITX_VCLK_PLLREF_SEL_MASK	BIT(4)
#define SW_HDMITX_VCLK_PLLREF_SEL(x)	UPDATE(x, 4, 4)
#define SW_HDMITX_DCLK_INV_EN		BIT(3)
#define SW_SPLIT_MODE(x)		UPDATE(x, 1, 1)
#define SW_SPLIT_EN			BIT(0)
#define GRF_CSC_CTRL_CON		0x0038
#define SW_Y2R_MODE(x)			HIWORD_UPDATE(x, 13, 12)
#define SW_FROM_CSC_MATRIX_EN(x)	HIWORD_UPDATE(x, 11, 11)
#define SW_YUV2VYU_SWP(x)		HIWORD_UPDATE(x, 8, 8)
#define SW_R2Y_EN(x)			HIWORD_UPDATE(x, 4, 4)
#define SW_Y2R_EN(x)			HIWORD_UPDATE(x, 0, 0)
#define GRF_LVDS_TX_CON			0x003c
#define SW_LVDS_CON_DUAL_SEL(x)		HIWORD_UPDATE(x, 12, 12)
#define SW_LVDS_CON_DEN_POLARITY(x)	HIWORD_UPDATE(x, 11, 11)
#define SW_LVDS_CON_HS_POLARITY(x)	HIWORD_UPDATE(x, 10, 10)
#define SW_LVDS_CON_CLKINV(x)		HIWORD_UPDATE(x, 9, 9)
#define SW_LVDS_STARTPHASE(x)		HIWORD_UPDATE(x, 8, 8)
#define SW_LVDS_CON_STARTSEL(x)		HIWORD_UPDATE(x, 7, 7)
#define SW_LVDS_CON_CHASEL(x)		HIWORD_UPDATE(x, 6, 6)
#define SW_LVDS_TIE_VSYNC_VALUE(x)	HIWORD_UPDATE(x, 5, 5)
#define SW_LVDS_TIE_HSYNC_VALUE(x)	HIWORD_UPDATE(x, 4, 4)
#define SW_LVDS_TIE_DEN_ONLY(x)		HIWORD_UPDATE(x, 3, 3)
#define SW_LVDS_CON_MSBSEL(x)		HIWORD_UPDATE(x, 2, 2)
#define SW_LVDS_CON_SELECT(x)		HIWORD_UPDATE(x, 1, 0)
#define GRF_RGB_DEC_CON0		0x0040
#define SW_HRES_MASK			GENMASK(28, 16)
#define SW_HRES(x)			UPDATE(x, 28, 16)
#define DUAL_DATA_SWAP			BIT(6)
#define DEC_DUALEDGE_EN			BIT(5)
#define SW_PROGRESS_EN			BIT(4)
#define SW_BT1120_YC_SWAP		BIT(3)
#define SW_BT1120_UV_SWAP		BIT(2)
#define SW_CAP_EN_ASYNC			BIT(1)
#define SW_CAP_EN_PSYNC			BIT(0)
#define GRF_RGB_DEC_CON1		0x0044
#define SW_SET_X_MASK			GENMASK(28, 16)
#define SW_SET_X(x)			HIWORD_UPDATE(x, 28, 16)
#define SW_SET_Y_MASK			GENMASK(28, 16)
#define SW_SET_Y(x)			HIWORD_UPDATE(x, 28, 16)
#define GRF_RGB_DEC_CON2		0x0048
#define GRF_RGB_ENC_CON			0x004c
#define BT1120_UV_SWAP(x)		HIWORD_UPDATE(x, 5, 5)
#define ENC_DUALEDGE_EN(x)		HIWORD_UPDATE(x, 3, 3)
#define GRF_MIPI_LANE_DELAY_CON0	0x0050
#define GRF_MIPI_LANE_DELAY_CON1	0x0054
#define GRF_BT1120_DCLK_DELAY_CON0	0x0058
#define GRF_BT1120_DCLK_DELAY_CON1	0x005c
#define GRF_MIPI_TX0_CON		0x0060
#define DPIUPDATECFG			BIT(26)
#define DPICOLORM			BIT(25)
#define DPISHUTDN			BIT(24)
#define CSI_PHYRSTZ			BIT(21)
#define CSI_PHYSHUTDOWNZ		BIT(20)
#define FORCETXSTOPMODE_MASK		GENMASK(19, 16)
#define FORCETXSTOPMODE(x)		UPDATE(x, 19, 16)
#define FORCERXMODE_MASK		GENMASK(15, 12)
#define FORCERXMODE(x)			UPDATE(x, 15, 12)
#define PHY_TESTCLR			BIT(10)
#define PHY_TESTCLK			BIT(9)
#define PHY_TESTEN			BIT(8)
#define PHY_TESTDIN_MASK		GENMASK(7, 0)
#define PHY_TESTDIN(x)			UPDATE(x, 7, 0)
#define GRF_DPHY0_STATUS		0x0064
#define DPHY_PHYLOCK			BIT(24)
#define PHY_TESTDOUT_SHIFT		8
#define GRF_MIPI_TX1_CON		0x0068
#define GRF_DPHY1_STATUS		0x006c
#define GRF_GPIO0AB_SEL_CON		0x0070
#define GRF_GPIO1AB_SEL_CON		0x0074
#define GRF_GPIO2AB_SEL_CON		0x0078
#define GRF_GPIO2C_SEL_CON		0x007c
#define GRF_GPIO3AB_SEL_CON		0x0080
#define GRF_GPIO2A_SMT			0x0090
#define GRF_GPIO2B_SMT			0x0094
#define GRF_GPIO2C_SMT			0x0098
#define GRF_GPIO3AB_SMT			0x009c
#define GRF_GPIO0A_P_CON		0x00a0
#define GRF_GPIO1A_P_CON		0x00a4
#define GRF_GPIO2A_P_CON		0x00a8
#define GRF_GPIO2B_P_CON		0x00ac
#define GRF_GPIO2C_P_CON		0x00b0
#define GRF_GPIO3A_P_CON		0x00b4
#define GRF_GPIO3B_P_CON		0x00b8
#define GRF_GPIO0B_D_CON		0x00c0
#define GRF_GPIO1B_D_CON		0x00c4
#define GRF_GPIO2A_D0_CON		0x00c8
#define GRF_GPIO2A_D1_CON		0x00cc
#define GRF_GPIO2B_D0_CON		0x00d0
#define GRF_GPIO2B_D1_CON		0x00d4
#define GRF_GPIO2C_D0_CON		0x00d8
#define GRF_GPIO2C_D1_CON		0x00dc
#define GRF_GPIO3A_D0_CON		0x00e0
#define GRF_GPIO3A_D1_CON		0x00e4
#define GRF_GPIO3B_D_CON		0x00e8
#define GRF_GPIO_SR_CON			0x00ec
#define GRF_SW_HDMIRXPHY_CRTL		0x00f4
#define GRF_INTR0_EN			0x0100
#define RK628F_HDMIRX_IRQ_EN(x)		HIWORD_UPDATE(x, 9, 9)
#define RK628D_HDMIRX_IRQ_EN(x)		HIWORD_UPDATE(x, 8, 8)
#define GRF_INTR0_CLR_EN		0x0104
#define GRF_INTR0_STATUS		0x0108
#define GRF_INTR0_RAW_STATUS		0x010c
#define GRF_INTR1_EN			0x0110
#define GRF_INTR1_CLR_EN		0x0114
#define GRF_INTR1_STATUS		0x0118
#define GRF_INTR1_RAW_STATUS		0x011c
#define GRF_SYSTEM_STATUS0		0x0120
/* 0: i2c mode and mcu mode; 1: i2c mode only */
#define I2C_ONLY_FLAG			BIT(6)
#define GRF_SYSTEM_STATUS3		0x012c
#define DECODER_1120_LAST_LINE_NUM_MASK	GENMASK(12, 0)
#define GRF_SYSTEM_STATUS4		0x0130
#define DECODER_1120_LAST_PIX_NUM_MASK	GENMASK(12, 0)
#define GRF_OS_REG0			0x0140
#define GRF_OS_REG1			0x0144
#define GRF_OS_REG2			0x0148
#define GRF_OS_REG3			0x014c
#define GRF_RGB_RX_DBG_MEAS0		0x0170
#define RGB_RX_EVAL_TIME_MASK		GENMASK(27, 16)
#define RGB_RX_MODET_EN			BIT(1)
#define RGB_RX_DCLK_EN			BIT(0)
#define GRF_RGB_RX_DBG_MEAS2		0x0178
#define RGB_RX_CLKRATE_MASK		GENMASK(15, 0)
#define GRF_RGB_RX_DBG_MEAS3		0x017c
#define RGB_RX_CNT_EN_MASK		BIT(0)
#define RGB_RX_CNT_EN(x)		UPDATE(x, 0, 0)
#define GRF_RGB_RX_DBG_MEAS4		0x0180
#define GRF_BT1120_TIMING_CTRL0		0x0190
#define BT1120_DSP_HS_END(x)		UPDATE(x, 28, 16)
#define BT1120_DSP_HTOTAL(x)		UPDATE(x, 12, 0)
#define GRF_BT1120_TIMING_CTRL1		0x0194
#define BT1120_DSP_HACT_ST(x)		UPDATE(x, 28, 16)
#define GRF_BT1120_TIMING_CTRL2		0x0198
#define	BT1120_DSP_VS_END(x)		UPDATE(x, 28, 16)
#define BT1120_DSP_VTOTAL(x)		UPDATE(x, 12, 0)
#define GRF_BT1120_TIMING_CTRL3		0x019c
#define BT1120_DSP_VACT_ST(x)		UPDATE(x, 28, 16)
#define GRF_CSC_MATRIX_COE01_COE00	0x01a0
#define GRF_CSC_MATRIX_COE10_COE02	0x01a4
#define GRF_CSC_MATRIX_COE12_COE11	0x01a8
#define GRF_CSC_MATRIX_COE21_COE20	0x01ac
#define GRF_CSC_MATRIX_COE22		0x01b0
#define GRF_CSC_MATRIX_OFFSET0		0x01b4
#define GRF_CSC_MATRIX_OFFSET1		0x01b8
#define GRF_CSC_MATRIX_OFFSET2		0x01bc
#define GRF_SOC_VERSION			0x0200
#define GRF_OBS_REG			0X0300
#define GRF_MAX_REGISTER		GRF_OBS_REG

#define RK628D_VERSION 0x20200326
#define RK628F_VERSION 0x20230321

enum {
	COMBTXPHY_MODULEA_EN = BIT(0),
	COMBTXPHY_MODULEB_EN = BIT(1),
};

enum {
	RK628_DEV_GRF,
	RK628_DEV_COMBRXPHY,
	RK628_DEV_HDMIRX = 3,
	RK628_DEV_CSI,
	RK628_DEV_DSI0,
	RK628_DEV_DSI1,
	RK628_DEV_HDMITX,
	RK628_DEV_GVI,
	RK628_DEV_COMBTXPHY,
	RK628_DEV_ADAPTER,
	RK628_DEV_EFUSE,
	RK628_DEV_CRU,
	RK628_DEV_GPIO0,
	RK628_DEV_GPIO1,
	RK628_DEV_GPIO2,
	RK628_DEV_GPIO3,
	RK628_DEV_MAX,
};

enum rk628_input_mode {
	INPUT_MODE_HDMI,
	INPUT_MODE_BT1120 = 2,
	INPUT_MODE_RGB,
	INPUT_MODE_YUV,
};


enum rk628_output_mode {
	OUTPUT_MODE_GVI = 1,
	OUTPUT_MODE_LVDS,
	OUTPUT_MODE_HDMI,
	OUTPUT_MODE_CSI,
	OUTPUT_MODE_DSI,
	OUTPUT_MODE_BT1120 = 8,
	OUTPUT_MODE_RGB = 16,
	OUTPUT_MODE_YUV = 24,
};

enum rk628_phy_mode {
	RK628_PHY_MODE_INVALID,
	RK628_PHY_MODE_VIDEO_MIPI,
	RK628_PHY_MODE_VIDEO_LVDS,
	RK628_PHY_MODE_VIDEO_GVI,
};

enum lvds_format {
	LVDS_FORMAT_VESA_24BIT,
	LVDS_FORMAT_JEIDA_24BIT,
	LVDS_FORMAT_JEIDA_18BIT,
	LVDS_FORMAT_VESA_18BIT,
};

enum lvds_link_type {
	LVDS_SINGLE_LINK,
	LVDS_DUAL_LINK_ODD_EVEN_PIXELS,
	LVDS_DUAL_LINK_EVEN_ODD_PIXELS,
	LVDS_DUAL_LINK_LEFT_RIGHT_PIXELS,
	LVDS_DUAL_LINK_RIGHT_LEFT_PIXELS,
};

enum gvi_color_depth {
	COLOR_DEPTH_RGB_YUV444_18BIT,
	COLOR_DEPTH_RGB_YUV444_24BIT,
	COLOR_DEPTH_RGB_YUV444_30BIT,
	COLOR_DEPTH_YUV422_16BIT = 8,
	COLOR_DEPTH_YUV422_20BIT,
};

enum dsi_mode_flags {
	RK628_MIPI_DSI_MODE_VIDEO = 1,
	RK628_MIPI_DSI_MODE_VIDEO_BURST = 2,
	RK628_MIPI_DSI_MODE_VIDEO_SYNC_PULSE = 4,
	RK628_MIPI_DSI_MODE_VIDEO_HFP = 8,
	RK628_MIPI_DSI_MODE_VIDEO_HBP = 16,
	RK628_MIPI_DSI_MODE_EOT_PACKET = 32,
	RK628_MIPI_DSI_CLOCK_NON_CONTINUOUS = 64,
	RK628_MIPI_DSI_MODE_LPM = 128,
};

enum dsi_bus_format {
	RK628_MIPI_DSI_FMT_RGB888,
	RK628_MIPI_DSI_FMT_RGB666,
	RK628_MIPI_DSI_FMT_RGB666_PACKED,
	RK628_MIPI_DSI_FMT_RGB565,
};

enum gvi_bus_format {
	GVI_MEDIA_BUS_FMT_RGB666_1X18 = 9,
	GVI_MEDIA_BUS_FMT_RGB888_1X24 = 10,
	GVI_MEDIA_BUS_FMT_YUYV10_1X20 = 13,
	GVI_MEDIA_BUS_FMT_YUYV8_1X16 = 17,
	GVI_MEDIA_BUS_FMT_RGB101010_1X30 = 24,
};

enum bus_format {
	BUS_FMT_RGB = 0,
	BUS_FMT_YUV422 = 1,
	BUS_FMT_YUV444 = 2,
	BUS_FMT_YUV420 = 3,
	BUS_FMT_UNKNOWN,
};

enum rk628_mode_sync_pol {
	MODE_FLAG_NSYNC,
	MODE_FLAG_PSYNC,
};

struct rk628_videomode {
	u32 pixelclock;	/* pixelclock in Hz */

	u32 hactive;
	u32 hfront_porch;
	u32 hback_porch;
	u32 hsync_len;

	u32 vactive;
	u32 vfront_porch;
	u32 vback_porch;
	u32 vsync_len;

	unsigned int flags; /* display flags */
};

struct rk628_display_mode {
	int clock; /* in kHz */
	int hdisplay;
	int hsync_start;
	int hsync_end;
	int htotal;
	int vdisplay;
	int vsync_start;
	int vsync_end;
	int vtotal;
	unsigned int flags;
};

struct cmd_ctrl_hdr {
	u8 dtype;       /* data type */
	u8 wait;        /* ms */
	u8 dlen;        /* payload len */
} __packed;

struct cmd_desc {
	struct cmd_ctrl_hdr dchdr;
	u8 *payload;
};

struct panel_cmds {
	u8 *buf;
	unsigned int blen;
	struct cmd_desc *cmds;
	int cmd_cnt;
};

struct rk628_panel_simple {
	struct udevice *backlight;

	struct udevice *supply;
	struct gpio_desc enable_gpio;
	struct gpio_desc reset_gpio;
	struct panel_cmds *on_cmds;
	struct panel_cmds *off_cmds;

	struct {
		unsigned int prepare;
		unsigned int enable;
		unsigned int disable;
		unsigned int unprepare;
		unsigned int reset;
		unsigned int init;
	} delay;
};

struct rk628_dsi {
	int bpp; /* 24/18/16*/
	enum dsi_bus_format bus_format;
	enum dsi_mode_flags mode_flags;
	bool slave;
	bool master;
	uint8_t channel;
	uint8_t  lanes;
	uint8_t  id; /* 0:dsi0 1:dsi1 */
	struct rk628 *rk628;
	unsigned int lane_mbps; /* per lane */
};

struct rk628_lvds {
	enum lvds_format format;
	enum lvds_link_type link_type;
};

struct rk628_gvi {
	enum gvi_bus_format bus_format;
	enum gvi_color_depth color_depth;
	int retry_times;
	uint8_t lanes;
	bool division_mode;
	bool frm_rst;
	u8 byte_mode;
};

struct rk628_combtxphy {
	enum rk628_phy_mode mode;
	unsigned int flags;
	u8 ref_div;
	u8 fb_div;
	u16 frac_div;
	u8 rate_div;
	u32 bus_width;
	bool division_mode;
};

struct rk628_rgb {
	struct udevice *vccio_rgb;
	bool bt1120_dual_edge;
	bool bt1120_yc_swap;
	bool bt1120_uv_swap;
};

struct rk628_hdmi_mode {
	u32 hdisplay;
	u32 hstart;
	u32 hend;
	u32 htotal;
	u32 vdisplay;
	u32 vstart;
	u32 vend;
	u32 vtotal;
	u32 clock;
	unsigned int flags;
};

struct rk628_hdmirx {
	struct udevice *dev;
	struct rk628 *parent;
	struct rk628_hdmi_mode mode;
	bool src_mode_4K_yuv420;
	bool src_depth_10bit;
	bool phy_lock;
	bool is_hdmi2;
	u32 input_format;
};

struct rk628 {
	struct udevice *dev;
	struct udevice *power_supply;
	struct gpio_desc reset_gpio;
	struct gpio_desc enable_gpio;
	struct rk628_panel_simple *panel;
	bool display_enabled;
	u32 input_mode;
	u32 output_mode;
	struct drm_display_mode src_mode;
	struct drm_display_mode dst_mode;
	enum bus_format input_fmt;
	enum bus_format output_fmt;
	struct rk628_hdmirx hdmirx;
	struct rk628_dsi dsi0;
	struct rk628_dsi dsi1;
	struct rk628_lvds lvds;
	struct rk628_gvi gvi;
	struct rk628_combtxphy combtxphy;
	int sync_pol;
	u32 version;
	struct rk628_rgb rgb;
};

static inline bool rk628_input_is_hdmi(struct rk628 *rk628)
{
	return rk628->input_mode & BIT(INPUT_MODE_HDMI);
}

static inline bool rk628_input_is_rgb(struct rk628 *rk628)
{
	return rk628->input_mode & BIT(INPUT_MODE_RGB);
}

static inline bool rk628_input_is_bt1120(struct rk628 *rk628)
{
	return rk628->input_mode & BIT(INPUT_MODE_BT1120);
}

static inline bool rk628_output_is_rgb(struct rk628 *rk628)
{
	return rk628->output_mode & BIT(OUTPUT_MODE_RGB);
}

static inline bool rk628_output_is_bt1120(struct rk628 *rk628)
{
	return rk628->output_mode & BIT(OUTPUT_MODE_BT1120);
}

static inline bool rk628_output_is_gvi(struct rk628 *rk628)
{
	return rk628->output_mode & BIT(OUTPUT_MODE_GVI);
}

static inline bool rk628_output_is_lvds(struct rk628 *rk628)
{
	return rk628->output_mode & BIT(OUTPUT_MODE_LVDS);
}

static inline bool rk628_output_is_dsi(struct rk628 *rk628)
{
	return rk628->output_mode & BIT(OUTPUT_MODE_DSI);
}

static inline bool rk628_output_is_csi(struct rk628 *rk628)
{
	return rk628->output_mode & BIT(OUTPUT_MODE_CSI);
}

static inline bool rk628_output_is_hdmi(struct rk628 *rk628)
{
	return rk628->output_mode & BIT(OUTPUT_MODE_HDMI);
}

static inline void rk628_set_input_bus_format(struct rk628 *rk628, enum bus_format format)
{
	rk628->input_fmt = format;
}

static inline enum bus_format rk628_get_input_bus_format(struct rk628 *rk628)
{
	return rk628->input_fmt;
}

static inline void rk628_set_output_bus_format(struct rk628 *rk628, enum bus_format format)
{
	rk628->output_fmt = format;
}

static inline enum bus_format rk628_get_output_bus_format(struct rk628 *rk628)
{
	return rk628->output_fmt;
}

static inline int rk628_i2c_write(struct rk628 *rk628, u32 reg, u32 val)
{
	struct dm_i2c_chip *chip = dev_get_parent_platdata(rk628->dev);
	struct i2c_msg msg;
	u8 buf[] = {
		(reg >> 0) & 0xff, (reg >> 8) & 0xff,
		(reg >> 16) & 0xff, (reg >> 24) & 0xff,
		(val >> 0) & 0xff, (val >> 8) & 0xff,
		(val >> 16) & 0xff, (val >> 24) & 0xff
	};
	int ret;

	msg.addr = chip->chip_addr;
	msg.flags = 0;
	msg.len = sizeof(buf);
	msg.buf = buf;

	ret = dm_i2c_xfer(rk628->dev, &msg, 1);
	if (ret) {
		dev_err(rk628->dev, "Could not execute transfer: %d\n", ret);
		return ret;
	}

	return 0;
}

static inline int rk628_i2c_read(struct rk628 *rk628, u32 reg, u32 *val)
{
	struct dm_i2c_chip *chip = dev_get_parent_platdata(rk628->dev);
	u32 data;
	struct i2c_msg msg[] = {
		{
			.addr = chip->chip_addr,
			.flags = 0,
			.buf = (u8 *)&reg,
			.len = 4,
		}, {
			.addr = chip->chip_addr,
			.flags = I2C_M_RD,
			.buf = (u8 *)&data,
			.len = 4,
		}
	};
	int ret;

	ret = dm_i2c_xfer(rk628->dev, msg, 2);
	if (ret) {
		dev_err(rk628->dev, "Could not execute transfer: %d\n", ret);
		return ret;
	}

	*val = data;

	return 0;
}

static inline int rk628_i2c_update_bits(struct rk628 *rk628, u32 reg, u32 mask, u32 val)
{
	u32 tmp, orig;
	int ret;

	ret = rk628_i2c_read(rk628, reg, &orig);
	if (ret)
		return ret;

	tmp = orig & ~mask;
	tmp |= val & mask;

	return rk628_i2c_write(rk628, reg, tmp);
}

#define rk628_read_poll_timeout(rk628, addr, val, cond, sleep_us, timeout_us) \
({ \
	unsigned long timeout = timer_get_us() + (timeout_us); \
	for (;;) { \
		rk628_i2c_read(rk628, addr, &val); \
		if (cond) \
			break; \
		if ((timeout_us) && time_after(timer_get_us(), timeout)) { \
			rk628_i2c_read(rk628, addr, &val); \
			break; \
		} \
		if (sleep_us) \
			udelay(sleep_us); \
	} \
	(cond) ? 0 : -ETIMEDOUT; \
})

#endif
