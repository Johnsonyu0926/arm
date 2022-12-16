#pragma once

#include "doorsbase.h"
#include "volume.hpp"
#include "audiocfg.hpp"
#include "add_column_custom_audio_file.hpp"
#include "talk.h"
#include "login.hpp"
#include "file_recv.hpp"
#include "utils.h"

#include <thread>

/*{
   “address”: “01”,
   “ip”: “192.168.1.100”,
   “version”: “0.0.01”,
    “gateway”: “”,
    “netmask”: “”,
    “storageType”: “”, // 0-内置存储；1-外挂存储
    “volume”: “”,
    “relayMode”: “”, // 闪灯模式：1-自定义模式；2-播放模式
    “relayStatus”: “”, // 闪灯状态：0-关闭；1-打开
    “playStatus”: “”, // 播放状态：0-未播放；1-播放中
    “coreVersion”: “”, // 内核版本：IO-触发版本；COMMON-普通版本
}
*/
extern int g_playing_priority;
extern asns::CVolumeSet g_volumeSet;

class DeviceBaseInfo {
public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(DeviceBaseInfo, address, ip, gateway,version, netmask, storageType, volume, relayMode,
                                   relayStatus, playStatus, coreVersion)

    int do_success() {
        address = "01";
        CUtils util;
        ip = util.get_lan_addr();
        asns::CAudioCfgBusiness cfg;
        cfg.load();
        version = cfg.business[0].codeVersion;
        gateway = util.get_ros_gateway();
        netmask = util.get_ros_netmask();
        storageType = "1";
        g_volumeSet.load();
        volume = std::to_string(g_volumeSet.getVolume());
        relayMode = "2";
        relayStatus = "1";
        playStatus = g_playing_priority == NON_PLAY_PRIORITY ? "0" : "1";
        coreVersion = "LuatOS-Air_V4010_RDA8910_BT_TTS_FLOAT";
    }

private:
    std::string address;
    std::string ip;
    std::string version;
    std::string gateway;
    std::string netmask;
    std::string storageType;
    std::string volume;
    std::string relayMode;
    std::string relayStatus;
    std::string playStatus;
    std::string coreVersion;
};

class DeviceBaseState {
public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(DeviceBaseState, relayStatus, volume, storageType)

    int do_success() {
        relayStatus = g_playing_priority == NON_PLAY_PRIORITY ? "0" : "1";
        g_volumeSet.load();
        volume = g_volumeSet.getVolume();
        storageType = "1";
    }

private:
    std::string relayStatus;
    std::string volume;
    std::string storageType;
};

namespace asns {
    const int TCPNETWORKSET = 101;
    const int UPDATEPWD = 102;
    const int NETWORKSET = 104;
    const int LOGIN = 103;
    const int SETDEVICEADDRRESS = 0;
    const int AUDIOPLAY = 1;
    const int AUDIOSTOP = 2;
    const int VOLUMSET = 3;
    const int REBOOT = 4;
    const int GETDEVICESTATUS = 5;
    const int TTSPLAY = 6;
    const int LIGHTSWITCH = 7;
    const int FILEUPLOAD = 8;
    const int RESTORE = 9;
    const int AUDIONUMORTPLAY = 10;
    const int TTSNUMORTPLAY = 11;
    const int GETDEVICEBASEINFO = 12;
    const int RECORD = 13;
    const int REMOTEUPGRADE = 14;
    const int GETAUDIOLIST = 15;
    const int LIGHTCONFIG = 16;
    const int RECORDBEGIN = 18;
    const int RECORDEND = 19;
    const int AUDIOFILEUPLOAD = 20;
    const int REMOTEFILEUPGRADE = 21;

    int SendTrue(CSocket *pClient) {
        std::string res = "01 E1";
        return pClient->Send(res.c_str(), res.length());
    }

    int SendFast(const char *err_code, CSocket *pClient) {
        char buf[64] = {0};
        sprintf(buf, "%s %s", "01", err_code);
        pClient->Send(buf, sizeof(buf));
        return 0;
    }

    bool isNumber(const string &str) {
        for (const char &c: str) {
            if (std::isdigit(c) == 0)
                return false;
        }
        return true;
    }

    int Login(const std::vector<std::string> &m_str, CSocket *pClient) {
        CAudioCfgBusiness cfg;
        cfg.load();
        std::cout << m_str[4] << " " << cfg.business[0].password << " " << m_str[5] << std::endl;
        if (m_str[4].compare("admin") == 0 && m_str[5] == cfg.business[0].password) {
            std::cout << "return login ok" << std::endl;
            return SendTrue(pClient);
        } else {
            std::cout << "return login error" << std::endl;
            return SendFast("F6", pClient);
        }
    }

