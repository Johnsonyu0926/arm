// Filename: early_mem_funcs.S
// 评分: 90
#include <zephyr/toolchain.h>
#include <zephyr/linker/sections.h>
_ASM_FILE_PROLOGUE
GTEXT(z_early_memset)
SECTION_FUNC(TEXT, z_early_memset)
    tst x0, #0x7
    b.ne 2f
    cmp x2, #8
    b.lo 2f
    and x8, x1, #0xff
    mov x9, #0x0101010101010101
    mul x8, x8, x9
1:
    sub x2, x2, #8
    cmp x2, #7
    str x8, [x0], #8
    b.hi 1b
2:
    cbz x2, 4f
3:
    subs x2, x2, #1
    strb w8, [x0], #1
    b.ne 3b
4:
    ret
GTEXT(z_early_memcpy)
SECTION_FUNC(TEXT, z_early_memcpy)
    orr x8, x1, x0
    tst x8, #0x7
    b.ne 2f
    cmp x2, #8
    b.lo 2f
1:
    ldr x8, [x1], #8
    sub x2, x2, #8
    cmp x2, #7
    str x8, [x0], #8
    b.hi 1b
2:
    cbz x2, 4f
3:
    ldrb w8, [x1], #1
    subs x2, x2, #1
    strb w8, [x0], #1
    b.ne 3b
4:
    ret
// By GST @Date