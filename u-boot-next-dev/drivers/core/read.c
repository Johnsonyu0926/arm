// read.c

#include <common.h>
#include <dm.h>
#include <mapmem.h>
#include <asm/io.h>
#include <dm/of_access.h>

/**
 * @brief Read a 32-bit integer property from a device, with a default value
 *
 * @param dev Pointer to the device
 * @param propname Name of the property
 * @param def Default value
 * @return Property value, or default value if not found
 */
int dev_read_u32_default(struct udevice *dev, const char *propname, int def)
{
	return ofnode_read_u32_default(dev_ofnode(dev), propname, def);
}

/**
 * @brief Read a signed 32-bit integer property from a device, with a default value
 *
 * @param dev Pointer to the device
 * @param propname Name of the property
 * @param def Default value
 * @return Property value, or default value if not found
 */
int dev_read_s32_default(struct udevice *dev, const char *propname, int def)
{
	return ofnode_read_s32_default(dev_ofnode(dev), propname, def);
}

/**
 * @brief Read a string property from a device
 *
 * @param dev Pointer to the device
 * @param propname Name of the property
 * @return Pointer to the string, or NULL if not found
 */
const char *dev_read_string(struct udevice *dev, const char *propname)
{
	return ofnode_read_string(dev_ofnode(dev), propname);
}

/**
 * @brief Read a boolean property from a device
 *
 * @param dev Pointer to the device
 * @param propname Name of the property
 * @return true if the property is present, false otherwise
 */
bool dev_read_bool(struct udevice *dev, const char *propname)
{
	return ofnode_read_bool(dev_ofnode(dev), propname);
}

/**
 * @brief Read a subnode from a device
 *
 * @param dev Pointer to the device
 * @param subnode_name Name of the subnode
 * @return ofnode representing the subnode, or ofnode_null() if not found
 */
ofnode dev_read_subnode(struct udevice *dev, const char *subnode_name)
{
	return ofnode_find_subnode(dev_ofnode(dev), subnode_name);
}

/**
 * @brief Read the first subnode from a device
 *
 * @param dev Pointer to the device
 * @return ofnode representing the first subnode, or ofnode_null() if not found
 */
ofnode dev_read_first_subnode(struct udevice *dev)
{
	return ofnode_first_subnode(dev_ofnode(dev));
}

/**
 * @brief Read the next subnode from a device
 *
 * @param node Current ofnode
 * @return ofnode representing the next subnode, or ofnode_null() if not found
 */
ofnode dev_read_next_subnode(ofnode node)
{
	return ofnode_next_subnode(node);
}

/**
 * @brief Read the size of a property from a device
 *
 * @param dev Pointer to the device
 * @param propname Name of the property
 * @return Size of the property, or negative error code on failure
 */
int dev_read_size(struct udevice *dev, const char *propname)
{
	return ofnode_read_size(dev_ofnode(dev), propname);
}

/**
 * @brief Read an address from a device by index
 *
 * @param dev Pointer to the device
 * @param index Index of the address
 * @return Address, or FDT_ADDR_T_NONE on failure
 */
fdt_addr_t dev_read_addr_index(struct udevice *dev, int index)
{
	if (ofnode_is_np(dev_ofnode(dev)))
		return ofnode_get_addr_index(dev_ofnode(dev), index);
	else
		return devfdt_get_addr_index(dev, index);
}

/**
 * @brief Read the first address from a device
 *
 * @param dev Pointer to the device
 * @return Address, or FDT_ADDR_T_NONE on failure
 */
fdt_addr_t dev_read_addr(struct udevice *dev)
{
	return dev_read_addr_index(dev, 0);
}

/**
 * @brief Remap an address from a device by index
 *
 * @param dev Pointer to the device
 * @param index Index of the address
 * @return Pointer to the remapped address, or NULL on failure
 */
void *dev_remap_addr_index(struct udevice *dev, int index)
{
        fdt_addr_t addr = dev_read_addr_index(dev, index);

        if (addr == FDT_ADDR_T_NONE)
                return NULL;

        return map_physmem(addr, 0, MAP_NOCACHE);
}

/**
 * @brief Read and remap the first address from a device
 *
 * @param dev Pointer to the device
 * @return Pointer to the remapped address, or NULL on failure
 */
void *dev_read_addr_ptr(struct udevice *dev)
{
	fdt_addr_t addr = dev_read_addr(dev);

	return (addr == FDT_ADDR_T_NONE) ? NULL : map_sysmem(addr, 0);
}

/**
 * @brief Read an address and size from a device
 *
 * @param dev Pointer to the device
 * @param property Name of the property
 * @param sizep Pointer to store the size
 * @return Address, or FDT_ADDR_T_NONE on failure
 */
