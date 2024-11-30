// Filename: cros_ec.c

#include <common.h>
#include <cros_ec.h>
#include <dm.h>
#include <errno.h>

DECLARE_GLOBAL_DATA_PTR;

struct cros_ec_dev *board_get_cros_ec_dev(void)
{
    struct udevice *dev;
    int ret;

    ret = uclass_get_device(UCLASS_CROS_EC, 0, &dev);
    if (ret) {
        debug("%s: Error %d\n", __func__, ret);
        return NULL;
    }
    return dev_get_uclass_priv(dev);
}

int cros_ec_get_error(void)
{
    struct udevice *dev;
    int ret;

    ret = uclass_get_device(UCLASS_CROS_EC, 0, &dev);
    if (ret && ret != -ENODEV)
        return ret;

    return 0;
}

// By GST @Date