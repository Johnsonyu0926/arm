// Filename: fsl_lsch2_speed.c 
 
#include <common.h> 
#include <linux/compiler.h> 
#include <asm/io.h> 
#include <asm/processor.h> 
#include <asm/arch/clock.h> 
#include <asm/arch/soc.h> 
#include <fsl_ifc.h> 
#include "cpu.h" 
 
DECLARE_GLOBAL_DATA_PTR; 
 
#ifndef CONFIG_SYS_FSL_NUM_CC_PLLS 
#define CONFIG_SYS_FSL_NUM_CC_PLLS 2 
#endif 
 
void get_sys_info(struct sys_info *sys_info) 
{ 
 struct ccsr_gur __iomem *gur = (void *)(CONFIG_SYS_FSL_GUTS_ADDR); 
#if (defined(CONFIG_FSL_ESDHC) && defined(CONFIG_FSL_ESDHC_USE_PERIPHERAL_CLK)) || defined(CONFIG_SYS_DPAA_FMAN) 
 uint32_t rcw_tmp; 
#endif 
 struct ccsr_clk *clk = (void *)(CONFIG_SYS_FSL_CLK_ADDR); 
 unsigned int cpu; 
 const uint8_t core_cplx_pll[8] = { 
 [0] = 0, [1] = 0, [4] = 1, [5] = 1 
 }; 
 
 const uint8_t core_cplx_pll_div[8] = { 
 [0] = 1, [1] = 2, [4] = 1, [5] = 2 
 }; 
 
 uint32_t i, cluster; 
 uint32_t freq_c_pll[CONFIG_SYS_FSL_NUM_CC_PLLS]; 
 uint32_t ratio[CONFIG_SYS_FSL_NUM_CC_PLLS]; 
 unsigned long sysclk = CONFIG_SYS_CLK_FREQ; 
 unsigned long cluster_clk; 
 
 sys_info->freq_systembus = sysclk; 
#ifndef CONFIG_CLUSTER_CLK_FREQ 
#define CONFIG_CLUSTER_CLK_FREQ CONFIG_SYS_CLK_FREQ 
#endif 
 cluster_clk = CONFIG_CLUSTER_CLK_FREQ; 
 
#ifdef CONFIG_DDR_CLK_FREQ 
 sys_info->freq_ddrbus = CONFIG_DDR_CLK_FREQ; 
#else 
 sys_info->freq_ddrbus = sysclk; 
#endif 
 
 sys_info->freq_systembus *= (gur_in32(&gur->rcwsr[0]) >> FSL_CHASSIS2_RCWSR0_SYS_PLL_RAT_SHIFT) & FSL_CHASSIS2_RCWSR0_SYS_PLL_RAT_MASK; 
 
#ifdef CONFIG_ARCH_LS1012A 
 sys_info->freq_ddrbus = 2 * sys_info->freq_systembus; 
#else 
 sys_info->freq_ddrbus *= (gur_in32(&gur->rcwsr[0]) >> FSL_CHASSIS2_RCWSR0_MEM_PLL_RAT_SHIFT) & FSL_CHASSIS2_RCWSR0_MEM_PLL_RAT_MASK; 
#endif 
 
 for (i = 0; i < CONFIG_SYS_FSL_NUM_CC_PLLS; i++) { 
 ratio[i] = (in_be32(&clk->pllcgsr[i].pllcngsr) >> 1) & 0xff; 
 if (ratio[i] > 4) 
 freq_c_pll[i] = cluster_clk * ratio[i]; 
 else 
 freq_c_pll[i] = sys_info->freq_systembus * ratio[i]; 
 } 
 
 for_each_cpu(i, cpu, cpu_numcores(), cpu_mask()) { 
 cluster = fsl_qoriq_core_to_cluster(cpu); 
 uint32_t c_pll_sel = (in_be32(&clk->clkcsr[cluster].clkcncsr) >> 27) & 0xf; 
 uint32_t cplx_pll = core_cplx_pll[c_pll_sel]; 
 
 sys_info->freq_processor[cpu] = freq_c_pll[cplx_pll] / core_cplx_pll_div[c_pll_sel]; 
 } 
 
#define HWA_CGA_M1_CLK_SEL 0xe0000000 
#define HWA_CGA_M1_CLK_SHIFT 29 
#ifdef CONFIG_SYS_DPAA_FMAN 
 rcw_tmp = in_be32(&gur->rcwsr[7]); 
 switch ((rcw_tmp & HWA_CGA_M1_CLK_SEL) >> HWA_CGA_M1_CLK_SHIFT) { 
 case 2: 
 sys_info->freq_fman[0] = freq_c_pll[0] / 2; 
 break; 
 case 3: 
 sys_info->freq_fman[0] = freq_c_pll[0] / 3; 
 break; 
 case 4: 
 sys_info->freq_fman[0] = freq_c_pll[0] / 4; 
 break; 
 case 5: 
 sys_info->freq_fman[0] = sys_info->freq_systembus; 
 break; 
 case 6: 
 sys_info->freq_fman[0] = freq_c_pll[1] / 2; 
 break; 
 case 7: 
 sys_info->freq_fman[0] = freq_c_pll[1] / 3; 
 break; 
 default: 
 printf("Error: Unknown FMan1 clock select!\n"); 
 break; 
 } 
#endif 
 
#define HWA_CGA_M2_CLK_SEL 0x00000007 
#define HWA_CGA_M2_CLK_SHIFT 0 
#ifdef CONFIG_FSL_ESDHC 
#ifdef CONFIG_FSL_ESDHC_USE_PERIPHERAL_CLK 
 rcw_tmp = in_be32(&gur->rcwsr[15]); 
 switch ((rcw_tmp & HWA_CGA_M2_CLK_SEL) >> HWA_CGA_M2_CLK_SHIFT) { 
 case 1: 
 sys_info->freq_sdhc = freq_c_pll[1]; 
 break; 
 case 2: 
 sys_info->freq_sdhc = freq_c_pll[1] / 2; 
 break; 
 case 3: 
 sys_info->freq_sdhc = freq_c_pll[1] / 3; 
 break; 
 case 6: 
 sys_info->freq_sdhc = freq_c_pll[0] / 2; 
 break; 
 default: 
 printf("Error: Unknown ESDHC clock select!\n"); 
 break; 
 } 
#else 
 sys_info->freq_sdhc = (sys_info->freq_systembus / CONFIG_SYS_FSL_PCLK_DIV) / CONFIG_SYS_FSL_SDHC_CLK_DIV; 
#endif 
#endif 
 
#if defined(CONFIG_FSL_IFC) 
 sys_info->freq_localbus = sys_info->freq_systembus / CONFIG_SYS_FSL_IFC_CLK_DIV; 
#endif 
} 
 
