#pragma once

#include "json.hpp"

namespace asns {

    class CRebootResult {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CRebootResult, cmd, resultId, msg)

        int do_success() {
            cmd = "Reboot";
            resultId = 1;
            msg = "触发重启成功";
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
            system("reboot");
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