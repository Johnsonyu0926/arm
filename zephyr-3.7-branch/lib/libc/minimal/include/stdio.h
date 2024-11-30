//zephyr-3.7-branch/lib/libc/minimal/include/stdio.h
#ifndef ZEPHYR_LIB_LIBC_MINIMAL_INCLUDE_STDIO_H_
#define ZEPHYR_LIB_LIBC_MINIMAL_INCLUDE_STDIO_H_

#include <zephyr/toolchain.h>
#include <stdarg.h>     /* Needed to get definition of va_list */
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(__FILE_defined)
#define __FILE_defined
typedef int  FILE; /**< File type */
#endif

#if !defined(EOF)
#define EOF  (-1) /**< End of file */
#endif

#define stdin  ((FILE *) 1) /**< Standard input */
#define stdout ((FILE *) 2) /**< Standard output */
#define stderr ((FILE *) 3) /**< Standard error */

#define SEEK_SET 0 /**< Seek from beginning of file */
#define SEEK_CUR 1 /**< Seek from current position */
#define SEEK_END 2 /**< Seek from end of file */

/**
 * @brief Print formatted data to stdout
 *
 * @param format Format string
 * @return Number of characters printed, or a negative value on error
 */
int __printf_like(1, 2) printf(const char *ZRESTRICT format, ...);

/**
 * @brief Print formatted data to a string
 *
 * @param str Destination string
 * @param len Maximum number of characters to write
 * @param format Format string
 * @return Number of characters written, or a negative value on error
 */
int __printf_like(3, 4) snprintf(char *ZRESTRICT str, size_t len,
				 const char *ZRESTRICT format, ...);

/**
 * @brief Print formatted data to a string
 *
 * @param str Destination string
 * @param format Format string
 * @return Number of characters written, or a negative value on error
 */
int __printf_like(2, 3) sprintf(char *ZRESTRICT str,
				const char *ZRESTRICT format, ...);

/**
 * @brief Print formatted data to a stream
 *
 * @param stream Destination stream
 * @param format Format string
 * @return Number of characters written, or a negative value on error
 */
int __printf_like(2, 3) fprintf(FILE *ZRESTRICT stream,
				const char *ZRESTRICT format, ...);

/**
 * @brief Print formatted data to stdout using a va_list
 *
 * @param format Format string
 * @param list Argument list
 * @return Number of characters printed, or a negative value on error
 */
int __printf_like(1, 0) vprintf(const char *ZRESTRICT format, va_list list);

/**
 * @brief Print formatted data to a string using a va_list
 *
 * @param str Destination string
 * @param len Maximum number of characters to write
 * @param format Format string
 * @param list Argument list
 * @return Number of characters written, or a negative value on error
 */
int __printf_like(3, 0) vsnprintf(char *ZRESTRICT str, size_t len,
				  const char *ZRESTRICT format,
				  va_list list);

/**
 * @brief Print formatted data to a string using a va_list
 *
 * @param str Destination string
 * @param format Format string
 * @param list Argument list
 * @return Number of characters written, or a negative value on error
 */
int __printf_like(2, 0) vsprintf(char *ZRESTRICT str,
				 const char *ZRESTRICT format, va_list list);

/**
 * @brief Print formatted data to a stream using a va_list
 *
 * @param stream Destination stream
 * @param format Format string
 * @param ap Argument list
 * @return Number of characters written, or a negative value on error
 */
int __printf_like(2, 0) vfprintf(FILE *ZRESTRICT stream,
				 const char *ZRESTRICT format,
				 va_list ap);

/**
 * @brief Print an error message to stderr
 *
 * @param s Error message
 */
void perror(const char *s);

/**
 * @brief Write a string to stdout
 *
 * @param s String to write
 * @return Non-negative value on success, or EOF on error
 */
int puts(const char *s);

/**
 * @brief Write a character to a stream
 *
 * @param c Character to write
 * @param stream Destination stream
 * @return Character written, or EOF on error
 */
int fputc(int c, FILE *stream);

/**
 * @brief Write a string to a stream
 *
 * @param s String to write
 * @param stream Destination stream
 * @return Non-negative value on success, or EOF on error
 */
int fputs(const char *ZRESTRICT s, FILE *ZRESTRICT stream);

/**
 * @brief Write data to a stream
 *
 * @param ptr Pointer to the data
 * @param size Size of each element
 * @param nitems Number of elements
 * @param stream Destination stream
 * @return Number of elements written
 */
size_t fwrite(const void *ZRESTRICT ptr, size_t size, size_t nitems,
	      FILE *ZRESTRICT stream);

#define putc(c, stream) fputc(c, stream) /**< Write a character to a stream */
#define putchar(c) putc(c, stdout) /**< Write a character to stdout */

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_LIB_LIBC_MINIMAL_INCLUDE_STDIO_H_ */
//GST