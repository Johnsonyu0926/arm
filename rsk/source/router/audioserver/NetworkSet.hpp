// NetworkSet.hpp
#pragma once

#include <string>
#include <string_view>
#include <nlohmann/json.hpp>
#include "utils.h"
#include "CSocket.h"

namespace asns {

class CNetworkSetResult {
public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CNetworkSetResult, cmd, resultId, msg)

    void do_success() noexcept {
        cmd = "NetworkSet";
        resultId = 1;
        msg = "NetworkSet handle success";
    }

private:
    std::string cmd;
    int resultId{};
    std::string msg;
};

class CNetworkSet {
public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CNetworkSet, cmd, ipAddress, gateway, netMask)

    [[nodiscard]] int do_req(CSocket* pClient) const {
        CUtils utils;
        CNetworkSetResult res;
        res.do_success();
        
        const auto j = nlohmann::json(res);
        const auto s = j.dump();
        
        int sendResult = pClient->Send(s.data(), static_cast<int>(s.length()));
        
        if (sendResult > 0) {
            utils.network_set(gateway, ipAddress, netMask);
        }
        
        return sendResult;
    }

private:
    std::string cmd;
    std::string ipAddress;
    std::string gateway;
    std::string netMask;
};

}  // namespace asns

//BY GST ARMV8 GCC 13.2