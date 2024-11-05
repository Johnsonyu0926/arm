// Filename: smccc.S
#include <zephyr/toolchain.h>
#include <zephyr/linker/sections.h>
#include <zephyr/arch/cpu.h>
#include <offsets_short.h>
.macro SMCCC instr
	\instr  #0
	ldr     x4, [sp]
	stp     x0, x1, [x4, __arm_smccc_res_t_a0_a1_OFFSET]
	stp     x2, x3, [x4, __arm_smccc_res_t_a2_a3_OFFSET]
	stp     x4, x5, [x4, __arm_smccc_res_t_a4_a5_OFFSET]
	stp     x6, x7, [x4, __arm_smccc_res_t_a6_a7_OFFSET]
	ret
.endm
/*
 * The SMC instruction is used to generate a synchronous exception that is
 * handled by Secure Monitor code running in EL3.
 */
GTEXT(arm_smccc_smc)
SECTION_FUNC(TEXT, arm_smccc_smc)
	SMCCC	smc
/*
 * The HVC instruction is used to generate a synchronous exception that is
 * handled by a hypervisor running in EL2.
 */
GTEXT(arm_smccc_hvc)
SECTION_FUNC(TEXT, arm_smccc_hvc)
	SMCCC	hvc
// By GST @Date