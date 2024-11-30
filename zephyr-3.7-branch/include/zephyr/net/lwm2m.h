// zephyr-3.7-branch/include/zephyr/net/lwm2m.h

/** @file lwm2m.h
 *
 * @brief LwM2M高层API
 *
 * @details
 * LwM2M高层接口在此头文件中定义。
 *
 * @note 实现假定启用了UDP模块。
 *
 * @note 有关更多信息，请参阅技术规范
 * OMA-TS-LightweightM2M_Core-V1_1_1-20190617-A
 *
 * @defgroup lwm2m_api LwM2M高层API
 * @since 1.9
 * @version 0.8.0
 * @ingroup networking
 * @{
 */

#ifndef ZEPHYR_INCLUDE_NET_LWM2M_H_
#define ZEPHYR_INCLUDE_NET_LWM2M_H_

#include <time.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/lwm2m_path.h>

/**
 * @name OMA管理的LwM2M对象。LwM2M技术规范中的对象。
 * 对象ID范围为0到1023。
 * @{
 */

/* clang-format off */
#define LWM2M_OBJECT_SECURITY_ID                0  /**< 安全对象 */
#define LWM2M_OBJECT_SERVER_ID                  1  /**< 服务器对象 */
#define LWM2M_OBJECT_ACCESS_CONTROL_ID          2  /**< 访问控制对象 */
#define LWM2M_OBJECT_DEVICE_ID                  3  /**< 设备对象 */
#define LWM2M_OBJECT_CONNECTIVITY_MONITORING_ID 4  /**< 连接监控对象 */
#define LWM2M_OBJECT_FIRMWARE_ID                5  /**< 固件对象 */
#define LWM2M_OBJECT_LOCATION_ID                6  /**< 位置对象 */
#define LWM2M_OBJECT_CONNECTIVITY_STATISTICS_ID 7  /**< 连接统计对象 */
#define LWM2M_OBJECT_SOFTWARE_MANAGEMENT_ID     9  /**< 软件管理对象 */
#define LWM2M_OBJECT_PORTFOLIO_ID               16 /**< 投资组合对象 */
#define LWM2M_OBJECT_BINARYAPPDATACONTAINER_ID  19 /**< 二进制应用数据容器对象 */
#define LWM2M_OBJECT_EVENT_LOG_ID               20 /**< 事件日志对象 */
#define LWM2M_OBJECT_OSCORE_ID                  21 /**< OSCORE对象 */
#define LWM2M_OBJECT_GATEWAY_ID                 25 /**< 网关对象 */
/* clang-format on */

/** @} */

/**
 * @name 由第三方标准开发组织生产的LwM2M对象。
 * 请参阅OMA LightweightM2M（LwM2M）对象和资源注册表：
 * http://www.openmobilealliance.org/wp/OMNA/LwM2M/LwM2MRegistry.html
 * @{
 */

/* clang-format off */
#define IPSO_OBJECT_GENERIC_SENSOR_ID       3300 /**< IPSO通用传感器对象 */
#define IPSO_OBJECT_TEMP_SENSOR_ID          3303 /**< IPSO温度传感器对象 */
#define IPSO_OBJECT_HUMIDITY_SENSOR_ID      3304 /**< IPSO湿度传感器对象 */
#define IPSO_OBJECT_LIGHT_CONTROL_ID        3311 /**< IPSO灯光控制对象 */
#define IPSO_OBJECT_ACCELEROMETER_ID        3313 /**< IPSO加速度计对象 */
#define IPSO_OBJECT_VOLTAGE_SENSOR_ID       3316 /**< IPSO电压传感器对象 */
#define IPSO_OBJECT_CURRENT_SENSOR_ID       3317 /**< IPSO电流传感器对象 */
#define IPSO_OBJECT_PRESSURE_ID             3323 /**< IPSO压力传感器对象 */
#define IPSO_OBJECT_BUZZER_ID               3338 /**< IPSO蜂鸣器对象 */
#define IPSO_OBJECT_TIMER_ID                3340 /**< IPSO定时器对象 */
#define IPSO_OBJECT_ONOFF_SWITCH_ID         3342 /**< IPSO开关对象 */
#define IPSO_OBJECT_PUSH_BUTTON_ID          3347 /**< IPSO按钮对象 */
#define UCIFI_OBJECT_BATTERY_ID             3411 /**< uCIFI电池对象 */
#define IPSO_OBJECT_FILLING_LEVEL_SENSOR_ID 3435 /**< IPSO填充液位传感器对象 */
/* clang-format on */

/** @} */

/**
 * @brief 当遇到套接字错误时调用的回调函数
 *
 * @param error 错误代码
 */
typedef void (*lwm2m_socket_fault_cb_t)(int error);

/** @brief LwM2M对象路径结构 */
struct lwm2m_obj_path {
	uint16_t obj_id;         /**< 对象ID */
	uint16_t obj_inst_id;    /**< 对象实例ID */
	uint16_t res_id;         /**< 资源ID */
	uint16_t res_inst_id;    /**< 资源实例ID */
	uint8_t  level;          /**< 路径级别（0-4）。例如，4 = 资源实例。 */
};

/**
 * @brief 观察回调事件
 */
enum lwm2m_observe_event {
	LWM2M_OBSERVE_EVENT_OBSERVER_ADDED,    /**< 添加观察者 */
	LWM2M_OBSERVE_EVENT_OBSERVER_REMOVED,  /**< 删除观察者 */
	LWM2M_OBSERVE_EVENT_NOTIFY_ACK,        /**< 通知已确认 */
	LWM2M_OBSERVE_EVENT_NOTIFY_TIMEOUT,    /**< 通知超时 */
};

/**
 * @brief 观察回调，指示观察者添加和删除，以及通知确认和超时
 *
 * @param[in] event 观察者添加/删除或通知确认/超时
 * @param[in] path LwM2M路径
 * @param[in] user_data 指向用户数据缓冲区的指针，如在send_traceable_notification()中提供。用于确定已确认/超时通知的数据。
 */
typedef void (*lwm2m_observe_cb_t)(enum lwm2m_observe_event event, struct lwm2m_obj_path *path,
				   void *user_data);

struct lwm2m_ctx;
enum lwm2m_rd_client_event;
/**
 * @brief 异步RD客户端事件回调
 *
 * @param[in] ctx 生成事件的LwM2M上下文
 * @param[in] event LwM2M RD客户端事件代码
 */
typedef void (*lwm2m_ctx_event_cb_t)(struct lwm2m_ctx *ctx,
				     enum lwm2m_rd_client_event event);

/**
 * @brief LwM2M套接字的不同流量状态。
 *
 * 此信息可用于为网络接口提供提示，网络接口可以决定应使用哪种电源管理。
 *
 * 这些提示来自CoAP层消息，因此使用DTLS可能会影响预期数据报的实际数量。
 */
enum lwm2m_socket_states {
	LWM2M_SOCKET_STATE_ONGOING,	 /**< 预期有持续的流量。 */
	LWM2M_SOCKET_STATE_ONE_RESPONSE, /**< 预期下一个消息有一个响应。 */
	LWM2M_SOCKET_STATE_LAST,	 /**< 下一个消息是最后一个。 */
	LWM2M_SOCKET_STATE_NO_DATA,	 /**< 不再预期有数据。 */
};

