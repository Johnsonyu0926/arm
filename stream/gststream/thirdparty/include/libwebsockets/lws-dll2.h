#define lws_start_foreach_ll(type, it, start)\
{ \
        type it = start; \
        while (it) {

#define lws_end_foreach_ll(it, nxt) \
                it = it->nxt; \
        } \
}

#define lws_start_foreach_ll_safe(type, it, start, nxt)\
{ \
        type it = start; \
        while (it) { \
                type next_##it = it->nxt;

#define lws_end_foreach_ll_safe(it) \
                it = next_##it; \
        } \
}

#define lws_start_foreach_llp(type, it, start)\
{ \
        type it = &(start); \
        while (*(it)) {

#define lws_start_foreach_llp_safe(type, it, start, nxt)\
{ \
        type it = &(start); \
        type next; \
        while (*(it)) { \
                next = &((*(it))->nxt); \

#define lws_end_foreach_llp(it, nxt) \
                it = &(*(it))->nxt; \
        } \
}

#define lws_end_foreach_llp_safe(it) \
                it = next; \
        } \
}

#define lws_ll_fwd_insert(___new_object, ___m_list, ___list_head) {\
                ___new_object->___m_list = ___list_head; \
                ___list_head = ___new_object; \
        }

#define lws_ll_fwd_remove(___type, ___m_list, ___target, ___list_head) { \
                lws_start_foreach_llp(___type **, ___ppss, ___list_head) { \
                        if (*___ppss == ___target) { \
                                *___ppss = ___target->___m_list; \
                                break; \
                        } \
                } lws_end_foreach_llp(___ppss, ___m_list); \
        }

struct lws_dll2;
struct lws_dll2_owner;

typedef struct lws_dll2 {
        struct lws_dll2 *prev;
        struct lws_dll2 *next;
        struct lws_dll2_owner *owner;
} lws_dll2_t;

typedef struct lws_dll2_owner {
        struct lws_dll2 *tail;
        struct lws_dll2 *head;
        uint32_t count;
} lws_dll2_owner_t;

LWS_VISIBLE LWS_EXTERN int
lws_dll2_is_detached(const struct lws_dll2 *d);

static LWS_INLINE const struct lws_dll2_owner *
lws_dll2_owner(const struct lws_dll2 *d) { return d->owner; }

static LWS_INLINE struct lws_dll2 *
lws_dll2_get_head(struct lws_dll2_owner *owner) { return owner->head; }

static LWS_INLINE struct lws_dll2 *
lws_dll2_get_tail(struct lws_dll2_owner *owner) { return owner->tail; }

LWS_VISIBLE LWS_EXTERN void
lws_dll2_add_head(struct lws_dll2 *d, struct lws_dll2_owner *owner);

LWS_VISIBLE LWS_EXTERN void
lws_dll2_add_tail(struct lws_dll2 *d, struct lws_dll2_owner *owner);

LWS_VISIBLE LWS_EXTERN void
lws_dll2_remove(struct lws_dll2 *d);

typedef int (*lws_dll2_foreach_cb_t)(struct lws_dll2 *d, void *user);

LWS_VISIBLE LWS_EXTERN int
lws_dll2_foreach_safe(struct lws_dll2_owner *owner, void *user,
                      lws_dll2_foreach_cb_t cb);

LWS_VISIBLE LWS_EXTERN void
lws_dll2_clear(struct lws_dll2 *d);

LWS_VISIBLE LWS_EXTERN void
lws_dll2_owner_clear(struct lws_dll2_owner *d);

LWS_VISIBLE LWS_EXTERN void
lws_dll2_add_before(struct lws_dll2 *d, struct lws_dll2 *after);

LWS_VISIBLE LWS_EXTERN void
lws_dll2_add_sorted(lws_dll2_t *d, lws_dll2_owner_t *own,
                    int (*compare)(const lws_dll2_t *d, const lws_dll2_t *i));

LWS_VISIBLE LWS_EXTERN void
lws_dll2_add_sorted_priv(lws_dll2_t *d, lws_dll2_owner_t *own, void *priv,
                         int (*compare3)(void *priv, const lws_dll2_t *d,
                                         const lws_dll2_t *i));

LWS_VISIBLE LWS_EXTERN void *
_lws_dll2_search_sz_pl(lws_dll2_owner_t *own, const char *name, size_t namelen,
                      size_t dll2_ofs, size_t ptr_ofs);

#define lws_dll2_search_sz_pl(own, name, namelen, type, membd2list, membptr) \
                ((type *)_lws_dll2_search_sz_pl(own, name, namelen, \
                                       offsetof(type, membd2list), \
                                       offsetof(type, membptr)))

#if defined(_DEBUG)
void
lws_dll2_describe(struct lws_dll2_owner *owner, const char *desc);
#else
#define lws_dll2_describe(x, y)
#endif

#define lws_start_foreach_dll_safe(___type, ___it, ___tmp, ___start) \
{ \
        ___type ___it = ___start; \
        while (___it) { \
                ___type ___tmp = (___it)->next;

#define lws_end_foreach_dll_safe(___it, ___tmp) \
                ___it = ___tmp; \
        } \
}

#define lws_start_foreach_dll(___type, ___it, ___start) \
{ \
        ___type ___it = ___start; \
        while (___it) {

#define lws_end_foreach_dll(___it) \
                ___it = (___it)->next; \
        } \
}