// Filename: bootm-fdt.c

#include <common.h>
#include <fdt_support.h>
#ifdef CONFIG_ARMV7_NONSEC
#include <asm/armv7.h>
#endif
#include <asm/psci.h>
#include <asm/spin_table.h>
#include <bidram.h>

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_FMAN_ENET
__weak int fdt_update_ethernet_dt(void *blob)
{
    return 0;
}
#endif

__weak int board_fdt_fixup(void *blob)
{
    return 0;
}

int arch_fixup_fdt(void *blob)
{
    int ret = 0;

    ret = board_fdt_fixup(blob);
    if (ret)
        return ret;

    /* Show "/reserved-memory" */
    boot_mem_rsv_regions(NULL, blob);

#if defined(CONFIG_ARMV7_NONSEC) || defined(CONFIG_OF_LIBFDT)
    bd_t *bd = gd->bd;
    int bank;
    u64 start[CONFIG_NR_DRAM_BANKS];
    u64 size[CONFIG_NR_DRAM_BANKS];

#ifdef CONFIG_BIDRAM
    bidram_fixup();
#endif
    for (bank = 0; bank < CONFIG_NR_DRAM_BANKS; bank++) {
        start[bank] = bd->bi_dram[bank].start;
        size[bank] = bd->bi_dram[bank].size;
        if (size[bank] == 0)
            continue;
#ifdef CONFIG_ARCH_FIXUP_FDT_MEMORY
        printf("Adding bank: 0x%08llx - 0x%08llx (size: 0x%08llx)\n",
               start[bank], start[bank] + size[bank], size[bank]);
#endif
#ifdef CONFIG_ARMV7_NONSEC
        ret = armv7_apply_memory_carveout(&start[bank], &size[bank]);
        if (ret)
            return ret;
#endif
    }

#ifdef CONFIG_OF_LIBFDT
    ret = fdt_fixup_memory_banks(blob, start, size, CONFIG_NR_DRAM_BANKS);
    if (ret)
        return ret;
#endif

#ifdef CONFIG_ARMV8_SPIN_TABLE
    ret = spin_table_update_dt(blob);
    if (ret)
        return ret;
#endif

#if defined(CONFIG_ARMV7_NONSEC) || defined(CONFIG_ARMV8_PSCI) || \
    defined(CONFIG_SEC_FIRMWARE_ARMV8_PSCI)
    ret = psci_update_dt(blob);
    if (ret)
        return ret;
#endif
#endif

#ifdef CONFIG_FMAN_ENET
    ret = fdt_update_ethernet_dt(blob);
    if (ret)
        return ret;
#endif
    return 0;
}

// By GST @Date