// Rs485Manage.hpp
#pragma once

#include <string>
#include <string_view>
#include "json.hpp"
#include "utils.h"
#include "Rs485.hpp"
#include "CSocket.hpp"

namespace asns {

class CRs485ManageResult {
public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CRs485ManageResult, cmd, resultId, msg)

    void do_fail(std::string_view str) noexcept {
        cmd = "Rs485Manage";
        resultId = 2;
        msg = str;
    }

    void do_success() noexcept {
        cmd = "Rs485Manage";
        resultId = 1;
        msg = "Rs485Manage success";
    }

private:
    std::string cmd;
    int resultId{};
    std::string msg;
};

class CRs485Manage {
public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CRs485Manage, cmd, rs485Config)

    [[nodiscard]] int do_req(CSocket* pClient) const {
        CUtils utils;
        CRs485ManageResult res;

        if (rs485Config.empty()) {
            res.do_fail("rs485Config is empty");
        } else {
            std::string str = utils.hex_to_string(rs485Config);
            if (Rs485::_uart_work(str.c_str(), str.length()) != 1) {
                res.do_fail("rs485Config write fail");
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
    std::string rs485Config;
};

} // namespace asns

//BY GST ARMV8 GCC 13.2