/**
 * @brief LwM2M上下文结构，用于维护单个LwM2M连接的信息。
 */
struct lwm2m_ctx {
	/** 目标地址存储 */
	struct sockaddr remote_addr;

	/** @cond INTERNAL_HIDDEN
	 * 私有CoAP和网络结构 + 1用于RD客户端自己的消息
	 */
	struct coap_pending pendings[CONFIG_LWM2M_ENGINE_MAX_PENDING + 1];
	struct coap_reply replies[CONFIG_LWM2M_ENGINE_MAX_REPLIES + 1];
	sys_slist_t pending_sends;
#if defined(CONFIG_LWM2M_QUEUE_MODE_ENABLED)
	sys_slist_t queued_messages;
#endif
	sys_slist_t observer;
	/** @endcond */

	/** 指向当前处理的请求的指针，用于内部LwM2M引擎使用。底层类型是``struct lwm2m_message``，但由于它在私有头文件中声明，不向应用程序公开，因此存储为void指针。 */
	void *processed_req;

#if defined(CONFIG_LWM2M_DTLS_SUPPORT) || defined(__DOXYGEN__)
	/**
	 * @name DTLS相关信息
	 * 仅在启用@kconfig{CONFIG_LWM2M_DTLS_SUPPORT}并设置@ref lwm2m_ctx.use_dtls为true时可用。
	 * @{
	 */
	/** TLS标签由客户端设置，作为LwM2M引擎调用tls_credential_(add|delete)时的参考 */
	int tls_tag;

	/** 目标主机名。
	 *  启用MBEDTLS SNI时，必须使用目标服务器主机名设置套接字。
	 */
	char *desthostname;
	/** 目标主机名长度 */
	uint16_t desthostnamelen;
	/** 指示是否启用主机名验证的标志 */
	bool hostname_verify;

	/** 自定义加载凭据函数。
	 *  客户端可以设置load_credentials函数，以覆盖lwm2m_engine.c中的load_tls_credential()的默认行为
	 */
	int (*load_credentials)(struct lwm2m_ctx *client_ctx);
	/** @} */
#endif
	/** 自定义套接字选项。
	 * 客户端可以通过提供回调来覆盖默认套接字选项，该回调在创建套接字后和连接之前调用。
	 */
	int (*set_socketoptions)(struct lwm2m_ctx *client_ctx);

	/** 指示上下文是否应使用DTLS的标志。
	 *  通过在连接信息中使用coaps://协议前缀启用。
	 *  注意：需要@kconfig{CONFIG_LWM2M_DTLS_SUPPORT}
	 */
	bool use_dtls;

	/**
	 * 指示套接字连接是否挂起的标志。
	 * 使用队列模式时，这将指示是否需要重新连接。
	 */
	bool connection_suspended;

#if defined(CONFIG_LWM2M_QUEUE_MODE_ENABLED) || defined(__DOXYGEN__)
	/**
	 * 指示客户端是否正在缓冲通知和发送消息的标志。
	 * true值缓冲通知和发送消息。
	 */
	bool buffer_client_messages;
#endif
	/** 用于服务器凭据的安全对象的当前索引 */
	int sec_obj_inst;

	/** 此上下文中使用的服务器对象的当前索引。 */
	int srv_obj_inst;

	/** 启用BOOTSTRAP接口的标志。有关更多信息，请参阅LwM2M技术规范的“引导接口”部分。 */
	bool bootstrap_mode;

	/** 套接字文件描述符 */
	int sock_fd;

	/** 套接字故障回调。在接收时遇到套接字错误时，LwM2M处理线程将调用此回调。 */
	lwm2m_socket_fault_cb_t fault_cb;

	/** 新观察者或取消观察者的回调，以及已确认或超时的通知。 */
	lwm2m_observe_cb_t observe_cb;

	/** 客户端事件的回调 */
	lwm2m_ctx_event_cb_t event_cb;

	/** 验证缓冲区。用于在验证资源值之前解码资源值的临时缓冲区。验证成功后，其内容将复制到实际资源缓冲区中。 */
	uint8_t validate_buf[CONFIG_LWM2M_ENGINE_VALIDATION_BUFFER_SIZE];

	/**
	 * 指示传输状态的回调。
	 * 客户端应用程序可以请求LwM2M引擎指示传输状态的提示，并使用该信息控制各种节能模式。
	 */
	void (*set_socket_state)(int fd, enum lwm2m_socket_states state);
};

/**
 * LwM2M时间序列数据结构
 */
struct lwm2m_time_series_elem {
	/** 缓存数据的Unix时间戳 */
	time_t t;
	/** 元素值 */
	union {
		/** @cond INTERNAL_HIDDEN */
		uint8_t u8;
		uint16_t u16;
		uint32_t u32;
		uint64_t u64;
		int8_t i8;
		int16_t i16;
		int32_t i32;
		int64_t i64;
		time_t time;
		double f;
		bool b;
		/** @endcond */
	};
};

/**
 * @brief 异步回调以获取资源缓冲区和长度。
 *
 * 在访问资源的数据缓冲区之前，引擎可以使用此回调获取缓冲区指针和长度，而不是使用资源的数据缓冲区。
 *
 * 客户端或LwM2M对象可以通过以下方式注册此类型的函数：
 * lwm2m_register_read_callback()
 * lwm2m_register_pre_write_callback()
 *
 * @param[in] obj_inst_id 生成回调的对象实例ID。
 * @param[in] res_id 生成回调的资源ID。
 * @param[in] res_inst_id 生成回调的资源实例ID（通常为0，非多实例资源）。
 * @param[out] data_len 数据缓冲区的长度。
 *
 * @return 回调返回指向数据缓冲区的指针，失败时返回NULL。
 */
typedef void *(*lwm2m_engine_get_data_cb_t)(uint16_t obj_inst_id,
					    uint16_t res_id,
					    uint16_t res_inst_id,
					    size_t *data_len);

/**
 * @brief 异步回调，当数据已设置到资源缓冲区时。
 *
 * 更改资源缓冲区的数据后，LwM2M引擎可以使用此回调将数据传递回客户端或LwM2M对象。
 *
 * 在块传输中，处理程序会多次调用数据块并增加偏移量。最后一个块的last_block标志设置为true。
 * 块传输的开始偏移量设置为0。
 *
 * 可以通过以下方式注册此类型的函数：
 * lwm2m_register_validate_callback()
 * lwm2m_register_post_write_callback()
 *
 * @param[in] obj_inst_id 生成回调的对象实例ID。
 * @param[in] res_id 生成回调的资源ID。
 * @param[in] res_inst_id 生成回调的资源实例ID（通常为0，非多实例资源）。
 * @param[in] data 指向数据的指针。
 * @param[in] data_len 数据的长度。
 * @param[in] last_block 在块传输期间用于指示最后一个数据块的标志。对于非块传输，此标志始终为false。
 * @param[in] total_size 块传输的预期总数据大小。对于非块传输，此值为0。
 * @param[in] offset 数据块的偏移量。对于非块传输，此值始终为0。
 *
 * @return 回调返回负错误代码（errno.h），指示失败原因，成功返回0。
 */
