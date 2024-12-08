#pragma once

#include <string>
#include "json.hpp"
#include "audiocfg.hpp"
#include "utils.h"
#include "add_custom_audio_file.hpp"
#include "spdlog/spdlog.h"

namespace asns {
    class CAudioPlayResult {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CAudioPlayResult, cmd, resultId, msg)

        void do_success() {
            cmd = "AudioPlay";
            resultId = 1;
            msg = "play success";
        }

        void do_fail(const std::string& str) {
            cmd = "AudioPlay";
            resultId = 2;
            msg = str;
        }

    private:
        std::string cmd;
        int resultId{0};
        std::string msg;
    };

    class CAudioPlay {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CAudioPlay, cmd, audioName, playType, duration)

        int do_req(CSocket *pClient) {
            CAudioCfgBusiness cfg;
            if (cfg.load() != 0) {
                return send_failure(pClient, "Failed to load audio configuration");
            }

            CAudioPlayResult audioPlayResult;
            CUtils utils;

            if (CUtils::get_process_status("madplay")) {
                return send_failure(pClient, "Existing playback task");
            }

            CAddCustomAudioFileBusiness business;
            if (!business.exist(audioName) || !utils.find_dir_file_exists(cfg.getAudioFilePath(), audioName)) {
                return send_failure(pClient, "no file");
            }

            if (CUtils::get_process_status("ffplay")) {
                AudioPlayUtil::audio_stop();
            }

            switch (playType) {
                case 0:
                    AudioPlayUtil::audio_loop_play(cfg.getAudioFilePath() + audioName, ASYNC_START);
                    break;
                case 1:
                    if (duration < 1) {
                        return send_failure(pClient, "parameter cannot be less than 1");
                    }
                    AudioPlayUtil::audio_num_play(duration, cfg.getAudioFilePath() + audioName, ASYNC_START);
                    break;
                case 2:
                    if (duration < 1) {
                        return send_failure(pClient, "parameter cannot be less than 1");
                    }
                    AudioPlayUtil::audio_time_play(duration, cfg.getAudioFilePath() + audioName, ASYNC_START);
                    break;
                default:
                    return send_failure(pClient, "Invalid play type");
            }

            audioPlayResult.do_success();
            return send_response(pClient, audioPlayResult);
        }

    private:
        std::string cmd;
        std::string audioName;
        int playType{0};
        int duration{0};

        int send_failure(CSocket *pClient, const std::string& msg) {
            CAudioPlayResult audioPlayResult;
            audioPlayResult.do_fail(msg);
            return send_response(pClient, audioPlayResult);
        }

        int send_response(CSocket *pClient, const CAudioPlayResult& result) {
            json js = result;
            std::string str = js.dump();
            spdlog::info("return json: {}", str);
            return pClient->Send(str.c_str(), str.length());
        }
    };
} // namespace asns