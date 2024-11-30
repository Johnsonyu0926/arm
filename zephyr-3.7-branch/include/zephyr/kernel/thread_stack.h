// zephyr-3.7-branch/include/zephyr/kernel/thread_stack.h

/**
 * @file
 *
 * @brief 声明线程堆栈的宏
 */

/**
 * @brief 线程堆栈API
 * @ingroup kernel_apis
 * @defgroup thread_stack_api 线程堆栈API
 * @{
 * @}
 */

#ifndef ZEPHYR_INCLUDE_KERNEL_THREAD_STACK_H
#define ZEPHYR_INCLUDE_KERNEL_THREAD_STACK_H

#if !defined(_ASMLANGUAGE)
#include <zephyr/arch/cpu.h>
#include <zephyr/sys/util.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 有意使用typedef，这确实是一个不透明的类型。
 *
 * 此数据类型的目的是清楚地区分堆栈的声明符号（类型为k_thread_stack_t）和组成实际由底层线程使用的堆栈数据的底层缓冲区；
 * 它们不能互换使用，因为某些架构在堆栈缓冲区区域之前有保护区，如果写入会触发MPU或MMU故障。
 *
 * 想要处理内部缓冲区的API应继续使用char *。
 *
 * 堆栈应始终使用K_THREAD_STACK_DEFINE()创建。
 */
struct __packed z_thread_stack_element {
	char data;
};

/**
 * @typedef k_thread_stack_t
 * @brief struct z_thread_stack_element的typedef
 *
 * @see z_thread_stack_element
 */

/**
 * @brief 正确对齐CPU堆栈指针值
 *
 * 获取提供的值并将其向下舍入，使其对齐到CPU和ABI要求。这不用于任何内存保护硬件要求。
 *
 * @param ptr 提议的堆栈指针地址
 * @return 正确对齐的堆栈指针地址
 */
static inline char *z_stack_ptr_align(char *ptr)
{
	return (char *)ROUND_DOWN(ptr, ARCH_STACK_PTR_ALIGN);
}
#define Z_STACK_PTR_ALIGN(ptr) ((uintptr_t)z_stack_ptr_align((char *)(ptr)))

/**
 * @brief 获取堆栈帧结构的助手宏
 *
 * 架构定义一个包含在arch_new_thread()中预填充的所有数据成员的结构是非常常见的。
 *
 * 给定一个类型和一个初始堆栈指针，返回一个正确转换为帧结构的指针。
 *
 * @param type 初始堆栈帧结构的类型
 * @param ptr 初始对齐的堆栈指针值
 * @return 堆栈缓冲区内的堆栈帧结构指针
 */
#define Z_STACK_PTR_TO_FRAME(type, ptr) \
	(type *)((ptr) - sizeof(type))

#ifdef ARCH_KERNEL_STACK_RESERVED
#define K_KERNEL_STACK_RESERVED	((size_t)ARCH_KERNEL_STACK_RESERVED)
#else
#define K_KERNEL_STACK_RESERVED	((size_t)0)
#endif /* ARCH_KERNEL_STACK_RESERVED */

#define Z_KERNEL_STACK_SIZE_ADJUST(size) (ROUND_UP(size, \
						   ARCH_STACK_PTR_ALIGN) + \
					  K_KERNEL_STACK_RESERVED)

#ifdef ARCH_KERNEL_STACK_OBJ_ALIGN
#define Z_KERNEL_STACK_OBJ_ALIGN	ARCH_KERNEL_STACK_OBJ_ALIGN
#else
#define Z_KERNEL_STACK_OBJ_ALIGN	ARCH_STACK_PTR_ALIGN
#endif /* ARCH_KERNEL_STACK_OBJ_ALIGN */

#define K_KERNEL_STACK_LEN(size) \
	ROUND_UP(Z_KERNEL_STACK_SIZE_ADJUST(size), Z_KERNEL_STACK_OBJ_ALIGN)

/**
 * @addtogroup thread_stack_api
 * @{
 */

