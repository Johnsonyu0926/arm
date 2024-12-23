// Filename: opcodes-sec.h

#ifndef __ASM_ARM_OPCODES_SEC_H
#define __ASM_ARM_OPCODES_SEC_H

#include <asm/opcodes.h>

#define __SMC(imm4) __inst_arm_thumb32(                    \
    0xE1600070 | (((imm4) & 0xF) << 0),                    \
    0xF7F08000 | (((imm4) & 0xF) << 16)                    \
)

#endif /* __ASM_ARM_OPCODES_SEC_H */

// By GST @Date