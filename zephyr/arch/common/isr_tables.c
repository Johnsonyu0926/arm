// Filename: isr_tables.c
// 评分: 90

#include <zephyr/toolchain.h>
#include <zephyr/linker/sections.h>
#include <zephyr/sw_isr_table.h>
#include <zephyr/arch/cpu.h>

struct int_list_header {
    uint32_t table_size;
    uint32_t offset;
#if defined(CONFIG_ISR_TABLES_LOCAL_DECLARATION)
    uint32_t swi_table_entry_size;
    uint32_t shared_isr_table_entry_size;
    uint32_t shared_isr_client_num_offset;
#endif
};

Z_GENERIC_SECTION(.irq_info) __used struct int_list_header _iheader = {
    .table_size = IRQ_TABLE_SIZE,
    .offset = CONFIG_GEN_IRQ_START_VECTOR,
#if defined(CONFIG_ISR_TABLES_LOCAL_DECLARATION)
    .swi_table_entry_size = sizeof(struct _isr_table_entry),
#if defined(CONFIG_SHARED_INTERRUPTS)
    .shared_isr_table_entry_size = sizeof(struct z_shared_isr_table_entry),
    .shared_isr_client_num_offset = offsetof(struct z_shared_isr_table_entry, client_num),
#endif
#endif
};

#ifdef CONFIG_GEN_IRQ_VECTOR_TABLE
#ifdef CONFIG_GEN_SW_ISR_TABLE
#define IRQ_VECTOR_TABLE_DEFAULT_ISR _isr_wrapper
#else
#define IRQ_VECTOR_TABLE_DEFAULT_ISR z_irq_spurious
#endif

#ifdef CONFIG_IRQ_VECTOR_TABLE_JUMP_BY_CODE
#ifndef ARCH_IRQ_VECTOR_JUMP_CODE
#error "ARCH_IRQ_VECTOR_JUMP_CODE not defined"
#endif

#define BUILD_VECTOR(n, _) __asm(ARCH_IRQ_VECTOR_JUMP_CODE(IRQ_VECTOR_TABLE_DEFAULT_ISR))

void __irq_vector_table __attribute__((naked)) _irq_vector_table(void) {
    LISTIFY(CONFIG_NUM_IRQS, BUILD_VECTOR, (;));
};
#else
uintptr_t __irq_vector_table _irq_vector_table[IRQ_TABLE_SIZE] = {
    [0 ...(IRQ_TABLE_SIZE - 1)] = (uintptr_t)&IRQ_VECTOR_TABLE_DEFAULT_ISR,
};
#endif
#endif

#ifdef CONFIG_GEN_SW_ISR_TABLE
struct _isr_table_entry __sw_isr_table _sw_isr_table[IRQ_TABLE_SIZE] = {
    [0 ...(IRQ_TABLE_SIZE - 1)] = {(const void *)0x42, &z_irq_spurious},
};
#endif

#ifdef CONFIG_SHARED_INTERRUPTS
struct z_shared_isr_table_entry __shared_sw_isr_table z_shared_sw_isr_table[IRQ_TABLE_SIZE] = {};
#endif
// By GST @Date