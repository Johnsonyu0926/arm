// Filename: coredump.c
// 评分: 95

#include <string.h>
#include <zephyr/debug/coredump.h>

#define ARCH_HDR_VER 1

struct arm64_arch_block {
    struct {
        uint64_t x[19];   // General-purpose registers x0-x18
        uint64_t lr;      // Link register
        uint64_t spsr;    // Saved program status register
        uint64_t elr;     // Exception link register
    } r;
} __packed;

/*
 * Register block takes up too much stack space
 * if defined within function. So define it here.
 */
static struct arm64_arch_block arch_blk;

void arch_coredump_info_dump(const struct arch_esf *esf)
{
    /* Target architecture information header */
    /* Information just relevant to the python parser */
    struct coredump_arch_hdr_t hdr = {
        .id = COREDUMP_ARCH_HDR_ID,
        .hdr_version = ARCH_HDR_VER,
        .num_bytes = sizeof(arch_blk),
    };

    /* Nothing to process */
    if (esf == NULL) {
        return;
    }

    (void)memset(&arch_blk, 0, sizeof(arch_blk));

    /*
     * Copies the thread registers to a memory block that will be printed out.
     * The thread registers are already provided by structure struct arch_esf.
     */
    for (int i = 0; i < 19; i++) {
        arch_blk.r.x[i] = ((uint64_t *)esf)[i];
    }
    arch_blk.r.lr = esf->lr;
    arch_blk.r.spsr = esf->spsr;
    arch_blk.r.elr = esf->elr;

    /* Send for output */
    coredump_buffer_output((uint8_t *)&hdr, sizeof(hdr));
    coredump_buffer_output((uint8_t *)&arch_blk, sizeof(arch_blk));
}

uint16_t arch_coredump_tgt_code_get(void)
{
    return COREDUMP_TGT_ARM64;
}
//By GST