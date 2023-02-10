#pragma once

#include "json.hpp"
#include "utils.h"
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
            utils.txt_to_audio(content);
            switch (timeType) {
                case 0: {
                   utils.tts_loop_play(ASYNC_START);
                    break;
                }
                case 1: {
                    utils.tts_num_play(playCount,ASYNC_START);
                    break;
                }
                case 2: {
                    if (playDuration < 1) {
                        return 2;
                    }
                    utils.tts_time_play(playDuration,ASYNC_START);
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