/**
 * @brief 声明线程堆栈的引用
 *
 * 此宏声明当前作用域中定义的线程堆栈的符号。
 *
 * @param sym 线程堆栈符号名称
 * @param size 堆栈内存区域的大小
 */
#define K_KERNEL_STACK_DECLARE(sym, size) \
	extern struct z_thread_stack_element \
		sym[K_KERNEL_STACK_LEN(size)]

/**
 * @brief 声明线程堆栈数组的引用
 *
 * 此宏声明当前作用域中定义的线程堆栈数组的符号。
 *
 * @param sym 线程堆栈符号名称
 * @param nmemb 定义的堆栈数量
 * @param size 堆栈内存区域的大小
 */
#define K_KERNEL_STACK_ARRAY_DECLARE(sym, nmemb, size) \
	extern struct z_thread_stack_element \
		sym[nmemb][K_KERNEL_STACK_LEN(size)]

/**
 * @brief 声明固定线程堆栈数组的引用
 *
 * 此宏声明当前作用域中定义的固定线程堆栈数组的符号。
 *
 * @param sym 线程堆栈符号名称
 * @param nmemb 定义的堆栈数量
 * @param size 堆栈内存区域的大小
 */
#define K_KERNEL_PINNED_STACK_ARRAY_DECLARE(sym, nmemb, size) \
	extern struct z_thread_stack_element \
		sym[nmemb][K_KERNEL_STACK_LEN(size)]

/**
 * @brief 在指定段中定义顶级内核堆栈内存区域
 *
 * 这定义了用于线程堆栈的内存区域在指定的链接器段中。
 *
 * 可以合法地在此定义之前加上'static'关键字。
 *
 * 不合法的是获取sizeof(sym)并将其传递给k_thread_create()的stackSize参数，它可能与'size'参数不同。请使用K_KERNEL_STACK_SIZEOF()。
 *
 * 分配的内存总量可能会增加以适应固定大小的堆栈溢出保护区。
 *
 * @param sym 线程堆栈符号名称
 * @param size 堆栈内存区域的大小
 * @param lsect 此堆栈的链接器段
 */
#define Z_KERNEL_STACK_DEFINE_IN(sym, size, lsect) \
	struct z_thread_stack_element lsect \
		__aligned(Z_KERNEL_STACK_OBJ_ALIGN) \
		sym[K_KERNEL_STACK_LEN(size)]

/**
 * @brief 在指定段中定义顶级内核堆栈内存区域数组
 *
 * @param sym 内核堆栈数组符号名称
 * @param nmemb 要定义的堆栈数量
 * @param size 堆栈内存区域的大小
 * @param lsect 此堆栈数组的链接器段
 */
#define Z_KERNEL_STACK_ARRAY_DEFINE_IN(sym, nmemb, size, lsect) \
	struct z_thread_stack_element lsect \
		__aligned(Z_KERNEL_STACK_OBJ_ALIGN) \
		sym[nmemb][K_KERNEL_STACK_LEN(size)]

/**
 * @brief 定义顶级内核堆栈内存区域
 *
 * 这定义了用于线程堆栈的内存区域，适用于仅在超级用户模式下运行的线程。这也适用于声明用于中断或异常处理的特殊堆栈。
 *
 * 使用此宏定义的堆栈可能不会托管用户模式线程。
 *
 * 可以合法地在此定义之前加上'static'关键字。
 *
 * 不合法的是获取sizeof(sym)并将其传递给k_thread_create()的stackSize参数，它可能与'size'参数不同。请使用K_KERNEL_STACK_SIZEOF()。
 *
 * 分配的内存总量可能会增加以适应固定大小的堆栈溢出保护区。
 *
 * @param sym 线程堆栈符号名称
 * @param size 堆栈内存区域的大小
 */
#define K_KERNEL_STACK_DEFINE(sym, size) \
	Z_KERNEL_STACK_DEFINE_IN(sym, size, __kstackmem)

