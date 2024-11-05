/*
 * Copyright (c) 2010-2011, 2013-2014 Wind River Systems, Inc.
 * Copyright (c) 2016 Intel Corporation.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Memory-mapped register I/O operations
 *
 * This file contains the interface for memory mapped register I/O
 * access. The functions are just wrappers around volatile pointer
 * dereferencing. The purpose of this interface is to provide a
 * well-defined set of APIs for memory mapped register access that can
 * be used by device drivers and other kernel-level software.
 */

#ifndef ZEPHYR_INCLUDE_ARCH_COMMON_SYS_IO_H_
#define ZEPHYR_INCLUDE_ARCH_COMMON_SYS_IO_H_

#include <zephyr/types.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Read a 8-bit value from an I/O register
 *
 * @param addr Address of the I/O register
 *
 * @return The 8-bit value read from the I/O register
 */
static ALWAYS_INLINE uint8_t sys_read8(mem_addr_t addr)
{
    return *(volatile uint8_t *)addr;
}

/**
 * @brief Write a 8-bit value to an I/O register
 *
 * @param data The 8-bit value to write
 * @param addr Address of the I/O register
 */
static ALWAYS_INLINE void sys_write8(uint8_t data, mem_addr_t addr)
{
    *(volatile uint8_t *)addr = data;
}

/**
 * @brief Read a 16-bit value from an I/O register
 *
 * @param addr Address of the I/O register
 *
 * @return The 16-bit value read from the I/O register
 */
static ALWAYS_INLINE uint16_t sys_read16(mem_addr_t addr)
{
    return *(volatile uint16_t *)addr;
}

/**
 * @brief Write a 16-bit value to an I/O register
 *
 * @param data The 16-bit value to write
 * @param addr Address of the I/O register
 */
static ALWAYS_INLINE void sys_write16(uint16_t data, mem_addr_t addr)
{
    *(volatile uint16_t *)addr = data;
}

/**
 * @brief Read a 32-bit value from an I/O register
 *
 * @param addr Address of the I/O register
 *
 * @return The 32-bit value read from the I/O register
 */
static ALWAYS_INLINE uint32_t sys_read32(mem_addr_t addr)
{
    return *(volatile uint32_t *)addr;
}

/**
 * @brief Write a 32-bit value to an I/O register
 *
 * @param data The 32-bit value to write
 * @param addr Address of the I/O register
 */
static ALWAYS_INLINE void sys_write32(uint32_t data, mem_addr_t addr)
{
    *(volatile uint32_t *)addr = data;
}

/**
 * @brief Clear a bit in a 32-bit I/O register
 *
 * @param addr Address of the I/O register
 * @param bit Bit position to clear
 */
static ALWAYS_INLINE void sys_clear_bit(mem_addr_t addr, unsigned int bit)
{
    uint32_t temp = sys_read32(addr);

    sys_write32(temp & ~(1 << bit), addr);
}

/**
 * @brief Set a bit in a 32-bit I/O register
 *
 * @param addr Address of the I/O register
 * @param bit Bit position to set
 */
static ALWAYS_INLINE void sys_set_bit(mem_addr_t addr, unsigned int bit)
{
    uint32_t temp = sys_read32(addr);

    sys_write32(temp | (1 << bit), addr);
}

/**
 * @brief Test a bit in a 32-bit I/O register
 *
 * @param addr Address of the I/O register
 * @param bit Bit position to test
 *
 * @return 1 if the bit is set, 0 if the bit is clear
 */
static ALWAYS_INLINE int sys_test_bit(mem_addr_t addr, unsigned int bit)
{
    uint32_t temp = sys_read32(addr);

    return (temp & (1 << bit)) != 0;
}

/**
 * @brief Clear bits in a 32-bit I/O register
 *
 * @param addr Address of the I/O register
 * @param mask Mask of bits to clear
 */
static ALWAYS_INLINE void sys_clear_bits(mem_addr_t addr, uint32_t mask)
{
    uint32_t temp = sys_read32(addr);

    sys_write32(temp & ~mask, addr);
}

/**
 * @brief Set bits in a 32-bit I/O register
 *
 * @param addr Address of the I/O register
 * @param mask Mask of bits to set
 */
static ALWAYS_INLINE void sys_set_bits(mem_addr_t addr, uint32_t mask)
{
    uint32_t temp = sys_read32(addr);

    sys_write32(temp | mask, addr);
}

/**
 * @brief Test bits in a 32-bit I/O register
 *
 * @param addr Address of the I/O register
 * @param mask Mask of bits to test
 *
 * @return 1 if any of the bits are set, 0 if none of the bits are set
 */
static ALWAYS_INLINE int sys_test_bits(mem_addr_t addr, uint32_t mask)
{
    uint32_t temp = sys_read32(addr);

    return (temp & mask) != 0;
}

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ARCH_COMMON_SYS_IO_H_ */
// By GST @Date