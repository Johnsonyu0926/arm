#if defined(LWS_WITH_SPAWN)

#if defined(WIN32) || defined(_WIN32)
#else
#include <sys/wait.h>
#include <sys/times.h>
#endif
#endif

#if defined(__OpenBSD__)
#include <sys/siginfo.h>
#endif

struct lws_buflist;

LWS_VISIBLE LWS_EXTERN int LWS_WARN_UNUSED_RESULT lws_buflist_append_segment(struct lws_buflist **head, const uint8_t *buf, size_t len);
LWS_VISIBLE LWS_EXTERN size_t lws_buflist_next_segment_len(struct lws_buflist **head, uint8_t **buf);
LWS_VISIBLE LWS_EXTERN size_t lws_buflist_use_segment(struct lws_buflist **head, size_t len);
LWS_VISIBLE LWS_EXTERN size_t lws_buflist_total_len(struct lws_buflist **head);
LWS_VISIBLE LWS_EXTERN int lws_buflist_linear_copy(struct lws_buflist **head, size_t ofs, uint8_t *buf, size_t len);
LWS_VISIBLE LWS_EXTERN int lws_buflist_linear_use(struct lws_buflist **head, uint8_t *buf, size_t len);
LWS_VISIBLE LWS_EXTERN int lws_buflist_fragment_use(struct lws_buflist **head, uint8_t *buf, size_t len, char *frag_first, char *frag_fin);
LWS_VISIBLE LWS_EXTERN void lws_buflist_destroy_all_segments(struct lws_buflist **head);
LWS_VISIBLE LWS_EXTERN void lws_buflist_describe(struct lws_buflist **head, void *id, const char *reason);

#define lws_ptr_diff(head, tail) ((int)((char *)(head) - (char *)(tail)))
#define lws_ptr_diff_size_t(head, tail) ((size_t)(ssize_t)((char *)(head) - (char *)(tail)))

LWS_VISIBLE LWS_EXTERN int lws_snprintf(char *str, size_t size, const char *format, ...) LWS_FORMAT(3);
LWS_VISIBLE LWS_EXTERN char * lws_strncpy(char *dest, const char *src, size_t size);
#define lws_strnncpy(dest, src, size1, destsize) lws_strncpy(dest, src, (size_t)(size1 + 1) < (size_t)(destsize) ? (size_t)(size1 + 1) : (size_t)(destsize))

LWS_VISIBLE LWS_EXTERN const char * lws_nstrstr(const char *buf, size_t len, const char *name, size_t nl);
LWS_VISIBLE LWS_EXTERN const char * lws_json_simple_find(const char *buf, size_t len, const char *name, size_t *alen);
LWS_VISIBLE LWS_EXTERN int lws_json_simple_strcmp(const char *buf, size_t len, const char *name, const char *comp);
LWS_VISIBLE LWS_EXTERN int lws_hex_to_byte_array(const char *h, uint8_t *dest, int max);
LWS_VISIBLE LWS_EXTERN void lws_hex_from_byte_array(const uint8_t *src, size_t slen, char *dest, size_t len);
LWS_VISIBLE LWS_EXTERN int lws_hex_random(struct lws_context *context, char *dest, size_t len);
LWS_VISIBLE LWS_EXTERN int lws_timingsafe_bcmp(const void *a, const void *b, uint32_t len);
LWS_VISIBLE LWS_EXTERN size_t lws_get_random(struct lws_context *context, void *buf, size_t len);
LWS_VISIBLE LWS_EXTERN int LWS_WARN_UNUSED_RESULT lws_daemonize(const char *_lock_path);
LWS_VISIBLE LWS_EXTERN const char * LWS_WARN_UNUSED_RESULT lws_get_library_version(void);
LWS_VISIBLE LWS_EXTERN void * lws_wsi_user(struct lws *wsi);
LWS_VISIBLE LWS_EXTERN int lws_wsi_tsi(struct lws *wsi);
LWS_VISIBLE LWS_EXTERN void lws_set_wsi_user(struct lws *wsi, void *user);
LWS_VISIBLE LWS_EXTERN int LWS_WARN_UNUSED_RESULT lws_parse_uri(char *p, const char **prot, const char **ads, int *port, const char **path);
LWS_VISIBLE LWS_EXTERN const char * lws_cmdline_option(int argc, const char **argv, const char *val);
LWS_VISIBLE LWS_EXTERN void lws_cmdline_option_handle_builtin(int argc, const char **argv, struct lws_context_creation_info *info);
LWS_VISIBLE LWS_EXTERN unsigned long lws_now_secs(void);
LWS_VISIBLE LWS_EXTERN lws_usec_t lws_now_usecs(void);
LWS_VISIBLE LWS_EXTERN struct lws_context * LWS_WARN_UNUSED_RESULT lws_get_context(const struct lws *wsi);
LWS_VISIBLE LWS_EXTERN int LWS_WARN_UNUSED_RESULT lws_get_vhost_listen_port(struct lws_vhost *vhost);
LWS_VISIBLE LWS_EXTERN int LWS_WARN_UNUSED_RESULT lws_get_count_threads(struct lws_context *context);
LWS_VISIBLE LWS_EXTERN struct lws * LWS_WARN_UNUSED_RESULT lws_get_parent(const struct lws *wsi);
LWS_VISIBLE LWS_EXTERN struct lws * LWS_WARN_UNUSED_RESULT lws_get_child(const struct lws *wsi);
LWS_VISIBLE LWS_EXTERN void lws_get_effective_uid_gid(struct lws_context *context, uid_t *uid, gid_t *gid);
LWS_VISIBLE LWS_EXTERN const struct lws_udp * LWS_WARN_UNUSED_RESULT lws_get_udp(const struct lws *wsi);
LWS_VISIBLE LWS_EXTERN void * lws_get_opaque_parent_data(const struct lws *wsi);
LWS_VISIBLE LWS_EXTERN void lws_set_opaque_parent_data(struct lws *wsi, void *data);
LWS_VISIBLE LWS_EXTERN void * lws_get_opaque_user_data(const struct lws *wsi);
LWS_VISIBLE LWS_EXTERN void lws_set_opaque_user_data(struct lws *wsi, void *data);
LWS_VISIBLE LWS_EXTERN int lws_get_child_pending_on_writable(const struct lws *wsi);
LWS_VISIBLE LWS_EXTERN void lws_clear_child_pending_on_writable(struct lws *wsi);
LWS_VISIBLE LWS_EXTERN int lws_get_close_length(struct lws *wsi);
LWS_VISIBLE LWS_EXTERN unsigned char * lws_get_close_payload(struct lws *wsi);
LWS_VISIBLE LWS_EXTERN struct lws * lws_get_network_wsi(struct lws *wsi);
LWS_VISIBLE LWS_EXTERN void lws_set_allocator(void *(*realloc)(void *ptr, size_t size, const char *reason));

