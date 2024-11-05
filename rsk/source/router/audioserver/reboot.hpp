// Reboot.hpp
#pragma once

#include <string>
#include <string_view>
#include "json.hpp"
#include "utils.h"
#include "CSocket.hpp"

namespace asns {

class CRebootResult {
public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CRebootResult, cmd, resultId, msg)

    void do_fail(std::string_view str) noexcept {
        cmd = "Reboot";
        resultId = 2;
        msg = str;
    }

    void do_success() noexcept {
        cmd = "Reboot";
        resultId = 1;
        msg = "Reboot success";
    }

private:
    std::string cmd;
    int resultId{};
    std::string msg;
};

class CReboot {
public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CReboot, cmd)

    [[nodiscard]] int do_req(CSocket* pClient) const {
        CUtils utils;
        CRebootResult res;
        res.do_success();
        
        const auto j = nlohmann::json(res);
        const auto s = j.dump();
        
        int sendResult = pClient->Send(s.data(), static_cast<int>(s.length()));
        
        if (sendResult > 0) {
            utils.exec("reboot");
        }
        
        return sendResult;
    }

private:
    std::string cmd;
};

} // namespace asns

//BY GST ARMV8 GCC 13.2