// Filename: cpu.c

#include <common.h>
#include <command.h>
#include <asm/system.h>
#include <asm/secure.h>
#include <linux/compiler.h>

/*
 * sdelay() - simple spin loop.
 *
 * Will delay execution by roughly (@loops * 2) cycles.
 * This is necessary to be used before timers are accessible.
 *
 * A value of "0" will result in 2^64 loops.
 */
void sdelay(unsigned long loops)
{
    __asm__ volatile ("1:\n"
                      "subs %0, %0, #1\n"
                      "b.ne 1b"
                      : "=r" (loops)
                      : "0"(loops)
                      : "cc");
}

int cleanup_before_linux(void)
{
    /*
     * This function is called just before we call Linux.
     * It prepares the processor for Linux.
     *
     * Disable interrupts and turn off caches, etc.
     */
    disable_interrupts();

    disable_serror();

    /*
     * Turn off I-cache and invalidate it.
     */
    icache_disable();
    invalidate_icache_all();

    /*
     * Turn off D-cache.
     * dcache_disable() in turn flushes the D-cache and disables MMU.
     */
    dcache_disable();
    invalidate_dcache_all();

    return 0;
}

#ifdef CONFIG_ARMV8_PSCI
static void relocate_secure_section(void)
{
#ifdef CONFIG_ARMV8_SECURE_BASE
    size_t sz = __secure_end - __secure_start;

    memcpy((void *)CONFIG_ARMV8_SECURE_BASE, __secure_start, sz);
    flush_dcache_range(CONFIG_ARMV8_SECURE_BASE,
                       CONFIG_ARMV8_SECURE_BASE + sz + 1);
    invalidate_icache_all();
#endif
}

void armv8_setup_psci(void)
{
    relocate_secure_section();
    secure_ram_addr(psci_setup_vectors)();
    secure_ram_addr(psci_arch_init)();
}
#endif

// By GST @Date