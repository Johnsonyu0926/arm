//zephyr-3.7-branch/lib/libc/minimal/include/limits.h
#ifndef ZEPHYR_LIB_LIBC_MINIMAL_INCLUDE_LIMITS_H_
#define ZEPHYR_LIB_LIBC_MINIMAL_INCLUDE_LIMITS_H_

#ifdef __cplusplus
extern "C" {
#endif

#if __CHAR_BIT__ == 8
#define UCHAR_MAX 0xFFU /**< Maximum value for an unsigned char */
#else
#error "unexpected __CHAR_BIT__ value"
#endif

#define SCHAR_MAX __SCHAR_MAX__ /**< Maximum value for a signed char */
#define SCHAR_MIN (-SCHAR_MAX - 1) /**< Minimum value for a signed char */

#ifdef __CHAR_UNSIGNED__
/* 'char' is an unsigned type */
#define CHAR_MAX UCHAR_MAX /**< Maximum value for a char */
#define CHAR_MIN 0 /**< Minimum value for a char */
#else
/* 'char' is a signed type */
#define CHAR_MAX SCHAR_MAX /**< Maximum value for a char */
#define CHAR_MIN SCHAR_MIN /**< Minimum value for a char */
#endif

#define CHAR_BIT __CHAR_BIT__ /**< Number of bits in a char */
#define LONG_BIT (__SIZEOF_LONG__ * CHAR_BIT) /**< Number of bits in a long */
#define WORD_BIT (__SIZEOF_POINTER__ * CHAR_BIT) /**< Number of bits in a pointer */

#define INT_MAX __INT_MAX__ /**< Maximum value for an int */
#define SHRT_MAX __SHRT_MAX__ /**< Maximum value for a short */
#define LONG_MAX __LONG_MAX__ /**< Maximum value for a long */
#define LLONG_MAX __LONG_LONG_MAX__ /**< Maximum value for a long long */

#define INT_MIN (-INT_MAX - 1) /**< Minimum value for an int */
#define SHRT_MIN (-SHRT_MAX - 1) /**< Minimum value for a short */
#define LONG_MIN (-LONG_MAX - 1L) /**< Minimum value for a long */
#define LLONG_MIN (-LLONG_MAX - 1LL) /**< Minimum value for a long long */

#if __SIZE_MAX__ == __UINT32_MAX__
#define SSIZE_MAX __INT32_MAX__ /**< Maximum value for a ssize_t */
#elif __SIZE_MAX__ == __UINT64_MAX__
#define SSIZE_MAX __INT64_MAX__ /**< Maximum value for a ssize_t */
#else
#error "unexpected __SIZE_MAX__ value"
#endif

#if __SIZEOF_SHORT__ == 2
#define USHRT_MAX 0xFFFFU /**< Maximum value for an unsigned short */
#else
#error "unexpected __SIZEOF_SHORT__ value"
#endif

#if __SIZEOF_INT__ == 4
#define UINT_MAX 0xFFFFFFFFU /**< Maximum value for an unsigned int */
#else
#error "unexpected __SIZEOF_INT__ value"
#endif

#if __SIZEOF_LONG__ == 4
#define ULONG_MAX 0xFFFFFFFFUL /**< Maximum value for an unsigned long */
#elif __SIZEOF_LONG__ == 8
#define ULONG_MAX 0xFFFFFFFFFFFFFFFFUL /**< Maximum value for an unsigned long */
#else
#error "unexpected __SIZEOF_LONG__ value"
#endif

#if __SIZEOF_LONG_LONG__ == 8
#define ULLONG_MAX 0xFFFFFFFFFFFFFFFFULL /**< Maximum value for an unsigned long long */
#else
#error "unexpected __SIZEOF_LONG_LONG__ value"
#endif

#define PATH_MAX 256 /**< Maximum length of a path */

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_LIB_LIBC_MINIMAL_INCLUDE_LIMITS_H_ */
//GST