    int SettingDeviceAddress(const std::vector<std::string> &m_str, CSocket *pClient) {
    }

    // AA 04 01 01 23.mp3 BB EF
    int AudioPlay(const std::vector<std::string> &m_str, CSocket *pClient) {
        std::string name = m_str[4].substr(0, m_str[4].find_first_of('.'));
        CAddColumnCustomAudioFileBusiness busines;
        if (!isNumber(name) || std::stoi(name) > 100 || std::stoi(name) < 0) {
            //音频文件名校验失败
            return SendFast("F21", pClient);
        } else if (busines.isNameEmpty(name)) {
            //音频文件查询不存在
            return SendFast("F23", pClient);
        } else if (g_playing_priority != NON_PLAY_PRIORITY) {
            /*设备正在播放中，触发失败*/
            return SendFast("F22", pClient);
        } else {
            char cmd[256] = {0};
            CAudioCfgBusiness cfg;
            cfg.load();
            sprintf(cmd, "madplay %s%s -r &", cfg.getAudioFilePath().c_str(), m_str[4].c_str());
            std::cout << "cmd: " << cmd << std::endl;
            std::thread([&] { system(cmd); }).detach();

            return SendTrue(pClient);
        }
    }

    int AudioStop(CSocket *pClient) {
        system("killall -9 madplay");
        g_playing_priority = NON_PLAY_PRIORITY;
        return SendTrue(pClient);
    }

    int VolumeSet(const std::vector<std::string> &m_str, CSocket *pClient) {
        int volume = std::stoi(m_str[4]);
        if (volume > 7 || volume < 0) {
            //音量值不在范围
            return SendFast("F31", pClient);
        } else {
            CVolumeSet volumeSet;
            volumeSet.setVolume(volume);
            volumeSet.addj(volume);
            volumeSet.saveToJson();
            return SendTrue(pClient);
        }
    }

    int Reboot(CSocket *pClient) {
        SendTrue(pClient);
        system("reboot");
        return 1;
    }

    int GetDeviceStatus(CSocket *pClient) {
        DeviceBaseState deviceState;
        deviceState.do_success();
        json res = deviceState;
        std::string str = "01 E1 " + res.dump();
        pClient->Send(str.c_str(), str.length());
    }

    int Restore(CSocket *pClient) {
        CUtils utils;
        if (utils.is_ros_platform()) {
            system("cm default");
            CAudioCfgBusiness cfg;
            cfg.load();
            cfg.business[0].password = "Aa123456";
            cfg.business[0].server = "192.168.1.90";
            cfg.business[0].port = 7681;
            cfg.saveToJson();
            return SendTrue(pClient);
        } else {
            return SendFast("F35", pClient);
        }
    }

    int AudioNumberOrTimePlay(const std::vector<std::string> &m_str, CSocket *pClient) {
        std::string name = m_str[4].substr(0, m_str[4].find_first_of('.'));
        CAddColumnCustomAudioFileBusiness busines;
        if (!isNumber(name) || std::stoi(name) > 100 || std::stoi(name) < 0) {
            //音频文件名校验失败
            return SendFast("F21", pClient);
        } else if (g_playing_priority != NON_PLAY_PRIORITY) {
            return SendFast("F22", pClient);
        } else if (busines.isNameEmpty(name)) {
            //音频文件查询不存在
            return SendFast("F23", pClient);
        } else {
            char command[256] = {0};
            int duration = std::stoi(m_str[6]);
            CAudioCfgBusiness cfg;
            cfg.load();
            switch (std::stoi(m_str[5])) {
                case 1: {
                    std::string cmd = "madplay ";
                    for (int i = 0; i < duration; ++i) {
                        cmd += cfg.getAudioFilePath() + m_str[4] + ' ';
                    }
                    cmd += "&";
                    std::cout << "cmd: " << cmd << std::endl;
                    system(cmd.c_str());
                    break;
                }
                case 2: {
                    int d = duration / (3600 * 24);
                    int t = duration % (3600 * 24) / 3600;
                    int m = duration % (3600 * 24) % 3600 / 60;
                    int s = duration % (3600 * 24) % 3600 % 60;
                    char buf[64] = {0};
                    sprintf(buf, "%d:%d:%d:%d", d, t, m, s);
                    sprintf(command, "madplay %s%s -r -t %s &", cfg.getAudioFilePath().c_str(), m_str[4].c_str(),
                            buf);
                    std::cout << "cmd: " << command << std::endl;
                    system(command);
                    break;
                }
                default:
                    return SendFast("F4", pClient);
            }
            return SendTrue(pClient);
        }
    }

