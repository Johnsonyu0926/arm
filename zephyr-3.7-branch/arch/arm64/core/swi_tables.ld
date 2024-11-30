//arch/arc/core/swi_tables.ld
#if LINKER_ZEPHYR_FINAL && defined(CONFIG_ISR_TABLES_LOCAL_DECLARATION)
INCLUDE isr_tables_swi.ld
#endif
