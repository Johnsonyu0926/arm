/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <stdio.h>

/**
 * @brief Default stdout hook function
 *
 * @param c Character to write
 * @return Always returns EOF
 */
static int _stdout_hook_default(int c)
{
	(void)(c);  /* Prevent warning about unused argument */

	return EOF;
}

static int (*_stdout_hook)(int) = _stdout_hook_default;

/**
 * @brief Install a custom stdout hook function
 *
 * @param hook Pointer to the custom hook function
 */
void __stdout_hook_install(int (*hook)(int))
{
	_stdout_hook = hook;
}

/**
 * @brief Get the address of the errno variable
 *
 * @return Pointer to the errno variable
 */
volatile int *__aeabi_errno_addr(void)
{
	return &_current->errno_var;
}

/**
 * @brief Write a character to the specified stream
 *
 * @param c Character to write
 * @param f Pointer to the FILE object that identifies the stream
 * @return The character written as an unsigned char cast to an int or EOF on error
 */
int fputc(int c, FILE *f)
{
	return (_stdout_hook)(c);
}
//GST
