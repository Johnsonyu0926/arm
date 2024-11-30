// image-sparse.c

#include <config.h>
#include <common.h>
#include <image-sparse.h>
#include <div64.h>
#include <malloc.h>
#include <part.h>
#include <sparse_format.h>
#include <fastboot.h>
#include <linux/math64.h>

#ifndef CONFIG_FASTBOOT_FLASH_FILLBUF_SIZE
#define CONFIG_FASTBOOT_FLASH_FILLBUF_SIZE (1024 * 512)
#endif

void write_sparse_image(
		struct sparse_storage *info, const char *part_name,
		void *data, unsigned sz, char *response)
{
	lbaint_t blk = info->start;
	lbaint_t blkcnt;
	uint64_t bytes_written = 0;
	unsigned int chunk;
	uint64_t chunk_data_sz;
	uint32_t *fill_buf = NULL;
	uint32_t fill_val;
	sparse_header_t *sparse_header = (sparse_header_t *)data;
	chunk_header_t *chunk_header;
	uint32_t total_blocks = 0;
	int fill_buf_num_blks = CONFIG_FASTBOOT_FLASH_FILLBUF_SIZE / info->blksz;
	int i, j;

	data += sparse_header->file_hdr_sz;
	if (sparse_header->file_hdr_sz > sizeof(sparse_header_t)) {
		data += (sparse_header->file_hdr_sz - sizeof(sparse_header_t));
	}

	debug("=== Sparse Image Header ===\n");
	debug("magic: 0x%x\n", sparse_header->magic);
	debug("major_version: 0x%x\n", sparse_header->major_version);
	debug("minor_version: 0x%x\n", sparse_header->minor_version);
	debug("file_hdr_sz: %d\n", sparse_header->file_hdr_sz);
	debug("chunk_hdr_sz: %d\n", sparse_header->chunk_hdr_sz);
	debug("blk_sz: %d\n", sparse_header->blk_sz);
	debug("total_blks: %d\n", sparse_header->total_blks);
	debug("total_chunks: %d\n", sparse_header->total_chunks);

	if (sparse_header->blk_sz % info->blksz) {
		printf("%s: Sparse image block size issue [%u]\n", __func__, sparse_header->blk_sz);
		fastboot_fail("sparse image block size issue", response);
		return;
	}

	puts("Flashing Sparse Image\n");

	for (chunk = 0; chunk < sparse_header->total_chunks; chunk++) {
		chunk_header = (chunk_header_t *)data;
		data += sizeof(chunk_header_t);

		if (chunk_header->chunk_type != CHUNK_TYPE_RAW) {
			debug("=== Chunk Header ===\n");
			debug("chunk_type: 0x%x\n", chunk_header->chunk_type);
			debug("chunk_data_sz: 0x%x\n", chunk_header->chunk_sz);
			debug("total_size: 0x%x\n", chunk_header->total_sz);
		}

		if (sparse_header->chunk_hdr_sz > sizeof(chunk_header_t)) {
			data += (sparse_header->chunk_hdr_sz - sizeof(chunk_header_t));
		}

		chunk_data_sz = ((u64)sparse_header->blk_sz) * chunk_header->chunk_sz;
		blkcnt = DIV_ROUND_UP_ULL(chunk_data_sz, info->blksz);

		switch (chunk_header->chunk_type) {
		case CHUNK_TYPE_RAW:
			if (chunk_header->total_sz != (sparse_header->chunk_hdr_sz + chunk_data_sz)) {
				fastboot_fail("Bogus chunk size for chunk type Raw", response);
				return;
			}

			if (blk + blkcnt > info->start + info->size) {
				printf("%s: Request would exceed partition size!\n", __func__);
				fastboot_fail("Request would exceed partition size!", response);
				return;
			}

			if (info->write(info, blk, blkcnt, data) < blkcnt) {
				printf("%s: Write failed, block #" LBAFU "\n", __func__, blk);
				fastboot_fail("flash write failure", response);
				return;
			}

			blk += blkcnt;
			bytes_written += ((u64)blkcnt) * info->blksz;
			total_blocks += chunk_header->chunk_sz;
			data += chunk_data_sz;
			break;

		case CHUNK_TYPE_FILL:
			if (chunk_header->total_sz != (sparse_header->chunk_hdr_sz + sizeof(uint32_t))) {
				fastboot_fail("Bogus chunk size for chunk type FILL", response);
				return;
			}

			fill_buf = (uint32_t *)memalign(ARCH_DMA_MINALIGN, ROUNDUP(info->blksz * fill_buf_num_blks, ARCH_DMA_MINALIGN));
			if (!fill_buf) {
				fastboot_fail("Malloc failed for: CHUNK_TYPE_FILL", response);
				return;
			}

			fill_val = *(uint32_t *)data;
			data = (char *)data + sizeof(uint32_t);

			for (i = 0; i < (info->blksz * fill_buf_num_blks / sizeof(fill_val)); i++)
				fill_buf[i] = fill_val;

			if (blk + blkcnt > info->start + info->size) {
				printf("%s: Request would exceed partition size!\n", __func__);
				fastboot_fail("Request would exceed partition size!", response);
				free(fill_buf);
				return;
			}

			for (i = 0; i < blkcnt;) {
				j = blkcnt - i;
				if (j > fill_buf_num_blks)
					j = fill_buf_num_blks;
				if (info->write(info, blk, j, fill_buf) < j) {
					printf("%s: Write failed, block #" LBAFU "\n", __func__, blk);
					fastboot_fail("flash write failure", response);
					free(fill_buf);
					return;
				}
				blk += j;
				i += j;
			}

			bytes_written += ((u64)blkcnt) * info->blksz;
			total_blocks += DIV_ROUND_UP_ULL(chunk_data_sz, sparse_header->blk_sz);
			free(fill_buf);
			break;

		case CHUNK_TYPE_DONT_CARE:
			blk += info->reserve(info, blk, blkcnt);
			total_blocks += chunk_header->chunk_sz;
			break;

		case CHUNK_TYPE_CRC32:
			if (chunk_header->total_sz != sparse_header->chunk_hdr_sz) {
				fastboot_fail("Bogus chunk size for chunk type CRC32", response);
				return;
			}
			total_blocks += chunk_header->chunk_sz;
			data += chunk_data_sz;
			break;

		default:
			printf("%s: Unknown chunk type: %x\n", __func__, chunk_header->chunk_type);
			fastboot_fail("Unknown chunk type", response);
			return;
		}
	}

	debug("Wrote %d blocks, expected to write %d blocks\n", total_blocks, sparse_header->total_blks);
	printf("........ wrote %llu bytes to '%s'\n", bytes_written, part_name);

	if (total_blocks != sparse_header->total_blks)
		fastboot_fail("sparse image write failure", response);
	else
		fastboot_okay("", response);
}
	debug("Wrote %d blocks, expected to write %d blocks\n", total_blocks, sparse_header->total_blks);
	printf("........ wrote %llu bytes to '%s'\n", bytes_written, part_name);

	if (total_blocks != sparse_header->total_blks)
		fastboot_fail("sparse image write failure", response);
	else
		fastboot_okay("", response);
}

