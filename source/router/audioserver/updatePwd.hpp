#pragma once

#include "json.hpp"
#include "audiocfg.hpp"
#include "utils.h"

namespace asns {
    class CUpdateResult {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CUpdateResult, cmd, resultId, msg)

        int do_souccess() {
            cmd = "UpdatePwd";
            resultId = 1;
            msg = "update password success";
        }

        int do_fail() {
            cmd = "UpdatePwd";
            resultId = 2;
            msg = "update password fail";
        }

    private:
        std::string cmd;
        int resultId;
        std::string msg;
    };

    class CUpdate {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CUpdate, cmd, password, oldPassword)

        int do_req(CSocket *pClient) {
            CAudioCfgBusiness cfg;
            cfg.load();
            if (oldPassword != cfg.business[0].serverPassword) {
                CUpdateResult updateResult;
                updateResult.do_fail();
                json resp = updateResult;
                std::string str = resp.dump();
                pClient->Send(str.c_str(), str.length());
                return 0;
            } else {
                CUtils utils;
                if (utils.is_ros_platform()) {
                    char buf[64] = {0};
                    sprintf(buf, "cm set_val sys serverpassword %s", password.c_str());
                    system(buf);
                }
                cfg.business[0].serverPassword = password;
                cfg.saveToJson();
                CUpdateResult updateResult;
                updateResult.do_souccess();
                json resp = updateResult;
                std::string str = resp.dump();
                pClient->Send(str.c_str(), str.length());
                return 1;
            }
        }

    private:
        std::string cmd;
        std::string password;
        std::string oldPassword;
    };
}