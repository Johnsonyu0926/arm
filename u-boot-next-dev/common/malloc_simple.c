//malloc_simple.c

#include <common.h>
#include <malloc.h>
#include <mapmem.h>
#include <asm/io.h>

DECLARE_GLOBAL_DATA_PTR;

void *malloc_simple(size_t bytes)
{
	ulong new_ptr;
	void *ptr;

	new_ptr = gd->malloc_ptr + bytes;
	debug("%s: size=0x%lx, ptr=0x%lx, limit=0x%lx: ", __func__, (unsigned long)bytes, new_ptr,
	      gd->malloc_limit);
	if (new_ptr > gd->malloc_limit) {
		debug("space exhausted\n");
		return NULL;
	}
	ptr = map_sysmem(gd->malloc_base + gd->malloc_ptr, bytes);
	gd->malloc_ptr = ALIGN(new_ptr, sizeof(new_ptr));
	debug("0x%lx\n", (ulong)ptr);

	return ptr;
}

void *memalign_simple(size_t align, size_t bytes)
{
	ulong addr, new_ptr;
	void *ptr;

	addr = ALIGN(gd->malloc_base + gd->malloc_ptr, align);
	new_ptr = addr + bytes - gd->malloc_base;
	if (new_ptr > gd->malloc_limit) {
		printf("sys malloc pool space exhausted\n");
		return NULL;
	}

	ptr = map_sysmem(addr, bytes);
	gd->malloc_ptr = ALIGN(new_ptr, sizeof(new_ptr));
	debug("%lx\n", (ulong)ptr);

	return ptr;
}

#if CONFIG_IS_ENABLED(SYS_MALLOC_SIMPLE)
void *calloc(size_t nmemb, size_t elem_size)
{
	size_t size = nmemb * elem_size;
	void *ptr;

	ptr = malloc(size);
	memset(ptr, '\0', size);

	return ptr;
}
#endif
