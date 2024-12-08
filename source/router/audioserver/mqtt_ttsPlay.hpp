#pragma once

#include <string>
#include "json.hpp"
#include "utils.h"
#include "public.hpp"
#include <thread>

/**
 * {
        "content":"[v10]广大市民您好，预�?分钟后地震将到达日本，请做好紧急避险措施！",
        "playCount":1, // 播放次数，单位次；当timeType为次数播放时生效
        "playDuration":60, // 播放时长，单位秒，当timeType为时长播放时生效
        "playStatus":1, // 播放状态，�?代表音频播放，详细参�?播放状态枚�?
        "playType":2, // 播放模式�?-音频文件�?-文字播报�?-录音文件播放
        "priority":6, // 播放优先�?
        "timeType":0 // 0-时长播放�?-次数播放
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

        template<typename Quest, typename Result, typename T>
        int do_success(const CReQuest<Quest, Result> &c, CResult<T> &r) {
            if (CUtils::get_process_status("madplay") || CUtils::get_process_status("ffplay") || PlayStatus::getInstance().getPlayState()) {
                r.resultId = 2;
                r.result = "Already played";
                return 2;
            }
            switch (c.data.timeType) {
                case 1: {
                    if (c.data.playCount < 1) {
                        r.resultId = 2;
                        r.result = "play count error";
                        return 2;
                    }
                    AcquisitionNoise::getInstance().noise_auto_set_volume();
                    AcquisitionNoise::getInstance().noiseDeque.clear();
                    PlayStatus::getInstance().setMqttPlayStatus(c.data.playStatus);
                    AudioPlayUtil::tts_num_play(c.data.playCount, c.data.content, ASYNC_START);
                    break;
                }
                case 0: {
                    if (c.data.playDuration < 1) {
                        r.resultId = 2;
                        r.result = "play time error";
                        return 2;
                    }
                    AcquisitionNoise::getInstance().noise_auto_set_volume();
                    AcquisitionNoise::getInstance().noiseDeque.clear();
                    PlayStatus::getInstance().setMqttPlayStatus(c.data.playStatus);
                    AudioPlayUtil::tts_time_play(c.data.playDuration, c.data.content, ASYNC_START);
                    break;
                }
                default:
                    r.resultId = 2;
                    r.result = "play error";
                    return 2;
            }
            r.resultId = 1;
            r.result = "success";
            return 1;
        }

    private:
        std::nullptr_t null{nullptr};
    };

    class CTtsPlayData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CTtsPlayData, content, playCount, playDuration, playStatus, playType, priority, timeType)

        std::string content;
        int playCount{0};
        int playDuration{0};
        int playStatus{0};
        int playType{0};
        int priority{0};
        int timeType{0};
    };
}