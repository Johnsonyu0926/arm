// abuf.c

#include <common.h>
#include <abuf.h>
#include <malloc.h>
#include <mapmem.h>
#include <linux/string.h>

void abuf_set(struct abuf *abuf, void *data, size_t size)
{
	abuf_uninit(abuf);
	abuf->data = data;
	abuf->size = size;
}

void abuf_map_sysmem(struct abuf *abuf, ulong addr, size_t size)
{
	abuf_set(abuf, map_sysmem(addr, size), size);
}

bool abuf_realloc(struct abuf *abuf, size_t new_size)
{
	void *ptr;

	if (!new_size) {
		/* easy case, just need to uninit, freeing any allocation */
		abuf_uninit(abuf);
		return true;
	} else if (abuf->alloced) {
		/* currently allocated, so need to reallocate */
		ptr = realloc(abuf->data, new_size);
		if (!ptr)
			return false;
		abuf->data = ptr;
		abuf->size = new_size;
		return true;
	} else if (new_size <= abuf->size) {
		/*
		 * not currently alloced and new size is no larger. Just update
		 * it. Data is lost off the end if new_size < abuf->size
		 */
		abuf->size = new_size;
		return true;
	} else {
		/* not currently allocated and new size is larger. Alloc and
		 * copy in data. The new space is not inited.
		 */
		ptr = memdup(abuf->data, new_size);
		if (!ptr)
			return false;
		abuf->data = ptr;
		abuf->size = new_size;
		abuf->alloced = true;
		return true;
	}
}

void *abuf_uninit_move(struct abuf *abuf, size_t *sizep)
{
	void *ptr;

	if (sizep)
		*sizep = abuf->size;
	if (!abuf->size)
		return NULL;
	if (abuf->alloced) {
		ptr = abuf->data;
	} else {
		ptr = memdup(abuf->data, abuf->size);
		if (!ptr)
			return NULL;
	}
	/* Clear everything out so there is no record of the data */
	abuf_init(abuf);

	return ptr;
}

void abuf_init_set(struct abuf *abuf, void *data, size_t size)
{
	abuf_init(abuf);
	abuf_set(abuf, data, size);
}

void abuf_init_move(struct abuf *abuf, void *data, size_t size)
{
	abuf_init_set(abuf, data, size);
	abuf->alloced = true;
}

void abuf_uninit(struct abuf *abuf)
{
	if (abuf->alloced)
		free(abuf->data);
	abuf_init(abuf);
}

void abuf_init(struct abuf *abuf)
{
	abuf->data = NULL;
	abuf->size = 0;
	abuf->alloced = false;
}