typedef int (*lwm2m_engine_set_data_cb_t)(uint16_t obj_inst_id,
					  uint16_t res_id, uint16_t res_inst_id,
					  uint8_t *data, uint16_t data_len,
					  bool last_block, size_t total_size, size_t offset);

/**
 * @brief 异步事件通知回调。
 *
 * LwM2M引擎中的各种对象实例和资源事件可以触发此类型的回调：对象实例创建和对象实例删除。
 *
 * 可以通过以下方式注册此类型的函数：
 * lwm2m_register_create_callback()
 * lwm2m_register_delete_callback()
 *
 * @param[in] obj_inst_id 生成回调的对象实例ID。
 *
 * @return 回调返回负错误代码（errno.h），指示失败原因，成功返回0。
 */
typedef int (*lwm2m_engine_user_cb_t)(uint16_t obj_inst_id);

/**
 * @brief 异步执行通知回调。
 *
 * 资源执行触发此类型的回调。
 *
 * 可以通过以下方式注册此类型的函数：
 * lwm2m_register_exec_callback()
 *
 * @param[in] obj_inst_id 生成回调的对象实例ID。
 * @param[in] args 指向执行参数有效负载的指针。（如果未提供参数，则可以为NULL）
 * @param[in] args_len 参数有效负载的长度（字节）。
 *
 * @return 回调返回负错误代码（errno.h），指示失败原因，成功返回0。
 */
typedef int (*lwm2m_engine_execute_cb_t)(uint16_t obj_inst_id,
					 uint8_t *args, uint16_t args_len);

/**
 * @name 用于LwM2M设备对象（3/0/6）的“可用电源源”资源的电源源类型。
 * @{
 */
#define LWM2M_DEVICE_PWR_SRC_TYPE_DC_POWER	0 /**< 直流电源 */
#define LWM2M_DEVICE_PWR_SRC_TYPE_BAT_INT	1 /**< 内部电池 */
#define LWM2M_DEVICE_PWR_SRC_TYPE_BAT_EXT	2 /**< 外部电池 */
#define LWM2M_DEVICE_PWR_SRC_TYPE_FUEL_CELL	3 /**< 燃料电池 */
#define LWM2M_DEVICE_PWR_SRC_TYPE_PWR_OVER_ETH	4 /**< 以太网供电 */
#define LWM2M_DEVICE_PWR_SRC_TYPE_USB		5 /**< USB */
#define LWM2M_DEVICE_PWR_SRC_TYPE_AC_POWER	6 /**< 交流电源 */
#define LWM2M_DEVICE_PWR_SRC_TYPE_SOLAR		7 /**< 太阳能 */
#define LWM2M_DEVICE_PWR_SRC_TYPE_MAX		8 /**< 可用电源源类型的最大值 */
/** @} */

/**
 * @name 用于LwM2M设备对象的“错误代码”资源的错误代码。
 * LwM2M客户端可以通过lwm2m_device_add_err()函数注册以下错误代码之一。
 * @{
 */
#define LWM2M_DEVICE_ERROR_NONE			0 /**< 无错误 */
#define LWM2M_DEVICE_ERROR_LOW_POWER		1 /**< 电池电量低 */
#define LWM2M_DEVICE_ERROR_EXT_POWER_SUPPLY_OFF	2 /**< 外部电源关闭 */
#define LWM2M_DEVICE_ERROR_GPS_FAILURE		3 /**< GPS模块故障 */
#define LWM2M_DEVICE_ERROR_LOW_SIGNAL_STRENGTH	4 /**< 接收信号强度低 */
#define LWM2M_DEVICE_ERROR_OUT_OF_MEMORY	5 /**< 内存不足 */
#define LWM2M_DEVICE_ERROR_SMS_FAILURE		6 /**< SMS故障 */
#define LWM2M_DEVICE_ERROR_NETWORK_FAILURE	7 /**< IP连接故障 */
#define LWM2M_DEVICE_ERROR_PERIPHERAL_FAILURE	8 /**< 外围设备故障 */

/** @} */

/**
 * @name 用于LwM2M设备对象（3/0/20）的“电池状态”资源的电池状态代码。
 * 随着电池状态的变化，LwM2M客户端可以通过以下方式设置以下代码之一：
 * lwm2m_set_u8("3/0/20", [电池状态])
 * @{
 */
#define LWM2M_DEVICE_BATTERY_STATUS_NORMAL	0 /**< 电池正常工作且未充电 */
#define LWM2M_DEVICE_BATTERY_STATUS_CHARGING	1 /**< 电池正在充电 */
#define LWM2M_DEVICE_BATTERY_STATUS_CHARGE_COMP	2 /**< 电池已充满电且充电器仍连接 */
#define LWM2M_DEVICE_BATTERY_STATUS_DAMAGED	3 /**< 电池有问题 */
#define LWM2M_DEVICE_BATTERY_STATUS_LOW		4 /**< 电池电量低 */
#define LWM2M_DEVICE_BATTERY_STATUS_NOT_INST	5 /**< 电池未安装 */
#define LWM2M_DEVICE_BATTERY_STATUS_UNKNOWN	6 /**< 电池信息不可用 */
/** @} */
/**
 * @brief 向LwM2M设备对象注册新的错误代码。
 *
 * @param[in] error_code 新的错误代码。
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_device_add_err(uint8_t error_code);
/**
 * @name LWM2M固件更新对象状态
 *
 * LwM2M客户端或LwM2M固件更新对象使用以下代码表示LwM2M固件更新状态（5/0/3）。
 * @{
 */
/**
 * 空闲。在下载之前或成功更新之后。
 */
#define STATE_IDLE		0
/**
 * 下载中。数据序列正在下载。
 */
#define STATE_DOWNLOADING	1
/**
 * 已下载。整个数据序列已下载。
 */
#define STATE_DOWNLOADED	2
/**
 * 更新中。设备正在更新。
 */
#define STATE_UPDATING		3
/** @} */
/**
 * @name LWM2M固件更新对象结果代码
 *
 * 处理固件更新后，客户端通过以下代码之一设置结果：
 * lwm2m_set_u8("5/0/5", [结果代码])
 * @{
 */
#define RESULT_DEFAULT		0	/**< 初始值 */
#define RESULT_SUCCESS		1	/**< 固件更新成功 */
#define RESULT_NO_STORAGE	2	/**< 新固件包的闪存不足 */
#define RESULT_OUT_OF_MEM	3	/**< 下载过程中内存不足 */
#define RESULT_CONNECTION_LOST	4	/**< 下载过程中连接丢失 */
#define RESULT_INTEGRITY_FAILED	5	/**< 新下载包的完整性检查失败 */
#define RESULT_UNSUP_FW		6	/**< 不支持的包类型 */
#define RESULT_INVALID_URI	7	/**< 无效的URI */
#define RESULT_UPDATE_FAILED	8	/**< 固件更新失败 */
#define RESULT_UNSUP_PROTO	9	/**< 不支持的协议 */
/** @} */
#if defined(CONFIG_LWM2M_FIRMWARE_UPDATE_OBJ_SUPPORT) || defined(__DOXYGEN__)
/**
 * @brief 设置固件块传输的数据回调。
 *
 * LwM2M客户端使用此函数注册回调，以在执行固件更新时接收块传输数据。
 *
 * @param[in] cb 接收块传输数据的回调函数
 */
