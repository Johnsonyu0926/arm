// Filename: lws_system.h
// Score: 97

#ifndef __LWS_SYSTEM_H__
#define __LWS_SYSTEM_H__

#if defined(LWS_WITH_STRUCT_SQLITE3)
#include <sqlite3.h>
#endif

typedef enum {
    LSMT_SIGNED,
    LSMT_UNSIGNED,
    LSMT_BOOLEAN,
    LSMT_STRING_CHAR_ARRAY,
    LSMT_STRING_PTR,
    LSMT_LIST,
    LSMT_CHILD_PTR,
    LSMT_SCHEMA,
    LSMT_BLOB_PTR,
} lws_struct_map_type_eum;

typedef struct lejp_collation {
    struct lws_dll2 chunks;
    int len;
    char buf[LEJP_STRING_CHUNK + 1];
} lejp_collation_t;

typedef struct lws_struct_map {
    const char *colname;
    const struct lws_struct_map *child_map;
    lejp_callback lejp_cb;
    size_t ofs; /* child dll2; points to dll2_owner */
    size_t aux;
    size_t ofs_clist;
    size_t child_map_size;
    lws_struct_map_type_eum type;
} lws_struct_map_t;

typedef int (*lws_struct_args_cb)(void *obj, void *cb_arg);

typedef struct lws_struct_args {
    const lws_struct_map_t *map_st[LEJP_MAX_PARSING_STACK_DEPTH];
    lws_struct_args_cb cb;
    struct lwsac *ac;
    void *cb_arg;
    void *dest;
    size_t dest_len;
    size_t toplevel_dll2_ofs;
    size_t map_entries_st[LEJP_MAX_PARSING_STACK_DEPTH];
    size_t ac_block_size;
    int subtype;
    int top_schema_index;
    struct lwsac *ac_chunks;
    struct lws_dll2_owner chunks_owner;
    size_t chunks_length;
} lws_struct_args_t;

#define LSM_SIGNED(type, name, qname) \
    { \
      qname, \
      NULL, \
      NULL, \
      offsetof(type, name), \
      sizeof ((type *)0)->name, \
      0, \
      0, \
      LSMT_SIGNED \
    }

#define LSM_UNSIGNED(type, name, qname) \
    { \
      qname, \
      NULL, \
      NULL, \
      offsetof(type, name), \
      sizeof ((type *)0)->name, \
      0, \
      0, \
      LSMT_UNSIGNED \
    }

#define LSM_BOOLEAN(type, name, qname) \
    { \
      qname, \
      NULL, \
      NULL, \
      offsetof(type, name), \
      sizeof ((type *)0)->name, \
      0, \
      0, \
      LSMT_BOOLEAN \
    }

#define LSM_CARRAY(type, name, qname) \
    { \
      qname, \
      NULL, \
      NULL, \
      offsetof(type, name), \
      sizeof (((type *)0)->name), \
      0, \
      0, \
      LSMT_STRING_CHAR_ARRAY \
    }

#define LSM_STRING_PTR(type, name, qname) \
    { \
      qname, \
      NULL, \
      NULL, \
      offsetof(type, name), \
      sizeof (((type *)0)->name), \
      0, \
      0, \
      LSMT_STRING_PTR \
    }

#define LSM_LIST(ptype, pname, ctype, cname, lejp_cb, cmap, qname) \
    { \
      qname, \
      cmap, \
      lejp_cb, \
      offsetof(ptype, pname), \
      sizeof (ctype), \
      offsetof(ctype, cname), \
      LWS_ARRAY_SIZE(cmap), \
      LSMT_LIST \
    }

#define LSM_CHILD_PTR(ptype, pname, ctype, lejp_cb, cmap, qname) \
    { \
      qname, \
      cmap, \
      lejp_cb, \
      offsetof(ptype, pname), \
      sizeof (ctype), \
      0, \
      LWS_ARRAY_SIZE(cmap), \
      LSMT_CHILD_PTR \
    }

