#pragma once

#include "json.hpp"
#include "utils.h"
#include "Singleton.hpp"
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
                std::string cmd = "tts -t " + txt + " -p " + std::to_string(speed) + " -f /tmp/output.pcm";
                system(cmd.c_str());
                switch (playType) {
                    case 0:
                        Singleton::getInstance().setStatus(1);
                        std::thread([&] {
                            while (Singleton::getInstance().getStatus()) {
                                system("aplay -t raw -c 1 -f S16_LE -r 16000 /tmp/output.pcm");
                            }
                        }).detach();
                        break;
                    case 1: {
                        if (duration < 1) {
                            ttsPlayResult.do_fail("parameter cannot be less than 1");
                            break;
                        }
                        std::string cmd = "aplay -t raw -c 1 -f S16_LE -r 16000 ";
                        for (int i = 0; i < duration; ++i) {
                            cmd += "/tmp/output.pcm ";
                        }
                        cmd += "&";
                        std::cout << "cmd: " << cmd << std::endl;
                        system(cmd.c_str());
                        break;
                    }
                    case 2: {
                        if (duration < 1) {
                            ttsPlayResult.do_fail("parameter cannot be less than 1");
                            break;
                        }
                        Singleton::getInstance().setStatus(1);
                        std::thread([&] {
                            utils.start_timer(duration, [&] {
                                Singleton::getInstance().setStatus(0);
                                system("killall -9 aplay");
                            });
                        }).detach();
                        std::thread([&] {
                            while (Singleton::getInstance().getStatus()) {
                                system("aplay -t raw -c 1 -f S16_LE -r 16000 /tmp/output.pcm");
                            }
                        }).detach();
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