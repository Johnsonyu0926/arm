enum {
	LWSCOSE_WKL_ALG = 1,
	LWSCOSE_WKL_CRIT,
	LWSCOSE_WKL_CONTENT_TYPE,
	LWSCOSE_WKL_KID,
	LWSCOSE_WKL_IV,
	LWSCOSE_WKL_IV_PARTIAL,
	LWSCOSE_WKL_COUNTERSIG,
	LWSCOSE_WKL_COUNTERSIG0 = 9,
	LWSCOSE_WKL_KID_CONTEXT,
	LWSCOSE_WKL_CUPH_NONCE = 256,
	LWSCOSE_WKL_CUPH_OWNER_PUBKEY = 257,

	LWSCOSE_WKK_KTY = 1,
	LWSCOSE_WKK_KID,
	LWSCOSE_WKK_ALG,
	LWSCOSE_WKK_KEY_OPS,
	LWSCOSE_WKK_BASE_IV,

	LWSCOSE_WKKO_SIGN = 1,
	LWSCOSE_WKKO_VERIFY,
	LWSCOSE_WKKO_ENCRYPT,
	LWSCOSE_WKKO_DECRYPT,
	LWSCOSE_WKKO_WRAP_KEY,
	LWSCOSE_WKKO_UNWRAP_KEY,
	LWSCOSE_WKKO_DERIVE_KEY,
	LWSCOSE_WKKO_DERIVE_BITS,
	LWSCOSE_WKKO_MAC_CREATE,
	LWSCOSE_WKKO_MAC_VERIFY,

	LWSCOSE_WKAECDSA_ALG_ES256 = -7,
	LWSCOSE_WKAECDSA_ALG_ES384 = -35,
	LWSCOSE_WKAECDSA_ALG_ES512 = -36,

	LWSCOSE_WKAEDDSA_ALG_EDDSA = -8,

	LWSCOSE_WKAHMAC_256_64 = 4,
	LWSCOSE_WKAHMAC_256_256,
	LWSCOSE_WKAHMAC_384_384,
	LWSCOSE_WKAHMAC_512_512,

	LWSCOSE_WKAAES_128_64 = 14,
	LWSCOSE_WKAAES_256_64,
	LWSCOSE_WKAAES_128_128 = 25,
	LWSCOSE_WKAAES_256_128,

	LWSCOSE_WKAAESGCM_128 = 1,
	LWSCOSE_WKAAESGCM_192,
	LWSCOSE_WKAAESGCM_256,

	LWSCOSE_WKAAESCCM_16_64_128 = 10,
	LWSCOSE_WKAAESCCM_16_64_256,
	LWSCOSE_WKAAESCCM_64_64_128,
	LWSCOSE_WKAAESCCM_64_64_256,
	LWSCOSE_WKAAESCCM_16_128_128,
	LWSCOSE_WKAAESCCM_16_128_256,
	LWSCOSE_WKAAESCCM_64_128_128,
	LWSCOSE_WKAAESCCM_64_128_256,

	LWSCOSE_WKACHACHA_POLY1305 = 24,

	LWSCOSE_WKAPHKDF_SALT = -20,

	LWSCOSE_WKAPCTX_PARTY_U_IDENTITY = -21,
	LWSCOSE_WKAPCTX_PARTY_U_NONCE = -22,
	LWSCOSE_WKAPCTX_PARTY_U_OTHER = -23,
	LWSCOSE_WKAPCTX_PARTY_V_IDENTITY = -24,
	LWSCOSE_WKAPCTX_PARTY_V_NONCE = -25,
	LWSCOSE_WKAPCTX_PARTY_V_OTHER = -26,

	LWSCOSE_WKK_DIRECT_CEK = -6,

	LWSCOSE_WKK_DIRECT_HKDF_SHA_256 = -10,
	LWSCOSE_WKK_DIRECT_HKDF_SHA_512 = -11,
	LWSCOSE_WKK_DIRECT_HKDF_AES_128 = -12,
	LWSCOSE_WKK_DIRECT_HKDF_AES_256 = -13,

	LWSCOSE_WKK_DIRECT_HKDFKW_SHA_256 = -3,
	LWSCOSE_WKK_DIRECT_HKDFKW_SHA_512 = -4,
	LWSCOSE_WKK_DIRECT_HKDFKW_AES_128 = -5,

	LWSCOSE_WKAECDH_ALG_ES_HKDF_256 = -25,
	LWSCOSE_WKAECDH_ALG_ES_HKDF_512 = -26,
	LWSCOSE_WKAECDH_ALG_SS_HKDF_256 = -27,
	LWSCOSE_WKAECDH_ALG_SS_HKDF_512 = -28,

	LWSCOSE_WKAPECDH_EPHEMERAL_KEY = -1,
	LWSCOSE_WKAPECDH_STATIC_KEY = -2,
	LWSCOSE_WKAPECDH_STATIC_KEY_ID = -3,

	LWSCOSE_WKAPECDH_ES_A128KW = -29,
	LWSCOSE_WKAPECDH_ES_A192KW = -30,
	LWSCOSE_WKAPECDH_ES_A256KW = -31,
	LWSCOSE_WKAPECDH_SS_A128KW = -32,
	LWSCOSE_WKAPECDH_SS_A192KW = -33,
	LWSCOSE_WKAPECDH_SS_A256KW = -34,

	LWSCOSE_WKKTV_OKP = 1,
	LWSCOSE_WKKTV_EC2 = 2,
	LWSCOSE_WKKTV_RSA = 3,
	LWSCOSE_WKKTV_SYMMETRIC = 4,
	LWSCOSE_WKKTV_HSS_LMS = 5,
	LWSCOSE_WKKTV_WALNUTDSA = 6,

	LWSCOSE_WKEC_P256 = 1,
	LWSCOSE_WKEC_P384,
	LWSCOSE_WKEC_P521,
	LWSCOSE_WKEC_X25519,
	LWSCOSE_WKEC_X448,
	LWSCOSE_WKEC_ED25519,
	LWSCOSE_WKEC_ED448,
	LWSCOSE_WKEC_SECP256K1,

	LWSCOSE_WKECKP_CRV = -1,
	LWSCOSE_WKECKP_X = -2,
	LWSCOSE_WKECKP_Y = -3,
	LWSCOSE_WKECKP_D = -4,

	LWSCOSE_WKOKP_CRV = -1,
	LWSCOSE_WKOKP_X = -2,
	LWSCOSE_WKOKP_D = -4,

	LWSCOSE_WKKPRSA_N = -1,
	LWSCOSE_WKKPRSA_E = -2,
	LWSCOSE_WKKPRSA_D = -3,
	LWSCOSE_WKKPRSA_P = -4,
	LWSCOSE_WKKPRSA_Q = -5,
	LWSCOSE_WKKPRSA_DP = -6,
	LWSCOSE_WKKPRSA_DQ = -7,
	LWSCOSE_WKKPRSA_QINV = -8,
	LWSCOSE_WKKPRSA_OTHER = -9,
	LWSCOSE_WKKPRSA_RI = -10,
	LWSCOSE_WKKPRSA_DI = -11,
	LWSCOSE_WKKPRSA_TI = -12,

	LWSCOSE_WKSYMKP_KEY_VALUE = 4,

	LWSCOAP_CONTENTFORMAT_COSE_SIGN = 98,
	LWSCOAP_CONTENTFORMAT_COSE_SIGN1 = 18,
	LWSCOAP_CONTENTFORMAT_COSE_ENCRYPT = 96,
	LWSCOAP_CONTENTFORMAT_COSE_ENCRYPT0 = 16,
	LWSCOAP_CONTENTFORMAT_COSE_MAC = 97,
	LWSCOAP_CONTENTFORMAT_COSE_MAC0 = 17,
	LWSCOAP_CONTENTFORMAT_COSE_KEY = 101,
	LWSCOAP_CONTENTFORMAT_COSE_KEY_SET = 102,

	LWSCOSE_WKL_COUNTERSIGNATURE0 = 9,

	LWSCOSE_WKARSA_ALG_RS256 = -257,
	LWSCOSE_WKARSA_ALG_RS384 = -258,
	LWSCOSE_WKARSA_ALG_RS512 = -259,
};

