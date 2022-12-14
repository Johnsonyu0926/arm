#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>

void set_send_dir() {
    system("echo 1 > /sys/class/gpio/gpio3/value");
    printf("set send dir! gpio is 1\n");
}

void set_receive_dir() {
    system("echo 0 > /sys/class/gpio/gpio3/value");
    printf("set receive dir! gpio is 0\n");
}

static int g_irs485 = -1;
static int g_tty = 1; //ttyUSB1 as default.

static int _uart_open(void) {
    int iFd = -1;
    struct termios opt;
    int iBdVal = 9600;

    system("echo 3 > /sys/class/gpio/export");
    system("echo out > /sys/class/gpio/gpio3/direction");

    char name[32];
    sprintf(name, "/dev/ttyS%d", g_tty);

    iFd = open(name, O_RDWR | O_NOCTTY);  /* 读写方式打开串口 */

    if (iFd < 0) {
        return -1;
    }

    tcgetattr(iFd, &opt);
    cfmakeraw(&opt);

    cfsetispeed(&opt, iBdVal);
    cfsetospeed(&opt, iBdVal);

    tcsetattr(iFd, TCSANOW, &opt);

    return iFd;
}

/* Write data to uart dev, return 0 means OK */
static int _uart_write(const char *pcData, int iLen) {
    int iFd = g_irs485;
    int iRet = -1;

    printf("to write :%s, len:%d\n", pcData, iLen);
    iRet = write(iFd, pcData, iLen);
    if (iRet < 0) {
        close(iFd);
        g_irs485 = -1;
        printf("error write %d , len:%d\n", iFd, iLen);
    } else {
        iRet = 0;
    }

    return iRet;
}

/* Read data from uart dev, return read len */
static int _uart_read(char *pcBuf, int iBufLen) {
    int iFd = g_irs485, iLen = 0;

    *pcBuf = '\0';
    printf("reading... from fd:%d\n", iFd);
    iLen = read(iFd, pcBuf, iBufLen);
    if (iLen < 0) {
        close(iFd);
        g_irs485 = -1;
        printf("error read from fd %d\n", iFd);
        return iLen;
    }

    printf("read success: iLen= %d , buf:%s\n", iLen, pcBuf);

    /*
    while ((iLen > 5) &&
        ('B' != pcBuf[iLen - 5]) &&
        ('B' != pcBuf[iLen - 4]) &&
        (' ' != pcBuf[iLen - 3]) &&
        ('E' != pcBuf[iLen - 2]) && ('F' != pcBuf[iLen - 1])) {
        printf("continue to read BB EF...\n");
        int next = read(iFd, pcBuf + iLen, iBufLen - iLen);
        iLen += next;
        printf("read next :%d, iLen:%d, buf:%s\n", next, iLen, pcBuf);
    }
    */

    while (1) {
        if ((iLen > 5) &&
            ('B' == pcBuf[iLen - 5]) &&
            ('B' == pcBuf[iLen - 4]) &&
            (' ' == pcBuf[iLen - 3]) &&
            ('E' == pcBuf[iLen - 2]) && ('F' == pcBuf[iLen - 1])) {
            printf("receive completed.\n");
            break;
        }

        int next = read(iFd, pcBuf + iLen, iBufLen - iLen);
        iLen += next;
        printf("read next :%d, iLen:%d, buf:%s\n", next, iLen, pcBuf);

    }
    return iLen;
}

int handle_receive(char *buf, int len) {
    return _uart_write(buf, len);//todo
}

int worker() {

    printf("set tty to %d\n", g_tty);

    int fd = _uart_open();
    if (fd < 0) {
        printf("failed to open ttyS%d to read write.\n", g_tty);
        return -1;
    }
    g_irs485 = fd;


    int readcount = 0;
    char buf[1024] = {0};
    while (1) {
        memset(buf, 0, sizeof(buf));

        set_receive_dir();
        readcount = _uart_read(buf, sizeof(buf));

        if (readcount < 0) {
            printf("failed to read ! errno = %d, strerror=%s\n", errno, strerror(errno));
            return 0;
        }
        printf("recv request:%s, len:%d, handle it...\n", buf, readcount);

        set_send_dir();
        handle_receive(buf, readcount);
    }
}
