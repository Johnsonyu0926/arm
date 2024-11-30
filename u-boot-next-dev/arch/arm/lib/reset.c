// Filename: reset.c

#include <common.h>

__weak void reset_misc(void)
{
}

int do_reset(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    puts("resetting ...\n");

    udelay(50000);             /* wait 50 ms */

    disable_interrupts();

    reset_misc();
    reset_cpu(0);

    /*NOTREACHED*/
    return 0;
}

// By GST @Date