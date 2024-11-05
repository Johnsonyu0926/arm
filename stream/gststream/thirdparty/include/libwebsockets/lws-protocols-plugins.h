#ifndef __LWS_OPTEE_H
#define __LWS_OPTEE_H

struct lws_protocols {
    const char *name;
    lws_callback_function *callback;
    size_t per_session_data_size;
    size_t rx_buffer_size;
    unsigned int id;
    void *user;
    size_t tx_packet_size;
};

#define LWS_PROTOCOL_LIST_TERM { NULL, NULL, 0, 0, 0, NULL, 0 }

LWS_VISIBLE LWS_EXTERN const struct lws_protocols *lws_vhost_name_to_protocol(struct lws_vhost *vh, const char *name);

LWS_VISIBLE LWS_EXTERN const struct lws_protocols *lws_get_protocol(struct lws *wsi);

LWS_VISIBLE LWS_EXTERN const struct lws_protocols *lws_protocol_get(struct lws *wsi) LWS_WARN_DEPRECATED;

LWS_VISIBLE LWS_EXTERN void *lws_protocol_vh_priv_zalloc(struct lws_vhost *vhost, const struct lws_protocols *prot, int size);

LWS_VISIBLE LWS_EXTERN void *lws_protocol_vh_priv_get(struct lws_vhost *vhost, const struct lws_protocols *prot);

LWS_VISIBLE LWS_EXTERN void *lws_vhd_find_by_pvo(struct lws_context *cx, const char *protname, const char *pvo_name, const char *pvo_value);

LWS_VISIBLE LWS_EXTERN void *lws_adjust_protocol_psds(struct lws *wsi, size_t new_size);

LWS_VISIBLE LWS_EXTERN int lws_finalize_startup(struct lws_context *context);

LWS_VISIBLE LWS_EXTERN const struct lws_protocol_vhost_options *lws_pvo_search(const struct lws_protocol_vhost_options *pvo, const char *name);

LWS_VISIBLE LWS_EXTERN int lws_pvo_get_str(void *in, const char *name, const char **result);

LWS_VISIBLE LWS_EXTERN int lws_protocol_init(struct lws_context *context);

#define LWS_PLUGIN_API_MAGIC 191

typedef struct lws_plugin_header {
    const char *name;
    const char *_class;
    const char *lws_build_hash;
    unsigned int api_magic;
} lws_plugin_header_t;

typedef struct lws_plugin_protocol {
    lws_plugin_header_t hdr;
    const struct lws_protocols *protocols;
    const struct lws_extension *extensions;
    int count_protocols;
    int count_extensions;
} lws_plugin_protocol_t;

struct lws_plugin {
    struct lws_plugin *list;
    const lws_plugin_header_t *hdr;
    union {
#if defined(LWS_WITH_LIBUV) && defined(UV_ERRNO_MAP)
#if (UV_VERSION_MAJOR > 0)
        uv_lib_t lib;
#endif
#endif
        void *l;
    } u;
};

typedef struct lws_plugin_evlib {
    lws_plugin_header_t hdr;
    const struct lws_event_loop_ops *ops;
} lws_plugin_evlib_t;

typedef int (*each_plugin_cb_t)(struct lws_plugin *p, void *user);

LWS_VISIBLE LWS_EXTERN int lws_plugins_init(struct lws_plugin **pplugin, const char * const *d, const char *_class, const char *filter, each_plugin_cb_t each, void *each_user);

LWS_VISIBLE LWS_EXTERN int lws_plugins_destroy(struct lws_plugin **pplugin, each_plugin_cb_t each, void *each_user);

#if defined(LWS_WITH_PLUGINS_BUILTIN)

extern const struct lws_protocols post_demo_protocols[1];
extern const struct lws_protocols lws_raw_proxy_protocols[1];
extern const struct lws_protocols lws_status_protocols[1];
extern const struct lws_protocols lws_mirror_protocols[1];
extern const struct lws_protocols lws_ssh_base_protocols[2];
extern const struct lws_protocols post_demo_protocols[1];
extern const struct lws_protocols dumb_increment_protocols[1];
extern const struct lws_protocols deaddrop_protocols[1];
extern const struct lws_protocols lws_raw_test_protocols[1];
extern const struct lws_protocols lws_sshd_demo_protocols[1];
extern const struct lws_protocols lws_acme_client_protocols[1];
extern const struct lws_protocols client_loopback_test_protocols[1];
extern const struct lws_protocols fulltext_demo_protocols[1];
extern const struct lws_protocols lws_openmetrics_export_protocols[
#if defined(LWS_WITH_SERVER) && defined(LWS_WITH_CLIENT) && defined(LWS_ROLE_WS)
    4
#else
#if defined(LWS_WITH_SERVER)
    3
#else
    1
#endif
#endif
];

#define LWSOMPROIDX_DIRECT_HTTP_SERVER 0
#define LWSOMPROIDX_PROX_HTTP_SERVER 1
#define LWSOMPROIDX_PROX_WS_SERVER 2
#define LWSOMPROIDX_PROX_WS_CLIENT 3
#endif

#endif // __LWS_OPTEE_H
// By GST @Date