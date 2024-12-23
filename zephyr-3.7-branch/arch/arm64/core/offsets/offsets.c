//arch/arc/core/offsets/offsets.c

#ifndef _ARM_OFFSETS_INC_
#define _ARM_OFFSETS_INC_

#include <gen_offset.h>
#include <zephyr/kernel.h>
#include <kernel_arch_data.h>
#include <kernel_offsets.h>

/* Generate offset symbols for _thread_arch_t structure */
GEN_OFFSET_SYM(_thread_arch_t, exception_depth);

/* Generate named offset symbols for _callee_saved_t structure */
GEN_NAMED_OFFSET_SYM(_callee_saved_t, x19, x19_x20);
GEN_NAMED_OFFSET_SYM(_callee_saved_t, x21, x21_x22);
GEN_NAMED_OFFSET_SYM(_callee_saved_t, x23, x23_x24);
GEN_NAMED_OFFSET_SYM(_callee_saved_t, x25, x25_x26);
GEN_NAMED_OFFSET_SYM(_callee_saved_t, x27, x27_x28);
GEN_NAMED_OFFSET_SYM(_callee_saved_t, x29, x29_sp_el0);
GEN_NAMED_OFFSET_SYM(_callee_saved_t, sp_elx, sp_elx_lr);

#ifdef CONFIG_FRAME_POINTER
/* Generate named offset symbol for frame pointer in _esf_t structure */
GEN_NAMED_OFFSET_SYM(_esf_t, fp, fp);
#endif

/* Generate named offset symbols for _esf_t structure */
GEN_NAMED_OFFSET_SYM(_esf_t, spsr, spsr_elr);
GEN_NAMED_OFFSET_SYM(_esf_t, x18, x18_lr);
GEN_NAMED_OFFSET_SYM(_esf_t, x16, x16_x17);
GEN_NAMED_OFFSET_SYM(_esf_t, x14, x14_x15);
GEN_NAMED_OFFSET_SYM(_esf_t, x12, x12_x13);
GEN_NAMED_OFFSET_SYM(_esf_t, x10, x10_x11);
GEN_NAMED_OFFSET_SYM(_esf_t, x8, x8_x9);
GEN_NAMED_OFFSET_SYM(_esf_t, x6, x6_x7);
GEN_NAMED_OFFSET_SYM(_esf_t, x4, x4_x5);
GEN_NAMED_OFFSET_SYM(_esf_t, x2, x2_x3);
GEN_NAMED_OFFSET_SYM(_esf_t, x0, x0_x1);

/* Generate absolute symbol for the size of _esf_t structure */
GEN_ABSOLUTE_SYM(___esf_t_SIZEOF, sizeof(_esf_t));

#ifdef CONFIG_ARM64_SAFE_EXCEPTION_STACK
/* Generate offset symbols for safe exception stack in _cpu_arch_t structure */
GEN_OFFSET_SYM(_cpu_arch_t, safe_exception_stack);
GEN_OFFSET_SYM(_cpu_arch_t, current_stack_limit);
GEN_OFFSET_SYM(_cpu_arch_t, corrupted_sp);
GEN_OFFSET_SYM(_thread_arch_t, stack_limit);
GEN_NAMED_OFFSET_SYM(_esf_t, sp, sp_el0);
#endif

#ifdef CONFIG_HAS_ARM_SMCCC
#include <zephyr/arch/arm64/arm-smccc.h>

/* Generate named offset symbols for arm_smccc_res_t structure */
GEN_NAMED_OFFSET_SYM(arm_smccc_res_t, a0, a0_a1);
GEN_NAMED_OFFSET_SYM(arm_smccc_res_t, a2, a2_a3);
GEN_NAMED_OFFSET_SYM(arm_smccc_res_t, a4, a4_a5);
GEN_NAMED_OFFSET_SYM(arm_smccc_res_t, a6, a6_a7);

#endif /* CONFIG_HAS_ARM_SMCCC */

/* End of absolute symbol generation */
GEN_ABS_SYM_END

#endif /* _ARM_OFFSETS_INC_ */
//GST