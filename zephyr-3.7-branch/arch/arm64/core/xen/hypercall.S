//arch/arc/core/xen/hypercall.c
#include <zephyr/toolchain.h>
#include <zephyr/linker/sections.h>
#include <zephyr/xen/public/arch-arm.h>
#include <zephyr/xen/public/xen.h>

/**
 * @brief Macro to define a hypercall function.
 *
 * This macro defines a function for a specific hypercall by setting up the
 * appropriate assembly instructions.
 *
 * @param hypercall The name of the hypercall.
 */
#define HYPERCALL(hypercall)			\
GTEXT(HYPERVISOR_##hypercall);			\
SECTION_FUNC(TEXT, HYPERVISOR_##hypercall)	\
	mov x16, #__HYPERVISOR_##hypercall;	\
	hvc XEN_HYPERCALL_TAG;			\
	ret;

/* Assembly file prologue */
_ASM_FILE_PROLOGUE

/* Define hypercall functions */
HYPERCALL(console_io);
HYPERCALL(grant_table_op);
HYPERCALL(sched_op);
HYPERCALL(event_channel_op);
HYPERCALL(hvm_op);
HYPERCALL(memory_op);

#ifdef CONFIG_XEN_DOM0
HYPERCALL(domctl);
#endif
//GST
