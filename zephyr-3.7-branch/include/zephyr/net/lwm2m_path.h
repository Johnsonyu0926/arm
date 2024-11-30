// zephyr-3.7-branch/include/zephyr/net/lwm2m_path.h

#ifndef ZEPHYR_INCLUDE_NET_LWM2M_PATH_H_
#define ZEPHYR_INCLUDE_NET_LWM2M_PATH_H_

/**
 * @file lwm2m.h
 *
 * @brief LwM2M路径助手宏
 *
 * @defgroup lwm2m_path_helpers LwM2M路径助手宏
 * @ingroup lwm2m_api
 * @{
 */

/**
 * @brief 使用数字组件生成LwM2M字符串路径。
 *
 * 接受至少一个且最多四个参数。每个参数将由预处理器字符串化，因此使用非字面量调用此函数可能不会得到您想要的结果。
 *
 * 例如：
 *
 * @code{c}
 * #define MY_OBJ_ID 3
 * LWM2M_PATH(MY_OBJ_ID, 0, 1)
 * @endcode
 *
 * 将评估为"3/0/1"，而
 *
 * @code{c}
 * int x = 3;
 * LWM2M_PATH(x, 0, 1)
 * @endcode
 *
 * 评估为"x/0/1"。
 */
#define LWM2M_PATH(...) \
	LWM2M_PATH_MACRO(__VA_ARGS__, LWM2M_PATH4, LWM2M_PATH3,	\
			 LWM2M_PATH2, LWM2M_PATH1)(__VA_ARGS__)

/** @cond INTERNAL_HIDDEN */
/* LWM2M_PATH宏的内部助手宏 */
#define LWM2M_PATH_VA_NUM_ARGS(...) \
	LWM2M_PATH_VA_NUM_ARGS_IMPL(__VA_ARGS__, 5, 4, 3, 2, 1)
#define LWM2M_PATH_VA_NUM_ARGS_IMPL(_1, _2, _3, _4, N, ...) N

#define LWM2M_PATH1(_x) #_x
#define LWM2M_PATH2(_x, _y) #_x "/" #_y
#define LWM2M_PATH3(_x, _y, _z) #_x "/" #_y "/" #_z
#define LWM2M_PATH4(_a, _x, _y, _z) #_a "/" #_x "/" #_y "/" #_z

#define LWM2M_PATH_MACRO(_1, _2, _3, _4, NAME, ...) NAME
/** @endcond */

/**
 * @brief 初始化LwM2M对象结构
 *
 * 接受至少一个且最多四个参数。填充@ref lwm2m_obj_path结构并设置级别。
 *
 * 例如：
 *
 * @code{c}
 * struct lwm2m_obj_path p = LWM2M_OBJ(MY_OBJ, 0, RESOURCE);
 * @endcode
 *
 * 也可以在函数参数中使用，以返回从堆栈分配的结构
 *
 * @code{c}
 * lwm2m_notify_observer_path(&LWM2M_OBJ(MY_OBJ, inst_id, RESOURCE));
 * @endcode
 *
 */
#define LWM2M_OBJ(...) \
	GET_OBJ_MACRO(__VA_ARGS__, LWM2M_OBJ4, LWM2M_OBJ3, LWM2M_OBJ2, LWM2M_OBJ1)(__VA_ARGS__)

/** @cond INTERNAL_HIDDEN */
/* LWM2M_OBJ宏的内部助手宏 */
#define GET_OBJ_MACRO(_1, _2, _3, _4, NAME, ...) NAME
#define LWM2M_OBJ1(oi) (struct lwm2m_obj_path) {.obj_id = oi, .level = 1}
#define LWM2M_OBJ2(oi, oii) (struct lwm2m_obj_path) {.obj_id = oi, .obj_inst_id = oii, .level = 2}
#define LWM2M_OBJ3(oi, oii, ri) (struct lwm2m_obj_path) \
	{.obj_id = oi, .obj_inst_id = oii, .res_id = ri, .level = 3}
#define LWM2M_OBJ4(oi, oii, ri, rii) (struct lwm2m_obj_path) \
	{.obj_id = oi, .obj_inst_id = oii, .res_id = ri, .res_inst_id = rii, .level = 4}
/** @endcond */

/** @} */

#endif /* ZEPHYR_INCLUDE_NET_LWM2M_PATH_H_ */

// By GST @Data