// ServerSet.hpp
#pragma once

#include <string>
#include <string_view>
#include "json.hpp"
#include "utils.h"
#include "Rs485.hpp"
#include "CSocket.hpp"

namespace asns {

class CServerSetResult {
public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CServerSetResult, cmd, resultId, msg)

    void do_fail(std::string_view str) noexcept {
        cmd = "ServerSet";
        resultId = 2;
        msg = str;
    }

    void do_success() noexcept {
        cmd = "ServerSet";
        resultId = 1;
        msg = "ServerSet success";
    }

private:
    std::string cmd;
    int resultId{};
    std::string msg;
};

class CServerSet {
public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CServerSet, cmd, serverConfig)

    [[nodiscard]] int do_req(CSocket* pClient) const {
        CUtils utils;
        CServerSetResult res;

        if (serverConfig.empty()) {
            res.do_fail("serverConfig is empty");
        } else {
            std::string str = utils.hex_to_string(serverConfig);
            if (Rs485::_uart_work(str.c_str(), str.length()) != 1) {
                res.do_fail("serverConfig write fail");
            } else {
                res.do_success();
            }
        }

        const auto j = nlohmann::json(res);
        const auto s = j.dump();
        return pClient->Send(s.data(), static_cast<int>(s.length()));
    }

private:
    std::string cmd;
    std::string serverConfig;
};

} // namespace asns

//BY GST ARMV8 GCC 13.2