#pragma once

#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include "json.hpp"
#include "mqtt_reTemp.hpp"
#include "utils.h"
#include "Rs485NoiseMange.hpp"
#include "rs485Manage.hpp"

using json = nlohmann::json;

namespace asns {

    template<typename Quest, typename Result>
    class CReQuest;

    template<typename T>
    class CResult;

    class CLedShowSetResultData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CLedShowSetResultData, ledShowRecordId, data)

        static void recover_state() {
            AcquisitionNoise::getInstance().setMonitorStatus(!AcquisitionNoise::getInstance().getMonitorStatus());
            if (AcquisitionNoise::getInstance().getMonitorStatus()) {
                CUtils::async_wait(1, 0, 0, [] {
                    Rs485NoiseMange::startCollectingNoise();
                });
            } else {
                CUtils::async_wait(1, 0, 0, [] {
                    RSBusinessManage rs;
                    rs.worker();
                });
            }
        }

        template<typename Quest, typename Result, typename T>
        int do_success(const CReQuest<Quest, Result> &c, CResult<T> &r) {
            if (c.data.ledCommand.empty()) {
                r.resultId = 2;
                r.result = "failed ledCommand size == 0";
                return 2;
            }

            CUtils utils;
            std::string str;
            for (const auto &i : c.data.ledCommand) {
                str += utils.hex_to_string(i.substr(2));
            }

            ledShowRecordId = c.data.ledShowRecordId;
            AcquisitionNoise::getInstance().setMonitorStatus(!AcquisitionNoise::getInstance().getMonitorStatus());
            while (Rs485::get_rs485_state()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            const int fd = Rs485::_uart_open();
            if (fd < 0) {
                r.resultId = 2;
                r.result = "failed to open ttyS";
                recover_state();
                return 2;
            }

            Rs485::_uart_write(str.c_str(), str.length());
            tcflush(fd, TCIOFLUSH);

            unsigned char buf[255] = {0};
            int read_count = Rs485::while_select(fd, buf, sizeof(buf), 3);
            Rs485::set_send_dir();
            LOG(INFO) << "read_count: " << read_count;

            if (read_count < 0) {
                r.resultId = 2;
                r.result = "no response";
                Rs485::uart_close();
                recover_state();
                return 2;
            }

            data = CUtils::ustr_to_hex(buf, read_count);
            LOG(INFO) << "buf: " << data;

            if (data.compare(0, led_check.length(), led_check) != 0) {
                r.resultId = 2;
                r.result = "Execution failure";
                Rs485::uart_close();
                recover_state();
                return 2;
            }

            Rs485::uart_close();
            recover_state();
            r.resultId = 1;
            r.result = "success";
            return 1;
        }

    private:
        long ledShowRecordId{0};
        std::string data;
    };

    class CLedShowSetData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CLedShowSetData, ledCommand, showContent, ledShowRecordId)

        std::vector<std::string> ledCommand;
        std::string showContent;
        long ledShowRecordId{0};
    };
}