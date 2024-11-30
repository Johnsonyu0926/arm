//zephyr-3.7-branch/lib/libc/minimal/source/stdlib/qsort.c
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <zephyr/sys/util.h>

/*
 * Normally parent is defined as parent(k) = floor((k-1) / 2) but we can avoid a
 * divide by noticing that floor((k-1) / 2) = ((k - 1) >> 1).
 */
#define parent(k) (((k) - 1) >> 1)

/*
 * Normally left is defined as left(k) = (2 * k + 1) but we can avoid a
 * multiply by noticing that (2 * k + 1) = ((k << 1) + 1).
 */
#define left(k) (((k) << 1) + 1)

/*
 * Normally right is defined as right(k) = (2 * k + 2) but we can avoid a
 * multiply by noticing that right(k) = left(k) + 1
 */
#define right(k) (left(k) + 1)

#define A(k) ((uint8_t *)base + size * (k))

struct qsort_comp {
	bool has3;
	void *arg;
	union {
		int (*comp2)(const void *a, const void *b);
		int (*comp3)(const void *a, const void *b, void *arg);
	};
};

/**
 * @brief Compare two elements using the comparison function
 *
 * @param cmp Pointer to the comparison structure
 * @param a Pointer to the first element
 * @param b Pointer to the second element
 * @return Result of the comparison
 */
static inline int compare(struct qsort_comp *cmp, void *a, void *b)
{
	if (cmp->has3) {
		return cmp->comp3(a, b, cmp->arg);
	}

	return cmp->comp2(a, b);
}

/**
 * @brief Sift down the heap
 *
 * @param base Pointer to the base of the array
 * @param start Start index
 * @param end End index
 * @param size Size of each element
 * @param cmp Pointer to the comparison structure
 */
static void sift_down(void *base, int start, int end, size_t size, struct qsort_comp *cmp)
{
	int root;
	int child;
	int swap;

	for (swap = start, root = swap; left(root) < end; root = swap) {
		child = left(root);

		/* if root < left */
		if (compare(cmp, A(swap), A(child)) < 0) {
			swap = child;
		}

		/* right exists and min(A(root),A(left)) < A(right) */
		if (right(root) < end && compare(cmp, A(swap), A(right(root))) < 0) {
			swap = right(root);
		}

		if (swap == root) {
			return;
		}

		byteswp(A(root), A(swap), size);
	}
}

/**
 * @brief Heapify the array
 *
 * @param base Pointer to the base of the array
 * @param nmemb Number of elements in the array
 * @param size Size of each element
 * @param cmp Pointer to the comparison structure
 */
static void heapify(void *base, int nmemb, size_t size, struct qsort_comp *cmp)
{
	int start;

	for (start = parent(nmemb - 1); start >= 0; --start) {
		sift_down(base, start, nmemb, size, cmp);
	}
}

/**
 * @brief Perform heap sort on the array
 *
 * @param base Pointer to the base of the array
 * @param nmemb Number of elements in the array
 * @param size Size of each element
 * @param cmp Pointer to the comparison structure
 */
static void heap_sort(void *base, int nmemb, size_t size, struct qsort_comp *cmp)
{
	int end;

	heapify(base, nmemb, size, cmp);

	for (end = nmemb - 1; end > 0; --end) {
		byteswp(A(end), A(0), size);
		sift_down(base, 0, end, size, cmp);
	}
}

/**
 * @brief Sort an array with a comparison function and an argument
 *
 * @param base Pointer to the array to sort
 * @param nmemb Number of elements in the array
 * @param size Size of each element
 * @param comp3 Comparison function
 * @param arg Argument to pass to the comparison function
 */
void qsort_r(void *base, size_t nmemb, size_t size,
	     int (*comp3)(const void *a, const void *b, void *arg), void *arg)
{
	struct qsort_comp cmp = {
		.has3 = true,
		.arg = arg,
		{
			.comp3 = comp3
		}
	};

	heap_sort(base, nmemb, size, &cmp);
}

/**
 * @brief Sort an array
 *
 * @param base Pointer to the array to sort
 * @param nmemb Number of elements in the array
 * @param size Size of each element
 * @param comp2 Comparison function
 */
void qsort(void *base, size_t nmemb, size_t size,
	   int (*comp2)(const void *a, const void *b))
{
	struct qsort_comp cmp = {
		.has3 = false,
		.arg = NULL,
		{
			.comp2 = comp2
		}
	};

	heap_sort(base, nmemb, size, &cmp);
}
//GST