// Filename: config.h

#ifndef _ASM_CONFIG_H_
#define _ASM_CONFIG_H_

#define CONFIG_LMB
#define CONFIG_SYS_BOOT_RAMDISK_HIGH

#if defined(CONFIG_ARCH_LS1021A) || \
    defined(CONFIG_CPU_PXA27X) || \
    defined(CONFIG_CPU_MONAHANS) || \
    defined(CONFIG_CPU_PXA25X) || \
    defined(CONFIG_FSL_LAYERSCAPE)
#include <asm/arch/config.h>
#endif

#endif // _ASM_CONFIG_H_

// By GST @Date