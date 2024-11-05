LWS_VISIBLE LWS_EXTERN struct lws *
lws_adopt_socket(struct lws_context *context, lws_sockfd_type accept_fd);

LWS_VISIBLE LWS_EXTERN struct lws *
lws_adopt_socket_vhost(struct lws_vhost *vh, lws_sockfd_type accept_fd);

typedef enum {
    LWS_ADOPT_RAW_FILE_DESC     =  0,   /* convenience constant */
    LWS_ADOPT_HTTP          =  1,   /* flag: absent implies RAW */
    LWS_ADOPT_SOCKET        =  2,   /* flag: absent implies file */
    LWS_ADOPT_ALLOW_SSL     =  4,   /* flag: use tls */
    LWS_ADOPT_FLAG_UDP      = 16,   /* flag: socket is UDP */
    LWS_ADOPT_FLAG_RAW_PROXY    = 32,   /* flag: raw proxy */

    LWS_ADOPT_RAW_SOCKET_UDP = LWS_ADOPT_SOCKET | LWS_ADOPT_FLAG_UDP,
} lws_adoption_type;

typedef union {
    lws_sockfd_type sockfd;
    lws_filefd_type filefd;
} lws_sock_file_fd_type;

#if defined(LWS_ESP_PLATFORM)
#include <lwip/sockets.h>
#endif

typedef union {
#if defined(LWS_WITH_IPV6)
    struct sockaddr_in6 sa6;
#else
#if defined(LWS_ESP_PLATFORM)
    uint8_t _pad_sa6[28];
#endif
#endif
    struct sockaddr_in sa4;
} lws_sockaddr46;

#define sa46_sockaddr(_sa46) ((struct sockaddr *)(_sa46))

#if defined(LWS_WITH_IPV6)
#define sa46_socklen(_sa46) (socklen_t)((_sa46)->sa4.sin_family == AF_INET ? \
                sizeof(struct sockaddr_in) : \
                sizeof(struct sockaddr_in6))
#define sa46_sockport(_sa46, _sp)  { if ((_sa46)->sa4.sin_family == AF_INET) \
                    (_sa46)->sa4.sin_port = (_sp); else \
                    (_sa46)->sa6.sin6_port = (_sp); }
#define sa46_address(_sa46) ((uint8_t *)((_sa46)->sa4.sin_family == AF_INET ? \
             &_sa46->sa4.sin_addr : &_sa46->sa6.sin6_addr ))
#else
#define sa46_socklen(_sa46) (socklen_t)sizeof(struct sockaddr_in)
#define sa46_sockport(_sa46, _sp)  (_sa46)->sa4.sin_port = (_sp)
#define sa46_address(_sa46) (uint8_t *)&_sa46->sa4.sin_addr
#endif

#define sa46_address_len(_sa46) ((_sa46)->sa4.sin_family == AF_INET ? 4 : 16)

#if defined(LWS_WITH_UDP)
struct lws_udp {
    lws_sockaddr46      sa46;
    lws_sockaddr46      sa46_pending;
    uint8_t         connected:1;
};
#endif

LWS_VISIBLE LWS_EXTERN struct lws *
lws_adopt_descriptor_vhost(struct lws_vhost *vh, lws_adoption_type type,
               lws_sock_file_fd_type fd, const char *vh_prot_name,
               struct lws *parent);

typedef struct lws_adopt_desc {
    struct lws_vhost *vh;       /**< vhost the wsi should belong to */
    lws_adoption_type type;     /**< OR-ed combinations of lws_adoption_type flags */
    lws_sock_file_fd_type fd;   /**< union with either .sockfd or .filefd set */
    const char *vh_prot_name;   /**< NULL or vh protocol name to bind raw connection to */
    struct lws *parent;     /**< NULL or struct lws to attach new_wsi to as a child */
    void *opaque;           /**< opaque pointer to set on created wsi */
    const char *fi_wsi_name;    /**< NULL, or Fault Injection inheritence filter for wsi=string/ context faults */
} lws_adopt_desc_t;

LWS_VISIBLE LWS_EXTERN struct lws *
lws_adopt_descriptor_vhost_via_info(const lws_adopt_desc_t *info);

LWS_VISIBLE LWS_EXTERN struct lws *
lws_adopt_socket_readbuf(struct lws_context *context, lws_sockfd_type accept_fd,
                         const char *readbuf, size_t len);

LWS_VISIBLE LWS_EXTERN struct lws *
lws_adopt_socket_vhost_readbuf(struct lws_vhost *vhost,
                   lws_sockfd_type accept_fd, const char *readbuf,
                   size_t len);

#define LWS_CAUDP_BIND (1 << 0)
#define LWS_CAUDP_BROADCAST (1 << 1)
#define LWS_CAUDP_PF_PACKET (1 << 2)

#if defined(LWS_WITH_UDP)
LWS_VISIBLE LWS_EXTERN struct lws *
lws_create_adopt_udp(struct lws_vhost *vhost, const char *ads, int port,
             int flags, const char *protocol_name, const char *ifname,
             struct lws *parent_wsi, void *opaque,
             const lws_retry_bo_t *retry_policy, const char *fi_wsi_name);
#endif

// By GST @Date