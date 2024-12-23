// Filename: spl_onenand.c

#include <common.h>
#include <config.h>
#include <spl.h>
#include <asm/io.h>
#include <onenand_uboot.h>

static int spl_onenand_load_image(struct spl_image_info *spl_image,
                                  struct spl_boot_device *bootdev)
{
    struct image_header *header;
    int ret;

    debug("spl: onenand\n");

    /*use CONFIG_SYS_TEXT_BASE as temporary storage area */
    header = (struct image_header *)(CONFIG_SYS_TEXT_BASE);
    /* Load u-boot */
    onenand_spl_load_image(CONFIG_SYS_ONENAND_U_BOOT_OFFS,
        CONFIG_SYS_ONENAND_PAGE_SIZE, (void *)header);
    ret = spl_parse_image_header(spl_image, header);
    if (ret)
        return ret;
    onenand_spl_load_image(CONFIG_SYS_ONENAND_U_BOOT_OFFS,
        spl_image->size, (void *)spl_image->load_addr);

    return 0;
}
/* Use priorty 1 so that Ubi can override this */
SPL_LOAD_IMAGE_METHOD("OneNAND", 1, BOOT_DEVICE_ONENAND,
                      spl_onenand_load_image);

// By GST @Date