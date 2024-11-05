struct lwsac;
typedef unsigned char * lwsac_cached_file_t;

#define lws_list_ptr_container(P,T,M) ((T *)((char *)(P) - offsetof(T, M)))

typedef void * lws_list_ptr;
typedef int (*lws_list_ptr_sort_func_t)(lws_list_ptr a, lws_list_ptr b);

#define lws_list_ptr_advance(_lp) _lp = *((void **)_lp)

LWS_VISIBLE LWS_EXTERN void lws_list_ptr_insert(lws_list_ptr *phead, lws_list_ptr *add, lws_list_ptr_sort_func_t sort);

LWS_VISIBLE LWS_EXTERN void * lwsac_use(struct lwsac **head, size_t ensure, size_t chunk_size);
LWS_VISIBLE LWS_EXTERN void * lwsac_use_backfill(struct lwsac **head, size_t ensure, size_t chunk_size);
LWS_VISIBLE LWS_EXTERN void * lwsac_use_zero(struct lwsac **head, size_t ensure, size_t chunk_size);
#define lwsac_use_zeroed lwsac_use_zero

LWS_VISIBLE LWS_EXTERN void lwsac_free(struct lwsac **head);
LWS_VISIBLE LWS_EXTERN void lwsac_detach(struct lwsac **head);
LWS_VISIBLE LWS_EXTERN void lwsac_reference(struct lwsac *head);
LWS_VISIBLE LWS_EXTERN void lwsac_unreference(struct lwsac **head);
LWS_VISIBLE LWS_EXTERN int lwsac_extend(struct lwsac *head, size_t amount);

LWS_VISIBLE LWS_EXTERN void lwsac_use_cached_file_start(lwsac_cached_file_t cache);
LWS_VISIBLE LWS_EXTERN void lwsac_use_cached_file_end(lwsac_cached_file_t *cache);
LWS_VISIBLE LWS_EXTERN void lwsac_use_cached_file_detach(lwsac_cached_file_t *cache);
LWS_VISIBLE LWS_EXTERN int lwsac_cached_file(const char *filepath, lwsac_cached_file_t *cache, size_t *len);

LWS_VISIBLE LWS_EXTERN size_t lwsac_sizeof(int first);
LWS_VISIBLE LWS_EXTERN size_t lwsac_get_tail_pos(struct lwsac *lac);
LWS_VISIBLE LWS_EXTERN struct lwsac * lwsac_get_next(struct lwsac *lac);
LWS_VISIBLE LWS_EXTERN size_t lwsac_align(size_t length);
LWS_VISIBLE LWS_EXTERN void lwsac_info(struct lwsac *head);
LWS_VISIBLE LWS_EXTERN uint64_t lwsac_total_alloc(struct lwsac *head);
LWS_VISIBLE LWS_EXTERN uint64_t lwsac_total_overhead(struct lwsac *head);

LWS_VISIBLE LWS_EXTERN uint8_t * lwsac_scan_extant(struct lwsac *head, uint8_t *find, size_t len, int nul);
// By GST @Date