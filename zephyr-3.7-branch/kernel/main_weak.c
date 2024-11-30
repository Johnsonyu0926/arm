//kernel/main_weak.c
#include <kernel_internal.h>

int __weak main(void)
{
	/* NOP default main() if the application does not provide one. */
	arch_nop();

	return 0;
}
