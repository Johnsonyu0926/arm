//zephyr-3.7-branch/lib/libc/minimal/source/string/string.c
#include <string.h>
#include <stdint.h>
#include <sys/types.h>

/**
 * @brief Copy a string
 *
 * @param d Destination buffer
 * @param s Source string
 * @return Pointer to destination buffer
 */
char *strcpy(char *ZRESTRICT d, const char *ZRESTRICT s)
{
	char *dest = d;

	while (*s != '\0') {
		*d = *s;
		d++;
		s++;
	}

	*d = '\0';

	return dest;
}

/**
 * @brief Copy part of a string
 *
 * @param d Destination buffer
 * @param s Source string
 * @param n Maximum number of characters to copy
 * @return Pointer to destination buffer
 */
char *strncpy(char *ZRESTRICT d, const char *ZRESTRICT s, size_t n)
{
	char *dest = d;

	while ((n > 0) && (*s != '\0')) {
		*d = *s;
		s++;
		d++;
		n--;
	}

	while (n > 0) {
		*d = '\0';
		d++;
		n--;
	}

	return dest;
}

/**
 * @brief Locate the first occurrence of a character in a string
 *
 * @param s String to search
 * @param c Character to locate
 * @return Pointer to the first occurrence of the character, or NULL if not found
 */
char *strchr(const char *s, int c)
{
	char tmp = (char) c;

	while ((*s != tmp) && (*s != '\0')) {
		s++;
	}

	return (*s == tmp) ? (char *) s : NULL;
}

/**
 * @brief Locate the last occurrence of a character in a string
 *
 * @param s String to search
 * @param c Character to locate
 * @return Pointer to the last occurrence of the character, or NULL if not found
 */
char *strrchr(const char *s, int c)
{
	char *match = NULL;

	do {
		if (*s == (char)c) {
			match = (char *)s;
		}
	} while (*s++ != '\0');

	return match;
}

/**
 * @brief Get the length of a string
 *
 * @param s String to measure
 * @return Number of bytes in the string
 */
size_t strlen(const char *s)
{
	size_t n = 0;

	while (*s != '\0') {
		s++;
		n++;
	}

	return n;
}

/**
 * @brief Compare two strings
 *
 * @param s1 First string
 * @param s2 Second string
 * @return Negative value if s1 < s2, 0 if s1 == s2, positive value if s1 > s2
 */
int strcmp(const char *s1, const char *s2)
{
	while ((*s1 == *s2) && (*s1 != '\0')) {
		s1++;
		s2++;
	}

	return *s1 - *s2;
}

/**
 * @brief Compare part of two strings
 *
 * @param s1 First string
 * @param s2 Second string
 * @param n Maximum number of characters to compare
 * @return Negative value if s1 < s2, 0 if s1 == s2, positive value if s1 > s2
 */
int strncmp(const char *s1, const char *s2, size_t n)
{
	while ((n > 0) && (*s1 == *s2) && (*s1 != '\0')) {
		s1++;
		s2++;
		n--;
	}

	return (n == 0) ? 0 : (*s1 - *s2);
}

/**
 * @brief Separate a string into tokens
 *
 * @param str String to tokenize
 * @param sep Delimiters
 * @param state Pointer to a char* variable that is used internally by strtok_r to maintain context between successive calls that parse the same string
 * @return Pointer to the next token, or NULL if there are no more tokens
 */
char *strtok_r(char *str, const char *sep, char **state)
{
	char *start, *end;

	start = str ? str : *state;

	/* Skip leading delimiters */
	while (*start && strchr(sep, *start)) {
		start++;
	}

	if (*start == '\0') {
		*state = start;
		return NULL;
	}

	/* Look for token chars */
	end = start;
	while (*end && !strchr(sep, *end)) {
		end++;
	}

	if (*end != '\0') {
		*end = '\0';
		*state = end + 1;
	} else {
		*state = end;
	}

	return start;
}

/**
 * @brief Concatenate two strings
 *
 * @param dest Destination string
 * @param src Source string
 * @return Pointer to the destination string
 */
char *strcat(char *ZRESTRICT dest, const char *ZRESTRICT src)
{
	strcpy(dest + strlen(dest), src);
	return dest;
}

/**
 * @brief Concatenate two strings with a maximum length
 *
 * @param dest Destination string
 * @param src Source string
 * @param n Maximum number of characters to concatenate
 * @return Pointer to the destination string
 */
char *strncat(char *ZRESTRICT dest, const char *ZRESTRICT src, size_t n)
{
	char *orig_dest = dest;
	size_t len = strlen(dest);

	dest += len;
	while ((n-- > 0) && (*src != '\0')) {
		*dest++ = *src++;
	}
	*dest = '\0';

	return orig_dest;
}

