// Filename: app_memdomain.h
#ifndef ZEPHYR_INCLUDE_APP_MEMORY_APP_MEMDOMAIN_H_
#define ZEPHYR_INCLUDE_APP_MEMORY_APP_MEMDOMAIN_H_

#include <zephyr/linker/linker-defs.h>
#include <zephyr/sys/dlist.h>
#include <zephyr/kernel.h>

#ifdef CONFIG_USERSPACE

#define K_APP_DMEM_SECTION(id) data_smem_##id##_data
#define K_APP_BMEM_SECTION(id) data_smem_##id##_bss
#define K_APP_DMEM(id) Z_GENERIC_SECTION(K_APP_DMEM_SECTION(id))
#define K_APP_BMEM(id) Z_GENERIC_SECTION(K_APP_BMEM_SECTION(id))

struct z_app_region {
	void *bss_start;
	size_t bss_size;
};

#define Z_APP_START(id) z_data_smem_##id##_part_start
#define Z_APP_SIZE(id) z_data_smem_##id##_part_size
#define Z_APP_BSS_START(id) z_data_smem_##id##_bss_start
#define Z_APP_BSS_SIZE(id) z_data_smem_##id##_bss_size

#if defined(CONFIG_ARM) || defined(CONFIG_ARM64)
#define Z_PROGBITS_SYM "%progbits"
#else
#define Z_PROGBITS_SYM "@progbits"
#endif

#if defined(CONFIG_ARC) && defined(__CCAC__)
#define Z_PUSHSECTION_DIRECTIV ".pushsect"
#define Z_POPSECTION_DIRECTIVE ".popsect"
#else
#define Z_PUSHSECTION_DIRECTIV ".pushsection"
#define Z_POPSECTION_DIRECTIVE ".popsection"
#endif

#define Z_APPMEM_PLACEHOLDER(name) \
	__asm__ ( \
		Z_PUSHSECTION_DIRECTIV " " STRINGIFY(K_APP_DMEM_SECTION(name)) \
			",\"aw\"," Z_PROGBITS_SYM "\n\t" \
		".global " STRINGIFY(name) "_placeholder\n\t" \
		STRINGIFY(name) "_placeholder:\n\t" \
		Z_POPSECTION_DIRECTIVE "\n\t")

#define K_APPMEM_PARTITION_DEFINE(name) \
	extern char Z_APP_START(name)[]; \
	extern char Z_APP_SIZE(name)[]; \
	struct k_mem_partition name = { \
		.start = (uintptr_t) &Z_APP_START(name)[0], \
		.size = (size_t) &Z_APP_SIZE(name)[0], \
		.attr = K_MEM_PARTITION_P_RW_U_RW \
	}; \
	extern char Z_APP_BSS_START(name)[]; \
	extern char Z_APP_BSS_SIZE(name)[]; \
	Z_GENERIC_SECTION(.app_regions.name) \
	const struct z_app_region name##_region = { \
		.bss_start = &Z_APP_BSS_START(name)[0], \
		.bss_size = (size_t) &Z_APP_BSS_SIZE(name)[0] \
	}; \
	Z_APPMEM_PLACEHOLDER(name)
#else

#define K_APP_BMEM(ptn)
#define K_APP_DMEM(ptn)
#define K_APP_DMEM_SECTION(ptn) .data
#define K_APP_BMEM_SECTION(ptn) .bss
#define K_APPMEM_PARTITION_DEFINE(name)

#endif /* CONFIG_USERSPACE */

#endif /* ZEPHYR_INCLUDE_APP_MEMORY_APP_MEMDOMAIN_H_ */

// By GST @Date