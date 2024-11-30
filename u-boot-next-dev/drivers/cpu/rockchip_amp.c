// rockchip_amp.c
#include <common.h>
#include <amp.h>
#include <bidram.h>
#include <boot_rkimg.h>
#include <config.h>
#include <sysmem.h>
#include <asm/gic.h>
#include <asm/io.h>
#include <asm/arch/rockchip_smccc.h>
DECLARE_GLOBAL_DATA_PTR;
/*
 * [Design Principles]
 *
 * [amp.img]
 *	The amp image with FIT format which consists of non-linux firmwares.
 *	Please refer to: driver/cpu/amp.its.
 *
 *	amp.img generation: ./tools/mkimage -f drivers/cpu/amp.its -E -p 0xe00 amp.img
 *
 * [linux]
 *	We still use the traditional solution for a better compatibility:
 *	boot.img/recovery.img with FIT format or Android format.
 *
 *	The developer need add "/configurations/conf/linux" node to configure:
 *	description, arch, cpu, thumb, hyp, udelay(optional) properties.
 *	The addresses depend on U-Boot: kernel_addr_r, fdt_addr_r and
 *	ramdisk_addr_r. Please refer to: driver/cpu/amp.its.
 *
 * [memory management]
 *	U-Boot is not responsible for memory distribution/fixup any more, please
 *	handle this on kernel dts "/memory".
 *
 * [trust]
 *	The AMP feature requires trust support.
 */
#define AMP_PART		"amp"
#define FIT_HEADER_SIZE		SZ_4K
#define gicd_readl(offset)	readl((void *)GICD_BASE + (offset))
#define gicd_writel(v, offset)	writel(v, (void *)g_bootcpu.entry;
		armv7_entry();
#endif
	}
	/* return: boot cpu continue to boot linux */
	return 0;
}
/**
 * @brief Power on AMP CPUs
 *
 * @return 0 on success, otherwise error
 */
int amp_cpus_on(void)
{
	struct blk_desc *dev_desc;
	bootm_headers_t images;
	disk_partition_t part;
	void *hdr, *fit;
	int offset, cnt;
	int totalsize;
	int ret = 0;
	dev_desc = rockchip_get_bootdev();
	if (!dev_desc)
		return -EIO;
	if (part_get_info_by_name(dev_desc, AMP_PART, &part) < 0)
		return -ENODEV;
	hdr = memalign(ARCH_DMA_MINALIGN, FIT_HEADER_SIZE);
	if (!hdr)
		return -ENOMEM;
	/* get totalsize */
	offset = part.start;
	cnt = DIV_ROUND_UP(FIT_HEADER_SIZE, part.blksz);
	if (blk_dread(dev_desc, offset, cnt, hdr) != cnt) {
		ret = -EIO;
		goto out2;
	}
	if (fdt_check_header(hdr)) {
		AMP_E("Not fit\n");
		ret = of_get_property(device, "status", &statlen);
	if (status == NULL)
		return true;
	if (statlen > 0) {
		if (!strcmp(status, "okay"))
			return true;
	}
	return false;
}
/**
 * @brief Get the parent of a device node
 *
 * @param node Device node
 * @return Pointer to the parent node, or NULL if no parent
 */
struct device_node *of_get_parent(const struct device_node *node)
{
	const struct device_node *np;
	if (!node)
		return NULL;
	np = of_node_get(node->parent);
	return (struct device_node *)np;
}
/**
 * @brief Get the next child of a device node
 *
 * @param node Device node
 * @param prev Previous child node
 * @return Pointer to the next child node, or NULL if no more children
 */
static struct device_node *__of_get_next_child(const struct device_node *node,
					       struct device_node *prev)
{
	struct device_node *next;
	if (!node)
		return NULL;
	next = prev ? prev->sibling : node->child;
	for (; next; next = next->sibling)
		if (of_node_get(next))
			break;
	of_node_put(prev);
	return next;
}
#define __for_each_child_of_node(parent, child) \
	for (child = __of_get_next_child(parent, NULL); child != NULL; \
	     child = __of_get_next_child(parent, child))
/**
 * @brief Find a node by its path
 *
 * @param parent Parent node
 * @param path Path to the node
 * @return Pointer to the found node, or NULL if not found
 */
static struct device_node *__of_find_node_by_path(struct device_node *parent,
						  const char *path)
{
	struct device_node *child;
	int len;
 */
int of_alias_get_id(const struct device_node *np, const char *stem)
{
	struct alias_prop *app;
	int id = -ENODEV;
	mutex_lock(&of_mutex);
	list_for_each_entry(app, &aliases_lookup, link) {
		if (strcmp(app->stem, stem) != 0)
			continue;
		if (np == app->np) {
			id = app->id;
			break;
		}
	}
	mutex_unlock(&of_mutex);
	return id;
}
/**
 * @brief Get a device node by alias
 *
 * @param stem Alias string without the index
 * @param id Alias ID
 * @return Pointer to the device node, or NULL if not found
 */
struct device_node *of_alias_get_dev(const char *stem, int id)
{
	struct alias_prop *app;
	struct device_node *np = NULL;
	mutex_lock(&of_mutex);
	list_for_each_entry(app, &aliases_lookup, link) {
		if (strcmp(app->stem, stem) != 0)
			continue;
		if (id == app->id) {
			np = app->np;
			break;
		}
	}
	mutex_unlock(&of_mutex);
	return np;
}
/**
 * @brief Dump all aliases
 *
 * @return Pointer to the last device node, or NULL if no aliases
 */
struct device_node *of_alias_dump(void)
{
	struct alias_prop *app;
	struct device_node *np = NULL;
	mutex_lock(&of_mutex);
	list_for_each_entry(app, &aliases_lookup, link) {
		printf("%10s%d: %20s, phandle=%d %4s\n",
		       app->stem, app->id,
		       app->np->full_name, app->np->phandle,
		       of_get_property(app->np, "u-boot,dm-pre-reloc", NULL;
	}
	return np;
}
/**
 * @brief Dump all aliases
 *
 * @return Pointer to the last device node, or NULL if no aliases
 */
struct device_node *of_alias_dump(void)
{
	struct alias_prop *app;
	struct device_node *np = NULL;
	mutex_lock(&of_mutex);
	list_for_each_entry(app, &aliases_lookup, link) {
		printf("%10s%d: %20s, phandle=%d %4s\n",
		       app->stem, app->id,
		       app->np->full_name, app->np->phandle,
		       of_get_property(app->np, "u-boot,dm-pre-reloc", NULL) ||
		       of_get_property(app->np, "u-boot,dm-spl", NULL) ? "*" : "");
	}
	mutex_unlock(&of_mutex);
	return np;
}
/**
 * @brief Get the stdout device node
 *
 * @return Pointer to the stdout device node, or NULL if not found
 */
struct device_node *of_get_stdout(void)
{
	return of_stdout;
}
//GST