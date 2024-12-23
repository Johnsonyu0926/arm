// Filename: spl_usb.c

#include <common.h>
#include <spl.h>
#include <asm/u-boot.h>
#include <errno.h>
#include <usb.h>
#include <fat.h>

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_USB_STORAGE
static int usb_stor_curr_dev = -1; /* current device */
#endif

static int spl_usb_load_image(struct spl_image_info *spl_image,
                              struct spl_boot_device *bootdev)
{
    int err;
    struct blk_desc *stor_dev;

    usb_stop();
    err = usb_init();
    if (err) {
#ifdef CONFIG_SPL_LIBCOMMON_SUPPORT
        printf("%s: usb init failed: err - %d\n", __func__, err);
#endif
        return err;
    }

#ifdef CONFIG_USB_STORAGE
    /* try to recognize storage devices immediately */
    usb_stor_curr_dev = usb_stor_scan(1);
    stor_dev = blk_get_devnum_by_type(IF_TYPE_USB, usb_stor_curr_dev);
    if (!stor_dev)
        return -ENODEV;
#endif

    debug("boot mode - FAT\n");

#ifdef CONFIG_SPL_OS_BOOT
    if (spl_start_uboot() ||
        spl_load_image_fat_os(spl_image, stor_dev,
                              CONFIG_SYS_USB_FAT_BOOT_PARTITION))
#endif
    {
        err = spl_load_image_fat(spl_image, stor_dev,
                                CONFIG_SYS_USB_FAT_BOOT_PARTITION,
                                CONFIG_SPL_FS_LOAD_PAYLOAD_NAME);
    }

    if (err) {
        puts("Error loading from USB device\n");
        return err;
    }

    return 0;
}
SPL_LOAD_IMAGE_METHOD("USB", 0, BOOT_DEVICE_USB, spl_usb_load_image);

// By GST @Date