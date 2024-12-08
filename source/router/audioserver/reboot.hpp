#pragma once

#include <string>
#include "json.hpp"
#include "utils.h"
#include "spdlog/spdlog.h"

namespace asns {

    class CRebootResult {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CRebootResult, cmd, resultId, msg)

        void do_success() {
            cmd = "Reboot";
            resultId = 1;
            msg = "reboot success";
        }

    private:
        std::string cmd;
        int resultId{0};
        std::string msg;
    };

    class CReboot {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CReboot, cmd)

        int do_req(CSocket *pClient) {
            try {
                CUtils utils;
                utils.reboot();
                CRebootResult rebootResult;
                rebootResult.do_success();
                spdlog::info("Reboot success");
                return send_response(pClient, rebootResult);
            } catch (const std::exception& e) {
                spdlog::error("Reboot failed: {}", e.what());
                return send_error_response(pClient, e.what());
            }
        }

    private:
        std::string cmd;

        int send_response(CSocket *pClient, const CRebootResult& result) {
            json js = result;
            std::string s = js.dump();
            return pClient->Send(s.c_str(), s.length());
        }

        int send_error_response(CSocket *pClient, const std::string& error_msg) {
            CRebootResult rebootResult;
            rebootResult.cmd = "Reboot";
            rebootResult.resultId = 2;
            rebootResult.msg = error_msg;
            json js = rebootResult;
            std::string s = js.dump();
            return pClient->Send(s.c_str(), s.length());
        }
    };

} // namespace asns