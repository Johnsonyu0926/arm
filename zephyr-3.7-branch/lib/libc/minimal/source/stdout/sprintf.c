//zephyr-3.7-branch/lib/libc/minimal/source/stdout/sprintf.c
#include <stdarg.h>
#include <stdio.h>
#include <zephyr/sys/cbprintf.h>

struct emitter {
	char *ptr;
	int len;
};

/**
 * @brief Output a character to the emitter
 *
 * @param c Character to output
 * @param p Pointer to the emitter
 * @return 0 to indicate continue
 */
static int sprintf_out(int c, struct emitter *p)
{
	if (p->len > 1) { /* need to reserve a byte for EOS */
		*(p->ptr) = c;
		p->ptr += 1;
		p->len -= 1;
	}
	return 0; /* indicate keep going so we get the total count */
}

/**
 * @brief Print formatted data to a string with a maximum length
 *
 * @param str Destination string
 * @param len Maximum number of characters to write
 * @param format Format string
 * @return Number of characters written, or a negative value on error
 */
int snprintf(char *ZRESTRICT str, size_t len, const char *ZRESTRICT format, ...)
{
	va_list vargs;
	struct emitter p;
	int r;
	char dummy;

	if (len == 0) {
		str = &dummy; /* write final NUL to dummy, can't change *s */
	}

	p.ptr = str;
	p.len = (int) len;

	va_start(vargs, format);
	r = cbvprintf(sprintf_out, (void *) (&p), format, vargs);
	va_end(vargs);

	*(p.ptr) = 0;
	return r;
}

/**
 * @brief Print formatted data to a string
 *
 * @param str Destination string
 * @param format Format string
 * @return Number of characters written, or a negative value on error
 */
int sprintf(char *ZRESTRICT str, const char *ZRESTRICT format, ...)
{
	va_list vargs;
	struct emitter p;
	int r;

	p.ptr = str;
	p.len = (int) 0x7fffffff; /* allow up to "maxint" characters */

	va_start(vargs, format);
	r = cbvprintf(sprintf_out, (void *) (&p), format, vargs);
	va_end(vargs);

	*(p.ptr) = 0;
	return r;
}

/**
 * @brief Print formatted data to a string with a maximum length using a va_list
 *
 * @param str Destination string
 * @param len Maximum number of characters to write
 * @param format Format string
 * @param vargs Argument list
 * @return Number of characters written, or a negative value on error
 */
int vsnprintf(char *ZRESTRICT str, size_t len, const char *ZRESTRICT format, va_list vargs)
{
	struct emitter p;
	int r;
	char dummy;

	if (len == 0) {
		str = &dummy; /* write final NUL to dummy, can't change * *s */
	}

	p.ptr = str;
	p.len = (int) len;

	r = cbvprintf(sprintf_out, (void *) (&p), format, vargs);

	*(p.ptr) = 0;
	return r;
}

/**
 * @brief Print formatted data to a string using a va_list
 *
 * @param str Destination string
 * @param format Format string
 * @param vargs Argument list
 * @return Number of characters written, or a negative value on error
 */
int vsprintf(char *ZRESTRICT str, const char *ZRESTRICT format, va_list vargs)
{
	struct emitter p;
	int r;

	p.ptr = str;
	p.len = (int) 0x7fffffff; /* allow up to "maxint" characters */

	r = cbvprintf(sprintf_out, (void *) (&p), format, vargs);

	*(p.ptr) = 0;
	return r;
}
//GST