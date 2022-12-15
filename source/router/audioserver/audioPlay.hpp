#pragma once

#include "json.hpp"
#include "audiocfg.hpp"

namespace asns {
    class CAudioPlayResult {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CAudioPlayResult, cmd, resultId, msg)

        int do_success() {
            cmd = "AudioPlay";
            resultId = 1;
            msg = "触发播放成功";
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
            char command[256] = {0};
            switch (playType) {
                case 0:
                    sprintf(command, "madplay %s%s -r &", cfg.getAudioFilePath().c_str(),
                            audioName.c_str());
                    break;
                case 1: {
                    std::string cmd = "madplay " + cfg.getAudioFilePath() + ' ';
                    for (int i = 0; i < duration; ++i) {
                        cmd += audioName + ' ';
                    }
                    cmd += "&";
                    std::cout << "cmd: " << cmd << std::endl;
                    system(cmd.c_str());
                    break;
                }
                case 2: {
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

            CAudioPlayResult audioPlayResult;
            audioPlayResult.do_success();
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