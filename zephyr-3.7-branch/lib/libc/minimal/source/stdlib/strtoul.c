//zephyr-3.7-branch/lib/libc/minimal/source/stdlib/strtoul.c
#include <limits.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>

/**
 * @brief Convert a string to an unsigned long integer
 *
 * @param nptr Pointer to the string to convert
 * @param endptr Pointer to the end of the parsed string
 * @param base Base of the number in the string
 * @return The converted value
 */
unsigned long strtoul(const char *nptr, char **endptr, register int base)
{
	register const char *s = nptr;
	register unsigned long acc;
	register int c;
	register unsigned long cutoff;
	register int neg = 0, any, cutlim;

	/* Skip white space and pick up leading +/- sign if any.
	 * If base is 0, allow 0x for hex and 0 for octal, else
	 * assume decimal; if base is already 16, allow 0x.
	 */
	do {
		c = *s++;
	} while (isspace((unsigned char)c) != 0);
	if (c == '-') {
		neg = 1;
		c = *s++;
	} else if (c == '+') {
		c = *s++;
	}

	if (((base == 0) || (base == 16)) &&
	    (c == '0') && ((*s == 'x') || (*s == 'X'))) {
		c = s[1];
		s += 2;
		base = 16;
	}

	if (base == 0) {
		base = (c == '0') ? 8 : 10;
	}

	/* Compute the cutoff value between legal numbers and illegal
	 * numbers. That is the largest legal value, divided by the
	 * base. An input number that is greater than this value, if
	 * followed by a legal input character, is too big. One that
	 * is equal to this value may be valid or not; the limit
	 * between valid and invalid numbers is then based on the last
	 * digit. For instance, if the range for longs is
	 * [-2147483648..2147483647] and the input base is 10,
	 * cutoff will be set to 214748364 and cutlim to either
	 * 7 (neg==0) or 8 (neg==1), meaning that if we have accumulated
	 * a value > 214748364, or equal but the next digit is > 7 (or 8),
	 * the number is too big, and we will return a range error.
	 *
	 * Set any if any `digits' consumed; make it negative to indicate
	 * overflow.
	 */
	cutoff = (unsigned long)ULONG_MAX / (unsigned long)base;
	cutlim = (unsigned long)ULONG_MAX % (unsigned long)base;
	for (acc = 0, any = 0;; c = *s++) {
		if (isdigit((unsigned char)c) != 0) {
			c -= '0';
		} else if (isalpha((unsigned char)c) != 0) {
			c -= isupper((unsigned char)c) != 0 ? 'A' - 10 : 'a' - 10;
		} else {
			break;
		}
		if (c >= base) {
			break;
		}
		if ((any < 0) || (acc > cutoff) || ((acc == cutoff) && (c > cutlim))) {
			any = -1;
		} else {
			any = 1;
			acc *= base;
			acc += c;
		}
	}
	if (any < 0) {
		acc = ULONG_MAX;
		errno = ERANGE;
	} else if (neg != 0) {
		acc = -acc;
	}
	if (endptr != NULL) {
		*endptr = (char *)(any ? (s - 1) : nptr);
	}
	return acc;
}
//GST