/**
 * @brief Compare two memory areas
 *
 * @param m1 First memory area
 * @param m2 Second memory area
 * @param n Number of bytes to compare
 * @return Negative value if m1 < m2, 0 if m1 == m2, positive value if m1 > m2
 */
int memcmp(const void *m1, const void *m2, size_t n)
{
	const char *c1 = m1;
	const char *c2 = m2;

	if (!n) {
		return 0;
	}

	while ((--n > 0) && (*c1 == *c2)) {
		c1++;
		c2++;
	}

	return *c1 - *c2;
}

/**
 * @brief Copy bytes in memory with overlapping areas
 *
 * @param d Destination buffer
 * @param s Source buffer
 * @param n Number of bytes to copy
 * @return Pointer to destination buffer
 */
void *memmove(void *d, const void *s, size_t n)
{
	char *dest = d;
	const char *src  = s;

	if ((size_t) (dest - src) < n) {
		/*
		 * The <src> buffer overlaps with the start of the <dest> buffer.
		 * Copy backwards to prevent the premature corruption of <src>.
		 */
		while (n > 0) {
			n--;
			dest[n] = src[n];
		}
	} else {
		/* It is safe to perform a forward-copy */
		while (n > 0) {
			*dest = *src;
			dest++;
			src++;
			n--;
		}
	}

	return d;
}

/**
 * @brief Copy bytes in memory
 *
 * @param d Destination buffer
 * @param s Source buffer
 * @param n Number of bytes to copy
 * @return Pointer to destination buffer
 */
void *memcpy(void *ZRESTRICT d, const void *ZRESTRICT s, size_t n)
{
	/* Attempt word-sized copying only if buffers have identical alignment */
	unsigned char *d_byte = (unsigned char *)d;
	const unsigned char *s_byte = (const unsigned char *)s;

#if !defined(CONFIG_MINIMAL_LIBC_OPTIMIZE_STRING_FOR_SIZE)
	const uintptr_t mask = sizeof(mem_word_t) - 1;

	if ((((uintptr_t)d ^ (uintptr_t)s_byte) & mask) == 0) {
		/* Do byte-sized copying until word-aligned or finished */
		while (((uintptr_t)d_byte) & mask) {
			if (n == 0) {
				return d;
			}
			*(d_byte++) = *(s_byte++);
			n--;
		}

		/* Do word-sized copying as long as possible */
		mem_word_t *d_word = (mem_word_t *)d_byte;
		const mem_word_t *s_word = (const mem_word_t *)s_byte;

		while (n >= sizeof(mem_word_t)) {
			*(d_word++) = *(s_word++);
			n -= sizeof(mem_word_t);
		}

		d_byte = (unsigned char *)d_word;
		s_byte = (unsigned char *)s_word;
	}
#endif

	/* Do byte-sized copying until finished */
	while (n > 0) {
		*(d_byte++) = *(s_byte++);
		n--;
	}

	return d;
}

/**
 * @brief Set bytes in memory
 *
 * @param buf Buffer to set
 * @param c Value to set
 * @param n Number of bytes to set
 * @return Pointer to start of buffer
 */
void *memset(void *buf, int c, size_t n)
{
	/* Do byte-sized initialization until word-aligned or finished */
	unsigned char *d_byte = (unsigned char *)buf;
	unsigned char c_byte = (unsigned char)c;

#if !defined(CONFIG_MINIMAL_LIBC_OPTIMIZE_STRING_FOR_SIZE)
	while (((uintptr_t)d_byte) & (sizeof(mem_word_t) - 1)) {
		if (n == 0) {
			return buf;
		}
		*(d_byte++) = c_byte;
		n--;
	}

	/* Do word-sized initialization as long as possible */
	mem_word_t *d_word = (mem_word_t *)d_byte;
	mem_word_t c_word = (mem_word_t)c_byte;

	c_word |= c_word << 8;
	c_word |= c_word << 16;
#if Z_MEM_WORD_T_WIDTH > 32
	c_word |= c_word << 32;
#endif

	while (n >= sizeof(mem_word_t)) {
		*(d_word++) = c_word;
		n -= sizeof(mem_word_t);
	}

	/* Do byte-sized initialization until finished */
	d_byte = (unsigned char *)d_word;
#endif

	while (n > 0) {
		*(d_byte++) = c_byte;
		n--;
	}

	return buf;
}

/**
 * @brief Scan byte in memory
 *
 * @param s Memory area to search
 * @param c Byte to locate
 * @param n Number of bytes to search
 * @return Pointer to the first occurrence of the byte, or NULL if not found
 */
void *memchr(const void *s, int c, size_t n)
{
	if (n != 0) {
		const unsigned char *p = s;

		do {
			if (*p++ == (unsigned char)c) {
				return ((void *)(p - 1));
			}
		} while (--n != 0);
	}

	return NULL;
}
//GST