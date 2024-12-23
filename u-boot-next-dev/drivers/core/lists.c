// list.c

#include <common.h>
#include <errno.h>
#include <dm/device.h>
#include <dm/device-internal.h>
#include <dm/lists.h>
#include <dm/platdata.h>
#include <dm/uclass.h>
#include <dm/util.h>
#include <fdtdec.h>
#include <linux/compiler.h>

/**
 * @brief Lookup a driver by its name
 *
 * @param name Name of the driver
 * @return Pointer to the driver, or NULL if not found
 */
struct driver *lists_driver_lookup_name(const char *name)
{
	struct driver *drv = ll_entry_start(struct driver, driver);
	const int n_ents = ll_entry_count(struct driver, driver);
	struct driver *entry;

	for (entry = drv; entry != drv + n_ents; entry++) {
		if (!strcmp(name, entry->name))
			return entry;
	}

	/* Not found */
	return NULL;
}

/**
 * @brief Lookup a uclass driver by its ID
 *
 * @param id Uclass ID
 * @return Pointer to the uclass driver, or NULL if not found
 */
struct uclass_driver *lists_uclass_lookup(enum uclass_id id)
{
	struct uclass_driver *uclass = ll_entry_start(struct uclass_driver, uclass);
	const int n_ents = ll_entry_count(struct uclass_driver, uclass);
	struct uclass_driver *entry;

	for (entry = uclass; entry != uclass + n_ents; entry++) {
		if (entry->id == id)
			return entry;
	}

	return NULL;
}

/**
 * @brief Bind drivers to a parent device
 *
 * @param parent Pointer to the parent device
 * @param pre_reloc_only Flag indicating if only pre-relocation drivers should be bound
 * @return 0 on success, negative error code on failure
 */
int lists_bind_drivers(struct udevice *parent, bool pre_reloc_only)
{
	struct driver_info *info = ll_entry_start(struct driver_info, driver_info);
	const int n_ents = ll_entry_count(struct driver_info, driver_info);
	struct driver_info *entry;
	struct udevice *dev;
	int result = 0;
	int ret;

	for (entry = info; entry != info + n_ents; entry++) {
		ret = device_bind_by_name(parent, pre_reloc_only, entry, &dev);
		if (ret && ret != -EPERM) {
			dm_warn("No match for driver '%s'\n", entry->name);
			if (!result || ret != -ENOENT)
				result = ret;
		}
	}

	return result;
}

/**
 * @brief Bind a driver to a parent device by driver name
 *
 * @param parent Pointer to the parent device
 * @param drv_name Name of the driver
 * @param dev_name Name of the device
 * @param devp Pointer to store the bound device
 * @return 0 on success, negative error code on failure
 */
int device_bind_driver(struct udevice *parent, const char *drv_name,
		       const char *dev_name, struct udevice **devp)
{
	return device_bind_driver_to_node(parent, drv_name, dev_name,
					  ofnode_null(), devp);
}

/**
 * @brief Bind a driver to a parent device by driver name and node
 *
 * @param parent Pointer to the parent device
 * @param drv_name Name of the driver
 * @param dev_name Name of the device
 * @param node ofnode representing the device tree node
 * @param devp Pointer to store the bound device
 * @return 0 on success, negative error code on failure
 */
int device_bind_driver_to_node(struct udevice *parent, const char *drv_name,
			       const char *dev_name, ofnode node,
			       struct udevice **devp)
{
	struct driver *drv;
	int ret;

	drv = lists_driver_lookup_name(drv_name);
	if (!drv) {
		debug("Cannot find driver '%s'\n", drv_name);
		return -ENOENT;
	}
	ret = device_bind_with_driver_data(parent, drv, dev_name, 0 /* data */,
					   node, devp);

	return ret;
}

#if CONFIG_IS_ENABLED(OF_CONTROL) && !CONFIG_IS_ENABLED(OF_PLATDATA)
/**
 * @brief Check if a driver matches a compatible string
 *
 * @param of_match List of compatible strings to match
 * @param of_idp Pointer to store the matched compatible string
 * @param compat Compatible string to search for
 * @return 0 if there is a match, -ENOENT if no match
 */
static int driver_check_compatible(const struct udevice_id *of_match,
				   const struct udevice_id **of_idp,
				   const char *compat)
{
	if (!of_match)
		return -ENOENT;

	while (of_match->compatible) {
		if (!strcmp(of_match->compatible, compat)) {
			*of_idp = of_match;
			return 0;
		}
		of_match++;
	}

	return -ENOENT;
}

/**
 * @brief Bind a device tree node to a driver
 *
 * @param parent Pointer to the parent device
 * @param node ofnode representing the device tree node
 * @param devp Pointer to store the bound device
 * @return 0 on success, negative error code on failure
 */
int lists_bind_fdt(struct udevice *parent, ofnode node, struct udevice **devp)
{
	struct driver *driver = ll_entry_start(struct driver, driver);
	const int n_ents = ll_entry_count(struct driver, driver);
	const struct udevice_id *id;
	struct driver *entry;
	struct udevice *dev;
	bool found = false;
	const char *name, *compat_list, *compat;
	int compat_length, i;
	int result = 0;
	int ret = 0;

	if (devp)
		*devp = NULL;
	name = ofnode_get_name(node);
	pr_debug("bind node %s\n", name);

	compat_list = ofnode_get_property(node, "compatible", &compat_length);
	if (!compat_list) {
		if (compat_length == -FDT_ERR_NOTFOUND) {
			pr_debug("Device '%s' has no compatible string\n",
				 name);
			return 0;
		}

		dm_warn("Device tree error at node '%s'\n", name);
		return compat_length;
	}

	/*
	 * Walk through the compatible string list, attempting to match each
	 * compatible string in order such that we match in order of priority
	 * from the first string to the last.
	 */
	for (i = 0; i < compat_length; i += strlen(compat) + 1) {
		compat = compat_list + i;
		pr_debug("   - attempt to match compatible string '%s'\n",
			 compat);

		for (entry = driver; entry != driver + n_ents; entry++) {
			ret = driver_check_compatible(entry->of_match, &id,
						      compat);
			if (!ret)
				break;
		}
		if (entry == driver + n_ents)
			continue;

		pr_debug("   - found match at '%s'\n", entry->name);
		ret = device_bind_with_driver_data(parent, entry, name,
						   id->data, node, &dev);
		if (ret == -ENODEV) {
			pr_debug("Driver '%s' refuses to bind\n", entry->name);
			continue;
		}
		if (ret) {
			dm_warn("Error binding driver '%s': %d\n", entry->name,
				ret);
			return ret;
		} else {
			found = true;
			if (devp)
				*devp = dev;
		}
		break;
	}

	if (!found && !result && ret != -ENODEV)
		pr_debug("No match for node '%s'\n", name);

	return result;
}
#endif
//GST