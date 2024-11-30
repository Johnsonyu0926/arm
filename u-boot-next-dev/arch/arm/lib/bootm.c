// Filename: bootm.c

#include <common.h>
#include <command.h>
#include <amp.h>
#include <dm.h>
#include <dm/root.h>
#include <image.h>
#include <u-boot/zlib.h>
#include <asm/byteorder.h>
#include <linux/libfdt.h>
#include <mapmem.h>
#include <mp_boot.h>
#include <fdt_support.h>
#include <asm/bootm.h>
#include <asm/secure.h>
#include <linux/compiler.h>
#include <bootm.h>
#include <vxworks.h>

#ifdef CONFIG_ARMV7_NONSEC
#include <asm/armv7.h>
#endif
#include <asm/setup.h>
#include <asm/arch/rockchip_smccc.h>

DECLARE_GLOBAL_DATA_PTR;

static struct tag *params;

static ulong get_sp(void)
{
    ulong ret;

    asm("mov %0, sp" : "=r"(ret) : );
    return ret;
}

void arch_lmb_reserve(struct lmb *lmb)
{
    ulong sp;

    /*
     * Booting a (Linux) kernel image
     *
     * Allocate space for command line and board info - the
     * address should be as high as possible within the reach of
     * the kernel (see CONFIG_SYS_BOOTMAPSZ settings), but in unused
     * memory, which means far enough below the current stack
     * pointer.
     */
    sp = get_sp();
    debug("## Current stack ends at 0x%08lx ", sp);

    /* adjust sp by 4K to be safe */
    sp -= 4096;
    lmb_reserve(lmb, sp,
                gd->ram_top - sp);
}

__weak void board_quiesce_devices(void *images)
{
}

/**
 * announce_and_cleanup() - Print message and prepare for kernel boot
 *
 * @fake: non-zero to do everything except actually boot
 */
static void announce_and_cleanup(bootm_headers_t *images, int fake)
{
    ulong us, tt_us;

    bootstage_mark_name(BOOTSTAGE_ID_BOOTM_HANDOFF, "start_kernel");
#ifdef CONFIG_BOOTSTAGE_FDT
    bootstage_fdt_add_report();
#endif
#ifdef CONFIG_BOOTSTAGE_REPORT
    bootstage_report();
#endif

#ifdef CONFIG_USB_DEVICE
    udc_disconnect();
#endif

    board_quiesce_devices(images);

    /* Flush all console data */
    flushc();

    /*
     * Call remove function of all devices with a removal flag set.
     * This may be useful for last-stage operations, like cancelling
     * of DMA operation or releasing device internal buffers.
     */
    dm_remove_devices_flags(DM_REMOVE_ACTIVE_ALL);

    cleanup_before_linux();

#ifdef CONFIG_MP_BOOT
    mpb_post(4);
#endif
    us = (get_ticks() - gd->sys_start_tick) / (COUNTER_FREQUENCY / 1000000);
    tt_us = get_ticks() / (COUNTER_FREQUENCY / 1000000);
    printf("Total: %ld.%ld/%ld.%ld ms\n", us / 1000, us % 1000, tt_us / 1000, tt_us % 1000);

    printf("\nStarting kernel ...%s\n\n", fake ?
        "(fake run for tracing)" : "");
}

static void setup_start_tag(bd_t *bd)
{
    params = (struct tag *)bd->bi_boot_params;

    params->hdr.tag = ATAG_CORE;
    params->hdr.size = tag_size(tag_core);

    params->u.core.flags = 0;
    params->u.core.pagesize = 0;
    params->u.core.rootdev = 0;

    params = tag_next(params);
}

static void setup_memory_tags(bd_t *bd)
{
    int i;

    for (i = 0; i < CONFIG_NR_DRAM_BANKS; i++) {
        params->hdr.tag = ATAG_MEM;
        params->hdr.size = tag_size(tag_mem32);

        params->u.mem.start = bd->bi_dram[i].start;
        params->u.mem.size = bd->bi_dram[i].size;

        params = tag_next(params);
    }
}

