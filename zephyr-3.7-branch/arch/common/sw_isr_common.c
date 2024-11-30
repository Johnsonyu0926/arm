//arch/arc/common/include/sw_isr_common.c

#include "sw_isr_common.h"
#include <zephyr/sw_isr_table.h>
#include <zephyr/irq.h>
#include <zephyr/sys/__assert.h>

/**
 * @file
 * @brief Common code for architectures that use software ISR tables (CONFIG_GEN_ISR_TABLES)
 */

/**
 * @brief Get the index in the software ISR table for the given IRQ
 *
 * This function computes the index in the software ISR table based on the
 * specified IRQ number.
 *
 * @param irq IRQ number to query
 * @return Index in the software ISR table
 */
unsigned int __weak z_get_sw_isr_table_idx(unsigned int irq)
{
	unsigned int table_idx = irq - CONFIG_GEN_IRQ_START_VECTOR;

	__ASSERT_NO_MSG(table_idx < IRQ_TABLE_SIZE);

	return table_idx;
}
//GST