void lwm2m_firmware_set_write_cb(lwm2m_engine_set_data_cb_t cb);
/**
 * @brief 获取固件块传输写入的数据回调。
 *
 * @return 注册的接收块传输数据的回调函数
 */
lwm2m_engine_set_data_cb_t lwm2m_firmware_get_write_cb(void);
/**
 * @brief 设置固件块传输的数据回调。
 *
 * LwM2M客户端使用此函数注册回调，以在执行固件更新时接收块传输数据。
 *
 * @param[in] obj_inst_id 对象实例ID
 * @param[in] cb 接收块传输数据的回调函数
 */
void lwm2m_firmware_set_write_cb_inst(uint16_t obj_inst_id, lwm2m_engine_set_data_cb_t cb);
/**
 * @brief 获取固件块传输写入的数据回调。
 *
 * @param[in] obj_inst_id 对象实例ID
 * @return 注册的接收块传输数据的回调函数
 */
lwm2m_engine_set_data_cb_t lwm2m_firmware_get_write_cb_inst(uint16_t obj_inst_id);
/**
 * @brief 设置固件更新取消的回调。
 *
 * LwM2M客户端使用此函数注册回调，以在固件更新取消时执行操作。
 *
 * @param[in] cb 在固件更新取消时执行操作的回调函数。
 */
void lwm2m_firmware_set_cancel_cb(lwm2m_engine_user_cb_t cb);
/**
 * @brief 获取固件更新取消的回调。
 *
 * @return 注册的在固件更新取消时执行操作的回调函数。
 */
lwm2m_engine_user_cb_t lwm2m_firmware_get_cancel_cb(void);
/**
 * @brief 设置固件更新取消的数据回调。
 *
 * LwM2M客户端使用此函数注册回调，以在固件更新取消时执行操作。
 *
 * @param[in] obj_inst_id 对象实例ID
 * @param[in] cb 在固件更新取消时执行操作的回调函数。
 */
void lwm2m_firmware_set_cancel_cb_inst(uint16_t obj_inst_id, lwm2m_engine_user_cb_t cb);
/**
 * @brief 获取固件更新取消的回调。
 *
 * @param[in] obj_inst_id 对象实例ID
 * @return 注册的在固件更新取消时执行操作的回调函数。
 */
lwm2m_engine_user_cb_t lwm2m_firmware_get_cancel_cb_inst(uint16_t obj_inst_id);
/**
 * @brief 设置处理固件更新执行事件的数据回调。
 *
 * LwM2M客户端使用此函数注册回调，以在LwM2M固件更新对象上接收更新资源的“执行”操作。
 *
 * @param[in] cb 接收执行事件的回调函数。
 */
void lwm2m_firmware_set_update_cb(lwm2m_engine_execute_cb_t cb);
/**
 * @brief 获取固件更新执行事件的回调。
 *
 * @return 注册的接收执行事件的回调函数。
 */
lwm2m_engine_execute_cb_t lwm2m_firmware_get_update_cb(void);
/**
 * @brief 设置处理固件更新执行事件的数据回调。
 *
 * LwM2M客户端使用此函数注册回调，以在LwM2M固件更新对象上接收更新资源的“执行”操作。
 *
 * @param[in] obj_inst_id 对象实例ID
 * @param[in] cb 接收执行事件的回调函数。
 */
void lwm2m_firmware_set_update_cb_inst(uint16_t obj_inst_id, lwm2m_engine_execute_cb_t cb);
/**
 * @brief 获取固件更新执行事件的回调。
 *
 * @param[in] obj_inst_id 对象实例ID
 * @return 注册的接收执行事件的回调函数。
 */
lwm2m_engine_execute_cb_t lwm2m_firmware_get_update_cb_inst(uint16_t obj_inst_id);
#endif
#if defined(CONFIG_LWM2M_SWMGMT_OBJ_SUPPORT) || defined(__DOXYGEN__)
/**
 * @brief 设置处理软件激活请求的回调
 *
 * 当LWM2M执行操作在相应对象的激活资源实例上调用时，将执行回调。
 *
 * @param[in] obj_inst_id 要设置回调的实例编号。
 * @param[in] cb 接收执行事件的回调函数。
 *
 * @return 成功返回0，否则返回负整数。
 */
int lwm2m_swmgmt_set_activate_cb(uint16_t obj_inst_id, lwm2m_engine_execute_cb_t cb);
/**
 * @brief 设置处理软件停用请求的回调
 *
 * 当LWM2M执行操作在相应对象的停用资源实例上调用时，将执行回调。
 *
 * @param[in] obj_inst_id 要设置回调的实例编号。
 * @param[in] cb 接收执行事件的回调函数。
 *
 * @return 成功返回0，否则返回负整数。
 */
int lwm2m_swmgmt_set_deactivate_cb(uint16_t obj_inst_id, lwm2m_engine_execute_cb_t cb);
/**
 * @brief 设置处理软件安装请求的回调
 *
 * 当LWM2M执行操作在相应对象的安装资源实例上调用时，将执行回调。
 *
 * @param[in] obj_inst_id 要设置回调的实例编号。
 * @param[in] cb 接收执行事件的回调函数。
 *
 * @return 成功返回0，否则返回负整数。
 */
int lwm2m_swmgmt_set_install_package_cb(uint16_t obj_inst_id, lwm2m_engine_execute_cb_t cb);
/**
 * @brief 设置处理软件卸载请求的回调
 *
 * 当LWM2M执行操作在相应对象的卸载资源实例上调用时，将执行回调。
 *
 * @param[in] obj_inst_id 要设置回调的实例编号。
 * @param[in] cb 处理执行事件的回调函数。
 *
 * @return 成功返回0，否则返回负整数。
 */
int lwm2m_swmgmt_set_delete_package_cb(uint16_t obj_inst_id, lwm2m_engine_execute_cb_t cb);
/**
 * @brief 设置读取软件包的回调
 *
 * 当LWM2M读取操作在相应对象上调用时，将执行回调。
 *
 * @param[in] obj_inst_id 要设置回调的实例编号。
 * @param[in] cb 处理读取事件的回调函数。
 *
 * @return 成功返回0，否则返回负整数。
 */
int lwm2m_swmgmt_set_read_package_version_cb(uint16_t obj_inst_id, lwm2m_engine_get_data_cb_t cb);
/**
 * @brief 设置软件管理块传输的数据回调。
 *
 * 当LWM2M块写入操作在相应对象的资源实例上调用时，将执行回调。
 *
 * @param[in] obj_inst_id 要设置回调的实例编号。
 * @param[in] cb 处理块写入事件的回调函数。
 *
 * @return 成功返回0，否则返回负整数。
 */
int lwm2m_swmgmt_set_write_package_cb(uint16_t obj_inst_id, lwm2m_engine_set_data_cb_t cb);
/**
 * 当软件管理对象实例完成安装操作时调用的函数。
 *
 * @param[in] obj_inst_id 软件管理对象实例
 * @param[in] error_code 操作的结果代码。成功时为零，否则应为负整数。
 *
 * @return 成功返回0，否则返回负整数。
 */
