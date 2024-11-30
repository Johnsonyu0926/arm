// device-remove.c

#include <common.h>
#include <errno.h>
#include <malloc.h>
#include <dm/device.h>
#include <dm/device-internal.h>
#include <dm/uclass.h>
#include <dm/uclass-internal.h>
#include <dm/util.h>

/**
 * @brief Unbind all children of a device
 *
 * @param dev Pointer to the device
 * @return 0 on success, negative error code on failure
 */
static int device_chld_unbind(struct udevice *dev)
{
	struct udevice *pos, *n;
	int ret, saved_ret = 0;

	assert(dev);

	list_for_each_entry_safe(pos, n, &dev->child_head, sibling_node) {
		ret = device_unbind(pos);
		if (ret && !saved_ret)
			saved_ret = ret;
	}

	return saved_ret;
}

/**
 * @brief Remove all children of a device
 *
 * @param dev Pointer to the device
 * @param flags Flags for device removal
 * @return 0 on success, negative error code on failure
 */
static int device_chld_remove(struct udevice *dev, uint flags)
{
	struct udevice *pos, *n;
	int ret;

	assert(dev);

	list_for_each_entry_safe(pos, n, &dev->child_head, sibling_node) {
		ret = device_remove(pos, flags);
		if (ret)
			return ret;
	}

	return 0;
}

/**
 * @brief Unbind a device
 *
 * @param dev Pointer to the device
 * @return 0 on success, negative error code on failure
 */
int device_unbind(struct udevice *dev)
{
	const struct driver *drv;
	int ret;

	if (!dev)
		return -EINVAL;

	if (dev->flags & DM_FLAG_ACTIVATED)
		return -EINVAL;

	if (!(dev->flags & DM_FLAG_BOUND))
		return -EINVAL;

	drv = dev->driver;
	assert(drv);

	if (drv->unbind) {
		ret = drv->unbind(dev);
		if (ret)
			return ret;
	}

	ret = device_chld_unbind(dev);
	if (ret)
		return ret;

	if (dev->flags & DM_FLAG_ALLOC_PDATA) {
		free(dev->platdata);
		dev->platdata = NULL;
	}
	if (dev->flags & DM_FLAG_ALLOC_UCLASS_PDATA) {
		free(dev->uclass_platdata);
		dev->uclass_platdata = NULL;
	}
	if (dev->flags & DM_FLAG_ALLOC_PARENT_PDATA) {
		free(dev->parent_platdata);
		dev->parent_platdata = NULL;
	}
	ret = uclass_unbind_device(dev);
	if (ret)
		return ret;

	if (dev->parent)
		list_del(&dev->sibling_node);

	devres_release_all(dev);

	if (dev->flags & DM_FLAG_NAME_ALLOCED)
		free((char *)dev->name);
	free(dev);

	return 0;
}

/**
 * @brief Free memory buffers allocated by a device
 *
 * @param dev Pointer to the device
 */
void device_free(struct udevice *dev)
{
	int size;

	if (dev->driver->priv_auto_alloc_size) {
		free(dev->priv);
		dev->priv = NULL;
	}
	size = dev->uclass->uc_drv->per_device_auto_alloc_size;
	if (size) {
		free(dev->uclass_priv);
		dev->uclass_priv = NULL;
	}
	if (dev->parent) {
		size = dev->parent->driver->per_child_auto_alloc_size;
		if (!size) {
			size = dev->parent->uclass->uc_drv->per_child_auto_alloc_size;
		}
		if (size) {
			free(dev->parent_priv);
			dev->parent_priv = NULL;
		}
	}

	devres_release_probe(dev);
}

/**
 * @brief Check if the flags indicate that the device should be removed
 *
 * @param flags Flags for device removal
 * @param drv_flags Driver flags
 * @return true if the device should be removed, false otherwise
 */
static bool flags_remove(uint flags, uint drv_flags)
{
	if ((flags & DM_REMOVE_NORMAL) ||
	    (flags & (drv_flags & (DM_FLAG_ACTIVE_DMA | DM_FLAG_OS_PREPARE))))
		return true;

	return false;
}

/**
 * @brief Remove a device
 *
 * @param dev Pointer to the device
 * @param flags Flags for device removal
 * @return 0 on success, negative error code on failure
 */
int device_remove(struct udevice *dev, uint flags)
{
	const struct driver *drv;
	int ret;

	if (!dev)
		return -EINVAL;

	if (!(dev->flags & DM_FLAG_ACTIVATED))
		return 0;

	drv = dev->driver;
	assert(drv);

	ret = uclass_pre_remove_device(dev);
	if (ret)
		return ret;

	ret = device_chld_remove(dev, flags);
	if (ret)
		goto err;

	/* Remove the device if called with the "normal" remove flag set, or if the remove flag matches any of the drivers remove flags */
	if (drv->remove && flags_remove(flags, drv->flags)) {
		ret = drv->remove(dev);
		if (ret)
			goto err_remove;
	}

	if (dev->parent && dev->parent->driver->child_post_remove) {
		ret = dev->parent->driver->child_post_remove(dev);
		if (ret) {
			dm_warn("%s: Device '%s' failed child_post_remove()",
				__func__, dev->name);
		}
	}

	if (flags_remove(flags, drv->flags)) {
		device_free(dev);

		dev->seq = -1;
		dev->flags &= ~DM_FLAG_ACTIVATED;
	}

	return ret;

err_remove:
	/* We can't put the children back */
	dm_warn("%s: Device '%s' failed to remove, but children are gone\n",
		__func__, dev->name);
err:
	ret = uclass_post_probe_device(dev);
	if (ret) {
		dm_warn("%s: Device '%s' failed to post_probe on error path\n",
			__func__, dev->name);
	}

	return ret;
}
//GST