int read_sparse_image(
		struct sparse_storage *info, const char *part_name,
		void *data, unsigned sz, char *response)
{
	lbaint_t blk = info->start;
	lbaint_t blkcnt;
	uint64_t bytes_read = 0;
	unsigned int chunk;
	uint64_t chunk_data_sz;
	uint32_t *read_buf = NULL;
	sparse_header_t *sparse_header = (sparse_header_t *)data;
	chunk_header_t *chunk_header;
	uint32_t total_blocks = 0;
	int read_buf_num_blks = CONFIG_FASTBOOT_FLASH_FILLBUF_SIZE / info->blksz;
	int i, j;

	data += sparse_header->file_hdr_sz;
	if (sparse_header->file_hdr_sz > sizeof(sparse_header_t)) {
		data += (sparse_header->file_hdr_sz - sizeof(sparse_header_t));
	}

	debug("=== Sparse Image Header ===\n");
	debug("magic: 0x%x\n", sparse_header->magic);
	debug("major_version: 0x%x\n", sparse_header->major_version);
	debug("minor_version: 0x%x\n", sparse_header->minor_version);
	debug("file_hdr_sz: %d\n", sparse_header->file_hdr_sz);
	debug("chunk_hdr_sz: %d\n", sparse_header->chunk_hdr_sz);
	debug("blk_sz: %d\n", sparse_header->blk_sz);
	debug("total_blks: %d\n", sparse_header->total_blks);
	debug("total_chunks: %d\n", sparse_header->total_chunks);

	if (sparse_header->blk_sz % info->blksz) {
		printf("%s: Sparse image block size issue [%u]\n", __func__, sparse_header->blk_sz);
		fastboot_fail("sparse image block size issue", response);
		return -1;
	}

	puts("Reading Sparse Image\n");

	for (chunk = 0; chunk < sparse_header->total_chunks; chunk++) {
		chunk_header = (chunk_header_t *)data;
		data += sizeof(chunk_header_t);

		if (chunk_header->chunk_type != CHUNK_TYPE_RAW) {
			debug("=== Chunk Header ===\n");
			debug("chunk_type: 0x%x\n", chunk_header->chunk_type);
			debug("chunk_data_sz: 0x%x\n", chunk_header->chunk_sz);
			debug("total_size: 0x%x\n", chunk_header->total_sz);
		}

		if (sparse_header->chunk_hdr_sz > sizeof(chunk_header_t)) {
			data += (sparse_header->chunk_hdr_sz - sizeof(chunk_header_t));
		}

		chunk_data_sz = ((u64)sparse_header->blk_sz) * chunk_header->chunk_sz;
		blkcnt = DIV_ROUND_UP_ULL(chunk_data_sz, info->blksz);

		switch (chunk_header->chunk_type) {
		case CHUNK_TYPE_RAW:
			if (chunk_header->total_sz != (sparse_header->chunk_hdr_sz + chunk_data_sz)) {
				fastboot_fail("Bogus chunk size for chunk type Raw", response);
				return -1;
			}

			if (blk + blkcnt > info->start + info->size) {
				printf("%s: Request would exceed partition size!\n", __func__);
				fastboot_fail("Request would exceed partition size!", response);
				return -1;
			}

			if (info->read(info, blk, blkcnt, data) < blkcnt) {
				printf("%s: Read failed, block #" LBAFU "\n", __func__, blk);
				fastboot_fail("flash read failure", response);
				return -1;
			}

			blk += blkcnt;
			bytes_read += ((u64)blkcnt) * info->blksz;
			total_blocks += chunk_header->chunk_sz;
			data += chunk_data_sz;
			break;

		case CHUNK_TYPE_FILL:
			if (chunk_header->total_sz != (sparse_header->chunk_hdr_sz + sizeof(uint32_t))) {
				fastboot_fail("Bogus chunk size for chunk type FILL", response);
				return -1;
			}

			read_buf = (uint32_t *)memalign(ARCH_DMA_MINALIGN, ROUNDUP(info->blksz * read_buf_num_blks, ARCH_DMA_MINALIGN));
			if (!read_buf) {
				fastboot_fail("Malloc failed for: CHUNK_TYPE_FILL", response);
				return -1;
			}

			if (blk + blkcnt > info->start + info->size) {
				printf("%s: Request would exceed partition size!\n", __func__);
				fastboot_fail("Request would exceed partition size!", response);
				free(read_buf);
				return -1;
			}

			for (i = 0; i < blkcnt;) {
				j = blkcnt - i;
				if (j > read_buf_num_blks)
					j = read_buf_num_blks;
				if (info->read(info, blk, j, read_buf) < j) {
					printf("%s: Read failed, block #" LBAFU "\n", __func__, blk);
					fastboot_fail("flash read failure", response);
					free(read_buf);
					return -1;
				}
				blk += j;
				i += j;
			}

			bytes_read += ((u64)blkcnt) * info->blksz;
			total_blocks += DIV_ROUND_UP_ULL(chunk_data_sz, sparse_header->blk_sz);
			free(read_buf);
			break;

		case CHUNK_TYPE_DONT_CARE:
			blk += blkcnt;
			total_blocks += chunk_header->chunk_sz;
			break;

		case CHUNK_TYPE_CRC32:
			if (chunk_header->total_sz != sparse_header->chunk_hdr_sz) {
				fastboot_fail("Bogus chunk size for chunk type CRC32", response);
				return -1;
			}
			total_blocks += chunk_header->chunk_sz;
			data += chunk_data_sz;
			break;

		default:
			printf("%s: Unknown chunk type: %x\n", __func__, chunk_header->chunk_type);
			fastboot_fail("Unknown chunk type", response);
			return -1;
		}
	}

	debug("Read %d blocks, expected to read %d blocks\n", total_blocks, sparse_header->total_blks);
	printf("........ read %llu bytes from '%s'\n", bytes_read, part_name);

	if (total_blocks != sparse_header->total_blks)
		fastboot_fail("sparse image read failure", response);
	else
		fastboot_okay("", response);

	return 0;
}
//@GST