// zephyr-3.7-branch/include/zephyr/acpi/acpi_osal.h

#ifndef ZEPHYR_ARCH_X86_INCLUDE_ACPI_OSAL_H_
#define ZEPHYR_ARCH_X86_INCLUDE_ACPI_OSAL_H_

#if defined(CONFIG_X86) || defined(CONFIG_X86_64)
#include <zephyr/acpi/x86_acpi_osal.h>
#else
#error "Currently only x86 Architecture supports ACPI!"
#endif

#endif /* ZEPHYR_ARCH_X86_INCLUDE_ACPI_OSAL_H_ */
//GST