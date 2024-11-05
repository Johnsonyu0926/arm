// Filename: arm_mpu.c
// 评分: 92

#include <zephyr/device.h>
#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <kernel_arch_func.h>
#include <zephyr/arch/arm64/mm.h>
#include <zephyr/linker/linker-defs.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/check.h>
#include <zephyr/sys/barrier.h>
#include <zephyr/cache.h>
#include <kernel_internal.h>
#include <zephyr/mem_mgmt/mem_attr.h>
#include <zephyr/dt-bindings/memory-attr/memory-attr-arm.h>

LOG_MODULE_REGISTER(mpu, CONFIG_MPU_LOG_LEVEL);

#define NODE_HAS_PROP_AND_OR(node_id, prop) \
    DT_NODE_HAS_PROP(node_id, prop) ||

BUILD_ASSERT((DT_FOREACH_STATUS_OKAY_NODE_VARGS(
          NODE_HAS_PROP_AND_OR, zephyr_memory_region_mpu) false) == false,
          "`zephyr,memory-region-mpu` was deprecated in favor of `zephyr,memory-attr`");

#define MPU_DYNAMIC_REGION_AREAS_NUM 3

#if defined(CONFIG_USERSPACE) || defined(CONFIG_ARM64_STACK_PROTECTION)
static struct dynamic_region_info
          sys_dyn_regions[CONFIG_MP_MAX_NUM_CPUS][MPU_DYNAMIC_REGION_AREAS_NUM];
static int sys_dyn_regions_num[CONFIG_MP_MAX_NUM_CPUS];

static void dynamic_regions_init(void);
static int dynamic_areas_init(uintptr_t start, size_t size);
static int flush_dynamic_regions_to_mpu(struct dynamic_region_info *dyn_regions,
                                        uint8_t region_num);

#if defined(CONFIG_USERSPACE)
#define MPU_DYNAMIC_REGIONS_AREA_START ((uintptr_t)&_app_smem_start)
#else
#define MPU_DYNAMIC_REGIONS_AREA_START ((uintptr_t)&__kernel_ram_start)
#endif

#define MPU_DYNAMIC_REGIONS_AREA_SIZE  ((size_t)((uintptr_t)&__kernel_ram_end - \
    MPU_DYNAMIC_REGIONS_AREA_START))
#endif

#define ID_AA64MMFR0_MSA_msk (0xFFUL << 48U)
#define ID_AA64MMFR0_PMSA_EN (0x1FUL << 48U)
#define ID_AA64MMFR0_PMSA_VMSA_EN (0x2FUL << 48U)

static uint8_t static_regions_num;

static ALWAYS_INLINE uint8_t get_num_regions(void) {
    uint64_t type = read_mpuir_el1() & MPU_IR_REGION_Msk;
    return (uint8_t)type;
}

FUNC_NO_STACK_PROTECTOR void arm_core_mpu_enable(void) {
    uint64_t val = read_sctlr_el1();
    val |= SCTLR_M_BIT;
    write_sctlr_el1(val);
    barrier_dsync_fence_full();
    barrier_isync_fence_full();
}

void arm_core_mpu_disable(void) {
    uint64_t val = read_sctlr_el1();
    val &= ~SCTLR_M_BIT;
    write_sctlr_el1(val);
    barrier_dsync_fence_full();
    barrier_isync_fence_full();
}

static void mpu_init(void) {
    uint64_t mair = MPU_MAIR_ATTRS;
    write_mair_el1(mair);
    barrier_dsync_fence_full();
    barrier_isync_fence_full();
}

static ALWAYS_INLINE void mpu_set_region(uint32_t rnr, uint64_t rbar, uint64_t rlar) {
    write_prselr_el1(rnr);
    barrier_dsync_fence_full();
    write_prbar_el1(rbar);
    write_prlar_el1(rlar);
    barrier_dsync_fence_full();
    barrier_isync_fence_full();
}

static ALWAYS_INLINE void mpu_clr_region(uint32_t rnr) {
    write_prselr_el1(rnr);
    barrier_dsync_fence_full();
    write_prlar_el1(0);
    write_prbar_el1(0);
    barrier_dsync_fence_full();
    barrier_isync_fence_full();
}

