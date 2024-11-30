//zephyr-3.7-branch/lib/libc/minimal/include/time.h
#ifndef ZEPHYR_LIB_LIBC_MINIMAL_INCLUDE_TIME_H_
#define ZEPHYR_LIB_LIBC_MINIMAL_INCLUDE_TIME_H_

#include <stdint.h>
#include <zephyr/toolchain.h>
#include <sys/_types.h>
#include <sys/_timespec.h>

/* Minimal time.h to fulfill the requirements of certain libraries
 * like mbedTLS and to support time APIs.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Structure representing calendar time
 */
struct tm {
	int tm_sec;   /**< Seconds [0,60] */
	int tm_min;   /**< Minutes [0,59] */
	int tm_hour;  /**< Hours [0,23] */
	int tm_mday;  /**< Day of the month [1,31] */
	int tm_mon;   /**< Month of the year [0,11] */
	int tm_year;  /**< Years since 1900 */
	int tm_wday;  /**< Day of the week [0,6] (Sunday = 0) */
	int tm_yday;  /**< Day of the year [0,365] */
	int tm_isdst; /**< Daylight saving time flag */
};

#if !defined(__time_t_defined)
#define __time_t_defined
typedef _TIME_T_ time_t; /**< Time type */
#endif

#if !defined(__suseconds_t_defined)
#define __suseconds_t_defined
typedef _SUSECONDS_T_ suseconds_t; /**< Microseconds type */
#endif

/**
 * @brief Convert a time value to a tm structure as UTC time
 *
 * @param timep Pointer to the time value
 * @return Pointer to the tm structure
 */
struct tm *gmtime(const time_t *timep);

/**
 * @brief Convert a time value to a tm structure as UTC time (reentrant)
 *
 * @param timep Pointer to the time value
 * @param result Pointer to the tm structure to store the result
 * @return Pointer to the tm structure
 */
struct tm *gmtime_r(const time_t *ZRESTRICT timep,
		    struct tm *ZRESTRICT result);

/**
 * @brief Get the current time
 *
 * @param tloc Pointer to store the current time
 * @return The current time in seconds since the epoch, or -1 on failure
 */
time_t time(time_t *tloc);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_LIB_LIBC_MINIMAL_INCLUDE_TIME_H_ */
@GST