// Filename: board_info.c

#include <common.h>
#include <linux/libfdt.h>
#include <linux/compiler.h>

int __weak checkboard(void)
{
    return 0;
}

/*
 * If the root node of the DTB has a "model" property, show it.
 * Then call checkboard().
 */
int __weak show_board_info(void)
{
#ifdef CONFIG_OF_CONTROL
    DECLARE_GLOBAL_DATA_PTR;
    const char *model;

    model = fdt_getprop(gd->fdt_blob, 0, "model", NULL);

    if (model)
        printf("Model: %s\n", model);
#endif
    if (!(gd->flags & GD_FLG_RELOC))
        printf("MPIDR: 0x%lx\n", (ulong)read_mpidr() & 0xfff);

#ifdef CONFIG_ARM64_BOOT_AARCH32
    if (!(gd->flags & GD_FLG_RELOC))
        printf("CPU: AArch32\n");
#endif

    return checkboard();
}

// By GST @Date