#pragma once

#include "json.hpp"
#include "utils.h"
#include "public.hpp"
#include <thread>

namespace asns {
    class CTtsPlayResult {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CTtsPlayResult, cmd, resultId, msg)

        int do_success() {
            cmd = "TtsPlay";
            resultId = 1;
            msg = "play success";
        }

        int do_fail(const std::string str) {
            cmd = "TtsPlay";
            resultId = 2;
            msg = str;
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
            CUtils utils;
            if (utils.get_process_status("madplay") || utils.get_process_status("aplay")) {
                ttsPlayResult.do_fail("Existing playback task");
            } else {
                std::string txt = utils.hex_to_string(content);
                utils.txt_to_audio(txt, speed);
                switch (playType) {
                    case 0:
                        utils.tts_loop_play(ASYNC_START);
                        break;
                    case 1: {
                        if (duration < 1) {
                            ttsPlayResult.do_fail("parameter cannot be less than 1");
                            break;
                        }
                        utils.tts_num_play(duration, ASYNC_START);
                        break;
                    }
                    case 2: {
                        if (duration < 1) {
                            ttsPlayResult.do_fail("parameter cannot be less than 1");
                            break;
                        }
                        utils.tts_time_play(duration, ASYNC_START);
                        break;
                    }
                }
            }
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