static void setup_commandline_tag(bd_t *bd, char *commandline)
{
    char *p;

    if (!commandline)
        return;

    /* eat leading white space */
    for (p = commandline; *p == ' '; p++);

    /* skip non-existent command lines so the kernel will still
     * use its default command line.
     */
    if (*p == '\0')
        return;

    params->hdr.tag = ATAG_CMDLINE;
    params->hdr.size =
        (sizeof(struct tag_header) + strlen(p) + 1 + 4) >> 2;

    strcpy(params->u.cmdline.cmdline, p);

    params = tag_next(params);
}

static void setup_initrd_tag(bd_t *bd, ulong initrd_start, ulong initrd_end)
{
    /* an ATAG_INITRD node tells the kernel where the compressed
     * ramdisk can be found. ATAG_RDIMG is a better name, actually.
     */
    params->hdr.tag = ATAG_INITRD2;
    params->hdr.size = tag_size(tag_initrd);

    params->u.initrd.start = initrd_start;
    params->u.initrd.size = initrd_end - initrd_start;

    params = tag_next(params);
}

static void setup_serial_tag(struct tag **tmp)
{
    struct tag *params = *tmp;
    struct tag_serialnr serialnr;

    get_board_serial(&serialnr);
    params->hdr.tag = ATAG_SERIAL;
    params->hdr.size = tag_size(tag_serialnr);
    params->u.serialnr.low = serialnr.low;
    params->u.serialnr.high = serialnr.high;
    params = tag_next(params);
    *tmp = params;
}

static void setup_revision_tag(struct tag **in_params)
{
    u32 rev = 0;

    rev = get_board_rev();
    params->hdr.tag = ATAG_REVISION;
    params->hdr.size = tag_size(tag_revision);
    params->u.revision.rev = rev;
    params = tag_next(params);
}

static void setup_end_tag(bd_t *bd)
{
    params->hdr.tag = ATAG_NONE;
    params->hdr.size = 0;
}

__weak void setup_board_tags(struct tag **in_params) {}

#ifdef CONFIG_ARM64
static void do_nonsec_virt_switch(void)
{
    smp_kick_all_cpus();
    dcache_disable();    /* flush cache before switching to EL2 */
}
#endif

/* Subcommand: PREP */
static void boot_prep_linux(bootm_headers_t *images)
{
    char *commandline = env_get("bootargs");

    if (IMAGE_ENABLE_OF_LIBFDT && images->ft_len) {
#ifdef CONFIG_OF_LIBFDT
        debug("using: FDT\n");
        if (image_setup_linux(images)) {
            printf("FDT creation failed! hanging...");
            hang();
        }
#endif
    } else if (BOOTM_ENABLE_TAGS) {
        debug("using: ATAGS\n");
        setup_start_tag(gd->bd);
        if (BOOTM_ENABLE_SERIAL_TAG)
            setup_serial_tag(&params);
        if (BOOTM_ENABLE_CMDLINE_TAG)
            setup_commandline_tag(gd->bd, commandline);
        if (BOOTM_ENABLE_REVISION_TAG)
            setup_revision_tag(&params);
        if (BOOTM_ENABLE_MEMORY_TAGS)
            setup_memory_tags(gd->bd);
        if (BOOTM_ENABLE_INITRD_TAG) {
            /*
             * In boot_ramdisk_high(), it may relocate ramdisk to
             * a specified location. And set images->initrd_start &
             * images->initrd_end to relocated ramdisk's start/end
             * addresses. So use them instead of images->rd_start &
             * images->rd_end when possible.
             */
            if (images->initrd_start && images->initrd_end) {
                setup_initrd_tag(gd->bd, images->initrd_start,
                         images->initrd_end);
            } else if (images->rd_start && images->rd_end) {
                setup_initrd_tag(gd->bd, images->rd_start,
                         images->rd_end);
            }
        }
        setup_board_tags(&params);
        setup_end_tag(gd->bd);
    } else {
        printf("FDT and ATAGS support not compiled in - hanging\n");
        hang();
    }
}

