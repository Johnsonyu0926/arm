enum enum_jws_sig_elements {
    /* JWS block namespace */
    LJWS_JOSE,
    LJWS_PYLD,
    LJWS_SIG,
    LJWS_UHDR,

    /* JWE block namespace */
    LJWE_JOSE = 0,
    LJWE_EKEY,
    LJWE_IV,
    LJWE_CTXT,
    LJWE_ATAG,
    LJWE_AAD,

    LWS_JWS_MAX_COMPACT_BLOCKS
};

struct lws_jws_map {
    const char *buf[LWS_JWS_MAX_COMPACT_BLOCKS];
    uint32_t len[LWS_JWS_MAX_COMPACT_BLOCKS];
};

#define LWS_JWS_MAX_SIGS 3

struct lws_jws {
    struct lws_jwk *jwk; /* the struct lws_jwk containing the signing key */
    struct lws_context *context; /* the lws context (used to get random) */
    struct lws_jws_map map, map_b64;
};

LWS_VISIBLE LWS_EXTERN void
lws_jws_init(struct lws_jws *jws, struct lws_jwk *jwk,
         struct lws_context *context);

LWS_VISIBLE LWS_EXTERN void
lws_jws_destroy(struct lws_jws *jws);

LWS_VISIBLE LWS_EXTERN int
lws_jws_sig_confirm_compact(struct lws_jws_map *map, struct lws_jwk *jwk,
                struct lws_context *context,
                char *temp, int *temp_len);

LWS_VISIBLE LWS_EXTERN int
lws_jws_sig_confirm_compact_b64_map(struct lws_jws_map *map_b64,
                    struct lws_jwk *jwk,
                        struct lws_context *context,
                        char *temp, int *temp_len);

LWS_VISIBLE LWS_EXTERN int
lws_jws_sig_confirm_compact_b64(const char *in, size_t len,
                struct lws_jws_map *map,
                struct lws_jwk *jwk,
                struct lws_context *context,
                char *temp, int *temp_len);

LWS_VISIBLE LWS_EXTERN int
lws_jws_sig_confirm(struct lws_jws_map *map_b64, /* b64-encoded */
            struct lws_jws_map *map,    /* non-b64 */
            struct lws_jwk *jwk, struct lws_context *context);

LWS_VISIBLE LWS_EXTERN int
lws_jws_sign_from_b64(struct lws_jose *jose, struct lws_jws *jws, char *b64_sig,
            size_t sig_len);

LWS_VISIBLE LWS_EXTERN int
lws_jws_compact_decode(const char *in, int len, struct lws_jws_map *map,
        struct lws_jws_map *map_b64, char *out, int *out_len);

LWS_VISIBLE LWS_EXTERN int
lws_jws_compact_encode(struct lws_jws_map *map_b64, /* b64-encoded */
               const struct lws_jws_map *map,   /* non-b64 */
               char *buf, int *out_len);

LWS_VISIBLE LWS_EXTERN int
lws_jws_sig_confirm_json(const char *in, size_t len,
             struct lws_jws *jws, struct lws_jwk *jwk,
             struct lws_context *context,
             char *temp, int *temp_len);

/**
 * lws_jws_write_flattened_json() - create flattened JSON sig
 *
 * \param jws: information to include in the signature
 * \param flattened: output buffer for JSON
 * \param len: size of \p flattened output buffer
 *
 */
LWS_VISIBLE LWS_EXTERN int
lws_jws_write_flattened_json(struct lws_jws *jws, char *flattened, size_t len);

LWS_VISIBLE LWS_EXTERN int
lws_jws_write_compact(struct lws_jws *jws, char *compact, size_t len);

LWS_VISIBLE LWS_EXTERN int
lws_jws_dup_element(struct lws_jws_map *map, int idx,
            char *temp, int *temp_len, const void *in, size_t in_len,
            size_t actual_alloc);

LWS_VISIBLE LWS_EXTERN int
lws_jws_randomize_element(struct lws_context *context,
              struct lws_jws_map *map,
              int idx, char *temp, int *temp_len, size_t random_len,
              size_t actual_alloc);

