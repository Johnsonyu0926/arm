// Filename: ppa.c 
 
#include <common.h> 
#include <malloc.h> 
#include <config.h> 
#include <errno.h> 
#include <asm/system.h> 
#include <asm/types.h> 
#include <asm/arch/soc.h> 
#ifdef CONFIG_FSL_LSCH3 
#include <asm/arch/immap_lsch3.h> 
#elif defined(CONFIG_FSL_LSCH2) 
#include <asm/arch/immap_lsch2.h> 
#endif 
#ifdef CONFIG_ARMV8_SEC_FIRMWARE_SUPPORT 
#include <asm/armv8/sec_firmware.h> 
#endif 
#ifdef CONFIG_CHAIN_OF_TRUST 
#include <fsl_validate.h> 
#endif 
 
#ifdef CONFIG_SYS_LS_PPA_FW_IN_NAND 
#include <nand.h> 
#elif defined(CONFIG_SYS_LS_PPA_FW_IN_MMC) 
#include <mmc.h> 
#endif 
 
DECLARE_GLOBAL_DATA_PTR; 
 
int ppa_init(void) 
{ 
 unsigned int el = current_el(); 
 void *ppa_fit_addr; 
 uint32_t *boot_loc_ptr_l, *boot_loc_ptr_h; 
 int ret; 
 
#ifdef CONFIG_CHAIN_OF_TRUST 
 uintptr_t ppa_esbc_hdr = 0; 
 uintptr_t ppa_img_addr = 0; 
#if defined(CONFIG_SYS_LS_PPA_FW_IN_MMC) || defined(CONFIG_SYS_LS_PPA_FW_IN_NAND) 
 void *ppa_hdr_ddr; 
#endif 
#endif 
 
 /* Skip if running at lower exception level */ 
 if (el < 3) { 
 debug("Skipping PPA init, running at EL%d\n", el); 
 return 0; 
 } 
 
#ifdef CONFIG_SYS_LS_PPA_FW_IN_XIP 
 ppa_fit_addr = (void *)CONFIG_SYS_LS_PPA_FW_ADDR; 
 debug("%s: PPA image load from XIP\n", __func__); 
#ifdef CONFIG_CHAIN_OF_TRUST 
 ppa_esbc_hdr = CONFIG_SYS_LS_PPA_ESBC_ADDR; 
#endif 
#else /* !CONFIG_SYS_LS_PPA_FW_IN_XIP */ 
 size_t fw_length, fdt_header_len = sizeof(struct fdt_header); 
 
 /* Copy PPA image from MMC/SD/NAND to allocated memory */ 
#ifdef CONFIG_SYS_LS_PPA_FW_IN_MMC 
 struct mmc *mmc; 
 int dev = CONFIG_SYS_MMC_ENV_DEV; 
 struct fdt_header *fitp; 
 uint32_t cnt; 
 uint32_t blk; 
 
 debug("%s: PPA image load from eMMC/SD\n", __func__); 
 
 ret = mmc_initialize(gd->bd); 
 if (ret) { 
 printf("%s: mmc_initialize() failed\n", __func__); 
 return ret; 
 } 
 mmc = find_mmc_device(dev); 
 if (!mmc) { 
 printf("PPA: MMC cannot find device for PPA firmware\n"); 
 return -ENODEV; 
 } 
 
 ret = mmc_init(mmc); 
 if (ret) { 
 printf("%s: mmc_init() failed\n", __func__); 
 return ret; 
 } 
 
 fitp = malloc(roundup(fdt_header_len, 512)); 
 if (!fitp) { 
 printf("PPA: malloc failed for FIT header(size 0x%zx)\n", roundup(fdt_header_len, 512)); 
 return -ENOMEM; 
 } 
 
 blk = CONFIG_SYS_LS_PPA_FW_ADDR / 512; 
 cnt = DIV_ROUND_UP(fdt_header_len, 512); 
 debug("%s: MMC read PPA FIT header: dev # %u, block # %u, count %u\n", __func__, dev, blk, cnt); 
 ret = mmc->block_dev.block_read(&mmc->block_dev, blk, cnt, fitp); 
 if (ret != cnt) { 
 free(fitp); 
 printf("MMC/SD read of PPA FIT header at offset 0x%x failed\n", CONFIG_SYS_LS_PPA_FW_ADDR); 
 return -EIO; 
 } 
 
 ret = fdt_check_header(fitp); 
 if (ret) { 
 free(fitp); 
 printf("%s: fdt_check_header() failed\n", __func__); 
 return ret; 
 } 
 
#ifdef CONFIG_CHAIN_OF_TRUST 
 ppa_hdr_ddr = malloc(CONFIG_LS_PPA_ESBC_HDR_SIZE); 
 if (!ppa_hdr_ddr) { 
 printf("PPA: malloc failed for PPA header\n"); 
 return -ENOMEM; 
 } 
 
 blk = CONFIG_SYS_LS_PPA_ESBC_ADDR >> 9; 
 cnt = DIV_ROUND_UP(CONFIG_LS_PPA_ESBC_HDR_SIZE, 512); 
 ret = mmc->block_dev.block_read(&mmc->block_dev, blk, cnt, ppa_hdr_ddr); 
 if (ret != cnt) { 
 free(ppa_hdr_ddr); 
 printf("MMC/SD read of PPA header failed\n"); 
 return -EIO; 
 } 
 debug("Read PPA header to 0x%p\n", ppa_hdr_ddr); 
 
 ppa_esbc_hdr = (uintptr_t)ppa_hdr_ddr; 
#endif 
 
 fw_length = fdt_totalsize(fitp); 
 free(fitp); 
 
 fw_length = roundup(fw_length, 512); 
 ppa_fit_addr = malloc(fw_length); 
 if (!ppa_fit_addr) { 
 printf("PPA: malloc failed for PPA image(size 0x%zx)\n", fw_length); 
 return -ENOMEM; 
 } 
 
 blk = CONFIG_SYS_LS_PPA_FW_ADDR / 512; 
 cnt = DIV_ROUND_UP(fw_length, 512); 
 debug("%s: MMC read PPA FIT image: dev # %u, block # %u, count %u\n", __func__, dev, blk, cnt); 
 ret = mmc->block_dev.block_read(&mmc->block_dev, blk, cnt, ppa_fit_addr); 
 if (ret != cnt) { 
 free(ppa_fit_addr); 
 printf("MMC/SD read of PPA FIT header at offset 0x%x failed\n", CONFIG_SYS_LS_PPA_FW_ADDR); 
 return -EIO; 
 } 
 
#elif defined(CONFIG_SYS_LS_PPA_FW_IN_NAND) 
 struct fdt_header fit; 
 
 debug("%s: PPA image load from NAND\n", __func__); 
 
 nand_init(); 
 ret = nand_read(get_nand_dev_by_index(0), (loff_t)CONFIG_SYS_LS_PPA_FW_ADDR, &fdt_header_len, (u_char *)&fit); 
 if (ret == -EUCLEAN) { 
 printf("NAND read of PPA FIT header at offset 0x%x failed\n", CONFIG_SYS_LS_PPA_FW_ADDR); 
 return -EIO; 
 } 
 
 ret = fdt_check_header(&fit); 
 if (ret) { 
 printf("%s: fdt_check_header() failed\n", __func__); 
 return ret; 
 } 
 
#ifdef CONFIG_CHAIN_OF_TRUST 
 ppa_hdr_ddr = malloc(CONFIG_LS_PPA_ESBC_HDR_SIZE); 
 if (!ppa_hdr_ddr) { 
 printf("PPA: malloc failed for PPA header\n"); 
 return -ENOMEM; 
 } 
 
 fw_length = CONFIG_LS_PPA_ESBC_HDR_SIZE; 
 
 ret = nand_read(get_nand_dev_by_index(0), (loff_t)CONFIG_SYS_LS_PPA_ESBC_ADDR, &fw_length, (u_char *)ppa_hdr_ddr); 
 if (ret == -EUCLEAN) { 
 free(ppa_hdr_ddr); 
 printf("NAND read of PPA firmware at offset 0x%x failed\n", CONFIG_SYS_LS_PPA_FW_ADDR); 
 return -EIO; 
 } 
 debug("Read PPA header to 0x%p\n", ppa_hdr_ddr); 
 
 ppa_esbc_hdr = (uintptr_t)ppa_hdr_ddr; 
#endif 
 
 fw_length = fdt_totalsize(&fit); 
 
 ppa_fit_addr = malloc(fw_length); 
 if (!ppa_fit_addr) { 
 printf("PPA: malloc failed for PPA image(size 0x%zx)\n", fw_length); 
 return -ENOMEM; 
 } 
 
 ret = nand_read(get_nand_dev_by_index(0), (loff_t)CONFIG_SYS_LS_PPA_FW_ADDR, &fw_length, (u_char *)ppa_fit_addr); 
 if (ret == -EUCLEAN) { 
 free(ppa_fit_addr); 
 printf("NAND read of PPA firmware at offset 0x%x failed\n", CONFIG_SYS_LS_PPA_FW_ADDR); 
 return -EIO; 
 } 
#else 
#error "No CONFIG_SYS_LS_PPA_FW_IN_xxx defined" 
#endif 
 
#endif 
 
#ifdef CONFIG_CHAIN_OF_TRUST 
 ppa_img_addr = (uintptr_t)ppa_fit_addr; 
 if (fsl_check_boot_mode_secure() != 0) { 
 ret = fsl_secboot_validate(ppa_esbc_hdr, PPA_KEY_HASH, &ppa_img_addr); 
 if (ret != 0) 
 printf("PPA validation failed\n"); 
 else 
 printf("PPA validation Successful\n"); 
 } 
#if defined(CONFIG_SYS_LS_PPA_FW_IN_MMC) || defined(CONFIG_SYS_LS_PPA_FW_IN_NAND) 
 free(ppa_hdr_ddr); 
#endif 
#endif 
 
#ifdef CONFIG_FSL_LSCH3 
 struct ccsr_gur __iomem *gur = (void *)(CONFIG_SYS_FSL_GUTS_ADDR); 
 boot_loc_ptr_l = &gur->bootlocptrl; 
 boot_loc_ptr_h = &gur->bootlocptrh; 
#elif defined(CONFIG_FSL_LSCH2) 
 struct ccsr_scfg __iomem *scfg = (void *)(CONFIG_SYS_FSL_SCFG_ADDR); 
 boot_loc_ptr_l = &scfg->scratchrw[1]; 
 boot_loc_ptr_h = &scfg->scratchrw[0]; 
#endif 
 
 debug("fsl-ppa: boot_loc_ptr_l = 0x%p, boot_loc_ptr_h =0x%p\n", boot_loc_ptr_l, boot_loc_ptr_h); 
 ret = sec_firmware_init(ppa_fit_addr, boot_loc_ptr_l, boot_loc_ptr_h); 
 
#if defined(CONFIG_SYS_LS_PPA_FW_IN_MMC) || defined(CONFIG_SYS_LS_PPA_FW_IN_NAND) 
 free(ppa_fit_addr); 
#endif 
 
 return ret; 
} 
 
// By GST @Date ok