__weak bool armv7_boot_nonsec_default(void)
{
#ifdef CONFIG_ARMV7_BOOT_SEC_DEFAULT
    return false;
#else
    return true;
#endif
}

#ifdef CONFIG_ARMV7_NONSEC
bool armv7_boot_nonsec(void)
{
    char *s = env_get("bootm_boot_mode");
    bool nonsec = armv7_boot_nonsec_default();

    if (s && !strcmp(s, "sec"))
        nonsec = false;

    if (s && !strcmp(s, "nonsec"))
        nonsec = true;

    return nonsec;
}
#endif

#ifdef CONFIG_ARM64
__weak void update_os_arch_secondary_cores(uint8_t os_arch)
{
}

#ifdef CONFIG_ARMV8_SWITCH_TO_EL1
static void switch_to_el1(void)
{
    if ((IH_ARCH_DEFAULT == IH_ARCH_ARM64) &&
        (images.os.arch == IH_ARCH_ARM))
        armv8_switch_to_el1(0, (u64)gd->bd->bi_arch_number,
                    (u64)images.ft_addr, 0,
                    (u64)images.ep,
                    ES_TO_AARCH32);
    else
        armv8_switch_to_el1((u64)images.ft_addr, 0, 0, 0,
                    images.ep,
                    ES_TO_AARCH64);
}
#endif
#endif

#ifdef CONFIG_ARM64_SWITCH_TO_AARCH32
static int arm64_switch_aarch32(bootm_headers_t *images)
{
    void *fdt = images->ft_addr;
    ulong mpidr;
    int ret, es_flag;
    int nodeoff, tmp;
    int num = -1;

    /* arm aarch32 SVC */
    images->os.arch = IH_ARCH_ARM;
    es_flag = PE_STATE(0, 0, 0, 0);

    nodeoff = fdt_path_offset(fdt, "/cpus");
    if (nodeoff < 0) {
        printf("couldn't find /cpus\n");
        return nodeoff;
    }

    /* config all nonboot cpu state */
    for (tmp = fdt_first_subnode(fdt, nodeoff);
         tmp >= 0;
         tmp = fdt_next_subnode(fdt, tmp)) {
        const struct fdt_property *prop;
        int len;

        prop = fdt_get_property(fdt, tmp, "device_type", &len);
        if (!prop)
            continue;
        if (len < 4)
            continue;
        if (strcmp(prop->data, "cpu"))
            continue;
        /* skip boot(first) cpu */
        num++;
        if (num == 0)
            continue;

        mpidr = (ulong)fdtdec_get_addr_size_auto_parent(fdt,
                    nodeoff, tmp, "reg", 0, NULL, false);
        ret = sip_smc_amp_cfg(AMP_PE_STATE, mpidr, es_flag, 0);
        if (ret) {
            printf("CPU@%lx init AArch32 failed: %d\n", mpidr, ret);
            continue;
        }
    }

    return es_flag;
}
#endif

