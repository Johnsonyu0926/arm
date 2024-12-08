#pragma once

#include <string>
#include "json.hpp"
#include "audiocfg.hpp"
#include "utils.h"
#include "volume.hpp"
#include "Relay.hpp"
#include "PlayStatus.hpp"
#include "spdlog/spdlog.h"

extern asns::CVolumeSet g_volumeSet;

namespace asns {

    class CRestoreData {
    public:
        std::string codeVersion;
        std::string coreVersion;
        int relayMode{0};
        std::string ip;
        int storageType{0};
        int port{0};
        int playStatus{0};
        int volume{0};
        int relayStatus{0};
        std::string hardwareReleaseTime;
        int spiFreeSpace{0};
        int flashFreeSpace{0};
        std::string hardwareVersion;
        std::string password;
        int temperature{0};
        std::string netmask;
        std::string address;
        std::string gateway;
        std::string userName;
        std::string imei;
        std::string functionVersion;
        std::string deviceCode;
        std::string serverAddress;
        std::string serverPort;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CRestoreData, codeVersion, coreVersion, relayMode, ip, storageType, port,
                                       playStatus, volume, relayStatus, hardwareReleaseTime, spiFreeSpace,
                                       flashFreeSpace, hardwareVersion, password, temperature, netmask, address,
                                       gateway, userName, imei, functionVersion, deviceCode, serverAddress, serverPort)
    };

    class CRestoreResult {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CRestoreResult, cmd, resultId, data, msg)

        int do_success() {
            CAudioCfgBusiness cfg;
            cfg.load();
            CUtils util;
            cmd = "Restore";
            resultId = 1;
            msg = "restore success";

            data.codeVersion = "2.1.01";
            data.coreVersion = util.get_core_version();
            data.relayMode = Relay::getInstance().getGpioModel();
            data.ip = "192.168.3.100";
            data.storageType = 1;
            data.port = 34508;
            data.playStatus = PlayStatus::getInstance().getPlayState();
            g_volumeSet.load();
            data.volume = g_volumeSet.getVolume();
            data.relayStatus = Relay::getInstance().getGpioStatus();
            data.hardwareReleaseTime = util.get_hardware_release_time();
            data.spiFreeSpace = 9752500;
            data.flashFreeSpace = util.get_available_Disk("/mnt");
            data.hardwareVersion = util.get_res_by_cmd("uname -r");
            data.password = "Aa123456";
            data.temperature = 12;
            data.netmask = "255.255.255.0";
            data.address = "01";
            data.gateway = "192.168.3.1";
            data.userName = "admin";
            data.imei = cfg.business[0].deviceID;
            data.functionVersion = "COMMON";
            data.deviceCode = cfg.business[0].deviceID;
            data.serverAddress = cfg.business[0].server;
            data.serverPort = std::to_string(cfg.business[0].port);
            spdlog::info("data.serverPort: {}", data.serverPort);
            return 1;
        }

    private:
        std::string cmd;
        int resultId{0};
        CRestoreData data;
        std::string msg;
    };

    class CRestore {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CRestore, cmd)

        int do_req(CSocket *pClient) {
            CUtils utils;
            CAudioCfgBusiness cfg;
            cfg.load();
            cfg.business[0].serverPassword = "Aa123456";
            cfg.business[0].server = "192.168.1.90";
            cfg.business[0].port = 7681;
            cfg.saveToJson();

            CRestoreResult restoreResult;
            restoreResult.do_success();
            json js = restoreResult;
            std::string str = js.dump();
            pClient->Send(str.c_str(), str.length());

            utils.restore(cfg.business[0].savePrefix);
            return 1;
        }

    private:
        std::string cmd;
    };
} // namespace asns