int lwm2m_swmgmt_install_completed(uint16_t obj_inst_id, int error_code);
#endif
#if defined(CONFIG_LWM2M_EVENT_LOG_OBJ_SUPPORT) || defined(__DOXYGEN__)
/**
 * @brief 设置读取日志数据的回调
 *
 * 当LWM2M读取操作在相应对象上调用时，将执行回调。
 *
 * @param[in] cb 处理读取事件的回调函数。
 */
void lwm2m_event_log_set_read_log_data_cb(lwm2m_engine_get_data_cb_t cb);
#endif
/**
 * @brief 对象链接资源字段的最大值
 */
#define LWM2M_OBJLNK_MAX_ID USHRT_MAX
/**
 * @brief LWM2M对象链接资源类型结构
 */
struct lwm2m_objlnk {
	uint16_t obj_id;     /**< 对象ID */
	uint16_t obj_inst;   /**< 对象实例ID */
};
/**
 * @brief 更改观察者的pmin值。
 *
 * LwM2M客户端使用此函数修改正在进行的观察的pmin属性。
 * 例如，更新正在观察的温度传感器值的pmin：
 * lwm2m_update_observer_min_period(client_ctx, &LWM2M_OBJ(3303, 0, 5700), 5);
 *
 * @param[in] client_ctx LwM2M上下文
 * @param[in] path LwM2M路径作为结构
 * @param[in] period_s 要设置的pmin值（秒）。
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_update_observer_min_period(struct lwm2m_ctx *client_ctx,
				     const struct lwm2m_obj_path *path, uint32_t period_s);
/**
 * @brief 更改观察者的pmax值。
 *
 * LwM2M客户端使用此函数修改正在进行的观察的pmax属性。
 * 例如，更新正在观察的温度传感器值的pmax：
 * lwm2m_update_observer_max_period(client_ctx, &LWM2M_OBJ(3303, 0, 5700), 5);
 *
 * @param[in] client_ctx LwM2M上下文
 * @param[in] path LwM2M路径作为结构
 * @param[in] period_s 要设置的pmax值（秒）。
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_update_observer_max_period(struct lwm2m_ctx *client_ctx,
				     const struct lwm2m_obj_path *path, uint32_t period_s);
/**
 * @brief 创建LwM2M对象实例。
 *
 * LwM2M客户端使用此函数创建非默认的LwM2M对象：
 * 例如，创建第一个温度传感器对象：
 * lwm2m_create_obj_inst(&LWM2M_OBJ(3303, 0));
 *
 * @param[in] path LwM2M路径作为结构
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_create_object_inst(const struct lwm2m_obj_path *path);
/**
 * @brief 删除LwM2M对象实例。
 *
 * LwM2M客户端使用此函数删除LwM2M对象。
 *
 * @param[in] path LwM2M路径作为结构
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_delete_object_inst(const struct lwm2m_obj_path *path);
/**
 * @brief 锁定此线程的注册表。
 *
 * 在写入多个资源之前使用此函数。这将暂停lwm2m主线程，直到所有写操作完成。
 *
 */
void lwm2m_registry_lock(void);
/**
 * @brief 解锁先前由lwm2m_registry_lock()锁定的注册表。
 *
 */
void lwm2m_registry_unlock(void);
/**
 * @brief 设置资源（实例）值（不透明缓冲区）
 *
 * @param[in] path LwM2M路径作为结构
 * @param[in] data_ptr 数据缓冲区
 * @param[in] data_len 缓冲区长度
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_set_opaque(const struct lwm2m_obj_path *path, const char *data_ptr, uint16_t data_len);
/**
 * @brief 设置资源（实例）值（字符串）
 *
 * @param[in] path LwM2M路径作为结构
 * @param[in] data_ptr 以NULL结尾的字符缓冲区
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_set_string(const struct lwm2m_obj_path *path, const char *data_ptr);
/**
 * @brief 设置资源（实例）值（u8）
 *
 * @param[in] path LwM2M路径作为结构
 * @param[in] value u8值
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_set_u8(const struct lwm2m_obj_path *path, uint8_t value);
/**
 * @brief 设置资源（实例）值（u16）
 *
 * @param[in] path LwM2M路径作为结构
 * @param[in] value u16值
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_set_u16(const struct lwm2m_obj_path *path, uint16_t value);
/**
 * @brief 设置资源（实例）值（u32）
 *
 * @param[in] path LwM2M路径作为结构
 * @param[in] value u32值
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_set_u32(const struct lwm2m_obj_path *path, uint32_t value);
/**
 * @brief 设置资源（实例）值（u64）
 *
 * @deprecated 无符号64位值类型不存在。
 *             这在内部处理为int64_t。
 *             请改用lwm2m_set_s64()。
 *
 * @param[in] path LwM2M路径作为结构
 * @param[in] value u64值
 *
 * @return 成功返回0，错误返回负值。
 */
__deprecated
int lwm2m_set_u64(const struct lwm2m_obj_path *path, uint64_t value);
/**
 * @brief 设置资源（实例）值（s8）
 *
 * @param[in] path LwM2M路径作为结构
 * @param[in] value s8值
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_set_s8(const struct lwm2m_obj_path *path, int8_t value);
/**
 * @brief 设置资源（实例）值（s16）
 *
 * @param[in] path LwM2M路径作为结构
 * @param[in] value s16值
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_set_s16(const struct lwm2m_obj_path *path, int16_t value);
/**
 * @brief 设置资源（实例）值（s32）
 *
 * @param[in] path LwM2M路径作为结构
 * @param[in] value s32值
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_set_s32(const struct lwm2m_obj_path *path, int32_t value);
/**
 * @brief 设置资源（实例）值（s64）
 *
 * @param[in] path LwM2M路径作为结构
 * @param[in] value s64值
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_set_s64(const struct lwm2m_obj_path *path, int64_t value);
/**
 * @brief 设置资源（实例）值（bool）
 *
 * @param[in] path LwM2M路径作为结构
 * @param[in] value bool值
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_set_bool(const struct lwm2m_obj_path *path, bool value);
/**
 * @brief 设置资源（实例）值（double）
 *
 * @param[in] path LwM2M路径作为结构
 * @param[in] value double值
 *
 * @return 成功返回0，错误返回负值
 */
int lwm2m_set_f64(const struct lwm2m_obj_path *path, const double value);
/**
 * @brief 设置资源（实例）值（Objlnk）
 *
 * @param[in] path LwM2M路径作为结构
 * @param[in] value 指向lwm2m_objlnk结构的指针
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_set_objlnk(const struct lwm2m_obj_path *path, const struct lwm2m_objlnk *value);
/**
 * @brief 设置资源（实例）值（时间）
 *
 * @param[in] path LwM2M路径作为结构
 * @param[in] value 纪元时间戳
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_set_time(const struct lwm2m_obj_path *path, time_t value);
/**
 * @brief LwM2M资源项结构
 *
 * 值类型必须与目标资源匹配，因为不会进行类型转换，值只是内存复制。
 *
 * 以下C类型用于资源类型：
 * * BOOL 是 uint8_t
 * * U8 是 uint8_t
 * * S8 是 int8_t
 * * U16 是 uint16_t
 * * S16 是 int16_t
 * * U32 是 uint32_t
 * * S32 是 int32_t
 * * S64 是 int64_t
 * * TIME 是 time_t
 * * FLOAT 是 double
 * * OBJLNK 是 struct lwm2m_objlnk
 * * STRING 是 char *，并且应包含空终止符。
 * * OPAQUE 是任何二进制数据。当在OPAQUE资源中写入空终止字符串时，不应包含终止符。
 *
 */