/**
 * @brief 在固定段中定义顶级内核堆栈内存区域
 *
 * 有关更多信息和约束，请参见K_KERNEL_STACK_DEFINE()。
 *
 * 如果启用了CONFIG_LINKER_USE_PINNED_SECTION，则将堆栈放入固定的noinit链接器段，否则将堆栈放入与K_KERNEL_STACK_DEFINE()相同的段中。
 *
 * @param sym 线程堆栈符号名称
 * @param size 堆栈内存区域的大小
 */
#if defined(CONFIG_LINKER_USE_PINNED_SECTION)
#define K_KERNEL_PINNED_STACK_DEFINE(sym, size) \
	Z_KERNEL_STACK_DEFINE_IN(sym, size, __pinned_noinit)
#else
#define K_KERNEL_PINNED_STACK_DEFINE(sym, size) \
	Z_KERNEL_STACK_DEFINE_IN(sym, size, __kstackmem)
#endif /* CONFIG_LINKER_USE_PINNED_SECTION */

/**
 * @brief 定义顶级内核堆栈内存区域数组
 *
 * 使用此宏定义的堆栈可能不会托管用户模式线程。
 *
 * @param sym 内核堆栈数组符号名称
 * @param nmemb 要定义的堆栈数量
 * @param size 堆栈内存区域的大小
 */
#define K_KERNEL_STACK_ARRAY_DEFINE(sym, nmemb, size) \
	Z_KERNEL_STACK_ARRAY_DEFINE_IN(sym, nmemb, size, __kstackmem)

/**
 * @brief 在固定段中定义顶级内核堆栈内存区域数组
 *
 * 有关更多信息和约束，请参见K_KERNEL_STACK_ARRAY_DEFINE()。
 *
 * 如果启用了CONFIG_LINKER_USE_PINNED_SECTION，则将堆栈放入固定的noinit链接器段，否则将堆栈放入与K_KERNEL_STACK_ARRAY_DEFINE()相同的段中。
 *
 * @param sym 内核堆栈数组符号名称
 * @param nmemb 要定义的堆栈数量
 * @param size 堆栈内存区域的大小
 */
#if defined(CONFIG_LINKER_USE_PINNED_SECTION)
#define K_KERNEL_PINNED_STACK_ARRAY_DEFINE(sym, nmemb, size) \
	Z_KERNEL_STACK_ARRAY_DEFINE_IN(sym, nmemb, size, __pinned_noinit)
#else
#define K_KERNEL_PINNED_STACK_ARRAY_DEFINE(sym, nmemb, size) \
	Z_KERNEL_STACK_ARRAY_DEFINE_IN(sym, nmemb, size, __kstackmem)
#endif /* CONFIG_LINKER_USE_PINNED_SECTION */

/**
 * @brief 定义嵌入式堆栈内存区域
 *
 * 用于嵌入在其他数据结构中的内核堆栈。
 *
 * 使用此宏定义的堆栈可能不会托管用户模式线程。
 * @param sym 线程堆栈符号名称
 * @param size 堆栈内存区域的大小
 */
#define K_KERNEL_STACK_MEMBER(sym, size) \
	Z_KERNEL_STACK_DEFINE_IN(sym, size,)

#define K_KERNEL_STACK_SIZEOF(sym) (sizeof(sym) - K_KERNEL_STACK_RESERVED)

/** @} */

static inline char *K_KERNEL_STACK_BUFFER(k_thread_stack_t *sym)
{
	return (char *)sym + K_KERNEL_STACK_RESERVED;
}
#ifndef CONFIG_USERSPACE
#define K_THREAD_STACK_RESERVED		K_KERNEL_STACK_RESERVED
#define K_THREAD_STACK_SIZEOF		K_KERNEL_STACK_SIZEOF
#define K_THREAD_STACK_LEN		K_KERNEL_STACK_LEN
#define K_THREAD_STACK_DEFINE		K_KERNEL_STACK_DEFINE
#define K_THREAD_STACK_ARRAY_DEFINE	K_KERNEL_STACK_ARRAY_DEFINE
#define K_THREAD_STACK_MEMBER		K_KERNEL_STACK_MEMBER
#define K_THREAD_STACK_BUFFER		K_KERNEL_STACK_BUFFER
#define K_THREAD_STACK_DECLARE		K_KERNEL_STACK_DECLARE
#define K_THREAD_STACK_ARRAY_DECLARE	K_KERNEL_STACK_ARRAY_DECLARE
#define K_THREAD_PINNED_STACK_DEFINE	K_KERNEL_PINNED_STACK_DEFINE
#define K_THREAD_PINNED_STACK_ARRAY_DEFINE \
					K_KERNEL_PINNED_STACK_ARRAY_DEFINE