enum {
    LWS_RXFLOW_REASON_USER_BOOL     = (1 << 0),
    LWS_RXFLOW_REASON_HTTP_RXBUFFER     = (1 << 6),
    LWS_RXFLOW_REASON_H2_PPS_PENDING    = (1 << 7),
    LWS_RXFLOW_REASON_APPLIES       = (1 << 14),
    LWS_RXFLOW_REASON_APPLIES_ENABLE_BIT    = (1 << 13),
    LWS_RXFLOW_REASON_APPLIES_ENABLE    = LWS_RXFLOW_REASON_APPLIES | LWS_RXFLOW_REASON_APPLIES_ENABLE_BIT,
    LWS_RXFLOW_REASON_APPLIES_DISABLE   = LWS_RXFLOW_REASON_APPLIES,
    LWS_RXFLOW_REASON_FLAG_PROCESS_NOW  = (1 << 12),
};

LWS_VISIBLE LWS_EXTERN int lws_rx_flow_control(struct lws *wsi, int enable);
LWS_VISIBLE LWS_EXTERN void lws_rx_flow_allow_all_protocol(const struct lws_context *context, const struct lws_protocols *protocol);
LWS_VISIBLE LWS_EXTERN size_t lws_remaining_packet_payload(struct lws *wsi);

#if defined(LWS_WITH_DIR)

typedef enum {
    LDOT_UNKNOWN,
    LDOT_FILE,
    LDOT_DIR,
    LDOT_LINK,
    LDOT_FIFO,
    LDOTT_SOCKET,
    LDOT_CHAR,
    LDOT_BLOCK
} lws_dir_obj_type_t;

struct lws_dir_entry {
    const char *name;
    lws_dir_obj_type_t type;
};

typedef int lws_dir_callback_function(const char *dirpath, void *user, struct lws_dir_entry *lde);

LWS_VISIBLE LWS_EXTERN int lws_dir(const char *dirpath, void *user, lws_dir_callback_function cb);
LWS_VISIBLE LWS_EXTERN int lws_dir_rm_rf_cb(const char *dirpath, void *user, struct lws_dir_entry *lde);

typedef int (*lws_dir_glob_cb_t)(void *data, const char *path);

typedef struct lws_dir_glob {
    const char      *filter;
    lws_dir_glob_cb_t   cb;
    void            *user;
} lws_dir_glob_t;

LWS_VISIBLE LWS_EXTERN int lws_dir_glob_cb(const char *dirpath, void *user, struct lws_dir_entry *lde);

#endif

