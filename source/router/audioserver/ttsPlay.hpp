#pragma once

#include "json.hpp"

namespace asns {
    class CTtsPlayResult {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CTtsPlayResult, cmd, resultId, msg)

        int do_success() {
            cmd = "TtsPlay";
            resultId = 1;
            msg = "触发播放成功";
        }

    private:
        std::string cmd;
        int resultId;
        std::string msg;
    };

    class CTtsPlay {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CTtsPlay, cmd, content, playType, duration, voiceType, speed)

        int do_req(CSocket *pClient) {
            CTtsPlayResult ttsPlayResult;
            ttsPlayResult.do_success();
            json js = ttsPlayResult;
            std::string str = js.dump();
            pClient->Send(str.c_str(), str.length());
        }

    private:
        std::string cmd;
        std::string content;
        int playType;
        int duration;
        int voiceType;
        int speed;
    };
}