#else
/**
 * @brief 指示为堆栈对象保留了多少额外内存
 *
 * 任何给定的堆栈声明可能包含额外的内存用于保护区、超级用户模式堆栈或平台特定数据。此宏指示为此保留了多少空间。
 *
 * 此值仅指示在堆栈对象中永久保留的内存。线程堆栈缓冲区中“借用”的内存永远不会在此处计算。
 *
 * 保留的内存位于堆栈对象的开头。保留区域的大小必须适当，以便紧随其后的堆栈缓冲区正确对齐。
 */
#ifdef ARCH_THREAD_STACK_RESERVED
#define K_THREAD_STACK_RESERVED		((size_t)(ARCH_THREAD_STACK_RESERVED))
#else
#define K_THREAD_STACK_RESERVED		((size_t)0U)
#endif /* ARCH_THREAD_STACK_RESERVED */

/**
 * @brief 正确对齐堆栈对象的最低地址
 *
 * 返回堆栈对象最低地址的对齐值，考虑到CPU、ABI和任何内存管理策略施加的所有对齐约束，包括堆栈对象中保留平台数据所需的任何对齐。
 * 这将始终至少为ARCH_STACK_PTR_ALIGN或其整数倍。
 *
 * 根据硬件，这可能是一个固定值或提供的大小的函数。仅当启用CONFIG_MPU_REQUIRES_POWER_OF_TWO_ALIGNMENT时，请求的大小才有意义。
 *
 * 如果启用了CONFIG_USERSPACE，这将确定可由用户模式线程使用的堆栈的对齐方式，或可能稍后降级为用户模式的超级用户模式线程的对齐方式。
 *
 * 架构使用ARCH_THREAD_STACK_OBJ_ALIGN()定义此值。
 *
 * 如果未定义ARCH_THREAD_STACK_OBJ_ALIGN，则假定ARCH_STACK_PTR_ALIGN是合适的。
 *
 * @param size 请求的堆栈缓冲区大小（可能会被忽略）
 * @return 堆栈对象的对齐方式
 */
#if defined(ARCH_THREAD_STACK_OBJ_ALIGN)
#define Z_THREAD_STACK_OBJ_ALIGN(size)	\
	ARCH_THREAD_STACK_OBJ_ALIGN(Z_THREAD_STACK_SIZE_ADJUST(size))
#else
#define Z_THREAD_STACK_OBJ_ALIGN(size)	ARCH_STACK_PTR_ALIGN
#endif /* ARCH_THREAD_STACK_OBJ_ALIGN */

/**
 * @brief 向上舍入请求的堆栈大小以满足约束
 *
 * 给定请求的堆栈缓冲区大小，返回调整后的堆栈对象大小，考虑到：
 *
 * - 平台数据的保留内存
 * - 堆栈缓冲区边界对CPU/ABI约束的对齐
 * - 堆栈缓冲区边界对齐以满足内存管理硬件约束，以便保护区域可以覆盖堆栈缓冲区区域
 *
 * 如果启用了CONFIG_USERSPACE，这将确定可由用户模式线程使用的堆栈对象的大小，或可能稍后降级为用户模式的超级用户模式线程的大小。
 *
 * 架构使用ARCH_THREAD_STACK_SIZE_ADJUST()定义此值。
 *
 * 如果未定义ARCH_THREAD_STACK_SIZE_ADJUST，则假定向上舍入到ARCH_STACK_PTR_ALIGN是合适的。
 *
 * 任何为平台数据保留的内存也包括在返回的总量中。
 *
 * @param size 请求的堆栈缓冲区大小
 * @return 调整后的堆栈对象大小
 */
