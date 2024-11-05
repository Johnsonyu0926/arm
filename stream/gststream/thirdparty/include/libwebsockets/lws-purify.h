LWS_VISIBLE LWS_EXTERN const char *lws_sql_purify(char *escaped, const char *string, size_t len);

int lws_sql_purify_len(const char *p);

LWS_VISIBLE LWS_EXTERN const char *lws_json_purify(char *escaped, const char *string, int len, int *in_used);

LWS_VISIBLE LWS_EXTERN int lws_json_purify_len(const char *string);

LWS_VISIBLE LWS_EXTERN void lws_filename_purify_inplace(char *filename);

LWS_VISIBLE LWS_EXTERN int lws_plat_write_cert(struct lws_vhost *vhost, int is_key, int fd, void *buf, size_t len);

LWS_VISIBLE LWS_EXTERN int lws_plat_write_file(const char *filename, void *buf, size_t len);

LWS_VISIBLE LWS_EXTERN int lws_plat_read_file(const char *filename, void *buf, size_t len);

LWS_VISIBLE LWS_EXTERN int lws_plat_recommended_rsa_bits(void);
// By GST @Date