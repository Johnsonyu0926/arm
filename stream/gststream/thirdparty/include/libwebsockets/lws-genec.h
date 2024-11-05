enum enum_genec_alg {
    LEGENEC_UNKNOWN,
    LEGENEC_ECDH,
    LEGENEC_ECDSA
};

struct lws_genec_ctx {
#if defined(LWS_WITH_MBEDTLS)
    union {
        mbedtls_ecdh_context *ctx_ecdh;
        mbedtls_ecdsa_context *ctx_ecdsa;
    } u;
#else
    EVP_PKEY_CTX *ctx[2];
#endif
    struct lws_context *context;
    const struct lws_ec_curves *curve_table;
    enum enum_genec_alg genec_alg;
    char has_private;
};

#if defined(LWS_WITH_MBEDTLS)
enum enum_lws_dh_side {
    LDHS_OURS = MBEDTLS_ECDH_OURS,
    LDHS_THEIRS = MBEDTLS_ECDH_THEIRS
};
#else
enum enum_lws_dh_side {
    LDHS_OURS,
    LDHS_THEIRS
};
#endif

struct lws_ec_curves {
    const char *name;
    int tls_lib_nid;
    uint16_t key_bytes;
};

/* ECDH-specific APIs */
LWS_VISIBLE int
lws_genecdh_create(struct lws_genec_ctx *ctx, struct lws_context *context,
           const struct lws_ec_curves *curve_table);

LWS_VISIBLE LWS_EXTERN int
lws_genecdh_set_key(struct lws_genec_ctx *ctx, struct lws_gencrypto_keyelem *el,
            enum enum_lws_dh_side side);

LWS_VISIBLE LWS_EXTERN int
lws_genecdh_new_keypair(struct lws_genec_ctx *ctx, enum enum_lws_dh_side side,
                const char *curve_name, struct lws_gencrypto_keyelem *el);

LWS_VISIBLE LWS_EXTERN int
lws_genecdh_compute_shared_secret(struct lws_genec_ctx *ctx, uint8_t *ss,
          int *ss_len);

/* ECDSA-specific APIs */
LWS_VISIBLE int
lws_genecdsa_create(struct lws_genec_ctx *ctx, struct lws_context *context,
            const struct lws_ec_curves *curve_table);

LWS_VISIBLE LWS_EXTERN int
lws_genecdsa_new_keypair(struct lws_genec_ctx *ctx, const char *curve_name,
             struct lws_gencrypto_keyelem *el);

LWS_VISIBLE LWS_EXTERN int
lws_genecdsa_set_key(struct lws_genec_ctx *ctx,
             const struct lws_gencrypto_keyelem *el);

LWS_VISIBLE LWS_EXTERN int
lws_genecdsa_hash_sig_verify_jws(struct lws_genec_ctx *ctx, const uint8_t *in,
                 enum lws_genhash_types hash_type, int keybits,
                 const uint8_t *sig, size_t sig_len);

LWS_VISIBLE LWS_EXTERN int
lws_genecdsa_hash_sign_jws(struct lws_genec_ctx *ctx, const uint8_t *in,
               enum lws_genhash_types hash_type, int keybits,
               uint8_t *sig, size_t sig_len);

/* APIs that apply to both ECDH and ECDSA */
LWS_VISIBLE LWS_EXTERN void
lws_genec_destroy(struct lws_genec_ctx *ctx);

LWS_VISIBLE LWS_EXTERN void
lws_genec_destroy_elements(struct lws_gencrypto_keyelem *el);

LWS_VISIBLE LWS_EXTERN int
lws_genec_dump(struct lws_gencrypto_keyelem *el);

///@}
// By GST @Date