#if defined(ARCH_THREAD_STACK_SIZE_ADJUST)
#define Z_THREAD_STACK_SIZE_ADJUST(size) \
	ARCH_THREAD_STACK_SIZE_ADJUST((size) + K_THREAD_STACK_RESERVED)
#else
#define Z_THREAD_STACK_SIZE_ADJUST(size) \
	(ROUND_UP((size), ARCH_STACK_PTR_ALIGN) + K_THREAD_STACK_RESERVED)
#endif /* ARCH_THREAD_STACK_SIZE_ADJUST */

/**
 * @addtogroup thread_stack_api
 * @{
 */

/**
 * @brief 声明线程堆栈的引用
 *
 * 此宏声明当前作用域中定义的线程堆栈的符号。
 *
 * @param sym 线程堆栈符号名称
 * @param size 堆栈内存区域的大小
 */
#define K_THREAD_STACK_DECLARE(sym, size) \
	extern struct z_thread_stack_element \
		sym[K_THREAD_STACK_LEN(size)]

/**
 * @brief 声明线程堆栈数组的引用
 *
 * 此宏声明当前作用域中定义的线程堆栈数组的符号。
 *
 * @param sym 线程堆栈符号名称
 * @param nmemb 定义的堆栈数量
 * @param size 堆栈内存区域的大小
 */
#define K_THREAD_STACK_ARRAY_DECLARE(sym, nmemb, size) \
	extern struct z_thread_stack_element \
		sym[nmemb][K_THREAD_STACK_LEN(size)]

/**
 * @brief 返回堆栈内存区域的大小（以字节为单位）
 *
 * 方便宏，用于将所需的堆栈大小传递给k_thread_create()，因为底层实现可能实际上会创建更大的东西（例如保护区）。
 *
 * 此处返回的值不保证与传递给K_THREAD_STACK_DEFINE的'size'参数匹配，可能更大，但始终可以安全地传递给关联的堆栈对象的k_thread_create()。
 *
 * @param sym 堆栈内存符号
 * @return 堆栈缓冲区的大小
 */
#define K_THREAD_STACK_SIZEOF(sym)	(sizeof(sym) - K_THREAD_STACK_RESERVED)

/**
 * @brief 在指定区域中定义顶级线程堆栈内存区域
 *
 * 这定义了适合作为线程堆栈的内存区域在指定区域中。
 *
 * 这是通用的、历史定义。对齐到Z_THREAD_STACK_OBJ_ALIGN并放入'noinit'段，以便在启动时不会清零
 *
 * 定义的符号将始终是k_thread_stack_t，可以传递给k_thread_create()，但不应进行其他操作。如果需要检查内部缓冲区，请检查关联线程对象的thread->stack_info以获取边界。
 *
 * 可以合法地在此定义之前加上'static'关键字。
 *
 * 不合法的是获取sizeof(sym)并将其传递给k_thread_create()的stackSize参数，它可能与'size'参数不同。请使用K_THREAD_STACK_SIZEOF()。
 *
 * 某些架构可能会向上舍入可用堆栈区域的大小以满足对齐约束。K_THREAD_STACK_SIZEOF()将返回对齐后的大小。
 *
 * @param sym 线程堆栈符号名称
 * @param size 堆栈内存区域的大小
 * @param lsect 此堆栈的链接器段
 */
#define Z_THREAD_STACK_DEFINE_IN(sym, size, lsect) \
	struct z_thread_stack_element lsect \
		__aligned(Z_THREAD_STACK_OBJ_ALIGN(size)) \
		sym[K_THREAD_STACK_LEN(size)]

/**
 * @brief 在指定区域中定义顶级线程堆栈内存区域数组
 *
 * 创建一个等大小堆栈的数组。有关其他详细信息和约束，请参见Z_THREAD_STACK_DEFINE_IN定义。
 *
 * 这是通用的、历史定义。对齐到Z_THREAD_STACK_OBJ_ALIGN并放入指定段，以便在启动时不会清零。
 *
 * @param sym 线程堆栈符号名称
 * @param nmemb 要定义的堆栈数量
 * @param size 堆栈内存区域的大小
 * @param lsect 此堆栈的链接器段
 */
