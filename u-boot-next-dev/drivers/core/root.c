// root.c

#include <common.h>
#include <errno.h>
#include <fdtdec.h>
#include <malloc.h>
#include <linux/libfdt.h>
#include <dm/device.h>
#include <dm/device-internal.h>
#include <dm/lists.h>
#include <dm/of.h>
#include <dm/of_access.h>
#include <dm/platdata.h>
#include <dm/read.h>
#include <dm/root.h>
#include <dm/uclass.h>
#include <dm/util.h>
#include <linux/list.h>

DECLARE_GLOBAL_DATA_PTR;

struct root_priv {
	fdt_addr_t translation_offset;	/* optional translation offset */
};

static const struct driver_info root_info = {
	.name		= "root_driver",
};

/**
 * @brief Get the root device
 *
 * @return Pointer to the root device, or NULL if not found
 */
struct udevice *dm_root(void)
{
	if (!gd->dm_root) {
		dm_warn("Virtual root driver does not exist!\n");
		return NULL;
	}

	return gd->dm_root;
}

/**
 * @brief Fix up global data pointers for GD move
 *
 * @param new_gd Pointer to the new global data
 */
void dm_fixup_for_gd_move(struct global_data *new_gd)
{
	/* The sentinel node has moved, so update things that point to it */
	if (gd->dm_root) {
		new_gd->uclass_root.next->prev = &new_gd->uclass_root;
		new_gd->uclass_root.prev->next = &new_gd->uclass_root;
	}
}

/**
 * @brief Get the translation offset for the root device
 *
 * @return Translation offset
 */
fdt_addr_t dm_get_translation_offset(void)
{
	struct udevice *root = dm_root();
	struct root_priv *priv = dev_get_priv(root);

	return priv->translation_offset;
}

/**
 * @brief Set the translation offset for the root device
 *
 * @param offs Translation offset
 */
void dm_set_translation_offset(fdt_addr_t offs)
{
	struct udevice *root = dm_root();
	struct root_priv *priv = dev_get_priv(root);

	priv->translation_offset = offs;
}

#if defined(CONFIG_NEEDS_MANUAL_RELOC)
/**
 * @brief Fix driver pointers after relocation
 */
void fix_drivers(void)
{
	struct driver *drv = ll_entry_start(struct driver, driver);
	const int n_ents = ll_entry_count(struct driver, driver);
	struct driver *entry;

	for (entry = drv; entry != drv + n_ents; entry++) {
		if (entry->of_match)
			entry->of_match = (const struct udevice_id *)((u32)entry->of_match + gd->reloc_off);
		if (entry->bind)
			entry->bind += gd->reloc_off;
		if (entry->probe)
			entry->probe += gd->reloc_off;
		if (entry->remove)
			entry->remove += gd->reloc_off;
		if (entry->unbind)
			entry->unbind += gd->reloc_off;
		if (entry->ofdata_to_platdata)
			entry->ofdata_to_platdata += gd->reloc_off;
		if (entry->child_post_bind)
			entry->child_post_bind += gd->reloc_off;
		if (entry->child_pre_probe)
			entry->child_pre_probe += gd->reloc_off;
		if (entry->child_post_remove)
			entry->child_post_remove += gd->reloc_off;
		if (entry->ops)
			entry->ops += gd->reloc_off;
	}
}

/**
 * @brief Fix uclass pointers after relocation
 */
void fix_uclass(void)
{
	struct uclass_driver *uclass = ll_entry_start(struct uclass_driver, uclass);
	const int n_ents = ll_entry_count(struct uclass_driver, uclass);
	struct uclass_driver *entry;

	for (entry = uclass; entry != uclass + n_ents; entry++) {
		if (entry->post_bind)
			entry->post_bind += gd->reloc_off;
		if (entry->pre_unbind)
			entry->pre_unbind += gd->reloc_off;
		if (entry->pre_probe)
			entry->pre_probe += gd->reloc_off;
		if (entry->post_probe)
			entry->post_probe += gd->reloc_off;
		if (entry->pre_remove)
			entry->pre_remove += gd->reloc_off;
		if (entry->child_post_bind)
			entry->child_post_bind += gd->reloc_off;
		if (entry->child_pre_probe)
			entry->child_pre_probe += gd->reloc_off;
		if (entry->init)
			entry->init += gd->reloc_off;
		if (entry->destroy)
			entry->destroy += gd->reloc_off;
		if (entry->ops)
			entry->ops += gd->reloc_off;
	}
}

/**
 * @brief Fix device pointers after relocation
 */
void fix_devices(void)
{
	struct driver_info *dev = ll_entry_start(struct driver_info, driver_info);
	const int n_ents = ll_entry_count(struct driver_info, driver_info);
	struct driver_info *entry;

	for (entry = dev; entry != dev + n_ents; entry++) {
		if (entry->platdata)
			entry->platdata += gd->reloc_off;
	}
}
#endif

