// Filename: spl_net.c

#include <common.h>
#include <errno.h>
#include <spl.h>
#include <net.h>
#include <linux/libfdt.h>

DECLARE_GLOBAL_DATA_PTR;

#if defined(CONFIG_SPL_ETH_SUPPORT) || defined(CONFIG_SPL_USBETH_SUPPORT)
static ulong spl_net_load_read(struct spl_load_info *load, ulong sector,
                               ulong count, void *buf)
{
    debug("%s: sector %lx, count %lx, buf %lx\n",
          __func__, sector, count, (ulong)buf);
    memcpy(buf, (void *)(load_addr + sector), count);
    return count;
}

static int spl_net_load_image(struct spl_image_info *spl_image,
                              struct spl_boot_device *bootdev)
{
    struct image_header *header = (struct image_header *)load_addr;
    int rv;

    env_init();
    env_relocate();
    env_set("autoload", "yes");
    rv = eth_initialize();
    if (rv == 0) {
        printf("No Ethernet devices found\n");
        return -ENODEV;
    }
    if (bootdev->boot_device_name)
        env_set("ethact", bootdev->boot_device_name);
    rv = net_loop(BOOTP);
    if (rv < 0) {
        printf("Problem booting with BOOTP\n");
        return rv;
    }

#ifdef CONFIG_SPL_FIT_IMAGE_MULTIPLE
    if ((IS_ENABLED(CONFIG_SPL_LOAD_FIT) &&
         image_get_magic(header) == FDT_MAGIC) ||
         CONFIG_SPL_FIT_IMAGE_MULTIPLE > 1) {
#else
    if (IS_ENABLED(CONFIG_SPL_LOAD_FIT) &&
        image_get_magic(header) == FDT_MAGIC) {
#endif
        struct spl_load_info load;

        debug("Found FIT\n");
        load.bl_len = 1;
        load.read = spl_net_load_read;
        rv = spl_load_simple_fit(spl_image, &load, 0, header);
    } else {
        debug("Legacy image\n");

        rv = spl_parse_image_header(spl_image, header);
        if (rv)
            return rv;

        memcpy((void *)spl_image->load_addr, header, spl_image->size);
    }

    return rv;
}
#endif

#ifdef CONFIG_SPL_ETH_SUPPORT
int spl_net_load_image_cpgmac(struct spl_image_info *spl_image,
                              struct spl_boot_device *bootdev)
{
#ifdef CONFIG_SPL_ETH_DEVICE
    bootdev->boot_device_name = CONFIG_SPL_ETH_DEVICE;
#endif

    return spl_net_load_image(spl_image, bootdev);
}
SPL_LOAD_IMAGE_METHOD("eth device", 0, BOOT_DEVICE_CPGMAC,
                      spl_net_load_image_cpgmac);
#endif

#ifdef CONFIG_SPL_USBETH_SUPPORT
int spl_net_load_image_usb(struct spl_image_info *spl_image,
                           struct spl_boot_device *bootdev)
{
    bootdev->boot_device_name = "usb_ether";

    return spl_net_load_image(spl_image, bootdev);
}
SPL_LOAD_IMAGE_METHOD("USB eth", 0, BOOT_DEVICE_USBETH, spl_net_load_image_usb);
#endif

// By GST @Date