// util.c

#include <common.h>
#include <dm/util.h>
#include <linux/libfdt.h>
#include <vsprintf.h>

#ifdef CONFIG_DM_WARN
/**
 * @brief Print a warning message
 *
 * @param fmt Format string
 * @param ... Additional arguments
 */
void dm_warn(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
}
#endif

/**
 * @brief Count the number of items in a list
 *
 * @param head Pointer to the head of the list
 * @return Number of items in the list
 */
int list_count_items(struct list_head *head)
{
	struct list_head *node;
	int count = 0;

	list_for_each(node, head)
		count++;

	return count;
}

/**
 * @brief Check if a device tree node has a pre-relocation property
 *
 * @param blob Pointer to the device tree blob
 * @param offset Offset of the node
 * @return true if the node has a pre-relocation property, false otherwise
 */
bool dm_fdt_pre_reloc(const void *blob, int offset)
{
	if (fdt_getprop(blob, offset, "u-boot,dm-pre-reloc", NULL))
		return true;

#ifdef CONFIG_TPL_BUILD
	if (fdt_getprop(blob, offset, "u-boot,dm-tpl", NULL))
		return true;
#elif defined(CONFIG_SPL_BUILD)
	if (fdt_getprop(blob, offset, "u-boot,dm-spl", NULL))
		return true;
#else
	/*
	 * In regular builds individual spl and tpl handling both
	 * count as handled pre-relocation for later second init.
	 */
	if (fdt_getprop(blob, offset, "u-boot,dm-spl", NULL) ||
	    fdt_getprop(blob, offset, "u-boot,dm-tpl", NULL))
		return true;
#endif

	return false;
}

/**
 * @brief Print an error message
 *
 * @param fmt Format string
 * @param ... Additional arguments
 */
void dm_error(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
}

/**
 * @brief Print an informational message
 *
 * @param fmt Format string
 * @param ... Additional arguments
 */
void dm_info(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
}

/**
 * @brief Print a debug message
 *
 * @param fmt Format string
 * @param ... Additional arguments
 */
void dm_debug(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
}

/**
 * @brief Check if a device tree node has a specific property
 *
 * @param blob Pointer to the device tree blob
 * @param offset Offset of the node
 * @param prop Property name
 * @return true if the node has the property, false otherwise
 */
bool dm_fdt_has_prop(const void *blob, int offset, const char *prop)
{
	return fdt_getprop(blob, offset, prop, NULL) != NULL;
}
//GST