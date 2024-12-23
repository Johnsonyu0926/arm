// Filename: is1046a_serdes.c

#include <common.h>
#include <asm/arch/fsl_serdes.h>
#include <asm/arch/immap_lsch2.h>

struct serdes_config {
    uint32_t protocol;
    uint8_t lanes[SRDS_MAX_LANES];
};

static struct serdes_config serdes1_cfg_tbl[] = {
    /* SerDes 1 */
    {0x3333, {SGMII_FM1_DTSEC9, SGMII_FM1_DTSEC10, SGMII_FM1_DTSEC5, SGMII_FM1_DTSEC6} },
    {0x1133, {XFI_FM1_MAC9, XFI_FM1_MAC10, SGMII_FM1_DTSEC5, SGMII_FM1_DTSEC6} },
    {0x1333, {XFI_FM1_MAC9, SGMII_FM1_DTSEC10, SGMII_FM1_DTSEC5, SGMII_FM1_DTSEC6} },
    {0x2333, {SGMII_2500_FM1_DTSEC9, SGMII_FM1_DTSEC10, SGMII_FM1_DTSEC5, SGMII_FM1_DTSEC6} },
    {0x2233, {SGMII_2500_FM1_DTSEC9, SGMII_2500_FM1_DTSEC10, SGMII_FM1_DTSEC5, SGMII_FM1_DTSEC6} },
    {0x1040, {XFI_FM1_MAC9, NONE, QSGMII_FM1_A, NONE} },
    {0x2040, {SGMII_2500_FM1_DTSEC9, NONE, QSGMII_FM1_A, NONE} },
    {0x1163, {XFI_FM1_MAC9, XFI_FM1_MAC10, PCIE1, SGMII_FM1_DTSEC6} },
    {0x2263, {SGMII_2500_FM1_DTSEC9, SGMII_2500_FM1_DTSEC10, PCIE1, SGMII_FM1_DTSEC6} },
    {0x3363, {SGMII_FM1_DTSEC5, SGMII_FM1_DTSEC6, PCIE1, SGMII_FM1_DTSEC6} },
    {0x2223, {SGMII_2500_FM1_DTSEC9, SGMII_2500_FM1_DTSEC10, SGMII_2500_FM1_DTSEC5, SGMII_FM1_DTSEC6} },
    {}
};

static struct serdes_config serdes2_cfg_tbl[] = {
    /* SerDes 2 */
    {0x8888, {PCIE1, PCIE1, PCIE1, PCIE1} },
    {0x5559, {PCIE1, PCIE2, PCIE3, SATA1} },
    {0x5577, {PCIE1, PCIE2, PCIE3, PCIE3} },
    {0x5506, {PCIE1, PCIE2, NONE, PCIE3} },
    {0x0506, {NONE, PCIE2, NONE, PCIE3} },
    {0x0559, {NONE, PCIE2, PCIE3, SATA1} },
    {0x5A59, {PCIE1, SGMII_FM1_DTSEC2, PCIE3, SATA1} },
    {0x5A06, {PCIE1, SGMII_FM1_DTSEC2, NONE, PCIE3} },
    {}
};

static struct serdes_config *serdes_cfg_tbl[] = {
    serdes1_cfg_tbl,
    serdes2_cfg_tbl,
};

enum srds_prtcl serdes_get_prtcl(int serdes, int cfg, int lane)
{
    struct serdes_config *ptr;

    if (serdes >= ARRAY_SIZE(serdes_cfg_tbl))
        return 0;

    ptr = serdes_cfg_tbl[serdes];
    while (ptr->protocol) {
        if (ptr->protocol == cfg)
            return ptr->lanes[lane];
        ptr++;
    }

    return 0;
}

int is_serdes_prtcl_valid(int serdes, uint32_t prtcl)
{
    int i;
    struct serdes_config *ptr;

    if (serdes >= ARRAY_SIZE(serdes_cfg_tbl))
        return 0;

    ptr = serdes_cfg_tbl[serdes];
    while (ptr->protocol) {
        if (ptr->protocol == prtcl)
            break;
        ptr++;
    }

    if (!ptr->protocol)
        return 0;

    for (i = 0; i < SRDS_MAX_LANES; i++) {
        if (ptr->lanes[i] != NONE)
            return 1;
    }

    return 0;
}

// By GST @Date ok