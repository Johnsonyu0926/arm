// Filename: mmu.c
// Score: 92

/* 
 * Copyright 2019 Broadcom
 * The term "Broadcom" refers to Broadcom Inc. and/or its subsidiaries.
 * Copyright (c) 2021 BayLibre, SAS
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/cache.h>
#include <zephyr/device.h>
#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/kernel/mm/demand_paging.h>
#include <kernel_arch_func.h>
#include <kernel_arch_interface.h>
#include <kernel_internal.h>
#include <zephyr/logging/log.h>
#include <zephyr/arch/arm64/cpu.h>
#include <zephyr/arch/arm64/lib_helpers.h>
#include <zephyr/arch/arm64/mm.h>
#include <zephyr/linker/linker-defs.h>
#include <zephyr/spinlock.h>
#include <zephyr/sys/util.h>
#include <mmu.h>

#include "mmu.h"
#include "paging.h"

LOG_MODULE_DECLARE(os, CONFIG_KERNEL_LOG_LEVEL);

static uint64_t xlat_tables[CONFIG_MAX_XLAT_TABLES * Ln_XLAT_NUM_ENTRIES] __aligned(Ln_XLAT_NUM_ENTRIES * sizeof(uint64_t));
static int xlat_use_count[CONFIG_MAX_XLAT_TABLES];
static struct k_spinlock xlat_lock;

/* Usage count value range */
#define XLAT_PTE_COUNT_MASK GENMASK(15, 0)
#define XLAT_REF_COUNT_UNIT BIT(16)

/* Returns a reference to a free table */
static uint64_t *new_table(void)
{
    uint64_t *table;
    unsigned int i;

    /* Look for a free table. */
    for (i = 0U; i < CONFIG_MAX_XLAT_TABLES; i++) {
        if (xlat_use_count[i] == 0) {
            table = &xlat_tables[i * Ln_XLAT_NUM_ENTRIES];
            xlat_use_count[i] = XLAT_REF_COUNT_UNIT;
            MMU_DEBUG("allocating table [%d]%p\n", i, table);
            return table;
        }
    }

    LOG_ERR("CONFIG_MAX_XLAT_TABLES, too small");
    return NULL;
}

static inline unsigned int table_index(uint64_t *pte)
{
    unsigned int i = (pte - xlat_tables) / Ln_XLAT_NUM_ENTRIES;

    __ASSERT(i < CONFIG_MAX_XLAT_TABLES, "table %p out of range", pte);
    return i;
}

/* Adjusts usage count and returns current count. */
static int table_usage(uint64_t *table, int adjustment)
{
    unsigned int i = table_index(table);
    int prev_count = xlat_use_count[i];
    int new_count = prev_count + adjustment;

    /* be reasonable not to always create a debug flood */
    if ((IS_ENABLED(DUMP_PTE) && adjustment != 0) || new_count == 0) {
        MMU_DEBUG("table [%d]%p: usage %#x -> %#x\n", i, table, prev_count, new_count);
    }

    __ASSERT(new_count >= 0, "table use count underflow");
    __ASSERT(new_count == 0 || new_count >= XLAT_REF_COUNT_UNIT, "table in use with no reference to it");
    __ASSERT((new_count & XLAT_PTE_COUNT_MASK) <= Ln_XLAT_NUM_ENTRIES, "table PTE count overflow");

    xlat_use_count[i] = new_count;
    return new_count;
}

static inline void inc_table_ref(uint64_t *table)
{
    table_usage(table, XLAT_REF_COUNT_UNIT);
}

static inline void dec_table_ref(uint64_t *table)
{
    int ref_unit = XLAT_REF_COUNT_UNIT;

    table_usage(table, -ref_unit);
}

static inline bool is_table_unused(uint64_t *table)
{
    return (table_usage(table, 0) & XLAT_PTE_COUNT_MASK) == 0;
}

static inline bool is_table_single_referenced(uint64_t *table)
{
    return table_usage(table, 0) < (2 * XLAT_REF_COUNT_UNIT);
}

#ifdef CONFIG_TEST
/* Hooks to let test code peek at table states */

int arm64_mmu_nb_free_tables(void)
{
    int count = 0;

    for (int i = 0; i < CONFIG_MAX_XLAT_TABLES; i++) {
        if (xlat_use_count[i] == 0) {
            count++;
        }
    }

    return count;
}

