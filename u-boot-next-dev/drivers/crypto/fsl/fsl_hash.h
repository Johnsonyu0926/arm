// jobdesc.h

#ifndef __JOBDESC_H
#define __JOBDESC_H

#include <common.h>
#include <asm/io.h>
#include "rsa_caam.h"

#define KEY_IDNFR_SZ_BYTES		16

#ifdef CONFIG_CMD_DEKBLOB
/**
 * @brief Initialize and construct the job descriptor for DEK encapsulation
 *
 * @param desc Reference to the job descriptor
 * @param plain_txt Reference to the DEK
 * @param enc_blob Reference where to store the blob
 * @param in_sz Size in bytes of the DEK
 * @return 0 on success, ECONSTRJDESC otherwise
 */
int inline_cnstr_jobdesc_blob_dek(uint32_t *desc, const uint8_t *plain_txt,
				uint8_t *enc_blob, uint32_t in_sz);
#endif

/**
 * @brief Initialize and construct the job descriptor for hashing
 *
 * @param desc Reference to the job descriptor
 * @param msg Reference to the message
 * @param msgsz Size of the message
 * @param digest Reference to store the digest
 * @param alg_type Algorithm type
 * @param alg_size Algorithm size
 * @param sg_tbl Scatter-gather table flag
 */
void inline_cnstr_jobdesc_hash(uint32_t *desc,
			  const uint8_t *msg, uint32_t msgsz, uint8_t *digest,
			  u32 alg_type, uint32_t alg_size, int sg_tbl);

/**
 * @brief Initialize and construct the job descriptor for blob encapsulation
 *
 * @param desc Reference to the job descriptor
 * @param key_idnfr Reference to the key identifier
 * @param plain_txt Reference to the plain text
 * @param enc_blob Reference where to store the blob
 * @param in_sz Size in bytes of the plain text
 */
void inline_cnstr_jobdesc_blob_encap(uint32_t *desc, uint8_t *key_idnfr,
				     uint8_t *plain_txt, uint8_t *enc_blob,
				     uint32_t in_sz);

/**
 * @brief Initialize and construct the job descriptor for blob decapsulation
 *
 * @param desc Reference to the job descriptor
 * @param key_idnfr Reference to the key identifier
 * @param enc_blob Reference to the encapsulated blob
 * @param plain_txt Reference where to store the plain text
 * @param out_sz Size in bytes of the plain text
 */
void inline_cnstr_jobdesc_blob_decap(uint32_t *desc, uint8_t *key_idnfr,
				     uint8_t *enc_blob, uint8_t *plain_txt,
				     uint32_t out_sz);

/**
 * @brief Initialize and construct the job descriptor for RNG instantiation
 *
 * @param desc Reference to the job descriptor
 */
void inline_cnstr_jobdesc_rng_instantiation(uint32_t *desc);

/**
 * @brief Initialize and construct the job descriptor for PKHA RSA exponentiation
 *
 * @param desc Reference to the job descriptor
 * @param pkin Reference to the PKHA input parameters
 * @param out Reference where to store the output
 * @param out_siz Size in bytes of the output
 */
void inline_cnstr_jobdesc_pkha_rsaexp(uint32_t *desc,
				      struct pk_in_params *pkin, uint8_t *out,
				      uint32_t out_siz);

#endif /* __JOBDESC_H */
//GST