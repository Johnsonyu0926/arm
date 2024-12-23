// fsl_blob.c

#include <common.h>
#include <malloc.h>
#include <fsl_sec.h>
#include <linux/errno.h>
#include "jobdesc.h"
#include "desc.h"
#include "jr.h"

/**
 * @brief Decapsulate a blob to retrieve data
 *
 * @param key_mod Key modifier
 * @param src Source data
 * @param dst Destination buffer
 * @param len Length of the data
 * @return 0 on success, or an error code on failure
 */
int blob_decap(u8 *key_mod, u8 *src, u8 *dst, u32 len)
{
	int ret, i = 0;
	u32 *desc;

	printf("\nDecapsulating blob to get data\n");
	desc = malloc(sizeof(int) * MAX_CAAM_DESCSIZE);
	if (!desc) {
		debug("Not enough memory for descriptor allocation\n");
		return -1;
	}

	inline_cnstr_jobdesc_blob_decap(desc, key_mod, src, dst, len);

	debug("Descriptor dump:\n");
	for (i = 0; i < 14; i++)
		debug("Word[%d]: %08x\n", i, *(desc + i));
	ret = run_descriptor_jr(desc);

	if (ret)
		printf("Error in Decapsulation %d\n", ret);
	else
		printf("Decapsulation Success\n");

	free(desc);
	return ret;
}

/**
 * @brief Encapsulate data to form a blob
 *
 * @param key_mod Key modifier
 * @param src Source data
 * @param dst Destination buffer
 * @param len Length of the data
 * @return 0 on success, or an error code on failure
 */
int blob_encap(u8 *key_mod, u8 *src, u8 *dst, u32 len)
{
	int ret, i = 0;
	u32 *desc;

	printf("\nEncapsulating data to form blob\n");
	desc = malloc(sizeof(int) * MAX_CAAM_DESCSIZE);
	if (!desc) {
		debug("Not enough memory for descriptor allocation\n");
		return -1;
	}

	inline_cnstr_jobdesc_blob_encap(desc, key_mod, src, dst, len);

	debug("Descriptor dump:\n");
	for (i = 0; i < 14; i++)
		debug("Word[%d]: %08x\n", i, *(desc + i));
	ret = run_descriptor_jr(desc);

	if (ret)
		printf("Error in Encapsulation %d\n", ret);
	else
		printf("Encapsulation Success\n");

	free(desc);
	return ret;
}

#ifdef CONFIG_CMD_DEKBLOB
/**
 * @brief Encapsulate a DEK to form a blob
 *
 * @param src Source data
 * @param dst Destination buffer
 * @param len Length of the data
 * @return 0 on success, or an error code on failure
 */
int blob_dek(const u8 *src, u8 *dst, u8 len)
{
	int ret, size, i = 0;
	u32 *desc;

	int out_sz =  WRP_HDR_SIZE + len + KEY_BLOB_SIZE + MAC_SIZE;

	puts("\nEncapsulating provided DEK to form blob\n");
	desc = memalign(ARCH_DMA_MINALIGN,
			sizeof(uint32_t) * DEK_BLOB_DESCSIZE);
	if (!desc) {
		debug("Not enough memory for descriptor allocation\n");
		return -ENOMEM;
	}

	ret = inline_cnstr_jobdesc_blob_dek(desc, src, dst, len);
	if (ret) {
		debug("Error in Job Descriptor Construction:  %d\n", ret);
	} else {
		size = roundup(sizeof(uint32_t) * DEK_BLOB_DESCSIZE,
			      ARCH_DMA_MINALIGN);
		flush_dcache_range((unsigned long)desc,
				   (unsigned long)desc + size);
		size = roundup(sizeof(uint8_t) * out_sz, ARCH_DMA_MINALIGN);
		flush_dcache_range((unsigned long)dst,
				   (unsigned long)dst + size);

		ret = run_descriptor_jr(desc);
	}

	if (ret) {
		debug("Error in Encapsulation %d\n", ret);
	   goto err;
	}

	size = roundup(out_sz, ARCH_DMA_MINALIGN);
	invalidate_dcache_range((unsigned long)dst, (unsigned long)dst+size);

	puts("DEK Blob\n");
	for (i = 0; i < out_sz; i++)
		printf("%02X", ((uint8_t *)dst)[i]);
	printf("\n");

err:
	free(desc);
	return ret;
}
#endif
//GST