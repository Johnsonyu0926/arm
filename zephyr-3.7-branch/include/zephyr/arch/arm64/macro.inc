// zephyr-3.7-branch/include/zephyr/arch/arm64/macro.inc
#ifndef ZEPHYR_INCLUDE_ARCH_ARM64_MACRO_INC_
#define ZEPHYR_INCLUDE_ARCH_ARM64_MACRO_INC_
#ifdef _ASMLANGUAGE
/**
 * @brief Switch to the appropriate exception level
 *
 * This macro switches to the appropriate exception level based on the current exception level.
 *
 * @param xreg The register to use for the current exception level
 * @param el3_label The label to jump to if the current exception level is EL3
 * @param el2_label The label to jump to if the current exception level is EL2
 * @param el1_label The label to jump to if the current exception level is EL1
 */
.macro switch_el, xreg, el3_label, el2_label, el1_label
    mrs \xreg, CurrentEL
    cmp \xreg, 0xc
    beq \el3_label
    cmp \xreg, 0x8
    beq \el2_label
    cmp \xreg, 0x4
    beq \el1_label
.endm
/**
 * @brief Move an immediate constant to a 64-bit register
 *
 * This macro moves an immediate constant to a 64-bit register. It generates an instruction sequence
 * of 'mov'/'movz' and one to three 'movk' instructions depending on the immediate value.
 *
 * @param xreg The register to move the immediate constant to
 * @param imm The immediate constant to move
 */
.macro mov_imm, xreg, imm
    .if ((\imm) == 0)
        mov \xreg, \imm
    .else
        .if (((\imm) >> 31) == 0 || ((\imm) >> 31) == 0x1ffffffff)
            movz \xreg, (\imm >> 16) & 0xffff, lsl 16
        .else
            .if (((\imm) >> 47) == 0 || ((\imm) >> 47) == 0x1ffff)
                movz \xreg, (\imm >> 32) & 0xffff, lsl 32
            .else
                movz \xreg, (\imm >> 48) & 0xffff, lsl 48
                movk \xreg, (\imm >> 32) & 0xffff, lsl 32
            .endif
            movk \xreg, (\imm >> 16) & 0xffff, lsl 16
        .endif
        movk \xreg, (\imm) & 0xffff, lsl 0
    .endif
.endm
#endif /* _ASMLANGUAGE */
#endif /* ZEPHYR_INCLUDE_ARCH_ARM64_MACRO_INC_ */
//GST