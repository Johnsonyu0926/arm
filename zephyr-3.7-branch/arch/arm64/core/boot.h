//arch/arc/core/boot.h
/**
 * @file
 * @brief Definitions for boot code
 */

#ifndef _BOOT_H_
#define _BOOT_H_

#ifndef _ASMLANGUAGE

/**
 * @brief External declaration of the vector table.
 */
extern void *_vector_table[];

/**
 * @brief Entry point for the start of the program.
 */
extern void __start(void);

#endif /* _ASMLANGUAGE */

/* Offsets into the boot_params structure */
#define BOOT_PARAM_MPID_OFFSET    0  /**< Offset for MPID */
#define BOOT_PARAM_SP_OFFSET      8  /**< Offset for Stack Pointer */
#define BOOT_PARAM_VOTING_OFFSET  16 /**< Offset for Voting */

#endif /* _BOOT_H_ */
//GST
