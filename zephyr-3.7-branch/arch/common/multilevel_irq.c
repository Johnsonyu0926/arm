//arch/arc/common/include/multilevel_irq.c
#include <zephyr/device.h>
#include <zephyr/irq.h>
#include <zephyr/sw_isr_table.h>
#include <zephyr/sys/__assert.h>
#include <zephyr/sys/util.h>

BUILD_ASSERT((CONFIG_NUM_2ND_LEVEL_AGGREGATORS * CONFIG_MAX_IRQ_PER_AGGREGATOR) <=
		     BIT(CONFIG_2ND_LEVEL_INTERRUPT_BITS),
	     "L2 bits not enough to cover the number of L2 IRQs");

/**
 * @brief Get the aggregator responsible for the given IRQ
 *
 * This function retrieves the aggregator entry responsible for handling the
 * specified IRQ.
 *
 * @param irq IRQ number to query
 * @return Aggregator entry, NULL if IRQ is level 1 or not found
 */
static const struct _irq_parent_entry *get_intc_entry_for_irq(unsigned int irq)
{
	const unsigned int level = irq_get_level(irq);

	/* 1st level aggregator is not registered */
	if (level == 1) {
		return NULL;
	}

	const unsigned int intc_irq = irq_get_intc_irq(irq);

	/* Find an aggregator entry that matches the level & intc_irq */
	STRUCT_SECTION_FOREACH_ALTERNATE(intc_table, _irq_parent_entry, intc) {
		if ((intc->level == level) && (intc->irq == intc_irq)) {
			return intc;
		}
	}

	return NULL;
}

/**
 * @brief Get the parent interrupt controller device for the given IRQ
 *
 * This function retrieves the parent interrupt controller device responsible
 * for handling the specified IRQ.
 *
 * @param irq IRQ number to query
 * @return Parent interrupt controller device
 */
const struct device *z_get_sw_isr_device_from_irq(unsigned int irq)
{
	const struct _irq_parent_entry *intc = get_intc_entry_for_irq(irq);

	__ASSERT(intc != NULL, "can't find an aggregator to handle irq(%X)", irq);

	return intc != NULL ? intc->dev : NULL;
}

/**
 * @brief Get the IRQ number of the parent interrupt controller device
 *
 * This function retrieves the IRQ number of the specified parent interrupt
 * controller device.
 *
 * @param dev Parent interrupt controller device
 * @return IRQ number of the interrupt controller
 */
unsigned int z_get_sw_isr_irq_from_device(const struct device *dev)
{
	/* Get the IRQN for the aggregator */
	STRUCT_SECTION_FOREACH_ALTERNATE(intc_table, _irq_parent_entry, intc) {
		if (intc->dev == dev) {
			return intc->irq;
		}
	}

	__ASSERT(false, "dev(%p) not found", dev);

	return 0;
}

/**
 * @brief Get the index in the software ISR table for the given IRQ
 *
 * This function computes the index in the software ISR table based on the
 * specified IRQ number.
 *
 * @param irq IRQ number to query
 * @return Index in the software ISR table
 */
unsigned int z_get_sw_isr_table_idx(unsigned int irq)
{
	unsigned int table_idx, local_irq;
	const struct _irq_parent_entry *intc = get_intc_entry_for_irq(irq);
	const unsigned int level = irq_get_level(irq);

	if (intc != NULL) {
		local_irq = irq_from_level(irq, level);
		__ASSERT_NO_MSG(local_irq < CONFIG_MAX_IRQ_PER_AGGREGATOR);

		table_idx = intc->offset + local_irq;
	} else {
		/* IRQ level must be 1 if no intc entry */
		__ASSERT(level == 1, "can't find an aggregator to handle irq(%X)", irq);
		table_idx = irq;
	}

	table_idx -= CONFIG_GEN_IRQ_START_VECTOR;

	__ASSERT_NO_MSG(table_idx < IRQ_TABLE_SIZE);

	return table_idx;
}

//GST