/**
 * @brief Initialize the device model
 *
 * @param of_live Flag indicating if live device tree is used
 * @return 0 on success, negative error code on failure
 */
int dm_init(bool of_live)
{
	int ret;

	if (gd->dm_root) {
		dm_warn("Virtual root driver already exists!\n");
		return -EINVAL;
	}
	INIT_LIST_HEAD(&DM_UCLASS_ROOT_NON_CONST);

#if defined(CONFIG_NEEDS_MANUAL_RELOC)
	fix_drivers();
	fix_uclass();
	fix_devices();
#endif

	ret = device_bind_by_name(NULL, false, &root_info, &DM_ROOT_NON_CONST);
	if (ret)
		return ret;
#if CONFIG_IS_ENABLED(OF_CONTROL)
# if CONFIG_IS_ENABLED(OF_LIVE)
	if (of_live)
		DM_ROOT_NON_CONST->node = np_to_ofnode(gd->of_root);
	else
#endif
		DM_ROOT_NON_CONST->node = offset_to_ofnode(0);
#endif
	ret = device_probe(DM_ROOT_NON_CONST);
	if (ret)
		return ret;

	return 0;
}

/**
 * @brief Uninitialize the device model
 *
 * @return 0 on success, negative error code on failure
 */
int dm_uninit(void)
{
	device_remove(dm_root(), DM_REMOVE_NORMAL);
	device_unbind(dm_root());

	return 0;
}

#if CONFIG_IS_ENABLED(DM_DEVICE_REMOVE)
/**
 * @brief Remove devices with specified flags
 *
 * @param flags Flags for device removal
 * @return 0 on success, negative error code on failure
 */
int dm_remove_devices_flags(uint flags)
{
	device_remove(dm_root(), flags);

	return 0;
}
#endif

/**
 * @brief Scan platform data and bind drivers
 *
 * @param pre_reloc_only Flag indicating if only pre-relocation drivers should be bound
 * @return 0 on success, negative error code on failure
 */
int dm_scan_platdata(bool pre_reloc_only)
{
	int ret;

	ret = lists_bind_drivers(DM_ROOT_NON_CONST, pre_reloc_only);
	if (ret == -ENOENT) {
		dm_warn("Some drivers were not found\n");
		ret = 0;
	}

	return ret;
}

#if CONFIG_IS_ENABLED(OF_LIVE)
/**
 * @brief Scan the live device tree and bind drivers
 *
 * @param parent Parent device
 * @param node_parent Parent node in the live device tree
 * @param pre_reloc_only Flag indicating if only pre-relocation drivers should be bound
 * @return 0 on success, negative error code on failure
 */
static int dm_scan_fdt_live(struct udevice *parent,
			    const struct device_node *node_parent,
			    bool pre_reloc_only)
{
	struct device_node *np;
	int ret = 0, err;

	for (np = node_parent->child; np; np = np->sibling) {
		if (pre_reloc_only &&
		   (!of_find_property(np, "u-boot,dm-pre-reloc", NULL) &&
		    !of_find_property(np, "u-boot,dm-spl", NULL)))
			continue;
		if (!of_device_is_available(np)) {
			pr_debug("   - ignoring disabled device\n");
			continue;
		}
		err = lists_bind_fdt(parent, np_to_ofnode(np), NULL);
		if (err && !ret) {
			ret = err;
			debug("%s: ret=%d\n", np->name, ret);
		}

		/* There is no compatible in "/firmware", bind it by default. */
		if (!pre_reloc_only && !strcmp(np->name, "firmware"))
			ret = device_bind_driver_to_node(gd->dm_root,
				"firmware", np->name, np_to_ofnode(np), NULL);
	}

	if (ret)
		dm_warn("Some drivers failed to bind\n");

	return ret;
}
#endif /* CONFIG_IS_ENABLED(OF_LIVE) */

#if CONFIG_IS_ENABLED(OF_CONTROL) && !CONFIG_IS_ENABLED(OF_PLATDATA)
/**
 * @brief Scan the device tree and bind drivers for a node
 *
 * This scans the subnodes of a device tree node and creates a driver for each one.
 *
 * @param parent Parent device for the devices that will be created
 * @param blob Pointer to device tree blob
 * @param offset Offset of node to scan
 * @param pre_reloc_only Flag indicating if only pre-relocation drivers should be bound
 * @return 0 on success, negative error code on failure
 */
