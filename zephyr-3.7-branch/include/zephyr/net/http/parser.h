// zephyr-3.7-branch/include/zephyr/net/http/parser.h

#ifndef ZEPHYR_INCLUDE_NET_HTTP_PARSER_H_
#define ZEPHYR_INCLUDE_NET_HTTP_PARSER_H_

/* Also update SONAME in the Makefile whenever you change these. */
#define HTTP_PARSER_VERSION_MAJOR 2
#define HTTP_PARSER_VERSION_MINOR 7
#define HTTP_PARSER_VERSION_PATCH 1

#include <sys/types.h>
#if defined(_WIN32) && !defined(__MINGW32__) && \
	(!defined(_MSC_VER) || _MSC_VER < 1600) && !defined(__WINE__)
#include <BaseTsd.h>
#include <stddef.h>
typedef __int8 int8_t;
typedef unsigned __int8 uint8_t;
typedef __int16 int16_t;
typedef unsigned __int16 uint16_t;
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#else
#include <zephyr/types.h>
#include <stddef.h>
#endif
#include <zephyr/net/http/method.h>
#include <zephyr/net/http/parser_state.h>
#include <zephyr/net/http/parser_url.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Maximum header size allowed. If the macro is not defined
 * before including this header then the default is used. To
 * change the maximum header size, define the macro in the build
 * environment (e.g. -DHTTP_MAX_HEADER_SIZE=<value>). To remove
 * the effective limit on the size of the header, define the macro
 * to a very large number (e.g. -DHTTP_MAX_HEADER_SIZE=0x7fffffff)
 */
#ifndef HTTP_MAX_HEADER_SIZE
# define HTTP_MAX_HEADER_SIZE (80 * 1024)
#endif

struct http_parser;
struct http_parser_settings;

/**
 * @brief HTTP data callback type
 *
 * @param parser HTTP parser instance
 * @param at Pointer to the data
 * @param length Length of the data
 *
 * @return Non-zero to indicate an error, zero to continue parsing
 */
typedef int (*http_data_cb)(struct http_parser *, const char *at,
			    size_t length);

/**
 * @brief HTTP callback type
 *
 * @param parser HTTP parser instance
 *
 * @return Non-zero to indicate an error, zero to continue parsing
 */
typedef int (*http_cb)(struct http_parser *);

enum http_parser_type { HTTP_REQUEST, HTTP_RESPONSE, HTTP_BOTH };

/* Flag values for http_parser.flags field */
enum flags {
	F_CHUNKED               = 1 << 0,
	F_CONNECTION_KEEP_ALIVE = 1 << 1,
	F_CONNECTION_CLOSE      = 1 << 2,
	F_CONNECTION_UPGRADE    = 1 << 3,
	F_TRAILING              = 1 << 4,
	F_UPGRADE               = 1 << 5,
	F_SKIPBODY              = 1 << 6,
	F_CONTENTLENGTH         = 1 << 7
};

enum http_errno {
	HPE_OK,
	HPE_CB_message_begin,
	HPE_CB_url,
	HPE_CB_header_field,
	HPE_CB_header_value,
	HPE_CB_headers_complete,
	HPE_CB_body,
	HPE_CB_message_complete,
	HPE_CB_status,
	HPE_CB_chunk_header,
	HPE_CB_chunk_complete,
	HPE_INVALID_EOF_STATE,
	HPE_HEADER_OVERFLOW,
	HPE_CLOSED_CONNECTION,
	HPE_INVALID_VERSION,
	HPE_INVALID_STATUS,
	HPE_INVALID_METHOD,
	HPE_INVALID_URL,
	HPE_INVALID_HOST,
	HPE_INVALID_PORT,
	HPE_INVALID_PATH,
	HPE_INVALID_QUERY_STRING,
	HPE_INVALID_FRAGMENT,
	HPE_LF_EXPECTED,
	HPE_INVALID_HEADER_TOKEN,
	HPE_INVALID_CONTENT_LENGTH,
	HPE_UNEXPECTED_CONTENT_LENGTH,
	HPE_INVALID_CHUNK_SIZE,
	HPE_INVALID_CONSTANT,
	HPE_INVALID_INTERNAL_STATE,
	HPE_STRICT,
	HPE_PAUSED,
	HPE_UNKNOWN
};

/* Get an http_errno value from an http_parser */
#define HTTP_PARSER_ERRNO(p)            ((enum http_errno) (p)->http_errno)

