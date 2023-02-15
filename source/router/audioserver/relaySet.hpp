#pragma once

#include "json.hpp"
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
            CUtils utils;
            utils.gpio_set(model, status);
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