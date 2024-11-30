#ifndef RS485_H
#define RS485_H

#include <stdbool.h>
#include <termios.h>

#define MAX_SEND 48

void Rs485_set_send_dir();
void Rs485_set_receive_dir();
void Rs485_noise_write(int iFd);
ssize_t Rs485_select_read(int fd, unsigned char buffer[], int length, int timeout);
int Rs485_while_select(int fd, unsigned char buffer[], int length, int timeout);
int Rs485_uart_read(char *pcBuf, int iBufLen);
int Rs485_uart_open();
int Rs485_uart_write(const char *pData, int iLen);
int Rs485_get_rs485_state();
int Rs485_uart_work(const char *buf, int len);
void Rs485_uart_fcntl_set(int flag);
void Rs485_uart_close();
int Rs485_SendTrue();
int Rs485_SendFast(const char *err_code);

#endif // RS485_H