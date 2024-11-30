//zephyr-3.7-branch/lib/libc/minimal/include/math.h
#ifndef ZEPHYR_LIB_LIBC_MINIMAL_INCLUDE_MATH_H_
#define ZEPHYR_LIB_LIBC_MINIMAL_INCLUDE_MATH_H_

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(FLT_EVAL_METHOD) && defined(__FLT_EVAL_METHOD__)
#define FLT_EVAL_METHOD __FLT_EVAL_METHOD__
#endif

#if defined FLT_EVAL_METHOD
	#if FLT_EVAL_METHOD == 0
		typedef float  float_t;
		typedef double double_t;
	#elif FLT_EVAL_METHOD == 1
		typedef double float_t;
		typedef double double_t;
	#elif FLT_EVAL_METHOD == 2
		typedef long double float_t;
		typedef long double double_t;
	#else
		/* Implementation-defined. Assume float_t and double_t have
		 * already been defined */
	#endif
#else
	typedef float  float_t;
	typedef double double_t;
#endif

/* Useful constants. */
#define MAXFLOAT    3.40282347e+38F

#define M_E         2.7182818284590452354   /**< e */
#define M_LOG2E     1.4426950408889634074   /**< log_2 e */
#define M_LOG10E    0.43429448190325182765  /**< log_10 e */
#define M_LN2       0.693147180559945309417 /**< log_e 2 */
#define M_LN10      2.30258509299404568402  /**< log_e 10 */
#define M_PI        3.14159265358979323846  /**< pi */
#define M_PI_2      1.57079632679489661923  /**< pi/2 */
#define M_PI_4      0.78539816339744830962  /**< pi/4 */
#define M_1_PI      0.31830988618379067154  /**< 1/pi */
#define M_2_PI      0.63661977236758134308  /**< 2/pi */
#define M_2_SQRTPI  1.12837916709551257390  /**< 2/sqrt(pi) */
#define M_SQRT2     1.41421356237309504880  /**< sqrt(2) */
#define M_SQRT1_2   0.70710678118654752440  /**< 1/sqrt(2) */

/**
 * @brief Compute the square root of a float
 *
 * @param square The value to compute the square root of
 * @return The square root of the input value
 */
float sqrtf(float square);

/**
 * @brief Compute the square root of a double
 *
 * @param square The value to compute the square root of
 * @return The square root of the input value
 */
double sqrt(double square);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_LIB_LIBC_MINIMAL_INCLUDE_MATH_H_ */

//GST