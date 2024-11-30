// rsa_caam.h

#ifndef __RSA_CAAM_H
#define __RSA_CAAM_H

#include <common.h>

/**
 * @brief Structure to hold input parameters for PKHA block in CAAM
 *
 * These parameters are required to perform Modular Exponentiation
 * using PKHA Block in CAAM.
 */
struct pk_in_params {
	const uint8_t *e;	/* public exponent as byte array */
	uint32_t e_siz;		/* size of e[] in number of bytes */
	const uint8_t *n;	/* modulus as byte array */
	uint32_t n_siz;		/* size of n[] in number of bytes */
	const uint8_t *a;	/* Signature as byte array */
	uint32_t a_siz;		/* size of a[] in number of bytes */
	uint8_t *b;		/* Result exp. modulus in number of bytes */
	uint32_t b_siz;		/* size of b[] in number of bytes */
};

#endif /* __RSA_CAAM_H */
//GST