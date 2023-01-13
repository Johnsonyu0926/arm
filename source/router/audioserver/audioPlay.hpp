#pragma once

#include "json.hpp"
#include "audiocfg.hpp"
#include "utils.h"
#include "add_custom_audio_file.hpp"
#include "Singleton.hpp"

namespace asns {
    class CAudioPlayResult {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CAudioPlayResult, cmd, resultId, msg)

        int do_success() {
            cmd = "AudioPlay";
            resultId = 1;
            msg = "play success";
        }

        int do_fail(std::string str) {
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

        int do_req(CSocket *pClient) {
            CAudioCfgBusiness cfg;
            cfg.load();
            CAudioPlayResult audioPlayResult;
            CUtils utils;
            if (utils.get_process_status("madplay")) {
                audioPlayResult.do_fail("Existing playback task");
                json js = audioPlayResult;
                std::string str = js.dump();
                pClient->Send(str.c_str(), str.length());
                return 1;
            }
            CAddCustomAudioFileBusiness business;
            if (business.exist(audioName) && utils.find_dir_file_exists(cfg.getAudioFilePath(), audioName) == false) {
                audioPlayResult.do_fail("no file");
            } else if (business.exist(audioName) == false &&
                       utils.find_dir_file_exists(cfg.getAudioFilePath(), audioName)) {
                audioPlayResult.do_fail("no file");
            } else if (business.exist(audioName) == false &&
                       utils.find_dir_file_exists(cfg.getAudioFilePath(), audioName) == false) {
                audioPlayResult.do_fail("no file");
            } else if (business.exist(audioName) && utils.find_dir_file_exists(cfg.getAudioFilePath(), audioName)) {
                if (utils.get_process_status("aplay")) {
                    Singleton::getInstance().setStatus(0);
                    system("killall -9 aplay");
                }
                char command[256] = {0};
                switch (playType) {
                    case 0:
                        sprintf(command, "madplay %s%s -r &", cfg.getAudioFilePath().c_str(),
                                audioName.c_str());
                        break;
                    case 1: {
                        if (duration < 1) {
                            audioPlayResult.do_fail("parameter cannot be less than 1");
                            break;
                        }
                        std::string cmd = "madplay ";
                        for (int i = 0; i < duration; ++i) {
                            cmd += cfg.getAudioFilePath() + audioName + ' ';
                        }
                        cmd += "&";
                        std::cout << "cmd: " << cmd << std::endl;
                        system(cmd.c_str());
                        break;
                    }
                    case 2: {
                        if (duration < 1) {
                            audioPlayResult.do_fail("parameter cannot be less than 1");
                            break;
                        }
                        int d = duration / (3600 * 24);
                        int t = duration % (3600 * 24) / 3600;
                        int m = duration % (3600 * 24) % 3600 / 60;
                        int s = duration % (3600 * 24) % 3600 % 60;
                        char buf[64] = {0};
                        sprintf(buf, "%d:%d:%d:%d", d, t, m, s);
                        sprintf(command, "madplay %s%s -r -t %s &", cfg.getAudioFilePath().c_str(),
                                audioName.c_str(), buf);
                        break;
                    }
                }
                std::cout << "command: " << command << std::endl;
                system(command);
            }

            json js = audioPlayResult;
            std::string str = js.dump();
            pClient->Send(str.c_str(), str.length());
        }

    private:
        std::string cmd;
        std::string audioName;
        int playType;
        int duration;
    };
}