#define LWS_WIFI_MAX_SCAN_TRACK 16
#define LWS_ETH_ALEN 6

typedef uint8_t lws_wifi_ch_t;
typedef int8_t lws_wifi_rssi_t;
struct lws_netdev_instance;

typedef enum {
	LWSNDTYP_UNKNOWN,
	LWSNDTYP_WIFI,
	LWSNDTYP_ETH,
} lws_netdev_type_t;

typedef struct lws_netdev_config {
	void *plat_config;
} lws_netdev_config_t;

typedef struct lws_netdev_ops {
	struct lws_netdev_instance *(*create)(struct lws_context *ctx, const struct lws_netdev_ops *ops, const char *name, void *platinfo);
	int (*configure)(struct lws_netdev_instance *nd, lws_netdev_config_t *config);
	int (*up)(struct lws_netdev_instance *nd);
	int (*down)(struct lws_netdev_instance *nd);
	int (*event)(struct lws_netdev_instance *nd, lws_usec_t timestamp, void *buf, size_t len);
	void (*destroy)(struct lws_netdev_instance **pnd);
	int (*connect)(struct lws_netdev_instance *wnd, const char *ssid, const char *passphrase, uint8_t *bssid);
	void (*scan)(struct lws_netdev_instance *nd);
} lws_netdev_ops_t;

typedef struct lws_netdevs {
	lws_dll2_owner_t owner;
	lws_dll2_owner_t owner_creds;
	struct lwsac *ac_creds;
	lws_settings_instance_t *si;
	lws_sockaddr46 sa46_dns_resolver;
	uint8_t refcount_creds;
} lws_netdevs_t;

typedef struct lws_netdev_instance {
	const char *name;
	const lws_netdev_ops_t *ops;
	void *platinfo;
	lws_dll2_t list;
	uint8_t mac[LWS_ETH_ALEN];
	uint8_t type;
} lws_netdev_instance_t;

enum {
	LNDIW_ALG_OPEN,
	LNDIW_ALG_WPA2,
	LNDIW_MODE_STA = (1 << 0),
	LNDIW_MODE_AP = (1 << 1),
	LNDIW_UP = (1 << 7),
	LNDIW_ACQ_IPv4 = (1 << 0),
	LNDIW_ACQ_IPv6 = (1 << 1),
};

typedef enum {
	LWSNDVWIFI_STATE_INITIAL,
	LWSNDVWIFI_STATE_SCAN,
	LWSNDVWIFI_STATE_AP,
	LWSNDVWIFI_STATE_AP_SCAN,
	LWSNDVWIFI_STATE_STAT_GRP_AP,
	LWSNDVWIFI_STATE_STAT_GRP_AP_SCAN,
	LWSNDVWIFI_STATE_STAT,
	LWSNDVWIFI_STATE_STAT_HAPPY,
} lws_netdev_wifi_state_t;

typedef struct lws_wifi_creds {
	lws_dll2_t list;
	uint8_t bssid[LWS_ETH_ALEN];
	char passphrase[64];
	char ssid[33];
	uint8_t alg;
} lws_wifi_creds_t;

typedef struct lws_netdev_instance_wifi {
	lws_netdev_instance_t inst;
	lws_dll2_owner_t scan;
	lws_sorted_usec_list_t sul_scan;
	lws_wifi_creds_t *ap_cred;
	const char *ap_ip;
	const char *sta_ads;
	char current_attempt_ssid[33];
	uint8_t current_attempt_bssid[LWS_ETH_ALEN];
	uint8_t flags;
	uint8_t state;
} lws_netdev_instance_wifi_t;

typedef struct lws_wifi_sta {
	lws_dll2_t list;
	uint32_t last_seen;
	uint32_t last_tried;
	uint8_t bssid[LWS_ETH_ALEN];
	char *ssid;
	uint8_t ssid_len;
	lws_wifi_ch_t ch;
	lws_wifi_rssi_t rssi[8];
	int16_t rssi_avg;
	uint8_t authmode;
	uint8_t rssi_count;
	uint8_t rssi_next;
} lws_wifi_sta_t;

#define rssi_averaged(_x) (_x->rssi_count ? ((int)_x->rssi_avg / (int)_x->rssi_count) : -200)

