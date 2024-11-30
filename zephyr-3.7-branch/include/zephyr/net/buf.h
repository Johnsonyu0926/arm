//zephyr-3.7-branch/include/zephyr/net/buf.h


#ifndef ZEPHYR_INCLUDE_NET_BUF_H_
#define ZEPHYR_INCLUDE_NET_BUF_H_

#include <stddef.h>
#include <zephyr/types.h>
#include <zephyr/sys/util.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/iterable_sections.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 网络缓冲区库
 * @defgroup net_buf 网络缓冲区库
 * @ingroup networking
 * @{
 */

/* 缓冲区定义的各个部分所需的对齐 */
#if CONFIG_NET_BUF_ALIGNMENT == 0
#define __net_buf_align __aligned(sizeof(void *))
#else
#define __net_buf_align __aligned(CONFIG_NET_BUF_ALIGNMENT)
#endif

/**
 *  @brief 定义一个net_buf_simple栈变量。
 *
 *  这是一个用于在栈上定义net_buf_simple对象的辅助宏。
 *
 *  @param _name net_buf_simple对象的名称。
 *  @param _size 缓冲区的最大数据存储。
 */
#define NET_BUF_SIMPLE_DEFINE(_name, _size)     \
	uint8_t net_buf_data_##_name[_size];       \
	struct net_buf_simple _name = {         \
		.data   = net_buf_data_##_name, \
		.len    = 0,                    \
		.size   = _size,                \
		.__buf  = net_buf_data_##_name, \
	}

/**
 *
 * @brief 定义一个静态net_buf_simple变量。
 *
 * 这是一个用于定义静态net_buf_simple对象的辅助宏。
 *
 * @param _name net_buf_simple对象的名称。
 * @param _size 缓冲区的最大数据存储。
 */
#define NET_BUF_SIMPLE_DEFINE_STATIC(_name, _size)        \
	static __noinit uint8_t net_buf_data_##_name[_size]; \
	static struct net_buf_simple _name = {            \
		.data   = net_buf_data_##_name,           \
		.len    = 0,                              \
		.size   = _size,                          \
		.__buf  = net_buf_data_##_name,           \
	}

/**
 * @brief 简单的网络缓冲区表示。
 *
 * 这是net_buf对象的简化版本（实际上net_buf在内部使用net_buf_simple）。它不提供任何类型的引用计数、用户数据、动态分配，或一般情况下无法通过内核对象（如FIFO）传递。
 *
 * 主要用途是当普通net_buf的元数据不需要且会导致过多开销的情况下。例如，当缓冲区只需要在栈上分配或缓冲区的访问和生命周期得到良好控制和约束时。
 */
struct net_buf_simple {
	/** 缓冲区中数据起始位置的指针。 */
	uint8_t *data;

	/**
	 * 数据指针后面的数据长度。
	 *
	 * 要确定最大长度，请使用net_buf_simple_max_len()，而不是#size！
	 */
	uint16_t len;

	/** net_buf_simple#__buf可以存储的数据量。 */
	uint16_t size;

	/** 数据存储的起始位置。不应直接访问（应使用数据指针）。 */
	uint8_t *__buf;
};

/**
 *
 * @brief 定义一个net_buf_simple栈变量并获取指向它的指针。
 *
 * 这是一个用于在栈上定义net_buf_simple对象并获取指向它的指针的辅助宏，如下所示：
 *
 * struct net_buf_simple *my_buf = NET_BUF_SIMPLE(10);
 *
 * 创建对象后，需要通过调用net_buf_simple_init()进行初始化。
 *
 * @param _size 缓冲区的最大数据存储。
 *
 * @return 指向栈分配的net_buf_simple对象的指针。
 */
#define NET_BUF_SIMPLE(_size)                        \
	((struct net_buf_simple *)(&(struct {        \
		struct net_buf_simple buf;           \
		uint8_t data[_size];                 \
	}) {                                         \
		.buf.size = _size,                   \
	}))

/**
 * @brief 初始化net_buf_simple对象。
 *
 * 在使用NET_BUF_SIMPLE宏创建net_buf_simple对象后需要调用此函数。
 *
 * @param buf 要初始化的缓冲区。
 * @param reserve_head 要保留的头部空间。
 */
static inline void net_buf_simple_init(struct net_buf_simple *buf,
				       size_t reserve_head)
{
	if (!buf->__buf) {
		buf->__buf = (uint8_t *)buf + sizeof(*buf);
	}

	buf->data = buf->__buf + reserve_head;
	buf->len = 0U;
}

/**
 * @brief 使用数据初始化net_buf_simple对象。
 *
 * 使用外部数据初始化缓冲区对象。
 *
 * @param buf 要初始化的缓冲区。
 * @param data 外部数据指针
 * @param size 指向的数据缓冲区能够容纳的数据量。
 */
void net_buf_simple_init_with_data(struct net_buf_simple *buf,
				   void *data, size_t size);

/**
 * @brief 重置缓冲区
 *
 * 重置缓冲区数据，以便可以将其重新用于其他目的。
 *
 * @param buf 要重置的缓冲区。
 */
static inline void net_buf_simple_reset(struct net_buf_simple *buf)
{
	buf->len  = 0U;
	buf->data = buf->__buf;
}

/**
 * 克隆缓冲区状态，使用相同的数据缓冲区。
 *
 * 初始化缓冲区以指向与现有缓冲区相同的数据。允许对相同数据进行操作而不改变原始数据的长度和偏移量。
 *
 * @param original 要克隆的缓冲区。
 * @param clone 新的克隆。
 */
void net_buf_simple_clone(const struct net_buf_simple *original,
			  struct net_buf_simple *clone);

/**
 * @brief 准备在缓冲区末尾添加数据
 *
 * 增加缓冲区的数据长度以考虑在末尾添加更多数据。
 *
 * @param buf 要更新的缓冲区。
 * @param len 要增加的长度。
 *
 * @return 缓冲区的原始尾部。
 */
void *net_buf_simple_add(struct net_buf_simple *buf, size_t len);

/**
 * @brief 从内存中复制给定数量的字节到缓冲区末尾
 *
 * 增加缓冲区的数据长度以考虑在末尾添加更多数据。
 *
 * @param buf 要更新的缓冲区。
 * @param mem 要添加的数据的位置。
 * @param len 要添加的数据长度
 *
 * @return 缓冲区的原始尾部。
 */
void *net_buf_simple_add_mem(struct net_buf_simple *buf, const void *mem,
			     size_t len);

/**
 * @brief 在缓冲区末尾添加（8位）字节
 *
 * 增加缓冲区的数据长度以考虑在末尾添加更多数据。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加的字节值。
 *
 * @return 指向添加的值的指针
 */
uint8_t *net_buf_simple_add_u8(struct net_buf_simple *buf, uint8_t val);

/**
 * @brief 在缓冲区末尾添加16位值
 *
 * 以小端格式在缓冲区末尾添加16位值。增加缓冲区的数据长度以考虑在末尾添加更多数据。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加的16位值。
 */
void net_buf_simple_add_le16(struct net_buf_simple *buf, uint16_t val);

/**
 * @brief 在缓冲区末尾添加16位值
 *
 * 以大端格式在缓冲区末尾添加16位值。增加缓冲区的数据长度以考虑在末尾添加更多数据。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加的16位值。
 */
void net_buf_simple_add_be16(struct net_buf_simple *buf, uint16_t val);

/**
 * @brief 在缓冲区末尾添加24位值
 *
 * 以小端格式在缓冲区末尾添加24位值。增加缓冲区的数据长度以考虑在末尾添加更多数据。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加的24位值。
 */
void net_buf_simple_add_le24(struct net_buf_simple *buf, uint32_t val);

/**
 * @brief 在缓冲区末尾添加24位值
 *
 * 以大端格式在缓冲区末尾添加24位值。增加缓冲区的数据长度以考虑在末尾添加更多数据。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加的24位值。
 */
void net_buf_simple_add_be24(struct net_buf_simple *buf, uint32_t val);

/**
 * @brief 在缓冲区末尾添加32位值
 *
 * 以小端格式在缓冲区末尾添加32位值。增加缓冲区的数据长度以考虑在末尾添加更多数据。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加的32位值。
 */
void net_buf_simple_add_le32(struct net_buf_simple *buf, uint32_t val);

/**
 * @brief 在缓冲区末尾添加32位值
 *
 * 以大端格式在缓冲区末尾添加32位值。增加缓冲区的数据长度以考虑在末尾添加更多数据。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加的32位值。
 */
void net_buf_simple_add_be32(struct net_buf_simple *buf, uint32_t val);

/**
 * @brief 在缓冲区末尾添加40位值
 *
 * 以小端格式在缓冲区末尾添加40位值。增加缓冲区的数据长度以考虑在末尾添加更多数据。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加的40位值。
 */
void net_buf_simple_add_le40(struct net_buf_simple *buf, uint64_t val);

/**
 * @brief 在缓冲区末尾添加40位值
 *
 * 以大端格式在缓冲区末尾添加40位值。增加缓冲区的数据长度以考虑在末尾添加更多数据。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加的40位值。
 */
void net_buf_simple_add_be40(struct net_buf_simple *buf, uint64_t val);

/**
 * @brief 在缓冲区末尾添加48位值
 *
 * 以小端格式在缓冲区末尾添加48位值。增加缓冲区的数据长度以考虑在末尾添加更多数据。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加的48位值。
 */
void net_buf_simple_add_le48(struct net_buf_simple *buf, uint64_t val);

/**
 * @brief 在缓冲区末尾添加48位值
 *
 * 以大端格式在缓冲区末尾添加48位值。增加缓冲区的数据长度以考虑在末尾添加更多数据。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加的48位值。
 */
void net_buf_simple_add_be48(struct net_buf_simple *buf, uint64_t val);

/**
 * @brief 在缓冲区末尾添加64位值
 *
 * 以小端格式在缓冲区末尾添加64位值。增加缓冲区的数据长度以考虑在末尾添加更多数据。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加的64位值。
 */
void net_buf_simple_add_le64(struct net_buf_simple *buf, uint64_t val);

/**
 * @brief 在缓冲区末尾添加64位值
 *
 * 以大端格式在缓冲区末尾添加64位值。增加缓冲区的数据长度以考虑在末尾添加更多数据。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加的64位值。
 */
void net_buf_simple_add_be64(struct net_buf_simple *buf, uint64_t val);

/**
 * @brief 从缓冲区末尾移除数据。
 *
 * 通过修改缓冲区长度从缓冲区末尾移除数据。
 *
 * @param buf 要更新的缓冲区。
 * @param len 要移除的字节数。
 *
 * @return 缓冲区数据的新末尾。
 */
void *net_buf_simple_remove_mem(struct net_buf_simple *buf, size_t len);

/**
 * @brief 从缓冲区末尾移除8位值
 *
 * 与net_buf_simple_remove_mem()相同，但用于操作8位值的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 移除的8位值
 */
uint8_t net_buf_simple_remove_u8(struct net_buf_simple *buf);

/**
 * @brief 从缓冲区末尾移除并转换16位值。
 *
 * 与net_buf_simple_remove_mem()相同，但用于操作16位小端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从小端转换为主机端的16位值。
 */
uint16_t net_buf_simple_remove_le16(struct net_buf_simple *buf);

/**
 * @brief 从缓冲区末尾移除并转换16位值。
 *
 * 与net_buf_simple_remove_mem()相同，但用于操作16位大端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从大端转换为主机端的16位值。
 */
uint16_t net_buf_simple_remove_be16(struct net_buf_simple *buf);

/**
 * @brief 从缓冲区末尾移除并转换24位值。
 *
 * 与net_buf_simple_remove_mem()相同，但用于操作24位小端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从小端转换为主机端的24位值。
 */
uint32_t net_buf_simple_remove_le24(struct net_buf_simple *buf);

/**
 * @brief 从缓冲区末尾移除并转换24位值。
 *
 * 与net_buf_simple_remove_mem()相同，但用于操作24位大端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从大端转换为主机端的24位值。
 */
uint32_t net_buf_simple_remove_be24(struct net_buf_simple *buf);

/**
 * @brief 从缓冲区末尾移除并转换32位值。
 *
 * 与net_buf_simple_remove_mem()相同，但用于操作32位小端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从小端转换为主机端的32位值。
 */
uint32_t net_buf_simple_remove_le32(struct net_buf_simple *buf);

/**
 * @brief 从缓冲区末尾移除并转换32位值。
 *
 * 与net_buf_simple_remove_mem()相同，但用于操作32位大端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从大端转换为主机端的32位值。
 */
uint32_t net_buf_simple_remove_be32(struct net_buf_simple *buf);

/**
 * @brief 从缓冲区末尾移除并转换40位值。
 *
 * 与net_buf_simple_remove_mem()相同，但用于操作40位小端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从小端转换为主机端的40位值。
 */
uint64_t net_buf_simple_remove_le40(struct net_buf_simple *buf);

/**
 * @brief 从缓冲区末尾移除并转换40位值。
 *
 * 与net_buf_simple_remove_mem()相同，但用于操作40位大端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从大端转换为主机端的40位值。
 */
uint64_t net_buf_simple_remove_be40(struct net_buf_simple *buf);

/**
 * @brief 从缓冲区末尾移除并转换48位值。
 *
 * 与net_buf_simple_remove_mem()相同，但用于操作48位小端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从小端转换为主机端的48位值。
 */
uint64_t net_buf_simple_remove_le48(struct net_buf_simple *buf);

/**
 * @brief 从缓冲区末尾移除并转换48位值。
 *
 * 与net_buf_simple_remove_mem()相同，但用于操作48位大端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从大端转换为主机端的48位值。
 */
uint6464_t net_buf_simple_remove_be48(struct net_buf_simple *buf);

/**
 * @brief 从缓冲区末尾移除并转换64位值。
 *
 * 与net_buf_simple_remove_mem()相同，但用于操作64位小端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从小端转换为主机端的64位值。
 */
uint64_t net_buf_simple_remove_le64(struct net_buf_simple *buf);

/**
 * @brief 从缓冲区末尾移除并转换64位值。
 *
 * 与net_buf_simple_remove_mem()相同，但用于操作64位大端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从大端转换为主机端的64位值。
 */
uint64_t net_buf_simple_remove_be64(struct net_buf_simple *buf);

/**
 * @brief 准备在缓冲区开头添加数据
 *
 * 修改数据指针和缓冲区长度以考虑在缓冲区开头添加更多数据。
 *
 * @param buf 要更新的缓冲区。
 * @param len 要添加到开头的字节数。
 *
 * @return 缓冲区数据的新开头。
 */
void *net_buf_simple_push(struct net_buf_simple *buf, size_t len);

/**
 * @brief 从内存中复制给定数量的字节到缓冲区开头。
 *
 * 修改数据指针和缓冲区长度以考虑在缓冲区开头添加更多数据。
 *
 * @param buf 要更新的缓冲区。
 * @param mem 要添加的数据的位置。
 * @param len 要添加的数据长度。
 *
 * @return 缓冲区数据的新开头。
 */
void *net_buf_simple_push_mem(struct net_buf_simple *buf, const
 * void *mem, size_t len);

/**
 * @brief 在缓冲区开头添加8位值
 *
 * 在缓冲区开头添加8位值。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加到缓冲区的8位值。
 */
void net_buf_simple_push_u8(struct net_buf_simple *buf, uint8_t val);

/** * @brief 在缓冲区开头添加16位值
 *
 * 以小端格式在缓冲区开头添加16位值。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加到缓冲区的16位值。
 */
void net_buf_simple_push_le16(struct net_buf_simple *buf, uint16_t val);

/**
 * @brief 在缓冲区开头添加16位值
 *
 * 以大端格式在缓冲区开头添加16位值。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加到缓冲区的16位值。
 */
void net_buf_simple_push_be16(struct net_buf_simple *buf, uint16_t val);

/**
 * @brief 在缓冲区开头添加24位值
 *
 * 以小端格式在缓冲区开头添加24位值。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加到缓冲区的24位值。
 */
void net_buf_simple_push_le24(struct net_buf_simple *buf, uint32_t val);

/**
 * @brief 在缓冲区开头添加24位值
 *
 * 以大端格式在缓冲区开头添加24位值。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加到缓冲区的24位值。
 */
void net_buf_simple_push_be24(struct net_buf_simple *buf, uint32_t val);

/**
 * @brief 在缓冲区开头添加32位值
 *
 * 以小端格式在缓冲区开头添加32位值。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加到缓冲区的32位值。
 */
void net_buf_simple_push_le32(struct net_buf_simple *buf, uint32_t val);

/**
 * @brief 在缓冲区开头添加32位值
 *
 * 以大端格式在缓冲区开头添加32位值。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加到缓冲区的32位值。
 */
void net_buf_simple_push_be32(struct net_buf_simple *buf, uint32_t val);

/**
 * @brief 在缓冲区开头添加40位值
 *
 * 以小端格式在缓冲区开头添加40位值。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加到缓冲区的40位值。
 */
void net_buf_simple_push_le40(struct net_buf_simple *buf, uint64_t val);

/**
 * @brief 在缓冲区开头添加40位值
 *
 * 以大端格式在缓冲区开头添加40位值。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加到缓冲区的40位值。
 */
void net_buf_simple_push_be40(struct net_buf_simple *buf, uint64_t val);

/**
 * @brief 在缓冲区开头添加48位值
 *
 * 以小端格式在缓冲区开头添加48位值。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加到缓冲区的48位值。
 */
void net_buf_simple_push_le48(struct net_buf_simple *buf, uint64_t val);

/**
 * @brief 在缓冲区开头添加48位值
 *
 * 以大端格式在缓冲区开头添加48位值。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加到缓冲区的48位值。
 */
void net_buf_simple_push_be48(struct net_buf_simple *buf, uint64_t val);

/**
 * @brief 在缓冲区开头添加64位值
 *
 * 以小端格式在缓冲区开头添加64位值。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加到缓冲区的64位值。
 */
void net_buf_simple_push_le64(struct net_buf_simple *buf, uint64_t val);

/**
 * @brief 在缓冲区开头添加64位值
 *
 * 以大端格式在缓冲区开头添加64位值。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加到缓冲区的64位值。
 */
void net_buf_simple_push_be64(struct net_buf_simple *buf, uint64_t val);

/**
 * @brief 从缓冲区开头移除数据。
 *
 * 通过修改数据指针和缓冲区长度从缓冲区开头移除数据。
 *
 * @param buf 要更新的缓冲区。
 * @param len 要移除的字节数。
 *
 * @return 缓冲区数据的新开头。
 */
void *net_buf_simple_pull(struct net_buf_simple *buf, size_t len);

/**
 * @brief 从缓冲区开头移除数据。
 *
 * 通过修改数据指针和缓冲区长度从缓冲区开头移除数据。
 *
 * @param buf 要更新的缓冲区。
 * @param len 要移除的字节数。
 *
 * @return 指向缓冲区数据旧位置的指针。
 */
void *net_buf_simple_pull_mem(struct net_buf_simple *buf, size_t len);

/**
 * @brief 从缓冲区开头移除8位值
 *
 * 与net_buf_simple_pull()相同，但用于操作8位值的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 移除的8位值
 */
uint8_t net_buf_simple_pull_u8(struct net_buf_simple *buf);

/**
 * @brief 从缓冲区开头移除并转换16位值。
 *
 * 与net_buf_simple_pull()相同，但用于操作16位小端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从小端转换为主机端的16位值。
 */
uint16_t net_buf_simple_pull_le16(struct net_buf_simple *buf);

/**
 * @brief 从缓冲区开头移除并转换16位值。
 *
 * 与net_buf_simple_pull()相同，但用于操作16位大端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从大端转换为主机端的16位值。
 */
uint16_t net_buf_simple_pull_be16(struct net_buf_simple *buf);

/**
 * @brief 从缓冲区开头移除并转换24位值。
 *
 * 与net_buf_simple_pull()相同，但用于操作24位小端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从小端转换为主机端的24位值。
 */
uint32_t net_buf_simple_pull_le24(struct net_buf_simple *buf);

/**
 * @brief 从缓冲区开头移除并转换24位值。
 *
 * 与net_buf_simple_pull()相同，但用于操作24位大端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从大端转换为主机端的24位值。
 */
uint32_t net_buf_simple_pull_be24(struct net_buf_simple *buf);

/**
 * @brief 从缓冲区开头移除并转换32位值。
 *
 * 与net_buf_simple_pull()相同，但用于操作32位小端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从小端转换为主机端的32位值。
 */
uint32_t net_buf_simple_pull_le32(struct net_buf_simple *buf);

/**
 * @brief 从缓冲区开头移除并转换32位值。
 *
 * 与net_buf_simple_pull()相同，但用于操作32位大端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从大端转换为主机端的32位值。
 */
uint32_t net_buf_simple_pull_be32(struct net_buf_simple *buf);

/**
 * @brief 从缓冲区开头移除并转换40位值。
 *
 * 与net_buf_simple_pull()相同，但用于操作40位小端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从小端转换为主机端的40位值。
 */
uint64_t net_buf_simple_pull_le40(struct net_buf_simple *buf);

/**
 * @brief 从缓冲区开头移除并转换40位值。
 *
 * 与net_buf_simple_pull()相同，但用于操作40位大端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从大端转换为主机端的40位值。
 */
uint64_t net_buf_simple_pull_be40(struct net_buf_simple *buf);

/**
 * @brief 从缓冲区开头移除并转换48位值。
 *
 * 与net_buf_simple_pull()相同，但用于操作48位小端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从小端转换为主机端的48位值。
 */
uint64_t net_buf_simple_pull_le48(struct net_buf_simple *buf);

/**
 * @brief 从缓冲区开头移除并转换48位值。
 *
 * 与net_buf_simple_pull()相同，但用于操作48位大端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从大端转换为主机端的48位值。
 */
uint64_t net_buf_simple_pull_be48(struct net_buf_simple *buf);

/**
 * @brief 从缓冲区开头移除并转换64位值。
 *
 * 与net_buf_simple_pull()相同，但用于操作64位小端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从小端转换为主机端的64位值。
 */
uint64_t net_buf_simple_pull_le64(struct net_buf_simple *buf);

/**
 * @brief 从缓冲区开头移除并转换64位值。
 *
 * 与net_buf_simple_pull()相同，但用于操作64位大端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从大端转换为主机端的64位值。
 */
uint6464_t net_buf_simple_pull_be64(struct net_buf_simple *buf);

/**
 * @brief 获取缓冲区的尾指针。
 *
 * 获取指向缓冲区中数据末尾的指针。
 *
 * @param buf 缓冲区。
 *
 * @return 缓冲区的尾指针。
 */
static inline uint8_t *net_buf_simple_tail(const struct net_buf_simple *buf)
{
	return buf->data + buf->len;
}

/**
 * @brief 检查缓冲区的头部空间。
 *
 * 检查缓冲区开头有多少空闲空间。
 *
 * @param buf 缓冲区的有效指针
 *
 * @return 缓冲区开头可用的字节数。
 */
size_t net_buf_simple_headroom(const struct net_buf_simple *buf);

/**
 * @brief 检查缓冲区的尾部空间。
 *
 * 检查缓冲区末尾有多少空闲空间。
 *
 * @param buf 缓冲区的有效指针
 *
 * @return 缓冲区末尾可用的字节数。
 */
size_t net_buf_simple_tailroom(const struct net_buf_simple *buf);

/**
 * @brief 检查net_buf_simple::len的最大值。
 *
 * 该值取决于保留为头部空间的字节数。
 *
 * @param buf 缓冲区的有效指针
 *
 * @return net_buf_simple::data指针后面可用的字节数。
 */
uint16_t net_buf_simple_max_len(const struct net_buf_simple *buf);

/**
 * @brief 缓冲区的解析状态。
 *
 * 这用于在将解析控制权交给我们无法控制的例程时临时存储缓冲区的解析状态。
 */
struct net_buf_simple_state {
	/** 数据指针相对于存储起始位置的偏移量 */
	uint16_t offset;
	/** 数据长度 */
	uint16_t len;
};

/**
 * @brief 保存缓冲区的解析状态。
 *
 * 保存缓冲区的解析状态，以便以后恢复。
 *
 * @param buf 要保存状态的缓冲区。
 * @param state 状态的存储位置。
 */
static inline void net_buf_simple_save(const struct net_buf_simple *buf,
				       struct net_buf_simple_state *state)
{
	state->offset = (uint16_t)net_buf_simple_headroom(buf);
	state->len = buf->len;
}

/**
 * @brief 恢复缓冲区的解析状态。
 *
 * 从之前由net_buf_simple_save()存储的状态恢复缓冲区的解析状态。
 *
 * @param buf 要恢复状态的缓冲区。
 * @param state 存储的状态。
 */
static inline void net_buf_simple_restore(struct net_buf_simple *buf,
					  struct net_buf_simple_state *state)
{
	buf->data = buf->__buf + state->offset;
	buf->len = state->len;
}

/**
 * 标志，指示缓冲区的关联数据指针指向外部分配的内存。因此，一旦引用计数降为零，指向的数据将不需要释放。net_buf API用户不需要显式设置或取消设置此标志。这样的net_buf只能通过net_buf_alloc_with_data()函数实例化。然而，引用计数机制将以相同的方式工作，引用计数降为0将释放net_buf，但不会释放其中的数据指针。
 */
#define NET_BUF_EXTERNAL_DATA  BIT(0)

/**
 * @brief 网络缓冲区表示。
 *
 * 此结构用于表示网络缓冲区。这样的缓冲区通常通过NET_BUF_POOL_*_DEFINE() API定义，并使用net_buf_alloc() API分配。
 */
struct net_buf {
	/** 允许将缓冲区放入sys_slist_t */
	sys_snode_t node;

	/** 与此缓冲区关联的片段。 */
	struct net_buf *frags;

	/** 引用计数。 */
	uint8_t ref;

	/** 缓冲区标志的位字段。 */
	uint8_t flags;

	/** 缓冲区释放时应去往的位置。 */
	uint8_t pool_id;

	/** 此缓冲区上分配的用户数据大小 */
	uint8_t user_data_size;

	/** 方便访问net_buf_simple成员的联合，同时保留旧API。 */
	union {
		/* 此结构的ABI必须与net_buf_simple匹配 */
		struct {
			/** 指向缓冲区中数据起始位置的指针。 */
			uint8_t *data;

			/** 数据指针后面的数据长度。 */
			uint16_t len;

			/** 此缓冲区可以存储的数据量。 */
			uint16_t size;

			/** 数据存储的起始位置。不应直接访问（应使用数据指针）。 */
			uint8_t *__buf;
		};

		/** @cond INTERNAL_HIDDEN */
		struct net_buf_simple b;
		/** @endcond */
	};

	/** 此缓冲区的系统元数据。 */
	uint8_t user_data[] __net_buf_align;
};

/** @cond INTERNAL_HIDDEN */

struct net_buf_data_cb {
	uint8_t * __must_check (*alloc)(struct net_buf *buf, size_t *size,
			   k_timeout_t timeout);
	uint8_t * __must_check (*ref)(struct net_buf *buf, uint8_t *data);
	void   (*unref)(struct net_buf *buf, uint8_t *data);
};

struct net_buf_data_alloc {
	const struct net_buf_data_cb *cb;
	void *alloc_data;
	size_t max_alloc_size;
};

/** @endcond */

/**
 * @brief 网络缓冲区池表示。
 *
 * 此结构用于表示网络缓冲区池。
 */
struct net_buf_pool {
	/** 释放时将缓冲区放入的LIFO */
	struct k_lifo free;

	/** 防止并发访问/修改 */
	struct k_spinlock lock;

	/** 池中的缓冲区数量 */
	const uint16_t buf_count;

	/** 未初始化的缓冲区数量 */
	uint16_t uninit_count;

	/** 分配给此池的用户数据大小 */
	uint8_t user_data_size;

#if defined(CONFIG_NET_BUF_POOL_USAGE)
	/** 池中可用缓冲区的数量。 */
	atomic_t avail_count;

	/** 池的总大小。 */
	const uint16_t pool_size;

	/** 池的名称。用于打印池信息时。 */
	const char *name;
#endif /* CONFIG_NET_BUF_POOL_USAGE */

	/** 缓冲区释放时的可选销毁回调。 */
	void (*const destroy)(struct net_buf *buf);

	/** 数据分配处理程序。 */
	const struct net_buf_data_alloc *alloc;

	/** 缓冲区存储数组的起始位置 */
	struct net_buf * const __bufs;
};

/** @cond INTERNAL_HIDDEN */
#define NET_BUF_POOL_USAGE_INIT(_pool, _count) \
	IF_ENABLED(CONFIG_NET_BUF_POOL_USAGE, (.avail_count = ATOMIC_INIT(_count),)) \
	IF_ENABLED(CONFIG_NET_BUF_POOL_USAGE, (.name = STRINGIFY(_pool),))

#define NET_BUF_POOL_INITIALIZER(_pool, _alloc, _bufs, _count, _ud_size, _destroy) \
	{                                                                          \
		.free = Z_LIFO_INITIALIZER(_pool.free),                            \
		.lock = { },                                                       \
		.buf_count = _count,                                               \
		.uninit_count = _count,                                            \
		.user_data_size = _ud_size,                                        \
		NET_BUF_POOL_USAGE_INIT(_pool, _count)                             \
		.destroy = _destroy,                                               \
		.alloc = _alloc,                                                   \
		.__bufs = (struct net_buf *)_bufs,                                 \
	}

#define _NET_BUF_ARRAY_DEFINE(_name, _count, _ud_size)					       \
	struct _net_buf_##_name { uint8_t b[sizeof(struct net_buf)];			       \
				  uint8_t ud[_ud_size]; } __net_buf_align;		       \
	BUILD_ASSERT(_ud_size <= UINT8_MAX);						       \
	BUILD_ASSERT(offsetof(struct net_buf, user_data) ==				       \
		     offsetof(struct _net_buf_##_name, ud), "Invalid offset");		       \
	BUILD_ASSERT(__alignof__(struct net_buf) ==					       \
		     __alignof__(struct _net_buf_##_name), "Invalid alignment");	       \
	BUILD_ASSERT(sizeof(struct _net_buf_##_name) ==					       \
		     ROUND_UP(sizeof(struct net_buf) + _ud_size, __alignof__(struct net_buf)), \
		     "Size cannot be determined");					       \
	static struct _net_buf_##_name _net_buf_##_name[_count] __noinit
extern const struct net_buf_data_alloc net_buf_heap_alloc;
/** @endcond */
/**
 *
 * @brief 定义一个使用堆作为数据的缓冲区池。
 *
 * 定义一个net_buf_pool结构和所需数量的缓冲区的必要内存存储（结构数组）。之后，可以通过net_buf_alloc从池中访问缓冲区。池定义为静态变量，因此如果需要在当前模块之外导出它，则需要使用单独的指针而不是extern声明来实现。
 *
 * 缓冲区的数据有效载荷将从堆中分配，使用k_malloc，因此必须将CONFIG_HEAP_MEM_POOL_SIZE设置为正值。这种类型的池不支持数据分配时的阻塞，因此在尝试分配数据时传递给net_buf_alloc的超时将始终视为K_NO_WAIT。这意味着分配失败，即返回NULL，必须始终干净地处理。
 *
 * 如果提供了自定义销毁回调，则此回调负责最终调用net_buf_destroy()以完成将缓冲区返回池的过程。
 *
 * @param _name      池变量的名称。
 * @param _count     池中的缓冲区数量。
 * @param _ud_size   每个缓冲区保留的用户数据空间。
 * @param _destroy   缓冲区释放时的可选销毁回调。
 */
#define NET_BUF_POOL_HEAP_DEFINE(_name, _count, _ud_size, _destroy)          \
	_NET_BUF_ARRAY_DEFINE(_name, _count, _ud_size);                      \
	static STRUCT_SECTION_ITERABLE(net_buf_pool, _name) =                \
		NET_BUF_POOL_INITIALIZER(_name, &net_buf_heap_alloc,         \
					 _net_buf_##_name, _count, _ud_size, \
					 _destroy)
/** @cond INTERNAL_HIDDEN */
struct net_buf_pool_fixed {
	uint8_t *data_pool;
};
extern const struct net_buf_data_cb net_buf_fixed_cb;
/** @endcond */
/**
 *
 * @brief 定义一个基于固定大小数据的缓冲区池
 *
 * 定义一个net_buf_pool结构和所需数量的缓冲区的必要内存存储（结构数组）。之后，可以通过net_buf_alloc从池中访问缓冲区。池定义为静态变量，因此如果需要在当前模块之外导出它，则需要使用单独的指针而不是extern声明来实现。
 *
 * 缓冲区的数据有效载荷将从固定大小的字节数组中分配。这种类型的池不支持数据分配时的阻塞，因此在尝试分配数据时传递给net_buf_alloc的超时将始终视为K_NO_WAIT。这意味着分配失败，即返回NULL，必须始终干净地处理。
 *
 * 如果提供了自定义销毁回调，则此回调负责最终调用net_buf_destroy()以完成将缓冲区返回池的过程。
 *
 * @param _name      池变量的名称。
 * @param _count     池中的缓冲区数量。
 * @param _data_size 每个缓冲区的最大数据有效载荷。
 * @param _ud_size   每个缓冲区保留的用户数据空间。
 * @param _destroy   缓冲区释放时的可选销毁回调。
 */
#define NET_BUF_POOL_FIXED_DEFINE(_name, _count, _data_size, _ud_size, _destroy) \
	_NET_BUF_ARRAY_DEFINE(_name, _count, _ud_size);                        \
	static uint8_t __noinit net_buf_data_##_name[_count][_data_size] __net_buf_align; \
	static const struct net_buf_pool_fixed net_buf_fixed_##_name = {       \
		.data_pool = (uint8_t *)net_buf_data_##_name,                  \
	};                                                                     \
	static const struct net_buf_data_alloc net_buf_fixed_alloc_##_name = { \
		.cb = &net_buf_fixed_cb,                                       \
		.alloc_data = (void *)&net_buf_fixed_##_name,                  \
		.max_alloc_size = _data_size,                                  \
	};                                                                     \
	static STRUCT_SECTION_ITERABLE(net_buf_pool, _name) =                  \
		NET_BUF_POOL_INITIALIZER(_name, &net_buf_fixed_alloc_##_name,  \
					 _net_buf_##_name, _count, _ud_size,   \
					 _destroy)
/** @cond INTERNAL_HIDDEN */
extern const struct net_buf_data_cb net_buf_var_cb;
/** @endcond */
/**
 *
 * @brief 定义一个具有可变大小有效载荷的缓冲区池
 *
 * 定义一个net_buf_pool结构和所需数量的缓冲区的必要内存存储（结构数组）。之后，可以通过net_buf_alloc从池中访问缓冲区。池定义为静态变量，因此如果需要在当前模块之外导出它，则需要使用单独的指针而不是extern声明来实现。
 *
 * 缓冲区的数据有效载荷将基于内存池，从中可以分配可变大小的有效载荷。
 *
 * 如果提供了自定义销毁回调，则此回调负责最终调用net_buf_destroy()以完成将缓冲区返回池的过程。
 *
 * @param _name      池变量的名称。
 * @param _count     池中的缓冲区数量。
 * @param _data_size 数据有效载荷的总内存量。
 * @param _ud_size   每个缓冲区保留的用户数据空间。
 * @param _destroy   缓冲区释放时的可选销毁回调。
 */
#define NET_BUF_POOL_VAR_DEFINE(_name, _count, _data_size, _ud_size, _destroy) \
	_NET_BUF_ARRAY_DEFINE(_name, _count, _ud_size);                        \
	K_HEAP_DEFINE(net_buf_mem_pool_##_name, _data_size);                   \
	static const struct net_buf_data_alloc net_buf_data_alloc_##_name = {  \
		.cb = &net_buf_var_cb,                                         \
		.alloc_data = &net_buf_mem_pool_##_name,                       \
		.max_alloc_size = 0,                                           \
	};                                                                     \
	static STRUCT_SECTION_ITERABLE(net_buf_pool, _name) =                  \
		NET_BUF_POOL_INITIALIZER(_name, &net_buf_data_alloc_##_name,   \
					 _net_buf_##_name, _count, _ud_size,   \
					 _destroy)
/**
 *
 * @brief 定义一个新的缓冲区池
 *
 * 定义一个net_buf_pool结构和所需数量的缓冲区的必要内存存储（结构数组）。之后，可以通过net_buf_alloc从池中访问缓冲区。池定义为静态变量，因此如果需要在当前模块之外导出它，则需要使用单独的指针而不是extern声明来实现。
 *
 * 如果提供了自定义销毁回调，则此回调负责最终调用net_buf_destroy()以完成将缓冲区返回池的过程。
 *
 * @param _name     池变量的名称。
 * @param _count    池中的缓冲区数量。
 * @param _size     每个缓冲区的最大数据大小。
 * @param _ud_size  保留的用户数据空间量。
 * @param _destroy  缓冲区释放时的可选销毁回调。
 */
#define NET_BUF_POOL_DEFINE(_name, _count, _size, _ud_size, _destroy)        \
	NET_BUF_POOL_FIXED_DEFINE(_name, _count, _size, _ud_size, _destroy)
/**
 * @brief 根据其ID查找池。
 *
 * @param id 池ID（例如来自buf->pool_id）。
 *
 * @return 指向池的指针。
 */
struct net_buf_pool *net_buf_pool_get(int id);
/**
 * @brief 获取缓冲区的零基索引。
 *
 * 此函数将缓冲区转换为零基索引，基于其在缓冲区池中的位置。如果您希望将外部元数据上下文数组与池的缓冲区关联，这可能很有用。
 *
 * @param buf  网络缓冲区。
 *
 * @return 缓冲区的零基索引。
 */
int net_buf_id(const struct net_buf *buf);
/**
 * @brief 从池中分配一个新的固定缓冲区。
 *
 * @note 某些类型的数据分配器不支持阻塞（例如HEAP类型）。在这种情况下，即使给定了K_FOREVER，net_buf_alloc()仍可能失败（返回NULL）。
 *
 * @note 如果从系统工作队列调用，则超时值将被覆盖为K_NO_WAIT。
 *
 * @param pool 从哪个池分配缓冲区。
 * @param timeout 如果池为空，影响采取的操作。如果是K_NO_WAIT，则立即返回。如果是K_FOREVER，则等待必要的时间。否则，等待指定的超时。
 *
 * @return 新缓冲区或如果缓冲区不足则返回NULL。
 */
#if defined(CONFIG_NET_BUF_LOG)
struct net_buf * __must_check net_buf_alloc_fixed_debug(struct net_buf_pool *pool,
							k_timeout_t timeout,
							const char *func,
							int line);
#define net_buf_alloc_fixed(_pool, _timeout) \
	net_buf_alloc_fixed_debug(_pool, _timeout, __func__, __LINE__)
#else
struct net_buf * __must_check net_buf_alloc_fixed(struct net_buf_pool *pool,
						  k_timeout_t timeout);
#endif
/**
 * @copydetails net_buf_alloc_fixed
 */
static inline struct net_buf * __must_check net_buf_alloc(struct net_buf_pool *pool,
							  k_timeout_t timeout)
{
	return net_buf_alloc_fixed(pool, timeout);
}
/**
 * @brief 从池中分配一个新的可变长度缓冲区。
 *
 * @note 某些类型的数据分配器不支持阻塞（例如HEAP类型）。在这种情况下，即使给定了K_FOREVER，net_buf_alloc()仍可能失败（返回NULL）。
 *
 * @note 如果从系统工作队列调用，则超时值将被覆盖为K_NO_WAIT。
 *
 * @param pool 从哪个池分配缓冲区。
 * @param size 缓冲区必须能够容纳的数据量。
 * @param timeout 如果池为空，影响采取的操作。如果是K_NO_WAIT，则立即返回。如果是K_FOREVER，则等待必要的时间。否则，等待指定的超时。
 *
 * @return 新缓冲区或如果缓冲区不足则返回NULL。
 */
#if defined(CONFIG_NET_BUF_LOG)
struct net_buf * __must_check net_buf_alloc_len_debug(struct net_buf_pool *pool,
						      size_t size,
						      k_timeout_t timeout,
						      const char *func,
						      int line);
#define net_buf_alloc_len(_pool, _size, _timeout) \
	net_buf_alloc_len_debug(_pool, _size, _timeout, __func__, __LINE__)
#else
struct net_buf * __must_check net_buf_alloc_len(struct net_buf_pool *pool,
						size_t size,
						k_timeout_t timeout);
#endif
/**
 * @brief 从池中分配一个新的缓冲区，但具有外部数据指针。
 *
 * 从池中分配一个新的缓冲区，其中数据指针来自用户而不是池。
 *
 * @note 某些类型的数据分配器不支持阻塞（例如HEAP类型）。在这种情况下，即使给定了K_FOREVER，net_buf_alloc()仍可能失败（返回NULL）。
 *
 * @note 如果从系统工作队列调用，则超时值将被覆盖为K_NO_WAIT。
 *
 * @param pool 从哪个池分配缓冲区。
 * @param data 外部数据指针
 * @param size 指向的数据缓冲区能够容纳的数据量。
 * @param timeout 如果池为空，影响采取的操作。如果是K_NO_WAIT，则立即返回。如果是K_FOREVER，则等待必要的时间。否则，等待指定的超时。
 *
 * @return 新缓冲区或如果缓冲区不足则返回NULL。
 */
#if defined(CONFIG_NET_BUF_LOG)
struct net_buf * __must_check net_buf_alloc_with_data_debug(struct net_buf_pool *pool,
							    void *data, size_t size,
							    k_timeout_t timeout,
							    const char *func, int line);
#define net_buf_alloc_with_data(_pool, _data_, _size, _timeout)		\
	net_buf_alloc_with_data_debug(_pool, _data_, _size, _timeout,	\
				      __func__, __LINE__)
#else
struct net_buf * __must_check net_buf_alloc_with_data(struct net_buf_pool *pool,
						      void *data, size_t size,
						      k_timeout_t timeout);
#endif
/**
 * @brief 从FIFO中获取缓冲区。
 *
 * 如果FIFO中的缓冲区包含片段，则此函数不是线程安全的。
 *
 * @param fifo 从哪个FIFO获取缓冲区。
 * @param timeout 如果FIFO为空，影响采取的操作。如果是K_NO_WAIT，则立即返回。如果是K_FOREVER，则等待必要的时间。否则，等待指定的超时。
 *
 * @return 新缓冲区或如果FIFO为空则返回NULL。
 */
#if defined(CONFIG_NET_BUF_LOG)
struct net_buf * __must_check net_buf_get_debug(struct k_fifo *fifo,
						k_timeout_t timeout,
						const char *func, int line);
#define	net_buf_get(_fifo, _timeout) \
	net_buf_get_debug(_fifo, _timeout, __func__, __LINE__)
#else
struct net_buf * __must_check net_buf_get(struct k_fifo *fifo,
					  k_timeout_t timeout);
#endif
/**
 * @brief 从自定义销毁回调中销毁缓冲区
 *
 * 此助手仅用于自定义销毁回调。如果没有为NET_BUF_POOL_*_DEFINE()提供自定义销毁回调，则无需使用此API。
 *
 * @param buf 要销毁的缓冲区。
 */
static inline void net_buf_destroy(struct net_buf *buf)
{
	struct net_buf_pool *pool = net_buf_pool_get(buf->pool_id);
	if (buf->__buf) {
		if (!(buf->flags & NET_BUF_EXTERNAL_DATA)) {
			pool->alloc->cb->unref(buf, buf->__buf);
		}
		buf->__buf = NULL;
	}
	k_lifo_put(&pool->free, buf);
}
/**
 * @brief 重置缓冲区
 *
 * 重置缓冲区数据和标志，以便可以将其重新用于其他目的。
 *
 * @param buf 要重置的缓冲区。
 */
void net_buf_reset(struct net_buf *buf);
/**
 * @brief 使用给定的头部空间初始化缓冲区。
 *
 * 调用此API时，缓冲区不应包含任何数据。
 *
 * @param buf 要初始化的缓冲区。
 * @param reserve 要保留的头部空间。
 */
void net_buf_simple_reserve(struct net_buf_simple *buf, size_t reserve);
/**
 * @brief 将缓冲区放入列表中
 *
 * 如果缓冲区包含后续片段，此函数将负责将它们也插入列表中。
 *
 * @param list 要将缓冲区附加到的列表。
 * @param buf 缓冲区。
 */
void net_buf_slist_put(sys_slist_t *list, struct net_buf *buf);
/**
 * @brief 从列表中获取缓冲区。
 *
 * 如果缓冲区有任何片段，这些片段将自动从列表中恢复，并放置到缓冲区的片段列表中。
 *
 * @param list 要从中获取缓冲区的列表。
 *
 * @return 新缓冲区或如果FIFO为空则返回NULL。
 */
struct net_buf * __must_check net_buf_slist_get(sys_slist_t *list);
/**
 * @brief 将缓冲区放入FIFO的末尾。
 *
 * 如果缓冲区包含后续片段，此函数将负责将它们也插入FIFO中。
 *
 * @param fifo 要将缓冲区放入的FIFO。
 * @param buf 缓冲区。
 */
void net_buf_put(struct k_fifo *fifo, struct net_buf *buf);
/**
 * @brief 减少缓冲区的引用计数。
 *
 * 如果引用计数达到零，则将缓冲区放回池中。
 *
 * @param buf 缓冲区的有效指针
 */
#if defined(CONFIG_NET_BUF_LOG)
void net_buf_unref_debug(struct net_buf *buf, const char *func, int line);
#define	net_buf_unref(_buf) \
	net_buf_unref_debug(_buf, __func__, __LINE__)
#else
void net_buf_unref(struct net_buf *buf);
#endif
/**
 * @brief 增加缓冲区的引用计数。
 *
 * @param buf 缓冲区的有效指针
 *
 * @return 新引用的缓冲区
 */
struct net_buf * __must_check net_buf_ref(struct net_buf *buf);
/**
 * @brief 克隆缓冲区
 *
 * 复制给定缓冲区，包括当前存储的任何（用户）数据和头部。
 *
 * @param buf 缓冲区的有效指针
 * @param timeout 如果池为空，影响采取的操作。如果是K_NO_WAIT，则立即返回。如果是K_FOREVER，则等待必要的时间。否则，等待指定的超时。
 *
 * @return 克隆的缓冲区或如果缓冲区不足则返回NULL。
 */
struct net_buf * __must_check net_buf_clone(struct net_buf *buf,
					    k_timeout_t timeout);
/**
 * @brief 获取缓冲区的用户数据指针。
 *
 * @param buf 缓冲区的有效指针
 *
 * @return 指向缓冲区用户数据的指针。
 */
static inline void * __must_check net_buf_user_data(const struct net_buf *buf)
{
	return (void *)buf->user_data;
}
/**
 * @brief 将用户数据从一个缓冲区复制到另一个缓冲区。
 *
 * @param dst 有效指向缓冲区的指针，其用户数据将被覆盖。
 * @param src 有效指向缓冲区的指针，其用户数据将被复制。用户数据大小必须等于或大于@a dst。
 *
 * @return 成功返回0，失败返回负错误号。
 */
int net_buf_user_data_copy(struct net_buf *dst, const struct net_buf *src);
/**
 * @brief 使用给定的头部空间初始化缓冲区。
 *
 * 调用此API时，缓冲区不应包含任何数据。
 *
 * @param buf 要初始化的缓冲区。
 * @param reserve
 * 要保留的头部空间。
 */
static inline void net_buf_reserve(struct net_buf *buf, size_t reserve)
{
	net_buf_simple_reserve(&buf->b, reserve);
}
/**
 * @brief 准备在缓冲区末尾添加数据
 *
 * 增加缓冲区的数据长度以考虑在末尾添加更多数据。
 *
 * @param buf 要更新的缓冲区。
 * @param len 要增加的长度。
 *
 * @return 缓冲区的原始尾部。
 */
static inline void *net_buf_add(struct net_buf *buf, size_t len)
{
	return net_buf_simple_add(&buf->b, len);
}
/**
 * @brief 将给定数量的字节复制到缓冲区末尾
 *
 * 增加缓冲区的数据长度以考虑在末尾添加更多数据。
 *
 * @param buf 要更新的缓冲区。
 * @param mem 要添加的数据的位置。
 * @param len 要添加的数据长度
 *
 * @return 缓冲区的原始尾部。
 */
static inline void *net_buf_add_mem(struct net_buf *buf, const void *mem,
				    size_t len)
{
	return net_buf_simple_add_mem(&buf->b, mem, len);
}
/**
 * @brief 在缓冲区末尾添加（8位）字节
 *
 * 增加缓冲区的数据长度以考虑在末尾添加更多数据。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加的字节值。
 *
 * @return 指向添加的值的指针
 */
static inline uint8_t *net_buf_add_u8(struct net_buf *buf, uint8_t val)
{
	return net_buf_simple_add_u8(&buf->b, val);
}
/**
 * @brief 在缓冲区末尾添加16位值
 *
 * 以小端格式在缓冲区末尾添加16位值。增加缓冲区的数据长度以考虑在末尾添加更多数据。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加的16位值。
 */
static inline void net_buf_add_le16(struct net_buf *buf, uint16_t val)
{
	net_buf_simple_add_le16(&buf->b, val);
}
/**
 * @brief 在缓冲区末尾添加16位值
 *
 * 以大端格式在缓冲区末尾添加16位值。增加缓冲区的数据长度以考虑在末尾添加更多数据。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加的16位值。
 */
static inline void net_buf_add_be16(struct net_buf *buf, uint16_t val)
{
	net_buf_simple_add_be16(&buf->b, val);
}
/**
 * @brief 在缓冲区末尾添加24位值
 *
 * 以小端格式在缓冲区末尾添加24位值。增加缓冲区的数据长度以考虑在末尾添加更多数据。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加的24位值。
 */
static inline void net_buf_add_le24(struct net_buf *buf, uint32_t val)
{
	net_buf_simple_add_le24(&buf->b, val);
}
/**
 * @brief 在缓冲区末尾添加24位值
 *
 * 以大端格式在缓冲区末尾添加24位值。增加缓冲区的数据长度以考虑在末尾添加更多数据。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加的24位值。
 */
static inline void net_buf_add_be24(struct net_buf *buf, uint32_t val)
{
	net_buf_simple_add_be24(&buf->b, val);
}
/**
 * @brief 在缓冲区末尾添加32位值
 *
 * 以小端格式在缓冲区末尾添加32位值。增加缓冲区的数据长度以考虑在末尾添加更多数据。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加的32位值。
 */
static inline void net_buf_add_le32(struct net_buf *buf, uint32_t val)
{
	net_buf_simple_add_le32(&buf->b, val);
}
/**
 * @brief 在缓冲区末尾添加32位值
 *
 * 以大端格式在缓冲区末尾添加32位值。增加缓冲区的数据长度以考虑在末尾添加更多数据。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加的32位值。
 */
static inline void net_buf_add_be32(struct net_buf *buf, uint32_t val)
{
	net_buf_simple_add_be32(&buf->b, val);
}
/**
 * @brief 在缓冲区末尾添加40位值
 *
 * 以小端格式在缓冲区末尾添加40位值。增加缓冲区的数据长度以考虑在末尾添加更多数据。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加的40位值。
 */
static inline void net_buf_add_le40(struct net_buf *buf, uint64_t val)
{
	net_buf_simple_add_le40(&buf->b, val);
}
/**
 * @brief 在缓冲区末尾添加40位值
 *
 * 以大端格式在缓冲区末尾添加40位值。增加缓冲区的数据长度以考虑在末尾添加更多数据。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加的40位值。
 */
static inline void net_buf_add_be40(struct net_buf *buf, uint64_t val)
{
	net_buf_simple_add_be40(&buf->b, val);
}
/**
 * @brief 在缓冲区末尾添加48位值
 *
 * 以小端格式在缓冲区末尾添加48位值。增加缓冲区的数据长度以考虑在末尾添加更多数据。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加的48位值。
 */
static inline void net_buf_add_le48(struct net_buf *buf, uint64_t val)
{
	net_buf_simple_add_le48(&buf->b, val);
}
/**
 * @brief 在缓冲区末尾添加48位值
 *
 * 以大端格式在缓冲区末尾添加48位值。增加缓冲区的数据长度以考虑在末尾添加更多数据。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加的48位值。
 */
static inline void net_buf_add_be48(struct net_buf *buf, uint64_t val)
{
	net_buf_simple_add_be48(&buf->b, val);
}
/**
 * @brief 在缓冲区末尾添加64位值
 *
 * 以小端格式在缓冲区末尾添加64位值。增加缓冲区的数据长度以考虑在末尾添加更多数据。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加的64位值。
 */
static inline void net_buf_add_le64(struct net_buf *buf, uint64_t val)
{
	net_buf_simple_add_le64(&buf->b, val);
}
/**
 * @brief 在缓冲区末尾添加64位值
 *
 * 以大端格式在缓冲区末尾添加64位值。增加缓冲区的数据长度以考虑在末尾添加更多数据。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加的64位值。
 */
static inline void net_buf_add_be64(struct net_buf *buf, uint64_t val)
{
	net_buf_simple_add_be64(&buf->b, val);
}
/**
 * @brief 从缓冲区末尾移除数据。
 *
 * 通过修改缓冲区长度从缓冲区末尾移除数据。
 *
 * @param buf 要更新的缓冲区。
 * @param len 要移除的字节数。
 *
 * @return 缓冲区数据的新末尾。
 */
static inline void *net_buf_remove_mem(struct net_buf *buf, size_t len)
{
	return net_buf_simple_remove_mem(&buf->b, len);
}
/**
 * @brief 从缓冲区末尾移除8位值
 *
 * 与net_buf_remove_mem()相同，但用于操作8位值的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 移除的8位值
 */
static inline uint8_t net_buf_remove_u8(struct net_buf *buf)
{
	return net_buf_simple_remove_u8(&buf->b);
}
/**
 * @brief 从缓冲区末尾移除并转换16位值。
 *
 * 与net_buf_remove_mem()相同，但用于操作16位小端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从小端转换为主机端的16位值。
 */
static inline uint16_t net_buf_remove_le16(struct net_buf *buf)
{
	return net_buf_simple_remove_le16(&buf->b);
}
/**
 * @brief 从缓冲区末尾移除并转换16位值。
 *
 * 与net_buf_remove_mem()相同，但用于操作16位大端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从大端转换为主机端的16位值。
 */
static inline uint16_t net_buf_remove_be16(struct net_buf *buf)
{
	return net_buf_simple_remove_be16(&buf->b);
}
/**
 * @brief 从缓冲区末尾移除并转换24位值。
 *
 * 与net_buf_remove_mem()相同，但用于操作24位小端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从小端转换为主机端的24位值。
 */
static inline uint32_t net_buf_remove_le24(struct net_buf *buf)
{
	return net_buf_simple_remove_le24(&buf->b);
}
/**
 * @brief 从缓冲区末尾移除并转换24位值。
 *
 * 与net_buf_remove_mem()相同，但用于操作24位大端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从大端转换为主机端的24位值。
 */
static inline uint32_t net_buf_remove_be24(struct net_buf *buf)
{
	return net_buf_simple_remove_be24(&buf->b);
}
/**
 * @brief 从缓冲区末尾移除并转换32位值。
 *
 * 与net_buf_remove_mem()相同，但用于操作32位小端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从小端转换为主机端的32位值。
 */
static inline uint32_t net_buf_remove_le32(struct net_buf *buf)
{
	return net_buf_simple_remove_le32(&buf->b);
}
/**
 * @brief 从缓冲区末尾移除并转换32位值。
 *
 * 与net_buf_remove_mem()相同，但用于操作32位大端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从大端转换为主机端的32位值。
 */
static inline uint32_t net_buf_remove_be32(struct net_buf *buf)
{
	return net_buf_simple_remove_be32(&buf->b);
}
/**
 * @brief 从缓冲区末尾移除并转换40位值。
 *
 * 与net_buf_remove_mem()相同，但用于操作40位小端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从小端转换为主机端的40位值。
 */
static inline uint64_t net_buf_remove_le40(struct net_buf *buf)
{
	return net_buf_simple_remove_le40(&buf->b);
}
/**
 * @brief 从缓冲区末尾移除并转换40位值。
 *
 * 与net_buf_remove_mem()相同，但用于操作40位大端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从大端转换为主机端的40位值。
 */
static inline uint64_t net_buf_remove_be40(struct net_buf *buf)
{
	return net_buf_simple_remove_be40(&buf->b);
}
/**
 * @brief 从缓冲区末尾移除并转换48位值。
 *
 * 与net_buf_remove_mem()相同，但用于操作48位小端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从小端转换为主机端的48位值。
 */
static inline uint64_t net_buf_remove_le48(struct net_buf *buf)
{
	return net_buf_simple_remove_le48(&buf->b);
}
/**
 * @brief 从缓冲区末尾移除并转换48位值。
 *
 * 与net_buf_remove_mem()相同，但用于操作48位大端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从大端转换为主机端的48位值。
 */
static inline uint64_t net_buf_remove_be48(struct net_buf *buf)
{
	return net_buf_simple_remove_be48(&buf->b);
}
/**
 * @brief 从缓冲区末尾移除并转换64位值。
 *
 * 与net_buf_remove_mem()相同，但用于操作64位小端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从小端转换为主机端的64位值。
 */
static inline uint64_t net_buf_remove_le64(struct net_buf *buf)
{
	return net_buf_simple_remove_le64(&buf->b);
}
/**
 * @brief 从缓冲区末尾移除并转换64位值。
 *
 * 与net_buf_remove_mem()相同，但用于操作64位大端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从大端转换为主机端的64位值。
 */
static inline uint64_t net_buf_remove_be64(struct net_buf *buf)
{
	return net_buf_simple_remove_be64(&buf->b);
}
/**
 * @brief 准备在缓冲区开头添加数据
 *
 * 修改数据指针和缓冲区长度以考虑在缓冲区开头添加更多数据。
 *
 * @param buf 要更新的缓冲区。
 * @param len 要添加到开头的字节数。
 *
 * @return 缓冲区数据的新开头。
 */
static inline void *net_buf_push(struct net_buf *buf, size_t len)
{
	return net_buf_simple_push(&buf->b, len);
}
/**
 * @brief 将给定数量的字节复制到缓冲区开头
 *
 * 修改数据指针和缓冲区长度以考虑在缓冲区开头添加更多数据。
 *
 * @param buf 要更新的缓冲区。
 * @param mem 要添加的数据的位置。
 * @param len 要添加的数据长度。
 *
 * @return 缓冲区数据的新开头。
 */
static inline void *net_buf_push_mem(struct net_buf *buf, const void *mem,
				     size_t len)
{
	return net_buf_simple_push_mem(&buf->b, mem, len);
}
/**
 * @brief 在缓冲区开头添加8位值
 *
 * 在缓冲区开头添加8位值。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加到缓冲区的8位值。
 */
static inline void net_buf_push_u8(struct net_buf *buf, uint8_t val)
{
	net_buf_simple_push_u8(&buf->b, val);
}
/**
 * @brief 在缓冲区开头添加16位值
 *
 * 以小端格式在缓冲区开头添加16位值。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加到缓冲区的16位值。
 */
static inline void net_buf_push_le16(struct net_buf *buf, uint16_t val)
{
	net_buf_simple_push_le16(&buf->b, val);
}
/**
 * @brief 在缓冲区开头添加16位值
 *
 * 以大端格式在缓冲区开头添加16位值。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加到缓冲区的16位值。
 */
static inline void net_buf_push_be16(struct net_buf *buf, uint16_t val)
{
	net_buf_simple_push_be16(&buf->b, val);
}
/**
 * @brief 在缓冲区开头添加24位值
 *
 * 以小端格式在缓冲区开头添加24位值。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加到缓冲区的24位值。
 */
static inline void net_buf_push_le24(struct net_buf *buf, uint32_t val)
{
	net_buf_simple_push_le24(&buf->b, val);
}
/**
 * @brief 在缓冲区开头添加24位值
 *
 * 以大端格式在缓冲区开头添加24位值。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加到缓冲区的24位值。
 */
static inline void net_buf_push_be24(struct net_buf *buf, uint32_t val)
{
	net_buf_simple_push_be24(&buf->b, val);
}
/**
 * @brief 在缓冲区开头添加32位值
 *
 * 以小端格式在缓冲区开头添加32位值。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加到缓冲区的32位值。
 */
static inline void net_buf_push_le32(struct net_buf *buf, uint32_t val)
{
	net_buf_simple_push_le32(&buf->b, val);
}
/**
 * @brief 在缓冲区开头添加32位值
 *
 * 以大端格式在缓冲区开头添加32位值。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加到缓冲区的32位值。
 */
static inline void net_buf_push_be32(struct net_buf *buf, uint32_t val)
{
	net_buf_simple_push_be32(&buf->b, val);
}
/**
 * @brief 在缓冲区开头添加40位值
 *
 * 以小端格式在缓冲区开头添加40位值。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加到缓冲区的40位值。
 */
static inline void net_buf_push_le40(struct net_buf *buf, uint64_t val)
{
	net_buf_simple_push_le40(&buf->b, val);
}
/**
 * @brief 在缓冲区开头添加40位值
 *
 * 以大端格式在缓冲区开头添加40位值。
 *
 * @param buf 要更新的
 * 缓冲区。
 * @param val 要添加到缓冲区的40位值。
 */
static inline void net_buf_push_be40(struct net_buf *buf, uint64_t val)
{
	net_buf_simple_push_be40(&buf->b, val);
}
/**
 * @brief 在缓冲区开头添加48位值
 *
 * 以小端格式在缓冲区开头添加48位值。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加到缓冲区的48位值。
 */
static inline void net_buf_push_le48(struct net_buf *buf, uint64_t val)
{
	net_buf_simple_push_le48(&buf->b, val);
}
/**
 * @brief 在缓冲区开头添加48位值
 *
 * 以大端格式在缓冲区开头添加48位值。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加到缓冲区的48位值。
 */
static inline void net_buf_push_be48(struct net_buf *buf, uint64_t val)
{
	net_buf_simple_push_be48(&buf->b, val);
}
/**
 * @brief 在缓冲区开头添加64位值
 *
 * 以小端格式在缓冲区开头添加64位值。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加到缓冲区的64位值。
 */
static inline void net_buf_push_le64(struct net_buf *buf, uint64_t val)
{
	net_buf_simple_push_le64(&buf->b, val);
}
/**
 * @brief 在缓冲区开头添加64位值
 *
 * 以大端格式在缓冲区开头添加64位值。
 *
 * @param buf 要更新的缓冲区。
 * @param val 要添加到缓冲区的64位值。
 */
static inline void net_buf_push_be64(struct net_buf *buf, uint64_t val)
{
	net_buf_simple_push_be64(&buf->b, val);
}
/**
 * @brief 从缓冲区开头移除数据。
 *
 * 通过修改数据指针和缓冲区长度从缓冲区开头移除数据。
 *
 * @param buf 要更新的缓冲区。
 * @param len 要移除的字节数。
 *
 * @return 缓冲区数据的新开头。
 */
static inline void *net_buf_pull(struct net_buf *buf, size_t len)
{
	return net_buf_simple_pull(&buf->b, len);
}
/**
 * @brief 从缓冲区开头移除数据。
 *
 * 通过修改数据指针和缓冲区长度从缓冲区开头移除数据。
 *
 * @param buf 要更新的缓冲区。
 * @param len 要移除的字节数。
 *
 * @return 指向缓冲区数据旧位置的指针。
 */
static inline void *net_buf_pull_mem(struct net_buf *buf, size_t len)
{
	return net_buf_simple_pull_mem(&buf->b, len);
}
/**
 * @brief 从缓冲区开头移除8位值
 *
 * 与net_buf_pull()相同，但用于操作8位值的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 移除的8位值
 */
static inline uint8_t net_buf_pull_u8(struct net_buf *buf)
{
	return net_buf_simple_pull_u8(&buf->b);
}
/**
 * @brief 从缓冲区开头移除并转换16位值。
 *
 * 与net_buf_pull()相同，但用于操作16位小端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从小端转换为主机端的16位值。
 */
static inline uint16_t net_buf_pull_le16(struct net_buf *buf)
{
	return net_buf_simple_pull_le16(&buf->b);
}
/**
 * @brief 从缓冲区开头移除并转换16位值。
 *
 * 与net_buf_pull()相同，但用于操作16位大端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从大端转换为主机端的16位值。
 */
static inline uint16_t net_buf_pull_be16(struct net_buf *buf)
{
	return net_buf_simple_pull_be16(&buf->b);
}
/**
 * @brief 从缓冲区开头移除并转换24位值。
 *
 * 与net_buf_pull()相同，但用于操作24位小端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从小端转换为主机端的24位值。
 */
static inline uint32_t net_buf_pull_le24(struct net_buf *buf)
{
	return net_buf_simple_pull_le24(&buf->b);
}
/**
 * @brief 从缓冲区开头移除并转换24位值。
 *
 * 与net_buf_pull()相同，但用于操作24位大端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从大端转换为主机端的24位值。
 */
static inline uint32_t net_buf_pull_be24(struct net_buf *buf)
{
	return net_buf_simple_pull_be24(&buf->b);
}
/**
 * @brief 从缓冲区开头移除并转换32位值。
 *
 * 与net_buf_pull()相同，但用于操作32位小端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从小端转换为主机端的32位值。
 */
static inline uint32_t net_buf_pull_le32(struct net_buf *buf)
{
	return net_buf_simple_pull_le32(&buf->b);
}
/**
 * @brief 从缓冲区开头移除并转换32位值。
 *
 * 与net_buf_pull()相同，但用于操作32位大端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从大端转换为主机端的32位值。
 */
static inline uint32_t net_buf_pull_be32(struct net_buf *buf)
{
	return net_buf_simple_pull_be32(&buf->b);
}
/**
 * @brief 从缓冲区开头移除并转换40位值。
 *
 * 与net_buf_pull()相同，但用于操作40位小端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从小端转换为主机端的40位值。
 */
static inline uint64_t net_buf_pull_le40(struct net_buf *buf)
{
	return net_buf_simple_pull_le40(&buf->b);
}
/**
 * @brief 从缓冲区开头移除并转换40位值。
 *
 * 与net_buf_pull()相同，但用于操作40位大端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从大端转换为主机端的40位值。
 */
static inline uint64_t net_buf_pull_be40(struct net_buf *buf)
{
	return net_buf_simple_pull_be40(&buf->b);
}
/**
 * @brief 从缓冲区开头移除并转换48位值。
 *
 * 与net_buf_pull()相同，但用于操作48位小端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从小端转换为主机端的48位值。
 */
static inline uint64_t net_buf_pull_le48(struct net_buf *buf)
{
	return net_buf_simple_pull_le48(&buf->b);
}
/**
 * @brief 从缓冲区开头移除并转换48位值。
 *
 * 与net_buf_pull()相同，但用于操作48位大端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从大端转换为主机端的48位值。
 */
static inline uint64_t net_buf_pull_be48(struct net_buf *buf)
{
	return net_buf_simple_pull_be48(&buf->b);
}
/**
 * @brief 从缓冲区开头移除并转换64位值。
 *
 * 与net_buf_pull()相同，但用于操作64位小端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从小端转换为主机端的64位值。
 */
static inline uint64_t net_buf_pull_le64(struct net_buf *buf)
{
	return net_buf_simple_pull_le64(&buf->b);
}
/**
 * @brief 从缓冲区开头移除并转换64位值。
 *
 * 与net_buf_pull()相同，但用于操作64位大端数据的助手。
 *
 * @param buf 缓冲区的有效指针。
 *
 * @return 从大端转换为主机端的64位值。
 */
static inline uint64_t net_buf_pull_be64(struct net_buf *buf)
{
	return net_buf_simple_pull_be64(&buf->b);
}
/**
 * @brief 检查缓冲区的尾部空间。
 *
 * 检查缓冲区末尾有多少空闲空间。
 *
 * @param buf 缓冲区的有效指针
 *
 * @return 缓冲区末尾可用的字节数。
 */
static inline size_t net_buf_tailroom(const struct net_buf *buf)
{
	return net_buf_simple_tailroom(&buf->b);
}
/**
 * @brief 检查缓冲区的头部空间。
 *
 * 检查缓冲区开头有多少空闲空间。
 *
 * @param buf 缓冲区的有效指针
 *
 * @return 缓冲区开头可用的字节数。
 */
static inline size_t net_buf_headroom(const struct net_buf *buf)
{
	return net_buf_simple_headroom(&buf->b);
}
/**
 * @brief 检查net_buf::len的最大值。
 *
 * 该值取决于保留为头部空间的字节数。
 *
 * @param buf 缓冲区的有效指针
 *
 * @return net_buf::data指针后面可用的字节数。
 */
static inline uint16_t net_buf_max_len(const struct net_buf *buf)
{
	return net_buf_simple_max_len(&buf->b);
}
/**
 * @brief 获取缓冲区的尾指针。
 *
 * 获取指向缓冲区中数据末尾的指针。
 *
 * @param buf 缓冲区。
 *
 * @return 缓冲区的尾指针。
 */
static inline uint8_t *net_buf_tail(const struct net_buf *buf)
{
	return net_buf_simple_tail(&buf->b);
}
/**
 * @brief 查找片段列表中的最后一个片段。
 *
 * @return 指向列表中最后一个片段的指针。
 */
struct net_buf *net_buf_frag_last(struct net_buf *frags);
/**
 * @brief 将新片段插入缓冲区链中。
 *
 * 将新片段插入缓冲区片段列表中父片段之后。
 *
 * 注意：此函数接管片段引用的所有权，因此调用者不需要取消引用。
 *
 * @param parent 父缓冲区/片段。
 * @param frag 要插入的片段。
 */
void net_buf_frag_insert(struct net_buf *parent, struct net_buf *frag);
/**
 * @brief 将新片段添加到缓冲区链的末尾。
 *
 * 将新片段附加到缓冲区片段列表中。
 *
 * 注意：此函数接管片段引用的所有权，因此调用者不需要取消引用。
 *
 * @param head 片段链的头部。
 * @param frag 要添加的片段。
 *
 * @return 片段链的新头部。如果head为非NULL，则返回head；如果head为NULL，则返回frag。
 */
struct net_buf *net_buf_frag_add(struct net_buf *head, struct net_buf *frag);
/**
 * @brief 从缓冲区链中删除现有片段。
 *
 * @param parent 父缓冲区/片段，如果没有父片段，则为NULL。
 * @param frag 要删除的片段。
 *
 * @return 指向片段之后的缓冲区的指针，如果没有更多片段，则返回NULL。
 */
#if defined(CONFIG_NET_BUF_LOG)
struct net_buf *net_buf_frag_del_debug(struct net_buf *parent,
				       struct net_buf *frag,
				       const char *func, int line);
#define net_buf_frag_del(_parent, _frag) \
	net_buf_frag_del_debug(_parent, _frag, __func__, __LINE__)
#else
struct net_buf *net_buf_frag_del(struct net_buf *parent, struct net_buf *frag);
#endif
/**
 * @brief 从net_buf链的偏移量开始将字节复制到线性缓冲区
 *
 * 从@a src net_buf链中复制（提取）@a len字节，从中@a offset开始，复制到线性缓冲区@a dst。返回实际复制的字节数，如果net_buf链没有足够的数据，或者目标缓冲区太小，则可能少于请求的字节数。
 *
 * @param dst 目标缓冲区
 * @param dst_len 目标缓冲区长度
 * @param src 源net_buf链
 * @param offset 开始复制的偏移量
 * @param len 要复制的字节数
 * @return 实际复制的字节数
 */
size_t net_buf_linearize(void *dst, size_t dst_len,
			 const struct net_buf *src, size_t offset, size_t len);
/**
 * @typedef net_buf_allocator_cb
 * @brief 网络缓冲区分配器回调。
 *
 * @details 当net_buf_append_bytes需要分配新的net_buf时调用分配器回调。
 *
 * @param timeout 如果net buf池为空，影响采取的操作。如果是K_NO_WAIT，则立即返回。如果是K_FOREVER，则等待必要的时间。否则，等待指定的超时。
 * @param user_data 在net_buf_append_bytes调用中给定的用户数据。
 * @return 指向分配的net_buf的指针，如果出错则返回NULL。
 */
typedef struct net_buf * __must_check (*net_buf_allocator_cb)(k_timeout_t timeout,
							      void *user_data);
/**
 * @brief 将数据追加到net_buf列表
 *
 * @details 将数据追加到net_buf。如果缓冲区中没有足够的空间，则会添加更多的net_buf，除非没有可用的net_buf并且超时发生。如果未提供分配器，则尝试从与原始缓冲区相同的池中分配。
 *
 * @param buf 网络缓冲区。
 * @param len 输入数据的总长度
 * @param value 要添加的数据
 * @param timeout 超时传递给net_buf分配器回调。
 * @param allocate_cb 当需要新的net_buf时，使用此回调。
 * @param user_data 要提供给allocate_cb的用户数据指针。此指针可以是任何东西，从mem_pool到net_pkt，逻辑由allocate_cb函数决定。
 *
 * @return 实际添加的数据长度。如果使用了除K_FOREVER以外的其他超时，并且池中没有足够的片段来容纳所有数据，则可能少于输入长度。
 */
size_t net_buf_append_bytes(struct net_buf *buf, size_t len,
			    const void *value, k_timeout_t timeout,
			    net_buf_allocator_cb allocate_cb, void *user_data);
/**
 * @brief 将数据与net_buf的内容进行匹配
 *
 * @details 将数据与net_buf的内容进行比较。提供有关两者之间匹配字节数的信息。如果需要，遍历多个缓冲区片段。
 *
 * @param buf 网络缓冲区
 * @param offset 开始比较的偏移量
 * @param data 用于比较的数据缓冲区
 * @param len 要比较的字节数
 *
 * @return 在第一个差异之前比较的字节数。
 */
size_t net_buf_data_match(const struct net_buf *buf, size_t offset, const void *data, size_t len);
/**
 * @brief 跳过net_buf中的N个字节
 *
 * @details 从片段的偏移量开始跳过N个字节。如果数据的总长度放置在多个片段中，此函数将从所有片段中跳过，直到达到N个字节。任何完全跳过的缓冲区将从net_buf列表中删除。
 *
 * @param buf 网络缓冲区。
 * @param len 要跳过的数据总长度。
 *
 * @return 指向片段的指针，或成功跳过后pos为0时返回NULL，否则返回NULL且pos为0xffff。
 */
static inline struct net_buf *net_buf_skip(struct net_buf *buf, size_t len)
{
	while (buf && len--) {
		net_buf_pull_u8(buf);
		if (!buf->len) {
			buf = net_buf_frag_del(NULL, buf);
		}
	}
	return buf;
}
/**
 * @brief 计算片段中存储的字节数。
 *
 * 计算给定缓冲区及其链接的片段中存储的数据总量。
 *
 * @param buf 要开始的缓冲区。
 *
 * @return 缓冲区及其片段中的字节数。
 */
static inline size_t net_buf_frags_len(const struct net_buf *buf)
{
	size_t bytes = 0;
	while (buf) {
		bytes += buf->len;
		buf = buf->frags;
	}
	return bytes;
}
/**
 * @}
 */
#ifdef __cplusplus
}
#endif
#endif /* ZEPHYR_INCLUDE_NET_BUF_H_ */
//GST
