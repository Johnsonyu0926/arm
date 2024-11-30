//zephyr-3.7-branch/lib/libc/minimal/include/stdint.h
#ifndef ZEPHYR_LIB_LIBC_MINIMAL_INCLUDE_STDINT_H_
#define ZEPHYR_LIB_LIBC_MINIMAL_INCLUDE_STDINT_H_

#ifdef __cplusplus
extern "C" {
#endif

#define INT8_MAX    __INT8_MAX__    /**< Maximum value for int8_t */
#define INT16_MAX   __INT16_MAX__   /**< Maximum value for int16_t */
#define INT32_MAX   __INT32_MAX__   /**< Maximum value for int32_t */
#define INT64_MAX   __INT64_MAX__   /**< Maximum value for int64_t */
#define INTMAX_MAX  __INT64_MAX__   /**< Maximum value for intmax_t */

#define INT8_MIN    (-INT8_MAX - 1) /**< Minimum value for int8_t */
#define INT16_MIN   (-INT16_MAX - 1) /**< Minimum value for int16_t */
#define INT32_MIN   (-INT32_MAX - 1) /**< Minimum value for int32_t */
#define INT64_MIN   (-INT64_MAX - 1LL) /**< Minimum value for int64_t */

#define UINT8_MAX   __UINT8_MAX__   /**< Maximum value for uint8_t */
#define UINT16_MAX  __UINT16_MAX__  /**< Maximum value for uint16_t */
#define UINT32_MAX  __UINT32_MAX__  /**< Maximum value for uint32_t */
#define UINT64_MAX  __UINT64_MAX__  /**< Maximum value for uint64_t */
#define UINTMAX_MAX __UINT64_MAX__  /**< Maximum value for uintmax_t */

#define INT_FAST8_MAX    __INT_FAST8_MAX__    /**< Maximum value for int_fast8_t */
#define INT_FAST16_MAX   __INT_FAST16_MAX__   /**< Maximum value for int_fast16_t */
#define INT_FAST32_MAX   __INT_FAST32_MAX__   /**< Maximum value for int_fast32_t */
#define INT_FAST64_MAX   __INT_FAST64_MAX__   /**< Maximum value for int_fast64_t */

#define INT_FAST8_MIN    (-INT_FAST8_MAX - 1) /**< Minimum value for int_fast8_t */
#define INT_FAST16_MIN   (-INT_FAST16_MAX - 1) /**< Minimum value for int_fast16_t */
#define INT_FAST32_MIN   (-INT_FAST32_MAX - 1) /**< Minimum value for int_fast32_t */
#define INT_FAST64_MIN   (-INT_FAST64_MAX - 1LL) /**< Minimum value for int_fast64_t */

#define UINT_FAST8_MAX    __UINT_FAST8_MAX__    /**< Maximum value for uint_fast8_t */
#define UINT_FAST16_MAX   __UINT_FAST16_MAX__   /**< Maximum value for uint_fast16_t */
#define UINT_FAST32_MAX   __UINT_FAST32_MAX__   /**< Maximum value for uint_fast32_t */
#define UINT_FAST64_MAX   __UINT_FAST64_MAX__   /**< Maximum value for uint_fast64_t */

#define INT_LEAST8_MAX    __INT_LEAST8_MAX__    /**< Maximum value for int_least8_t */
#define INT_LEAST16_MAX   __INT_LEAST16_MAX__   /**< Maximum value for int_least16_t */
#define INT_LEAST32_MAX   __INT_LEAST32_MAX__   /**< Maximum value for int_least32_t */
#define INT_LEAST64_MAX   __INT_LEAST64_MAX__   /**< Maximum value for int_least64_t */

#define INT_LEAST8_MIN    (-INT_LEAST8_MAX - 1) /**< Minimum value for int_least8_t */
#define INT_LEAST16_MIN   (-INT_LEAST16_MAX - 1) /**< Minimum value for int_least16_t */
#define INT_LEAST32_MIN   (-INT_LEAST32_MAX - 1) /**< Minimum value for int_least32_t */
#define INT_LEAST64_MIN   (-INT_LEAST64_MAX - 1LL) /**< Minimum value for int_least64_t */

#define UINT_LEAST8_MAX    __UINT_LEAST8_MAX__    /**< Maximum value for uint_least8_t */
#define UINT_LEAST16_MAX   __UINT_LEAST16_MAX__   /**< Maximum value for uint_least16_t */
#define UINT_LEAST32_MAX   __UINT_LEAST32_MAX__   /**< Maximum value for uint_least32_t */
#define UINT_LEAST64_MAX   __UINT_LEAST64_MAX__   /**< Maximum value for uint_least64_t */

#define INTPTR_MAX  __INTPTR_MAX__  /**< Maximum value for intptr_t */
#define INTPTR_MIN  (-INTPTR_MAX - 1) /**< Minimum value for intptr_t */
#define UINTPTR_MAX __UINTPTR_MAX__ /**< Maximum value for uintptr_t */

#define PTRDIFF_MAX __PTRDIFF_MAX__ /**< Maximum value for ptrdiff_t */
#define PTRDIFF_MIN (-PTRDIFF_MAX - 1) /**< Minimum value for ptrdiff_t */

#define SIZE_MAX    __SIZE_MAX__ /**< Maximum value for size_t */

typedef __INT8_TYPE__		int8_t; /**< 8-bit signed integer */
typedef __INT16_TYPE__		int16_t; /**< 16-bit signed integer */
typedef __INT32_TYPE__		int32_t; /**< 32-bit signed integer */
typedef __INT64_TYPE__		int64_t; /**< 64-bit signed integer */
typedef __INT64_TYPE__		intmax_t; /**< Maximum-width signed integer */

typedef __INT_FAST8_TYPE__	int_fast8_t; /**< Fastest minimum-width 8-bit signed integer */
typedef __INT_FAST16_TYPE__	int_fast16_t; /**< Fastest minimum-width 16-bit signed integer */
typedef __INT_FAST32_TYPE__	int_fast32_t; /**< Fastest minimum-width 32-bit signed integer */
typedef __INT_FAST64_TYPE__	int_fast64_t; /**< Fastest minimum-width 64-bit signed integer */

typedef __INT_LEAST8_TYPE__	int_least8_t; /**< Least minimum-width 8-bit signed integer */
typedef __INT_LEAST16_TYPE__	int_least16_t; /**< Least minimum-width 16-bit signed integer */
typedef __INT_LEAST32_TYPE__	int_least32_t; /**< Least minimum-width 32-bit signed integer */
typedef __INT_LEAST64_TYPE__	int_least64_t; /**< Least minimum-width 64-bit signed integer */

typedef __UINT8_TYPE__		uint8_t; /**< 8-bit unsigned integer */
typedef __UINT16_TYPE__		uint16_t; /**< 16-bit unsigned integer */
typedef __UINT32_TYPE__		uint32_t; /**< 32-bit unsigned integer */
typedef __UINT64_TYPE__		uint64_t; /**< 64-bit unsigned integer */
typedef __UINT64_TYPE__		uintmax_t; /**< Maximum-width unsigned integer */

typedef __UINT_FAST8_TYPE__	uint_fast8_t; /**< Fastest minimum-width 8-bit unsigned integer */
typedef __UINT_FAST16_TYPE__	uint_fast16_t; /**< Fastest minimum-width 16-bit unsigned integer */
typedef __UINT_FAST32_TYPE__	uint_fast32_t; /**< Fastest minimum-width 32-bit unsigned integer */
typedef __UINT_FAST64_TYPE__	uint_fast64_t; /**< Fastest minimum-width 64-bit unsigned integer */

typedef __UINT_LEAST8_TYPE__	uint_least8_t; /**< Least minimum-width 8-bit unsigned integer */
typedef __UINT_LEAST16_TYPE__	uint_least16_t; /**< Least minimum-width 16-bit unsigned integer */
typedef __UINT_LEAST32_TYPE__	uint_least32_t; /**< Least minimum-width 32-bit unsigned integer */
typedef __UINT_LEAST64_TYPE__	uint_least64_t; /**< Least minimum-width 64-bit unsigned integer */

typedef __INTPTR_TYPE__		intptr_t; /**< Signed integer type capable of holding a pointer */
typedef __UINTPTR_TYPE__	uintptr_t; /**< Unsigned integer type capable of holding a pointer */

#if defined(__GNUC__) || defined(__clang__)
/* These macros must produce constant integer expressions, which can't
 * be done in the preprocessor (casts aren't allowed).  Defer to the
 * GCC internal functions where they're available.
 */
#define INT8_C(_v) __INT8_C(_v) /**< Macro for int8_t constants */
#define INT16_C(_v) __INT16_C(_v) /**< Macro for int16_t constants */
#define INT32_C(_v) __INT32_C(_v) /**< Macro for int32_t constants */
#define INT64_C(_v) __INT64_C(_v) /**< Macro for int64_t constants */
#define INTMAX_C(_v) __INTMAX_C(_v) /**< Macro for intmax_t constants */

#define UINT8_C(_v) __UINT8_C(_v) /**< Macro for uint8_t constants */
#define UINT16_C(_v) __UINT16_C(_v) /**< Macro for uint16_t constants */
#define UINT32_C(_v) __UINT32_C(_v) /**< Macro for uint32_t constants */
#define UINT64_C(_v) __UINT64_C(_v) /**< Macro for uint64_t constants */
#define UINTMAX_C(_v) __UINTMAX_C(_v) /**< Macro for uintmax_t constants */
#endif /* defined(__GNUC__) || defined(__clang__) */

#ifdef __CCAC__
#ifndef __INT8_C
#define __INT8_C(x)	x /**< Macro for int8_t constants */
#endif

#ifndef INT8_C
#define INT8_C(x)	__INT8_C(x) /**< Macro for int8_t constants */
#endif

#ifndef __UINT8_C
#define __UINT8_C(x)	x ## U /**< Macro for uint8_t constants */
#endif

#ifndef UINT8_C
#define UINT8_C(x)	__UINT8_C(x) /**< Macro for uint8_t constants */
#endif

#ifndef __INT16_C
#define __INT16_C(x)	x /**< Macro for int16_t constants */
#endif

#ifndef INT16_C
#define INT16_C(x)	__INT16_C(x) /**< Macro for int16_t constants */
#endif

#ifndef __UINT16_C
#define __UINT16_C(x)	x ## U /**< Macro for uint16_t constants */
#endif

#ifndef UINT16_C
#define UINT16_C(x)	__UINT16_C(x) /**< Macro for uint16_t constants */
#endif

#ifndef __INT32_C
#define __INT32_C(x)	x /**< Macro for int32_t constants */
#endif

#ifndef INT32_C
#define INT32_C(x)	__INT32_C(x) /**< Macro for int32_t constants */
#endif

#ifndef __UINT32_C
#define __UINT32_C(x)	x ## U /**< Macro for uint32_t constants */
#endif

#ifndef UINT32_C
#define UINT32_C(x)	__UINT32_C(x) /**< Macro for uint32_t constants */
#endif

#ifndef __INT64_C
#define __INT64_C(x)	x /**< Macro for int64_t constants */
#endif

#ifndef INT64_C
#define INT64_C(x)	__INT64_C(x) /**< Macro for int64_t constants */
#endif

#ifndef __UINT64_C
#define __UINT64_C(x)	x ## ULL /**< Macro for uint64_t constants */
#endif

#ifndef UINT64_C
#define UINT64_C(x)	__UINT64_C(x) /**< Macro for uint64_t constants */
#endif

#ifndef __INTMAX_C
#define __INTMAX_C(x)	x /**< Macro for intmax_t constants */
#endif

#ifndef INTMAX_C
#define INTMAX_C(x)	__INTMAX_C(x) /**< Macro for intmax_t constants */
#endif

#ifndef __UINTMAX_C
#define __UINTMAX_C(x)	x ## ULL /**< Macro for uintmax_t constants */
#endif

#ifndef UINTMAX_C
#define UINTMAX_C(x)	__UINTMAX_C(x) /**< Macro for uintmax_t constants */
#endif
#endif /* __CCAC__ */

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_LIB_LIBC_MINIMAL_INCLUDE_STDINT_H_ */
//GST