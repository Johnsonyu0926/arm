//zephyr-3.7-branch/lib/libc/minimal/source/stdlib/bsearch.c
#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Generic implementation of Binary Search
 *
 * @param key Pointer to the item being searched for
 * @param array Pointer to the array to search
 * @param count Number of elements in the array
 * @param size Size of each element in the array
 * @param cmp Pointer to the comparison function
 *
 * @return Pointer to the key if present in the array, or NULL otherwise
 */
void *bsearch(const void *key, const void *array, size_t count, size_t size,
	      int (*cmp)(const void *key, const void *element))
{
	size_t low = 0;
	size_t high = count;
	size_t index;
	int result;
	const char *pivot;

	while (low < high) {
		index = low + (high - low) / 2;
		pivot = (const char *)array + index * size;
		result = cmp(key, pivot);

		if (result == 0) {
			return (void *)pivot;
		} else if (result > 0) {
			low = index + 1;
		} else {
			high = index;
		}
	}
	return NULL;
}
//GST