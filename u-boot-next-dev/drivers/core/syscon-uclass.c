// syscon-uclass.c

#include <common.h>
#include <syscon.h>
#include <dm.h>
#include <errno.h>
#include <regmap.h>
#include <dm/device-internal.h>
#include <dm/lists.h>
#include <dm/root.h>
#include <linux/err.h>

/**
 * @brief Get the regmap of a syscon device
 *
 * @param dev Pointer to the syscon device
 * @return Pointer to the regmap, or ERR_PTR on error
 */
struct regmap *syscon_get_regmap(struct udevice *dev)
{
	struct syscon_uc_info *priv;

	if (device_get_uclass_id(dev) != UCLASS_SYSCON)
		return ERR_PTR(-ENOEXEC);
	priv = dev_get_uclass_priv(dev);
	return priv->regmap;
}

/**
 * @brief Pre-probe function for syscon devices
 *
 * @param dev Pointer to the syscon device
 * @return 0 on success, negative error code on failure
 */
static int syscon_pre_probe(struct udevice *dev)
{
	struct syscon_uc_info *priv = dev_get_uclass_priv(dev);

#if CONFIG_IS_ENABLED(OF_PLATDATA)
	struct syscon_base_platdata *plat = dev_get_platdata(dev);

	return regmap_init_mem_platdata(dev, plat->reg, ARRAY_SIZE(plat->reg),
					&priv->regmap);
#else
	return regmap_init_mem(dev, &priv->regmap);
#endif
}

/**
 * @brief Lookup a syscon regmap by phandle
 *
 * @param dev Pointer to the device
 * @param name Name of the phandle property
 * @return Pointer to the regmap, or ERR_PTR on error
 */
struct regmap *syscon_regmap_lookup_by_phandle(struct udevice *dev,
					       const char *name)
{
	struct udevice *syscon;
	struct regmap *r;
	int err;

	err = uclass_get_device_by_phandle(UCLASS_SYSCON, dev, name, &syscon);
	if (err)
		return ERR_PTR(err);

	r = syscon_get_regmap(syscon);
	if (!r)
		return ERR_PTR(-ENODEV);

	return r;
}

/**
 * @brief Get a syscon device by driver data
 *
 * @param driver_data Driver data
 * @param devp Pointer to store the device
 * @return 0 on success, negative error code on failure
 */
int syscon_get_by_driver_data(ulong driver_data, struct udevice **devp)
{
	struct udevice *dev;
	struct uclass *uc;
	int ret;

	*devp = NULL;
	ret = uclass_get(UCLASS_SYSCON, &uc);
	if (ret)
		return ret;
	uclass_foreach_dev(dev, uc) {
		if (dev->driver_data == driver_data) {
			*devp = dev;
			return device_probe(dev);
		}
	}

	return -ENODEV;
}

/**
 * @brief Get a syscon regmap by driver data
 *
 * @param driver_data Driver data
 * @return Pointer to the regmap, or ERR_PTR on error
 */
struct regmap *syscon_get_regmap_by_driver_data(ulong driver_data)
{
	struct syscon_uc_info *priv;
	struct udevice *dev;
	int ret;

	ret = syscon_get_by_driver_data(driver_data, &dev);
	if (ret)
		return ERR_PTR(ret);
	priv = dev_get_uclass_priv(dev);

	return priv->regmap;
}

/**
 * @brief Get the first range of a syscon device by driver data
 *
 * @param driver_data Driver data
 * @return Pointer to the first range, or ERR_PTR on error
 */
void *syscon_get_first_range(ulong driver_data)
{
	struct regmap *map;

	map = syscon_get_regmap_by_driver_data(driver_data);
	if (IS_ERR(map))
		return map;
	return regmap_get_range(map, 0);
}

UCLASS_DRIVER(syscon) = {
	.id		= UCLASS_SYSCON,
	.name		= "syscon",
	.per_device_auto_alloc_size = sizeof(struct syscon_uc_info),
	.pre_probe = syscon_pre_probe,
};

static const struct udevice_id generic_syscon_ids[] = {
	{ .compatible = "syscon" },
	{ }
};

U_BOOT_DRIVER(generic_syscon) = {
	.name	= "syscon",
	.id	= UCLASS_SYSCON,
#if !CONFIG_IS_ENABLED(OF_PLATDATA)
	.bind           = dm_scan_fdt_dev,
#endif
	.of_match = generic_syscon_ids,
};

// simple-bus.c

#include <common.h>
#include <dm.h>

struct simple_bus_plat {
	u32 base;
	u32 size;
	u32 target;
};

/**
 * @brief Translate an address for a simple bus device
 *
 * @param dev Pointer to the simple bus device
 * @param addr Address to translate
 * @return Translated address
 */
fdt_addr_t simple_bus_translate(struct udevice *dev, fdt_addr_t addr)
{
	struct simple_bus_plat *plat = dev_get_uclass_platdata(dev);

	if (addr >= plat->base && addr < plat->base + plat->size)
		addr = (addr - plat->base) + plat->target;

	return addr;
}

/**
 * @brief Post-bind function for simple bus devices
 *
 * @param dev Pointer to the simple bus device
 * @return 0 on success, negative error code on failure
 */
static int simple_bus_post_bind(struct udevice *dev)
{
#if CONFIG_IS_ENABLED(OF_PLATDATA)
	return 0;
#else
	u32 cell[3];
	int ret;

	ret = dev_read_u32_array(dev, "ranges", cell, ARRAY_SIZE(cell));
	if (!ret) {
		struct simple_bus_plat *plat = dev_get_uclass_platdata(dev);

		plat->base = cell[0];
		plat->target = cell[1];
		plat->size = cell[2];
	}

	return dm_scan_fdt_dev(dev);
#endif
}

UCLASS_DRIVER(simple_bus) = {
	.id		= UCLASS_SIMPLE_BUS,
	.name		= "simple_bus",
	.post_bind	= simple_bus_post_bind,
	.per_device_platdata_auto_alloc_size = sizeof(struct simple_bus_plat),
};

static const struct udevice_id generic_simple_bus_ids[] = {
	{ .compatible = "simple-bus" },
	{ .compatible = "simple-mfd" },
	{ }
};

U_BOOT_DRIVER(simple_bus_drv) = {
	.name	= "generic_simple_bus",
	.id	= UCLASS_SIMPLE_BUS,
	.of_match = generic_simple_bus_ids,
};
//GST