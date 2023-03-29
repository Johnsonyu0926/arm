#pragma once

#include <iostream>
#include "json.hpp"
#include "utils.h"
#include "public.hpp"

#define NON_PLAY_PRIORITY 100

extern int g_playing_priority;
using json = nlohmann::json;

namespace asns {
    class CStartTTSAudioData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CStartTTSAudioData, indexCode, command, TTSContent, audioLevel, audioOutID,
                                       ttscontent)

    public:
        std::string indexCode;
        std::string command;
        std::string TTSContent;
        int audioLevel;
        std::vector<int> audioOutID;
        std::string ttscontent;
    };

    class CStartTTSAudioResult {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CStartTTSAudioResult, status, code, errorMsg, data)

        int do_success(const std::string &msg) {
            status = 200;
            code = "0x00000000";
            errorMsg = msg;
            data = {};
        }

    private:
        int status;
        std::string code;
        std::string errorMsg;
        json data;
    };

    class CStartTTSAudio {
    public:
        std::string parse(const std::string &res) {
            try {
                json js = json::parse(res);
                data = js;
            } catch (json::parse_error &ex) {
                std::cerr << "parse error at byte " << ex.byte << std::endl;
                return 0;
            }
            DS_TRACE("g_playing_priority:" << g_playing_priority << "TTS level:" << data.audioLevel);
            if (g_playing_priority < data.audioLevel) {
                CStartTTSAudioResult result;
                result.do_success("A high-priority play task has been created");
                json s = result;
                DS_TRACE("hk tts res:" << s.dump().c_str());
                return s.dump();
            }
            CUtils utils;
            utils.audio_stop();
            g_playing_priority = data.audioLevel;
            std::string txt = data.ttscontent;
            utils.async_wait(1, 0, 0, [txt] {
                CUtils Util;
                DS_TRACE("hk tts:" << txt.c_str());
                Util.tts_num_play(1,txt);
            });
            g_playing_priority = NON_PLAY_PRIORITY;
            CStartTTSAudioResult result;
            result.do_success("success");
            json s = result;
            DS_TRACE("hk tts res:" << s.dump().c_str());
            return s.dump();
        }

    private:
        CStartTTSAudioData data;
    };
}