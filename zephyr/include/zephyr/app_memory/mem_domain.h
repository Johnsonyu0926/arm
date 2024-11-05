// Filename: mem_domain.h
#ifndef ZEPHYR_INCLUDE_APP_MEMORY_MEM_DOMAIN_H_
#define ZEPHYR_INCLUDE_APP_MEMORY_MEM_DOMAIN_H_

#include <stdint.h>
#include <stddef.h>
#include <zephyr/sys/dlist.h>
#include <zephyr/toolchain.h>
#include <zephyr/kernel/thread.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CONFIG_USERSPACE

#ifdef _ARCH_MEM_PARTITION_ALIGN_CHECK
#define K_MEM_PARTITION_DEFINE(name, start, size, attr) \
	_ARCH_MEM_PARTITION_ALIGN_CHECK(start, size); \
	struct k_mem_partition name = { (uintptr_t)start, size, attr }
#else
#define K_MEM_PARTITION_DEFINE(name, start, size, attr) \
	struct k_mem_partition name = { (uintptr_t)start, size, attr }
#endif

struct k_mem_partition {
	uintptr_t start;
	size_t size;
	k_mem_partition_attr_t attr;
};

struct k_mem_domain {
#ifdef CONFIG_ARCH_MEM_DOMAIN_DATA
	struct arch_mem_domain arch;
#endif
	struct k_mem_partition partitions[CONFIG_MAX_DOMAIN_PARTITIONS];
	sys_dlist_t mem_domain_q;
	uint8_t num_partitions;
};

extern struct k_mem_domain k_mem_domain_default;
#else
struct k_mem_domain;
struct k_mem_partition;
#endif

int k_mem_domain_init(struct k_mem_domain *domain, uint8_t num_parts,
		      struct k_mem_partition *parts[]);
int k_mem_domain_add_partition(struct k_mem_domain *domain,
			       struct k_mem_partition *part);
int k_mem_domain_remove_partition(struct k_mem_domain *domain,
				  struct k_mem_partition *part);
int k_mem_domain_add_thread(struct k_mem_domain *domain, k_tid_t thread);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_APP_MEMORY_MEM_DOMAIN_H_ */

// By GST @Date