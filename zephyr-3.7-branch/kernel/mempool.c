// kernel/mempool.c

#include <zephyr/kernel.h>
#include <string.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/util.h>

/**
 * @brief Allocate aligned memory from a heap
 *
 * @param heap Pointer to the heap
 * @param align Alignment requirement
 * @param size Size of the memory to allocate
 * @return Pointer to the allocated memory, or NULL on failure
 */
static void *z_heap_aligned_alloc(struct k_heap *heap, size_t align, size_t size)
{
	void *mem;
	struct k_heap **heap_ref;
	size_t __align;

	/*
	 * Adjust the size to make room for our heap reference.
	 * Merge a rewind bit with align value (see sys_heap_aligned_alloc()).
	 * This allows for storing the heap pointer right below the aligned
	 * boundary without wasting any memory.
	 */
	if (size_add_overflow(size, sizeof(heap_ref), &size)) {
		return NULL;
	}
	__align = align | sizeof(heap_ref);

	mem = k_heap_aligned_alloc(heap, __align, size, K_NO_WAIT);
	if (mem == NULL) {
		return NULL;
	}

	heap_ref = mem;
	*heap_ref = heap;
	mem = ++heap_ref;
	__ASSERT(align == 0 || ((uintptr_t)mem & (align - 1)) == 0,
		 "misaligned memory at %p (align = %zu)", mem, align);

	return mem;
}

/**
 * @brief Free allocated memory
 *
 * @param ptr Pointer to the memory to free
 */
void k_free(void *ptr)
{
	struct k_heap **heap_ref;

	if (ptr != NULL) {
		heap_ref = ptr;
		--heap_ref;
		ptr = heap_ref;

		SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_heap_sys, k_free, *heap_ref, heap_ref);

		k_heap_free(*heap_ref, ptr);

		SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_heap_sys, k_free, *heap_ref, heap_ref);
	}
}

#if (K_HEAP_MEM_POOL_SIZE > 0)

K_HEAP_DEFINE(_system_heap, K_HEAP_MEM_POOL_SIZE);
#define _SYSTEM_HEAP (&_system_heap)

/**
 * @brief Allocate aligned memory
 *
 * @param align Alignment requirement
 * @param size Size of the memory to allocate
 * @return Pointer to the allocated memory, or NULL on failure
 */
void *k_aligned_alloc(size_t align, size_t size)
{
	__ASSERT(align / sizeof(void *) >= 1
		&& (align % sizeof(void *)) == 0,
		"align must be a multiple of sizeof(void *)");

	__ASSERT((align & (align - 1)) == 0,
		"align must be a power of 2");

	SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_heap_sys, k_aligned_alloc, _SYSTEM_HEAP);

	void *ret = z_heap_aligned_alloc(_SYSTEM_HEAP, align, size);

	SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_heap_sys, k_aligned_alloc, _SYSTEM_HEAP, ret);

	return ret;
}

/**
 * @brief Allocate memory
 *
 * @param size Size of the memory to allocate
 * @return Pointer to the allocated memory, or NULL on failure
 */
void *k_malloc(size_t size)
{
	SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_heap_sys, k_malloc, _SYSTEM_HEAP);

	void *ret = k_aligned_alloc(sizeof(void *), size);

	SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_heap_sys, k_malloc, _SYSTEM_HEAP, ret);

	return ret;
}

/**
 * @brief Allocate and zero-initialize memory
 *
 * @param nmemb Number of elements
 * @param size Size of each element
 * @return Pointer to the allocated memory, or NULL on failure
 */
void *k_calloc(size_t nmemb, size_t size)
{
	void *ret;
	size_t bounds;

	SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_heap_sys, k_calloc, _SYSTEM_HEAP);

	if (size_mul_overflow(nmemb, size, &bounds)) {
		SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_heap_sys, k_calloc, _SYSTEM_HEAP, NULL);

		return NULL;
	}

	ret = k_malloc(bounds);
	if (ret != NULL) {
		(void)memset(ret, 0, bounds);
	}

	SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_heap_sys, k_calloc, _SYSTEM_HEAP, ret);

	return ret;
}

/**
 * @brief Reallocate memory
 *
 * @param ptr Pointer to the previously allocated memory
 * @param size New size of the memory
 * @return Pointer to the reallocated memory, or NULL on failure
 */
void *k_realloc(void *ptr, size_t size)
{
	struct k_heap *heap, **heap_ref;
	void *ret;

	if (size == 0) {
		k_free(ptr);
		return NULL;
	}
	if (ptr == NULL) {
		return k_malloc(size);
	}
	heap_ref = ptr;
	ptr = --heap_ref;
	heap = *heap_ref;

	SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_heap_sys, k_realloc, heap, ptr);

	if (size_add_overflow(size, sizeof(heap_ref), &size)) {
		SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_heap_sys, k_realloc, heap, ptr, NULL);
		return NULL;
	}

	ret = k_heap_realloc(heap, ptr, size, K_NO_WAIT);

	if (ret != NULL) {
		heap_ref = ret;
		ret = ++heap_ref;
	}

	SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_heap_sys, k_realloc, heap, ptr, ret);

	return ret;
}

/**
 * @brief Assign the system pool to a thread
 *
 * @param thread Pointer to the thread
 */
void k_thread_system_pool_assign(struct k_thread *thread)
{
	thread->resource_pool = _SYSTEM_HEAP;
}
#else
#define _SYSTEM_HEAP	NULL
#endif /* K_HEAP_MEM_POOL_SIZE */

/**
 * @brief Allocate aligned memory for a thread
 *
 * @param align Alignment requirement
 * @param size Size of the memory to allocate
 * @return Pointer to the allocated memory, or NULL on failure
 */
void *z_thread_aligned_alloc(size_t align, size_t size)
{
	void *ret;
	struct k_heap *heap;

	if (k_is_in_isr()) {
		heap = _SYSTEM_HEAP;
	} else {
		heap = _current->resource_pool;
	}

	if (heap != NULL) {
		ret = z_heap_aligned_alloc(heap, align, size);
	} else {
		ret = NULL;
	}

	return ret;
}
//GST