// Filename: cpu.h

#ifndef CPU_H
#define CPU_H

#include <stdint.h>

/**
 * @brief 获取核心对应的集群
 * @param core 核心编号
 * @return 返回核心对应的集群编号
 */
int fsl_qoriq_core_to_cluster(unsigned int core);

/**
 * @brief 获取发起者类型
 * @param cluster 集群编号
 * @param init_id 发起者ID
 * @return 返回发起者类型
 */
uint32_t initiator_type(uint32_t cluster, int init_id);

/**
 * @brief 获取CPU掩码
 * @return 返回CPU掩码
 */
uint32_t cpu_mask(void);

#endif // CPU_H

// By GST @Date