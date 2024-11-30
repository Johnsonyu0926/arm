// u-boot-next-dev/drivers/crypto/crypto-uclass.c

#include <crypto.h>

static const u8 null_hash_sha1_value[] = {
	0xda, 0x39, 0xa3, 0xee, 0x5e, 0x6b, 0x4b, 0x0d,
	0x32, 0x55, 0xbf, 0xef, 0x95, 0x60, 0x18, 0x90,
	0xaf, 0xd8, 0x07, 0x09
};

static const u8 null_hash_md5_value[] = {
	0xd4, 0x1d, 0x8c, 0xd9, 0x8f, 0x00, 0xb2, 0x04,
	0xe9, 0x80, 0x09, 0x98, 0xec, 0xf8, 0x42, 0x7e
};

static const u8 null_hash_sha256_value[] = {
	0xe3, 0xb0, 0xc4, 0x42, 0x98, 0xfc, 0x1c, 0x14,
	0x9a, 0xfb, 0xf4, 0xc8, 0x99, 0x6f, 0xb9, 0x24,
	0x27, 0xae, 0x41, 0xe4, 0x64, 0x9b, 0x93, 0x4c,
	0xa4, 0x95, 0x99, 0x1b, 0x78, 0x52, 0xb8, 0x55
};

static const u8 null_hash_sha512_value[] = {
	0xcf, 0x83, 0xe1, 0x35, 0x7e, 0xef, 0xb8, 0xbd,
	0xf1, 0x54, 0x28, 0x50, 0xd6, 0x6d, 0x80, 0x07,
	0xd6, 0x20, 0xe4, 0x05, 0x0b, 0x57, 0x15, 0xdc,
	0x83, 0xf4, 0xa9, 0x21, 0xd3, 0x6c, 0xe9, 0xce,
	0x47, 0xd0, 0xd1, 0x3c, 0x5d, 0x85, 0xf2, 0xb0,
	0xff, 0x83, 0x18, 0xd2, 0x87, 0x7e, 0xec, 0x2f,
	0x63, 0xb9, 0x31, 0xbd, 0x47, 0x41, 0x7a, 0x81,
	0xa5, 0x38, 0x32, 0x7a, 0xf9, 0x27, 0xda, 0x3e
};

const static u8 null_hash_sm3_value[] = {
	0x1a, 0xb2, 0x1d, 0x83, 0x55, 0xcf, 0xa1, 0x7f,
	0x8e, 0x61, 0x19, 0x48, 0x31, 0xe8, 0x1a, 0x8f,
	0x22, 0xbe, 0xc8, 0xc7, 0x28, 0xfe, 0xfb, 0x74,
	0x7e, 0xd0, 0x35, 0xeb, 0x50, 0x82, 0xaa, 0x2b
};

/**
 * @brief Get the number of bits for a given crypto algorithm
 *
 * @param algo Crypto algorithm
 * @return Number of bits, or 0 if unknown algorithm
 */
u32 crypto_algo_nbits(u32 algo)
{
	switch (algo) {
	case CRYPTO_MD5:
	case CRYPTO_HMAC_MD5:
		return 128;
	case CRYPTO_SHA1:
	case CRYPTO_HMAC_SHA1:
		return 160;
	case CRYPTO_SHA256:
	case CRYPTO_HMAC_SHA256:
		return 256;
	case CRYPTO_SHA512:
	case CRYPTO_HMAC_SHA512:
		return 512;
	case CRYPTO_SM3:
	case CRYPTO_HMAC_SM3:
		return 256;
	case CRYPTO_RSA512:
		return 512;
	case CRYPTO_RSA1024:
		return 1024;
	case CRYPTO_RSA2048:
		return 2048;
	case CRYPTO_RSA3072:
		return 3072;
	case CRYPTO_RSA4096:
		return 4096;
	}

	printf("Unknown crypto algorithm: 0x%x\n", algo);

	return 0;
}

/**
 * @brief Get a crypto device with the specified capability
 *
 * @param capability Capability required
 * @return Pointer to the crypto device, or NULL if not found
 */
struct udevice *crypto_get_device(u32 capability)
{
	const struct dm_crypto_ops *ops;
	struct udevice *dev;
	struct uclass *uc;
	int ret;
	u32 cap;

	ret = uclass_get(UCLASS_CRYPTO, &uc);
	if (ret)
		return NULL;

	for (uclass_first_device(UCLASS_CRYPTO, &dev);
	     dev;
	     uclass_next_device(&dev)) {
		ops = device_get_ops(dev);
		if (!ops || !ops->capability)
			continue;

		cap = ops->capability(dev);
		if ((cap & capability) == capability)
			return dev;
	}

