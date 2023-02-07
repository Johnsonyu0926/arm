#pragma once

#include "json.hpp"
#include "utils.h"
#include "Singleton.hpp"
#include "public.hpp"
#include <thread>
/**
 * {
        "content":"[v10]广大市民您好，预计1分钟后地震将到达日本，请做好紧急避险措施！",
        "playCount":1, // 播放次数，单位次；当timeType为次数播放时生效
        "playDuration":60, // 播放时长，单位秒，当timeType为时长播放时生效
        "playStatus":1, // 播放状态，即1代表音频播放，详细参照"播放状态枚举"
        "playType":2, // 播放模式：0-音频文件，1-文字播报，2-录音文件播放
        "priority":6, // 播放优先级
        "timeType":0 // 0-时长播放；1-次数播放
    }
 */
namespace asns {
    template<typename Quest, typename Result>
    class CReQuest;
    template<typename T>
    class CResult;
    class CTtsPlayResultData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CTtsPlayResultData, null)

        template<typename Quest, typename Result,typename T>
        void do_success(const CReQuest<Quest, Result> &c, CResult<T> &r) {}

    private:
        std::nullptr_t null;
    };

    class CTtsPlayData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CTtsPlayData, content, playCount, playDuration, playStatus, playType, priority,
                                       timeType)

        int do_req() {
            CUtils utils;
            if (utils.get_process_status("madplay") || utils.get_process_status("aplay")) {
                return 5;
            }
            std::string txt = utils.hex_to_string(content);
            std::string cmd = "tts -t " + txt + " -f /tmp/output.pcm";
            system(cmd.c_str());
            system("ffmpeg -f s16le -ar 16000 -ac 1 -i /tmp/output.pcm /tmp/output.wav");
            utils.volume_gain(asns::TTS_PATH, "wav");
            switch (timeType) {
                case 0: {
                    Singleton::getInstance().setStatus(1);
                    std::thread([&] {
                        while (Singleton::getInstance().getStatus()) {
                            system("aplay /tmp/output.wav");
                        }
                    }).detach();
                    break;
                }
                case 1: {
                    if (playCount < 1) {
                        return 2;
                    }
                    std::string cmd = "aplay ";
                    for (int i = 0; i < playCount; ++i) {
                        cmd += "/tmp/output.wav ";
                    }
                    cmd += "&";
                    std::cout << "cmd: " << cmd << std::endl;
                    system(cmd.c_str());
                    break;
                }
                case 2: {
                    if (playDuration < 1) {
                        return 2;
                    }
                    Singleton::getInstance().setStatus(1);
                    utils.async_wait(1,playDuration,0,[&]{
                        Singleton::getInstance().setStatus(0);
                        system("killall -9 aplay");
                    });
                    std::thread([&] {
                        while (Singleton::getInstance().getStatus()) {
                            system("aplay /tmp/output.wav");
                        }
                    }).detach();
                    break;
                }
                default:
                    return 2;
            }
            return 1;
        }

    private:
        std::string content;
        int playCount;
        int playDuration;
        int playStatus;
        int playType;
        int priority;
        int timeType;
    };
}