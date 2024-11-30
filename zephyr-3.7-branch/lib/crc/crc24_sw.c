//lib/crc/crc24_sw.c
#include <zephyr/sys/crc.h>

#define CRC24_PGP_POLY 0x01864cfbU

uint32_t crc24_pgp(const uint8_t *data, size_t len)
{
	return crc24_pgp_update(CRC24_PGP_INITIAL_VALUE, data, len) & CRC24_FINAL_VALUE_MASK;
}

/* CRC-24 implementation from the section 6.1 of the RFC 4880 */
uint32_t crc24_pgp_update(uint32_t crc, const uint8_t *data, size_t len)
{
	while (len--) {
		crc ^= (*data++) << 16;
		for (int i = 0; i < 8; i++) {
			crc <<= 1;
			if (crc & 0x01000000) {
				crc ^= CRC24_PGP_POLY;
			}
		}
	}

	return crc;
}
//GST
