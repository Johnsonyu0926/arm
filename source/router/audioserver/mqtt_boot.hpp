#pragma once

#include <string>
#include "json.hpp"
#include "audiocfg.hpp"
#include "volume.hpp"
#include "utils.h"

namespace asns {

    class CBootData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CBootData, sdcardSpace, flashSpace, iotVersion, version, volume, v5,
                                                    v12, v24, volt, storageType, imei, topic, cmd, hardwareModelId,
                                                    hardwareVersion, ipAddress, baud)

        void do_success() {
            CUtils utils;
            CAudioCfgBusiness cfg;
            cfg.load();
            CVolumeSet volumeSet;
            volumeSet.load();
            sdcardSpace = 16225000;
            flashSpace = utils.get_available_Disk("/mnt/");
            iotVersion = "COMMON";
            version = cfg.business[0].codeVersion;
            volume = volumeSet.getVolume();
            v5 = 0;
            v12 = 0;
            v24 = 0;
            volt = 971;
            storageType = 1;
            imei = cfg.business[0].serial;
            topic = asns::REQUEST_TOPIC + imei;
            cmd = "start";
            hardwareModelId = 2;
            hardwareVersion = utils.is_ros_platform() ? "7621" : "7688";
            ipAddress = utils.get_addr();
            baud = cfg.business[0].iBdVal;
        }

    private:
        long sdcardSpace{0};
        long flashSpace{0};
        std::string iotVersion;
        std::string version;
        int volume{0};
        int v5{0};
        int v12{0};
        int v24{0};
        int volt{0};
        int storageType{0};
        std::string imei;
        std::string topic;
        std::string cmd;
        int hardwareModelId{0};
        std::string hardwareVersion;
        std::string ipAddress;
        int baud{0};
    };
}