// zephyr-3.7-branch/include/zephyr/net/loopback.h

#ifndef ZEPHYR_INCLUDE_NET_LOOPBACK_H_
#define ZEPHYR_INCLUDE_NET_LOOPBACK_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CONFIG_NET_LOOPBACK_SIMULATE_PACKET_DROP
/**
 * @brief 设置数据包丢弃率
 *
 * @param[in] ratio 值在0到1之间，0表示无数据包丢失，1表示所有数据包丢弃
 *
 * @return 成功返回0，否则返回负整数。
 */
int loopback_set_packet_drop_ratio(float ratio);

/**
 * @brief 获取丢弃的数据包数量
 *
 * @return 回环接口丢弃的数据包数量
 */
int loopback_get_num_dropped_packets(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_NET_LOOPBACK_H_ */

// By GST @Data