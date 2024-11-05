typedef struct lws_map lws_map_t;
typedef struct lws_map_item lws_map_item_t;

typedef void * lws_map_key_t;
typedef void * lws_map_value_t;
typedef uint32_t lws_map_hash_t;

typedef lws_map_hash_t (*lws_map_hash_from_key_t)(const lws_map_key_t key, size_t kl);
typedef int (*lws_map_compare_key_t)(const lws_map_key_t key1, size_t kl1, const lws_map_value_t key2, size_t kl2);
typedef void * (*lws_map_alloc_t)(struct lws_map *mo, size_t x);
typedef void (*lws_map_free_t)(void *);

typedef struct lws_map_info {
    lws_map_hash_from_key_t     _hash;
    lws_map_compare_key_t       _compare;
    lws_map_alloc_t         _alloc;
    lws_map_free_t          _free;
    void                *opaque;
    void                *aux;
    size_t              modulo;
} lws_map_info_t;

LWS_VISIBLE LWS_EXTERN const void * lws_map_item_key(lws_map_item_t *_item);
LWS_VISIBLE LWS_EXTERN const void * lws_map_item_value(lws_map_item_t *_item);
LWS_VISIBLE LWS_EXTERN size_t lws_map_item_key_len(lws_map_item_t *_item);
LWS_VISIBLE LWS_EXTERN size_t lws_map_item_value_len(lws_map_item_t *_item);

#define lws_map_item_create_ks(_map, _str, _v, _vl) \
        lws_map_item_create(_map, (const lws_map_key_t)_str, \
                    strlen(_str), (const lws_map_value_t)_v, \
                            _vl)
#define lws_map_item_lookup_ks(_map, _str) \
        lws_map_item_lookup(_map, (const lws_map_key_t)_str, strlen(_str))

LWS_VISIBLE LWS_EXTERN lws_map_t * lws_map_create(const lws_map_info_t *info);

LWS_VISIBLE LWS_EXTERN void * lws_map_alloc_lwsac(struct lws_map *map, size_t x);
LWS_VISIBLE LWS_EXTERN void lws_map_free_lwsac(void *v);

LWS_VISIBLE LWS_EXTERN void lws_map_destroy(lws_map_t **pmap);

LWS_VISIBLE LWS_EXTERN lws_map_item_t * lws_map_item_create(lws_map_t *map, const lws_map_key_t key, size_t keylen, const lws_map_value_t value, size_t valuelen);

LWS_VISIBLE LWS_EXTERN void lws_map_item_destroy(lws_map_item_t *item);

LWS_VISIBLE LWS_EXTERN lws_map_item_t * lws_map_item_lookup(lws_map_t *map, const lws_map_key_t key, size_t keylen);
// By GST @Date