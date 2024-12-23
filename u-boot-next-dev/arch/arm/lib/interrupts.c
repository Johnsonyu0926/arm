// Filename: interrupts.c

#include <common.h>
#include <asm/proc-armv/ptrace.h>
#include <asm/u-boot-arm.h>
#include <efi_loader.h>
#include <iomem.h>
#include <stacktrace.h>

DECLARE_GLOBAL_DATA_PTR;

#if !CONFIG_IS_ENABLED(IRQ)
int interrupt_init(void)
{
    /*
     * setup up stacks if necessary
     */
    IRQ_STACK_START_IN = gd->irq_sp + 8;

    return 0;
}

void enable_interrupts(void)
{
    return;
}

int disable_interrupts(void)
{
    return 0;
}
#endif

void bad_mode(void)
{
    panic("Resetting CPU ...\n");
    reset_cpu(0);
}

void show_regs(struct pt_regs *regs)
{
    ulong pc, lr;
    unsigned long __maybe_unused flags;
    const char __maybe_unused *processor_modes[] = {
        "USER_26", "FIQ_26", "IRQ_26", "SVC_26",
        "UK4_26", "UK5_26", "UK6_26", "UK7_26",
        "UK8_26", "UK9_26", "UK10_26", "UK11_26",
        "UK12_26", "UK13_26", "UK14_26", "UK15_26",
        "USER_32", "FIQ_32", "IRQ_32", "SVC_32",
        "UK4_32", "UK5_32", "UK6_32", "ABT_32",
        "UK8_32", "UK9_32", "HYP_32", "UND_32",
        "UK12_32", "UK13_32", "UK14_32", "SYS_32",
    };

    flags = condition_codes(regs);

    if (gd->flags & GD_FLG_RELOC) {
        pc = instruction_pointer(regs) - gd->reloc_off;
        lr = regs->ARM_lr - gd->reloc_off;
    } else {
        pc = instruction_pointer(regs);
        lr = regs->ARM_lr;
    }

    printf("pc : %08lx  lr : %08lx\n", pc, lr);
    printf("sp : %08lx  ip : %08lx  fp : %08lx\n",
           regs->ARM_sp, regs->ARM_ip, regs->ARM_fp);
    printf("r10: %08lx  r9 : %08lx  r8 : %08lx\n",
           regs->ARM_r10, regs->ARM_r9, regs->ARM_r8);
    printf("r7 : %08lx  r6 : %08lx  r5 : %08lx  r4 : %08lx\n",
           regs->ARM_r7, regs->ARM_r6, regs->ARM_r5, regs->ARM_r4);
    printf("r3 : %08lx  r2 : %08lx  r1 : %08lx  r0 : %08lx\n",
           regs->ARM_r3, regs->ARM_r2, regs->ARM_r1, regs->ARM_r0);
    printf("Flags: %c%c%c%c",
           flags & CC_N_BIT ? 'N' : 'n',
           flags & CC_Z_BIT ? 'Z' : 'z',
           flags & CC_C_BIT ? 'C' : 'c', flags & CC_V_BIT ? 'V' : 'v');
    printf("  IRQs %s  FIQs %s  Mode %s%s\n\n",
           interrupts_enabled(regs) ? "on" : "off",
           fast_interrupts_enabled(regs) ? "on" : "off",
           processor_modes[processor_mode(regs)],
           thumb_mode(regs) ? " (T)" : "");

#ifdef CONFIG_ROCKCHIP_CRASH_DUMP
    iomem_show_by_compatible("-cru", 0, 0x400);
    iomem_show_by_compatible("-pmucru", 0, 0x400);
    iomem_show_by_compatible("-grf", 0, 0x400);
    iomem_show_by_compatible("-pmugrf", 0, 0x400);
#endif

    dump_core_stack(regs);
}

/* fixup PC to point to the instruction leading to the exception */
static inline void fixup_pc(struct pt_regs *regs, int offset)
{
    uint32_t pc = instruction_pointer(regs) + offset;
    regs->ARM_pc = pc | (regs->ARM_pc & PCMASK);
}

void do_undefined_instruction(struct pt_regs *pt_regs)
{
    efi_restore_gd();
    printf("undefined instruction\n");
    fixup_pc(pt_regs, -4);
    show_regs(pt_regs);
    bad_mode();
}

void do_software_interrupt(struct pt_regs *pt_regs)
{
    efi_restore_gd();
    printf("software interrupt\n");
    fixup_pc(pt_regs, -4);
    show_regs(pt_regs);
    bad_mode();
}

void do_prefetch_abort(struct pt_regs *pt_regs)
{
    efi_restore_gd();
    printf("prefetch abort\n");
    fixup_pc(pt_regs, -8);
    show_regs(pt_regs);
    bad_mode();
}

void do_data_abort(struct pt_regs *pt_regs)
{
    efi_restore_gd();
    printf("data abort\n");
    fixup_pc(pt_regs, -8);
    show_regs(pt_regs);
    bad_mode();
}

void do_not_used(struct pt_regs *pt_regs)
{
    efi_restore_gd();
    printf("not used\n");
    fixup_pc(pt_regs, -8);
    show_regs(pt_regs);
    bad_mode();
}

void do_fiq(struct pt_regs *pt_regs)
{
    efi_restore_gd();
    printf("fast interrupt request\n");
    fixup_pc(pt_regs, -8);
    show_regs(pt_regs);
    bad_mode();
}

#if !CONFIG_IS_ENABLED(IRQ)
void do_irq(struct pt_regs *pt_regs)
{
    efi_restore_gd();
    printf("interrupt request\n");
    fixup_pc(pt_regs, -8);
    show_regs(pt_regs);
    bad_mode();
}
#endif

// By GST @Date