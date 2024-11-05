enum enum_jwk_meta_tok {
    JWK_META_KTY,
    JWK_META_KID,
    JWK_META_USE,
    JWK_META_KEY_OPS,
    JWK_META_X5C,
    JWK_META_ALG,

    LWS_COUNT_JWK_ELEMENTS
};

struct lws_jwk {
    /* key data elements */
    struct lws_gencrypto_keyelem e[LWS_GENCRYPTO_MAX_KEYEL_COUNT];
    /* generic meta key elements, like KID */
    struct lws_gencrypto_keyelem meta[LWS_COUNT_JWK_ELEMENTS];
    int kty;            /**< one of LWS_GENCRYPTO_KTY_ */
    char private_key; /* nonzero = has private key elements */
};

typedef int (*lws_jwk_key_import_callback)(struct lws_jwk *s, void *user);

struct lws_jwk_parse_state {
    struct lws_jwk *jwk;
    char b64[(((8192 / 8) * 4) / 3) + 1]; /* enough for 8Kb key */
    lws_jwk_key_import_callback per_key_cb;
    void *user;
    int pos;
    int cose_state;
    int seen;
    unsigned short possible;
};

LWS_VISIBLE LWS_EXTERN int
lws_jwk_import(struct lws_jwk *jwk, lws_jwk_key_import_callback cb, void *user,
           const char *in, size_t len);

/** lws_jwk_destroy() - Destroy a JSON Web key
 *
 * \param jwk: the JWK object to destroy
 *
 * All allocations in the lws_jwk are destroyed
 */
LWS_VISIBLE LWS_EXTERN void
lws_jwk_destroy(struct lws_jwk *jwk);

/** lws_jwk_dup_oct() - Set a jwk to a dup'd binary OCT key
 *
 * \param jwk: the JWK object to set
 * \param key: the JWK object to destroy
 * \param len: the JWK object to destroy
 *
 * Sets the kty to OCT, allocates len bytes for K and copies len bytes of key
 * into the allocation.
 */
LWS_VISIBLE LWS_EXTERN int
lws_jwk_dup_oct(struct lws_jwk *jwk, const void *key, int len);

#define LWSJWKF_EXPORT_PRIVATE              (1 << 0)
#define LWSJWKF_EXPORT_NOCRLF               (1 << 1)

LWS_VISIBLE LWS_EXTERN int
lws_jwk_export(struct lws_jwk *jwk, int flags, char *p, int *len);

LWS_VISIBLE LWS_EXTERN int
lws_jwk_load(struct lws_jwk *jwk, const char *filename,
         lws_jwk_key_import_callback cb, void *user);

/** lws_jwk_save() - Export a JSON Web key to a file
 *
 * \param jwk: the JWK object to save from
 * \param filename: filename to save to
 *
 * Returns 0 for OK or -1 for failure
 */
LWS_VISIBLE LWS_EXTERN int
lws_jwk_save(struct lws_jwk *jwk, const char *filename);

/** lws_jwk_rfc7638_fingerprint() - jwk to RFC7638 compliant fingerprint
 *
 * \param jwk: the JWK object to fingerprint
 * \param digest32: buffer to take 32-byte digest
 *
 * Returns 0 for OK or -1 for failure
 */
LWS_VISIBLE LWS_EXTERN int
lws_jwk_rfc7638_fingerprint(struct lws_jwk *jwk, char *digest32);

/** lws_jwk_strdup_meta() - allocate a duplicated string meta element
 *
 * \param jwk: the JWK object to fingerprint
 * \param idx: JWK_META_ element index
 * \param in: string to copy
 * \param len: length of string to copy
 *
 * Returns 0 for OK or -1 for failure
 */
LWS_VISIBLE LWS_EXTERN int
lws_jwk_strdup_meta(struct lws_jwk *jwk, enum enum_jwk_meta_tok idx,
            const char *in, int len);

LWS_VISIBLE LWS_EXTERN int
lws_jwk_dump(struct lws_jwk *jwk);

LWS_VISIBLE int
lws_jwk_generate(struct lws_context *context, struct lws_jwk *jwk,
             enum lws_gencrypto_kty kty, int bits, const char *curve);
// By GST @Date