#define LSM_SCHEMA(ctype, lejp_cb, map, schema_name) \
    { \
      schema_name, \
      map, \
      lejp_cb, \
      0, \
      sizeof (ctype), \
      0, \
      LWS_ARRAY_SIZE(map), \
      LSMT_SCHEMA \
    }

#define LSM_SCHEMA_DLL2(ctype, cdll2mem, lejp_cb, map, schema_name) \
    { \
      schema_name, \
      map, \
      lejp_cb, \
      offsetof(ctype, cdll2mem), \
      sizeof (ctype), \
      0, \
      LWS_ARRAY_SIZE(map), \
      LSMT_SCHEMA \
    }

#define LSM_BLOB_PTR(type, blobptr_name, qname) \
    { \
      qname, \
      NULL, \
      NULL, \
      offsetof(type, blobptr_name), \
      sizeof (((type *)0)->blobptr_name), \
      0, \
      0, \
      LSMT_BLOB_PTR \
    }

typedef struct lws_struct_serialize_st {
    const struct lws_dll2 *dllpos;
    const lws_struct_map_t *map;
    const char *obj;
    size_t map_entries;
    size_t map_entry;
    size_t size;
    char subsequent;
    char idt;
} lws_struct_serialize_st_t;

enum {
    LSSERJ_FLAG_PRETTY = (1 << 0),
    LSSERJ_FLAG_OMIT_SCHEMA = (1 << 1)
};

typedef struct lws_struct_serialize {
    lws_struct_serialize_st_t st[LEJP_MAX_PARSING_STACK_DEPTH];
    size_t offset;
    size_t remaining;
    int sp;
    int flags;
} lws_struct_serialize_t;

typedef enum {
    LSJS_RESULT_CONTINUE,
    LSJS_RESULT_FINISH,
    LSJS_RESULT_ERROR
} lws_struct_json_serialize_result_t;

LWS_VISIBLE LWS_EXTERN int lws_struct_json_init_parse(struct lejp_ctx *ctx, lejp_callback cb, void *user);

LWS_VISIBLE LWS_EXTERN signed char lws_struct_schema_only_lejp_cb(struct lejp_ctx *ctx, char reason);

LWS_VISIBLE LWS_EXTERN signed char lws_struct_default_lejp_cb(struct lejp_ctx *ctx, char reason);

LWS_VISIBLE LWS_EXTERN lws_struct_serialize_t *lws_struct_json_serialize_create(const lws_struct_map_t *map,
                                                                                size_t map_entries, int flags,
                                                                                const void *ptoplevel);

LWS_VISIBLE LWS_EXTERN void lws_struct_json_serialize_destroy(lws_struct_serialize_t **pjs);

LWS_VISIBLE LWS_EXTERN lws_struct_json_serialize_result_t lws_struct_json_serialize(lws_struct_serialize_t *js, uint8_t *buf,
                                                                                    size_t len, size_t *written);

typedef struct sqlite3 sqlite3;

LWS_VISIBLE LWS_EXTERN int lws_struct_sq3_serialize(sqlite3 *pdb, const lws_struct_map_t *schema,
                                                    lws_dll2_owner_t *owner, uint32_t manual_idx);

LWS_VISIBLE LWS_EXTERN int lws_struct_sq3_deserialize(sqlite3 *pdb, const char *filter, const char *order,
                                                      const lws_struct_map_t *schema, lws_dll2_owner_t *o,
                                                      struct lwsac **ac, int start, int limit);

LWS_VISIBLE LWS_EXTERN int lws_struct_sq3_create_table(sqlite3 *pdb, const lws_struct_map_t *schema);

LWS_VISIBLE LWS_EXTERN int lws_struct_sq3_open(struct lws_context *context, const char *sqlite3_path,
                                               char create_if_missing, sqlite3 **pdb);

LWS_VISIBLE LWS_EXTERN int lws_struct_sq3_close(sqlite3 **pdb);