struct lwm2m_res_item {
	/** 指向LwM2M路径的指针，作为结构 */
	struct lwm2m_obj_path *path;
	/** 指向资源值的指针 */
	void *value;
	/** 值的大小。对于字符串资源，应包含空终止符。 */
	uint16_t size;
};
/**
 * @brief 设置多个资源（实例）值
 *
 * 注意：值类型必须与目标资源匹配，因为此函数不会进行任何类型转换。
 * 请参阅struct @ref lwm2m_res_item以获取资源类型列表。
 *
 * @param[in] res_list LwM2M资源项列表
 * @param[in] res_list_size 资源列表的长度
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_set_bulk(const struct lwm2m_res_item res_list[], size_t res_list_size);
/**
 * @brief 获取资源（实例）值（不透明缓冲区）
 *
 * @param[in] path LwM2M路径作为结构
 * @param[out] buf 数据缓冲区以复制数据
 * @param[in] buflen 缓冲区长度
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_get_opaque(const struct lwm2m_obj_path *path, void *buf, uint16_t buflen);
/**
 * @brief 获取资源（实例）值（字符串）
 *
 * @param[in] path LwM2M路径作为结构
 * @param[out] str 字符串缓冲区以复制数据
 * @param[in] buflen 缓冲区长度
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_get_string(const struct lwm2m_obj_path *path, void *str, uint16_t buflen);
/**
 * @brief 获取资源（实例）值（u8）
 *
 * @param[in] path LwM2M路径作为结构
 * @param[out] value u8缓冲区以复制数据
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_get_u8(const struct lwm2m_obj_path *path, uint8_t *value);
/**
 * @brief 获取资源（实例）值（u16）
 *
 * @param[in] path LwM2M路径作为结构
 * @param[out] value u16缓冲区以复制数据
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_get_u16(const struct lwm2m_obj_path *path, uint16_t *value);
/**
 * @brief 获取资源（实例）值（u32）
 *
 * @param[in] path LwM2M路径作为结构
 * @param[out] value u32缓冲区以复制数据
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_get_u32(const struct lwm2m_obj_path *path, uint32_t *value);
/**
 * @brief 获取资源（实例）值（u64）
 *
 * @deprecated 无符号64位值类型不存在。
 *             这在内部处理为int64_t。
 *             请改用lwm2m_get_s64()。
 *
 * @param[in] path LwM2M路径作为结构
 * @param[out] value u64缓冲区以复制数据
 *
 * @return 成功返回0，错误返回负值。
 */
__deprecated
int lwm2m_get_u64(const struct lwm2m_obj_path *path, uint64_t *value);
/**
 * @brief 获取资源（实例）值（s8）
 *
 * @param[in] path LwM2M路径作为结构
 * @param[out] value s8缓冲区以复制数据
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_get_s8(const struct lwm2m_obj_path *path, int8_t *value);
/**
 * @brief 获取资源（实例）值（s16）
 *
 * @param[in] path LwM2M路径作为结构
 * @param[out] value s16缓冲区以复制数据
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_get_s16(const struct lwm2m_obj_path *path, int16_t *value);
/**
 * @brief 获取资源（实例）值（s32）
 *
 * @param[in] path LwM2M路径作为结构
 * @param[out] value s32缓冲区以复制数据
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_get_s32(const struct lwm2m_obj_path *path, int32_t *value);
/**
 * @brief 获取资源（实例）值（s64）
 *
 * @param[in] path LwM2M路径作为结构
 * @param[out] value s64缓冲区以复制数据
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_get_s64(const struct lwm2m_obj_path *path, int64_t *value);
/**
 * @brief 获取资源（实例）值（bool）
 *
 * @param[in] path LwM2M路径作为结构
 * @param[out] value bool缓冲区以复制数据
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_get_bool(const struct lwm2m_obj_path *path, bool *value);
/**
 * @brief 获取资源（实例）值（double）
 *
 * @param[in] path LwM2M路径作为结构
 * @param[out] value double缓冲区以复制数据
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_get_f64(const struct lwm2m_obj_path *path, double *value);
/**
 * @brief 获取资源（实例）值（Objlnk）
 *
 * @param[in] path LwM2M路径作为结构
 * @param[out] buf lwm2m_objlnk缓冲区以复制数据
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_get_objlnk(const struct lwm2m_obj_path *path, struct lwm2m_objlnk *buf);
/**
 * @brief 获取资源（实例）值（时间）
 *
 * @param[in] path LwM2M路径作为结构
 * @param[out] buf time_t指针以复制数据
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_get_time(const struct lwm2m_obj_path *path, time_t *buf);
/**
 * @brief 设置资源（实例）读取回调
 *
 * LwM2M客户端可以使用此函数设置资源读取的回调函数，以绕过LwM2M引擎中的数据处理。
 * 例如，从外部存储读取不透明的二进制数据。
 *
 * 此回调通常不应用于任何可能被观察的数据，因为引擎不知道数据的变化。
 *
 * 当应使用单独的数据缓冲区时，请改用lwm2m_set_res_buf()设置存储。
 *
 * @param[in] path LwM2M路径作为结构
 * @param[in] cb 读取资源回调
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_register_read_callback(const struct lwm2m_obj_path *path, lwm2m_engine_get_data_cb_t cb);
/**
 * @brief 设置资源（实例）预写回调
 *
 * 在设置资源值之前触发此回调。它可以将特殊数据缓冲区传递给引擎，以便稍后计算实际资源值等。
 *
 * @param[in] path LwM2M路径作为结构
 * @param[in] cb 预写资源回调
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_register_pre_write_callback(const struct lwm2m_obj_path *path,
				      lwm2m_engine_get_data_cb_t cb);
/**
 * @brief 设置资源（实例）验证回调
 *
 * 在将资源值设置到资源数据缓冲区之前触发此回调。
 *
 * 回调允许LwM2M客户端或对象在写入之前验证数据，并在任何原因导致数据应被丢弃时通知错误（通过返回负错误代码）。
 *
 * @note 所有注册了验证回调的资源最初都写入临时缓冲区。如果验证成功，数据将复制到实际资源缓冲区。
 *
 * @param[in] path LwM2M路径作为结构
 * @param[in] cb 验证资源数据回调
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_register_validate_callback(const struct lwm2m_obj_path *path,
				     lwm2m_engine_set_data_cb_t cb);
/**
 * @brief 设置资源（实例）后写回调
 *
 * 在将资源值设置到资源数据缓冲区之后触发此回调。
 *
 * 回调允许LwM2M客户端或对象在数据写入资源缓冲区后执行操作。
 *
 * @param[in] path LwM2M路径作为结构
 * @param[in] cb 后写资源回调
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_register_post_write_callback(const struct lwm2m_obj_path *path,
				       lwm2m_engine_set_data_cb_t cb);
/**
 * @brief 设置资源执行事件回调
 *
 * 启用资源的执行方法时触发此事件。
 *
 * @param[in] path LwM2M路径作为结构
 * @param[in] cb 执行资源回调
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_register_exec_callback(const struct lwm2m_obj_path *path, lwm2m_engine_execute_cb_t cb);
/**
 * @brief 设置对象实例创建事件回调
 *
 * 创建对象实例时触发此事件。
 *
 * @param[in] obj_id LwM2M对象ID
 * @param[in] cb 创建对象实例回调
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_register_create_callback(uint16_t obj_id,
				   lwm2m_engine_user_cb_t cb);
/**
 * @brief 设置对象实例删除事件回调
 *
 * 删除对象实例时触发此事件。
 *
 * @param[in] obj_id LwM2M对象ID
 * @param[in] cb 删除对象实例回调
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_register_delete_callback(uint16_t obj_id,
				   lwm2m_engine_user_cb_t cb);
/**
 * @brief 资源只读值位
 */
