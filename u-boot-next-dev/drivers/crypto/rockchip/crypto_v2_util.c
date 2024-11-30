// u-boot-next-dev/drivers/crypto/rockchip/crypto_v2_util.c

#include <common.h>
#include <rockchip/crypto_v2_util.h>

/**
 * @brief This function executes a reversed words copy on a specified buffer.
 *
 *        on a 6 words buffer:
 *
 *        buff[5] <---> buff[0]
 *        buff[4] <---> buff[1]
 *        buff[3] <---> buff[2]
 *
 * @param[in] buff_ptr - The buffer pointer.
 * @param[in] size_words - The buffer size in words.
 */
void util_reverse_words_buff(u32 *buff_ptr, u32 size_words)
{
	u32 i;
	u32 temp;
	u32 *high_swap_ptr, *low_swap_ptr;

	/* initialize the source and the destination position */
	high_swap_ptr = buff_ptr + size_words - 1;
	low_swap_ptr  = buff_ptr;

	/* execute the reverse memcpy */
	for (i = 0; i < (size_words / 2); i++) {
		temp = *high_swap_ptr;
		*(high_swap_ptr--) = *low_swap_ptr;
		*(low_swap_ptr++) = temp;
	}
}

/**
 * @brief This function executes a reversed byte copy on a specified buffer.
 *
 *        on a 6 byte buffer:
 *
 *        buff[5] <---> buff[0]
 *        buff[4] <---> buff[1]
 *        buff[3] <---> buff[2]
 *
 * @param[in] buff_ptr - The buffer pointer.
 * @param[in] size - The buffer size in bytes.
 */
void util_reverse_buff(u8 *buff_ptr, u32 size)
{
	u32 i;
	u32 temp;
	u8 *high_swap_ptr, *low_swap_ptr;

	/* initialize the source and the destination position */
	high_swap_ptr = buff_ptr + size - 1;
	low_swap_ptr  = buff_ptr;

	/* execute the reverse memcpy */
	for (i = 0; i < (size / 2); i++) {
		temp = *high_swap_ptr;
		*(high_swap_ptr--) = *low_swap_ptr;
		*(low_swap_ptr++) = temp;
	}
}

/**
 * @brief This function executes a reverse bytes copying from one buffer to
 * another buffer.
 *
 * @param[in] dst_ptr - The pointer to destination buffer.
 * @param[in] src_ptr - The pointer to source buffer.
 * @param[in] size - The size in bytes.
 */
void util_reverse_memcpy(u8 *dst_ptr, const u8 *src_ptr, u32 size)
{
	u32 i;
	u32 buff_dst_pos, buff_src_pos;

	/* execute the reverse copy in case of different buffers */
	/* initialize the source and the destination position */
	buff_dst_pos = size - 1;
	buff_src_pos = 0;

	for (i = 0; i < size; i++)
		dst_ptr[buff_dst_pos--] = src_ptr[buff_src_pos++];
}

/**
 * @brief This function executes a memory copy between 2 buffers.
 *
 * @param[in] dst_ptr - The destination buffer pointer.
 * @param[in] src_ptr - The source buffer pointer.
 * @param[in] size - The buffer size in words.
 */
void util_word_memcpy(u32 *dst_ptr, u32 *src_ptr, u32 size)
{
	u32 i;

	/* execute the memcpy */
	for (i = 0; i < size; i++)
		dst_ptr[i] = src_ptr[i];
}

/**
 * @brief This function executes a reverse bytes copying from one buffer
 * to another buffer.
 *
 * @param[in] dst_ptr - The pointer to destination buffer.
 * @param[in] src_ptr - The pointer to source buffer.
 * @param[in] size - The size in words.
 */
void util_reverse_word_memcpy(u32 *dst_ptr, u32 *src_ptr, u32 size)
{
	u32 i;
	u32 buff_dst_pos, buff_src_pos;

	/* execute the reverse copy in case of different buffers */
	/* initialize the source and the destination position */
	buff_dst_pos = size - 1;
	buff_src_pos = 0;

	for (i = 0; i < size; i++)
		dst_ptr[buff_dst_pos--] = src_ptr[buff_src_pos++];
}

/**
 * @brief This function executes a memory set operation on a buffer.
 *
 * @param[in] buff_ptr - The buffer pointer.
 * @param[in] val - The value to set the buffer.
 * @param[in] size - The buffer size in words.
 */
void util_word_memset(u32 *buff_ptr, u32 val, u32 size)
{
	u32 i;

	/* execute the memset */
	for (i = 0; i < size; i++)
		buff_ptr[i] = val;
}
//GST