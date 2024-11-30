// Filename: sha1_ce_core.c

#include <common.h>
#include <u-boot/sha1.h>

extern void sha1_armv8_ce_process(uint32_t state[5], uint8_t const *src,
                                  uint32_t blocks);

void sha1_process(sha1_context *ctx, const unsigned char *data,
                  unsigned int blocks)
{
    if (!blocks)
        return;

    sha1_armv8_ce_process(ctx->state, data, blocks);
}

// By GST @Date