#pragma once

#include "audiocfg.hpp"
#include "volume.hpp"
#include "json.hpp"
#include "utils.h"
/**
 * {
        "cmd":"hello",
        "currentVol":"4",
        "imei":"869298057540007",
        "playStatus":0,
        "rssi":18, // 当前4G信号强度
        "sdCardFreeSpace":"7684500",
        "space":"7684500",
        "stateCharge":"821",
        "storageType":1
    }
 */
namespace asns {
    class CHeartBeatData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CHeartBeatData, cmd, volume, imei, playStatus, sdcardSpace, flashSpace,
                                       storageType, v5, v12, v24)

        int do_success() {
            CAudioCfgBusiness cfg;
            cfg.load();
            CVolumeSet volumeSet;
            volumeSet.load();
            CUtils utils;
            cmd = "hello";
            volume = volumeSet.getVolume();
            imei = cfg.business[0].serial;
            playStatus = utils.get_process_status("madplay");
            sdcardSpace = "7684500";
            flashSpace = "7684500";
            storageType = 1;
            v5 = 0;
            v12 = 0;
            v24 = 0;
            return 1;
        }

    private:
        std::string cmd;
        int volume;
        std::string imei;
        int playStatus;
        std::string sdcardSpace;
        std::string flashSpace;
        int storageType;
        int v5;
        int v12;
        int v24;
    };
}