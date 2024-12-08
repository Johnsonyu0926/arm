#pragma once

#include <string>
#include <thread>
#include <chrono>
#include "json.hpp"
#include "audiocfg.hpp"
#include "utils.h"
#include "AcquisitionNoise.hpp"
#include "Rs485.hpp"
#include "Rs485NoiseMange.hpp"
#include "RSBusinessManage.hpp"

namespace asns {

    template<typename Quest, typename Result>
    class CReQuest;

    template<typename T>
    class CResult;

    class CBaudResultData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CBaudResultData, null)

        template<typename Quest, typename Result, typename T>
        int do_success(const CReQuest<Quest, Result> &c, CResult<T> &r) {
            if (c.data.baud > 115200 || c.data.baud < 2400) {
                r.resultId = 2;
                r.result = "Baud rate error!";
                return 2;
            }

            CAudioCfgBusiness cfg;
            cfg.load();
            if (cfg.business[0].iBdVal != c.data.baud) {
                cfg.business[0].iBdVal = c.data.baud;
                cfg.saveToJson();
                CUtils::cmd_system("cm set_val sys bd " + std::to_string(c.data.baud));
                if (AcquisitionNoise::getInstance().getMonitorStatus()) {
                    AcquisitionNoise::getInstance().setMonitorStatus(0);
                    CUtils::async_wait(1, 0, 0, [&] {
                        while (Rs485::get_rs485_state()) {
                            std::this_thread::sleep_for(std::chrono::seconds(1));
                        }
                        AcquisitionNoise::getInstance().setMonitorStatus(1);
                        Rs485NoiseMange::startCollectingNoise();
                    });
                } else {
                    AcquisitionNoise::getInstance().setMonitorStatus(1);
                    CUtils::async_wait(1, 0, 0, [&] {
                        while (Rs485::get_rs485_state()) {
                            std::this_thread::sleep_for(std::chrono::seconds(1));
                        }
                        AcquisitionNoise::getInstance().setMonitorStatus(0);
                        RSBusinessManage rs;
                        rs.worker();
                    });
                }
            }
            r.resultId = 1;
            r.result = "success";
            return 1;
        }

    private:
        std::nullptr_t null{nullptr};
    };

    class CBaudData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CBaudData, baud)

        int baud{0};
    };
}