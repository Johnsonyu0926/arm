// zephyr-3.7-branch/include/zephyr/net/ethernet_bridge.h

#ifndef ZEPHYR_INCLUDE_NET_ETHERNET_BRIDGE_H_
#define ZEPHYR_INCLUDE_NET_ETHERNET_BRIDGE_H_

#include <zephyr/sys/slist.h>
#include <zephyr/sys/iterable_sections.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Ethernet Bridging API
 * @defgroup eth_bridge Ethernet Bridging API
 * @ingroup networking
 * @{
 */

/** @cond INTERNAL_HIDDEN */

struct eth_bridge {
	struct k_mutex lock;
	sys_slist_t interfaces;
	sys_slist_t listeners;
	bool initialized;
};

#define ETH_BRIDGE_INITIALIZER(obj) \
	{ \
		.lock		= { }, \
		.interfaces	= SYS_SLIST_STATIC_INIT(&obj.interfaces), \
		.listeners	= SYS_SLIST_STATIC_INIT(&obj.listeners), \
	}

/** @endcond */

/**
 * @brief Statically定义并初始化桥实例。
 *
 * @param name 桥对象的名称
 */
#define ETH_BRIDGE_INIT(name) \
	STRUCT_SECTION_ITERABLE(eth_bridge, name) = \
		ETH_BRIDGE_INITIALIZER(name)

/** @cond INTERNAL_HIDDEN */

struct eth_bridge_iface_context {
	sys_snode_t node;
	struct eth_bridge *instance;
	bool allow_tx;
};

struct eth_bridge_listener {
	sys_snode_t node;
	struct k_fifo pkt_queue;
};

/** @endcond */

/**
 * @brief 将以太网网络接口添加到桥
 *
 * 这会将网络接口添加到桥。然后将接口置于混杂模式，所有通过此接口接收的数据包都将发送到桥，并且发送到桥的任何其他数据包（有一些例外）将通过此接口传输。
 *
 * 要使桥通过此接口进行传输，必须使用 eth_bridge_iface_tx() 启用 TX 模式。 TX 模式最初是禁用的。
 *
 * 一旦将接口添加到桥，所有传入流量都将转移到桥。然而，通过此接口发送出去的数据包不会受到桥的影响。
 *
 * @param br 指向已初始化桥对象的指针
 * @param iface 要添加的接口
 *
 * @return 0 表示成功，否则为负错误代码。
 */
int eth_bridge_iface_add(struct eth_bridge *br, struct net_if *iface);

/**
 * @brief 从桥中删除以太网网络接口
 *
 * @param br 指向已初始化桥对象的指针
 * @param iface 要删除的接口
 *
 * @return 0 表示成功，否则为负错误代码。
 */
int eth_bridge_iface_remove(struct eth_bridge *br, struct net_if *iface);

/**
 * @brief 启用/禁用桥接接口的传输模式
 *
 * 当 TX 模式关闭时，接口可以接收数据包并将其发送到桥，但不会通过此接口发送来自桥的数据包。当 TX 模式打开时，允许传入和传出数据包。
 *
 * @param iface 要配置的接口
 * @param allow true 表示激活 TX 模式，否则为 false
 *
 * @return 0 表示成功，否则为负错误代码。
 */
int eth_bridge_iface_allow_tx(struct net_if *iface, bool allow);

/**
 * @brief 向桥添加（注册）监听器
 *
 * 这允许软件监听器注册指向提供的 FIFO 的指针以接收发送到桥的数据包。监听器负责使用 k_fifo_get() 清空 FIFO，这将返回 struct net_pkt 指针，并在完成后使用 net_pkt_unref() 释放数据包。
 *
 * 希望不再接收任何数据包的监听器应简单地使用 eth_bridge_listener_remove() 注销自己。
 *
 * @param br 指向已初始化桥对象的指针
 * @param l 指向已初始化监听器实例的指针。
 *
 * @return 0 表示成功，否则为负错误代码。
 */
int eth_bridge_listener_add(struct eth_bridge *br, struct eth_bridge_listener *l);

/**
 * @brief 从桥中删除（注销）监听器
 *
 * @param br 指向已初始化桥对象的指针
 * @param l 要删除的监听器实例的指针。
 *
 * @return 0 表示成功，否则为负错误代码。
 */
int eth_bridge_listener_remove(struct eth_bridge *br, struct eth_bridge_listener *l);

/**
 * @brief 根据指针获取桥索引
 *
 * @param br 指向桥实例的指针
 *
 * @return 桥索引
 */
int eth_bridge_get_index(struct eth_bridge *br);

/**
 * @brief 根据索引获取桥实例
 *
 * @param index 桥实例索引
 *
 * @return 指向桥实例的指针，如果未找到则返回 NULL。
 */
struct eth_bridge *eth_bridge_get_by_index(int index);

/**
 * @typedef eth_bridge_cb_t
 * @brief 在遍历桥实例时使用的回调
 *
 * @param br 指向桥实例的指针
 * @param user_data 用户提供的数据
 */
typedef void (*eth_bridge_cb_t)(struct eth_bridge *br, void *user_data);

/**
 * @brief 遍历所有桥实例以获取有关它们的信息。这主要在 net-shell 中用于打印有关当前活动桥的数据。
 *
 * @param cb 要为每个桥实例调用的回调
 * @param user_data 用户提供的数据
 */
void net_eth_bridge_foreach(eth_bridge_cb_t cb, void *user_data);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_NET_ETHERNET_BRIDGE_H_ */
//GST