#define Z_THREAD_STACK_ARRAY_DEFINE_IN(sym, nmemb, size, lsect) \
	struct z_thread_stack_element lsect \
		__aligned(Z_THREAD_STACK_OBJ_ALIGN(size)) \
		sym[nmemb][K_THREAD_STACK_LEN(size)]
/**
 * @brief 定义顶级线程堆栈内存区域
 *
 * 这定义了适合作为线程堆栈的内存区域。
 *
 * 这是通用的、历史定义。对齐到Z_THREAD_STACK_OBJ_ALIGN并放入'noinit'段，以便在启动时不会清零。
 *
 * 定义的符号将始终是k_thread_stack_t，可以传递给k_thread_create()，但不应进行其他操作。如果需要检查内部缓冲区，请检查关联线程对象的thread->stack_info以获取边界。
 *
 * 可以合法地在此定义之前加上'static'关键字。
 *
 * 不合法的是获取sizeof(sym)并将其传递给k_thread_create()的stackSize参数，它可能与'size'参数不同。请使用K_THREAD_STACK_SIZEOF()。
 *
 * 某些架构可能会向上舍入可用堆栈区域的大小以满足对齐约束。K_THREAD_STACK_SIZEOF()将返回对齐后的大小。
 *
 * @param sym 线程堆栈符号名称
 * @param size 堆栈内存区域的大小
 */
#define K_THREAD_STACK_DEFINE(sym, size) \
	Z_THREAD_STACK_DEFINE_IN(sym, size, __stackmem)
/**
 * @brief 在固定段中定义顶级线程堆栈内存区域
 *
 * 这定义了适合作为线程堆栈的内存区域。
 *
 * 这是通用的、历史定义。对齐到Z_THREAD_STACK_OBJ_ALIGN并放入'noinit'段，以便在启动时不会清零。
 *
 * 定义的符号将始终是k_thread_stack_t，可以传递给k_thread_create()，但不应进行其他操作。如果需要检查内部缓冲区，请检查关联线程对象的thread->stack_info以获取边界。
 *
 * 可以合法地在此定义之前加上'static'关键字。
 *
 * 不合法的是获取sizeof(sym)并将其传递给k_thread_create()的stackSize参数，它可能与'size'参数不同。请使用K_THREAD_STACK_SIZEOF()。
 *
 * 某些架构可能会向上舍入可用堆栈区域的大小以满足对齐约束。K_THREAD_STACK_SIZEOF()将返回对齐后的大小。
 *
 * 如果启用了CONFIG_LINKER_USE_PINNED_SECTION，则将堆栈放入固定的noinit链接器段，否则将堆栈放入与K_THREAD_STACK_DEFINE()相同的段中。
 *
 * @param sym 线程堆栈符号名称
 * @param size 堆栈内存区域的大小
 */
#if defined(CONFIG_LINKER_USE_PINNED_SECTION)
#define K_THREAD_PINNED_STACK_DEFINE(sym, size) \
	Z_THREAD_STACK_DEFINE_IN(sym, size, __pinned_noinit)
#else
#define K_THREAD_PINNED_STACK_DEFINE(sym, size) \
	K_THREAD_STACK_DEFINE(sym, size)
#endif /* CONFIG_LINKER_USE_PINNED_SECTION */
/**
 * @brief 计算要在堆栈数组中分配的堆栈大小
 *
 * 此宏计算要在堆栈数组中分配的堆栈大小。它接受指示的“size”作为参数，并在需要时填充一些额外的字节（例如用于MPU场景）。请参见K_THREAD_STACK_ARRAY_DEFINE定义以了解如何使用此宏。
 * 返回的大小确保每个数组成员将对齐到所需的堆栈基准对齐。
 *
 * @param size 堆栈内存区域的大小
 * @return 数组成员的适当大小
 */
