/*
 * Copyright (c) 2023 Ambiq Micro Inc. <www.ambiq.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __APOLLO3_PINCTRL_H__
#define __APOLLO3_PINCTRL_H__

#define APOLLO3_ALT_FUNC_POS  0
#define APOLLO3_ALT_FUNC_MASK 0xf

#define APOLLO3_PIN_NUM_POS  4
#define APOLLO3_PIN_NUM_MASK 0x7f

#define APOLLO3_PINMUX(pin_num, alt_func)                                                          \
	(pin_num << APOLLO3_PIN_NUM_POS | alt_func << APOLLO3_ALT_FUNC_POS)

#define SLSCL_P0         APOLLO3_PINMUX(0, 0)
#define SLSCK_P0         APOLLO3_PINMUX(0, 1)
#define CLKOUT_P0        APOLLO3_PINMUX(0, 2)
#define GPIO_P0          APOLLO3_PINMUX(0, 3)
#define MSPI0_4_P0       APOLLO3_PINMUX(0, 5)
#define NCE0_P0          APOLLO3_PINMUX(0, 7)
#define SLSDAWIR3_P1     APOLLO3_PINMUX(1, 0)
#define SLMOSI_P1        APOLLO3_PINMUX(1, 1)
#define UART0TX_P1       APOLLO3_PINMUX(1, 2)
#define GPIO_P1          APOLLO3_PINMUX(1, 3)
#define MSPI0_5_P1       APOLLO3_PINMUX(1, 5)
#define NCE1_P1          APOLLO3_PINMUX(1, 7)
#define UART1RX_P2       APOLLO3_PINMUX(2, 0)
#define SLMISO_P2        APOLLO3_PINMUX(2, 1)
#define UART0RX_P2       APOLLO3_PINMUX(2, 2)
#define GPIO_P2          APOLLO3_PINMUX(2, 3)
#define MSPI0_6_P2       APOLLO3_PINMUX(2, 5)
#define NCE2_P2          APOLLO3_PINMUX(2, 7)
#define UA0RTS_P3        APOLLO3_PINMUX(3, 0)
#define SLNCE_P3         APOLLO3_PINMUX(3, 1)
#define NCE3_P3          APOLLO3_PINMUX(3, 2)
#define GPIO_P3          APOLLO3_PINMUX(3, 3)
#define MSPI0_7_P3       APOLLO3_PINMUX(3, 5)
#define TRIG1_P3         APOLLO3_PINMUX(3, 6)
#define I2SWCLK_P3       APOLLO3_PINMUX(3, 7)
#define UA0CTS_P4        APOLLO3_PINMUX(4, 0)
#define SLINT_P4         APOLLO3_PINMUX(4, 1)
#define NCE4_P4          APOLLO3_PINMUX(4, 2)
#define GPIO_P4          APOLLO3_PINMUX(4, 3)
#define UART1RX_P4       APOLLO3_PINMUX(4, 5)
#define CTIM17_P4        APOLLO3_PINMUX(4, 6)
#define MSPI0_2_P4       APOLLO3_PINMUX(4, 7)
#define M0SCL_P5         APOLLO3_PINMUX(5, 0)
#define M0SCK_P5         APOLLO3_PINMUX(5, 1)
#define UA0RTS_P5        APOLLO3_PINMUX(5, 2)
#define GPIO_P5          APOLLO3_PINMUX(5, 3)
#define CT8_P5           APOLLO3_PINMUX(5, 7)
#define M0SDAWIR3_P6     APOLLO3_PINMUX(6, 0)
#define M0MISO_P6        APOLLO3_PINMUX(6, 1)
#define UA0CTS_P6        APOLLO3_PINMUX(6, 2)
#define GPIO_P6          APOLLO3_PINMUX(6, 3)
#define CTIM10_P6        APOLLO3_PINMUX(6, 5)
#define I2SDAT_P6        APOLLO3_PINMUX(6, 7)
#define NCE7_P7          APOLLO3_PINMUX(7, 0)
#define M0MOSI_P7        APOLLO3_PINMUX(7, 1)
#define CLKOUT_P7        APOLLO3_PINMUX(7, 2)
#define GPIO_P7          APOLLO3_PINMUX(7, 3)
#define TRIG0_P7         APOLLO3_PINMUX(7, 4)
#define UART0TX_P7       APOLLO3_PINMUX(7, 5)
#define CTIM19_P7        APOLLO3_PINMUX(7, 7)
#define M1SCL_P8         APOLLO3_PINMUX(8, 0)
#define M1SCK_P8         APOLLO3_PINMUX(8, 1)
#define NCE8_P8          APOLLO3_PINMUX(8, 2)
#define GPIO_P8          APOLLO3_PINMUX(8, 3)
#define SCCCLK_P8        APOLLO3_PINMUX(8, 4)
#define UART1TX_P8       APOLLO3_PINMUX(8, 6)
#define M1SDAWIR3_P9     APOLLO3_PINMUX(9, 0)
#define M1MISO_P9        APOLLO3_PINMUX(9, 1)
#define NCE9_P9          APOLLO3_PINMUX(9, 2)
#define GPIO_P9          APOLLO3_PINMUX(9, 3)
#define SCCIO_P9         APOLLO3_PINMUX(9, 4)
#define UART1RX_P9       APOLLO3_PINMUX(9, 6)
#define UART1TX_P10      APOLLO3_PINMUX(10, 0)
#define M1MOSI_P10       APOLLO3_PINMUX(10, 1)
#define NCE10_P10        APOLLO3_PINMUX(10, 2)
#define GPIO_P10         APOLLO3_PINMUX(10, 3)
#define PDMCLK_P10       APOLLO3_PINMUX(10, 4)
#define UA1RTS_P10       APOLLO3_PINMUX(10, 6)
#define ADCSE2_P11       APOLLO3_PINMUX(11, 0)
#define NCE11_P11        APOLLO3_PINMUX(11, 1)
#define CTIM31_P11       APOLLO3_PINMUX(11, 2)
#define GPIO_P11         APOLLO3_PINMUX(11, 3)
#define SLINT_P11        APOLLO3_PINMUX(11, 4)
#define UA1CTS_P11       APOLLO3_PINMUX(11, 5)
#define UART0RX_P11      APOLLO3_PINMUX(11, 6)
#define PDMDATA_P11      APOLLO3_PINMUX(11, 7)
#define ADCD0NSE9_P12    APOLLO3_PINMUX(12, 0)
#define NCE12_P12        APOLLO3_PINMUX(12, 1)
#define CT0_P12          APOLLO3_PINMUX(12, 2)
#define GPIO_P12         APOLLO3_PINMUX(12, 3)
#define PDMCLK_P12       APOLLO3_PINMUX(12, 5)
#define UA0CTS_P12       APOLLO3_PINMUX(12, 6)
#define UART1TX_P12      APOLLO3_PINMUX(12, 7)
#define ADCD0PSE8_P13    APOLLO3_PINMUX(13, 0)
#define NCE13_P13        APOLLO3_PINMUX(13, 1)
#define CTIM2_P13        APOLLO3_PINMUX(13, 2)
#define GPIO_P13         APOLLO3_PINMUX(13, 3)
#define I2SBCLK_P13      APOLLO3_PINMUX(13, 4)
#define UA0RTS_P13       APOLLO3_PINMUX(13, 6)
#define UART1RX_P13      APOLLO3_PINMUX(13, 7)
#define ADCD1P_P14       APOLLO3_PINMUX(14, 0)
#define NCE14_P14        APOLLO3_PINMUX(14, 1)
#define UART1TX_P14      APOLLO3_PINMUX(14, 2)
#define GPIO_P14         APOLLO3_PINMUX(14, 3)
#define PDMCLK_P14       APOLLO3_PINMUX(14, 4)
#define SWDCK_P14        APOLLO3_PINMUX(14, 6)
#define XT32KHz_P14      APOLLO3_PINMUX(14, 7)
#define ADCD1N_P15       APOLLO3_PINMUX(15, 0)
#define NCE15_P15        APOLLO3_PINMUX(15, 1)
#define UART1RX_P15      APOLLO3_PINMUX(15, 2)
#define GPIO_P15         APOLLO3_PINMUX(15, 3)
#define PDMDATA_P15      APOLLO3_PINMUX(15, 4)
#define SWDIO_P15        APOLLO3_PINMUX(15, 6)
#define SWO_P15          APOLLO3_PINMUX(15, 7)
#define ADCSE0_P16       APOLLO3_PINMUX(16, 0)
#define NCE16_P16        APOLLO3_PINMUX(16, 1)
#define TRIG0_P16        APOLLO3_PINMUX(16, 2)
#define GPIO_P16         APOLLO3_PINMUX(16, 3)
#define SCCRST_P16       APOLLO3_PINMUX(16, 4)
#define CMPIN0_P16       APOLLO3_PINMUX(16, 5)
#define UART0TX_P16      APOLLO3_PINMUX(16, 6)
#define UA1RTS_P16       APOLLO3_PINMUX(16, 7)
#define CMPRF1_P17       APOLLO3_PINMUX(17, 0)
#define NCE17_P17        APOLLO3_PINMUX(17, 1)
#define TRIG1_P17        APOLLO3_PINMUX(17, 2)
#define GPIO_P17         APOLLO3_PINMUX(17, 3)
#define SCCCLK_P17       APOLLO3_PINMUX(17, 4)
#define UART0RX_P17      APOLLO3_PINMUX(17, 6)
#define UA1CTS_P17       APOLLO3_PINMUX(17, 7)
#define CMPIN1_P18       APOLLO3_PINMUX(18, 0)
#define NCE18_P18        APOLLO3_PINMUX(18, 1)
#define CTIM4_P18        APOLLO3_PINMUX(18, 2)
#define GPIO_P18         APOLLO3_PINMUX(18, 3)
#define UA0RTS_P18       APOLLO3_PINMUX(18, 4)
#define UART1TX_P18      APOLLO3_PINMUX(18, 6)
#define SCCIO_P18        APOLLO3_PINMUX(18, 7)
#define CMPRF0_P19       APOLLO3_PINMUX(19, 0)
#define NCE19_P19        APOLLO3_PINMUX(19, 1)
#define CTIM6_P19        APOLLO3_PINMUX(19, 2)
#define GPIO_P19         APOLLO3_PINMUX(19, 3)
#define SCCCLK_P19       APOLLO3_PINMUX(19, 4)
#define UART1RX_P19      APOLLO3_PINMUX(19, 6)
#define I2SBCLK_P19      APOLLO3_PINMUX(19, 7)
#define SWDCK_P20        APOLLO3_PINMUX(20, 0)
#define NCE20_P20        APOLLO3_PINMUX(20, 1)
#define GPIO_P20         APOLLO3_PINMUX(20, 3)
#define UART0TX_P20      APOLLO3_PINMUX(20, 4)
#define UART1TX_P20      APOLLO3_PINMUX(20, 5)
#define I2SBCLK_P20      APOLLO3_PINMUX(20, 6)
#define UA1RTS_P20       APOLLO3_PINMUX(20, 7)
#define SWDIO_P21        APOLLO3_PINMUX(21, 0)
#define NCE21_P21        APOLLO3_PINMUX(21, 1)
#define GPIO_P21         APOLLO3_PINMUX(21, 3)
#define UART0RX_P21      APOLLO3_PINMUX(21, 4)
#define UART1RX_P21      APOLLO3_PINMUX(21, 5)
#define SCCRST_P21       APOLLO3_PINMUX(21, 6)
#define UA1CTS_P21       APOLLO3_PINMUX(21, 7)
#define UART0TX_P22      APOLLO3_PINMUX(22, 0)
#define NCE22_P22        APOLLO3_PINMUX(22, 1)
#define CTIM12_P22       APOLLO3_PINMUX(22, 2)
#define GPIO_P22         APOLLO3_PINMUX(22, 3)
#define PDMCLK_P22       APOLLO3_PINMUX(22, 4)
#define MSPI0_0_P22      APOLLO3_PINMUX(22, 6)
#define SWO_P22          APOLLO3_PINMUX(22, 7)
#define UART0RX_P23      APOLLO3_PINMUX(23, 0)
#define NCE23_P23        APOLLO3_PINMUX(23, 1)
#define CTIM14_P23       APOLLO3_PINMUX(23, 2)
#define GPIO_P23         APOLLO3_PINMUX(23, 3)
#define I2SWCLK_P23      APOLLO3_PINMUX(23, 4)
#define CMPOUT_P23       APOLLO3_PINMUX(23, 5)
#define MSPI0_3_P23      APOLLO3_PINMUX(23, 6)
#define UART1TX_P24      APOLLO3_PINMUX(24, 0)
#define NCE24_P24        APOLLO3_PINMUX(24, 1)
#define MSPI0_8_P24      APOLLO3_PINMUX(24, 2)
#define GPIO_P24         APOLLO3_PINMUX(24, 3)
#define UA0CTS_P24       APOLLO3_PINMUX(24, 4)
#define CTIM21_P24       APOLLO3_PINMUX(24, 5)
#define XT32KHz_P24      APOLLO3_PINMUX(24, 6)
#define SWO_P24          APOLLO3_PINMUX(24, 7)
#define UART1RX_P25      APOLLO3_PINMUX(25, 0)
#define NCE25_P25        APOLLO3_PINMUX(25, 1)
#define CTIM1_P25        APOLLO3_PINMUX(25, 2)
#define GPIO_P25         APOLLO3_PINMUX(25, 3)
#define M2SDAWIR3_P25    APOLLO3_PINMUX(25, 4)
#define M2MISO_P25       APOLLO3_PINMUX(25, 5)
#define NCE26_P26        APOLLO3_PINMUX(26, 1)
#define CTIM3_P26        APOLLO3_PINMUX(26, 2)
#define GPIO_P26         APOLLO3_PINMUX(26, 3)
#define SCCRST_P26       APOLLO3_PINMUX(26, 4)
#define MSPI0_1_P26      APOLLO3_PINMUX(26, 5)
#define UART0TX_P26      APOLLO3_PINMUX(26, 6)
#define UA1CTS_P26       APOLLO3_PINMUX(26, 7)
#define UART0RX_P27      APOLLO3_PINMUX(27, 0)
#define NCE27_P27        APOLLO3_PINMUX(27, 1)
#define CTIM5_P27        APOLLO3_PINMUX(27, 2)
#define GPIO_P27         APOLLO3_PINMUX(27, 3)
#define M2SCL_P27        APOLLO3_PINMUX(27, 4)
#define M2SCK_P27        APOLLO3_PINMUX(27, 5)
#define I2SWCLK_P28      APOLLO3_PINMUX(28, 0)
#define NCE28_P28        APOLLO3_PINMUX(28, 1)
#define CTIM7_P28        APOLLO3_PINMUX(28, 2)
#define GPIO_P28         APOLLO3_PINMUX(28, 3)
#define M2MOSI_P28       APOLLO3_PINMUX(28, 5)
#define UART0TX_P28      APOLLO3_PINMUX(28, 6)
#define ADCSE1_P29       APOLLO3_PINMUX(29, 0)
#define NCE29_P29        APOLLO3_PINMUX(29, 1)
#define CTIM9_P29        APOLLO3_PINMUX(29, 2)
#define GPIO_P29         APOLLO3_PINMUX(29, 3)
#define UA0CTS_P29       APOLLO3_PINMUX(29, 4)
#define UA1CTS_P29       APOLLO3_PINMUX(29, 5)
#define UART0RX_P29      APOLLO3_PINMUX(29, 6)
#define PDMDATA_P29      APOLLO3_PINMUX(29, 7)
#define NCE30_P30        APOLLO3_PINMUX(30, 1)
#define CTIM11_P30       APOLLO3_PINMUX(30, 2)
#define GPIO_P30         APOLLO3_PINMUX(30, 3)
#define UART0TX_P30      APOLLO3_PINMUX(30, 4)
#define UA1RTS_P30       APOLLO3_PINMUX(30, 5)
#define BLEIF_SCK_P30    APOLLO3_PINMUX(30, 6)
#define I2SDAT_P30       APOLLO3_PINMUX(30, 7)
#define ADCSE3_P31       APOLLO3_PINMUX(31, 0)
#define NCE31_P31        APOLLO3_PINMUX(31, 1)
#define CTIM13_P31       APOLLO3_PINMUX(31, 2)
#define GPIO_P31         APOLLO3_PINMUX(31, 3)
#define UART0RX_P31      APOLLO3_PINMUX(31, 4)
#define SCCCLK_P31       APOLLO3_PINMUX(31, 5)
#define BLEIF_MISO_P31   APOLLO3_PINMUX(31, 6)
#define UA1RTS_P31       APOLLO3_PINMUX(31, 7)
#define ADCSE4_P32       APOLLO3_PINMUX(32, 0)
#define NCE32_P32        APOLLO3_PINMUX(32, 1)
#define CTIM15_P32       APOLLO3_PINMUX(32, 2)
#define GPIO_P32         APOLLO3_PINMUX(32, 3)
#define SCCIO_P32        APOLLO3_PINMUX(32, 4)
#define BLEIF_MOSI_P32   APOLLO3_PINMUX(32, 6)
#define UA1CTS_P32       APOLLO3_PINMUX(32, 7)
#define ADCSE5_P33       APOLLO3_PINMUX(33, 0)
#define NCE33_P33        APOLLO3_PINMUX(33, 1)
#define XT32KHz_P33      APOLLO3_PINMUX(33, 2)
#define GPIO_P33         APOLLO3_PINMUX(33, 3)
#define BLEIF_CSN_P33    APOLLO3_PINMUX(33, 4)
#define UA0CTS_P33       APOLLO3_PINMUX(33, 5)
#define CTIM23_P33       APOLLO3_PINMUX(33, 6)
#define SWO_P33          APOLLO3_PINMUX(33, 7)
#define ADCSE6_P34       APOLLO3_PINMUX(34, 0)
#define NCE34_P34        APOLLO3_PINMUX(34, 1)
#define UA1RTS_P34       APOLLO3_PINMUX(34, 2)
#define GPIO_P34         APOLLO3_PINMUX(34, 3)
#define CMPRF2_P34       APOLLO3_PINMUX(34, 4)
#define UA0RTS_P34       APOLLO3_PINMUX(34, 5)
#define UART0RX_P34      APOLLO3_PINMUX(34, 6)
#define PDMDATA_P34      APOLLO3_PINMUX(34, 7)
#define ADCSE7_P35       APOLLO3_PINMUX(35, 0)
#define NCE35_P35        APOLLO3_PINMUX(35, 1)
#define UART1TX_P35      APOLLO3_PINMUX(35, 2)
#define GPIO_P35         APOLLO3_PINMUX(35, 3)
#define I2SDAT_P35       APOLLO3_PINMUX(35, 4)
#define CTIM27_P35       APOLLO3_PINMUX(35, 5)
#define UA0RTS_P35       APOLLO3_PINMUX(35, 6)
#define BLEIF_STATUS_P35 APOLLO3_PINMUX(35, 7)
#define TRIG1_P36        APOLLO3_PINMUX(36, 0)
#define NCE36_P36        APOLLO3_PINMUX(36, 1)
#define UART1RX_P36      APOLLO3_PINMUX(36, 2)
#define GPIO_P36         APOLLO3_PINMUX(36, 3)
#define XT32KHz_P36      APOLLO3_PINMUX(36, 4)
#define UA1CTS_P36       APOLLO3_PINMUX(36, 5)
#define UA0CTS_P36       APOLLO3_PINMUX(36, 6)
#define PDMDATA_P36      APOLLO3_PINMUX(36, 7)
#define TRIG2_P37        APOLLO3_PINMUX(37, 0)
#define NCE37_P37        APOLLO3_PINMUX(37, 1)
#define UA0RTS_P37       APOLLO3_PINMUX(37, 2)
#define GPIO_P37         APOLLO3_PINMUX(37, 3)
#define SCCIO_P37        APOLLO3_PINMUX(37, 4)
#define UART1TX_P37      APOLLO3_PINMUX(37, 5)
#define PDMCLK_P37       APOLLO3_PINMUX(37, 6)
#define CTIM29_P37       APOLLO3_PINMUX(37, 7)
#define TRIG3_P38        APOLLO3_PINMUX(38, 0)
#define NCE38_P38        APOLLO3_PINMUX(38, 1)
#define UA0CTS_P38       APOLLO3_PINMUX(38, 2)
#define GPIO_P38         APOLLO3_PINMUX(38, 3)
#define M3MOSI_P38       APOLLO3_PINMUX(38, 5)
#define UART1RX_P38      APOLLO3_PINMUX(38, 6)
#define UART0TX_P39      APOLLO3_PINMUX(39, 0)
#define UART1TX_P39      APOLLO3_PINMUX(39, 1)
#define CTIM25_P39       APOLLO3_PINMUX(39, 2)
#define GPIO_P39         APOLLO3_PINMUX(39, 3)
#define M4SCL_P39        APOLLO3_PINMUX(39, 4)
#define M4SCK_P39        APOLLO3_PINMUX(39, 5)
#define UART0RX_P40      APOLLO3_PINMUX(40, 0)
#define UART1RX_P40      APOLLO3_PINMUX(40, 1)
#define TRIG0_P40        APOLLO3_PINMUX(40, 2)
#define GPIO_P40         APOLLO3_PINMUX(40, 3)
#define M4SDAWIR3_P40    APOLLO3_PINMUX(40, 4)
#define M4MISO_P40       APOLLO3_PINMUX(40, 5)
#define NCE41_P41        APOLLO3_PINMUX(41, 0)
#define BLEIF_IRQ_P41    APOLLO3_PINMUX(41, 1)
#define SWO_P41          APOLLO3_PINMUX(41, 2)
#define GPIO_P41         APOLLO3_PINMUX(41, 3)
#define I2SWCLK_P41      APOLLO3_PINMUX(41, 4)
#define UA1RTS_P41       APOLLO3_PINMUX(41, 5)
#define UART0TX_P41      APOLLO3_PINMUX(41, 6)
#define UA0RTS_P41       APOLLO3_PINMUX(41, 7)
#define UART1TX_P42      APOLLO3_PINMUX(42, 0)
#define NCE42_P42        APOLLO3_PINMUX(42, 1)
#define CTIM16_P42       APOLLO3_PINMUX(42, 2)
#define GPIO_P42         APOLLO3_PINMUX(42, 3)
#define M3SCL_P42        APOLLO3_PINMUX(42, 4)
#define M3SCK_P42        APOLLO3_PINMUX(42, 5)
#define UART1RX_P43      APOLLO3_PINMUX(43, 0)
#define NCE43_P43        APOLLO3_PINMUX(43, 1)
#define CTIM18_P43       APOLLO3_PINMUX(43, 2)
#define GPIO_P43         APOLLO3_PINMUX(43, 3)
#define M3SDAWIR3_P43    APOLLO3_PINMUX(43, 4)
#define M3MISO_P43       APOLLO3_PINMUX(43, 5)
#define UA1RTS_P44       APOLLO3_PINMUX(44, 0)
#define NCE44_P44        APOLLO3_PINMUX(44, 1)
#define CTIM20_P44       APOLLO3_PINMUX(44, 2)
#define GPIO_P44         APOLLO3_PINMUX(44, 3)
#define M4MOSI_P44       APOLLO3_PINMUX(44, 5)
#define UART0TX_P44      APOLLO3_PINMUX(44, 6)
#define UA1CTS_P45       APOLLO3_PINMUX(45, 0)
#define NCE45_P45        APOLLO3_PINMUX(45, 1)
#define CTIM22_P45       APOLLO3_PINMUX(45, 2)
#define GPIO_P45         APOLLO3_PINMUX(45, 3)
#define I2SDAT_P45       APOLLO3_PINMUX(45, 4)
#define PDMDATA_P45      APOLLO3_PINMUX(45, 5)
#define UART0RX_P45      APOLLO3_PINMUX(45, 6)
#define SWO_P45          APOLLO3_PINMUX(45, 7)
#define I2SBCLK_P46      APOLLO3_PINMUX(46, 0)
#define NCE46_P46        APOLLO3_PINMUX(46, 1)
#define CTIM24_P46       APOLLO3_PINMUX(46, 2)
#define GPIO_P46         APOLLO3_PINMUX(46, 3)
#define SCCRST_P46       APOLLO3_PINMUX(46, 4)
#define PDMCLK_P46       APOLLO3_PINMUX(46, 5)
#define UART1TX_P46      APOLLO3_PINMUX(46, 6)
#define SWO_P46          APOLLO3_PINMUX(46, 7)
#define XT32KHz_P47      APOLLO3_PINMUX(47, 0)
#define NCE47_P47        APOLLO3_PINMUX(47, 1)
#define CTIM26_P47       APOLLO3_PINMUX(47, 2)
#define GPIO_P47         APOLLO3_PINMUX(47, 3)
#define M5MOSI_P47       APOLLO3_PINMUX(47, 5)
#define UART1RX_P47      APOLLO3_PINMUX(47, 6)
#define UART0TX_P48      APOLLO3_PINMUX(48, 0)
#define NCE48_P48        APOLLO3_PINMUX(48, 1)
#define CTIM28_P48       APOLLO3_PINMUX(48, 2)
#define GPIO_P48         APOLLO3_PINMUX(48, 3)
#define M5SCL_P48        APOLLO3_PINMUX(48, 4)
#define M5SCK_P48        APOLLO3_PINMUX(48, 5)
#define UART0RX_P49      APOLLO3_PINMUX(49, 0)
#define NCE49_P49        APOLLO3_PINMUX(49, 1)
#define CTIM30_P49       APOLLO3_PINMUX(49, 2)
#define GPIO_P49         APOLLO3_PINMUX(49, 3)
#define M5SDAWIR3_P49    APOLLO3_PINMUX(49, 4)
#define M5MISO_P49       APOLLO3_PINMUX(49, 5)
#define SWO_P50          APOLLO3_PINMUX(50, 0)
#define NCE50_P50        APOLLO3_PINMUX(50, 1)
#define CT0_P50          APOLLO3_PINMUX(50, 2)
#define GPIO_P50         APOLLO3_PINMUX(50, 3)
#define UART0TX_P50      APOLLO3_PINMUX(50, 4)
#define UART0RX_P50      APOLLO3_PINMUX(50, 5)
#define UART1TX_P50      APOLLO3_PINMUX(50, 6)
#define UART1RX_P50      APOLLO3_PINMUX(50, 7)
#define MSPI1_0_P51      APOLLO3_PINMUX(51, 0)
#define NCE51_P51        APOLLO3_PINMUX(51, 1)
#define CTIM1_P51        APOLLO3_PINMUX(51, 2)
#define GPIO_P51         APOLLO3_PINMUX(51, 3)
#define MSPI1_1_P52      APOLLO3_PINMUX(52, 0)
#define NCE52_P52        APOLLO3_PINMUX(52, 1)
#define CTIM2_P52        APOLLO3_PINMUX(52, 2)
#define GPIO_P52         APOLLO3_PINMUX(52, 3)
#define MSPI1_2_P53      APOLLO3_PINMUX(53, 0)
#define NCE53_P53        APOLLO3_PINMUX(53, 1)
#define CTIM3_P53        APOLLO3_PINMUX(53, 2)
#define GPIO_P53         APOLLO3_PINMUX(53, 3)
#define MSPI1_3_P54      APOLLO3_PINMUX(54, 0)
#define NCE54_P54        APOLLO3_PINMUX(54, 1)
#define CTIM4_P54        APOLLO3_PINMUX(54, 2)
#define GPIO_P54         APOLLO3_PINMUX(54, 3)
#define MSPI1_4_P55      APOLLO3_PINMUX(55, 0)
#define NCE55_P55        APOLLO3_PINMUX(55, 1)
#define CTIM5_P55        APOLLO3_PINMUX(55, 2)
#define GPIO_P55         APOLLO3_PINMUX(55, 3)
#define MSPI1_5_P56      APOLLO3_PINMUX(56, 0)
#define NCE56_P56        APOLLO3_PINMUX(56, 1)
#define CTIM6_P56        APOLLO3_PINMUX(56, 2)
#define GPIO_P56         APOLLO3_PINMUX(56, 3)
#define MSPI1_6_P57      APOLLO3_PINMUX(57, 0)
#define NCE57_P57        APOLLO3_PINMUX(57, 1)
#define CTIM7_P57        APOLLO3_PINMUX(57, 2)
#define GPIO_P57         APOLLO3_PINMUX(57, 3)
#define MSPI1_7_P58      APOLLO3_PINMUX(58, 0)
#define NCE58_P58        APOLLO3_PINMUX(58, 1)
#define CTIM8_P58        APOLLO3_PINMUX(58, 2)
#define GPIO_P58         APOLLO3_PINMUX(58, 3)
#define MSPI1_8_P59      APOLLO3_PINMUX(59, 0)
#define NCE59_P59        APOLLO3_PINMUX(59, 1)
#define CTIM9_P59        APOLLO3_PINMUX(59, 2)
#define GPIO_P59         APOLLO3_PINMUX(59, 3)
#define MSPI1_9_P60      APOLLO3_PINMUX(60, 0)
#define NCE60_P60        APOLLO3_PINMUX(60, 1)
#define CTIM10_P60       APOLLO3_PINMUX(60, 2)
#define GPIO_P60         APOLLO3_PINMUX(60, 3)
#define SWO_P61          APOLLO3_PINMUX(61, 0)
#define NCE61_P61        APOLLO3_PINMUX(61, 1)
#define CTIM11_P61       APOLLO3_PINMUX(61, 2)
#define GPIO_P61         APOLLO3_PINMUX(61, 3)
#define UART0TX_P61      APOLLO3_PINMUX(61, 4)
#define UART0RX_P61      APOLLO3_PINMUX(61, 5)
#define UART1TX_P61      APOLLO3_PINMUX(61, 6)
#define UART1RX_P61      APOLLO3_PINMUX(61, 7)
#define SWO_P62          APOLLO3_PINMUX(62, 0)
#define NCE62_P62        APOLLO3_PINMUX(62, 1)
#define CTIM12_P62       APOLLO3_PINMUX(62, 2)
#define GPIO_P62         APOLLO3_PINMUX(62, 3)
#define UA0CTS_P62       APOLLO3_PINMUX(62, 4)
#define UA0RTS_P62       APOLLO3_PINMUX(62, 5)
#define UA1CTS_P62       APOLLO3_PINMUX(62, 6)
#define UA1RTS_P62       APOLLO3_PINMUX(62, 7)
#define SWO_P63          APOLLO3_PINMUX(63, 0)
#define NCE63_P63        APOLLO3_PINMUX(63, 1)
#define CTIM13_P63       APOLLO3_PINMUX(63, 2)
#define GPIO_P63         APOLLO3_PINMUX(63, 3)
#define UA0CTS_P63       APOLLO3_PINMUX(63, 4)
#define UA0RTS_P63       APOLLO3_PINMUX(63, 5)
#define UA1CTS_P63       APOLLO3_PINMUX(63, 6)
#define UA1RTS_P63       APOLLO3_PINMUX(63, 7)
#define MSPI2_0_P64      APOLLO3_PINMUX(64, 0)
#define NCE64_P64        APOLLO3_PINMUX(64, 1)
#define CTIM14_P64       APOLLO3_PINMUX(64, 2)
#define GPIO_P64         APOLLO3_PINMUX(64, 3)
#define MSPI2_1_P65      APOLLO3_PINMUX(65, 0)
#define NCE65_P65        APOLLO3_PINMUX(65, 1)
#define CTIM15_P65       APOLLO3_PINMUX(65, 2)
#define GPIO_P65         APOLLO3_PINMUX(65, 3)
#define MSPI2_2_P66      APOLLO3_PINMUX(66, 0)
#define NCE66_P66        APOLLO3_PINMUX(66, 1)
#define CTIM16_P66       APOLLO3_PINMUX(66, 2)
#define GPIO_P66         APOLLO3_PINMUX(66, 3)
#define MSPI2_3_P67      APOLLO3_PINMUX(67, 0)
#define NCE67_P67        APOLLO3_PINMUX(67, 1)
#define CTIM17_P67       APOLLO3_PINMUX(67, 2)
#define GPIO_P67         APOLLO3_PINMUX(67, 3)
#define MSPI2_4_P68      APOLLO3_PINMUX(68, 0)
#define NCE68_P68        APOLLO3_PINMUX(68, 1)
#define CTIM18_P68       APOLLO3_PINMUX(68, 2)
#define GPIO_P68         APOLLO3_PINMUX(68, 3)
#define SWO_P69          APOLLO3_PINMUX(69, 0)
#define NCE69_P69        APOLLO3_PINMUX(69, 1)
#define CTIM19_P69       APOLLO3_PINMUX(69, 2)
#define GPIO_P69         APOLLO3_PINMUX(69, 3)
#define UART0TX_P69      APOLLO3_PINMUX(69, 4)
#define UART0RX_P69      APOLLO3_PINMUX(69, 5)
#define UART1TX_P69      APOLLO3_PINMUX(69, 6)
#define UART1RX_P69      APOLLO3_PINMUX(69, 7)
#define SWO_P70          APOLLO3_PINMUX(70, 0)
#define NCE70_P70        APOLLO3_PINMUX(70, 1)
#define CTIM20_P70       APOLLO3_PINMUX(70, 2)
#define GPIO_P70         APOLLO3_PINMUX(70, 3)
#define UART0TX_P70      APOLLO3_PINMUX(70, 4)
#define UART0RX_P70      APOLLO3_PINMUX(70, 5)
#define UART1TX_P70      APOLLO3_PINMUX(70, 6)
#define UART1RX_P70      APOLLO3_PINMUX(70, 7)
#define SWO_P71          APOLLO3_PINMUX(71, 0)
#define NCE71_P71        APOLLO3_PINMUX(71, 1)
#define CTIM21_P71       APOLLO3_PINMUX(71, 2)
#define GPIO_P71         APOLLO3_PINMUX(71, 3)
#define UART0TX_P71      APOLLO3_PINMUX(71, 4)
#define UART0RX_P71      APOLLO3_PINMUX(71, 5)
#define UART1TX_P71      APOLLO3_PINMUX(71, 6)
#define UART1RX_P71      APOLLO3_PINMUX(71, 7)
#define SWO_P72          APOLLO3_PINMUX(72, 0)
#define NCE72_P72        APOLLO3_PINMUX(72, 1)
#define CTIM22_P72       APOLLO3_PINMUX(72, 2)
#define GPIO_P72         APOLLO3_PINMUX(72, 3)
#define UART0TX_P72      APOLLO3_PINMUX(72, 4)
#define UART0RX_P72      APOLLO3_PINMUX(72, 5)
#define UART1TX_P72      APOLLO3_PINMUX(72, 6)
#define UART1RX_P72      APOLLO3_PINMUX(72, 7)
#define SWO_P73          APOLLO3_PINMUX(73, 0)
#define NCE73_P73        APOLLO3_PINMUX(73, 1)
#define CTIM23_P73       APOLLO3_PINMUX(73, 2)
#define GPIO_P73         APOLLO3_PINMUX(73, 3)
#define UA0CTS_P73       APOLLO3_PINMUX(73, 4)
#define UA0RTS_P73       APOLLO3_PINMUX(73, 5)
#define UA1CTS_P73       APOLLO3_PINMUX(73, 6)
#define UA1RTS_P73       APOLLO3_PINMUX(73, 7)

#endif /* __APOLLO3_PINCTRL_H__ */