fdt_addr_t dev_read_addr_size(struct udevice *dev, const char *property,
				fdt_size_t *sizep)
{
	return ofnode_get_addr_size(dev_ofnode(dev), property, sizep);
}

/**
 * @brief Read the name of a device
 *
 * @param dev Pointer to the device
 * @return Name of the device, or NULL if not found
 */
const char *dev_read_name(struct udevice *dev)
{
	if (!dev_of_valid(dev))
		return NULL;

	return ofnode_get_name(dev_ofnode(dev));
}

/**
 * @brief Search for a string in a string list property of a device
 *
 * @param dev Pointer to the device
 * @param property Name of the property
 * @param string String to search for
 * @return Index of the string, or negative error code on failure
 */
int dev_read_stringlist_search(struct udevice *dev, const char *property,
			  const char *string)
{
	return ofnode_stringlist_search(dev_ofnode(dev), property, string);
}

/**
 * @brief Read a string from a string list property of a device by index
 *
 * @param dev Pointer to the device
 * @param propname Name of the property
 * @param index Index of the string
 * @param outp Pointer to store the string
 * @return 0 on success, negative error code on failure
 */
int dev_read_string_index(struct udevice *dev, const char *propname, int index,
			  const char **outp)
{
	return ofnode_read_string_index(dev_ofnode(dev), propname, index, outp);
}

/**
 * @brief Get the count of strings in a string list property of a device
 *
 * @param dev Pointer to the device
 * @param propname Name of the property
 * @return Count of strings, or negative error code on failure
 */
int dev_read_string_count(struct udevice *dev, const char *propname)
{
	return ofnode_read_string_count(dev_ofnode(dev), propname);
}

/**
 * @brief Read a phandle with arguments from a device
 *
 * @param dev Pointer to the device
 * @param list_name Name of the phandle list property
 * @param cells_name Name of the cells property
 * @param cell_count Number of cells
 * @param index Index of the phandle
 * @param out_args Pointer to store the phandle arguments
 * @return 0 on success, negative error code on failure
 */
int dev_read_phandle_with_args(struct udevice *dev, const char *list_name,
				const char *cells_name, int cell_count,
				int index,
				struct ofnode_phandle_args *out_args)
{
	return ofnode_parse_phandle_with_args(dev_ofnode(dev), list_name,
					      cells_name, cell_count, index,
					      out_args);
}

/**
 * @brief Count the number of phandles with arguments in a device
 *
 * @param dev Pointer to the device
 * @param list_name Name of the phandle list property
 * @param cells_name Name of the cells property
 * @return Count of phandles, or negative error code on failure
 */
int dev_count_phandle_with_args(struct udevice *dev,
		const char *list_name, const char *cells_name)
{
	return ofnode_count_phandle_with_args(dev_ofnode(dev), list_name,
					      cells_name);
}

/**
 * @brief Read the address cells property from a device
 *
 * @param dev Pointer to the device
 * @return Address cells value, or negative error code on failure
 */
int dev_read_addr_cells(struct udevice *dev)
{
	return ofnode_read_addr_cells(dev_ofnode(dev));
}

/**
 * @brief Read the size cells property from a device
 *
 * @param dev Pointer to the device
 * @return Size cells value, or negative error code on failure
 */
int dev_read_size_cells(struct udevice *dev)
{
	return ofnode_read_size_cells(dev_ofnode(dev));
}

/**
 * @brief Read the simple address cells property from a device
 *
 * @param dev Pointer to the device
 * @return Simple address cells value, or negative error code on failure
 */
int dev_read_simple_addr_cells(struct udevice *dev)
{
	return ofnode_read_simple_addr_cells(dev_ofnode(dev));
}

/**
 * @brief Read the simple size cells property from a device
 *
 * @param dev Pointer to the device
 * @return Simple size cells value, or negative error code on failure
 */
int dev_read_simple_size_cells(struct udevice *dev)
{
	return ofnode_read_simple_size_cells(dev_ofnode(dev));
}

/**
 * @brief Read the phandle property from a device
 *
 * @param dev Pointer to the device
 * @return Phandle value, or negative error code on failure
 */
int dev_read_phandle(struct udevice *dev)
{
	ofnode node = dev_ofnode(dev);

	if (ofnode_is_np(node))
		return ofnode_to_np(node)->phandle;
	else
		return fdt_get_phandle(gd->fdt_blob, ofnode_to_offset(node));
}

/**
 * @brief Read a property from a device
 *
 * @param dev Pointer to the device
 * @param propname Name of the property
 * @param lenp Pointer to store the length of the property
 * @return Pointer to the property, or NULL if not found
 */
const void *dev_read_prop(struct udevice *dev, const char *propname, int *lenp)
{
	return ofnode_get_property(dev_ofnode(dev), propname, lenp);
}

