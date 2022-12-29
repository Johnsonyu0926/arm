#pragma once

#include "json.hpp"
#include "audiocfg.hpp"
#include "utils.h"
#include "volume.hpp"
extern asns::CVolumeSet g_volumeSet;
namespace asns {

    class CDeviceBaseData {
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
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CDeviceBaseData, codeVersion, coreVersion, relayMode, ip, storageType, port,
                                       playStatus, volume, relayStatus, hardwareReleaseTime, spiFreeSpace,
                                       flashFreeSpace, hardwareVersion, password, temperature, netmask, address,
                                       gateway, userName, imei, functionVersion, deviceCode, serverAddress, serverPort)

    };

    class CGetDeviceBaseInfoResult {

    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CGetDeviceBaseInfoResult, cmd, resultId, data, msg)

        int do_success() {
            CAudioCfgBusiness cfg;
            cfg.load();

            cmd = "GetDeviceBaseInfo";
            resultId = 1;
            msg = "查询设备信息成功";
            data.codeVersion = "2.1.01";
            data.coreVersion = "LuatOS-Air_V4010_RDA8910_BT_TTS_FLOAT";
            data.relayMode = 2;

            CUtils util;
            data.ip = util.get_lan_addr();
            data.storageType = 1;
            data.port = 34508;
            data.playStatus = 0;
            g_volumeSet.load();
            data.volume = g_volumeSet.getVolume();
            data.relayStatus = 1;
            data.hardwareReleaseTime = "2022.10.25";
            data.spiFreeSpace = 9752500;
            data.flashFreeSpace = 1305000;
            data.hardwareVersion = "4.2.1";
            data.password = cfg.business[0].serverPassword;
            data.temperature = 12;
            data.netmask = util.get_lan_netmask();
            data.address = "01";
            data.gateway = util.get_lan_gateway();
            data.userName = "admin";
            data.imei = "869298057534588";
            data.functionVersion = "COMMON";
            data.deviceCode = cfg.business[0].deviceID;
            data.serverAddress = cfg.business[0].server;
            data.serverPort = to_string(cfg.business[0].port);
            cout << "data.serverPort:" << data.serverPort << endl;

        };
    private:
        std::string cmd;
        int resultId;
        CDeviceBaseData data;
        std::string msg;
    };

    class CGetDeviceBaseInfo {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CGetDeviceBaseInfo, cmd)

        int do_req(CSocket *pClient) {
            CGetDeviceBaseInfoResult deviceBaseInfoResult;
            deviceBaseInfoResult.do_success();
            json js = deviceBaseInfoResult;
            std::string s = js.dump();
            pClient->Send(s.c_str(), s.length());
        }

    private:
        std::string cmd;
    };
}