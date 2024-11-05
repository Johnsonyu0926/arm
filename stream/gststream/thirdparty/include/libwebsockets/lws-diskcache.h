struct lws_diskcache_scan;

LWS_VISIBLE LWS_EXTERN struct lws_diskcache_scan *
lws_diskcache_create(const char *cache_dir_base, uint64_t cache_size_limit);

LWS_VISIBLE LWS_EXTERN void
lws_diskcache_destroy(struct lws_diskcache_scan **lds);

LWS_VISIBLE LWS_EXTERN int
lws_diskcache_prepare(const char *cache_base_dir, int mode, uid_t uid);

#define LWS_DISKCACHE_QUERY_NO_CACHE 0
#define LWS_DISKCACHE_QUERY_EXISTS 1
#define LWS_DISKCACHE_QUERY_CREATING 2
#define LWS_DISKCACHE_QUERY_ONGOING 3

LWS_VISIBLE LWS_EXTERN int
lws_diskcache_query(struct lws_diskcache_scan *lds, int is_bot,
            const char *hash_hex, int *_fd, char *cache, int cache_len,
            size_t *extant_cache_len);

LWS_VISIBLE LWS_EXTERN int
lws_diskcache_finalize_name(char *cache);

/**
 * lws_diskcache_trim() - performs one or more file checks in the cache for size management
 *
 * \param lds: The opaque object representing the cache
 *
 * This should be called periodically to statefully walk the cache on disk
 * collecting the oldest files.  When it has visited every file, if the cache
 * is oversize it will delete the oldest files until it's back under size again.
 *
 * Each time it's called, it will look at one or more dir in the cache.  If
 * called when the cache is oversize, it increases the amount of work done each
 * call until it is reduced again.  Typically it will take 256 calls before it
 * deletes anything, so if called once per second, it will delete files once
 * every 4 minutes.  Each call is very inexpensive both in memory and time.
 */
LWS_VISIBLE LWS_EXTERN int
lws_diskcache_trim(struct lws_diskcache_scan *lds);

/**
 * lws_diskcache_secs_to_idle() - see how long to idle before calling trim
 *
 * \param lds: The opaque object representing the cache
 *
 * If the cache is undersize, there's no need to monitor it immediately.  This
 * suggests how long to "sleep" before calling `lws_diskcache_trim()` again.
 */
LWS_VISIBLE LWS_EXTERN int
lws_diskcache_secs_to_idle(struct lws_diskcache_scan *lds);

///@}
// By GST @Date