/* Subcommand: GO */
static void boot_jump_linux(bootm_headers_t *images, int flag)
{
#ifdef CONFIG_ARM64
    void (*kernel_entry)(void *fdt_addr, void *res0, void *res1,
            void *res2);
    int fake = (flag & BOOTM_STATE_OS_FAKE_GO);
    int es_flag = 0;

#if defined(CONFIG_AMP)
    es_flag = arm64_switch_amp_pe(images);
#elif defined(CONFIG_ARM64_SWITCH_TO_AARCH32)
    es_flag = arm64_switch_aarch32(images);
#endif
    kernel_entry = (void (*)(void *fdt_addr, void *res0, void *res1,
                void *res2))images->ep;

    debug("## Transferring control to Linux (at address %lx)...\n",
        (ulong) kernel_entry);
    bootstage_mark(BOOTSTAGE_ID_RUN_OS);

    announce_and_cleanup(images, fake);

    if (!fake) {
#ifdef CONFIG_ARMV8_PSCI
        armv8_setup_psci();
#endif
        do_nonsec_virt_switch();

        update_os_arch_secondary_cores(images->os.arch);

#ifdef CONFIG_ARMV8_SWITCH_TO_EL1
        armv8_switch_to_el2((u64)images->ft_addr, 0, 0, 0,
                    (u64)switch_to_el1, ES_TO_AARCH64);
#else
        if ((IH_ARCH_DEFAULT == IH_ARCH_ARM64) &&
            (images->os.arch == IH_ARCH_ARM))
            armv8_switch_to_el2(0, (u64)gd->bd->bi_arch_number,
                        (u64)images->ft_addr, es_flag,
                        (u64)images->ep,
                        ES_TO_AARCH32);
        else
            armv8_switch_to_el2((u64)images->ft_addr, 0, 0, es_flag,
                        images->ep,
                        ES_TO_AARCH64);
#endif
    }
#else
    unsigned long machid = gd->bd->bi_arch_number;
    char *s;
    void (*kernel_entry)(int zero, int arch, uint params);
    unsigned long r2;
    int fake = (flag & BOOTM_STATE_OS_FAKE_GO);

    kernel_entry = (void (*)(int, int, uint))images->ep;
#ifdef CONFIG_CPU_V7M
    ulong addr = (ulong)kernel_entry | 1;
    kernel_entry = (void *)addr;
#endif
    s = env_get("machid");
    if (s) {
        if (strict_strtoul(s, 16, &machid) < 0) {
            debug("strict_strtoul failed!\n");
            return;
        }
        printf("Using machid 0x%lx from environment\n", machid);
    }

    debug("## Transferring control to Linux (at address %08lx)" \
        "...\n", (ulong) kernel_entry);
    bootstage_mark(BOOTSTAGE_ID_RUN_OS);
    announce_and_cleanup(images, fake);

    if (IMAGE_ENABLE_OF_LIBFDT && images->ft_len)
        r2 = (unsigned long)images->ft_addr;
    else
        r2 = gd->bd->bi_boot_params;

    if (!fake) {
#ifdef CONFIG_ARMV7_NONSEC
        if (armv7_boot_nonsec()) {
            armv7_init_nonsec();
            secure_ram_addr(_do_nonsec_entry)(kernel_entry,
                              0, machid, r2);
        } else
#endif
            kernel_entry(0, machid, r2);
    }
#endif
}

/* Main Entry point for arm bootm implementation
 *
 * Modeled after the powerpc implementation
 * DIFFERENCE: Instead of calling prep and go at the end
 * they are called if subcommand is equal 0.
 */
int do_bootm_linux(int flag, int argc, char * const argv[],
           bootm_headers_t *images)
{
    /* No need for those on ARM */
    if (flag & BOOTM_STATE_OS_BD_T || flag & BOOTM_STATE_OS_CMDLINE)
        return -1;

    if (flag & BOOTM_STATE_OS_PREP) {
        boot_prep_linux(images);
        return 0;
    }

    if (flag & (BOOTM_STATE_OS_GO | BOOTM_STATE_OS_FAKE_GO)) {
        boot_jump_linux(images, flag);
        return 0;
    }

    boot_prep_linux(images);
    boot_jump_linux(images, flag);
    return 0;
}

#if defined(CONFIG_BOOTM_VXWORKS)
void boot_prep_vxworks(bootm_headers_t *images)
{
#if defined(CONFIG_OF_LIBFDT)
    int off;

    if (images->ft_addr) {
        off = fdt_path_offset(images->ft_addr, "/memory");
        if (off < 0) {
            if (arch_fixup_fdt(images->ft_addr))
                puts("## WARNING: fixup memory failed!\n");
        }
    }
#endif
    cleanup_before_linux();
}
void boot_jump_vxworks(bootm_headers_t *images)
{
    /* ARM VxWorks requires device tree physical address to be passed */
    ((void (*)(void *))images->ep)(images->ft_addr);
}
#endif

// By GST @Date