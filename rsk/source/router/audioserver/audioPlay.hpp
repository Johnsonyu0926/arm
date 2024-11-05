// audioplay.hpp
#pragma once

#include <string>
#include <memory>
#include <nlohmann/json.hpp>
#include "audiocfg.hpp"
#include "utils.h"
#include "add_custom_audio_file.hpp"

namespace asns {

class CAudioPlayResult {
public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CAudioPlayResult, cmd, resultId, msg)

    void do_success() {
        cmd = "AudioPlay";
        resultId = 1;
        msg = "play success";
    }

    void do_fail(std::string_view str) {
        cmd = "AudioPlay";
        resultId = 2;
        msg = str;
    }

private:
    std::string cmd;
    int resultId;
    std::string msg;
};

class CAudioPlay {
public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CAudioPlay, cmd, audioName, playType, duration)

    [[nodiscard]] int do_req(std::shared_ptr<CSocket> pClient) {
        CAudioCfgBusiness cfg;
        cfg.load();
        CAudioPlayResult audioPlayResult;
        audioPlayResult.do_success();

        if (CUtils::get_process_status("madplay")) {
            audioPlayResult.do_fail("Existing playback task");
            return send_response(pClient, audioPlayResult);
        }

        CAddCustomAudioFileBusiness business;
        if (!file_exists(business, cfg)) {
            audioPlayResult.do_fail("no file");
        } else {
            if (CUtils::get_process_status("ffplay")) {
                AudioPlayUtil::audio_stop();
            }
            switch (playType) {
                case 0:
                    AudioPlayUtil::audio_loop_play(cfg.getAudioFilePath() + audioName, ASYNC_START);
                    break;
                case 1:
                case 2:
                    if (duration < 1) {
                        audioPlayResult.do_fail("parameter cannot be less than 1");
                    } else {
                        (playType == 1) ? 
                            AudioPlayUtil::audio_num_play(duration, cfg.getAudioFilePath() + audioName, ASYNC_START) :
                            AudioPlayUtil::audio_time_play(duration, cfg.getAudioFilePath() + audioName, ASYNC_START);
                    }
                    break;
                default:
                    audioPlayResult.do_fail("Invalid play type");
            }
        }
        return send_response(pClient, audioPlayResult);
    }

private:
    std::string cmd;
    std::string audioName;
    int playType;
    int duration;

    [[nodiscard]] bool file_exists(const CAddCustomAudioFileBusiness& business, const CAudioCfgBusiness& cfg) const {
        return (business.exist(audioName) && CUtils::find_dir_file_exists(cfg.getAudioFilePath(), audioName)) ||
               (!business.exist(audioName) && CUtils::find_dir_file_exists(cfg.getAudioFilePath(), audioName));
    }

    [[nodiscard]] int send_response(const std::shared_ptr<CSocket>& pClient, const CAudioPlayResult& audioPlayResult) const {
        nlohmann::json js = audioPlayResult;
        std::string str = js.dump();
        return pClient->Send(str.c_str(), str.length());
    }
};

} // namespace asns

//By GST ARMV8 GCC13.2 audioplay.hpp