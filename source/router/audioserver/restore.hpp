#pragma once

#include "json.hpp"
#include "audiocfg.hpp"
#include "utils.h"
#include "volume.hpp"
#include "playStatus.hpp"
#include "gpio.hpp"

extern asns::CVolumeSet g_volumeSet;
namespace asns {

    class CRestoreData {
    public:
        string codeVersion;
        string coreVersion;
        int relayMode;
        string ip;
        int storageType;
        int port;
        int playStatus;
        int volume;
        int relayStatus;
        string hardwareReleaseTime;
        int spiFreeSpace;
        int flashFreeSpace;
        string hardwareVersion;
        string password;
        int temperature;
        string netmask;
        string address;
        string gateway;
        string userName;
        string imei;
        string functionVersion;
        string deviceCode;
        string serverAddress;
        string serverPort;

    public:
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
            data.coreVersion = util.get_by_cmd_res("webs -V");
            data.relayMode = CGpio::getInstance().getGpioModel();
            data.ip = util.get_lan_addr();
            data.storageType = 1;
            data.port = 34508;
            data.playStatus =PlayStatus::getInstance().getPlayId() == asns::STOP_TASK_PLAYING ? 0 : 1;
            g_volumeSet.load();
            data.volume = g_volumeSet.getVolume();
            data.relayStatus = CGpio::getInstance().getGpioStatus();
            data.hardwareReleaseTime = util.get_by_cmd_res("uname -a");
            data.spiFreeSpace = 9752500;
            data.flashFreeSpace = util.get_available_Disk("/mnt");
            data.hardwareVersion = util.get_by_cmd_res("uname -r");
            data.password = cfg.business[0].serverPassword;
            data.temperature = 12;
            data.netmask = util.get_lan_netmask();
            data.address = "01";
            data.gateway = util.get_lan_gateway();
            data.userName = "admin";
            data.imei = cfg.business[0].deviceID;
            data.functionVersion = "COMMON";
            data.deviceCode = cfg.business[0].deviceID;
            data.serverAddress = cfg.business[0].server;
            data.serverPort = to_string(cfg.business[0].port);
            cout << "data.serverPort:" << data.serverPort << endl;

        };
    private:
        std::string cmd;
        int resultId;
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
        }

    private:
        std::string cmd;
    };
}
