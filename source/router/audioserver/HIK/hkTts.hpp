#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "json.hpp"
#include "utils.h"
#include "public.hpp"

#define NON_PLAY_PRIORITY 100

extern int g_playing_priority;
using json = nlohmann::json;

namespace asns {
    class CStartTTSAudioData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CStartTTSAudioData, indexCode, command, TTSContent, audioLevel, audioOutID, ttscontent)

        std::string indexCode;
        std::string command;
        std::string TTSContent;
        int audioLevel{0};
        std::vector<int> audioOutID;
        std::string ttscontent;
    };

    class CStartTTSAudioResult {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CStartTTSAudioResult, status, code, errorMsg, data)

        void do_success(const std::string &msg) {
            status = 200;
            code = "0x00000000";
            errorMsg = msg;
            data = {};
        }

    private:
        int status{0};
        std::string code;
        std::string errorMsg;
        json data;
    };

    class CStartTTSAudio {
    public:
        std::string parse(const std::string &res) {
            try {
                json js = json::parse(res);
                data = js.get<CStartTTSAudioData>();
            } catch (json::parse_error &ex) {
                LOG(ERROR) << "parse error at byte " << ex.byte;
                return "";
            }
            LOG(INFO) << "g_playing_priority: " << g_playing_priority << " TTS level: " << data.audioLevel;
            if (g_playing_priority <= data.audioLevel) {
                return create_response("A high-priority play task has been created");
            }
            AudioPlayUtil::audio_stop();
            g_playing_priority = data.audioLevel;
            std::string txt = data.ttscontent;
            CUtils::async_wait(1, 0, 0, [txt] {
                LOG(INFO) << "hk tts: " << txt;
                AudioPlayUtil::tts_num_play(1, txt);
                g_playing_priority = NON_PLAY_PRIORITY;
            });
            return create_response("success");
        }

    private:
        CStartTTSAudioData data;

        std::string create_response(const std::string &msg) {
            CStartTTSAudioResult result;
            result.do_success(msg);
            json s = result;
            LOG(INFO) << "hk tts res: " << s.dump();
            return s.dump();
        }
    };
}