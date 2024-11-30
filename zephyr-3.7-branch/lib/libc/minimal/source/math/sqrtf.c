//zephyr-3.7-branch/lib/libc/common/math/sqrtf.c
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <zephyr/sys/util.h>

#define MAX_F_ITERATIONS 6  /**< Maximum number of iterations for convergence */
#define MAX_F_ERROR_COUNT 3 /**< Maximum error count for convergence */
#define EXP_MASK32 GENMASK(30, 23) /**< Mask for the exponent part of a float */

/**
 * @brief Compute the square root of a float
 *
 * @param square The value to compute the square root of
 * @return The square root of the input value
 */
float sqrtf(float square)
{
	int i;
	float root;
	float last;
	int32_t exponent;
	int32_t *p_square = (int32_t *)&square;
	int32_t *p_root = (int32_t *)&root;
	int32_t *p_last = (int32_t *)&last;

	if (square == 0.0f) {
		return square;
	}
	if (square < 0.0f) {
		return (square - square) / (square - square);
	}

	/* We need a good starting guess so that this will converge quickly.
	 * We can do this by dividing the exponent part of the float by 2.
	 * This assumes IEEE-754 format floats.
	 */
	exponent = ((*p_square & EXP_MASK32) >> 23) - 127;
	if (exponent == 0xFF - 127) {
		/* The number is a NaN or inf, return NaN or inf */
		return square + square;
	}
	exponent /= 2;
	*p_root = (*p_square & ~EXP_MASK32) | ((exponent + 127) << 23);

	for (i = 0; i < MAX_F_ITERATIONS; i++) {
		last = root;
		root = (root + square / root) * 0.5f;
		if ((*p_root ^ *p_last) < MAX_F_ERROR_COUNT) {
			break;
		}
	}
	return root;
}
//GST
