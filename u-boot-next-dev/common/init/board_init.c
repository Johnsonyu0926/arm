// Filename: board_init.c

#include <common.h>

DECLARE_GLOBAL_DATA_PTR;

/* Unfortunately x86 or ARM can't compile this code as gd cannot be assigned */
#if !defined(CONFIG_X86) && !defined(CONFIG_ARM)
__weak void arch_setup_gd(struct global_data *gd_ptr)
{
    gd = gd_ptr;
}
#endif /* !CONFIG_X86 && !CONFIG_ARM */

/*
 * Allocate reserved space for use as 'globals' from 'top' address and
 * return 'bottom' address of allocated space
 *
 * Notes:
 *
 * Actual reservation cannot be done from within this function as
 * it requires altering the C stack pointer, so this will be done by
 * the caller upon return from this function.
 *
 * IMPORTANT:
 *
 * Alignment constraints may differ for each 'chunk' allocated. For now:
 *
 * - GD is aligned down on a 16-byte boundary
 *
 *  - the early malloc arena is not aligned, therefore it follows the stack
 *   alignment constraint of the architecture for which we are building.
 *
 *  - GD is allocated last, so that the return value of this functions is
 *   both the bottom of the reserved area and the address of GD, should
 *   the calling context need it.
 */

ulong board_init_f_alloc_reserve(ulong top)
{
    /* Reserve early malloc arena */
#if CONFIG_VAL(SYS_MALLOC_F_LEN)
    top -= CONFIG_VAL(SYS_MALLOC_F_LEN);
#endif
    /* LAST : reserve GD (rounded up to a multiple of 16 bytes) */
    top = rounddown(top - sizeof(struct global_data), 16);

    return top;
}

void board_init_f_init_reserve(ulong base)
{
    struct global_data *gd_ptr;

    /*
     * clear GD entirely and set it up.
     * Use gd_ptr, as gd may not be properly set yet.
     */

    gd_ptr = (struct global_data *)base;
    /* zero the area */
    memset(gd_ptr, '\0', sizeof(*gd));
    /* set GD unless architecture did it already */
#if !defined(CONFIG_ARM)
    arch_setup_gd(gd_ptr);
#endif
    /* next alloc will be higher by one GD plus 16-byte alignment */
    base += roundup(sizeof(struct global_data), 16);

    /*
     * record early malloc arena start.
     * Use gd as it is now properly set for all architectures.
     */

#if CONFIG_VAL(SYS_MALLOC_F_LEN)
    /* go down one 'early malloc arena' */
    gd->malloc_base = base;
    /* next alloc will be higher by one 'early malloc arena' size */
    base += CONFIG_VAL(SYS_MALLOC_F_LEN);
#endif
}

/*
 * Board-specific Platform code can init boot flags if needed
 */
__weak int board_init_f_boot_flags(void)
{
    gd->baudrate = CONFIG_BAUDRATE;
    gd->serial.baudrate = CONFIG_BAUDRATE;
    gd->serial.addr = CONFIG_DEBUG_UART_BASE;
    gd->serial.using_pre_serial = 0;

    return 0;
}

/*
 * Board-specific Platform code can reimplement show_boot_progress () if needed
 */
__weak void show_boot_progress(int val) {}

// By GST @Date