int arm64_mmu_tables_total_usage(void)
{
    int count = 0;

    for (int i = 0; i < CONFIG_MAX_XLAT_TABLES; i++) {
        count += xlat_use_count[i];
    }

    return count;
}

#endif /* CONFIG_TEST */

static inline bool is_free_desc(uint64_t desc)
{
    return desc == 0;
}

static inline bool is_inval_desc(uint64_t desc)
{
    /* invalid descriptors aren't necessarily free */
    return (desc & PTE_DESC_TYPE_MASK) == PTE_INVALID_DESC;
}

static inline bool is_table_desc(uint64_t desc, unsigned int level)
{
    return level != XLAT_LAST_LEVEL && (desc & PTE_DESC_TYPE_MASK) == PTE_TABLE_DESC;
}

static inline bool is_block_desc(uint64_t desc)
{
    return (desc & PTE_DESC_TYPE_MASK) == PTE_BLOCK_DESC;
}

static inline uint64_t *pte_desc_table(uint64_t desc)
{
    uint64_t address = desc & PTE_PHYSADDR_MASK;

    /* tables use a 1:1 physical:virtual mapping */
    return (uint64_t *)address;
}

static inline bool is_desc_block_aligned(uint64_t desc, unsigned int level_size)
{
    bool aligned = (desc & PTE_PHYSADDR_MASK & (level_size - 1)) == 0;

    if (!aligned) {
        MMU_DEBUG("misaligned desc 0x%016llx for block size 0x%x\n", desc, level_size);
    }

    return aligned;
}

static inline bool is_desc_superset(uint64_t desc1, uint64_t desc2, unsigned int level)
{
    uint64_t mask = DESC_ATTRS_MASK | GENMASK64(47, LEVEL_TO_VA_SIZE_SHIFT(level));

    return (desc1 & mask) == (desc2 & mask);
}

#if DUMP_PTE
static void debug_show_pte(uint64_t *pte, unsigned int level)
{
    MMU_DEBUG("%.*s", level * 2U, ". . . ");
    MMU_DEBUG("[%d]%p: ", table_index(pte), pte);

    if (is_free_desc(*pte)) {
        MMU_DEBUG("---\n");
        return;
    }

    MMU_DEBUG("0x%016llx ", *pte);

    if (is_table_desc(*pte, level)) {
        uint64_t *table = pte_desc_table(*pte);

        MMU_DEBUG("[Table] [%d]%p\n", table_index(table), table);
        return;
    }

    if (is_block_desc(*pte)) {
        MMU_DEBUG("[Block] ");
    } else if (!is_inval_desc(*pte)) {
        MMU_DEBUG("[Page] ");
    } else {
        MMU_DEBUG("[paged-out] ");
    }

    uint8_t mem_type = (*pte >> 2) & MT_TYPE_MASK;

    MMU_DEBUG((mem_type == MT_NORMAL) ? "MEM" : ((mem_type == MT_NORMAL_NC) ? "NC" : "DEV"));
    MMU_DEBUG((*pte & PTE_BLOCK_DESC_AP_RO) ? "-RO" : "-RW");
    MMU_DEBUG((*pte & PTE_BLOCK_DESC_NS) ? "-NS" : "-S");
    MMU_DEBUG((*pte & PTE_BLOCK_DESC_AP_ELx) ? "-ELx" : "-ELh");
    MMU_DEBUG((*pte & PTE_BLOCK_DESC_PXN) ? "-PXN" : "-PX");
    MMU_DEBUG((*pte & PTE_BLOCK_DESC_UXN) ? "-UXN" : "-UX");
    MMU_DEBUG((*pte & PTE_SW_WRITABLE) ? "-WRITABLE" : "");
    MMU_DEBUG("\n");
}
#else
static inline void debug_show_pte(uint64_t *pte, unsigned int level) { }
#endif

static void set_pte_table_desc(uint64_t *pte, uint64_t *table, unsigned int level)
{
    /* Point pte to new table */
    *pte = PTE_TABLE_DESC | (uint64_t)table;
    debug_show_pte(pte, level);
}