LWS_VISIBLE LWS_EXTERN int
lws_jws_alloc_element(struct lws_jws_map *map, int idx, char *temp,
              int *temp_len, size_t len, size_t actual_alloc);

LWS_VISIBLE LWS_EXTERN int
lws_jws_encode_b64_element(struct lws_jws_map *map, int idx,
               char *temp, int *temp_len, const void *in,
               size_t in_len);

LWS_VISIBLE LWS_EXTERN int
lws_jws_b64_compact_map(const char *in, int len, struct lws_jws_map *map);

LWS_VISIBLE LWS_EXTERN int
lws_jws_base64_enc(const char *in, size_t in_len, char *out, size_t out_max);

LWS_VISIBLE LWS_EXTERN int
lws_jws_encode_section(const char *in, size_t in_len, int first, char **p,
               char *end);

LWS_VISIBLE LWS_EXTERN int
lws_jwt_signed_validate(struct lws_context *ctx, struct lws_jwk *jwk,
            const char *alg_list, const char *com, size_t len,
            char *temp, int tl, char *out, size_t *out_len);

LWS_VISIBLE LWS_EXTERN int
lws_jwt_sign_compact(struct lws_context *ctx, struct lws_jwk *jwk,
             const char *alg, char *out, size_t *out_len, char *temp,
             int tl, const char *format, ...) LWS_FORMAT(8);

struct lws_jwt_sign_info {
    const char *alg;
    /**< entry: signing alg name, like "RS256" */
    const char *jose_hdr;
    /**< entry: optional JOSE hdr; if present, alg field is ignored; instead the
     *          whole claim object has to be provided in this parameter */
    size_t jose_hdr_len;
    /**< entry: if jose_hdr is not NULL, JOSE header length without terminating '\0' */
    char *out;
    /**< exit: signed JWT in compact form*/
    size_t *out_len;
    /**< entry,exit: buffer size of out; actual size of JWT on exit */
    char *temp;
    /**< exit undefined content, used by the function as a temporary scratchpad; MUST
     * be large enogh to store various intermediate representations */
    int tl;
    /**< entry: size of temp buffer */
};

LWS_VISIBLE LWS_EXTERN int
lws_jwt_sign_via_info(struct lws_context *ctx, struct lws_jwk *jwk,
         const struct lws_jwt_sign_info *info, const char *format, ...) LWS_FORMAT(4);

LWS_VISIBLE LWS_EXTERN int
lws_jwt_token_sanity(const char *in, size_t in_len,
             const char *iss, const char *aud, const char *csrf_in,
             char *sub, size_t sub_len, unsigned long *exp_unix_time);

#if defined(LWS_ROLE_H1) || defined(LWS_ROLE_H2)

struct lws_jwt_sign_set_cookie {
    struct lws_jwk          *jwk;
    /**< entry: required signing key */
    const char          *alg;
    /**< entry: required signing alg, eg, "ES512" */
    const char          *iss;
    /**< entry: issuer name to use */
    const char          *aud;
    /**< entry: audience */
    const char          *cookie_name;
    /**< entry: the name of the cookie */
    char                sub[33];
    /**< sign-entry, validate-exit: subject */
    const char          *extra_json;
    /**< sign-entry, validate-exit:
     * optional "ext" JSON object contents for the JWT */
    size_t              extra_json_len;
    /**< validate-exit:
     * length of optional "ext" JSON object contents for the JWT */
    const char          *csrf_in;
    /**< validate-entry:
     * NULL, or an external CSRF token to check against what is in the JWT */
    unsigned long           expiry_unix_time;
    /**< sign-entry: seconds the JWT and cookie may live,
     * validate-exit: expiry unix time */
};

LWS_VISIBLE LWS_EXTERN int
lws_jwt_sign_token_set_http_cookie(struct lws *wsi,
                   const struct lws_jwt_sign_set_cookie *i,
                   uint8_t **p, uint8_t *end);
LWS_VISIBLE LWS_EXTERN int
lws_jwt_get_http_cookie_validate_jwt(struct lws *wsi,
                     struct lws_jwt_sign_set_cookie *i,
                     char *out, size_t *out_len);
#endif

///@}
// By GST @Date