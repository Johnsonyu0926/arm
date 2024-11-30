//zephyr-3.7-branch/lib/libc/minimal/include/ctype.h

#ifndef ZEPHYR_LIB_LIBC_MINIMAL_INCLUDE_CTYPE_H_
#define ZEPHYR_LIB_LIBC_MINIMAL_INCLUDE_CTYPE_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Check if a character is an uppercase letter
 *
 * @param a Character to check
 * @return Non-zero if the character is an uppercase letter, zero otherwise
 */
static inline int isupper(int a)
{
	return (int)(((unsigned)(a)-(unsigned)'A') < 26U);
}

/**
 * @brief Check if a character is an alphabetic letter
 *
 * @param c Character to check
 * @return Non-zero if the character is an alphabetic letter, zero otherwise
 */
static inline int isalpha(int c)
{
	return (int)((((unsigned)c|32u)-(unsigned)'a') < 26U);
}

/**
 * @brief Check if a character is a whitespace character
 *
 * @param c Character to check
 * @return Non-zero if the character is a whitespace character, zero otherwise
 */
static inline int isspace(int c)
{
	return (int)(c == (int)' ' || ((unsigned)c-(unsigned)'\t') < 5U);
}

/**
 * @brief Check if a character has a graphical representation
 *
 * @param c Character to check
 * @return Non-zero if the character has a graphical representation, zero otherwise
 */
static inline int isgraph(int c)
{
	return (int)((((unsigned)c) > ' ') &&
			(((unsigned)c) <= (unsigned)'~'));
}

/**
 * @brief Check if a character is printable
 *
 * @param c Character to check
 * @return Non-zero if the character is printable, zero otherwise
 */
static inline int isprint(int c)
{
	return (int)((((unsigned)c) >= ' ') &&
			(((unsigned)c) <= (unsigned)'~'));
}

/**
 * @brief Check if a character is a digit
 *
 * @param a Character to check
 * @return Non-zero if the character is a digit, zero otherwise
 */
static inline int isdigit(int a)
{
	return (int)(((unsigned)(a)-(unsigned)'0') < 10U);
}

/**
 * @brief Check if a character is a hexadecimal digit
 *
 * @param a Character to check
 * @return Non-zero if the character is a hexadecimal digit, zero otherwise
 */
static inline int isxdigit(int a)
{
	unsigned int ua = (unsigned int)a;

	return (int)(((ua - (unsigned)'0') < 10U) ||
			((ua | 32U) - (unsigned)'a' < 6U));
}

/**
 * @brief Convert a character to lowercase
 *
 * @param chr Character to convert
 * @return The lowercase equivalent of the character, or the character itself if it has no lowercase equivalent
 */
static inline int tolower(int chr)
{
	return (chr >= (int)'A' && chr <= (int)'Z') ? (chr + 32) : (chr);
}

/**
 * @brief Convert a character to uppercase
 *
 * @param chr Character to convert
 * @return The uppercase equivalent of the character, or the character itself if it has no uppercase equivalent
 */
static inline int toupper(int chr)
{
	return (int)((chr >= (int)'a' && chr <=
				(int)'z') ? (chr - 32) : (chr));
}

/**
 * @brief Check if a character is alphanumeric
 *
 * @param chr Character to check
 * @return Non-zero if the character is alphanumeric, zero otherwise
 */
static inline int isalnum(int chr)
{
	return (int)(isalpha(chr) || isdigit(chr));
}

/**
 * @brief Check if a character is a control character
 *
 * @param c Character to check
 * @return Non-zero if the character is a control character, zero otherwise
 */
static inline int iscntrl(int c)
{
	return (int)((((unsigned int)c) <= 31U) || (((unsigned int)c) == 127U));
}

#ifdef __cplusplus
}
#endif

#endif  /* ZEPHYR_LIB_LIBC_MINIMAL_INCLUDE_CTYPE_H_ */
//GST
