enum enum_genrsa_mode {
    LGRSAM_PKCS1_1_5,
    LGRSAM_PKCS1_OAEP_PSS,
    LGRSAM_COUNT
};

struct lws_genrsa_ctx {
#if defined(LWS_WITH_MBEDTLS)
    mbedtls_rsa_context *ctx;
#else
    BIGNUM *bn[LWS_GENCRYPTO_RSA_KEYEL_COUNT];
    EVP_PKEY_CTX *ctx;
    RSA *rsa;
#endif
    struct lws_context *context;
    enum enum_genrsa_mode mode;
};

LWS_VISIBLE LWS_EXTERN int
lws_genrsa_create(struct lws_genrsa_ctx *ctx,
          const struct lws_gencrypto_keyelem *el,
          struct lws_context *context, enum enum_genrsa_mode mode,
          enum lws_genhash_types oaep_hashid);

LWS_VISIBLE LWS_EXTERN void
lws_genrsa_destroy_elements(struct lws_gencrypto_keyelem *el);

LWS_VISIBLE LWS_EXTERN int
lws_genrsa_new_keypair(struct lws_context *context, struct lws_genrsa_ctx *ctx,
               enum enum_genrsa_mode mode, struct lws_gencrypto_keyelem *el,
               int bits);

LWS_VISIBLE LWS_EXTERN int
lws_genrsa_public_encrypt(struct lws_genrsa_ctx *ctx, const uint8_t *in,
              size_t in_len, uint8_t *out);

LWS_VISIBLE LWS_EXTERN int
lws_genrsa_private_encrypt(struct lws_genrsa_ctx *ctx, const uint8_t *in,
               size_t in_len, uint8_t *out);

LWS_VISIBLE LWS_EXTERN int
lws_genrsa_public_decrypt(struct lws_genrsa_ctx *ctx, const uint8_t *in,
              size_t in_len, uint8_t *out, size_t out_max);

LWS_VISIBLE LWS_EXTERN int
lws_genrsa_private_decrypt(struct lws_genrsa_ctx *ctx, const uint8_t *in,
               size_t in_len, uint8_t *out, size_t out_max);

LWS_VISIBLE LWS_EXTERN int
lws_genrsa_hash_sig_verify(struct lws_genrsa_ctx *ctx, const uint8_t *in,
               enum lws_genhash_types hash_type,
               const uint8_t *sig, size_t sig_len);

LWS_VISIBLE LWS_EXTERN int
lws_genrsa_hash_sign(struct lws_genrsa_ctx *ctx, const uint8_t *in,
             enum lws_genhash_types hash_type,
             uint8_t *sig, size_t sig_len);

LWS_VISIBLE LWS_EXTERN void
lws_genrsa_destroy(struct lws_genrsa_ctx *ctx);

LWS_VISIBLE LWS_EXTERN int
lws_genrsa_render_pkey_asn1(struct lws_genrsa_ctx *ctx, int _private,
                uint8_t *pkey_asn1, size_t pkey_asn1_len);

///@}
// By GST @Date