#pragma once

#include "json.hpp"
#include "gpio.hpp"
#include "utils.h"

namespace asns {
    class CRelaySetResult {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CRelaySetResult, cmd, resultId, msg)

        int do_success() {
            cmd = "RelaySet";
            resultId = 1;
            msg = "RelaySet success";
        }

        int do_fail(std::string str) {
            cmd = "RelaySet";
            resultId = 2;
            msg = str;
        }

    private:
        std::string cmd;
        int resultId;
        std::string msg;
    };

    class CRelaySet {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CRelaySet, cmd, model, status)

        int do_req(CSocket *pClient) {
            CGpio::getInstance().setGpioModel(model);
            switch (model) {
                case 1:
                    if (status == 0) {
                        CGpio::getInstance().set_gpio_off();
                    } else if (status == 1) {
                        CGpio::getInstance().set_gpio_on();
                    }
                    break;
                case 2:
                    CUtils utils;
                    utils.async_wait(0, 0, 0, [&] {
                        if (utils.get_process_status("madplay") || utils.get_process_status("aplay")) {
                            CGpio::getInstance().set_gpio_on();
                        } else {
                            CGpio::getInstance().set_gpio_off();
                        }
                        if (CGpio::getInstance().getGpioModel() != 2)return;
                    });
                    break;
            }

            CRelaySetResult relaySetResult;
            relaySetResult.do_success();
            json js = relaySetResult;
            std::string s = js.dump();
            pClient->Send(s.c_str(), s.length());
        }

    private:
        std::string cmd;
        int model;
        int status;
    };
};