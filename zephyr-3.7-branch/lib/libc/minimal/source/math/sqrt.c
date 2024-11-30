//zephyr-3.7-branch/lib/libc/common/source/thrd/sqrt.c
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <zephyr/sys/util.h>

#define MAX_D_ITERATIONS 8  /**< Maximum number of iterations for convergence */
#define MAX_D_ERROR_COUNT 5 /**< Maximum error count for convergence */
#define EXP_MASK64 GENMASK64(62, 52) /**< Mask for the exponent part of a double */

/**
 * @brief Compute the square root of a double
 *
 * @param square The value to compute the square root of
 * @return The square root of the input value
 */
double sqrt(double square)
{
	int i;
	int64_t exponent;
	double root;
	double last;
	int64_t *p_square = (int64_t *)&square;
	int64_t *p_root = (int64_t *)&root;
	int64_t *p_last = (int64_t *)&last;

	if (square == 0.0) {
		return square;
	}
	if (square < 0.0) {
		return (square - square) / (square - square);
	}

	/* We need a good starting guess so that this will converge quickly.
	 * We can do this by dividing the exponent part of the float by 2.
	 * This assumes IEEE-754 format doubles.
	 */
	exponent = ((*p_square & EXP_MASK64) >> 52) - 1023;
	if (exponent == 0x7FF - 1023) {
		/* The number is a NaN or inf, return NaN or inf */
		return square + square;
	}
	exponent /= 2;
	*p_root = (*p_square & ~EXP_MASK64) | ((exponent + 1023) << 52);

	for (i = 0; i < MAX_D_ITERATIONS; i++) {
		last = root;
		root = (root + square / root) * 0.5;
		if ((*p_root ^ *p_last) < MAX_D_ERROR_COUNT) {
			break;
		}
	}
	return root;
}
//GST