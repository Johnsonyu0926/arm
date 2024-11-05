#define LWS_JWE_RFC3394_OVERHEAD_BYTES 8
#define LWS_JWE_AES_IV_BYTES 16

#define LWS_JWE_LIMIT_RSA_KEY_BITS 4096
#define LWS_JWE_LIMIT_AES_KEY_BITS (512 + 64) /* RFC3394 Key Wrap adds 64b */
#define LWS_JWE_LIMIT_EC_KEY_BITS  528 /* 521 rounded to byte boundary */
#define LWS_JWE_LIMIT_HASH_BITS    (LWS_GENHASH_LARGEST * 8)

/* the largest key element for any cipher */
#define LWS_JWE_LIMIT_KEY_ELEMENT_BYTES (LWS_JWE_LIMIT_RSA_KEY_BITS / 8)

struct lws_jwe {
    struct lws_jose jose;
    struct lws_jws jws;
    struct lws_jwk jwk;

    /*
     * We have to keep a copy of the CEK so we can reuse it with later
     * key encryptions for the multiple recipient case.
     */
    uint8_t cek[LWS_JWE_LIMIT_KEY_ELEMENT_BYTES];
    unsigned int cek_valid:1;

    int recip;
};

LWS_VISIBLE LWS_EXTERN void
lws_jwe_init(struct lws_jwe *jwe, struct lws_context *context);

LWS_VISIBLE LWS_EXTERN void
lws_jwe_destroy(struct lws_jwe *jwe);

LWS_VISIBLE LWS_EXTERN void
lws_jwe_be64(uint64_t c, uint8_t *p8);

LWS_VISIBLE LWS_EXTERN int
lws_jwe_render_compact(struct lws_jwe *jwe, char *out, size_t out_len);

LWS_VISIBLE int
lws_jwe_render_flattened(struct lws_jwe *jwe, char *out, size_t out_len);

LWS_VISIBLE LWS_EXTERN int
lws_jwe_json_parse(struct lws_jwe *jwe, const uint8_t *buf, int len,
           char *temp, int *temp_len);

LWS_VISIBLE LWS_EXTERN int
lws_jwe_auth_and_decrypt(struct lws_jwe *jwe, char *temp, int *temp_len);

LWS_VISIBLE LWS_EXTERN int
lws_jwe_encrypt(struct lws_jwe *jwe, char *temp, int *temp_len);

LWS_VISIBLE LWS_EXTERN int
lws_jwe_create_packet(struct lws_jwe *jwe,
              const char *payload, size_t len, const char *nonce,
              char *out, size_t out_len, struct lws_context *context);

/* only exposed because we have test vectors that need it */
LWS_VISIBLE LWS_EXTERN int
lws_jwe_auth_and_decrypt_cbc_hs(struct lws_jwe *jwe, uint8_t *enc_cek,
                    uint8_t *aad, int aad_len);

/* only exposed because we have test vectors that need it */
LWS_VISIBLE LWS_EXTERN int
lws_jwa_concat_kdf(struct lws_jwe *jwe, int direct,
           uint8_t *out, const uint8_t *shared_secret, int sslen);
// By GST @Date