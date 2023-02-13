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

#include "audiocfg.hpp"

static int g_rs485 = -1;
static int g_rsTty = 1; //ttyUSB1 as default.
const int MAX_SEND = 12;

void set_send_dir() {
    system("echo 1 > /sys/class/gpio/gpio3/value");
    printf("set send dir! gpio is 1\n");
}

void set_receive_dir() {
    system("echo 0 > /sys/class/gpio/gpio3/value");
    printf("set receive dir! gpio is 0\n");
}

static int _uart_open(void) {
    int iFd = -1;
    struct termios opt;
    asns::CAudioCfgBusiness cfg;
    cfg.load();
    int iBdVal = cfg.business[0].iBdVal;

    system("echo 3 > /sys/class/gpio/export");
    system("echo out > /sys/class/gpio/gpio3/direction");

    char name[32];
    sprintf(name, "/dev/ttyS%d", g_rsTty);

    iFd = open(name, O_RDWR | O_NOCTTY);  /* 读写方式打开串口 */

    if (iFd < 0) {
        return -1;
    }

    tcgetattr(iFd, &opt);
    cfmakeraw(&opt);

    cfsetispeed(&opt, iBdVal);
    cfsetospeed(&opt, iBdVal);

    tcsetattr(iFd, TCSANOW, &opt);
    char cmd[64];
    sprintf(cmd, "stty -F /dev/ttyS%d %d", g_rsTty, iBdVal);
    system(cmd);

    return iFd;
}

/* Write data to uart dev, return 0 means OK */
static int _uart_write(const char *pcData, int iLen) {
    int iFd = g_rs485;
    int iRet = -1;
    int len = 0;
    printf("to write :%s, len:%d\n", pcData, iLen);
    int count = iLen / MAX_SEND;
    if (iLen % MAX_SEND) {
        count++;
    }
    int offset = 0;
    printf("count=%d\n", count);
    for (int i = 0; i < count; i++) {
        if ((i + 1) * MAX_SEND > iLen) {
            len = iLen - i * MAX_SEND;
        } else {
            len = MAX_SEND;
        }
        printf("no.%d : offset:%d len:%d \n", i, offset, len);
        const char *data = pcData + offset;
        for (int j = 0; j < len; j++) {
            printf("%02x ", data[j]);
        }
        iRet = write(iFd, data, len);
        if (iRet < 0) {
            close(iFd);
            g_rs485 = -1;
            printf("error write %d , len:%d\n", iFd, len);
        } else {
            printf("no.%d : write len:%d success, iRet:%d\n", i, len, iRet);
        }

        offset += MAX_SEND;

        usleep(100 * 1000);
    }
    return iRet;
}

int _uart_work(const char *buf, int len) {
    int fd = _uart_open();
    if (fd < 0) {
        printf("failed to open ttyS%d to read write.\n", g_rsTty);
        return 2;
    }
    g_rs485 = fd;
    set_send_dir();
    _uart_write(buf, len);
    return 1;
}


int SendTrue() {
    std::string res = "01 E1";
    return _uart_write(res.c_str(), res.length());
}

int SendFast(const char *err_code) {
    std::string code = err_code;
    std::string buf = "01 " + code;
    _uart_write(buf.c_str(), buf.length());
    return 0;
}