struct http_parser {
	/** PRIVATE **/
	unsigned int type : 2;         /* enum http_parser_type */
	unsigned int flags : 8;		/* F_xxx values from 'flags' enum;
					 * semi-public
					 */
	unsigned int state : 7;        /* enum state from http_parser.c */
	unsigned int header_state : 7; /* enum header_state from http_parser.c
					*/
	unsigned int index : 7;        /* index into current matcher */
	unsigned int lenient_http_headers : 1;

	uint32_t nread;          /* # bytes read in various scenarios */
	uint64_t content_length; /* # bytes in body (0 if no Content-Length
				  * header)
				  */
	/** READ-ONLY **/
	unsigned short http_major;
	unsigned short http_minor;
	unsigned int status_code : 16; /* responses only */
	unsigned int method : 8;       /* requests only */
	unsigned int http_errno : 7;

	/* 1 = Upgrade header was present and the parser has exited because of
	 * that.
	 * 0 = No upgrade header present.
	 * Should be checked when http_parser_execute() returns in addition to
	 * error checking.
	 */
	unsigned int upgrade : 1;

	/** PUBLIC **/
	void *data; /* A pointer to get hook to the "connection" or "socket"
		     * object
		     */

	/* Remote socket address of http connection, where parser can initiate
	 * replies if necessary.
	 */
	const struct sockaddr *addr;
};

struct http_parser_settings {
	http_cb      on_message_begin;
	http_data_cb on_url;
	http_data_cb on_status;
	http_data_cb on_header_field;
	http_data_cb on_header_value;
	http_cb      on_headers_complete;
	http_data_cb on_body;
	http_cb      on_message_complete;
	/* When on_chunk_header is called, the current chunk length is stored
	 * in parser->content_length.
	 */
	http_cb      on_chunk_header;
	http_cb      on_chunk_complete;
};

/**
 * @brief Returns the library version.
 *
 * Bits 16-23 contain the major version number,
 * bits 8-15 the minor version number and bits 0-7 the patch level.
 *
 * @return Library version
 */
unsigned long http_parser_version(void);

/**
 * @brief Initialize the HTTP parser
 *
 * @param parser Pointer to the HTTP parser
 * @param type Type of the parser (HTTP_REQUEST, HTTP_RESPONSE, HTTP_BOTH)
 */
void http_parser_init(struct http_parser *parser, enum http_parser_type type);

/**
 * @brief Initialize http_parser_settings members to 0
 *
 * @param settings Pointer to the HTTP parser settings
 */
void http_parser_settings_init(struct http_parser_settings *settings);

/**
 * @brief Executes the parser
 *
 * @param parser Pointer to the HTTP parser
 * @param settings Pointer to the HTTP parser settings
 * @param data Pointer to the data to parse
 * @param len Length of the data
 *
 * @return Number of parsed bytes, sets `parser->http_errno` on error
 */
size_t http_parser_execute(struct http_parser *parser,
			   const struct http_parser_settings *settings,
			   const char *data, size_t len);

/**
 * @brief Check if the connection should be kept alive
 *
 * If http_should_keep_alive() in the on_headers_complete or
 * on_message_complete callback returns 0, then this should be
 * the last message on the connection.
 * If you are the server, respond with the "Connection: close" header.
 * If you are the client, close the connection.
 *
 * @param parser Pointer to the HTTP parser
 *
 * @return 1 if the connection should be kept alive, 0 otherwise
 */
int http_should_keep_alive(const struct http_parser *parser);

/**
 * @brief Returns a string version of the HTTP method
 *
 * @param m HTTP method
 *
 * @return String version of the HTTP method
 */
const char *http_method_str(enum http_method m);

/**
 * @brief Return a string name of the given error
 *
 * @param err HTTP error
 *
 * @return String name of the error
 */
const char *http_errno_name(enum http_errno err);

/**
 * @brief Return a string description of the given error
 *
 * @param err HTTP error
 *
 * @return String description of the error
 */
const char *http_errno_description(enum http_errno err);

/**
 * @brief Pause or un-pause the parser
 *
 * @param parser Pointer to the HTTP parser
 * @param paused Non-zero to pause, zero to un-pause
 */
void http_parser_pause(struct http_parser *parser, int paused);

/**
 * @brief Checks if this is the final chunk of the body
 *
 * @param parser Pointer to the HTTP parser
 *
 * @return 1 if this is the final chunk, 0 otherwise
 */
int http_body_is_final(const struct http_parser *parser);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_NET_HTTP_PARSER_H_ */
//GST