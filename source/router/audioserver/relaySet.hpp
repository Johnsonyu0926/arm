#pragma once

#include <string>
#include "json.hpp"
#include "utils.h"
#include "Relay.hpp"
#include "spdlog/spdlog.h"

namespace asns {

    class CRelaySetResult {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CRelaySetResult, cmd, resultId, msg)

        void do_success() {
            cmd = "RelaySet";
            resultId = 1;
            msg = "RelaySet success";
        }

        void do_fail(const std::string& str) {
            cmd = "RelaySet";
            resultId = 2;
            msg = str;
        }

    private:
        std::string cmd;
        int resultId{0};
        std::string msg;
    };

    class CRelaySet {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CRelaySet, cmd, model, status)

        int do_req(CSocket *pClient) {
            try {
                Relay::getInstance().set_gpio_model(model, status);
                CRelaySetResult relaySetResult;
                relaySetResult.do_success();
                json js = relaySetResult;
                std::string s = js.dump();
                spdlog::info("RelaySet success: {}", s);
                return pClient->Send(s.c_str(), s.length());
            } catch (const std::exception& e) {
                CRelaySetResult relaySetResult;
                relaySetResult.do_fail(e.what());
                json js = relaySetResult;
                std::string s = js.dump();
                spdlog::error("RelaySet failed: {}", s);
                return pClient->Send(s.c_str(), s.length());
            }
        }

    private:
        std::string cmd;
        int model{0};
        int status{0};
    };
} // namespace asns