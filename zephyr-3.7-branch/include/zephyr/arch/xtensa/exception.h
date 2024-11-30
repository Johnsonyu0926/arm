// zephyr-3.7-branch/include/zephyr/arch/xtensa/exception.h

#ifndef ZEPHYR_INCLUDE_ARCH_XTENSA_EXCEPTION_H_
#define ZEPHYR_INCLUDE_ARCH_XTENSA_EXCEPTION_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _ASMLANGUAGE

/**
 * @brief Xtensa Exception Stack Frame
 *
 * Xtensa uses a variable length stack frame depending on how many
 * register windows are in use. This isn't a struct type, it just
 * matches the register/stack-unit width.
 */
struct arch_esf {
	int dummy; /**< Dummy field for sizeof compatibility */
};

#endif /* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ARCH_XTENSA_EXCEPTION_H_ */
//GST