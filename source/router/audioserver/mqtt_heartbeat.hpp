#pragma once

#include <string>
#include "audiocfg.hpp"
#include "volume.hpp"
#include "json.hpp"
#include "utils.h"
#include "VolumeAdaptSchemaSet.hpp"
#include "AudioPlayStatus.hpp"
#include "Relay.hpp"
#include "AcquisitionNoise.hpp"

namespace asns {

    class CHeartBeatData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CHeartBeatData, cmd, volume, imei, playStatus, sdcardSpace, flashSpace,
                                       storageType, v5, v12, v24, playContent, decibel, volumeAdaptSchema, baud, intervalTime)

        int do_success() {
            CAudioCfgBusiness cfg;
            cfg.load();
            CVolumeSet volumeSet;
            volumeSet.load();
            cmd = "hello";
            volume = volumeSet.getVolume();
            imei = cfg.business[0].serial;
            playStatus = PlayStatus::getInstance().getMqttPlayStatus() != -1 ? PlayStatus::getInstance().getMqttPlayStatus() : PlayStatus::getInstance().getPlayState();
            sdcardSpace = "7684500";
            flashSpace = std::to_string(CUtils::get_available_Disk("/mnt/"));
            storageType = 1;
            v5 = Relay::getInstance().getGpioStatus();
            v12 = 0;
            v24 = 0;
            playContent = PlayStatus::getInstance().getPlayConten();
            decibel = CUtils::fmt_float_to_str(AcquisitionNoise::getInstance().getDecibel());
            volumeAdaptSchema.do_data();
            baud = cfg.business[0].iBdVal;
            intervalTime = 10;
            return 1;
        }

    private:
        std::string cmd;
        int volume{0};
        std::string imei;
        int playStatus{0};
        std::string sdcardSpace;
        std::string flashSpace;
        int storageType{0};
        int v5{0};
        int v12{0};
        int v24{0};
        std::string playContent;
        std::string decibel;
        CVolumeAdaptSchemaData volumeAdaptSchema;
        int baud{0};
        int intervalTime{0};
    };
}