static void set_pte_block_desc(uint64_t *pte, uint64_t desc, unsigned int level)
{
    if (level != XLAT_LAST_LEVEL) {
        desc |= PTE_BLOCK_DESC;
    } else if (!IS_ENABLED(CONFIG_DEMAND_PAGING) || (desc & PTE_BLOCK_DESC_AF) != 0) {
        desc |= PTE_PAGE_DESC;
    } else {
        /*
         * Demand paging configured and AF unset: leave the descriptor
         * type to "invalid" as in arch_mem_page_out().
         */
    }
    *pte = desc;
    debug_show_pte(pte, level);
}

static uint64_t *expand_to_table(uint64_t *pte, unsigned int level)
{
    uint64_t *table;

    __ASSERT(level < XLAT_LAST_LEVEL, "can't expand last level");

    table = new_table();
    if (!table) {
        return NULL;
    }

    if (!is_free_desc(*pte)) {
        /*
         * If entry at current level was already populated
         * then we need to reflect that in the new table.
         */
        uint64_t desc = *pte;
        unsigned int i, stride_shift;

        MMU_DEBUG("expanding PTE 0x%016llx into table [%d]%p\n", desc, table_index(table), table);
        __ASSERT(is_block_desc(desc), "");

        if (level + 1 == XLAT_LAST_LEVEL) {
            desc |= PTE_PAGE_DESC;
        }

        stride_shift = LEVEL_TO_VA_SIZE_SHIFT(level + 1);
        for (i = 0U; i < Ln_XLAT_NUM_ENTRIES; i++) {
            table[i] = desc | (i << stride_shift);
        }
        table_usage(table, Ln_XLAT_NUM_ENTRIES);
    } else {
        /*
         * Adjust usage count for parent table's entry
         * that will no longer be free.
         */
        table_usage(pte, 1);
    }

    /* Link the new table in place of the pte it replaces */
    set_pte_table_desc(pte, table, level);

    return table;
}

static int set_mapping(uint64_t *top_table, uintptr_t virt, size_t size, uint64_t desc, bool may_overwrite)
{
    uint64_t *table = top_table;
    uint64_t *pte;
    uint64_t level_size;
    unsigned int level = BASE_XLAT_LEVEL;

    while (size) {
        __ASSERT(level <= XLAT_LAST_LEVEL, "max translation table level exceeded\n");

        /* Locate PTE for given virtual address and page table level */
        pte = &table[XLAT_TABLE_VA_IDX(virt, level)];

        if (is_table_desc(*pte, level)) {
            /* Move to the next translation table level */
            level++;
            table = pte_desc_table(*pte);
            continue;
        }

        if (!may_overwrite && !is_free_desc(*pte)) {
            /* the entry is already allocated */
            LOG_ERR("entry already in use: level %d pte %p *pte 0x%016llx", level, pte, *pte);
            return -EBUSY;
        }

        level_size = 1ULL << LEVEL_TO_VA_SIZE_SHIFT(level);

        if (is_desc_superset(*pte, desc, level)) {
            /* This block already covers our range */
            level_size -= (virt & (level_size - 1));
            if (level_size > size) {
                level_size = size;
            }
            goto move_on;
        }

        if ((size < level_size) || (virt & (level_size - 1)) || !is_desc_block_aligned(desc, level_size)) {
            /* Range doesn't fit, create subtable */
            table = expand_to_table(pte, level);
            if (!table) {
                return -ENOMEM;
            }
            level++;
            continue;
        }

        /* Adjust usage count for corresponding table */
        if (is_free_desc(*pte)) {
            table_usage(pte, 1);
        }
        /* Create block/page descriptor */
        set_pte_block_desc(pte, desc, level);

move_on:
        virt += level_size;
        desc += level_size;
        size -= level_size;

        /* Range is mapped, start again for next range */
        table = top_table;
        level = BASE_XLAT_LEVEL;
    }

    return 0;
}

static void del_mapping(uint64_t *table, uintptr_t virt, size_t size, unsigned int level)
{
    size_t step, level_size = 1ULL << LEVEL_TO_VA_SIZE_SHIFT(level);
    uint64_t *pte, *subtable;

    for (; size; virt += step, size -= step) {
        step = level_size - (virt & (level_size - 1));
        if (step > size) {
            step = size;
        }
        pte = &table[XLAT_TABLE_VA_IDX(virt, level)];

        if (is_free_desc(*pte)) {
            continue;
        }

        if (step != level_size && is_block_desc(*pte)) {
            /* need to split this block mapping */
            expand_to_table(pte, level);
        }

        if (is_table_desc(*pte, level)) {
            subtable = pte_desc_table(*pte);
            del_mapping(subtable, virt, step, level + 1);
            if (!is_table_unused(subtable)) {
                continue;
            }
            dec_table_ref(subtable);
        }

        /* free this entry */
        *pte = 0;
        table_usage(pte, -1);
    }
}

