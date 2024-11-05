#if !defined(__LWS_BUTTON_H__)
#define __LWS_BUTTON_H__

typedef uint16_t lws_button_idx_t;

#define LWS_BUTTON_MON_TIMER_MS 5

typedef void (*lws_button_cb_t)(void *opaque, lws_button_idx_t idx, int state);

enum {
    LWSBTNRGMFLAG_CLASSIFY_DOUBLECLICK = (1 << 0)
};

typedef struct lws_button_regime {
    uint16_t            ms_min_down;
    uint16_t            ms_min_down_longpress;
    uint16_t            ms_up_settle;
    uint16_t            ms_doubleclick_grace;
    uint16_t            ms_repeat_down;
    uint8_t             flags;
} lws_button_regime_t;

typedef struct lws_button_map {
    _lws_plat_gpio_t        gpio;
    const char          *smd_interaction_name;
    const lws_button_regime_t   *regime;
} lws_button_map_t;

typedef struct lws_button_controller {
    const char          *smd_bc_name;
    const lws_gpio_ops_t        *gpio_ops;
    const lws_button_map_t      *button_map;
    lws_button_idx_t        active_state_bitmap;
    uint8_t             count_buttons;
} lws_button_controller_t;

struct lws_button_state;

LWS_VISIBLE LWS_EXTERN struct lws_button_state *
lws_button_controller_create(struct lws_context *ctx,
                 const lws_button_controller_t *controller);

LWS_VISIBLE LWS_EXTERN void
lws_button_controller_destroy(struct lws_button_state *bcs);

LWS_VISIBLE LWS_EXTERN lws_button_idx_t
lws_button_get_bit(struct lws_button_state *bcs, const char *name);

LWS_VISIBLE LWS_EXTERN void
lws_button_enable(struct lws_button_state *bcs,
          lws_button_idx_t _reset, lws_button_idx_t _set);

#endif

struct lws_cache_ttl_lru;

LWS_VISIBLE LWS_EXTERN int
lws_cache_write_through(struct lws_cache_ttl_lru *cache,
            const char *specific_key, const uint8_t *source,
            size_t size, lws_usec_t expiry, void **ppay);

typedef struct lws_cache_match {
    lws_dll2_t          list;
    lws_usec_t          expiry;
    size_t              payload_size;
    size_t              tag_size;
} lws_cache_match_t;

LWS_VISIBLE LWS_EXTERN int
lws_cache_lookup(struct lws_cache_ttl_lru *cache, const char *wildcard_key,
         const void **pdata, size_t *psize);

LWS_VISIBLE LWS_EXTERN int
lws_cache_item_get(struct lws_cache_ttl_lru *cache, const char *specific_key,
           const void **pdata, size_t *psize);

LWS_VISIBLE LWS_EXTERN int
lws_cache_item_remove(struct lws_cache_ttl_lru *cache, const char *wildcard_key);

LWS_VISIBLE LWS_EXTERN uint64_t
lws_cache_footprint(struct lws_cache_ttl_lru *cache);

LWS_VISIBLE LWS_EXTERN void
lws_cache_debug_dump(struct lws_cache_ttl_lru *cache);

typedef struct lws_cache_results {
    const uint8_t       *ptr;
    size_t          size;
    size_t          payload_len;
    size_t          tag_len;
    const uint8_t       *tag;
} lws_cache_results_t;

LWS_VISIBLE LWS_EXTERN int
lws_cache_results_walk(lws_cache_results_t *walk_ctx);

typedef void (*lws_cache_item_destroy_cb)(void *item, size_t size);
struct lws_cache_creation_info {
    struct lws_context      *cx;
    const char          *name;
    lws_cache_item_destroy_cb   cb;
    struct lws_cache_ttl_lru    *parent;
    const struct lws_cache_ops  *ops;

    union {
        struct {
            const char  *filepath;
        } nscookiejar;
    } u;

    size_t              max_footprint;
    size_t              max_items;
    size_t              max_payload;
    int             tsi;
};

struct lws_cache_ops {
    struct lws_cache_ttl_lru *
    (*create)(const struct lws_cache_creation_info *info);

    void
    (*destroy)(struct lws_cache_ttl_lru **_cache);

    int
    (*expunge)(struct lws_cache_ttl_lru *cache);

    int
    (*write)(struct lws_cache_ttl_lru *cache, const char *specific_key,
         const uint8_t *source, size_t size, lws_usec_t expiry,
         void **ppvoid);
    int
    (*tag_match)(struct lws_cache_ttl_lru *cache, const char *wc,
             const char *tag, char lookup_rules);
    int
    (*lookup)(struct lws_cache_ttl_lru *cache, const char *wildcard_key,
              lws_dll2_owner_t *results_owner);
    int
    (*invalidate)(struct lws_cache_ttl_lru *cache, const char *wildcard_key);

    int
    (*get)(struct lws_cache_ttl_lru *cache, const char *specific_key,
           const void **pdata, size_t *psize);

    void
    (*debug_dump)(struct lws_cache_ttl_lru *cache);
};

LWS_VISIBLE LWS_EXTERN struct lws_cache_ttl_lru *
lws_cache_create(const struct lws_cache_creation_info *info);

LWS_VISIBLE LWS_EXTERN void
lws_cache_destroy(struct lws_cache_ttl_lru **cache);

LWS_VISIBLE LWS_EXTERN int
lws_cache_expunge(struct lws_cache_ttl_lru *cache);

LWS_VISIBLE extern const struct lws_cache_ops lws_cache_ops_heap,
                          lws_cache_ops_nscookiejar;

// By GST @Date