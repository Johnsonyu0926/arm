#pragma once

#include <string>
#include "json.hpp"
#include "audiocfg.hpp"
#include "utils.h"
#include "public.hpp"
#include "spdlog/spdlog.h"

namespace asns {

    class CMicRecordResult {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CMicRecordResult, cmd, resultId, msg)

        void do_success() {
            cmd = "MicRecord";
            resultId = 1;
            msg = "success";
        }

        void do_fail(const std::string& str) {
            cmd = "MicRecord";
            resultId = 2;
            msg = str;
        }

    private:
        std::string cmd;
        int resultId{0};
        std::string msg;
    };

    class CMicRecord {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CMicRecord, cmd, duration, uploadUrl)

        int do_req(CSocket *pClient) {
            CAudioCfgBusiness bus;
            bus.load();
            std::string imei = bus.business[0].deviceID;
            CUtils utils;

            if (utils.get_process_status("ffmpeg")) {
                return send_failure(pClient, "Currently recording");
            }

            int time = std::stoi(duration);
            std::string url = uploadUrl;
            utils.async_wait(1, 0, 0, [=] {
                std::string res = utils.record_upload(time, url, imei);
                spdlog::info("result: {}", res);
                if (res.find("true") != std::string::npos) {
                    send_success(pClient);
                } else {
                    send_failure(pClient, "post error");
                }
            });
            return 1;
        }

    private:
        std::string cmd;
        std::string duration;
        std::string uploadUrl;

        int send_success(CSocket *pClient) {
            CMicRecordResult result;
            result.do_success();
            return send_response(pClient, result);
        }

        int send_failure(CSocket *pClient, const std::string& msg) {
            CMicRecordResult result;
            result.do_fail(msg);
            return send_response(pClient, result);
        }

        int send_response(CSocket *pClient, const CMicRecordResult& result) {
            json js = result;
            std::string res = js.dump();
            return pClient->Send(res.c_str(), res.length());
        }
    };

    class CSThread;

} // namespace asns