#define LWM2M_RES_DATA_READ_ONLY	0
/**
 * @brief 资源只读标志
 */
#define LWM2M_RES_DATA_FLAG_RO		BIT(LWM2M_RES_DATA_READ_ONLY)
/**
 * @brief 读取资源标志助手宏
 */
#define LWM2M_HAS_RES_FLAG(res, f)	((res->data_flags & f) == f)
/**
 * @brief 设置资源的数据缓冲区
 *
 * 使用此函数设置指定LwM2M资源的数据缓冲区和标志。
 *
 * @param[in] path LwM2M路径作为结构
 * @param[in] buffer_ptr 数据缓冲区指针
 * @param[in] buffer_len 缓冲区长度
 * @param[in] data_len 缓冲区中现有数据的长度
 * @param[in] data_flags 数据缓冲区标志（例如只读等）
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_set_res_buf(const struct lwm2m_obj_path *path, void *buffer_ptr, uint16_t buffer_len,
		      uint16_t data_len, uint8_t data_flags);
/**
 * @brief 更新资源的数据大小
 *
 * 如果通过lwm2m_get_res_buf()函数获取缓冲区后直接写入缓冲区，请使用此函数设置缓冲区中数据的新大小。
 *
 * @param[in] path LwM2M路径作为结构
 * @param[in] data_len 数据长度
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_set_res_data_len(const struct lwm2m_obj_path *path, uint16_t data_len);
/**
 * @brief 获取资源的数据缓冲区
 *
 * 使用此函数获取指定LwM2M资源的数据缓冲区信息。
 *
 * 如果直接写入缓冲区，则必须使用lwm2m_set_res_data_len()函数更新写入数据的新大小。
 *
 * 除pathstr外，所有参数都可以为NULL，如果不想读取这些值。
 *
 * @param[in] path LwM2M路径作为结构
 * @param[out] buffer_ptr 数据缓冲区指针
 * @param[out] buffer_len 缓冲区长度
 * @param[out] data_len 缓冲区中现有数据的长度
 * @param[out] data_flags 数据缓冲区标志（例如只读等）
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_get_res_buf(const struct lwm2m_obj_path *path, void **buffer_ptr, uint16_t *buffer_len,
		      uint16_t *data_len, uint8_t *data_flags);
/**
 * @brief 创建资源实例
 *
 * LwM2M客户端使用此函数创建多资源实例：
 * 例如，创建设备可用电源源的0实例：
 * lwm2m_create_res_inst(&LWM2M_OBJ(3, 0, 6, 0));
 *
 * @param[in] path LwM2M路径作为结构
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_create_res_inst(const struct lwm2m_obj_path *path);
/**
 * @brief 删除资源实例
 *
 * 使用此函数删除现有资源实例
 *
 * @param[in] path LwM2M路径作为结构
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_delete_res_inst(const struct lwm2m_obj_path *path);
/**
 * @brief 更新设备服务的周期。
 *
 * 更改通知当前时间的周期性设备服务的持续时间。
 *
 * @param[in] period_ms 设备服务的新周期（毫秒）
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_update_device_service_period(uint32_t period_ms);
/**
 * @brief 检查路径是否被观察
 *
 * @param[in] path 要检查的LwM2M路径作为结构
 *
 * @return 当存在与给定路径相同级别或更低级别的观察时返回true，否则返回false或路径不是有效的LwM2M路径。
 *         例如，如果路径引用资源且父对象有观察，则返回true，反之则返回false。
 */
bool lwm2m_path_is_observed(const struct lwm2m_obj_path *path);
/**
 * @brief 停止LwM2M引擎
 *
 * LwM2M客户端通常不需要调用此函数，因为它在lwm2m_rd_client中调用。
 * 但是，如果客户端不使用RD客户端实现，则需要手动调用。
 *
 * @param[in] client_ctx LwM2M上下文
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_engine_stop(struct lwm2m_ctx *client_ctx);
/**
 * @brief 启动LwM2M引擎
 *
 * LwM2M客户端通常不需要调用此函数，因为它由lwm2m_rd_client_start()调用。
 * 但是，如果客户端不使用RD客户端实现，则需要手动调用。
 *
 * @param[in] client_ctx LwM2M上下文
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_engine_start(struct lwm2m_ctx *client_ctx);
/**
 * @brief 使用空ACK确认当前处理的请求。
 *
 * LwM2M引擎默认发送带有响应的确认消息。此函数允许在应用程序回调中提前发送空ACK。
 * 然后，LwM2M引擎将在所有回调执行后发送实际响应作为单独的确认消息。
 *
 * @param[in] client_ctx LwM2M上下文
 *
 */
void lwm2m_acknowledge(struct lwm2m_ctx *client_ctx);
/**
 * @brief LwM2M RD客户端事件
 *
 * LwM2M客户端事件通过lwm2m_rd_client_start()中的event_cb函数传递回去
 */
enum lwm2m_rd_client_event {
	/** 无效事件 */
	LWM2M_RD_CLIENT_EVENT_NONE,
	/** 引导注册失败 */
	LWM2M_RD_CLIENT_EVENT_BOOTSTRAP_REG_FAILURE,
	/** 引导注册完成 */
	LWM2M_RD_CLIENT_EVENT_BOOTSTRAP_REG_COMPLETE,
	/** 引导传输完成 */
	LWM2M_RD_CLIENT_EVENT_BOOTSTRAP_TRANSFER_COMPLETE,
	/** 注册失败 */
	LWM2M_RD_CLIENT_EVENT_REGISTRATION_FAILURE,
	/** 注册完成 */
	LWM2M_RD_CLIENT_EVENT_REGISTRATION_COMPLETE,
	/** 注册超时 */
	LWM2M_RD_CLIENT_EVENT_REG_TIMEOUT,
	/** 注册更新完成 */
	LWM2M_RD_CLIENT_EVENT_REG_UPDATE_COMPLETE,
	/** 注销失败 */
	LWM2M_RD_CLIENT_EVENT_DEREGISTER_FAILURE,
	/** 断开连接 */
	LWM2M_RD_CLIENT_EVENT_DISCONNECT,
	/** 队列模式RX关闭 */
	LWM2M_RD_CLIENT_EVENT_QUEUE_MODE_RX_OFF,
	/** 引擎挂起 */
	LWM2M_RD_CLIENT_EVENT_ENGINE_SUSPENDED,
	/** 网络错误 */
	LWM2M_RD_CLIENT_EVENT_NETWORK_ERROR,
	/** 注册更新 */
	LWM2M_RD_CLIENT_EVENT_REG_UPDATE,
	/** 注销 */
	LWM2M_RD_CLIENT_EVENT_DEREGISTER,
	/** 服务器禁用 */
	LWM2M_RD_CLIENT_EVENT_SERVER_DISABLED,
};

