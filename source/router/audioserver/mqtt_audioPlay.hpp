#pragma once

#include <string>
#include "json.hpp"
#include "audiocfg.hpp"
#include "add_mqtt_custom_audio_file.hpp"
#include "utils.h"
#include "volume.hpp"

using json = nlohmann::json;

namespace asns {

    template<typename Quest, typename Result>
    class CReQuest;

    template<typename T>
    class CResult;

    class CAudioPlayResultData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CAudioPlayResultData, null)

        template<typename Quest, typename Result, typename T>
        int do_success(const CReQuest<Quest, Result> &c, CResult<T> &r) {
            CUtils utils;
            if (CUtils::get_process_status("madplay") || CUtils::get_process_status("ffplay")) {
                r.resultId = 2;
                r.result = "Already played";
                return 2;
            }

            CAudioCfgBusiness cfg;
            cfg.load();

            CAddMqttCustomAudioFileBusiness business;
            if (business.exist(c.data.fileName) && !utils.find_dir_file_exists(cfg.getAudioFilePath(), c.data.fileName)) {
                business.deleteData(c.data.fileName);
                r.resultId = 2;
                r.result = "audio file does not exist";
                return 2;
            } else if (business.exist(c.data.fileName) && utils.find_dir_file_exists(cfg.getAudioFilePath(), c.data.fileName)) {
                switch (c.data.timeType) {
                    case 0:
                        if (c.data.playDuration < 1) {
                            r.resultId = 2;
                            r.result = "play time error";
                            return 2;
                        }
                        AcquisitionNoise::getInstance().noise_auto_set_volume();
                        AcquisitionNoise::getInstance().noiseDeque.clear();
                        PlayStatus::getInstance().setMqttPlayStatus(c.data.playStatus);
                        AudioPlayUtil::audio_time_play(c.data.playDuration, cfg.getAudioFilePath() + c.data.fileName, ASYNC_START);
                        break;
                    case 1:
                        if (c.data.playCount < 1) {
                            r.resultId = 2;
                            r.result = "play count error";
                            return 2;
                        }
                        AcquisitionNoise::getInstance().noise_auto_set_volume();
                        AcquisitionNoise::getInstance().noiseDeque.clear();
                        PlayStatus::getInstance().setMqttPlayStatus(c.data.playStatus);
                        AudioPlayUtil::audio_num_play(c.data.playCount, cfg.getAudioFilePath() + c.data.fileName, ASYNC_START);
                        break;
                    default:
                        r.resultId = 2;
                        r.result = "invalid time type";
                        return 2;
                }
            } else {
                r.resultId = 2;
                r.result = "audio file does not exist";
                return 2;
            }

            r.resultId = 1;
            r.result = "success";
            return 1;
        }

    public:
        std::nullptr_t null{nullptr};
    };

    class CAudioPlayData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CAudioPlayData, fileName, audioUploadRecordId, playCount, playDuration,
                                                    playStatus, playType, storageType, timeType)

        std::string fileName;
        int audioUploadRecordId{0};
        int playCount{0};
        int playDuration{0};
        int playStatus{0};
        int playType{0};
        int storageType{0};
        int timeType{0};
    };
}