#define K_THREAD_STACK_LEN(size) \
	ROUND_UP(Z_THREAD_STACK_SIZE_ADJUST(size), \
		 Z_THREAD_STACK_OBJ_ALIGN(size))
/**
 * @brief 定义顶级线程堆栈内存区域数组
 *
 * 创建一个等大小的堆栈数组。有关其他详细信息和约束，请参见K_THREAD_STACK_DEFINE定义。
 *
 * 这是通用的、历史定义。对齐到Z_THREAD_STACK_OBJ_ALIGN并放入'noinit'段，以便在启动时不会清零。
 *
 * @param sym 线程堆栈符号名称
 * @param nmemb 要定义的堆栈数量
 * @param size 堆栈内存区域的大小
 */
#define K_THREAD_STACK_ARRAY_DEFINE(sym, nmemb, size) \
	Z_THREAD_STACK_ARRAY_DEFINE_IN(sym, nmemb, size, __stackmem)
/**
 * @brief 在固定段中定义顶级线程堆栈内存区域数组
 *
 * 创建一个等大小的堆栈数组。有关其他详细信息和约束，请参见K_THREAD_STACK_DEFINE定义。
 *
 * 这是通用的、历史定义。对齐到Z_THREAD_STACK_OBJ_ALIGN并放入'noinit'段，以便在启动时不会清零。
 *
 * 如果启用了CONFIG_LINKER_USE_PINNED_SECTION，则将堆栈放入固定的noinit链接器段，否则将堆栈放入与K_THREAD_STACK_DEFINE()相同的段中。
 *
 * @param sym 线程堆栈符号名称
 * @param nmemb 要定义的堆栈数量
 * @param size 堆栈内存区域的大小
 */
#if defined(CONFIG_LINKER_USE_PINNED_SECTION)
#define K_THREAD_PINNED_STACK_ARRAY_DEFINE(sym, nmemb, size) \
	Z_THREAD_STACK_ARRAY_DEFINE_IN(sym, nmemb, size, __pinned_noinit)
#else
#define K_THREAD_PINNED_STACK_ARRAY_DEFINE(sym, nmemb, size) \
	K_THREAD_STACK_ARRAY_DEFINE(sym, nmemb, size)
#endif /* CONFIG_LINKER_USE_PINNED_SECTION */
/**
 * @brief 定义嵌入式堆栈内存区域
 *
 * 用于嵌入在其他数据结构中的堆栈。强烈不建议使用，但在某些情况下是必要的。对于内存保护场景，非常重要的是，任何位于此成员之前的RAM都不能被线程写入，否则堆栈溢出将导致静默损坏。换句话说，包含的数据结构应位于内核拥有的RAM中。
 *
 * 只有在启动线程的线程处于超级用户模式时，才能使用这种方式定义的堆栈启动用户线程。
 *
 * @deprecated 现在已弃用，因为以这种方式定义的堆栈不能从用户模式使用。请使用K_KERNEL_STACK_MEMBER。
 *
 * @param sym 线程堆栈符号名称
 * @param size 堆栈内存区域的大小
 */
#define K_THREAD_STACK_MEMBER(sym, size) __DEPRECATED_MACRO \
	Z_THREAD_STACK_DEFINE_IN(sym, size,)
/** @} */
/**
 * @brief 获取物理堆栈缓冲区的指针
 *
 * 获取指向堆栈对象的非保留区域的指针。这不保证是线程可写区域的起始位置；这不考虑为MPU堆栈溢出保护区划出的任何内存。
 *
 * 请谨慎使用。堆栈缓冲区的真实边界可在其关联线程的stack_info成员中找到。
 *
 * @param sym 定义的堆栈符号名称
 * @return 缓冲区本身，一个char *
 */
static inline char *K_THREAD_STACK_BUFFER(k_thread_stack_t *sym)
{
	return (char *)sym + K_THREAD_STACK_RESERVED;
}
#endif /* CONFIG_USERSPACE */
#ifdef __cplusplus
}
#endif
#endif /* _ASMLANGUAGE */
#endif /* ZEPHYR_INCLUDE_KERNEL_THREAD_STACK_H */
// By GST @Data