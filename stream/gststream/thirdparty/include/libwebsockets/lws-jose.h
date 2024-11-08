enum lws_jws_jose_hdr_indexes {
	LJJHI_ALG,	/* REQUIRED */
	LJJHI_JKU,	/* Optional: string */
	LJJHI_JWK,	/* Optional: jwk JSON object: public key: */
	LJJHI_KID,	/* Optional: string */
	LJJHI_X5U,	/* Optional: string: url of public key cert / chain */
	LJJHI_X5C,	/* Optional: base64 (NOT -url): actual cert */
	LJJHI_X5T,	/* Optional: base64url: SHA-1 of actual cert */
	LJJHI_X5T_S256, /* Optional: base64url: SHA-256 of actual cert */
	LJJHI_TYP,	/* Optional: string: media type */
	LJJHI_CTY,	/* Optional: string: content media type */
	LJJHI_CRIT,	/* Optional for send, REQUIRED: array of strings:
			 * mustn't contain standardized strings or null set */

	LJJHI_RECIPS_HDR,
	LJJHI_RECIPS_HDR_ALG,
	LJJHI_RECIPS_HDR_KID,
	LJJHI_RECIPS_EKEY,

	LJJHI_ENC,	/* JWE only: Optional: string */
	LJJHI_ZIP,	/* JWE only: Optional: string ("DEF" = deflate) */

	LJJHI_EPK,	/* Additional arg for JWE ECDH:  ephemeral public key */
	LJJHI_APU,	/* Additional arg for JWE ECDH:  base64url */
	LJJHI_APV,	/* Additional arg for JWE ECDH:  base64url */
	LJJHI_IV,	/* Additional arg for JWE AES:   base64url */
	LJJHI_TAG,	/* Additional arg for JWE AES:   base64url */
	LJJHI_P2S,	/* Additional arg for JWE PBES2: base64url: salt */
	LJJHI_P2C,	/* Additional arg for JWE PBES2: integer: count */

	LWS_COUNT_JOSE_HDR_ELEMENTS
};

enum lws_jose_algtype {
	LWS_JOSE_ENCTYPE_NONE,

	LWS_JOSE_ENCTYPE_RSASSA_PKCS1_1_5,
	LWS_JOSE_ENCTYPE_RSASSA_PKCS1_OAEP,
	LWS_JOSE_ENCTYPE_RSASSA_PKCS1_PSS,

	LWS_JOSE_ENCTYPE_ECDSA,
	LWS_JOSE_ENCTYPE_ECDHES,

	LWS_JOSE_ENCTYPE_AES_CBC,
	LWS_JOSE_ENCTYPE_AES_CFB128,
	LWS_JOSE_ENCTYPE_AES_CFB8,
	LWS_JOSE_ENCTYPE_AES_CTR,
	LWS_JOSE_ENCTYPE_AES_ECB,
	LWS_JOSE_ENCTYPE_AES_OFB,
	LWS_JOSE_ENCTYPE_AES_XTS,	/* care: requires double-length key */
	LWS_JOSE_ENCTYPE_AES_GCM,
};

/* there's a table of these defined in lws-gencrypto-common.c */

struct lws_jose_jwe_alg {
	enum lws_genhash_types hash_type;
	enum lws_genhmac_types hmac_type;
	enum lws_jose_algtype algtype_signing; /* the signing cipher */
	enum lws_jose_algtype algtype_crypto; /* the encryption cipher */
	const char *alg; /* the JWA enc alg name, eg "ES512" */
	const char *curve_name; /* NULL, or, eg, "P-256" */
	unsigned short keybits_min, keybits_fixed;
	unsigned short ivbits;
};

#define LWS_JWS_MAX_RECIPIENTS 3

struct lws_jws_recpient {
	/*
	 * JOSE per-recipient unprotected header... for JWS this contains
	 * protected / header / signature
	 */
	struct lws_gencrypto_keyelem unprot[LWS_COUNT_JOSE_HDR_ELEMENTS];
	struct lws_jwk jwk_ephemeral;	/* recipient ephemeral key if any */
	struct lws_jwk jwk;		/* recipient "jwk" key if any */
};

struct lws_jose {
	/* JOSE protected and unprotected header elements */
	struct lws_gencrypto_keyelem e[LWS_COUNT_JOSE_HDR_ELEMENTS];

	struct lws_jws_recpient recipient[LWS_JWS_MAX_RECIPIENTS];

	char typ[32];

	/* information from the protected header part */
	const struct lws_jose_jwe_alg *alg;
	const struct lws_jose_jwe_alg *enc_alg;

	int recipients; /* count of used recipient[] entries */
};

LWS_VISIBLE LWS_EXTERN void
lws_jose_init(struct lws_jose *jose);

LWS_VISIBLE LWS_EXTERN void
lws_jose_destroy(struct lws_jose *jose);

LWS_VISIBLE LWS_EXTERN int
lws_gencrypto_jws_alg_to_definition(const char *alg,
				    const struct lws_jose_jwe_alg **jose);

LWS_VISIBLE LWS_EXTERN int
lws_gencrypto_jwe_alg_to_definition(const char *alg,
				    const struct lws_jose_jwe_alg **jose);

LWS_VISIBLE LWS_EXTERN int
lws_gencrypto_jwe_enc_to_definition(const char *enc,
				    const struct lws_jose_jwe_alg **jose);

LWS_VISIBLE LWS_EXTERN int
lws_jws_parse_jose(struct lws_jose *jose,
		   const char *buf, int len, char *temp, int *temp_len);

LWS_VISIBLE LWS_EXTERN int
lws_jwe_parse_jose(struct lws_jose *jose,
		   const char *buf, int len, char *temp, int *temp_len);
// By GST @Date