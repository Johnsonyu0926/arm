//arch/arc/common/include/shared_irq.c
#include "sw_isr_common.h"
#include <zephyr/sw_isr_table.h>
#include <zephyr/spinlock.h>

/* An interrupt line can be considered shared only if there's
 * at least 2 clients using it. As such, enforce the fact that
 * the maximum number of allowed clients should be at least 2.
 */
BUILD_ASSERT(CONFIG_SHARED_IRQ_MAX_NUM_CLIENTS >= 2,
	     "maximum number of clients should be at least 2");

/**
 * @brief Shared ISR handler
 *
 * This function handles shared interrupts by iterating through the list of
 * registered clients and invoking their ISR routines.
 *
 * @param data Pointer to the shared ISR table entry
 */
void z_shared_isr(const void *data)
{
	size_t i;
	const struct z_shared_isr_table_entry *entry;
	const struct _isr_table_entry *client;

	entry = data;

	for (i = 0; i < entry->client_num; i++) {
		client = &entry->clients[i];

		if (client->isr) {
			client->isr(client->arg);
		}
	}
}

#ifdef CONFIG_DYNAMIC_INTERRUPTS

static struct k_spinlock lock;

/**
 * @brief Install an ISR for the specified IRQ
 *
 * This function installs an ISR for the specified IRQ. If the IRQ line is
 * already in use, it registers the ISR as a shared ISR.
 *
 * @param irq IRQ number
 * @param routine ISR routine to install
 * @param param Parameter to pass to the ISR routine
 */
void z_isr_install(unsigned int irq, void (*routine)(const void *),
		   const void *param)
{
	struct z_shared_isr_table_entry *shared_entry;
	struct _isr_table_entry *entry;
	struct _isr_table_entry *client;
	unsigned int table_idx;
	int i;
	k_spinlock_key_t key;

	table_idx = z_get_sw_isr_table_idx(irq);

	/* Check for out of bounds table index */
	if (table_idx >= IRQ_TABLE_SIZE) {
		return;
	}

	shared_entry = &z_shared_sw_isr_table[table_idx];
	entry = &_sw_isr_table[table_idx];

	key = k_spin_lock(&lock);

	/* Have we reached the client limit? */
	__ASSERT(shared_entry->client_num < CONFIG_SHARED_IRQ_MAX_NUM_CLIENTS,
		 "reached maximum number of clients");

	if (entry->isr == z_irq_spurious) {
		/* This is the first time an ISR/arg pair is registered
		 * for INTID => no need to share it.
		 */
		entry->isr = routine;
		entry->arg = param;

		k_spin_unlock(&lock, key);

		return;
	} else if (entry->isr != z_shared_isr) {
		/* INTID is being used by another ISR/arg pair.
		 * Push back the ISR/arg pair registered in _sw_isr_table
		 * to the list of clients and hijack the pair stored in
		 * _sw_isr_table with our own z_shared_isr/shared_entry pair.
		 */
		shared_entry->clients[shared_entry->client_num].isr = entry->isr;
		shared_entry->clients[shared_entry->client_num].arg = entry->arg;

		shared_entry->client_num++;

		entry->isr = z_shared_isr;
		entry->arg = shared_entry;
	}

	/* Don't register the same ISR/arg pair multiple times */
	for (i = 0; i < shared_entry->client_num; i++) {
		client = &shared_entry->clients[i];

		__ASSERT(client->isr != routine && client->arg != param,
			 "trying to register duplicate ISR/arg pair");
	}

	shared_entry->clients[shared_entry->client_num].isr = routine;
	shared_entry->clients[shared_entry->client_num].arg = param;
	shared_entry->client_num++;

	k_spin_unlock(&lock, key);
}

/**
 * @brief Swap client data between two ISR table entries
 *
 * This function swaps the ISR and argument data between two ISR table entries.
 *
 * @param a Pointer to the first ISR table entry
 * @param b Pointer to the second ISR table entry
 */
static void swap_client_data(struct _isr_table_entry *a,
			     struct _isr_table_entry *b)
{
	struct _isr_table_entry tmp;

	tmp.arg = a->arg;
	tmp.isr = a->isr;

