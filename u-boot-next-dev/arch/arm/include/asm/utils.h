// Filename: utils.h

#ifndef _UTILS_H_
#define _UTILS_H_

static inline s32 log_2_n_round_up(u32 n)
{
    s32 log2n = -1;
    u32 temp = n;

    while (temp) {
        log2n++;
        temp >>= 1;
    }

    if (n & (n - 1))
        return log2n + 1; /* not power of 2 - round up */
    else
        return log2n; /* power of 2 */
}

static inline s32 log_2_n_round_down(u32 n)
{
    s32 log2n = -1;
    u32 temp = n;

    while (temp) {
        log2n++;
        temp >>= 1;
    }

    return log2n;
}

#endif /* _UTILS_H_ */

// By GST @Date