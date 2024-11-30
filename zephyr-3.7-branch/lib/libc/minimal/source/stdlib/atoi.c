//zephyr-3.7-branch/lib/libc/minimal/source/stdlib/atoi.c
#include <stdlib.h>
#include <ctype.h>

/**
 * @brief Convert a string to an integer
 *
 * @param s Pointer to the string to convert
 * @return The converted value
 */
int atoi(const char *s)
{
	int n = 0;
	int neg = 0;

	/* Skip leading whitespace */
	while (isspace((unsigned char)*s) != 0) {
		s++;
	}

	/* Check for sign */
	switch (*s) {
	case '-':
		neg = 1;
		s++;
		break; /* Artifact to quiet coverity warning */
	case '+':
		s++;
		break;
	default:
		/* Add an empty default with break, this is defensive programming.
		 * Static analysis tool won't raise a violation if default is empty,
		 * but has that comment.
		 */
		break;
	}

	/* Compute n as a negative number to avoid overflow on INT_MIN */
	while (isdigit((unsigned char)*s) != 0) {
		n = 10 * n - (*s++ - '0');
	}

	return neg ? n : -n;
}
//GST