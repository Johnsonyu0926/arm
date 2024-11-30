//lib/hash/hash_func32_djb2.c
#include <stddef.h>
#include <stdint.h>

#include <zephyr/sys/hash_function.h>

uint32_t sys_hash32_djb2(const void *str, size_t n)
{
	uint32_t hash;
	const uint8_t *d;

	/* The number 5381 is the initializer for the djb2 hash */
	for (hash = 5381, d = str; n > 0; --n, ++d) {
		/* The djb2 hash multiplier is 33 (i.e. 2^5 + 1) */
		hash = (hash << 5) + hash;
		hash ^= *d;
	}

	return hash;
}
//GST
