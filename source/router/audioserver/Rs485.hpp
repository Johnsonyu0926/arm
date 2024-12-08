#pragma once

#include <cstdlib>
#include <csignal>
#include <cerrno>
#include <cstring>
#include <cstdio>
#include <fcntl.h>
#include <string>
#include <termios.h>
#include "audiocfg.hpp"
#include "AcquisitionNoise.hpp"
#include "json.hpp"
#include "utils.h"
#include "spdlog/spdlog.h"

static int m_rs485{-1};
static int m_rsTty{1};

class Rs485 {
public:
    static constexpr int MAX_SEND = 48;

    static void set_send_dir() {
        system("echo 1 > /sys/class/gpio/gpio3/value");
    }

    static void set_receive_dir() {
        tcdrain(m_rs485);
        system("echo 0 > /sys/class/gpio/gpio3/value");
    }

    static void noise_write(int iFd) {
        set_send_dir();
        const unsigned char data[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A};
        write(iFd, data, sizeof(data));
        system("echo 0 > /sys/class/gpio/gpio3/value");
    }

    static ssize_t select_read(int fd, unsigned char buffer[], int length, int timeout) {
        timeval time{};
        time.tv_sec = timeout / 1000; // set the rcv wait time
        time.tv_usec = (timeout % 1000) * 1000; // 100000us = 0.1s

        fd_set fs_read;
        FD_ZERO(&fs_read); // 每次循环都要清空集合，否则不能检测描述符变化
        FD_SET(fd, &fs_read); // 添加描述符

        // 超时等待读变化，>0：就绪描述字的正数目， -1：出错， 0 ：超时
        if (select(fd + 1, &fs_read, nullptr, nullptr, &time)) {
            return read(fd, buffer, length);
        } else {
            spdlog::error("select() failed: {}", strerror(errno));
            return 0;
        }
    }

    static int while_select(int fd, unsigned char buffer[], int length, int timeout) {
        timeval time{};
        time.tv_sec = timeout; // set the rcv wait time
        fd_set fs_read;
        while (true) {
            FD_ZERO(&fs_read); // 每次循环都要清空集合，否则不能检测描述符变化
            FD_SET(fd, &fs_read); // 添加描述符
            // 超时等待读变化，>0：就绪描述字的正数目， -1：出错， 0 ：超时
            int res = select(fd + 1, &fs_read, nullptr, nullptr, &time);
            if (res <= 0) {
                return -1;
            }
            return read(fd, buffer, length);
        }
    }

    static int _uart_read(char *pcBuf, int iBufLen) {
        set_receive_dir();
        int iFd = m_rs485, iLen = 0;
        int i;

        *pcBuf = '\0';
        spdlog::info("reading... from fd: {}", iFd);
        iLen = select_read(iFd, (unsigned char *)pcBuf, iBufLen, 100);
        if (iLen < 0) {
            close(iFd);
            m_rs485 = -1;
            spdlog::error("error read from fd {}", iFd);
            return iLen;
        }

        // ignore 0x0
        while (iLen == 1 && pcBuf[0] == 0x0) {
            spdlog::info("ignore the 0x0.");
            iLen = select_read(iFd, (unsigned char *)pcBuf, iBufLen, 100);
            if (AcquisitionNoise::getInstance().getMonitorStatus()) {
                close(iFd);
                m_rs485 = -1;
                return -1;
            }
        }

        spdlog::info("read success: iLen= {}, hex dump: ", iLen);
        for (i = 0; i < iLen; i++) {
            spdlog::info("{:02x} ", static_cast<unsigned char>(pcBuf[i]));
        }
        spdlog::info("hex dump end.");

        while (true) {
            if ((iLen > 5) &&
                ('B' == pcBuf[iLen - 5]) &&
                ('B' == pcBuf[iLen - 4]) &&
                (' ' == pcBuf[iLen - 3]) &&
                ('E' == pcBuf[iLen - 2]) &&
                ('F' == pcBuf[iLen - 1])) {
                spdlog::info("receive completed.");
                break;
            }

            int next = select_read(iFd, (unsigned char *)pcBuf + iLen, iBufLen - iLen, 100);
            if (AcquisitionNoise::getInstance().getMonitorStatus()) {
                close(iFd);
                m_rs485 = -1;
                return -1;
            }
            iLen += next;
        }
        spdlog::info("total len = {}", iLen);

        for (i = 0; i < iLen; i++) {
            spdlog::info("{:02x} ", static_cast<unsigned char>(pcBuf[i]));
        }
        spdlog::info("hex dump end.");
        return iLen;
    }

    static int _uart_open() {
        int iFd = -1;
        termios opt{};
        asns::CAudioCfgBusiness cfg;
        cfg.load();
        int iBdVal = cfg.business[0].iBdVal;

        system("echo 3 > /sys/class/gpio/export");
        system("echo out > /sys/class/gpio/gpio3/direction");

        char name[32] = {0};
        sprintf(name, "/dev/ttyS%d", m_rsTty);

        iFd = open(name, O_RDWR | O_NOCTTY); /* 读写方式打开串口 */
        spdlog::info("uart open fd: {}", iFd);
        if (iFd < 0) {
            spdlog::warn("uart open fd error {}", iFd);
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
        spdlog::info("uart open ibd : {}", cmd);
        m_rs485 = iFd;
        return iFd;
    }

    /* Write data to uart dev, return 0 means OK */
    static int _uart_write(const char *pData, int iLen) {
        set_send_dir();
        int iFd = m_rs485;
        int iRet = -1;
        int len = 0;
        const unsigned char *pcData = (const unsigned char *)pData;
        int count = iLen / MAX_SEND;
        if (iLen % MAX_SEND) {
            count++;
        }
        int offset = 0;
        spdlog::info("count= {}", count);
        for (int i = 0; i < count; i++) {
            if ((i + 1) * MAX_SEND > iLen) {
                len = iLen - i * MAX_SEND;
            } else {
                len = MAX_SEND;
            }
            spdlog::info("no. {}: offset: {} len: {}", i, offset, len);
            const unsigned char *data = pcData + offset;
            for (int j = 0; j < len; j++) {
                spdlog::info("{:02x} ", data[j]);
            }
            iRet = write(iFd, data, len);
            if (iRet < 0) {
                close(iFd);
                m_rs485 = -1;
                spdlog::error("error write {} , len: {}", iFd, len);
            } else {
                spdlog::info("no. {}: write len: {} success, iRet: {}", i, len, iRet);
            }

            offset += MAX_SEND;
        }
        set_receive_dir();
        return iRet;
    }

    static int get_rs485_state() {
        return m_rs485 != -1;
    }

    static int _uart_work(const char *buf, int len) {
        if (m_rs485 <= 0) {
            int fd = _uart_open();
            if (fd < 0) {
                spdlog::warn("failed to open ttyS{} to read write.", m_rsTty);
                return 0;
            }
            m_rs485 = fd;
        }
        _uart_write(buf, len);
        return 1;
    }

    static void uart_fcntl_set(int flag) {
        fcntl(m_rs485, F_SETFL, flag);
    }

    static void uart_close() {
        close(m_rs485);
        spdlog::info("m_rs485 close fd: {}", m_rs485);
        m_rs485 = -1;
    }

    static int SendTrue() {
        std::string res = "01 E1";
        return _uart_write(res.c_str(), res.length());
    }

    static int SendFast(const std::string &err_code) {
        std::string buf = "01 " + err_code;
        return _uart_write(buf.c_str(), buf.length());
    }
};