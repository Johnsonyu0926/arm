LWS_VISIBLE LWS_EXTERN struct lws_dsh *
lws_dsh_create(lws_dll2_owner_t *owner, size_t buffer_size, int count_kinds);

LWS_VISIBLE LWS_EXTERN void
lws_dsh_destroy(struct lws_dsh **pdsh);

LWS_VISIBLE LWS_EXTERN int
lws_dsh_alloc_tail(struct lws_dsh *dsh, int kind, const void *src1,
           size_t size1, const void *src2, size_t size2);

LWS_VISIBLE LWS_EXTERN void
lws_dsh_free(void **obj);

LWS_VISIBLE LWS_EXTERN size_t
lws_dsh_get_size(struct lws_dsh *dsh, int kind);

LWS_VISIBLE LWS_EXTERN int
lws_dsh_get_head(struct lws_dsh *dsh, int kind, void **obj, size_t *size);

LWS_VISIBLE LWS_EXTERN void
lws_dsh_describe(struct lws_dsh *dsh, const char *desc);

// By GST @Date