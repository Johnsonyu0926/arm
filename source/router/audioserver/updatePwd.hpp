#pragma once

#include "json.hpp"
#include "audiocfg.hpp"
#include "utils.h"
#include "spdlog/spdlog.h"
#include "Constants.hpp"
#include <regex>

namespace asns {

    class CUpdateResult {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CUpdateResult, cmd, resultId, msg)

        void do_success() {
            cmd = "UpdatePwd";
            resultId = 1;
            msg = "update password success";
        }

        void do_fail(const std::string& reason) {
            cmd = "UpdatePwd";
            resultId = 2;
            msg = "update password fail: " + reason;
        }

    private:
        std::string cmd;
        int resultId{0};
        std::string msg;
    };

    class CUpdate {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CUpdate, cmd, password, oldPassword)

        int do_req(CSocket *pClient) {
            CAudioCfgBusiness cfg;
            cfg.load();
            CUpdateResult updateResult;

            if (oldPassword != cfg.business[0].serverPassword) {
                updateResult.do_fail("old password does not match");
                spdlog::error("Update password failed: old password does not match");
            } else if (!is_password_strong(password)) {
                updateResult.do_fail("password must be at least 8 characters long and contain both letters and numbers");
                spdlog::error("Update password failed: password does not meet strength requirements");
            } else {
                try {
                    CUtils utils;
                    utils.change_password(password.c_str());

                    cfg.business[0].serverPassword = password;
                    cfg.saveToJson();
                    updateResult.do_success();
                    spdlog::info("Update password success");
                } catch (const std::exception& e) {
                    updateResult.do_fail(e.what());
                    spdlog::error("Update password failed: {}", e.what());
                }
            }
            return send_response(pClient, updateResult);
        }

    private:
        std::string cmd;
        std::string password;
        std::string oldPassword;

        int send_response(CSocket *pClient, const CUpdateResult& result) {
            json resp = result;
            std::string str = resp.dump();
            return pClient->Send(str.c_str(), str.length());
        }

        bool is_password_strong(const std::string& password) {
            if (password.length() < 8) {
                return false;
            }
            bool has_letter = std::regex_search(password, std::regex("[A-Za-z]"));
            bool has_digit = std::regex_search(password, std::regex("\\d"));
            return has_letter && has_digit;
        }
    };

} // namespace asns