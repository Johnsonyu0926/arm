//lib/heap/multi_heap.c
#include <zephyr/sys/__assert.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/sys_heap.h>
#include <zephyr/sys/multi_heap.h>

/**
 * @brief Initialize a multi-heap structure
 *
 * @param heap Pointer to the multi-heap structure
 * @param choice_fn Function pointer to choose the appropriate heap
 */
void sys_multi_heap_init(struct sys_multi_heap *heap, sys_multi_heap_fn_t choice_fn)
{
	heap->nheaps = 0;
	heap->choice = choice_fn;
}

/**
 * @brief Add a heap to the multi-heap structure
 *
 * @param mheap Pointer to the multi-heap structure
 * @param heap Pointer to the heap to be added
 * @param user_data User data associated with the heap
 */
void sys_multi_heap_add_heap(struct sys_multi_heap *mheap,
			struct sys_heap *heap, void *user_data)
{
	__ASSERT_NO_MSG(mheap->nheaps < ARRAY_SIZE(mheap->heaps));

	mheap->heaps[mheap->nheaps].heap = heap;
	mheap->heaps[mheap->nheaps++].user_data = user_data;

	/* Now sort them in memory order, simple extraction sort */
	for (int i = 0; i < mheap->nheaps; i++) {
		struct sys_multi_heap_rec swap;
		int lowest = -1;
		uintptr_t lowest_addr = UINTPTR_MAX;

		for (int j = i; j < mheap->nheaps; j++) {
			uintptr_t haddr = (uintptr_t)mheap->heaps[j].heap->heap;

			if (haddr < lowest_addr) {
				lowest = j;
				lowest_addr = haddr;
			}
		}
		swap = mheap->heaps[i];
		mheap->heaps[i] = mheap->heaps[lowest];
		mheap->heaps[lowest] = swap;
	}
}

/**
 * @brief Allocate memory from the multi-heap structure
 *
 * @param mheap Pointer to the multi-heap structure
 * @param cfg Configuration data for the allocation
 * @param bytes Number of bytes to allocate
 * @return Pointer to the allocated memory, or NULL if allocation failed
 */
void *sys_multi_heap_alloc(struct sys_multi_heap *mheap, void *cfg, size_t bytes)
{
	return mheap->choice(mheap, cfg, 0, bytes);
}

/**
 * @brief Allocate aligned memory from the multi-heap structure
 *
 * @param mheap Pointer to the multi-heap structure
 * @param cfg Configuration data for the allocation
 * @param align Alignment requirement
 * @param bytes Number of bytes to allocate
 * @return Pointer to the allocated memory, or NULL if allocation failed
 */
void *sys_multi_heap_aligned_alloc(struct sys_multi_heap *mheap,
				   void *cfg, size_t align, size_t bytes)
{
	return mheap->choice(mheap, cfg, align, bytes);
}

/**
 * @brief Get the heap containing the specified address
 *
 * @param mheap Pointer to the multi-heap structure
 * @param addr Address to find the heap for
 * @return Pointer to the heap record, or NULL if not found
 */
const struct sys_multi_heap_rec *sys_multi_heap_get_heap(const struct sys_multi_heap *mheap,
							 void *addr)
{
	uintptr_t haddr, baddr = (uintptr_t) addr;
	int i;

	/* Search the heaps array to find the correct heap
	 *
	 * FIXME: just a linear search currently, as the list is
	 * always short for reasonable apps and this code is very
	 * quick.  The array is stored in sorted order though, so a
	 * binary search based on the block address is the design
	 * goal.
	 */
	for (i = 0; i < mheap->nheaps; i++) {
		haddr = (uintptr_t)mheap->heaps[i].heap->heap;
		if (baddr < haddr) {
			break;
		}
	}

	/* Now i stores the index of the heap after our target (even
	 * if it's invalid and our target is the last!)
	 * FIXME: return -ENOENT when a proper heap is not found
	 */
	return &mheap->heaps[i-1];
}

/**
 * @brief Free memory allocated from the multi-heap structure
 *
 * @param mheap Pointer to the multi-heap structure
 * @param block Pointer to the memory block to free
 */
void sys_multi_heap_free(struct sys_multi_heap *mheap, void *block)
{
	const struct sys_multi_heap_rec *heap;

	heap = sys_multi_heap_get_heap(mheap, block);

	if (heap != NULL) {
		sys_heap_free(heap->heap, block);
	}
}
//GST