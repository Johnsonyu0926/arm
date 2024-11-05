#ifndef _HIK_MEM_TAB_H_
#define _HIK_MEM_TAB_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _MEM_ALIGNMENT {
    MEM_ALIGN_4BYTE = 4,
    MEM_ALIGN_8BYTE = 8,
    MEM_ALIGN_16BYTE = 16,
    MEM_ALIGN_32BYTE = 32,
    MEM_ALIGN_64BYTE = 64,
    MEM_ALIGN_128BYTE = 128
} MEM_ALIGNMENT;

typedef struct _MEM_SPACE {
    void *base;
    size_t size;
    MEM_ALIGNMENT align;
    int type;
} MEM_SPACE;

void* mem_alloc(size_t size, MEM_ALIGNMENT align) {
    void* ptr = nullptr;
    posix_memalign(&ptr, align, size);
    return ptr;
}

void mem_free(void* ptr) {
    free(ptr);
}

int init_mem_space(MEM_SPACE* space, size_t size, MEM_ALIGNMENT align, int type) {
    space->base = mem_alloc(size, align);
    if (!space->base) {
        return -1;
    }
    space->size = size;
    space->align = align;
    space->type = type;
    return 0;
}

void destroy_mem_space(MEM_SPACE* space) {
    if (space->base) {
        mem_free(space->base);
        space->base = nullptr;
    }
    space->size = 0;
    space->align = MEM_ALIGN_4BYTE;
    space->type = 0;
}

#ifdef __cplusplus
}
#endif

#endif // _HIK_MEM_TAB_H_
