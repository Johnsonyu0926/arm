#include "rs485.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/select.h>
#include <errno.h>

static int m_rs485 = -1;
static int m_rsTty = 1;

void Rs485_set_send_dir() {
    system("echo 1 > /sys/class/gpio/gpio3/value");
}

void Rs485_set_receive_dir() {
    tcdrain(m_rs485);
    system("echo 0 > /sys/class/gpio/gpio3/value");
}

void Rs485_noise_write(int iFd) {
    Rs485_set_send_dir();
    const unsigned char data[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A};
    write(iFd, data, sizeof(data));
    system("echo 0 > /sys/class/gpio/gpio3/value");
}

ssize_t Rs485_select_read(int fd, unsigned char buffer[], int length, int timeout) {
    struct timeval time;
    time.tv_sec = timeout / 1000;
    time.tv_usec = (timeout % 1000) * 1000;

    fd_set fs_read;
    FD_ZERO(&fs_read);
    FD_SET(fd, &fs_read);

    if (select(fd + 1, &fs_read, NULL, NULL, &time)) {
        return read(fd, buffer, length);
    } else {
        return 0;
    }
}

int Rs485_while_select(int fd, unsigned char buffer[], int length, int timeout) {
    struct timeval time;
    time.tv_sec = timeout;
    fd_set fs_read;
    while (true) {
        FD_ZERO(&fs_read);
        FD_SET(fd, &fs_read);
        int res = select(fd + 1, &fs_read, NULL, NULL, &time);
        if (res <= 0) {
            return -1;
        }
        return read(fd, buffer, length);
    }
}

int Rs485_uart_read(char *pcBuf, int iBufLen) {
    Rs485_set_receive_dir();
    int iFd = m_rs485, iLen = 0;
    int i;

    *pcBuf = '\0';
    printf("reading... from fd: %d\n", iFd);
    iLen = Rs485_select_read(iFd, (unsigned char *)pcBuf, iBufLen, 100);
    if (iLen < 0) {
        close(iFd);
        m_rs485 = -1;
        printf("error read from fd %d\n", iFd);
        return iLen;
    }

    while (iLen == 1 && pcBuf[0] == 0x0) {
        printf("ignore the 0x0.\n");
        iLen = Rs485_select_read(iFd, (unsigned char *)pcBuf, iBufLen, 100);
        if (AcquisitionNoise_getInstance()->getMonitorStatus()) {
            close(iFd);
            m_rs485 = -1;
            return -1;
        }
    }

    printf("read success: iLen= %d, hex dump:\n", iLen);
    for (i = 0; i < iLen; i++) {
        printf("%02x ", pcBuf[i]);
    }
    printf("\nhex dump end.\n");

    while (true) {
        if ((iLen > 5) &&
            ('B' == pcBuf[iLen - 5]) &&
            ('B' == pcBuf[iLen - 4]) &&
            (' ' == pcBuf[iLen - 3]) &&
            ('E' == pcBuf[iLen - 2]) &&
            ('F' == pcBuf[iLen - 1])) {
            printf("receive completed.\n");
            break;
        }

        int next = Rs485_select_read(iFd, (unsigned char *)pcBuf + iLen, iBufLen - iLen, 100);
        if (AcquisitionNoise_getInstance()->getMonitorStatus()) {
            close(iFd);
            m_rs485 = -1;
            return -1;
        }
        iLen += next;
    }
    printf("total len = %d\n", iLen);

    for (i = 0; i < iLen; i++) {
        printf("%02x ", pcBuf[i]);
    }
    printf("\nhex dump end.\n");
    return iLen;
}

int Rs485_uart_open() {
    int iFd = -1;
    struct termios opt;
    asns::CAudioCfgBusiness cfg;
    CAudioCfgBusiness_load(&cfg);
    int iBdVal = cfg.business[0].iBdVal;

    system("echo 3 > /sys/class/gpio/export");
    system("echo out > /sys/class/gpio/gpio3/direction");

    char name[32] = {0};
    sprintf(name, "/dev/ttyS%d", m_rsTty);

    iFd = open(name, O_RDWR | O_NOCTTY);
    printf("uart open fd: %d\n", iFd);
    if (iFd < 0) {
        printf("uart open fd error %d\n", iFd);
        return -1;
    }

    tcgetattr(iFd, &opt);
    cfmakeraw(&opt);

    cfsetispeed(&opt, iBdVal);
    cfsetospeed(&opt, iBdVal);

    tcsetattr(iFd, TCSANOW, &opt);
    char cmd[64] = {0};
    sprintf(cmd, "stty -F /dev/ttyS%d %d", m_rsTty, iBdVal);
    system(cmd);
    printf("uart open ibd: %s\n", cmd);
    m_rs485 = iFd;
    return iFd;
}

int Rs485_uart_write(const char *pData, int iLen) {
    Rs485_set_send_dir();
    int iFd = m_rs485;
    int iRet = -1;
    int len = 0;
    const unsigned char *pcData = (const unsigned char *)pData;
    int count = iLen / MAX_SEND;
    if (iLen % MAX_SEND) {
        count++;
    }
    int offset = 0;
    printf("count = %d\n", count);
    for (int i = 0; i < count; i++) {
        if ((i + 1) * MAX_SEND > iLen) {
            len = iLen - i * MAX_SEND;
        } else {
            len = MAX_SEND;
        }
        printf("no.%d: offset: %d len: %d\n", i, offset, len);
        const unsigned char *data = pcData + offset;
        for (int j = 0; j < len; j++) {
            printf("%02x ", data[j]);
        }
        printf("\n");
        iRet = write(iFd, data, len);
        if (iRet < 0) {
            close(iFd);
            m_rs485 = -1;
            printf("error write %d, len: %d\n", iFd, len);
        } else {
            printf("no.%d: write len: %d success, iRet: %d\n", i, len, iRet);
        }

        offset += MAX_SEND;
    }
    Rs485_set_receive_dir();
    return iRet;
}

int Rs485_get_rs485_state() {
    return m_rs485 != -1;
}

int Rs485_uart_work(const char *buf, int len) {
    if (m_rs485 <= 0) {
        int fd = Rs485_uart_open();
        if (fd < 0) {
            printf("failed to open ttyS%d to read write.\n", m_rsTty);
            return 0;
        }
        m_rs485 = fd;
    }
    Rs485_uart_write(buf, len);
    return 1;
}

void Rs485_uart_fcntl_set(int flag) {
    fcntl(m_rs485, F_SETFL, flag);
}

void Rs485_uart_close() {
    close(m_rs485);
    printf("m_rs485 close fd: %d\n", m_rs485);
    m_rs485 = -1;
}

int Rs485_SendTrue() {
    const char *res = "01 E1";
    return Rs485_uart_write(res, strlen(res));
}

int Rs485_SendFast(const char *err_code) {
    char buf[64];
    snprintf(buf, sizeof(buf), "01 %s", err_code);
    return Rs485_uart_write(buf, strlen(buf));
}