/**
 * @brief Read the first property from a device
 *
 * @param dev Pointer to the device
 * @param prop Pointer to store the property
 * @return 0 on success, negative error code on failure
 */
int dev_read_first_prop(struct udevice *dev, struct ofprop *prop)
{
	return ofnode_get_first_property(dev_ofnode(dev), prop);
}

/**
 * @brief Read the next property from a device
 *
 * @param prop Pointer to the current property
 * @return 0 on success, negative error code on failure
 */
int dev_read_next_prop(struct ofprop *prop)
{
	return ofnode_get_next_property(prop);
}

/**
 * @brief Read a property by property structure
 *
 * @param prop Pointer to the property structure
 * @param propname Pointer to store the property name
 * @param lenp Pointer to store the length of the property
 * @return Pointer to the property, or NULL if not found
 */
const void *dev_read_prop_by_prop(struct ofprop *prop,
				  const char **propname, int *lenp)
{
	return ofnode_get_property_by_prop(prop, propname, lenp);
}

/**
 * @brief Read the alias sequence number from a device
 *
 * @param dev Pointer to the device
 * @param devnump Pointer to store the alias sequence number
 * @return 0 on success, negative error code on failure
 */
int dev_read_alias_seq(struct udevice *dev, int *devnump)
{
	ofnode node = dev_ofnode(dev);
	const char *uc_name = dev->uclass->uc_drv->name;
	int ret;

	if (ofnode_is_np(node)) {
		ret = of_alias_get_id(ofnode_to_np(node), uc_name);
		if (ret >= 0)
			*devnump = ret;
	} else {
		ret = fdtdec_get_alias_seq(gd->fdt_blob, uc_name,
					   ofnode_to_offset(node), devnump);
	}

	return ret;
}

/**
 * @brief Read a 32-bit integer array property from a device
 *
 * @param dev Pointer to the device
 * @param propname Name of the property
 * @param out_values Pointer to store the values
 * @param sz Size of the array
 * @return 0 on success, negative error code on failure
 */
int dev_read_u32_array(struct udevice *dev, const char *propname,
		       u32 *out_values, size_t sz)
{
	if (!dev_of_valid(dev))
		return -EINVAL;
	return ofnode_read_u32_array(dev_ofnode(dev), propname, out_values, sz);
}

/**
 * @brief Write a 32-bit integer array property to a device
 *
 * @param dev Pointer to the device
 * @param propname Name of the property
 * @param values Pointer to the values
 * @param sz Size of the array
 * @return 0 on success, negative error code on failure
 */
int dev_write_u32_array(struct udevice *dev, const char *propname,
			u32 *values, size_t sz)
{
	if (!dev_of_valid(dev))
		return -EINVAL;
	return ofnode_write_u32_array(dev_ofnode(dev), propname, values, sz);
}

/**
 * @brief Read a pointer to a uint8_t array property from a device
 *
 * @param dev Pointer to the device
 * @param propname Name of the property
 * @param sz Size of the array
 * @return Pointer to the array, or NULL if not found
 */
const uint8_t *dev_read_u8_array_ptr(struct udevice *dev, const char *propname,
				     size_t sz)
{
	return ofnode_read_u8_array_ptr(dev_ofnode(dev), propname, sz);
}

/**
 * @brief Check if a device is enabled
 *
 * @param dev Pointer to the device
 * @return 1 if enabled, 0 if disabled, negative error code on failure
 */
int dev_read_enabled(struct udevice *dev)
{
	ofnode node = dev_ofnode(dev);

	if (ofnode_is_np(node))
		return of_device_is_available(ofnode_to_np(node));
	else
		return fdtdec_get_is_enabled(gd->fdt_blob,
					     ofnode_to_offset(node));
}

/**
 * @brief Read a resource from a device by index
 *
 * @param dev Pointer to the device
 * @param index Index of the resource
 * @param res Pointer to store the resource
 * @return 0 on success, negative error code on failure
 */
int dev_read_resource(struct udevice *dev, uint index, struct resource *res)
{
	return ofnode_read_resource(dev_ofnode(dev), index, res);
}

/**
 * @brief Read a resource from a device by name
 *
 * @param dev Pointer to the device
 * @param name Name of the resource
 * @param res Pointer to store the resource
 * @return 0 on success, negative error code on failure
 */
int dev_read_resource_byname(struct udevice *dev, const char *name,
			     struct resource *res)
{
	return ofnode_read_resource_byname(dev_ofnode(dev), name, res);
}

/**
 * @brief Translate an address for a device
 *
 * @param dev Pointer to the device
 * @param in_addr Pointer to the input address
 * @return Translated address
 */
u64 dev_translate_address(struct udevice *dev, const fdt32_t *in_addr)
{
	return ofnode_translate_address(dev_ofnode(dev), in_addr);
}
//GST