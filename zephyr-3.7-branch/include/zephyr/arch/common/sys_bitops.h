// zephyr-3.7-branch/include/zephyr/arch/common/sys_bitops.h

/* Memory bits manipulation functions in non-arch-specific C code */

#ifndef ZEPHYR_INCLUDE_ARCH_COMMON_SYS_BITOPS_H_
#define ZEPHYR_INCLUDE_ARCH_COMMON_SYS_BITOPS_H_

#ifndef _ASMLANGUAGE

#include <zephyr/toolchain.h>
#include <zephyr/types.h>
#include <zephyr/sys/sys_io.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Set a bit in a memory address
 *
 * This function sets the specified bit in the memory address.
 *
 * @param addr The memory address
 * @param bit The bit to set
 */
static ALWAYS_INLINE void sys_set_bit(mem_addr_t addr, unsigned int bit)
{
	uint32_t temp = *(volatile uint32_t *)addr;
	*(volatile uint32_t *)addr = temp | (1 << bit);
}

/**
 * @brief Clear a bit in a memory address
 *
 * This function clears the specified bit in the memory address.
 *
 * @param addr The memory address
 * @param bit The bit to clear
 */
static ALWAYS_INLINE void sys_clear_bit(mem_addr_t addr, unsigned int bit)
{
	uint32_t temp = *(volatile uint32_t *)addr;
	*(volatile uint32_t *)addr = temp & ~(1 << bit);
}

/**
 * @brief Test if a bit is set in a memory address
 *
 * This function tests if the specified bit is set in the memory address.
 *
 * @param addr The memory address
 * @param bit The bit to test
 * @return Non-zero if the bit is set, zero otherwise
 */
static ALWAYS_INLINE int sys_test_bit(mem_addr_t addr, unsigned int bit)
{
	uint32_t temp = *(volatile uint32_t *)addr;
	return temp & (1 << bit);
}

/**
 * @brief Set multiple bits in a memory address
 *
 * This function sets the bits specified by the mask in the memory address.
 *
 * @param addr The memory address
 * @param mask The mask specifying the bits to set
 */
static ALWAYS_INLINE void sys_set_bits(mem_addr_t addr, unsigned int mask)
{
	uint32_t temp = *(volatile uint32_t *)addr;
	*(volatile uint32_t *)addr = temp | mask;
}

/**
 * @brief Clear multiple bits in a memory address
 *
 * This function clears the bits specified by the mask in the memory address.
 *
 * @param addr The memory address
 * @param mask The mask specifying the bits to clear
 */
static ALWAYS_INLINE void sys_clear_bits(mem_addr_t addr, unsigned int mask)
{
	uint32_t temp = *(volatile uint32_t *)addr;
	*(volatile uint32_t *)addr = temp & ~mask;
}

/**
 * @brief Set a bit in a bitfield
 *
 * This function sets the specified bit in the bitfield.
 *
 * @param addr The memory address of the bitfield
 * @param bit The bit to set
 */
static ALWAYS_INLINE void sys_bitfield_set_bit(mem_addr_t addr, unsigned int bit)
{
	/* Doing memory offsets in terms of 32-bit values to prevent
	 * alignment issues
	 */
	sys_set_bit(addr + ((bit >> 5) << 2), bit & 0x1F);
}

/**
 * @brief Clear a bit in a bitfield
 *
 * This function clears the specified bit in the bitfield.
 *
 * @param addr The memory address of the bitfield
 * @param bit The bit to clear
 */
static ALWAYS_INLINE void sys_bitfield_clear_bit(mem_addr_t addr, unsigned int bit)
{
	sys_clear_bit(addr + ((bit >> 5) << 2), bit & 0x1F);
}

/**
 * @brief Test if a bit is set in a bitfield
 *
 * This function tests if the specified bit is set in the bitfield.
 *
 * @param addr The memory address of the bitfield
 * @param bit The bit to test
 * @return Non-zero if the bit is set, zero otherwise
 */
static ALWAYS_INLINE int sys_bitfield_test_bit(mem_addr_t addr, unsigned int bit)
{
	return sys_test_bit(addr + ((bit >> 5) << 2), bit & 0x1F);
}

/**
 * @brief Test and set a bit in a memory address
 *
 * This function tests if the specified bit is set in the memory address,
 * and then sets the bit.
 *
 * @param addr The memory address
 * @param bit The bit to test and set
 * @return Non-zero if the bit was set before the operation, zero otherwise
 */
static ALWAYS_INLINE int sys_test_and_set_bit(mem_addr_t addr, unsigned int bit)
{
	int ret = sys_test_bit(addr, bit);
	sys_set_bit(addr, bit);
	return ret;
}

/**
 * @brief Test and clear a bit in a memory address
 *
 * This function tests if the specified bit is set in the memory address,
 * and then clears the bit.
 *
 * @param addr The memory address
 * @param bit The bit to test and clear
 * @return Non-zero if the bit was set before the operation, zero otherwise
 */
static ALWAYS_INLINE int sys_test_and_clear_bit(mem_addr_t addr, unsigned int bit)
{
	int ret = sys_test_bit(addr, bit);
	sys_clear_bit(addr, bit);
	return ret;
}

/**
 * @brief Test and set a bit in a bitfield
 *
 * This function tests if the specified bit is set in the bitfield,
 * and then sets the bit.
 *
 * @param addr The memory address of the bitfield
 * @param bit The bit to test and set
 * @return Non-zero if the bit was set before the operation, zero otherwise
 */
static ALWAYS_INLINE int sys_bitfield_test_and_set_bit(mem_addr_t addr, unsigned int bit)
{
	int ret = sys_bitfield_test_bit(addr, bit);
	sys_bitfield_set_bit(addr, bit);
	return ret;
}

/**
 * @brief Test and clear a bit in a bitfield
 *
 * This function tests if the specified bit is set in the bitfield,
 * and then clears the bit.
 *
 * @param addr The memory address of the bitfield
 * @param bit The bit to test and clear
 * @return Non-zero if the bit was set before the operation, zero otherwise
 */
static ALWAYS_INLINE int sys_bitfield_test_and_clear_bit(mem_addr_t addr, unsigned int bit)
{
	int ret = sys_bitfield_test_bit(addr, bit);
	sys_bitfield_clear_bit(addr, bit);
	return ret;
}

#ifdef __cplusplus
}
#endif

#endif /* _ASMLANGUAGE */

#endif /* ZEPHYR_INCLUDE_ARCH_COMMON_SYS_BITOPS_H_ */
//GST