static ALWAYS_INLINE void region_init(const uint32_t index, const struct arm_mpu_region *region_conf) {
    uint64_t rbar = region_conf->base & MPU_RBAR_BASE_Msk;
    uint64_t rlar = (region_conf->limit - 1) & MPU_RLAR_LIMIT_Msk;

    rbar |= region_conf->attr.rbar & (MPU_RBAR_XN_Msk | MPU_RBAR_AP_Msk | MPU_RBAR_SH_Msk);
    rlar |= (region_conf->attr.mair_idx << MPU_RLAR_AttrIndx_Pos) & MPU_RLAR_AttrIndx_Msk;
    rlar |= MPU_RLAR_EN_Msk;

    mpu_set_region(index, rbar, rlar);
}

#define _BUILD_REGION_CONF(reg, _ATTR) \
    (struct arm_mpu_region) { .name  = (reg).dt_name, \
                              .base  = (reg).dt_addr, \
                              .limit = (reg).dt_addr + (reg).dt_size, \
                              .attr  = _ATTR, \
                            }

static int mpu_configure_regions_from_dt(uint8_t *reg_index) {
    const struct mem_attr_region_t *region;
    size_t num_regions = mem_attr_get_regions(&region);

    for (size_t idx = 0; idx < num_regions; idx++) {
        struct arm_mpu_region region_conf;

        switch (DT_MEM_ARM_GET(region[idx].dt_attr)) {
        case DT_MEM_ARM_MPU_RAM:
            region_conf = _BUILD_REGION_CONF(region[idx], REGION_RAM_ATTR);
            break;
#ifdef REGION_RAM_NOCACHE_ATTR
        case DT_MEM_ARM_MPU_RAM_NOCACHE:
            region_conf = _BUILD_REGION_CONF(region[idx], REGION_RAM_NOCACHE_ATTR);
            __ASSERT(!(region[idx].dt_attr & DT_MEM_CACHEABLE),
                     "RAM_NOCACHE with DT_MEM_CACHEABLE attribute\n");
            break;
#endif
#ifdef REGION_FLASH_ATTR
        case DT_MEM_ARM_MPU_FLASH:
            region_conf = _BUILD_REGION_CONF(region[idx], REGION_FLASH_ATTR);
            break;
#endif
#ifdef REGION_IO_ATTR
        case DT_MEM_ARM_MPU_IO:
            region_conf = _BUILD_REGION_CONF(region[idx], REGION_IO_ATTR);
            break;
#endif
        default:
            LOG_ERR("Invalid attribute for the region\n");
            return -EINVAL;
        }

        region_init((*reg_index), (const struct arm_mpu_region *) &region_conf);
        (*reg_index)++;
    }

    return 0;
}

FUNC_NO_STACK_PROTECTOR void z_arm64_mm_init(bool is_primary_core) {
    uint64_t val;
    uint32_t r_index;
    uint8_t tmp_static_num;

    val = read_currentel();
    __ASSERT(GET_EL(val) == MODE_EL1, "Exception level not EL1, MPU not enabled!\n");

    val = read_id_aa64mmfr0_el1() & ID_AA64MMFR0_MSA_msk;
    if ((val != ID_AA64MMFR0_PMSA_EN) && (val != ID_AA64MMFR0_PMSA_VMSA_EN)) {
        __ASSERT(0, "MPU not supported!\n");
        return;
    }

    if (mpu_config.num_regions > get_num_regions()) {
        __ASSERT(0, "Request to configure: %u regions (supported: %u)\n",
                 mpu_config.num_regions, get_num_regions());
        return;
    }

    arm_core_mpu_disable();
    mpu_init();

    for (r_index = 0U; r_index < mpu_config.num_regions; r_index++) {
        region_init(r_index, &mpu_config.mpu_regions[r_index]);
    }

    tmp_static_num = mpu_config.num_regions;

    if (mpu_configure_regions_from_dt(&tmp_static_num) == -EINVAL) {
        __ASSERT(0, "Failed to allocate MPU regions from DT\n");
        return;
    }

    arm_core_mpu_enable();

    if (!is_primary_core) {
        goto out;
    }

    static_regions_num = tmp_static_num;

#if defined(CONFIG_USERSPACE) || defined(CONFIG_ARM64_STACK_PROTECTION)
    dynamic_regions_init();
    int rc = dynamic_areas_init(MPU_DYNAMIC_REGIONS_AREA_START, MPU_DYNAMIC_REGIONS_AREA_SIZE);
    if (rc < 0) {
        __ASSERT(0, "Dynamic areas init fail");
        return;
    }
#endif

out:
#if defined(CONFIG_ARM64_STACK_PROTECTION)
    (void)flush_dynamic_regions_to_mpu(sys_dyn_regions[arch_curr_cpu()->id],
                                       sys_dyn_regions_num[arch_curr_cpu()->id]);
#endif
    return;
}

// By GST @Date