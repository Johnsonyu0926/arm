// Filename: cmd_boot.c

#include <common.h>
#include <command.h>

DECLARE_GLOBAL_DATA_PTR;

/*
 * ARMv7M does not support ARM instruction mode. However, the
 * interworking BLX and BX instructions do encode the ARM/Thumb
 * field in bit 0. This means that when executing any Branch
 * and eXchange instruction we must set bit 0 to one to guarantee
 * that we keep the processor in Thumb instruction mode. From The
 * ARMv7-M Instruction Set A4.1.1:
 *   "ARMv7-M only supports the Thumb instruction execution state,
 *    therefore the value of address bit [0] must be 1 in interworking
 *    instructions, otherwise a fault occurs."
 */
unsigned long do_go_exec(ulong (*entry)(int, char * const []),
                         int argc, char * const argv[])
{
    ulong addr = (ulong)entry | 1;
    entry = (void *)addr;

    return entry(argc, argv);
}

// By GST @Date