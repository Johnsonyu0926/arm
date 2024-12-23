// Filename: spl.h

#ifndef _ASM_SPL_H_
#define _ASM_SPL_H_

#if defined(CONFIG_ARCH_OMAP2PLUS) \
    || defined(CONFIG_EXYNOS4) || defined(CONFIG_EXYNOS5) \
    || defined(CONFIG_EXYNOS4210)
/* Platform-specific defines */
#include <asm/arch/spl.h>

#else
enum {
    BOOT_DEVICE_RAM,
    BOOT_DEVICE_MMC1,
    BOOT_DEVICE_MMC2,
    BOOT_DEVICE_MMC2_2,
    BOOT_DEVICE_NAND,
    BOOT_DEVICE_ONENAND,
    BOOT_DEVICE_NOR,
    BOOT_DEVICE_UART,
    BOOT_DEVICE_SPI,
    BOOT_DEVICE_USB,
    BOOT_DEVICE_SATA,
    BOOT_DEVICE_I2C,
    BOOT_DEVICE_BOARD,
    BOOT_DEVICE_DFU,
    BOOT_DEVICE_XIP,
    BOOT_DEVICE_BOOTROM,
    BOOT_DEVICE_MTD_BLK_NAND,
    BOOT_DEVICE_MTD_BLK_SPI_NAND,
    BOOT_DEVICE_MTD_BLK_SPI_NOR,
    BOOT_DEVICE_RKNAND,
    BOOT_DEVICE_UFS,
    BOOT_DEVICE_NONE
};
#endif

/* Linker symbols. */
extern char __bss_start[], __bss_end[];

#ifndef CONFIG_DM
extern gd_t gdata;
#endif

#endif /* _ASM_SPL_H_ */

// By GST @Date