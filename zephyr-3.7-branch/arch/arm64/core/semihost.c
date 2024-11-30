//arch/arc/core/semihost.c
#include <zephyr/arch/common/semihost.h>

/**
 * @brief Execute a semihosting instruction
 *
 * This function executes a semihosting instruction with the given arguments.
 *
 * @param instr The semihosting instruction to execute
 * @param args The arguments for the semihosting instruction
 * @return The result of the semihosting instruction
 */
long semihost_exec(enum semihost_instr instr, void *args)
{
	register unsigned long w0 __asm__("w0") = instr;
	register void *x1 __asm__("x1") = args;
	register long ret __asm__("x0");

	__asm__ volatile ("hlt 0xf000"
			  : "=r" (ret) : "r" (w0), "r" (x1) : "memory");
	return ret;
}
//GST