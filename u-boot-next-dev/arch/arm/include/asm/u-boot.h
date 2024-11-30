// Filename: u-boot.h

#ifndef _U_BOOT_H_
#define _U_BOOT_H_ 1

/* Use the generic board which requires a unified bd_info */
#include <asm-generic/u-boot.h>
#include <asm/u-boot-arm.h>

/* For image.h:image_check_target_arch() */
#ifndef CONFIG_ARM64
#define IH_ARCH_DEFAULT IH_ARCH_ARM
#else
#define IH_ARCH_DEFAULT IH_ARCH_ARM64
#endif

#endif /* _U_BOOT_H_ */

// By GST @Date