    int GetDeviceBaseInfo(CSocket *pClient) {
        DeviceBaseInfo devInfo;
        devInfo.do_success();
        json res = devInfo;
        std::string str = "01 E1 " + res.dump();
        std::cout << "GetDeviceBaseInfo Res: " << str << std::endl;
        pClient->Send(str.c_str(), str.length());
    }

    int NetworkSet(const std::vector<std::string> &m_str, CSocket *pClient) {
        CAudioCfgBusiness cfg;
        cfg.load();
        if (m_str[4].compare("admin") != 0 || m_str[5] != cfg.business[0].password) {
            return SendFast("F6", pClient);
        } else if (m_str[5].compare("Aa123456") == 0) {
            return SendFast("F7", pClient);
        } else {
            CUtils utils;
            const std::string &gateway = m_str[7];
            const std::string &ipAddress = m_str[6];
            const std::string &netMask = m_str[8];
            if (utils.is_ros_platform()) {
                char cm[128] = {0};
                sprintf(cm, "cm set_val WAN1 gateway %s", gateway.c_str());
                std::cout << cm << std::endl;
                system(cm);
                sprintf(cm, "cm set_val WAN1 ipaddress %s", ipAddress.c_str());
                std::cout << cm << std::endl;
                system(cm);
                sprintf(cm, "cm set_val WAN1 ipmask %s", netMask.c_str());
                std::cout << cm << std::endl;
                system(cm);
                sprintf(cm, "ifconfig eth0 inet %s netmask %s up", ipAddress.c_str(), netMask.c_str());
                std::cout << cm << std::endl;
                system(cm);
                sprintf(cm, "ip r add default via %s", gateway.c_str());
                std::cout << cm << std::endl;
            } else {
                char uci[128] = {0};
                sprintf(uci, "uci set network.wan.gateway=%s", gateway.c_str());
                system(uci);
                sprintf(uci, "uci set network.wan.ipaddr=%s", ipAddress.c_str());
                system(uci);
                sprintf(uci, "uci set network.wan.netmask=%s", netMask.c_str());
                system(uci);
                sprintf(uci, "uci commit");
                system(uci);
                sprintf(uci, "/etc/init.d/network reload");
                system(uci);
            }
            return SendTrue(pClient);
        }
    }

    int TCPNetworkSet(const std::vector<std::string> &m_str, CSocket *pClient) {
        CAudioCfgBusiness cfg;
        cfg.load();
        if (m_str[4].compare("admin") != 0 || m_str[5] != cfg.business[0].password) {
            return SendFast("F6", pClient);
        } else if (m_str[5].compare("Aa123456") == 0) {
            return SendFast("F7", pClient);
        } else {
            CUtils utils;
            const std::string &gateway = m_str[7];
            const std::string &ipAddress = m_str[6];
            if (utils.is_ros_platform()) {
                char cm[128] = {0};
                sprintf(cm, "cm set_val WAN1 ipaddress %s", ipAddress.c_str());
                system(cm);
                sprintf(cm, "cm set_val WAN1 gateway %s", gateway.c_str());
                system(cm);
                sprintf(cm, "ifconfig eth0 inet %s up", ipAddress.c_str());
                system(cm);
                sprintf(cm, "ip route add default via %s", gateway.c_str());
                system(cm);
            } else {
                char uci[128] = {0};
                sprintf(uci, "uci set network.wan.gateway=%s", gateway.c_str());
                system(uci);
                sprintf(uci, "uci set network.wan.ipaddr=%s", ipAddress.c_str());
                system(uci);
                sprintf(uci, "uci commit");
                system(uci);
                sprintf(uci, "/etc/init.d/network reload");
                system(uci);
            }
            return SendTrue(pClient);
        }
    }

    int UpdatePwd(const std::vector<std::string> &m_str, CSocket *pClient) {
        CAudioCfgBusiness cfg;
        cfg.load();
        if (m_str[5].length() < 8) {
            return SendFast("F25", pClient);
        } else if (m_str[4].compare("admin") != 0 || m_str[5] != cfg.business[0].password) {
            return SendFast("F6", pClient);
        } else {
            cfg.business[0].password = m_str[6];
            cfg.saveToJson();
            CUtils utils;
            if (utils.is_ros_platform()) {
                char buf[64] = {0};
                sprintf(buf, "cm set_val sys password %s", m_str[5].c_str());
                system(buf);
            }
            return SendTrue(pClient);
        }
    }