int get_clocks(void) 
{ 
 struct sys_info sys_info; 
 
 get_sys_info(&sys_info); 
 gd->cpu_clk = sys_info.freq_processor[0]; 
 gd->bus_clk = sys_info.freq_systembus / CONFIG_SYS_FSL_PCLK_DIV; 
 gd->mem_clk = sys_info.freq_ddrbus; 
 
#ifdef CONFIG_FSL_ESDHC 
 gd->arch.sdhc_clk = sys_info.freq_sdhc; 
#endif 
 
 return (gd->cpu_clk != 0) ? 0 : 1; 
} 
 
ulong get_bus_freq(ulong dummy) 
{ 
 if (!gd->bus_clk) 
 get_clocks(); 
 
 return gd->bus_clk; 
} 
 
ulong get_ddr_freq(ulong dummy) 
{ 
 if (!gd->mem_clk) 
 get_clocks(); 
 
 return gd->mem_clk; 
} 
 
#ifdef CONFIG_FSL_ESDHC 
int get_sdhc_freq(ulong dummy) 
{ 
 if (!gd->arch.sdhc_clk) 
 get_clocks(); 
 
 return gd->arch.sdhc_clk; 
} 
#endif 
 
int get_serial_clock(void) 
{ 
 return get_bus_freq(0) / CONFIG_SYS_FSL_DUART_CLK_DIV; 
} 
 
int get_i2c_freq(ulong dummy) 
{ 
 return get_bus_freq(0) / CONFIG_SYS_FSL_I2C_CLK_DIV; 
} 
 
int get_dspi_freq(ulong dummy) 
{ 
 return get_bus_freq(0) / CONFIG_SYS_FSL_DSPI_CLK_DIV; 
} 
 
#ifdef CONFIG_FSL_LPUART 
int get_uart_freq(ulong dummy) 
{ 
 return get_bus_freq(0) / CONFIG_SYS_FSL_LPUART_CLK_DIV; 
} 
#endif 
 
unsigned int mxc_get_clock(enum mxc_clock clk) 
{ 
 switch (clk) { 
 case MXC_I2C_CLK: 
 return get_i2c_freq(0); 
#if defined(CONFIG_FSL_ESDHC) 
 case MXC_ESDHC_CLK: 
 return get_sdhc_freq(0); 
#endif 
 case MXC_DSPI_CLK: 
 return get_dspi_freq(0); 
#ifdef CONFIG_FSL_LPUART 
 case MXC_UART_CLK: 
 return get_uart_freq(0); 
#endif 
 default: 
 printf("Unsupported clock\n"); 
 return 0; 
 } 
} 
 
// By GST @Date ok