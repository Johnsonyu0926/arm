// Filename: console.h
// Score: 92

#ifndef ZEPHYR_INCLUDE_CONSOLE_CONSOLE_H_
#define ZEPHYR_INCLUDE_CONSOLE_CONSOLE_H_

#include <sys/types.h>
#include <zephyr/types.h>
#include <zephyr/kernel.h>

#ifdef __cplusplus
extern "C" {
#endif

int console_init(void);
ssize_t console_read(void *dummy, void *buf, size_t size);
ssize_t console_write(void *dummy, const void *buf, size_t size);
int console_getchar(void);
int console_putchar(char c);
void console_getline_init(void);
char *console_getline(void);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_CONSOLE_CONSOLE_H_ */
// By GST @Date
