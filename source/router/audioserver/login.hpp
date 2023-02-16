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

        void do_success() {
            CUtils util;
            CAudioCfgBusiness cfg;
            cfg.load();
            codeVersion = cfg.business[0].codeVersion; //"2.1.01"; //"1.2";
            coreVersion = util.get_core_version();
            relayMode = util.get_gpio_model();
            ip = util.get_lan_addr();
            storageType = 0;
            port = 34508;
            playStatus = util.get_play_state();
            g_volumeSet.load();
            volume = g_volumeSet.getVolume();
            relayStatus = util.get_gpio_state();
            hardwareReleaseTime = util.gethardware_release_time();
            spiFreeSpace = 9752500;
            flashFreeSpace = util.get_available_Disk("/mnt");
            hardwareVersion = util.get_by_cmd_res("uname -r");
            hardwareModelId = 2;
            password = cfg.business[0].serverPassword;
            temperature = 12;
            netmask = util.get_lan_netmask();
            address = "01";
            gateway = util.get_lan_gateway();
            userName = "admin";
            imei = cfg.business[0].deviceID;
            functionVersion = "BROADCAST";
            deviceCode = cfg.business[0].deviceID;
            serverAddress = cfg.business[0].server;
            serverPort = to_string(cfg.business[0].port);
        }

    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CLoginData, codeVersion, coreVersion, relayMode, ip, storageType, port,
                                       playStatus, volume, relayStatus, hardwareReleaseTime, spiFreeSpace,
                                       flashFreeSpace, hardwareVersion, password, temperature, netmask, address,
                                       gateway, userName, imei, functionVersion, deviceCode, serverAddress, serverPort,
                                       hardwareModelId)
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
            cmd = "Login";
            resultId = 1;
            msg = "login success";
            data.do_success();
        };

        int do_fail() {
            cmd = "Login";
            resultId = 2;
            msg = "login fail";
            data.do_success();
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
