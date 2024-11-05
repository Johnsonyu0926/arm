// Filename: lws_network.h
// Score: 99

#ifndef LWS_NETWORK_H
#define LWS_NETWORK_H

#if defined(LWS_ESP_PLATFORM)
#include <lwip/sockets.h>
#endif

#include <stdint.h>
#include <stddef.h>
#include <libwebsockets.h>

typedef uint8_t lws_route_uidx_t;

typedef struct lws_dns_score {
    uint8_t precedence;
    uint8_t label;
} lws_dns_score_t;

typedef struct lws_route {
    lws_dll2_t list;
    lws_sockaddr46 src;
    lws_sockaddr46 dest;
    lws_sockaddr46 gateway;
    struct lws_route *source;
    lws_dns_score_t score;
    int if_idx;
    int priority;
    int ifa_flags;
    lws_route_uidx_t uidx;
    uint8_t proto;
    uint8_t dest_len;
    uint8_t src_len;
    uint8_t scope;
    uint8_t af;
    uint8_t source_ads:1;
} lws_route_t;

typedef lws_route_t lws_dns_sort_t;

LWS_VISIBLE LWS_EXTERN const char * LWS_WARN_UNUSED_RESULT lws_canonical_hostname(struct lws_context *context);

LWS_VISIBLE LWS_EXTERN void lws_get_peer_addresses(struct lws *wsi, lws_sockfd_type fd, char *name, int name_len, char *rip, int rip_len);

LWS_VISIBLE LWS_EXTERN const char * lws_get_peer_simple(struct lws *wsi, char *name, size_t namelen);

LWS_VISIBLE LWS_EXTERN const char * lws_get_peer_simple_fd(lws_sockfd_type fd, char *name, size_t namelen);

#define LWS_ITOSA_USABLE 0
#define LWS_ITOSA_NOT_EXIST -1
#define LWS_ITOSA_NOT_USABLE -2
#define LWS_ITOSA_BUSY -3

#if !defined(LWS_PLAT_FREERTOS) && !defined(LWS_PLAT_OPTEE)
LWS_VISIBLE LWS_EXTERN int lws_interface_to_sa(int ipv6, const char *ifname, struct sockaddr_in *addr, size_t addrlen);
#endif

LWS_VISIBLE LWS_EXTERN int lws_sa46_compare_ads(const lws_sockaddr46 *sa46a, const lws_sockaddr46 *sa46b);

LWS_VISIBLE LWS_EXTERN int lws_sa46_on_net(const lws_sockaddr46 *sa46a, const lws_sockaddr46 *sa46_net, int net_len);

LWS_VISIBLE LWS_EXTERN int lws_parse_numeric_address(const char *ads, uint8_t *result, size_t max_len);

LWS_VISIBLE LWS_EXTERN int lws_sa46_parse_numeric_address(const char *ads, lws_sockaddr46 *sa46);

LWS_VISIBLE LWS_EXTERN int lws_write_numeric_address(const uint8_t *ads, int size, char *buf, size_t len);

LWS_VISIBLE LWS_EXTERN int lws_sa46_write_numeric_address(lws_sockaddr46 *sa46, char *buf, size_t len);

#endif // LWS_NETWORK_H

// By GST @Date