typedef enum {
    LWS_SYSBLOB_TYPE_AUTH,
    LWS_SYSBLOB_TYPE_CLIENT_CERT_DER = LWS_SYSBLOB_TYPE_AUTH + 2,
    LWS_SYSBLOB_TYPE_CLIENT_KEY_DER,
    LWS_SYSBLOB_TYPE_DEVICE_SERIAL,
    LWS_SYSBLOB_TYPE_DEVICE_FW_VERSION,
    LWS_SYSBLOB_TYPE_DEVICE_TYPE,
    LWS_SYSBLOB_TYPE_NTP_SERVER,
    LWS_SYSBLOB_TYPE_MQTT_CLIENT_ID,
    LWS_SYSBLOB_TYPE_MQTT_USERNAME,
    LWS_SYSBLOB_TYPE_MQTT_PASSWORD,
#if defined(LWS_WITH_SECURE_STREAMS_AUTH_SIGV4)
    LWS_SYSBLOB_TYPE_EXT_AUTH1,
    LWS_SYSBLOB_TYPE_EXT_AUTH2 = LWS_SYSBLOB_TYPE_EXT_AUTH1 + 2,
    LWS_SYSBLOB_TYPE_EXT_AUTH3 = LWS_SYSBLOB_TYPE_EXT_AUTH2 + 2,
    LWS_SYSBLOB_TYPE_EXT_AUTH4 = LWS_SYSBLOB_TYPE_EXT_AUTH3 + 2,
    LWS_SYSBLOB_TYPE_EXT_AUTH4_1,
#endif
    LWS_SYSBLOB_TYPE_COUNT
} lws_system_blob_item_t;

typedef struct lws_system_blob lws_system_blob_t;

LWS_VISIBLE LWS_EXTERN void lws_system_blob_direct_set(lws_system_blob_t *b, const uint8_t *ptr, size_t len);

LWS_VISIBLE LWS_EXTERN void lws_system_blob_heap_empty(lws_system_blob_t *b);

LWS_VISIBLE LWS_EXTERN int lws_system_blob_heap_append(lws_system_blob_t *b, const uint8_t *ptr, size_t len);

LWS_VISIBLE LWS_EXTERN size_t lws_system_blob_get_size(lws_system_blob_t *b);

LWS_VISIBLE LWS_EXTERN int lws_system_blob_get_single_ptr(lws_system_blob_t *b, const uint8_t **ptr);

LWS_VISIBLE LWS_EXTERN int lws_system_blob_get(lws_system_blob_t *b, uint8_t *ptr, size_t *len, size_t ofs);

LWS_VISIBLE LWS_EXTERN void lws_system_blob_destroy(lws_system_blob_t *b);

LWS_VISIBLE LWS_EXTERN lws_system_blob_t *lws_system_get_blob(struct lws_context *context, lws_system_blob_item_t type, int idx);

typedef enum {
    LWS_SYSTATE_UNKNOWN,
    LWS_SYSTATE_CONTEXT_CREATED,
    LWS_SYSTATE_INITIALIZED,
    LWS_SYSTATE_IFACE_COLDPLUG,
    LWS_SYSTATE_DHCP,
    LWS_SYSTATE_CPD_PRE_TIME,
    LWS_SYSTATE_TIME_VALID,
    LWS_SYSTATE_CPD_POST_TIME,
    LWS_SYSTATE_POLICY_VALID,
    LWS_SYSTATE_REGISTERED,
    LWS_SYSTATE_AUTH1,
    LWS_SYSTATE_AUTH2,
    LWS_SYSTATE_OPERATIONAL,
    LWS_SYSTATE_POLICY_INVALID,
    LWS_SYSTATE_CONTEXT_DESTROYING,
} lws_system_states_t;

typedef enum {
    LWS_CPD_UNKNOWN = 0,
    LWS_CPD_INTERNET_OK,
    LWS_CPD_CAPTIVE_PORTAL,
    LWS_CPD_NO_INTERNET,
} lws_cpd_result_t;

