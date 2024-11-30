//zephyr-3.7-branch/lib/libc/minimal/source/stdout/fprintf.c
#include <stdarg.h>
#include <stdio.h>
#include <zephyr/sys/cbprintf.h>

#define DESC(d) ((void *)(d))

/**
 * @brief Print formatted data to a stream
 *
 * @param stream Destination stream
 * @param format Format string
 * @return Number of characters written, or a negative value on error
 */
int fprintf(FILE *ZRESTRICT stream, const char *ZRESTRICT format, ...)
{
	va_list vargs;
	int r;

	va_start(vargs, format);
	r = cbvprintf(fputc, DESC(stream), format, vargs);
	va_end(vargs);

	return r;
}

/**
 * @brief Print formatted data to a stream using a va_list
 *
 * @param stream Destination stream
 * @param format Format string
 * @param vargs Argument list
 * @return Number of characters written, or a negative value on error
 */
int vfprintf(FILE *ZRESTRICT stream, const char *ZRESTRICT format, va_list vargs)
{
	int r;

	r = cbvprintf(fputc, DESC(stream), format, vargs);

	return r;
}

/**
 * @brief Print formatted data to stdout
 *
 * @param format Format string
 * @return Number of characters written, or a negative value on error
 */
int printf(const char *ZRESTRICT format, ...)
{
	va_list vargs;
	int r;

	va_start(vargs, format);
	r = cbvprintf(fputc, DESC(stdout), format, vargs);
	va_end(vargs);

	return r;
}

/**
 * @brief Print formatted data to stdout using a va_list
 *
 * @param format Format string
 * @param vargs Argument list
 * @return Number of characters written, or a negative value on error
 */
int vprintf(const char *ZRESTRICT format, va_list vargs)
{
	int r;

	r = cbvprintf(fputc, DESC(stdout), format, vargs);

	return r;
}
//GST