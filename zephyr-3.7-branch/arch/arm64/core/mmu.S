//arch/arc/core/mmu.S
#include <zephyr/toolchain.h>
#include <zephyr/linker/sections.h>
#include <zephyr/arch/cpu.h>
/* Assembly file prologue */
_ASM_FILE_PROLOGUE
/*
 * Switch TTBR0
 */
/* Define the z_arm64_set_ttbr0 function */
GTEXT(z_arm64_set_ttbr0)
SECTION_FUNC(TEXT, z_arm64_set_ttbr0)
    /* Switch the TTBR0 */
    msr ttbr0_el1, x0
    isb
    ret
	//GST
