
enum lws_gencrypto_kty {
    LWS_GENCRYPTO_KTY_UNKNOWN,
    LWS_GENCRYPTO_KTY_OCT,
    LWS_GENCRYPTO_KTY_RSA,
    LWS_GENCRYPTO_KTY_EC
};

enum lws_gencrypto_oct_tok {
    LWS_GENCRYPTO_OCT_KEYEL_K, /* note... same offset as AES K */
    LWS_GENCRYPTO_OCT_KEYEL_COUNT
};

enum lws_gencrypto_rsa_tok {
    LWS_GENCRYPTO_RSA_KEYEL_E,
    LWS_GENCRYPTO_RSA_KEYEL_N,
    LWS_GENCRYPTO_RSA_KEYEL_D, /* note... same offset as EC D */
    LWS_GENCRYPTO_RSA_KEYEL_P,
    LWS_GENCRYPTO_RSA_KEYEL_Q,
    LWS_GENCRYPTO_RSA_KEYEL_DP,
    LWS_GENCRYPTO_RSA_KEYEL_DQ,
    LWS_GENCRYPTO_RSA_KEYEL_QI,
    LWS_GENCRYPTO_RSA_KEYEL_OTHER,
    LWS_GENCRYPTO_RSA_KEYEL_RI,
    LWS_GENCRYPTO_RSA_KEYEL_DI,
    LWS_GENCRYPTO_RSA_KEYEL_TI,
    LWS_GENCRYPTO_RSA_KEYEL_COUNT
};

enum lws_gencrypto_ec_tok {
    LWS_GENCRYPTO_EC_KEYEL_CRV,
    LWS_GENCRYPTO_EC_KEYEL_X,
    LWS_GENCRYPTO_EC_KEYEL_D = LWS_GENCRYPTO_RSA_KEYEL_D,
    LWS_GENCRYPTO_EC_KEYEL_Y,
    LWS_GENCRYPTO_EC_KEYEL_COUNT
};

enum lws_gencrypto_aes_tok {
    LWS_GENCRYPTO_AES_KEYEL_K = LWS_GENCRYPTO_OCT_KEYEL_K,
    LWS_GENCRYPTO_AES_KEYEL_COUNT
};

#define LWS_GENCRYPTO_MAX_KEYEL_COUNT LWS_GENCRYPTO_RSA_KEYEL_COUNT

typedef struct lws_gencrypto_keyelem {
    uint8_t *buf;
    uint32_t len;
} lws_gc_elem_t;

LWS_VISIBLE LWS_EXTERN int
lws_gencrypto_bits_to_bytes(int bits);

LWS_VISIBLE LWS_EXTERN int
lws_base64_size(int bytes);

LWS_VISIBLE LWS_EXTERN size_t
lws_gencrypto_padded_length(size_t block_size, size_t len);

///@}
// By GST @Date