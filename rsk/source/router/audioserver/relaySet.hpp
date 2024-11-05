// RelaySet.hpp
#pragma once

#include <string>
#include <string_view>
#include "json.hpp"
#include "utils.h"
#include "Rs485.hpp"
#include "CSocket.hpp"

namespace asns {

class CRelaySetResult {
public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CRelaySetResult, cmd, resultId, msg)

    void do_fail(std::string_view str) noexcept {
        cmd = "RelaySet";
        resultId = 2;
        msg = str;
    }

    void do_success() noexcept {
        cmd = "RelaySet";
        resultId = 1;
        msg = "RelaySet success";
    }

private:
    std::string cmd;
    int resultId{};
    std::string msg;
};

class CRelaySet {
public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CRelaySet, cmd, relayState)

    [[nodiscard]] int do_req(CSocket* pClient) const {
        CUtils utils;
        CRelaySetResult res;

        if (relayState.empty()) {
            res.do_fail("relayState is empty");
        } else {
            std::string str = utils.hex_to_string(relayState);
            if (Rs485::_uart_work(str.c_str(), str.length()) != 1) {
                res.do_fail("relayState write fail");
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
    std::string relayState;
};

} // namespace asns

//BY GST ARMV8 GCC 13.2