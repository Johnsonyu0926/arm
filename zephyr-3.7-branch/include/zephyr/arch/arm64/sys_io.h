// zephyr-3.7-branch/include/zephyr/arch/arm64/sys_io.h

#ifndef ZEPHYR_INCLUDE_ARCH_ARM64_SYS_IO_H_
#define ZEPHYR_INCLUDE_ARCH_ARM64_SYS_IO_H_

#ifndef _ASMLANGUAGE

#include <zephyr/types.h>
#include <zephyr/sys/sys_io.h>
#include <zephyr/sys/barrier.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Memory mapped registers I/O functions */

/**
 * @brief Read an 8-bit value from a memory-mapped register
 *
 * This function reads an 8-bit value from the specified memory-mapped register address.
 *
 * @param addr The address of the memory-mapped register
 * @return The 8-bit value read from the register
 */
static ALWAYS_INLINE uint8_t sys_read8(mem_addr_t addr)
{
	uint8_t val;

	__asm__ volatile("ldrb %w0, [%1]" : "=r" (val) : "r" (addr));

	barrier_dmem_fence_full();
	return val;
}

/**
 * @brief Write an 8-bit value to a memory-mapped register
 *
 * This function writes an 8-bit value to the specified memory-mapped register address.
 *
 * @param data The 8-bit value to write
 * @param addr The address of the memory-mapped register
 */
static ALWAYS_INLINE void sys_write8(uint8_t data, mem_addr_t addr)
{
	barrier_dmem_fence_full();
	__asm__ volatile("strb %w0, [%1]" : : "r" (data), "r" (addr));
}

/**
 * @brief Read a 16-bit value from a memory-mapped register
 *
 * This function reads a 16-bit value from the specified memory-mapped register address.
 *
 * @param addr The address of the memory-mapped register
 * @return The 16-bit value read from the register
 */
static ALWAYS_INLINE uint16_t sys_read16(mem_addr_t addr)
{
	uint16_t val;

	__asm__ volatile("ldrh %w0, [%1]" : "=r" (val) : "r" (addr));

	barrier_dmem_fence_full();
	return val;
}

/**
 * @brief Write a 16-bit value to a memory-mapped register
 *
 * This function writes a 16-bit value to the specified memory-mapped register address.
 *
 * @param data The 16-bit value to write
 * @param addr The address of the memory-mapped register
 */
static ALWAYS_INLINE void sys_write16(uint16_t data, mem_addr_t addr)
{
	barrier_dmem_fence_full();
	__asm__ volatile("strh %w0, [%1]" : : "r" (data), "r" (addr));
}

/**
 * @brief Read a 32-bit value from a memory-mapped register
 *
 * This function reads a 32-bit value from the specified memory-mapped register address.
 *
 * @param addr The address of the memory-mapped register
 * @return The 32-bit value read from the register
 */
static ALWAYS_INLINE uint32_t sys_read32(mem_addr_t addr)
{
	uint32_t val;

	__asm__ volatile("ldr %w0, [%1]" : "=r" (val) : "r" (addr));

	barrier_dmem_fence_full();
	return val;
}

/**
 * @brief Write a 32-bit value to a memory-mapped register
 *
 * This function writes a 32-bit value to the specified memory-mapped register address.
 *
 * @param data The 32-bit value to write
 * @param addr The address of the memory-mapped register
 */
static ALWAYS_INLINE void sys_write32(uint32_t data, mem_addr_t addr)
{
	barrier_dmem_fence_full();
	__asm__ volatile("str %w0, [%1]" : : "r" (data), "r" (addr));
}

/**
 * @brief Read a 64-bit value from a memory-mapped register
 *
 * This function reads a 64-bit value from the specified memory-mapped register address.
 *
 * @param addr The address of the memory-mapped register
 * @return The 64-bit value read from the register
 */
static ALWAYS_INLINE uint64_t sys_read64(mem_addr_t addr)
{
	uint64_t val;

	__asm__ volatile("ldr %x0, [%1]" : "=r" (val) : "r" (addr));

	barrier_dmem_fence_full();
	return val;
}

/**
 * @brief Write a 64-bit value to a memory-mapped register
 *
 * This function writes a 64-bit value to the specified memory-mapped register address.
 *
 * @param data The 64-bit value to write
 * @param addr The address of the memory-mapped register
 */
static ALWAYS_INLINE void sys_write64(uint64_t data, mem_addr_t addr)
{
	barrier_dmem_fence_full();
	__asm__ volatile("str %x0, [%1]" : : "r" (data), "r" (addr));
}

#ifdef __cplusplus
}
#endif

#endif /* _ASMLANGUAGE */

#endif /* ZEPHYR_INCLUDE_ARCH_ARM64_SYS_IO_H_ */
//GST