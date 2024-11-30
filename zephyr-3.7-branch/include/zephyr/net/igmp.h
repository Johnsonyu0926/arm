// zephyr-3.7-branch/include/zephyr/net/igmp.h

/** @file
 * @brief IGMP API
 */

#ifndef ZEPHYR_INCLUDE_NET_IGMP_H_
#define ZEPHYR_INCLUDE_NET_IGMP_H_

/**
 * @brief IGMP (Internet Group Management Protocol)
 * @defgroup igmp IGMP API
 * @ingroup networking
 * @{
 */

#include <zephyr/types.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_ip.h>

#ifdef __cplusplus
extern "C" {
#endif

/** IGMP参数 */
struct igmp_param {
	struct in_addr *source_list; /**< 要包含或排除的源列表 */
	size_t sources_len;          /**< 源列表的长度 */
	bool include;                /**< 源列表过滤类型 */
};

/**
 * @brief 加入给定的多播组。
 *
 * @param iface 发送加入消息的网络接口
 * @param addr 要加入的多播组
 * @param param 可选参数
 *
 * @return 如果加入成功则返回0，否则返回<0。
 */
#if defined(CONFIG_NET_IPV4_IGMP)
int net_ipv4_igmp_join(struct net_if *iface, const struct in_addr *addr,
		       const struct igmp_param *param);
#else
static inline int net_ipv4_igmp_join(struct net_if *iface, const struct in_addr *addr,
				     const struct igmp_param *param)
{
	ARG_UNUSED(iface);
	ARG_UNUSED(addr);
	ARG_UNUSED(param);

	return -ENOSYS;
}
#endif

/**
 * @brief 离开给定的多播组。
 *
 * @param iface 发送离开消息的网络接口
 * @param addr 要离开的多播组
 *
 * @return 如果离开成功则返回0，否则返回<0。
 */
#if defined(CONFIG_NET_IPV4_IGMP)
int net_ipv4_igmp_leave(struct net_if *iface, const struct in_addr *addr);
#else
static inline int net_ipv4_igmp_leave(struct net_if *iface,
				      const struct in_addr *addr)
{
	ARG_UNUSED(iface);
	ARG_UNUSED(addr);

	return -ENOSYS;
}
#endif

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* ZEPHYR_INCLUDE_NET_IGMP_H_ */

// By GST @Data