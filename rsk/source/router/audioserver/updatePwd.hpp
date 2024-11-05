// updatepwd.hpp
#pragma once

#include <string>
#include <string_view>
#include "json.hpp"
#include "audiocfg.hpp"
#include "utils.h"

namespace asns {

class CUpdateResult {
public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CUpdateResult, cmd, resultId, msg)

    void do_success() noexcept {
        cmd = "UpdatePwd";
        resultId = 1;
        msg = "update password success";
    }

    void do_fail() noexcept {
        cmd = "UpdatePwd";
        resultId = 2;
        msg = "update password fail";
    }

    [[nodiscard]] int get_result_id() const noexcept { return resultId; }

private:
    std::string cmd;
    int resultId{};
    std::string msg;
};

class CUpdate {
public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CUpdate, cmd, password, oldPassword)

    [[nodiscard]] int do_req(CSocket* pClient) const {
        CAudioCfgBusiness cfg;
        cfg.load();

        CUpdateResult updateResult;

        if (oldPassword != cfg.business[0].serverPassword) {
            updateResult.do_fail();
        } else {
            CUtils utils;
            utils.change_password(password);
            cfg.business[0].serverPassword = password;
            cfg.saveToJson();
            updateResult.do_success();
        }

        const auto resp = nlohmann::json(updateResult);
        const auto str = resp.dump();
        pClient->Send(str.data(), str.length());

        return (updateResult.get_result_id() == 1) ? 1 : 0;
    }

private:
    std::string cmd;
    std::string password;
    std::string oldPassword;
};

} // namespace asns

//BY GST ARMV8 GCC 13.2