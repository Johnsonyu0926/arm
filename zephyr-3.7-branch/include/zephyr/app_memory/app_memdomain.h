// zephyr-3.7-branch/include/zephyr/app_memory/app_memdomain.h

#ifndef ZEPHYR_INCLUDE_APP_MEMORY_APP_MEMDOMAIN_H_
#define ZEPHYR_INCLUDE_APP_MEMORY_APP_MEMDOMAIN_H_

#include <zephyr/linker/linker-defs.h>
#include <zephyr/sys/dlist.h>
#include <zephyr/kernel.h>

/**
 * @brief 应用程序内存域API
 * @defgroup mem_domain_apis_app 应用程序内存域API
 * @ingroup mem_domain_apis
 * @{
 */

#ifdef CONFIG_USERSPACE

/**
 * @brief 特定分区的数据段名称
 *
 * 用于定义内存池或任何其他以段名称作为参数的宏。
 *
 * @param id 分区名称
 */
#define K_APP_DMEM_SECTION(id) data_smem_##id##_data

/**
 * @brief 特定分区的bss段名称
 *
 * 用于定义内存池或任何其他以段名称作为参数的宏。
 *
 * @param id 分区名称
 */
#define K_APP_BMEM_SECTION(id) data_smem_##id##_bss

/**
 * @brief 将数据放置在分区的数据段中
 *
 * 使用此标记的全局变量将最终位于指定内存分区的数据段中。此数据应初始化为某个期望值。
 *
 * @param id 要关联此数据的内存分区名称
 */
#define K_APP_DMEM(id) Z_GENERIC_SECTION(K_APP_DMEM_SECTION(id))

/**
 * @brief 将数据放置在分区的bss段中
 *
 * 使用此标记的全局变量将最终位于指定内存分区的bss段中。此数据将在启动时清零。
 *
 * @param id 要关联此数据的内存分区名称
 */
#define K_APP_BMEM(id) Z_GENERIC_SECTION(K_APP_BMEM_SECTION(id))

struct z_app_region {
	void *bss_start;
	size_t bss_size;
};

#define Z_APP_START(id) z_data_smem_##id##_part_start
#define Z_APP_SIZE(id) z_data_smem_##id##_part_size
#define Z_APP_BSS_START(id) z_data_smem_##id##_bss_start
#define Z_APP_BSS_SIZE(id) z_data_smem_##id##_bss_size

/* 如果使用K_APPMEM_PARTITION声明了一个分区，但从未将任何数据分配给其内容，
 * 则其前缀的符号将不会出现在符号表中。这会阻止gen_app_partitions.py检测到分区的存在，
 * 并且不会生成指定分区边界的链接器符号，从而导致构建错误。
 *
 * 此内联汇编代码所做的是定义一个没有数据的符号。
 * 这应该适用于所有生成ELF二进制文件的架构，参见
 * https://sourceware.org/binutils/docs/as/Section.html
 *
 * 我们不知道推送段的活动标志/类型是什么，所以我们是具体的："aw"表示段是可分配和可写的，
 * "@progbits"表示段有数据。
 */
#if defined(CONFIG_ARM) || defined(CONFIG_ARM64)
/* ARM有一个怪癖，即'@'表示注释，所以我们必须将%progbits发送给汇编器。 */
#define Z_PROGBITS_SYM	"%"
#else
#define Z_PROGBITS_SYM "@"
#endif

#if defined(CONFIG_ARC) && defined(__CCAC__)
/* ARC MWDT汇编器具有略有不同的pushsection/popsection指令名称。 */
#define Z_PUSHSECTION_DIRECTIV		".pushsect"
#define Z_POPSECTION_DIRECTIVE		".popsect"
#else
#define Z_PUSHSECTION_DIRECTIV		".pushsection"
#define Z_POPSECTION_DIRECTIVE		".popsection"
#endif

#define Z_APPMEM_PLACEHOLDER(name) \
	__asm__ ( \
		Z_PUSHSECTION_DIRECTIV " " STRINGIFY(K_APP_DMEM_SECTION(name)) \
			",\"aw\"," Z_PROGBITS_SYM "progbits\n\t" \
		".global " STRINGIFY(name) "_placeholder\n\t" \
		STRINGIFY(name) "_placeholder:\n\t" \
		Z_POPSECTION_DIRECTIVE "\n\t")

/**
 * @brief 使用链接器支持定义应用程序内存分区
 *
 * 使用提供的名称定义k_mem_partition。
 * 此名称可与K_APP_DMEM和K_APP_BMEM宏一起使用，以自动将全局变量放置在此分区中。
 *
 * 注意：此处定义了占位符字符变量，以防止如果定义了分区但从未在其中放置任何内容时出现构建错误。
 *
 * @param name 要声明的k_mem_partition的名称
 */
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

/**
 * @}
 */

#endif /* ZEPHYR_INCLUDE_APP_MEMORY_APP_MEMDOMAIN_H_ */

// By GST @Data