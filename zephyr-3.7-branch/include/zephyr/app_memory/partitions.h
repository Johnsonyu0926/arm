// zephyr-3.7-branch/include/zephyr/app_memory/partitions.h

#ifndef ZEPHYR_APP_MEMORY_PARTITIONS_H
#define ZEPHYR_APP_MEMORY_PARTITIONS_H

#ifdef CONFIG_USERSPACE
#include <zephyr/kernel.h> /* For struct k_mem_partition */

#if defined(CONFIG_MBEDTLS)
/**
 * @brief Memory partition for mbedTLS
 *
 * This partition is used to define the memory region for mbedTLS.
 */
extern struct k_mem_partition k_mbedtls_partition;
#endif /* CONFIG_MBEDTLS */

#endif /* CONFIG_USERSPACE */

#endif /* ZEPHYR_APP_MEMORY_PARTITIONS_H */
//GST