	return NULL;
}

/**
 * @brief Initialize SHA context
 *
 * @param dev Pointer to the crypto device
 * @param ctx Pointer to the SHA context
 * @return 0 on success, negative error code on failure
 */
int crypto_sha_init(struct udevice *dev, sha_context *ctx)
{
	const struct dm_crypto_ops *ops = device_get_ops(dev);

	if (ctx && !ctx->length)
		return 0;

	if (!ops || !ops->sha_init)
		return -ENOSYS;

	return ops->sha_init(dev, ctx);
}

/**
 * @brief Update SHA context with input data
 *
 * @param dev Pointer to the crypto device
 * @param input Pointer to the input data
 * @param len Length of the input data
 * @return 0 on success, negative error code on failure
 */
int crypto_sha_update(struct udevice *dev, u32 *input, u32 len)
{
	const struct dm_crypto_ops *ops = device_get_ops(dev);

	if (!len)
		return 0;

	if (!ops || !ops->sha_update)
		return -ENOSYS;

	return ops->sha_update(dev, input, len);
}

/**
 * @brief Finalize SHA context and get the output hash
 *
 * @param dev Pointer to the crypto device
 * @param ctx Pointer to the SHA context
 * @param output Pointer to store the output hash
 * @return 0 on success, negative error code on failure
 */
int crypto_sha_final(struct udevice *dev, sha_context *ctx, u8 *output)
{
	const struct dm_crypto_ops *ops = device_get_ops(dev);
	const u8 *null_hash = NULL;
	u32 hash_size = 0;

	if (ctx && !ctx->length && output) {
		switch (ctx->algo) {
		case CRYPTO_MD5:
			null_hash = null_hash_md5_value;
			hash_size = sizeof(null_hash_md5_value);
			break;
		case CRYPTO_SHA1:
			null_hash = null_hash_sha1_value;
			hash_size = sizeof(null_hash_sha1_value);
			break;
		case CRYPTO_SHA256:
			null_hash = null_hash_sha256_value;
			hash_size = sizeof(null_hash_sha256_value);
			break;
		case CRYPTO_SHA512:
			null_hash = null_hash_sha512_value;
			hash_size = sizeof(null_hash_sha512_value);
			break;
		case CRYPTO_SM3:
			null_hash = null_hash_sm3_value;
			hash_size = sizeof(null_hash_sm3_value);
			break;
		default:
			return -EINVAL;
		}

		memcpy(output, null_hash, hash_size);

		return 0;
	}

	if (!ops || !ops->sha_final)
		return -ENOSYS;

	return ops->sha_final(dev, ctx, output);
}

/**
 * @brief Initialize HMAC context
 *
 * @param dev Pointer to the crypto device
 * @param ctx Pointer to the HMAC context
 * @param key Pointer to the key
 * @param key_len Length of the key
 * @return 0 on success, negative error code on failure
 */
int crypto_hmac_init(struct udevice *dev, sha_context *ctx,
		     u8 *key, u32 key_len)
{
	const struct dm_crypto_ops *ops = device_get_ops(dev);

	if (ctx && !ctx->length)
		return -EINVAL;

	if (!ops || !ops->hmac_init)
		return -ENOSYS;

	return ops->hmac_init(dev, ctx, key, key_len);
}

/**
 * @brief Update HMAC context with input data
 *
 * @param dev Pointer to the crypto device
 * @param input Pointer to the input data
 * @param len Length of the input data
 * @return 0 on success, negative error code on failure
 */
int crypto_hmac_update(struct udevice *dev, u32 *input, u32 len)
{
	const struct dm_crypto_ops *ops = device_get_ops(dev);

	if (!len)
		return 0;

	if (!ops || !ops->hmac_update)
		return -ENOSYS;

	return ops->hmac_update(dev, input, len);
}

/**
 * @brief Finalize HMAC context and get the output hash
 *
 * @param dev Pointer to the crypto device
 * @param ctx Pointer to the HMAC context
 * @param output Pointer to store the output hash
 * @return 0 on success, negative error code on failure
 */
int crypto_hmac_final(struct udevice *dev, sha_context *ctx, u8 *output)
{
	const struct dm_crypto_ops *ops = device_get_ops(dev);

	if (!ops || !ops->hmac_final)
		return -ENOSYS;

	return ops->hmac_final(dev, ctx, output);
}

/**
 * @brief Compute SHA checksum
 *
 * @param dev Pointer to the crypto device
 * @param ctx Pointer to the SHA context
 * @param input Pointer to the input data
 * @param input_len Length of the input data
 * @param output Pointer to store the output hash
 * @return 0 on success, negative error code on failure
 */
