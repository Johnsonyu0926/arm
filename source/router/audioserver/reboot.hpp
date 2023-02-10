#pragma once

#include "json.hpp"
#include "utils.h"

namespace asns {

    class CRebootResult {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CRebootResult, cmd, resultId, msg)

        int do_success() {
            cmd = "Reboot";
            resultId = 1;
            msg = "reboot success";
        }

    private:
        std::string cmd;
        int resultId;
        std::string msg;
    };

    class CReboot {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CReboot, cmd)

        int do_req(CSocket *pClient) {
            CUtils utils;
            utils.reboot();
            CRebootResult rebootResult;
            rebootResult.do_success();
            json js = rebootResult;
            std::string s = js.dump();
            pClient->Send(s.c_str(), s.length());
        }

    private:
        std::string cmd;
    };

}