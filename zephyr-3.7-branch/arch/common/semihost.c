//arch/arc/common/include/semilhost.c
#include <string.h>
#include <zephyr/kernel.h>
#include <zephyr/arch/common/semihost.h>

/**
 * @brief Structure for semihosting poll in arguments
 */
struct semihost_poll_in_args {
	long zero;
} __packed;

/**
 * @brief Structure for semihosting open arguments
 */
struct semihost_open_args {
	const char *path;
	long mode;
	long path_len;
} __packed;

/**
 * @brief Structure for semihosting close arguments
 */
struct semihost_close_args {
	long fd;
} __packed;

/**
 * @brief Structure for semihosting flen arguments
 */
struct semihost_flen_args {
	long fd;
} __packed;

/**
 * @brief Structure for semihosting seek arguments
 */
struct semihost_seek_args {
	long fd;
	long offset;
} __packed;

/**
 * @brief Structure for semihosting read arguments
 */
struct semihost_read_args {
	long fd;
	char *buf;
	long len;
} __packed;

/**
 * @brief Structure for semihosting write arguments
 */
struct semihost_write_args {
	long fd;
	const char *buf;
	long len;
} __packed;

/**
 * @brief Poll for input character using semihosting
 *
 * @return The input character
 */
char semihost_poll_in(void)
{
	struct semihost_poll_in_args args = {
		.zero = 0
	};

	return (char)semihost_exec(SEMIHOST_READC, &args);
}

/**
 * @brief Output a character using semihosting
 *
 * @param c The character to output
 */
void semihost_poll_out(char c)
{
	/* WRITEC takes a pointer directly to the character */
	(void)semihost_exec(SEMIHOST_WRITEC, &c);
}

/**
 * @brief Open a file using semihosting
 *
 * @param path The path to the file
 * @param mode The mode to open the file in
 * @return The file descriptor, or an error code
 */
long semihost_open(const char *path, long mode)
{
	struct semihost_open_args args = {
		.path = path,
		.mode = mode,
		.path_len = strlen(path)
	};

	return semihost_exec(SEMIHOST_OPEN, &args);
}

/**
 * @brief Close a file using semihosting
 *
 * @param fd The file descriptor
 * @return 0 on success, or an error code
 */
long semihost_close(long fd)
{
	struct semihost_close_args args = {
		.fd = fd
	};

	return semihost_exec(SEMIHOST_CLOSE, &args);
}

/**
 * @brief Get the length of a file using semihosting
 *
 * @param fd The file descriptor
 * @return The length of the file, or an error code
 */
long semihost_flen(long fd)
{
	struct semihost_flen_args args = {
		.fd = fd
	};

	return semihost_exec(SEMIHOST_FLEN, &args);
}

/**
 * @brief Seek to a position in a file using semihosting
 *
 * @param fd The file descriptor
 * @param offset The offset to seek to
 * @return 0 on success, or an error code
 */
long semihost_seek(long fd, long offset)
{
	struct semihost_seek_args args = {
		.fd = fd,
		.offset = offset
	};

	return semihost_exec(SEMIHOST_SEEK, &args);
}

/**
 * @brief Read from a file using semihosting
 *
 * @param fd The file descriptor
 * @param buf The buffer to read into
 * @param len The number of bytes to read
 * @return The number of bytes read, or an error code
 */
long semihost_read(long fd, void *buf, long len)
{
	struct semihost_read_args args = {
		.fd = fd,
		.buf = buf,
		.len = len
	};
	long ret;

	ret = semihost_exec(SEMIHOST_READ, &args);
	/* EOF condition */
	if (ret == len) {
		ret = -EIO;
	}
	/* All bytes read */
	else if (ret == 0) {
		ret = len;
	}
	return ret;
}

/**
 * @brief Write to a file using semihosting
 *
 * @param fd The file descriptor
 * @param buf The buffer to write from
 * @param len The number of bytes to write
 * @return The number of bytes written, or an error code
 */
long semihost_write(long fd, const void *buf, long len)
{
	struct semihost_write_args args = {
		.fd = fd,
		.buf = buf,
		.len = len
	};

	return semihost_exec(SEMIHOST_WRITE, &args);
}
//GST