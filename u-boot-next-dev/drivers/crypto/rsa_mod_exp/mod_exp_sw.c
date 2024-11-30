// u-boot-next-dev/drivers/crypto/rsa_mod_exp/mod_exp_sw.c

#include <config.h>
#include <common.h>
#include <dm.h>
#include <u-boot/rsa-mod-exp.h>

/**
 * @brief Perform software RSA modular exponentiation
 *
 * @param dev Pointer to the device
 * @param sig Pointer to the signature
 * @param sig_len Length of the signature
 * @param prop Pointer to the key properties
 * @param out Pointer to store the output
 * @return 0 on success, negative error code on failure
 */
int mod_exp_sw(struct udevice *dev, const uint8_t *sig, uint32_t sig_len,
		struct key_prop *prop, uint8_t *out)
{
	int ret = 0;

	ret = rsa_mod_exp_sw(sig, sig_len, prop, out);
	if (ret) {
		debug("%s: RSA failed to verify: %d\n", __func__, ret);
		return ret;
	}

	return 0;
}

static const struct mod_exp_ops mod_exp_ops_sw = {
	.mod_exp	= mod_exp_sw,
};

U_BOOT_DRIVER(mod_exp_sw) = {
	.name	= "mod_exp_sw",
	.id	= UCLASS_MOD_EXP,
	.ops	= &mod_exp_ops_sw,
	.flags	= DM_FLAG_PRE_RELOC,
};

U_BOOT_DEVICE(mod_exp_sw) = {
	.name = "mod_exp_sw",
};
//GST