enum enum_cose_key_meta_tok {
	COSEKEY_META_KTY,
	COSEKEY_META_KID,
	COSEKEY_META_KEY_OPS,
	COSEKEY_META_BASE_IV,
	COSEKEY_META_ALG,

	LWS_COUNT_COSE_KEY_ELEMENTS
};

typedef int64_t cose_param_t;

LWS_VISIBLE LWS_EXTERN const char *
lws_cose_alg_to_name(cose_param_t alg);

LWS_VISIBLE LWS_EXTERN cose_param_t
lws_cose_name_to_alg(const char *name);

typedef struct lws_cose_key {
	struct lws_gencrypto_keyelem e[LWS_GENCRYPTO_MAX_KEYEL_COUNT];
	struct lws_gencrypto_keyelem meta[LWS_COUNT_COSE_KEY_ELEMENTS];
	lws_dll2_t list;
	int gencrypto_kty;
	cose_param_t kty;
	cose_param_t cose_alg;
	cose_param_t cose_curve;
	char private_key;
} lws_cose_key_t;

typedef int (*lws_cose_key_import_callback)(struct lws_cose_key *s, void *user);

LWS_VISIBLE LWS_EXTERN lws_cose_key_t *
lws_cose_key_import(lws_dll2_owner_t *pkey_set, lws_cose_key_import_callback cb,
		    void *user, const uint8_t *in, size_t len);

