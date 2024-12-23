// Filename: spl_bootroom.c

#include <common.h>
#include <spl.h>

__weak void board_return_to_bootrom(void)
{
}

static int spl_return_to_bootrom(struct spl_image_info *spl_image,
                                 struct spl_boot_device *bootdev)
{
    /*
     * If the board implements a way to return to its ROM (with
     * the expectation that the next stage of will be booted by
     * the ROM), it will implement board_return_to_bootrom() and
     * should not return from it.
     */
    board_return_to_bootrom();
    return false;
}

SPL_LOAD_IMAGE_METHOD("BOOTROM", 0, BOOT_DEVICE_BOOTROM, spl_return_to_bootrom);

// By GST @Date