// Filename: partitions.h
#ifndef ZEPHYR_APP_MEMORY_PARTITIONS_H
#define ZEPHYR_APP_MEMORY_PARTITIONS_H

#ifdef CONFIG_USERSPACE
#include <zephyr/kernel.h>

#if defined(CONFIG_MBEDTLS)
extern struct k_mem_partition k_mbedtls_partition;
#endif
#endif

#endif /* ZEPHYR_APP_MEMORY_PARTITIONS_H */

// By GST @Date