typedef void (*lws_attach_cb_t)(struct lws_context *context, int tsi, void *opaque);
struct lws_attach_item;

LWS_VISIBLE LWS_EXTERN int lws_tls_jit_trust_got_cert_cb(struct lws_context *cx, void *got_opaque, const uint8_t *skid, size_t skid_len, const uint8_t *der, size_t der_len);

typedef struct lws_system_ops {
    int (*reboot)(void);
    int (*set_clock)(lws_usec_t us);
    int (*attach)(struct lws_context *context, int tsi, lws_attach_cb_t cb, lws_system_states_t state, void *opaque, struct lws_attach_item **get);
    int (*captive_portal_detect_request)(struct lws_context *context);
    int (*metric_report)(lws_metric_pub_t *mdata);
    int (*jit_trust_query)(struct lws_context *cx, const uint8_t *skid, size_t skid_len, void *got_opaque);
    uint32_t wake_latency_us;
} lws_system_ops_t;

#if defined(LWS_WITH_SYS_STATE)
LWS_VISIBLE LWS_EXTERN lws_state_manager_t *lws_system_get_state_manager(struct lws_context *context);
#endif

#define LWSSYSGAUTH_HEX (1 << 0)

LWS_VISIBLE LWS_EXTERN const lws_system_ops_t *lws_system_get_ops(struct lws_context *context);

#if defined(LWS_WITH_SYS_STATE)
LWS_VISIBLE LWS_EXTERN struct lws_context *lws_system_context_from_system_mgr(lws_state_manager_t *mgr);
#endif

LWS_VISIBLE LWS_EXTERN int __lws_system_attach(struct lws_context *context, int tsi, lws_attach_cb_t cb, lws_system_states_t state, void *opaque, struct lws_attach_item **get);

enum {
    LWSDH_IPV4_SUBNET_MASK = 0,
    LWSDH_IPV4_BROADCAST,
    LWSDH_LEASE_SECS,
    LWSDH_REBINDING_SECS,
    LWSDH_RENEWAL_SECS,
    _LWSDH_NUMS_COUNT,
    LWSDH_SA46_IP = 0,
    LWSDH_SA46_DNS_SRV_1,
    LWSDH_SA46_DNS_SRV_2,
    LWSDH_SA46_DNS_SRV_3,
    LWSDH_SA46_DNS_SRV_4,
    LWSDH_SA46_IPV4_ROUTER,
    LWSDH_SA46_NTP_SERVER,
    LWSDH_SA46_DHCP_SERVER,
    _LWSDH_SA46_COUNT,
};

typedef struct lws_dhcpc_ifstate {
    char ifname[16];
    char domain[64];
    uint8_t mac[6];
    uint32_t nums[_LWSDH_NUMS_COUNT];
    lws_sockaddr46 sa46[_LWSDH_SA46_COUNT];
} lws_dhcpc_ifstate_t;

typedef int (*dhcpc_cb_t)(void *opaque, lws_dhcpc_ifstate_t *is);

LWS_VISIBLE LWS_EXTERN int lws_dhcpc_request(struct lws_context *c, const char *i, int af, dhcpc_cb_t cb, void *opaque);

LWS_VISIBLE LWS_EXTERN int lws_dhcpc_remove(struct lws_context *context, const char *iface);

LWS_VISIBLE LWS_EXTERN int lws_dhcpc_status(struct lws_context *context, lws_sockaddr46 *sa46);

LWS_VISIBLE LWS_EXTERN int lws_system_cpd_start(struct lws_context *context);

LWS_VISIBLE LWS_EXTERN void lws_system_cpd_start_defer(struct lws_context *cx, lws_usec_t defer_us);

LWS_VISIBLE LWS_EXTERN void lws_system_cpd_set(struct lws_context *context, lws_cpd_result_t result);

LWS_VISIBLE LWS_EXTERN lws_cpd_result_t lws_system_cpd_state_get(struct lws_context *context);

#endif // __LWS_SYSTEM_H__

// By GST @Date