#ifdef CONFIG_USERSPACE

static uint64_t *dup_table(uint64_t *src_table, unsigned int level)
{
    uint64_t *dst_table = new_table();
    int i, usage_count = 0;

    if (!dst_table) {
        return NULL;
    }

    MMU_DEBUG("dup (level %d) [%d]%p to [%d]%p\n", level, table_index(src_table), src_table, table_index(dst_table), dst_table);

    for (i = 0; i < Ln_XLAT_NUM_ENTRIES; i++) {
        /*
         * After the table duplication, each table can be independently
         * updated. Thus, entries may become non-global.
         * To keep the invariants very simple, we thus force the non-global
         * bit on duplication. Moreover, there is no process to revert this
         * (e.g. in `globalize_table`). Could be improved in future work.
         */
        if (!is_free_desc(src_table[i]) && !is_table_desc(src_table[i], level)) {
            src_table[i] |= PTE_BLOCK_DESC_NG;
        }

        dst_table[i] = src_table[i];
        if (is_table_desc(dst_table[i], level)) {
            inc_table_ref(pte_desc_table(dst_table[i]));
        }
        if (!is_free_desc(dst_table[i])) {
            usage_count++;
        }
    }
    table_usage(dst_table, usage_count);

    return dst_table;
}

static int privatize_table(uint64_t *dst_table, uint64_t *src_table, uintptr_t virt, size_t size, unsigned int level)
{
    size_t step, level_size = 1ULL << LEVEL_TO_VA_SIZE_SHIFT(level);
    unsigned int i;
    int ret;

    for (; size; virt += step, size -= step) {
        step = level_size - (virt & (level_size - 1));
        if (step > size) {
            step = size;
        }
        i = XLAT_TABLE_VA_IDX(virt, level);

        if (!is_table_desc(dst_table[i], level) || !is_table_desc(src_table[i], level)) {
            /* this entry is already private */
            continue;
        }

        uint64_t *dst_subtable = pte_desc_table(dst_table[i]);
        uint64_t *src_subtable = pte_desc_table(src_table[i]);

        if (dst_subtable == src_subtable) {
            /* need to make a private copy of this table */
            dst_subtable = dup_table(src_subtable, level + 1);
            if (!dst_subtable) {
                return -ENOMEM;
            }
            set_pte_table_desc(&dst_table[i], dst_subtable, level);
            dec_table_ref(src_subtable);
        }

        ret = privatize_table(dst_subtable, src_subtable, virt, step, level + 1);
        if (ret) {
            return ret;
        }
    }

    return 0;
}

/*
 * Make the given virtual address range private in dst_pt with regards to
 * src_pt. By "private" this means that corresponding page tables in dst_pt
 * will be duplicated so not to share the same table(s) with src_pt.
 * If corresponding page tables in dst_pt are already distinct from src_pt
 * then nothing is done. This allows for subsequent mapping changes in that
 * range to affect only dst_pt.
 */
static int privatize_page_range(struct arm_mmu_ptables *dst_pt, struct arm_mmu_ptables *src_pt, uintptr_t virt_start, size_t size, const char *name)
{
    k_spinlock_key_t key;
    int ret;

    MMU_DEBUG("privatize [%s]: virt %lx size %lx\n", name, virt_start, size);

    key = k_spin_lock(&xlat_lock);

    ret = privatize_table(dst_pt->base_xlat_table, src_pt->base_xlat_table, virt_start, size, BASE_XLAT_LEVEL);

    k_spin_unlock(&xlat_lock, key);
    return ret;
}

static void discard_table(uint64_t *table, unsigned int level)
{
    unsigned int i;
    int free_count = 0;

    for (i = 0U; i < Ln_XLAT_NUM_ENTRIES; i++) {
        if (is_table_desc(table[i], level)) {
           
请从507行续写
回复 yuJohnson: 
请从507行续写
            discard_table(pte_desc_table(table[i]), level + 1);
        }
        if (!is_free_desc(table[i])) {
            free_count++;
        }
    }

    if (free_count > 0) {
        table_usage(table, -free_count);
    }
}

