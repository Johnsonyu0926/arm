// Filename: div0.c

/* Replacement (=dummy) for GNU/Linux division-by zero handler */
void __div0(void)
{
    extern void hang(void);

    hang();
}

// By GST @Date