	a->arg = b->arg;
	a->isr = b->isr;

	b->arg = tmp.arg;
	b->isr = tmp.isr;
}

/**
 * @brief Remove a client from the shared ISR table
 *
 * This function removes a client from the shared ISR table and updates the
 * table accordingly.
 *
 * @param shared_entry Pointer to the shared ISR table entry
 * @param client_idx Index of the client to remove
 * @param table_idx Index in the ISR table
 */
static void shared_irq_remove_client(struct z_shared_isr_table_entry *shared_entry,
				     int client_idx, unsigned int table_idx)
{
	int i;

	shared_entry->clients[client_idx].isr = NULL;
	shared_entry->clients[client_idx].arg = NULL;

	/* Push back the removed client to the end of the client list */
	for (i = client_idx; i <= (int)shared_entry->client_num - 2; i++) {
		swap_client_data(&shared_entry->clients[i],
				 &shared_entry->clients[i + 1]);
	}

	shared_entry->client_num--;

	/* "Unshare" interrupt if there will be a single client left */
	if (shared_entry->client_num == 1) {
		_sw_isr_table[table_idx].isr = shared_entry->clients[0].isr;
		_sw_isr_table[table_idx].arg = shared_entry->clients[0].arg;

		shared_entry->clients[0].isr = NULL;
		shared_entry->clients[0].arg = NULL;

		shared_entry->client_num--;
	}
}

/**
 * @brief Disconnect a dynamic ISR for the specified IRQ
 *
 * This function disconnects a dynamic ISR for the specified IRQ.
 *
 * @param irq IRQ number
 * @param priority IRQ priority
 * @param routine ISR routine to disconnect
 * @param parameter Parameter to pass to the ISR routine
 * @param flags IRQ flags
 * @return 0 on success, or an error code
 */
int __weak arch_irq_disconnect_dynamic(unsigned int irq, unsigned int priority,
				       void (*routine)(const void *parameter),
				       const void *parameter, uint32_t flags)
{
	ARG_UNUSED(priority);
	ARG_UNUSED(flags);

	return z_isr_uninstall(irq, routine, parameter);
}

/**
 * @brief Uninstall an ISR for the specified IRQ
 *
 * This function uninstalls an ISR for the specified IRQ. If the IRQ line is
 * shared, it removes the ISR from the shared ISR table.
 *
 * @param irq IRQ number
 * @param routine ISR routine to uninstall
 * @param parameter Parameter to pass to the ISR routine
 * @return 0 on success, or an error code
 */
int z_isr_uninstall(unsigned int irq,
		    void (*routine)(const void *),
		    const void *parameter)
{
	struct z_shared_isr_table_entry *shared_entry;
	struct _isr_table_entry *entry;
	struct _isr_table_entry *client;
	unsigned int table_idx;
	size_t i;
	k_spinlock_key_t key;

	table_idx = z_get_sw_isr_table_idx(irq);

	/* Check for out of bounds table index */
	if (table_idx >= IRQ_TABLE_SIZE) {
		return -EINVAL;
	}

	shared_entry = &z_shared_sw_isr_table[table_idx];
	entry = &_sw_isr_table[table_idx];

	key = k_spin_lock(&lock);

	/* Note: it's important that we remove the ISR/arg pair even if
	 * the IRQ line is not being shared because z_isr_install() will
	 * not overwrite it unless the _sw_isr_table entry for the given
	 * IRQ line contains the default pair which is z_irq_spurious/NULL.
	 */
	if (!shared_entry->client_num) {
		if (entry->isr == routine && entry->arg == parameter) {
			entry->isr = z_irq_spurious;
			entry->arg = NULL;
		}

		goto out_unlock;
	}

	for (i = 0; i < shared_entry->client_num; i++) {
		client = &shared_entry->clients[i];

		if (client->isr == routine && client->arg == parameter) {
			/* Note: this is the only match we're going to get */
			shared_irq_remove_client(shared_entry, i, table_idx);
			goto out_unlock;
		}
	}

out_unlock:
	k_spin_unlock(&lock, key);
	return 0;
}

#endif /* CONFIG_DYNAMIC_INTERRUPTS */
GST