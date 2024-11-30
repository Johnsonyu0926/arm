// kernel/spinlock_validate.c

#include <kernel_internal.h>
#include <zephyr/spinlock.h>

/**
 * @brief Check if the spinlock is valid for locking
 *
 * @param l Pointer to the spinlock
 * @return true if the spinlock is valid for locking, false otherwise
 */
bool z_spin_lock_valid(struct k_spinlock *l)
{
	uintptr_t thread_cpu = l->thread_cpu;

	if (thread_cpu != 0U) {
		if ((thread_cpu & 3U) == _current_cpu->id) {
			return false;
		}
	}
	return true;
}

/**
 * @brief Check if the spinlock is valid for unlocking
 *
 * @param l Pointer to the spinlock
 * @return true if the spinlock is valid for unlocking, false otherwise
 */
bool z_spin_unlock_valid(struct k_spinlock *l)
{
	uintptr_t tcpu = l->thread_cpu;

	l->thread_cpu = 0;

	if (arch_is_in_isr() && _current->base.thread_state & _THREAD_DUMMY) {
		/* Edge case where an ISR aborted _current */
		return true;
	}
	if (tcpu != (_current_cpu->id | (uintptr_t)_current)) {
		return false;
	}
	return true;
}

/**
 * @brief Set the owner of the spinlock
 *
 * @param l Pointer to the spinlock
 */
void z_spin_lock_set_owner(struct k_spinlock *l)
{
	l->thread_cpu = _current_cpu->id | (uintptr_t)_current;
}

#ifdef CONFIG_KERNEL_COHERENCE
/**
 * @brief Check if the spinlock memory is coherent
 *
 * @param l Pointer to the spinlock
 * @return true if the spinlock memory is coherent, false otherwise
 */
bool z_spin_lock_mem_coherent(struct k_spinlock *l)
{
	return arch_mem_coherent((void *)l);
}
#endif /* CONFIG_KERNEL_COHERENCE */
//GST