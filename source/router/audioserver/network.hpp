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
            if (utils.is_ros_platform()) {
                char cm[128] = {0};
                CNetworkSetResult res;
                res.do_success();
                json j = res;
                std::string s = j.dump();
                pClient->Send(s.c_str(), s.length());
                sprintf(cm, "cm set_val WAN1 gateway %s", gateway.c_str());
                std::cout << cm << std::endl;
                system(cm);
                sprintf(cm, "cm set_val WAN1 ipaddress %s", ipAddress.c_str());
                std::cout << cm << std::endl;
                system(cm);
                sprintf(cm, "cm set_val WAN1 ipmask %s", netMask.c_str());
                std::cout << cm << std::endl;
                system(cm);
                sprintf(cm, "ifconfig eth0 inet %s netmask %s up", ipAddress.c_str(), netMask.c_str());
                std::cout << cm << std::endl;
                system(cm);
                sprintf(cm, "ip r add default via %s", gateway.c_str());
                std::cout << cm << std::endl;
            } else {
                char uci[128] = {0};
                sprintf(uci, "uci set network.lan.ipaddr=%s", ipAddress.c_str());
                system(uci);
                sprintf(uci, "uci set network.lan.gateway=%s", gateway.c_str());
                system(uci);
                sprintf(uci, "uci set network.lan.netmask=%s", netMask.c_str());
                system(uci);
                sprintf(uci, "uci commit network");
                system(uci);
                sprintf(uci, "/etc/init.d/network reload");
                CNetworkSetResult res;
                res.do_success();
                json j = res;
                std::string s = j.dump();
                pClient->Send(s.c_str(), s.length());
                system(uci);
            }
            return 1;
        }
    };

} // namespace tcpserver
#endif
