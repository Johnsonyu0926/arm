// of_extra.c

#include <common.h>
#include <linux/libfdt.h>
#include <dm/of_access.h>
#include <dm/of_extra.h>
#include <dm/ofnode.h>

/**
 * @brief Read an fmap entry from a device tree node
 *
 * @param node ofnode representing the device tree node
 * @param name Name of the node
 * @param entry Pointer to the fmap_entry structure to store the entry data
 * @return 0 on success, negative error code on failure
 */
int of_read_fmap_entry(ofnode node, const char *name,
		       struct fmap_entry *entry)
{
	const char *prop;
	u32 reg[2];

	if (ofnode_read_u32_array(node, "reg", reg, 2)) {
		debug("Node '%s' has bad/missing 'reg' property\n", name);
		return -FDT_ERR_NOTFOUND;
	}
	entry->offset = reg[0];
	entry->length = reg[1];
	entry->used = ofnode_read_s32_default(node, "used", entry->length);
	prop = ofnode_read_string(node, "compress");
	entry->compress_algo = prop && !strcmp(prop, "lzo") ?
		FMAP_COMPRESS_LZO : FMAP_COMPRESS_NONE;
	prop = ofnode_read_string(node, "hash");
	if (prop)
		entry->hash_size = strlen(prop);
	entry->hash_algo = prop ? FMAP_HASH_SHA256 : FMAP_HASH_NONE;
	entry->hash = (uint8_t *)prop;

	return 0;
}
//GST