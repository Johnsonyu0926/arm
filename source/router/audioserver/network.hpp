#ifndef __CNETWORKSET_H__
#define __CNETWORKSET_H__

#include <iostream>
#include "json.hpp"
#include "utils.h"
#include "spdlog/spdlog.h"

namespace asns {

    class CNetworkSetResult {
    private:
        std::string cmd;
        int resultId{0};
        std::string msg;

    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CNetworkSetResult, cmd, resultId, msg)

        void do_success() {
            cmd = "NetworkSet";
            resultId = 1;
            msg = "NetworkSet handle success";
        }

        void do_fail(const std::string& str) {
            cmd = "NetworkSet";
            resultId = 2;
            msg = str;
        }
    };

    class CNetworkSet {
    private:
        std::string cmd;
        std::string ipAddress;
        std::string gateway;
        std::string netMask;

    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CNetworkSet, cmd, ipAddress, gateway, netMask)

        int do_req(CSocket *pClient) {
            CUtils utils;
            CNetworkSetResult res;
            try {
                utils.network_set(gateway, ipAddress, netMask);
                res.do_success();
                spdlog::info("NetworkSet success: IP={}, Gateway={}, Netmask={}", ipAddress, gateway, netMask);
            } catch (const std::exception& e) {
                res.do_fail(e.what());
                spdlog::error("NetworkSet failed: {}", e.what());
            }
            return send_response(pClient, res);
        }

    private:
        int send_response(CSocket *pClient, const CNetworkSetResult& result) {
            json j = result;
            std::string s = j.dump();
            return pClient->Send(s.c_str(), s.length());
        }
    };

} // namespace asns

#endif // __CNETWORKSET_H__