size_t lws_get_allocated_heap(void);

LWS_VISIBLE LWS_EXTERN int lws_get_tsi(struct lws *wsi);
LWS_VISIBLE LWS_EXTERN int lws_is_ssl(struct lws *wsi);
LWS_VISIBLE LWS_EXTERN int lws_is_cgi(struct lws *wsi);

LWS_VISIBLE LWS_EXTERN int lws_tls_jit_trust_blob_queury_skid(const void *_blob, size_t blen, const uint8_t *skid, size_t skid_len, const uint8_t **prpder, size_t *prder_len);
LWS_VISIBLE LWS_EXTERN int lws_open(const char *__file, int __oflag, ...);

struct lws_wifi_scan {
    struct lws_wifi_scan *next;
    char ssid[32];
    int32_t rssi;
    uint8_t bssid[6];
    uint8_t count;
    uint8_t channel;
    uint8_t authmode;
};

#if defined(LWS_WITH_TLS) && !defined(LWS_WITH_MBEDTLS)
LWS_VISIBLE LWS_EXTERN SSL* lws_get_ssl(struct lws *wsi);
#endif

LWS_VISIBLE LWS_EXTERN void lws_explicit_bzero(void *p, size_t len);

typedef struct lws_humanize_unit {
    const char *name;
    uint64_t factor;
} lws_humanize_unit_t;

LWS_VISIBLE extern const lws_humanize_unit_t humanize_schema_si[7];
LWS_VISIBLE extern const lws_humanize_unit_t humanize_schema_si_bytes[7];
LWS_VISIBLE extern const lws_humanize_unit_t humanize_schema_us[8];

LWS_VISIBLE LWS_EXTERN int lws_humanize(char *buf, size_t len, uint64_t value, const lws_humanize_unit_t *schema);
LWS_VISIBLE LWS_EXTERN void lws_ser_wu16be(uint8_t *b, uint16_t u);
LWS_VISIBLE LWS_EXTERN void lws_ser_wu32be(uint8_t *b, uint32_t u32);
LWS_VISIBLE LWS_EXTERN void lws_ser_wu64be(uint8_t *b, uint64_t u64);
LWS_VISIBLE LWS_EXTERN uint16_t lws_ser_ru16be(const uint8_t *b);
LWS_VISIBLE LWS_EXTERN uint32_t lws_ser_ru32be(const uint8_t *b);
LWS_VISIBLE LWS_EXTERN uint64_t lws_ser_ru64be(const uint8_t *b);
LWS_VISIBLE LWS_EXTERN int lws_vbi_encode(uint64_t value, void *buf);
LWS_VISIBLE LWS_EXTERN int lws_vbi_decode(const void *buf, uint64_t *value, size_t len);

#if defined(LWS_WITH_SPAWN)

struct lws_spawn_piped;

#if defined(WIN32)
struct _lws_siginfo_t {
    int retcode;
};
typedef struct _lws_siginfo_t siginfo_t;
#endif

typedef void (*lsp_cb_t)(void *opaque, lws_usec_t *accounting, siginfo_t *si, int we_killed_him);

struct lws_spawn_piped_info {
    struct lws_dll2_owner       *owner;
    struct lws_vhost        *vh;
    struct lws          *opt_parent;
    const char * const      *exec_array;
    const char          **env_array;
    const char          *protocol_name;
    const char          *chroot_path;
    const char          *wd;
    struct lws_spawn_piped      **plsp;
    void                *opaque;
    lsp_cb_t            reap_cb;
    lws_usec_t          timeout_us;
    int             max_log_lines;
    int             tsi;
    const struct lws_role_ops   *ops;
    uint8_t             disable_ctrlc;
};

LWS_VISIBLE LWS_EXTERN struct lws_spawn_piped * lws_spawn_piped(const struct lws_spawn_piped_info *lspi);
LWS_VISIBLE LWS_EXTERN int lws_spawn_piped_kill_child_process(struct lws_spawn_piped *lsp);
LWS_VISIBLE LWS_EXTERN void lws_spawn_stdwsi_closed(struct lws_spawn_piped *lsp, struct lws *wsi);
LWS_VISIBLE LWS_EXTERN int lws_spawn_get_stdfd(struct lws *wsi);

#endif

struct lws_fsmount {
    const char  *layers_path;
    const char  *overlay_path;
    char        mp[256];
    char        ovname[64];
    char        distro[64];
#if defined(__linux__)
    const char  *layers[4];
#endif
};

LWS_VISIBLE LWS_EXTERN int lws_fsmount_mount(struct lws_fsmount *fsm);
LWS_VISIBLE LWS_EXTERN int lws_fsmount_unmount(struct lws_fsmount *fsm);
// By GST @Date