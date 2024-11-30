//zephyr-3.7-branch/lib/open-amp/resource_table.c

#include <zephyr/kernel.h>
#include <resource_table.h>

extern char ram_console[];

#define __resource Z_GENERIC_SECTION(.resource_table)

/**
 * @brief Resource table structure
 */
static struct fw_resource_table __resource resource_table = {
	.hdr = {
		.ver = 1,
		.num = RSC_TABLE_NUM_ENTRY,
	},
	.offset = {

#if (CONFIG_OPENAMP_RSC_TABLE_NUM_RPMSG_BUFF > 0)
		offsetof(struct fw_resource_table, vdev),
#endif

#if defined(CONFIG_RAM_CONSOLE)
		offsetof(struct fw_resource_table, cm_trace),
#endif
	},

#if (CONFIG_OPENAMP_RSC_TABLE_NUM_RPMSG_BUFF > 0)
	/* Virtio device entry */
	.vdev = {
		RSC_VDEV, VIRTIO_ID_RPMSG, 0, RPMSG_IPU_C0_FEATURES, 0, 0, 0,
		VRING_COUNT, {0, 0},
	},

	/* Vring rsc entry - part of vdev rsc entry */
	.vring0 = {VRING_TX_ADDRESS, VRING_ALIGNMENT,
		   CONFIG_OPENAMP_RSC_TABLE_NUM_RPMSG_BUFF,
		   VRING0_ID, 0},
	.vring1 = {VRING_RX_ADDRESS, VRING_ALIGNMENT,
		   CONFIG_OPENAMP_RSC_TABLE_NUM_RPMSG_BUFF,
		   VRING1_ID, 0},
#endif

#if defined(CONFIG_RAM_CONSOLE)
	.cm_trace = {
		RSC_TRACE,
		(uint32_t)ram_console, CONFIG_RAM_CONSOLE_BUFFER_SIZE, 0,
		"Zephyr_log",
	},
#endif
};

/**
 * @brief Get the resource table
 *
 * @param table_ptr Pointer to store the resource table
 * @param length Pointer to store the length of the resource table
 */
void rsc_table_get(struct fw_resource_table **table_ptr, int *length)
{
	*table_ptr = &resource_table;
	*length = sizeof(resource_table);
}
//GST