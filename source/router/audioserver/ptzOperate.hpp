#pragma once

#include <string>
#include "json.hpp"
#include "utils.h"
#include "Rs485.hpp"

namespace asns {

    class CPtzOperateResult {
    private:
        std::string cmd;
        int resultId{0};
        std::string msg;

    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CPtzOperateResult, cmd, resultId, msg)

        void do_fail(const std::string& str) {
            cmd = "PtzOperate";
            resultId = 2;
            msg = str;
        }

        void do_success() {
            cmd = "PtzOperate";
            resultId = 1;
            msg = "PtzOperate success";
        }
    };

    class CPtzOperate {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CPtzOperate, cmd, operateCmd)

        int do_req(CSocket *pClient) {
            CUtils utils;
            CPtzOperateResult res;
            if (operateCmd.empty()) {
                res.do_fail("operateCmd is empty");
            } else {
                std::string str = utils.hex_to_string(operateCmd);
                if (Rs485::_uart_work(str.c_str(), str.length()) != 1) {
                    res.do_fail("operateCmd write fail");
                } else {
                    res.do_success();
                }
            }
            json j = res;
            std::string s = j.dump();
            pClient->Send(s.c_str(), s.length());
            return 1;
        }

    private:
        std::string cmd;
        std::string operateCmd;
    };
}

// Rs485.hpp
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <cstring>

class Rs485 {
public:
    static int _uart_work(const char *data, size_t length) {
        // 打开串口设备文件
        int fd = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_SYNC);
        if (fd < 0) {
            return -1;
        }

        // 配置串口参数
        struct termios tty;
        memset(&tty, 0, sizeof tty);
        if (tcgetattr(fd, &tty) != 0) {
            close(fd);
            return -1;
        }

        cfsetospeed(&tty, B9600);
        cfsetispeed(&tty, B9600);

        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
        tty.c_iflag &= ~IGNBRK;         // disable break processing
        tty.c_lflag = 0;                // no signaling chars, no echo,
                                        // no canonical processing
        tty.c_oflag = 0;                // no remapping, no delays
        tty.c_cc[VMIN]  = 0;            // read doesn't block
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                        // enable reading
        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
        tty.c_cflag |= 0;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CRTSCTS;

        if (tcsetattr(fd, TCSANOW, &tty) != 0) {
            close(fd);
            return -1;
        }

        // 写入数据到串口设备
        write(fd, data, length);
        close(fd);
        return 1;
    }
};