static void discard_page_range(struct arm_mmu_ptables *ptables, uintptr_t virt_start, size_t size)
{
    k_spinlock_key_t key;

    key = k_spin_lock(&xlat_lock);

    del_mapping(ptables->base_xlat_table, virt_start, size, BASE_XLAT_LEVEL);

    k_spin_unlock(&xlat_lock, key);
}

#endif /* CONFIG_USERSPACE */

void arm_mmu_ptables_init(struct arm_mmu_ptables *ptables)
{
    memset(ptables, 0, sizeof(*ptables));
    ptables->base_xlat_table = new_table();
    __ASSERT(ptables->base_xlat_table != NULL, "failed to allocate base translation table");
}

int arm_mmu_map(struct arm_mmu_ptables *ptables, uintptr_t virt, uintptr_t phys, size_t size, uint64_t attrs)
{
    k_spinlock_key_t key;
    int ret;

    key = k_spin_lock(&xlat_lock);

    ret = set_mapping(ptables->base_xlat_table, virt, size, phys | attrs, true);

    k_spin_unlock(&xlat_lock, key);
    return ret;
}

int arm_mmu_unmap(struct arm_mmu_ptables *ptables, uintptr_t virt, size_t size)
{
    k_spinlock_key_t key;

    key = k_spin_lock(&xlat_lock);

    del_mapping(ptables->base_xlat_table, virt, size, BASE_XLAT_LEVEL);

    k_spin_unlock(&xlat_lock, key);
    return 0;
}

void z_arm64_mmu_init(void)
{
    struct arm_mmu_ptables *ptables = &kernel_ptables;

    arm_mmu_ptables_init(ptables);

    /* Map kernel code and data */
    arm_mmu_map(ptables, (uintptr_t)_image_text_start, (uintptr_t)_image_text_start,
                (size_t)(_image_text_end - _image_text_start), MT_NORMAL | MT_RO | MT_SECURE);
    arm_mmu_map(ptables, (uintptr_t)_image_rodata_start, (uintptr_t)_image_rodata_start,
                (size_t)(_image_rodata_end - _image_rodata_start), MT_NORMAL | MT_RO | MT_SECURE);
    arm_mmu_map(ptables, (uintptr_t)_image_rw_start, (uintptr_t)_image_rw_start,
                (size_t)(_image_rw_end - _image_rw_start), MT_NORMAL | MT_RW | MT_SECURE);
    arm_mmu_map(ptables, (uintptr_t)_image_bss_start, (uintptr_t)_image_bss_start,
                (size_t)(_image_bss_end - _image_bss_start), MT_NORMAL | MT_RW | MT_SECURE);

    /* Enable MMU */
    z_arm64_set_ttbr0((uint64_t)ptables->base_xlat_table);
    __asm__ volatile ("dsb ish; isb; mrs x0, sctlr_el1; orr x0, x0, #1; msr sctlr_el1, x0; isb");
}

// By GST @Date

## 3. `mmu.h`
// Filename: mmu.h
// Score: 90

/* 
 * Copyright (c) 2021 Carlo Caione <ccaione@baylibre.com>
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_ARCH_ARM64_INCLUDE_MMU_H_
#define ZEPHYR_ARCH_ARM64_INCLUDE_MMU_H_

#include <zephyr/types.h>
#include <zephyr/toolchain.h>
#include <zephyr/linker/sections.h>

#define MAIR_EL1_VAL 0xFF
#define TCR_EL1_VAL 0x80000000
#define TTBR0_EL1_VAL 0x40000000
#define TTBR1_EL1_VAL 0x20000000
#define SCTLR_M 0x1

#define MT_NORMAL 0x0
#define MT_RW 0x1
#define MT_SECURE 0x2

struct arm_mmu_region {
    const char *name;
    uintptr_t base_pa;
    size_t size;
    uint32_t attrs;
};

#define MMU_REGION_FLAT_ENTRY(_name, _base_pa, _size, _attrs) \
    { .name = _name, .base_pa = _base_pa, .size = _size, .attrs = _attrs }

void arm_mmu_map_regions(const struct arm_mmu_region *regions, size_t num_regions);

#endif /* ZEPHYR_ARCH_ARM64_INCLUDE_MMU_H_ */

// By GST @Date