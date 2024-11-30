//zephyr-3.7-branch/lib/libc/minimal/include/string.h

#ifndef ZEPHYR_LIB_LIBC_MINIMAL_INCLUDE_STRING_H_
#define ZEPHYR_LIB_LIBC_MINIMAL_INCLUDE_STRING_H_

#include <stddef.h>
#include <zephyr/toolchain.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Copy a string
 *
 * @param d Destination string
 * @param s Source string
 * @return Pointer to the destination string
 */
extern char *strcpy(char *ZRESTRICT d, const char *ZRESTRICT s);

/**
 * @brief Get a string describing an error number
 *
 * @param errnum Error number
 * @return Pointer to the error string
 */
extern char *strerror(int errnum);

/**
 * @brief Get a string describing an error number (reentrant)
 *
 * @param errnum Error number
 * @param strerrbuf Buffer to store the error string
 * @param buflen Length of the buffer
 * @return 0 on success, or an error number on failure
 */
extern int strerror_r(int errnum, char *strerrbuf, size_t buflen);

/**
 * @brief Copy a string with a maximum length
 *
 * @param d Destination string
 * @param s Source string
 * @param n Maximum number of characters to copy
 * @return Pointer to the destination string
 */
extern char *strncpy(char *ZRESTRICT d, const char *ZRESTRICT s, size_t n);

/**
 * @brief Locate the first occurrence of a character in a string
 *
 * @param s String to search
 * @param c Character to locate
 * @return Pointer to the first occurrence of the character, or NULL if not found
 */
extern char *strchr(const char *s, int c);

/**
 * @brief Locate the last occurrence of a character in a string
 *
 * @param s String to search
 * @param c Character to locate
 * @return Pointer to the last occurrence of the character, or NULL if not found
 */
extern char *strrchr(const char *s, int c);

/**
 * @brief Get the length of a string
 *
 * @param s String to measure
 * @return Length of the string
 */
extern size_t strlen(const char *s);

/**
 * @brief Get the length of a string with a maximum length
 *
 * @param s String to measure
 * @param maxlen Maximum length to measure
 * @return Length of the string
 */
extern size_t strnlen(const char *s, size_t maxlen);

/**
 * @brief Compare two strings
 *
 * @param s1 First string
 * @param s2 Second string
 * @return An integer less than, equal to, or greater than zero if s1 is found, respectively, to be less than, to match, or be greater than s2
 */
extern int strcmp(const char *s1, const char *s2);

/**
 * @brief Compare two strings with a maximum length
 *
 * @param s1 First string
 * @param s2 Second string
 * @param n Maximum number of characters to compare
 * @return An integer less than, equal to, or greater than zero if s1 is found, respectively, to be less than, to match, or be greater than s2
 */
extern int strncmp(const char *s1, const char *s2, size_t n);

/**
 * @brief Tokenize a string
 *
 * @param str String to tokenize
 * @param sep Delimiters
 * @param state Pointer to a char* variable that is used internally by strtok_r to maintain context between successive calls that parse the same string
 * @return Pointer to the next token, or NULL if there are no more tokens
 */
extern char *strtok_r(char *str, const char *sep, char **state);

/**
 * @brief Concatenate two strings
 *
 * @param dest Destination string
 * @param src Source string
 * @return Pointer to the destination string
 */
extern char *strcat(char *ZRESTRICT dest, const char *ZRESTRICT src);

/**
 * @brief Concatenate two strings with a maximum length
 *
 * @param dest Destination string
 * @param src Source string
 * @param n Maximum number of characters to concatenate
 * @return Pointer to the destination string
 */
extern char *strncat(char *ZRESTRICT dest, const char *ZRESTRICT src, size_t n);

/**
 * @brief Locate a substring
 *
 * @param s String to search
 * @param find Substring to locate
 * @return Pointer to the first occurrence of the substring, or NULL if not found
 */
extern char *strstr(const char *s, const char *find);

/**
 * @brief Get the length of a prefix substring
 *
 * @param s String to search
 * @param accept Prefix substring
 * @return Length of the prefix substring
 */
extern size_t strspn(const char *s, const char *accept);

/**
 * @brief Get the length of a segment that does not contain any characters from a set
 *
 * @param s String to search
 * @param reject Set of characters to reject
 * @return Length of the segment
 */
extern size_t strcspn(const char *s, const char *reject);

/**
 * @brief Compare two memory areas
 *
 * @param m1 First memory area
 * @param m2 Second memory area
 * @param n Number of bytes to compare
 * @return An integer less than, equal to, or greater than zero if the first n bytes of m1 is found, respectively, to be less than, to match, or be greater than the first n bytes of m2
 */
extern int memcmp(const void *m1, const void *m2, size_t n);

/**
 * @brief Move memory
 *
 * @param d Destination memory area
 * @param s Source memory area
 * @param n Number of bytes to move
 * @return Pointer to the destination memory area
 */
extern void *memmove(void *d, const void *s, size_t n);

/**
 * @brief Copy memory
 *
 * @param d Destination memory area
 * @param s Source memory area
 * @param n Number of bytes to copy
 * @return Pointer to the destination memory area
 */
extern void *memcpy(void *ZRESTRICT d, const void *ZRESTRICT s, size_t n);

/**
 * @brief Set memory
 *
 * @param buf Memory area to set
 * @param c Value to set
 * @param n Number of bytes to set
 * @return Pointer to the memory area
 */
extern void *memset(void *buf, int c, size_t n);

/**
 * @brief Locate a byte in memory
 *
 * @param s Memory area to search
 * @param c Byte to locate
 * @param n Number of bytes to search
 * @return Pointer to the first occurrence of the byte, or NULL if not found
 */
extern void *memchr(const void *s, int c, size_t n);

#ifdef __cplusplus
}
#endif

#endif  /* ZEPHYR_LIB_LIBC_MINIMAL_INCLUDE_STRING_H_ */
//GST