/*
 * LwM2M RD客户端标志，用于配置LwM2M会话。
 */

/**
 * @brief 在当前会话中运行引导程序。
 */
#define LWM2M_RD_CLIENT_FLAG_BOOTSTRAP BIT(0)

/**
 * @brief 启动LwM2M RD（注册/发现）客户端
 *
 * RD客户端位于LwM2M引擎之上，执行实现“注册接口”所需的操作。
 * 有关更多信息，请参阅LwM2M技术规范的“客户端注册接口”部分。
 *
 * 注意：此函数会自动调用lwm2m_engine_start()。
 *
 * @param[in] client_ctx LwM2M上下文
 * @param[in] ep_name 注册的端点名称
 * @param[in] flags 用于配置当前LwM2M会话的标志。
 * @param[in] event_cb 客户端事件回调函数
 * @param[in] observe_cb 当添加或删除观察者以及通知被确认或超时时调用的观察回调函数
 *
 * @return 成功返回0，客户端已在运行时返回-EINPROGRESS，失败时返回负错误代码。
 */
int lwm2m_rd_client_start(struct lwm2m_ctx *client_ctx, const char *ep_name,
			   uint32_t flags, lwm2m_ctx_event_cb_t event_cb,
			   lwm2m_observe_cb_t observe_cb);

/**
 * @brief 停止LwM2M RD（注销）客户端
 *
 * RD客户端位于LwM2M引擎之上，执行实现“注册接口”所需的操作。
 * 有关更多信息，请参阅LwM2M技术规范的“客户端注册接口”部分。
 *
 * @param[in] client_ctx LwM2M上下文
 * @param[in] event_cb 客户端事件回调函数
 * @param[in] deregister 如果已注册，则为true注销客户端。如果为false，则强制关闭连接。
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_rd_client_stop(struct lwm2m_ctx *client_ctx,
			  lwm2m_ctx_event_cb_t event_cb, bool deregister);

/**
 * @brief 挂起LwM2M引擎线程
 *
 * 挂起LwM2M引擎。用例可能是在网络连接断开时。
 * LwM2M引擎在挂起前通过LWM2M_RD_CLIENT_EVENT_ENGINE_SUSPENDED事件指示。
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_engine_pause(void);

/**
 * @brief 恢复LwM2M引擎线程
 *
 * 恢复挂起的LwM2M引擎。成功恢复调用后，引擎将根据挂起时间执行完整注册或注册更新。
 * 事件LWM2M_RD_CLIENT_EVENT_REGISTRATION_COMPLETE或LWM2M_RD_CLIENT_EVENT_REG_UPDATE_COMPLETE指示客户端已连接到服务器。
 *
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_engine_resume(void);

/**
 * @brief 触发LwM2M RD客户端的注册更新
 */
void lwm2m_rd_client_update(void);

/**
 * @brief LwM2M路径的最大长度
 */
#define LWM2M_MAX_PATH_STR_SIZE sizeof("/65535/65535/65535/65535")

/**
 * @brief 帮助函数将路径对象的内容打印到日志
 *
 * @param[in] buf 用于格式化字符串的缓冲区
 * @param[in] path 要字符串化的路径
 *
 * @return 结果格式化路径字符串
 */
char *lwm2m_path_log_buf(char *buf, struct lwm2m_obj_path *path);

/**
 * @brief LwM2M发送状态
 *
 * LwM2M发送状态通过lwm2m_send_cb()函数传递回lwm2m_send_cb_t函数
 */
enum lwm2m_send_status {
	LWM2M_SEND_STATUS_SUCCESS,  /**< 成功 */
	LWM2M_SEND_STATUS_FAILURE,  /**< 失败 */
	LWM2M_SEND_STATUS_TIMEOUT,  /**< 超时 */
};

/**
 * @typedef lwm2m_send_cb_t
 * @brief 返回发送状态的回调
 */
typedef void (*lwm2m_send_cb_t)(enum lwm2m_send_status status);

/** 
 * @brief LwM2M SEND操作异步发送到给定路径列表，并带有确认回调
 *
 * @param ctx LwM2M上下文
 * @param path_list LwM2M路径结构列表
 * @param path_list_size 路径列表的长度。最大大小为CONFIG_LWM2M_COMPOSITE_PATH_LIST_SIZE
 * @param reply_cb 触发确认状态的回调或不使用时为NULL
 *
 * @return 成功返回0，错误返回负值。
 *
 */
int lwm2m_send_cb(struct lwm2m_ctx *ctx, const struct lwm2m_obj_path path_list[],
			  uint8_t path_list_size, lwm2m_send_cb_t reply_cb);

/** 
 * @brief 返回LwM2M客户端上下文
 *
 * @return ctx LwM2M上下文
 *
 */
struct lwm2m_ctx *lwm2m_rd_client_ctx(void);

/** 
 * @brief 启用资源的数据缓存。
 *
 * 应用程序可以通过为LwM2M引擎分配缓冲区来启用资源数据缓存。
 * 缓冲区必须是struct @ref lwm2m_time_series_elem的大小乘以cache_len
 *
 * @param path LwM2M路径作为结构
 * @param data_cache 数据缓存数组的指针
 * @param cache_len 缓存条目的数量
 *
 * @return 成功返回0，错误返回负值。
 *
 */
int lwm2m_enable_cache(const struct lwm2m_obj_path *path, struct lwm2m_time_series_elem *data_cache,
		       size_t cache_len);

/**
 * @brief LwM2M安全模式，如LwM2M安全对象中定义。
 */
enum lwm2m_security_mode_e {
	LWM2M_SECURITY_PSK = 0,      /**< 预共享密钥模式 */
	LWM2M_SECURITY_RAW_PK = 1,   /**< 原始公钥模式 */
	LWM2M_SECURITY_CERT = 2,     /**< 证书模式 */
	LWM2M_SECURITY_NOSEC = 3,    /**< 无安全模式 */
	LWM2M_SECURITY_CERT_EST = 4, /**< 证书模式与EST */
};

/**
 * @brief 从选定的安全对象实例读取安全模式。
 *
 * 仅在RD客户端运行时有效。
 *
 * @param ctx 指向客户端上下文的指针。
 * @return int 正值为@ref lwm2m_security_mode_e，负错误代码否则。
 */
int lwm2m_security_mode(struct lwm2m_ctx *ctx);

/**
 * @brief 设置DTLS连接的默认套接字选项。
 *
 * 当@ref lwm2m_ctx::set_socketoptions未被覆盖时，引擎会调用此函数。
 * 您可以从覆盖的回调中调用此函数，以在默认选项之前或之后设置额外选项。
 *
 * @param ctx 客户端上下文
 * @return 成功返回0，错误返回负值。
 */
int lwm2m_set_default_sockopt(struct lwm2m_ctx *ctx);

#endif	/* ZEPHYR_INCLUDE_NET_LWM2M_H_ */
/**@}  */