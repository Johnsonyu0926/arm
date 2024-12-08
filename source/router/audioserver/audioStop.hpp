#pragma once

#include <string>
#include "json.hpp"
#include "utils.h"
#include "AudioPlayUtil.hpp"
#include "spdlog/spdlog.h"

namespace asns {
    class CAudioStopResult {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CAudioStopResult, cmd, resultId, msg)

        void do_success() {
            cmd = "AudioStop";
            resultId = 1;
            msg = "stop play success";
        }

    private:
        std::string cmd;
        int resultId{0};
        std::string msg;
    };

    class CAudioStop {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CAudioStop, cmd)

        int do_req(CSocket *pClient) {
            try {
                AudioPlayUtil::audio_stop();
                CAudioStopResult audioStopResult;
                audioStopResult.do_success();
                spdlog::info("AudioStop success: {}", audioStopResult.msg);
                return send_response(pClient, audioStopResult);
            } catch (const std::exception& e) {
                spdlog::error("AudioStop failed: {}", e.what());
                return send_error_response(pClient, e.what());
            }
        }

    private:
        std::string cmd;

        int send_response(CSocket *pClient, const CAudioStopResult& result) {
            json js = result;
            std::string str = js.dump();
            return pClient->Send(str.c_str(), str.length());
        }

        int send_error_response(CSocket *pClient, const std::string& error_msg) {
            CAudioStopResult audioStopResult;
            audioStopResult.cmd = "AudioStop";
            audioStopResult.resultId = 2;
            audioStopResult.msg = error_msg;
            json js = audioStopResult;
            std::string str = js.dump();
            return pClient->Send(str.c_str(), str.length());
        }
    };
} // namespace asns