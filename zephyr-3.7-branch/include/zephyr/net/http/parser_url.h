// zephyr-3.7-branch/include/zephyr/net/http/parser_url.h

#ifndef ZEPHYR_INCLUDE_NET_HTTP_PARSER_URL_H_
#define ZEPHYR_INCLUDE_NET_HTTP_PARSER_URL_H_

#include <sys/types.h>
#include <zephyr/types.h>
#include <stddef.h>
#include <zephyr/net/http/parser_state.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief HTTP parser URL fields
 */
enum http_parser_url_fields {
	UF_SCHEMA = 0,
	UF_HOST = 1,
	UF_PORT = 2,
	UF_PATH = 3,
	UF_QUERY = 4,
	UF_FRAGMENT = 5,
	UF_USERINFO = 6,
	UF_MAX = 7
};

/**
 * @brief Result structure for http_parser_url_fields().
 *
 * Callers should index into field_data[] with UF_* values if field_set
 * has the relevant (1 << UF_*) bit set. As a courtesy to clients (and
 * because we probably have padding left over), we convert any port to
 * a uint16_t.
 */
struct http_parser_url {
	uint16_t field_set;           /**< Bitmask of (1 << UF_*) values */
	uint16_t port;                /**< Converted UF_PORT string */

	struct {
		uint16_t off;               /**< Offset into buffer in which field starts */
		uint16_t len;               /**< Length of run in buffer */
	} field_data[UF_MAX];
};

/**
 * @brief Parse a URL character
 *
 * @param s Current state
 * @param ch Character to parse
 *
 * @return New state
 */
enum state parse_url_char(enum state s, const char ch);

/**
 * @brief Initialize all http_parser_url members to 0
 *
 * @param u Pointer to http_parser_url structure
 */
void http_parser_url_init(struct http_parser_url *u);

/**
 * @brief Parse a URL
 *
 * @param buf URL buffer
 * @param buflen Length of the buffer
 * @param is_connect Is this a CONNECT request
 * @param u Pointer to http_parser_url structure
 *
 * @return 0 on success, nonzero on failure
 */
int http_parser_parse_url(const char *buf, size_t buflen,
			  int is_connect, struct http_parser_url *u);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_NET_HTTP_PARSER_URL_H_ */
//GST