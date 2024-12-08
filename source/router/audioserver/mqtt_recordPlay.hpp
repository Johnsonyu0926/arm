#pragma once

#include <string>
#include "json.hpp"
#include "public.hpp"
#include "utils.h"
#include "PlayStatus.hpp"
#include "AcquisitionNoise.hpp"
#include "AudioPlayUtil.hpp"

/**
 * {
        "downloadUrl":"http://172.16.1.20:9999/data/cms/record/e2567a91-8271-4011-905a-6f1d94845812.mp3", // 录音文件下载路径
        "playCount":1,
        "playType":3,
        "storageType":1,
        "timeType":1
    }
 */

namespace asns {

    template<typename Quest, typename Result>
    class CReQuest;

    template<typename T>
    class CResult;

    class CRecordPlayResultData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CRecordPlayResultData, null)

        template<typename Quest, typename Result, typename T>
        int do_success(const CReQuest<Quest, Result> &c, CResult<T> &r) {
            CUtils utils;
            if (CUtils::get_process_status("madplay") || CUtils::get_process_status("ffplay") || PlayStatus::getInstance().getPlayState()) {
                r.resultId = 2;
                r.result = "Already played";
                return 2;
            }
            std::string path = "/tmp/";
            std::string name = "record.mp3";
            std::string res = utils.get_upload_result(c.data.downloadUrl.c_str(), path.c_str(), name.c_str());
            if (res != "success") {
                r.resultId = 2;
                r.result = res;
                return 2;
            } else {
                utils.bit_rate_32_to_48(path + name);
                switch (c.data.timeType) {
                    case 0: {
                        if (c.data.playCount < 1) {
                            r.resultId = 2;
                            r.result = "play time error";
                            return 2;
                        }
                        AcquisitionNoise::getInstance().noise_auto_set_volume();
                        AcquisitionNoise::getInstance().noiseDeque.clear();
                        PlayStatus::getInstance().setMqttPlayStatus(c.data.playStatus);
                        AudioPlayUtil::ffplay_audio_time_play(c.data.playCount, path + name, ASYNC_START);
                        break;
                    }
                    case 1: {
                        if (c.data.playCount < 1) {
                            r.resultId = 2;
                            r.result = "play count error";
                            return 2;
                        }
                        AcquisitionNoise::getInstance().noise_auto_set_volume();
                        AcquisitionNoise::getInstance().noiseDeque.clear();
                        PlayStatus::getInstance().setMqttPlayStatus(c.data.playStatus);
                        AudioPlayUtil::ffplay_audio_num_play(c.data.playCount, path + name, ASYNC_START);
                        break;
                    }
                    default: {
                        r.resultId = 2;
                        r.result = "invalid time type";
                        return 2;
                    }
                }
                r.resultId = 1;
                r.result = "success";
                return 1;
            }
        }

    private:
        std::nullptr_t null{nullptr};
    };

    class CRecordPlayData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CRecordPlayData, downloadUrl, playCount, playType, storageType, timeType, playStatus)

        std::string downloadUrl; // 音频文件下载路径
        int playCount{0}; // 播放次数，单位次；当timeType为次数播放时生效
        int playType{0}; // 播放模式：1-音频文件，2-文字播报，3-录音文件播放
        int storageType{0}; // 存储位置：1-内置Flash，2-外置存储, 可不填
        int timeType{0}; // 0-时长播放，1-次数播放
        int playStatus{0};
    };
}