#ifndef __CSERVERSET_H__
#define __CSERVERSET_H__

#include <iostream>
#include "json.hpp"
#include "audiocfg.hpp"
#include "utils.h"
#include "spdlog/spdlog.h"

namespace asns {

    class CServerSetResult {
    private:
        std::string cmd;
        int resultId{0};
        std::string msg;

    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CServerSetResult, cmd, resultId, msg)

        void do_success() {
            cmd = "ServerSet";
            resultId = 1;
            msg = "ServerSet handle success";
        }
    };

    class CServerSet {
    private:
        std::string cmd;
        std::string serverAddress;
        std::string serverPort;
        std::string deviceCode;
        std::string password;

    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CServerSet, cmd, serverAddress, serverPort, deviceCode, password)

        int do_req(CSocket *pClient) {
            CAudioCfgBusiness cfg;
            cfg.load();

            cfg.business[0].server = serverAddress;
            cfg.business[0].port = std::stoi(serverPort);
            cfg.business[0].deviceID = deviceCode;
            cfg.business[0].password = password;
            cfg.saveToJson();

            CUtils utils;
            if (utils.is_ros_platform()) {
                std::string buf;
                buf = "cm set_val sys server " + serverAddress;
                system(buf.c_str());
                buf = "cm set_val sys port " + serverPort;
                system(buf.c_str());
                buf = "cm set_val sys password " + password;
                system(buf.c_str());
                buf = "cm set_val sys deviceid " + deviceCode;
                system(buf.c_str());
            }
            spdlog::info("serverAddress: {}, serverPort: {}, deviceCode: {}, password: {}", serverAddress, serverPort, deviceCode, password);
            CServerSetResult res;
            res.do_success();
            json j = res;
            std::string s = j.dump();
            pClient->Send(s.c_str(), s.length());
            utils.cmd_system("asctrl.sh restart");
            return 1;
        }
    };

} // namespace asns

#endif // __CSERVERSET_H__