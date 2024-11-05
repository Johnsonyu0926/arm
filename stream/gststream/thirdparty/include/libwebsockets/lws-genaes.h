#if defined(LWS_WITH_MBEDTLS)
#include <mbedtls/aes.h>
#include <mbedtls/gcm.h>
#endif

enum enum_aes_modes {
    LWS_GAESM_CBC,
    LWS_GAESM_CFB128,
    LWS_GAESM_CFB8,
    LWS_GAESM_CTR,
    LWS_GAESM_ECB,
    LWS_GAESM_OFB,
    LWS_GAESM_XTS,      /* care... requires double-length key */
    LWS_GAESM_GCM,
    LWS_GAESM_KW,
};

enum enum_aes_operation {
    LWS_GAESO_ENC,
    LWS_GAESO_DEC
};

enum enum_aes_padding {
    LWS_GAESP_NO_PADDING,
    LWS_GAESP_WITH_PADDING
};

#define LWS_AES_BLOCKSIZE 128
#define LWS_AES_CBC_BLOCKLEN 16

struct lws_genaes_ctx {
#if defined(LWS_WITH_MBEDTLS)
    union {
        mbedtls_aes_context ctx;
#if defined(MBEDTLS_CIPHER_MODE_XTS)
        mbedtls_aes_xts_context ctx_xts;
#endif
        mbedtls_gcm_context ctx_gcm;
    } u;
#else
    EVP_CIPHER_CTX *ctx;
    const EVP_CIPHER *cipher;
    ENGINE *engine;
    char init;
#endif
    unsigned char tag[16];
    struct lws_gencrypto_keyelem *k;
    enum enum_aes_operation op;
    enum enum_aes_modes mode;
    enum enum_aes_padding padding;
    int taglen;
    char underway;
};

LWS_VISIBLE LWS_EXTERN int
lws_genaes_create(struct lws_genaes_ctx *ctx, enum enum_aes_operation op,
          enum enum_aes_modes mode, struct lws_gencrypto_keyelem *el,
          enum enum_aes_padding padding, void *engine);

LWS_VISIBLE LWS_EXTERN int
lws_genaes_destroy(struct lws_genaes_ctx *ctx, unsigned char *tag, size_t tlen);

LWS_VISIBLE LWS_EXTERN int
lws_genaes_crypt(struct lws_genaes_ctx *ctx, const uint8_t *in, size_t len,
         uint8_t *out,
         uint8_t *iv_or_nonce_ctr_or_data_unit_16,
         uint8_t *stream_block_16,
         size_t *nc_or_iv_off, int taglen);

///@}
// By GST @Date