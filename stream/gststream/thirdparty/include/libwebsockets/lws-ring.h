//GST
struct lws_ring;

LWS_VISIBLE LWS_EXTERN struct lws_ring *lws_ring_create(size_t element_len, size_t count, void (*destroy_element)(void *element));

LWS_VISIBLE LWS_EXTERN void lws_ring_destroy(struct lws_ring *ring);

LWS_VISIBLE LWS_EXTERN size_t lws_ring_get_count_free_elements(struct lws_ring *ring);

LWS_VISIBLE LWS_EXTERN size_t lws_ring_get_count_waiting_elements(struct lws_ring *ring, uint32_t *tail);

LWS_VISIBLE LWS_EXTERN size_t lws_ring_insert(struct lws_ring *ring, const void *src, size_t max_count);

LWS_VISIBLE LWS_EXTERN size_t lws_ring_consume(struct lws_ring *ring, uint32_t *tail, void *dest, size_t max_count);

LWS_VISIBLE LWS_EXTERN const void *lws_ring_get_element(struct lws_ring *ring, uint32_t *tail);

LWS_VISIBLE LWS_EXTERN void lws_ring_update_oldest_tail(struct lws_ring *ring, uint32_t tail);

LWS_VISIBLE LWS_EXTERN uint32_t lws_ring_get_oldest_tail(struct lws_ring *ring);

LWS_VISIBLE LWS_EXTERN int lws_ring_next_linear_insert_range(struct lws_ring *ring, void **start, size_t *bytes);

LWS_VISIBLE LWS_EXTERN void lws_ring_bump_head(struct lws_ring *ring, size_t bytes);

LWS_VISIBLE LWS_EXTERN void lws_ring_dump(struct lws_ring *ring, uint32_t *tail);

#define lws_ring_consume_and_update_oldest_tail(___ring, ___type, ___ptail, ___count, ___list_head, ___mtail, ___mlist) { \
    int ___n, ___m; \
    ___n = lws_ring_get_oldest_tail(___ring) == *(___ptail); \
    lws_ring_consume(___ring, ___ptail, NULL, ___count); \
    if (___n) { \
        uint32_t ___oldest; \
        ___n = 0; \
        ___oldest = *(___ptail); \
        lws_start_foreach_llp(___type **, ___ppss, ___list_head) { \
            ___m = (int)lws_ring_get_count_waiting_elements(___ring, &(*___ppss)->___mtail); \
            if (___m >= ___n) { \
                ___n = ___m; \
                ___oldest = (*___ppss)->___mtail; \
            } \
        } lws_end_foreach_llp(___ppss, ___mlist); \
        lws_ring_update_oldest_tail(___ring, ___oldest); \
    } \
}

#define lws_ring_consume_single_tail(___ring, ___ptail, ___count) { \
    lws_ring_consume(___ring, ___ptail, NULL, ___count); \
    lws_ring_update_oldest_tail(___ring, *(___ptail)); \
}
// By GST @Date