int crypto_sha_csum(struct udevice *dev, sha_context *ctx,
		    char *input, u32 input_len, u8 *output)
{
	int ret;

	ret = crypto_sha_init(dev, ctx);
	if (ret)
		return ret;

	ret = crypto_sha_update(dev, (u32 *)input, input_len);
	if (ret)
		return ret;

	ret = crypto_sha_final(dev, ctx, output);

	return ret;
}

/**
 * @brief Compute SHA checksum for multiple regions
 *
 * @param dev Pointer to the crypto device
 * @param ctx Pointer to the SHA context
 * @param region Array of image regions
 * @param region_count Number of regions
 * @param output Pointer to store the output hash
 * @return 0 on success, negative error code on failure
 */
int crypto_sha_regions_csum(struct udevice *dev, sha_context *ctx,
			    const struct image_region region[],
			    int region_count, u8 *output)
{
	int i, ret;

	ctx->length = 0;
	for (i = 0; i < region_count; i++)
		ctx->length += region[i].size;

	ret = crypto_sha_init(dev, ctx);
	if (ret)
		return ret;

	for (i = 0; i < region_count; i++) {
		ret = crypto_sha_update(dev, (void *)region[i].data,
					region[i].size);
		if (ret)
			return ret;
	}

	return crypto_sha_final(dev, ctx, output);
}

/**
 * @brief Verify RSA signature
 *
 * @param dev Pointer to the crypto device
 * @param ctx Pointer to the RSA key context
 * @param sign Pointer to the signature
 * @param output Pointer to store the output hash
 * @return 0 on success, negative error code on failure
 */
int crypto_rsa_verify(struct udevice *dev, rsa_key *ctx, u8 *sign, u8 *output)
{
	const struct dm_crypto_ops *ops = device_get_ops(dev);

	if (!ops || !ops->rsa_verify)
		return -ENOSYS;

	if (!ctx || !ctx->n || !ctx->e || !sign || !output)
		return -EINVAL;

	return ops->rsa_verify(dev, ctx, sign, output);
}

/**
 * @brief Perform cipher operation
 *
 * @param dev Pointer to the crypto device
 * @param ctx Pointer to the cipher context
 * @param in Pointer to the input data
 * @param out Pointer to store the output data
 * @param len Length of the input data
 * @param enc Flag indicating if encryption (true) or decryption (false)
 * @return 0 on success, negative error code on failure
 */
int crypto_cipher(struct udevice *dev, cipher_context *ctx,
		  const u8 *in, u8 *out, u32 len, bool enc)
{
	const struct dm_crypto_ops *ops = device_get_ops(dev);

	if (!ops || !ops->cipher_crypt)
		return -ENOSYS;

	return ops->cipher_crypt(dev, ctx, in, out, len, enc);
}

/**
 * @brief Compute MAC
 *
 * @param dev Pointer to the crypto device
 * @param ctx Pointer to the cipher context
 * @param in Pointer to the input data
 * @param len Length of the input data
 * @param tag Pointer to store the MAC tag
 * @return 0 on success, negative error code on failure
 */
int crypto_mac(struct udevice *dev, cipher_context *ctx,
	       const u8 *in, u32 len, u8 *tag)
{
	const struct dm_crypto_ops *ops = device_get_ops(dev);

	if (!ops || !ops->cipher_mac)
		return -ENOSYS;

	return ops->cipher_mac(dev, ctx, in, len, tag);
}

/**
 * @brief Perform authenticated encryption
 *
 * @param dev Pointer to the crypto device
 * @param ctx Pointer to the cipher context
 * @param in Pointer to the input data
 * @param len Length of the input data
 * @param aad Pointer to the additional authenticated data
 * @param aad_len Length of the additional authenticated data
 * @param out Pointer to store the output data
 * @param tag Pointer to store the authentication tag
 * @return 0 on success, negative error code on failure
 */
int crypto_ae(struct udevice *dev, cipher_context *ctx,
	      const u8 *in, u32 len, const u8 *aad, u32 aad_len,
	      u8 *out, u8 *tag)
{
	const struct dm_crypto_ops *ops = device_get_ops(dev);

	if (!ops || !ops->cipher_ae)
		return -ENOSYS;

	return ops->cipher_ae(dev, ctx, in, len, aad, aad_len, out, tag);
}

UCLASS_DRIVER(crypto) = {
	.id	= UCLASS_CRYPTO,
	.name	= "crypto",
};
//GST