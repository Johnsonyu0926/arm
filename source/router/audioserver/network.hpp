#ifndef __CNETWORKSET_H__
#define __CNETWORKSET_H__

#include <iostream>
#include "json.hpp"
#include "utils.h"

using namespace std;

namespace asns {

    class CNetworkSetResult {
    private:
        string cmd;
        int resultId;
        string msg;

    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CNetworkSetResult, cmd, resultId, msg)

        int do_success() {
            cmd = "NetworkSet";
            resultId = 1;
            msg = "NetworkSet handle success";
        };
    };

    class CNetworkSet {
    private:
        string cmd;
        string ipAddress;
        string gateway;
        string netMask;

    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CNetworkSet, cmd, ipAddress, gateway, netMask)

        int do_req(CSocket *pClient) {
            CUtils utils;
            CNetworkSetResult res;
            res.do_success();
            json j = res;
            std::string s = j.dump();
            pClient->Send(s.c_str(), s.length());
            utils.network_set(gateway, ipAddress, netMask);
            return 1;
        }
    };

} // namespace tcpserver
#endif
