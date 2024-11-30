//zephyr-3.7-branch/lib/os/cbprintf.c
#include <stdarg.h>
#include <stddef.h>
#include <zephyr/sys/cbprintf.h>

/**
 * @brief cbprintf implementation
 *
 * @param out Output callback function
 * @param ctx Context for the callback function
 * @param format Format string
 * @param ... Additional arguments
 *
 * @return Number of characters printed
 */
int cbprintf(cbprintf_cb out, void *ctx, const char *format, ...)
{
	va_list ap;
	int rc;

	va_start(ap, format);
	rc = cbvprintf(out, ctx, format, ap);
	va_end(ap);

	return rc;
}

#if defined(CONFIG_CBPRINTF_LIBC_SUBSTS)

#include <stdio.h>

/* Context for sn* variants is the next space in the buffer, and the buffer
 * end.
 */
struct str_ctx {
	char *dp;
	char *const dpe;
};

/**
 * @brief Output character to string buffer
 *
 * @param c Character to output
 * @param ctx Context for the callback function
 *
 * @return Character output
 */
static int str_out(int c, void *ctx)
{
	struct str_ctx *scp = ctx;

	/* s*printf must return the number of characters that would be
	 * output, even if they don't all fit, so conditionally store
	 * and unconditionally succeed.
	 */
	if (scp->dp < scp->dpe) {
		*(scp->dp++) = c;
	}

	return c;
}

/**
 * @brief fprintfcb implementation
 *
 * @param stream Output stream
 * @param format Format string
 * @param ... Additional arguments
 *
 * @return Number of characters printed
 */
int fprintfcb(FILE *stream, const char *format, ...)
{
	va_list ap;
	int rc;

	va_start(ap, format);
	rc = vfprintfcb(stream, format, ap);
	va_end(ap);

	return rc;
}

/**
 * @brief vfprintfcb implementation
 *
 * @param stream Output stream
 * @param format Format string
 * @param ap Variable argument list
 *
 * @return Number of characters printed
 */
int vfprintfcb(FILE *stream, const char *format, va_list ap)
{
	return cbvprintf(fputc, stream, format, ap);
}

/**
 * @brief printfcb implementation
 *
 * @param format Format string
 * @param ... Additional arguments
 *
 * @return Number of characters printed
 */
int printfcb(const char *format, ...)
{
	va_list ap;
	int rc;

	va_start(ap, format);
	rc = vprintfcb(format, ap);
	va_end(ap);

	return rc;
}

/**
 * @brief vprintfcb implementation
 *
 * @param format Format string
 * @param ap Variable argument list
 *
 * @return Number of characters printed
 */
int vprintfcb(const char *format, va_list ap)
{
	return cbvprintf(fputc, stdout, format, ap);
}

/**
 * @brief snprintfcb implementation
 *
 * @param str Output string buffer
 * @param size Size of the buffer
 * @param format Format string
 * @param ... Additional arguments
 *
 * @return Number of characters printed
 */
int snprintfcb(char *str, size_t size, const char *format, ...)
{
	va_list ap;
	int rc;

	va_start(ap, format);
	rc = vsnprintfcb(str, size, format, ap);
	va_end(ap);

	return rc;
}

/**
 * @brief vsnprintfcb implementation
 *
 * @param str Output string buffer
 * @param size Size of the buffer
 * @param format Format string
 * @param ap Variable argument list
 *
 * @return Number of characters printed
 */
int vsnprintfcb(char *str, size_t size, const char *format, va_list ap)
{
	struct str_ctx ctx = {
		.dp = str,
		.dpe = str + size,
	};
	int rv = cbvprintf(str_out, &ctx, format, ap);

	if (ctx.dp < ctx.dpe) {
		ctx.dp[0] = 0;
	} else {
		ctx.dp[-1] = 0;
	}

	return rv;
}

#endif /* CONFIG_CBPRINTF_LIBC_SUBSTS */
//GST