LWS_VISIBLE LWS_EXTERN enum lws_lec_pctx_ret
lws_cose_key_export(lws_cose_key_t *ck, lws_lec_pctx_t *ctx, int flags);

LWS_VISIBLE LWS_EXTERN lws_cose_key_t *
lws_cose_key_generate(struct lws_context *context, cose_param_t cose_kty,
		      int use_mask, int bits, const char *curve,
		      const uint8_t *kid, size_t kl);

LWS_VISIBLE LWS_EXTERN lws_cose_key_t *
lws_cose_key_from_set(lws_dll2_owner_t *set, const uint8_t *kid, size_t kl);

LWS_VISIBLE LWS_EXTERN void
lws_cose_key_destroy(lws_cose_key_t **ck);

LWS_VISIBLE LWS_EXTERN void
lws_cose_key_set_destroy(lws_dll2_owner_t *o);

LWS_VISIBLE LWS_EXTERN void
lws_cose_key_dump(const lws_cose_key_t *ck);

struct lws_cose_validate_context;

enum lws_cose_sig_types {
	SIGTYPE_UNKNOWN,
	SIGTYPE_MULTI,
	SIGTYPE_SINGLE,
	SIGTYPE_COUNTERSIGNED,
	SIGTYPE_MAC,
	SIGTYPE_MAC0,
};

typedef struct {
	lws_dll2_t list;
	const lws_cose_key_t *cose_key;
	cose_param_t cose_alg;
	int result;
} lws_cose_validate_res_t;

enum {
	LCOSESIGEXTCB_RET_FINISHED,
	LCOSESIGEXTCB_RET_AGAIN,
	LCOSESIGEXTCB_RET_ERROR = -1
};

typedef struct {
	struct lws_cose_validate_context *cps;
	const uint8_t *ext;
	size_t xl;
} lws_cose_sig_ext_pay_t;

typedef int (*lws_cose_sign_ext_pay_cb_t)(lws_cose_sig_ext_pay_t *x);
typedef int (*lws_cose_validate_pay_cb_t)(struct lws_cose_validate_context *cps,
					  void *opaque, const uint8_t *paychunk,
					  size_t paychunk_len);

typedef struct lws_cose_validate_create_info {
	struct lws_context *cx;
	lws_dll2_owner_t *keyset;
	enum lws_cose_sig_types sigtype;
	lws_cose_validate_pay_cb_t pay_cb;
	void *pay_opaque;
	lws_cose_sign_ext_pay_cb_t ext_cb;
	void *ext_opaque;
	size_t ext_len;
} lws_cose_validate_create_info_t;

LWS_VISIBLE LWS_EXTERN struct lws_cose_validate_context *
lws_cose_validate_create(const lws_cose_validate_create_info_t *info);

LWS_VISIBLE LWS_EXTERN int
lws_cose_validate_chunk(struct lws_cose_validate_context *cps,
			const uint8_t *in, size_t in_len, size_t *used_in);

LWS_VISIBLE LWS_EXTERN lws_dll2_owner_t *
lws_cose_validate_results(struct lws_cose_validate_context *cps);

LWS_VISIBLE LWS_EXTERN void
lws_cose_validate_destroy(struct lws_cose_validate_context **cps);

struct lws_cose_sign_context;

#define LCSC_FL_ADD_CBOR_TAG		(1 << 0)
#define LCSC_FL_ADD_CBOR_PREFER_MAC0	(1 << 1)

typedef struct lws_cose_sign_create_info {
	struct lws_context *cx;
	lws_dll2_owner_t *keyset;
	lws_lec_pctx_t *lec;
	lws_cose_sign_ext_pay_cb_t ext_cb;
	void *ext_opaque;
	size_t ext_len;
	size_t inline_payload_len;
	int flags;
	enum lws_cose_sig_types sigtype;
} lws_cose_sign_create_info_t;

LWS_VISIBLE LWS_EXTERN struct lws_cose_sign_context *
lws_cose_sign_create(const lws_cose_sign_create_info_t *info);

LWS_VISIBLE LWS_EXTERN int
lws_cose_sign_add(struct lws_cose_sign_context *csc, cose_param_t alg,
		  const lws_cose_key_t *ck);

LWS_VISIBLE LWS_EXTERN enum lws_lec_pctx_ret
lws_cose_sign_payload_chunk(struct lws_cose_sign_context *csc,
			    const uint8_t *in, size_t in_len);

LWS_VISIBLE LWS_EXTERN void
lws_cose_sign_destroy(struct lws_cose_sign_context **csc);

//@}
// By GST @Date