//zephyr-3.7-branch/lib/libc/armstdc/include/sys/_timeval.h
#ifndef ZEPHYR_LIB_LIBC_ARMSTDC_INCLUDE_SYS__TIMEVAL_H_
#define ZEPHYR_LIB_LIBC_ARMSTDC_INCLUDE_SYS__TIMEVAL_H_

#include <stdint.h>

#if !defined(__time_t_defined)
#define __time_t_defined
/**
 * @brief Define time_t type
 */
typedef unsigned int time_t;
#endif

#if !defined(__suseconds_t_defined)
#define __suseconds_t_defined
/**
 * @brief Define suseconds_t type
 */
typedef int32_t suseconds_t;
#endif

/**
 * @brief timeval structure
 *
 * This structure represents a time value with seconds and microseconds.
 */
struct timeval {
	time_t tv_sec;       /**< seconds */
	suseconds_t tv_usec; /**< microseconds */
};

#endif /* ZEPHYR_LIB_LIBC_ARMSTDC_INCLUDE_SYS__TIMEVAL_H_ */
//GST