static int dm_scan_fdt_node(struct udevice *parent, const void *blob,
			    int offset, bool pre_reloc_only)
{
	int ret = 0, err;

	for (offset = fdt_first_subnode(blob, offset);
	     offset > 0;
	     offset = fdt_next_subnode(blob, offset)) {
		if (pre_reloc_only &&
		    !dm_fdt_pre_reloc(blob, offset))
			continue;
		if (!fdtdec_get_is_enabled(blob, offset)) {
			pr_debug("   - ignoring disabled device\n");
			continue;
		}
		err = lists_bind_fdt(parent, offset_to_ofnode(offset), NULL);
		if (err && !ret) {
			ret = err;
			debug("%s: ret=%d\n", fdt_get_name(blob, offset, NULL),
			      ret);
		}

#if CONFIG_IS_ENABLED(SCMI_FIRMWARE)
		const char *name;

		/* There is no compatible in "/firmware", bind it by default. */
		name = fdt_get_name(blob, offset, NULL);
		if (name && !strcmp(name, "firmware"))
			ret = device_bind_driver_to_node(parent, "firmware",
					name, offset_to_ofnode(offset), NULL);
#endif
	}

	if (ret)
		dm_warn("Some drivers failed to bind\n");

	return ret;
}

/**
 * @brief Scan the device tree and bind drivers for a device
 *
 * @param dev Pointer to the device
 * @return 0 on success, negative error code on failure
 */
int dm_scan_fdt_dev(struct udevice *dev)
{
	if (!dev_of_valid(dev))
		return 0;

#if CONFIG_IS_ENABLED(OF_LIVE)
	if (of_live_active())
		return dm_scan_fdt_live(dev, dev_np(dev),
				gd->flags & GD_FLG_RELOC ? false : true);
	else
#endif
	return dm_scan_fdt_node(dev, gd->fdt_blob, dev_of_offset(dev),
				gd->flags & GD_FLG_RELOC ? false : true);
}

/**
 * @brief Scan the device tree and bind drivers
 *
 * @param blob Pointer to device tree blob
 * @param pre_reloc_only Flag indicating if only pre-relocation drivers should be bound
 * @return 0 on success, negative error code on failure
 */
int dm_scan_fdt(const void *blob, bool pre_reloc_only)
{
#if CONFIG_IS_ENABLED(OF_LIVE)
	if (of_live_active())
		return dm_scan_fdt_live(gd->dm_root, gd->of_root,
					pre_reloc_only);
	else
#endif
	return dm_scan_fdt_node(gd->dm_root, blob, 0, pre_reloc_only);
}
#else
static int dm_scan_fdt_node(struct udevice *parent, const void *blob,
			    int offset, bool pre_reloc_only)
{
	return 0;
}
#endif

/**
 * @brief Extended scan of the device tree and bind drivers
 *
 * @param blob Pointer to device tree blob
 * @param pre_reloc_only Flag indicating if only pre-relocation drivers should be bound
 * @return 0 on success, negative error code on failure
 */
int dm_extended_scan_fdt(const void *blob, bool pre_reloc_only)
{
	int node, ret;

	ret = dm_scan_fdt(gd->fdt_blob, pre_reloc_only);
	if (ret) {
		debug("dm_scan_fdt() failed: %d\n", ret);
		return ret;
	}

	/* bind fixed-clock */
	node = ofnode_to_offset(ofnode_path("/clocks"));
	/* if no DT "clocks" node, no need to go further */
	if (node < 0)
		return ret;

	ret = dm_scan_fdt_node(gd->dm_root, gd->fdt_blob, node,
			       pre_reloc_only);
	if (ret)
		debug("dm_scan_fdt_node() failed: %d\n", ret);

	return ret;
}

/**
 * @brief Weak function for scanning other devices
 *
 * @param pre_reloc_only Flag indicating if only pre-relocation drivers should be bound
 * @return 0 on success, negative error code on failure
 */
__weak int dm_scan_other(bool pre_reloc_only)
{
	return 0;
}

/**
 * @brief Initialize and scan the device model
 *
 * @param pre_reloc_only Flag indicating if only pre-relocation drivers should be bound
 * @return 0 on success, negative error code on failure
 */
int dm_init_and_scan(bool pre_reloc_only)
{
	int ret;

	ret = dm_init(IS_ENABLED(CONFIG_OF_LIVE));
	if (ret) {
		debug("dm_init() failed: %d\n", ret);
		return ret;
	}
	ret = dm_scan_platdata(pre_reloc_only);
	if (ret) {
		debug("dm_scan_platdata() failed: %d\n", ret);
		return ret;
	}

	if (CONFIG_IS_ENABLED(OF_CONTROL) && !CONFIG_IS_ENABLED(OF_PLATDATA)) {
		ret = dm_extended_scan_fdt(gd->fdt_blob, pre_reloc_only);
		if (ret) {
			debug("dm_extended_scan_dt() failed: %d\n", ret);
			return ret;
		}
	}

	ret = dm_scan_other(pre_reloc_only);
	if (ret)
		return ret;

	return 0;
}

/* This is the root driver - all drivers are children of this */
U_BOOT_DRIVER(root_driver) = {
	.name	= "root_driver",
	.id	= UCLASS_ROOT,
	.priv_auto_alloc_size = sizeof(struct root_priv),
};

/* This is the root uclass */
UCLASS_DRIVER(root) = {
	.name	= "root",
	.id	= UCLASS_ROOT,
};
//GST