LWS_VISIBLE LWS_EXTERN lws_netdevs_t *lws_netdevs_from_ctx(struct lws_context *ctx);
LWS_VISIBLE LWS_EXTERN int lws_netdev_credentials_settings_set(lws_netdevs_t *nds);
LWS_VISIBLE LWS_EXTERN int lws_netdev_credentials_settings_get(lws_netdevs_t *nds);
LWS_VISIBLE LWS_EXTERN struct lws_netdev_instance *lws_netdev_wifi_create_plat(struct lws_context *ctx, const lws_netdev_ops_t *ops, const char *name, void *platinfo);
LWS_VISIBLE LWS_EXTERN int lws_netdev_wifi_configure_plat(struct lws_netdev_instance *nd, lws_netdev_config_t *config);
LWS_VISIBLE LWS_EXTERN int lws_netdev_wifi_event_plat(struct lws_netdev_instance *nd, lws_usec_t timestamp, void *buf, size_t len);
LWS_VISIBLE LWS_EXTERN int lws_netdev_wifi_up_plat(struct lws_netdev_instance *nd);
LWS_VISIBLE LWS_EXTERN int lws_netdev_wifi_down_plat(struct lws_netdev_instance *nd);
LWS_VISIBLE LWS_EXTERN void lws_netdev_wifi_destroy_plat(struct lws_netdev_instance **pnd);
LWS_VISIBLE LWS_EXTERN void lws_netdev_wifi_scan_plat(lws_netdev_instance_t *nd);
LWS_VISIBLE LWS_EXTERN int lws_netdev_wifi_connect_plat(lws_netdev_instance_t *wnd, const char *ssid, const char *passphrase, uint8_t *bssid);
LWS_VISIBLE LWS_EXTERN lws_netdev_instance_t *lws_netdev_find(lws_netdevs_t *netdevs, const char *ifname);

#define lws_netdev_wifi_plat_ops \
	.create = lws_netdev_wifi_create_plat, \
	.configure = lws_netdev_wifi_configure_plat, \
	.event = lws_netdev_wifi_event_plat, \
	.up = lws_netdev_wifi_up_plat, \
	.down = lws_netdev_wifi_down_plat, \
	.connect = lws_netdev_wifi_connect_plat, \
	.scan = lws_netdev_wifi_scan_plat, \
	.destroy = lws_netdev_wifi_destroy_plat

LWS_VISIBLE LWS_EXTERN int lws_netdev_plat_init(void);
LWS_VISIBLE LWS_EXTERN int lws_netdev_plat_wifi_init(void);

#if defined(LWS_ESP_PLATFORM)
#include <lwip/sockets.h>
#endif

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

#ifndef __LWS_OPTEE_H
#define __LWS_OPTEE_H

struct in6_addr {
	union {
		uint8_t u6_addr8[16];
		uint16_t u6_addr16[8];
		uint32_t u6_addr32[4];
	};
};

#define _SS_MAXSIZE 128U
#define _SS_ALIGNSIZE (sizeof(int64_t))
#define _SS_PAD1SIZE (_SS_ALIGNSIZE - sizeof(sa_family_t))
#define _SS_PAD2SIZE (_SS_MAXSIZE - sizeof(sa_family_t) - _SS_PAD1SIZE - _SS_ALIGNSIZE)

struct sockaddr_storage {
	sa_family_t ss_family;
	char __ss_pad1[_SS_PAD1SIZE];
	int64_t __ss_align;
	char __ss_pad2[_SS_PAD2SIZE];
};

#define __SOCK_SIZE__ 16

struct sockaddr {
	sa_family_t sa_family;
	uint8_t sa_data[__SOCK_SIZE__ - sizeof(sa_family_t)];
};

struct sockaddr_in {
	sa_family_t sin_family;
	in_port_t sin_port;
	struct in_addr sin_addr;
	uint8_t sin_zero[__SOCK_SIZE__ - sizeof(sa_family_t) - sizeof(in_port_t) - sizeof(struct in_addr)];
};

struct sockaddr_in6 {
	sa_family_t sin6_family;
	in_port_t sin6_port;
	uint32_t sin6_flowinfo;
	struct in6_addr sin6_addr;
	uint32_t sin6_scope_id;
};

#endif /* __LWS_OPTEE_H */
// By GST @Date