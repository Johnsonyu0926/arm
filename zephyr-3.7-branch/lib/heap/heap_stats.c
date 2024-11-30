//lib/heap/heap_stats.c
#include <zephyr/sys/sys_heap.h>
#include <zephyr/sys/util.h>
#include <zephyr/kernel.h>
#include "heap.h"

/**
 * @brief Get runtime statistics of the heap
 *
 * This function retrieves the runtime statistics of the heap, including
 * free bytes, allocated bytes, and maximum allocated bytes.
 *
 * @param heap Pointer to the heap
 * @param stats Pointer to the memory stats structure to store the results
 * @return 0 on success, -EINVAL if the heap or stats pointer is NULL
 */
int sys_heap_runtime_stats_get(struct sys_heap *heap, struct sys_memory_stats *stats)
{
	if ((heap == NULL) || (stats == NULL)) {
		return -EINVAL;
	}

	stats->free_bytes = heap->heap->free_bytes;
	stats->allocated_bytes = heap->heap->allocated_bytes;
	stats->max_allocated_bytes = heap->heap->max_allocated_bytes;

	return 0;
}

/**
 * @brief Reset the maximum allocated bytes statistic
 *
 * This function resets the maximum allocated bytes statistic to the current
 * allocated bytes value.
 *
 * @param heap Pointer to the heap
 * @return 0 on success, -EINVAL if the heap pointer is NULL
 */
int sys_heap_runtime_stats_reset_max(struct sys_heap *heap)
{
	if (heap == NULL) {
		return -EINVAL;
	}

	heap->heap->max_allocated_bytes = heap->heap->allocated_bytes;

	return 0;
}
//GST
