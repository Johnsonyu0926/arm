#pragma once

#include "json.hpp"
#include "audiocfg.hpp"
#include "volume.hpp"
#include "utils.h"

/**
* {
        "sdcardSpace":16225000, // sd卡剩余空间
        "lng":"120.2168731", // 经纬度
        "flashSpace":16225000, // 内置存储剩余空间
        "project":"0Tr1N2kbP5sKUDglAIOUxHdwp4VkLucz", // 设备项目名称，唯一
        "iotVersion":"COMMON", // 设备版本，详见设备版本号
        "version":"2.0.30", // 设备软件版本
        "volume":4, // 当前音量
        "iccId":"898604B41022C0341497", // sim卡号
        "v12":0, // 继电器当前状态
        "volt":971,
        "v24":0, // 继电器当前状态
        "storageType":1, // 当前默认存储位置
        "imei":"869298057401606",
        "topic":"TaDiao/device/report/test/869298057401606",
        "cmd":"start",
        "lat":"030.2099819",
        "v5":0 // 继电器当前状态
    }
*/

namespace asns {
    class CBootData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CBootData, sdcardSpace, flashSpace, iotVersion, version, volume, v5, v12, v24,
                                       volt, storageType, imei, topic, cmd, hardwareModelId, hardwareVersion, ipAddress)


        void do_success() {
            CAudioCfgBusiness cfg;
            cfg.load();
            CVolumeSet volumeSet;
            sdcardSpace = 16225000;
            flashSpace = 16225000;
            iotVersion = "COMMON";
            version = cfg.business[0].codeVersion;
            volume = volumeSet.getVolume();
            v5 = 0;
            v12 = 0;
            v24 = 0;
            volt = 971;
            storageType = 1;
            imei = cfg.business[0].serial;
            topic = "TaDiao/device/report/test/" + cfg.business[0].serial;
            cmd = "start";
            hardwareModelId = 2;
            CUtils utils;
            if (utils.is_ros_platform()) {
                hardwareVersion = "7621";
            } else {
                hardwareVersion = "7688";
            }
            ipAddress = utils.get_lan_addr();
        }

    private:
        long sdcardSpace;
        long flashSpace;
        std::string iotVersion;
        std::string version;
        int volume;
        int v5;
        int v12;
        int v24;
        int volt;
        int storageType;
        std::string imei;
        std::string topic;
        std::string cmd;
        int hardwareModelId;
        std::string hardwareVersion;
        std::string ipAddress;
    };
}