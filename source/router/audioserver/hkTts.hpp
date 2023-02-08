#pragma once

#include <iostream>
#include "json.hpp"
#include "utils.h"
#include "public.hpp"

using json = nlohmann::json;

namespace asns {
    class CStartTTSAudioData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CStartTTSAudioData, indexCode, command, TTSContent, audioLevel, audioOutID, ttscontent)

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

        int do_success() {
            status = 200;
            code = "0x00000000";
            errorMsg = "";
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
                data = json::parse(js.at("data").get<std::string>());
            } catch (json::parse_error &ex) {
                std::cerr << "parse error at byte " << ex.byte << std::endl;
                return 0;
            }
            CUtils utils;
            utils.txt_to_audio(data.ttscontent);
            utils.tts_num_play(1,ASYNC_START);

            CStartTTSAudioResult result;
            result.do_success();
            json s = result;
            return s.dump();
        }

    private:
        CStartTTSAudioData data;
    };
}