enum lws_tls_cert_info {
    LWS_TLS_CERT_INFO_VALIDITY_FROM,
    LWS_TLS_CERT_INFO_VALIDITY_TO,
    LWS_TLS_CERT_INFO_COMMON_NAME,
    LWS_TLS_CERT_INFO_ISSUER_NAME,
    LWS_TLS_CERT_INFO_USAGE,
    LWS_TLS_CERT_INFO_VERIFIED,
    LWS_TLS_CERT_INFO_OPAQUE_PUBLIC_KEY,
    LWS_TLS_CERT_INFO_DER_RAW,
    LWS_TLS_CERT_INFO_AUTHORITY_KEY_ID,
    LWS_TLS_CERT_INFO_AUTHORITY_KEY_ID_ISSUER,
    LWS_TLS_CERT_INFO_AUTHORITY_KEY_ID_SERIAL,
    LWS_TLS_CERT_INFO_SUBJECT_KEY_ID,
};

union lws_tls_cert_info_results {
    unsigned int verified;
    time_t time;
    unsigned int usage;
    struct {
        int len;
        char name[64];
    } ns;
};

struct lws_x509_cert;
struct lws_jwk;

LWS_VISIBLE LWS_EXTERN int lws_x509_create(struct lws_x509_cert **x509);

LWS_VISIBLE LWS_EXTERN int lws_x509_parse_from_pem(struct lws_x509_cert *x509, const void *pem, size_t len);

LWS_VISIBLE LWS_EXTERN int lws_x509_verify(struct lws_x509_cert *x509, struct lws_x509_cert *trusted, const char *common_name);

LWS_VISIBLE LWS_EXTERN int lws_x509_public_to_jwk(struct lws_jwk *jwk, struct lws_x509_cert *x509, const char *curves, int rsabits);

LWS_VISIBLE LWS_EXTERN int lws_x509_jwk_privkey_pem(struct lws_context *cx, struct lws_jwk *jwk, void *pem, size_t len, const char *passphrase);

LWS_VISIBLE LWS_EXTERN void lws_x509_destroy(struct lws_x509_cert **x509);

LWS_VISIBLE LWS_EXTERN int lws_x509_info(struct lws_x509_cert *x509, enum lws_tls_cert_info type, union lws_tls_cert_info_results *buf, size_t len);

LWS_VISIBLE LWS_EXTERN int lws_tls_peer_cert_info(struct lws *wsi, enum lws_tls_cert_info type, union lws_tls_cert_info_results *buf, size_t len);

LWS_VISIBLE LWS_EXTERN int lws_tls_vhost_cert_info(struct lws_vhost *vhost, enum lws_tls_cert_info type, union lws_tls_cert_info_results *buf, size_t len);

LWS_VISIBLE LWS_EXTERN int lws_tls_acme_sni_cert_create(struct lws_vhost *vhost, const char *san_a, const char *san_b);

LWS_VISIBLE LWS_EXTERN int lws_tls_acme_sni_csr_create(struct lws_context *context, const char *elements[], uint8_t *csr, size_t csr_len, char **privkey_pem, size_t *privkey_len);

LWS_VISIBLE LWS_EXTERN int lws_tls_cert_updated(struct lws_context *context, const char *certpath, const char *keypath, const char *mem_cert, size_t len_mem_cert, const char *mem_privkey, size_t len_mem_privkey);