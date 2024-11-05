// PtzOperate.hpp
#pragma once

#include <string>
#include <string_view>
#include "json.hpp"
#include "utils.h"
#include "Rs485.hpp"
#include "CSocket.hpp"

namespace asns {

class CPtzOperate {
public:
    class Result {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Result, cmd, resultId, msg)

        void do_fail(std::string_view str) noexcept {
            cmd = "PtzOperate";
            resultId = 2;
            msg = str;
        }

        void do_success() noexcept {
            cmd = "PtzOperate";
            resultId = 1;
            msg = "PtzOperate success";
        }

    private:
        std::string cmd;
        int resultId{};
        std::string msg;
    };

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CPtzOperate, cmd, operateCmd)

    [[nodiscard]] std::string_view do_req(CSocket* pClient) const {
        Result res;

        if (operateCmd.empty()) {
            res.do_fail("operateCmd is empty");
        } else {
            std::string str = CUtils::hex_to_string(operateCmd);
            if (Rs485::_uart_work(str.c_str(), str.length()) != 1) {
                res.do_fail("operateCmd write fail");
            } else {
                res.do_success();
            }
        }

        const auto j = nlohmann::json(res);
        const auto s = j.dump();
        if (pClient->Send(s.data(), static_cast<int>(s.length())) > 0) {
            return "Success";
        }
        return "Fail";
    }

private:
    std::string cmd;
    std::string_view operateCmd;
};

} // namespace asns

//BY GST ARMV8 GCC 13.2