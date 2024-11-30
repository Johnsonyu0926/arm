//zephyr-3.7-branch/lib/libc/minimal/source/string/strerror.c
#include <errno.h>
#include <string.h>
#include <zephyr/sys/util.h>

/*
 * See scripts/build/gen_strerror_table.py
 *
 * #define sys_nerr N
 * const char *const sys_errlist[sys_nerr];
 * const uint8_t sys_errlen[sys_nerr];
 */
#include "libc/minimal/strerror_table.h"

/**
 * @brief Get a string describing an error number
 *
 * @param errnum Error number
 * @return Pointer to the error string
 *
 * See https://pubs.opengroup.org/onlinepubs/9699919799/functions/strerror.html
 */
char *strerror(int errnum)
{
	if (IS_ENABLED(CONFIG_MINIMAL_LIBC_STRING_ERROR_TABLE) &&
	    errnum >= 0 && errnum < sys_nerr) {
		return (char *)sys_errlist[errnum];
	}

	return (char *) "";
}

/**
 * @brief Get a string describing an error number (reentrant)
 *
 * @param errnum Error number
 * @param strerrbuf Buffer to store the error string
 * @param buflen Length of the buffer
 * @return 0 on success, or an error number on failure
 *
 * See https://pubs.opengroup.org/onlinepubs/9699919799/functions/strerror_r.html
 */
int strerror_r(int errnum, char *strerrbuf, size_t buflen)
{
	const char *msg;
	size_t msg_len;

	if (errnum >= 0 && errnum < sys_nerr) {
		if (IS_ENABLED(CONFIG_MINIMAL_LIBC_STRING_ERROR_TABLE)) {
			msg = sys_errlist[errnum];
			msg_len = sys_errlen[errnum];
		} else {
			msg = "";
			msg_len = 1;
		}

		if (buflen < msg_len) {
			return ERANGE;
		}

		strncpy(strerrbuf, msg, msg_len);
	}

	if (errnum < 0 || errnum >= sys_nerr) {
		return EINVAL;
	}

	return 0;
}
//GST