    int GetAudioList(const std::vector<std::string> &m_str, CSocket *pClient) {
        asns::CAddColumnCustomAudioFileBusiness busines;
        busines.load();
        json res = busines.business;
        std::string str = "01 E1 " + res.dump();
        std::cout << str << std::endl;
        pClient->Send(str.c_str(), str.length());
    }

    // AA 05 01 13 5 http://192.168.85.1:8091/iot/1v1/api/v1/micRecordUpload BB EF
    int Record(const std::vector<std::string> &m_str, CSocket *pClient) {
        int time = std::stod(m_str[4]);
        if (time > 30 || time < 0) {
            return SendFast("F5", pClient);
        }
        char buf[128] = {0};
        sprintf(buf, "arecord -d %d -f cd /tmp/record.mp3 &", time);
        system(buf);
        // std::cout << "killall -9 arecord" << std::endl;

        std::string res = m_str[5] + " " + "8000";
        pClient->Send(res.c_str(), res.length());
        return 1;
    }

    int RecordBegin(const std::vector<std::string> &m_str, CSocket *pClient) {
        system("arecord -f cd /tmp/record.mp3 &");
        return SendTrue(pClient);
    }

    int RecordEnd(const std::vector<std::string> &m_str, CSocket *pClient) {
        system("killall -9 arecord");
        std::ifstream fs("/tmp/record.mp3", std::fstream::in | std::fstream::binary);
        std::streampos begin, end;
        begin = fs.tellg();
        fs.seekg(0, std::ios::end);
        end = fs.tellg();
        int file_size = end - begin;
        std::string res = "01 E1 " + m_str[4] + " " + std::to_string(file_size) + " 8002";
        pClient->Send(res.c_str(), res.length());

        Server ser(8000);
        ser.bind();
        ser.listen();
        int connfd = ser.accept();
        char buf[1024] = {0};
        while (!fs.eof()) {
            fs.read(buf, sizeof(buf));
            int len = fs.gcount();
            ser.write(connfd, buf, len);
        }
        fs.close();
    }

    int AudioFileUpload(const std::vector<std::string> &m_str, CSocket *pClient) {
        std::string name = m_str[6].substr(0, m_str[6].find_first_of('.'));
        std::string suffix = m_str[6].substr(m_str[6].find_first_of('.') + 1);
        if (!isNumber(name) || std::stoi(name) > 100 || std::stoi(name) < 0) {
            //音频文件名校验失败
            return SendFast("F21", pClient);
        }
        if (suffix.compare("mp3") != 0 && suffix.compare("MP3") != 0) {
            return SendFast("F27", pClient);
        }
        CAddColumnCustomAudioFileData node;
        node.setName(name);
        CAddColumnCustomAudioFileBusiness business;
        business.business.push_back(node);
        business.saveJson();

        CAudioCfgBusiness cfg;
        cfg.load();

        std::string path = cfg.getAudioFilePath() + m_str[6];
        std::fstream fs(path, std::fstream::out | std::fstream::binary | std::fstream::trunc);
        std::thread([&] {
            Server ser(8001);
            ser.bind();
            ser.listen();
            int connfd = ser.accept();
            int file_size = std::stod(m_str[4]);
            char buf[1024] = {0};
            while (true) {
                int len = ser.read(connfd, buf, sizeof(buf));
                if (len > 0) {
                    fs.write(buf, len);
                    file_size -= len;
                    if (file_size <= 0)break;
                }
            }
            fs.close();
        }).detach();
        std::string res = "01 E1 " + m_str[5] + " 8001";
        pClient->Send(res.c_str(), res.length());
        return 1;
    }

    int RemoteFileUpgrade(const std::vector<std::string> &m_str, CSocket *pClient) {
        std::thread([&] {
            std::fstream fs("/mnt/temp", std::fstream::out | std::fstream::binary | std::fstream::trunc);
            Server ser(8000);
            ser.bind();
            ser.listen();
            int connfd = ser.accept();
            char buf[1024] = {0};
            int file_size = std::stod(m_str[4]);
            while (true) {
                int len = ser.read(connfd, buf, sizeof(buf));
                if (len > 0) {
                    fs.write(buf, len);
                    file_size -= len;
                    if (file_size <= 0)break;
                }
            }
            fs.close();
            system("rm /mnt/audioserver");
            system("mv /mnt/temp /mnt/audioserver");
            system("chmod +x audioserver");
            system("reboot");
        }).detach();
        std::string res = "01 E1 " + m_str[5] + " 8000";
        pClient->Send(res.c_str(), res.length());
        return 1;
    }
}