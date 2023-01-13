#ifndef __CLOGIN_H__
#define __CLOGIN_H__

#include <iostream>
#include "json.hpp"
#include "audiocfg.hpp"
#include "utils.h"
#include "volume.hpp"

using namespace std;

extern asns::CVolumeSet g_volumeSet;

namespace asns {
    class CLoginData {
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
        int hardwareModelId;
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
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CLoginData, codeVersion, coreVersion, relayMode, ip, storageType, port,
                                       playStatus, volume, relayStatus, hardwareReleaseTime, spiFreeSpace,
                                       flashFreeSpace, hardwareVersion, password, temperature, netmask, address,
                                       gateway, userName, imei, functionVersion, deviceCode, serverAddress, serverPort,hardwareModelId)
    };

    class CLoginResult {
    private:
        string cmd;
        int resultId;
        CLoginData data;
        string msg;

    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CLoginResult, cmd, resultId, data, msg)

        int do_success() {
            CAudioCfgBusiness cfg;
            cfg.load();

            cmd = "Login";
            resultId = 1;
            msg = "login success";
            data.codeVersion = cfg.business[0].codeVersion; //"2.1.01"; //"1.2";
            data.coreVersion = "LuatOS-Air_V4010_RDA8910_BT_TTS_FLOAT";
            data.relayMode = 2;

            CUtils util;
            data.ip = util.get_lan_addr();
            data.storageType = 0;
            data.port = 34508;
            data.playStatus = 0;
            g_volumeSet.load();
            data.volume = g_volumeSet.getVolume();
            data.relayStatus = 1;
            data.hardwareReleaseTime = "2022.12.09";
            data.spiFreeSpace = 9752500;
            data.flashFreeSpace = util.get_available_Disk("/mnt");
            data.hardwareVersion = "4.2.1";
            data.hardwareModelId = 2;
            data.password = cfg.business[0].serverPassword;
            data.temperature = 12;
            data.netmask = util.get_lan_netmask();
            data.address = "01";
            data.gateway = util.get_lan_gateway();
            data.userName = "admin";
            data.imei = "11111";
            data.functionVersion = "BROADCAST";
            data.deviceCode = cfg.business[0].deviceID;
            data.serverAddress = cfg.business[0].server;
            data.serverPort = to_string(cfg.business[0].port);
            cout << "data.serverPort:" << data.serverPort << endl;
        };

        int do_fail() {
            CAudioCfgBusiness cfg;
            cfg.load();

            cmd = "Login";
            resultId = 2;
            msg = "login fail";
            data.codeVersion = cfg.business[0].codeVersion; //"2.1.01"; //"1.2";
            data.coreVersion = "LuatOS-Air_V4010_RDA8910_BT_TTS_FLOAT";
            data.relayMode = 2;

            CUtils util;
            data.ip = util.get_lan_addr();
            data.storageType = 0;
            data.port = 34508;
            data.playStatus = 0;
            g_volumeSet.load();
            data.volume = g_volumeSet.getVolume();
            data.relayStatus = 1;
            data.hardwareReleaseTime = "2022.12.09";
            data.spiFreeSpace = 9752500;
            data.hardwareModelId = 2;
            data.flashFreeSpace = 1305000;
            data.hardwareVersion = "4.2.1";
            data.password = cfg.business[0].serverPassword;
            data.temperature = 12;
            data.netmask = util.get_lan_netmask();
            data.address = "01";
            data.gateway = util.get_lan_gateway();
            data.userName = "admin";
            data.imei = "11111";
            data.functionVersion = "BROADCAST";
            data.deviceCode = cfg.business[0].deviceID;
            data.serverAddress = cfg.business[0].server;
            data.serverPort = to_string(cfg.business[0].port);
            cout << "data.serverPort:" << data.serverPort << endl;
        };
    };

    class CLogin {
    private:
        string cmd;
        string userName;
        string password;

    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CLogin, cmd, userName, password)

        int do_req(CSocket *pClient) {
            CAudioCfgBusiness cfg;
            cfg.load();
            if (userName.compare("admin") == 0 && password == cfg.business[0].serverPassword) {
                CLoginResult res;
                res.do_success();
                json j = res;
                std::string s = j.dump();
                cout << s << endl;
                pClient->Send(s.c_str(), s.length());
                return 1;
            } else {
                cout << "failed compare password." << endl;
                CLoginResult res;
                res.do_fail();
                json j = res;
                std::string s = j.dump();
                cout << s << endl;
                pClient->Send(s.c_str(), s.length());
                return 0;
            }
        }
    };
} // namespace tcpserver
#endif
