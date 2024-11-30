//zephyr-3.7-branch/lib/libc/minimal/source/stdout/stdout_console.c
#include <stdio.h>
#include <zephyr/sys/libc-hooks.h>
#include <zephyr/internal/syscall_handler.h>
#include <string.h>

/**
 * @brief Default stdout hook function
 *
 * @param c Character to output
 * @return EOF
 */
static int _stdout_hook_default(int c)
{
	(void)(c);  /* Prevent warning about unused argument */
	return EOF;
}

static int (*_stdout_hook)(int c) = _stdout_hook_default;

/**
 * @brief Install a custom stdout hook function
 *
 * @param hook Pointer to the custom hook function
 */
void __stdout_hook_install(int (*hook)(int c))
{
	_stdout_hook = hook;
}

/**
 * @brief Implementation of zephyr_fputc
 *
 * @param c Character to output
 * @param stream Output stream
 * @return Character written, or EOF on error
 */
int z_impl_zephyr_fputc(int c, FILE *stream)
{
	return ((stream == stdout) || (stream == stderr)) ? _stdout_hook(c) : EOF;
}

#ifdef CONFIG_USERSPACE
/**
 * @brief Verify and implement zephyr_fputc for userspace
 *
 * @param c Character to output
 * @param stream Output stream
 * @return Character written, or EOF on error
 */
static inline int z_vrfy_zephyr_fputc(int c, FILE *stream)
{
	return z_impl_zephyr_fputc(c, stream);
}
#include <zephyr/syscalls/zephyr_fputc_mrsh.c>
#endif

/**
 * @brief Output a character to a stream
 *
 * @param c Character to output
 * @param stream Output stream
 * @return Character written, or EOF on error
 */
int fputc(int c, FILE *stream)
{
	return zephyr_fputc(c, stream);
}

/**
 * @brief Output a string to a stream
 *
 * @param s String to output
 * @param stream Output stream
 * @return 0 on success, or EOF on error
 */
int fputs(const char *ZRESTRICT s, FILE *ZRESTRICT stream)
{
	int len = strlen(s);
	int ret;

	ret = fwrite(s, 1, len, stream);

	return (len == ret) ? 0 : EOF;
}

#undef putc
/**
 * @brief Output a character to a stream
 *
 * @param c Character to output
 * @param stream Output stream
 * @return Character written, or EOF on error
 */
int putc(int c, FILE *stream)
{
	return zephyr_fputc(c, stream);
}

#undef putchar
/**
 * @brief Output a character to stdout
 *
 * @param c Character to output
 * @return Character written, or EOF on error
 */
int putchar(int c)
{
	return zephyr_fputc(c, stdout);
}

/**
 * @brief Implementation of zephyr_fwrite
 *
 * @param ptr Pointer to the data to write
 * @param size Size of each element
 * @param nitems Number of elements
 * @param stream Output stream
 * @return Number of elements written
 */
size_t z_impl_zephyr_fwrite(const void *ZRESTRICT ptr, size_t size,
			    size_t nitems, FILE *ZRESTRICT stream)
{
	size_t i;
	size_t j;
	const unsigned char *p;

	if (((stream != stdout) && (stream != stderr)) ||
	    (nitems == 0) || (size == 0)) {
		return 0;
	}

	p = ptr;
	i = nitems;
	do {
		j = size;
		do {
			if (_stdout_hook((int) *p++) == EOF) {
				goto done;
			}
			j--;
		} while (j > 0);

		i--;
	} while (i > 0);

done:
	return (nitems - i);
}

#ifdef CONFIG_USERSPACE
/**
 * @brief Verify and implement zephyr_fwrite for userspace
 *
 * @param ptr Pointer to the data to write
 * @param size Size of each element
 * @param nitems Number of elements
 * @param stream Output stream
 * @return Number of elements written
 */
static inline size_t z_vrfy_zephyr_fwrite(const void *ZRESTRICT ptr,
					  size_t size, size_t nitems,
					  FILE *ZRESTRICT stream)
{
	K_OOPS(K_SYSCALL_MEMORY_ARRAY_READ(ptr, nitems, size));
	return z_impl_zephyr_fwrite(ptr, size, nitems, stream);
}
#include <zephyr/syscalls/zephyr_fwrite_mrsh.c>
#endif

/**
 * @brief Write data to a stream
 *
 * @param ptr Pointer to the data to write
 * @param size Size of each element
 * @param nitems Number of elements
 * @param stream Output stream
 * @return Number of elements written
 */
size_t fwrite(const void *ZRESTRICT ptr, size_t size, size_t nitems,
			  FILE *ZRESTRICT stream)
{
	return zephyr_fwrite(ptr, size, nitems, stream);
}

/**
 * @brief Output a string to stdout followed by a newline
 *
 * @param s String to output
 * @return 0 on success, or EOF on error
 */
int puts(const char *s)
{
	if (fputs(s, stdout) == EOF) {
		return EOF;
	}

	return (fputc('\n', stdout) == EOF) ? EOF : 0;
}
//GST