#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <thread>

#include "doorsbase.h"
#include "audiocfg.hpp"
#include "volume.hpp"
#include "audiocfg.hpp"
#include "add_column_custom_audio_file.hpp"
#include "add_custom_audio_file.hpp"
#include "talk.h"
#include "login.hpp"
#include "file_recv.hpp"
#include "utils.h"

extern int g_playing_priority;
static int g_irs485 = -1;
static int g_tty = 1; //ttyUSB1 as default.
#define MAX_SEND 12

class RSDeviceBaseInfo {
public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(RSDeviceBaseInfo, address, ip, gateway, netmask, storageType, volume, relayMode,
                                   relayStatus, playStatus, coreVersion)

    int do_success() {
        address = "01";
        CUtils util;
        ip = util.get_lan_addr();
        gateway = util.get_ros_gateway();
        netmask = util.get_ros_netmask();
        storageType = "1";
        g_volumeSet.load();
        volume = std::to_string(g_volumeSet.getVolume());
        relayMode = "2";
        relayStatus = "1";
        playStatus = g_playing_priority == NON_PLAY_PRIORITY ? "0" : "1";
        coreVersion = "LuatOS-Air_V4010_RDA8910_BT_TTS_FLOAT";
        // TODO
    }

private:
    std::string address;
    std::string ip;
    std::string gateway;
    std::string netmask;
    std::string storageType;
    std::string volume;
    std::string relayMode;
    std::string relayStatus;
    std::string playStatus;
    std::string coreVersion;
};

class RSDeviceBaseState {
public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(RSDeviceBaseState, relayStatus, volume, storageType)

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

namespace rs {

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
    const int GETRSDeviceBaseInfo = 12;
    const int RECORD = 13;
    const int REMOTEUPGRADE = 14;
    const int GETAUDIOLIST = 15;
    const int LIGHTCONFIG = 16;
    const int RECORDBEGIN = 18;
    const int RECORDEND = 19;
    const int AUDIOFILEUPLOAD = 20;
    const int REMOTEFILEUPGRADE = 21;

#define MAX_SEND 12

    /* Write data to uart dev, return 0 means OK */
    static int _uart_write(const char *pcData, int iLen) {
        int iFd = g_irs485;
        int iRet = -1;
        int len = 0;
        printf("to write :%s, len:%d\n", pcData, iLen);
        int count = iLen / MAX_SEND;
        if (iLen % MAX_SEND) {
            count++;
        }
        int offset = 0;
        printf("count=%d\n", count);
        for (int i = 0; i < count; i++) {
            if ((i + 1) * MAX_SEND > iLen) {
                len = iLen - i * MAX_SEND;
            } else {
                len = MAX_SEND;
            }
            printf("no.%d : offset:%d len:%d \n", i, offset, len);
            const char *data = pcData + offset;
            for (int j = 0; j < len; j++) {
                printf("%02x ", data[j]);
            }
            iRet = write(iFd, data, len);
            if (iRet < 0) {
                close(iFd);
                g_irs485 = -1;
                printf("error write %d , len:%d\n", iFd, len);
            } else {
                printf("no.%d : write len:%d success, iRet:%d\n", i, len, iRet);
            }

            offset += MAX_SEND;

            usleep(100 * 1000);
        }
        return iRet;
    }

    int SendTrue() {
        std::string res = "01 E1";
        return _uart_write(res.c_str(), res.length());
    }

    int SendFast(const char *err_code) {
        char buf[64] = {0};
        sprintf(buf, "%s %s", "01", err_code);
        _uart_write(buf, sizeof(buf));
        return 0;
    }

    bool isNumber(const string &str) {
        for (const char &c: str) {
            if (std::isdigit(c) == 0)
                return false;
        }
        return true;
    }

    int Login(const std::vector<std::string> &m_str) {
        asns::CAudioCfgBusiness cfg;
        cfg.load();
        std::cout << m_str[4] << " " << cfg.business[0].password << " " << m_str[5] << std::endl;
        if (m_str[4].compare("admin") == 0 && m_str[5] == cfg.business[0].password) {
            std::cout << "return login ok" << std::endl;
            return SendTrue();
        } else {
            std::cout << "return login error" << std::endl;
            return SendFast("F6");
        }
    }

    int SettingDeviceAddress(const std::vector<std::string> &m_str) {
    }

    // AA 04 01 01 23.mp3 BB EF
    int AudioPlay(std::vector<std::string> &m_str) {
        std::string name = m_str[4].substr(0, m_str[4].find_first_of('.'));
        asns::CAddColumnCustomAudioFileBusiness busines;
        if (!isNumber(name) || std::stoi(name) > 100 || std::stoi(name) < 0) {
            //音频文件名校验失败
            return SendFast("F21");
        } else if (busines.isNameEmpty(name)) {
            //音频文件查询不存在
            return SendFast("F23");
        } else if (g_playing_priority != NON_PLAY_PRIORITY) {
            /*设备正在播放中，触发失败*/
            return SendFast("F22");
        } else {
            char cmd[256] = {0};
            asns::CAudioCfgBusiness cfg;
            cfg.load();
            sprintf(cmd, "madplay %s%s -r &", cfg.getAudioFilePath().c_str(), m_str[4].c_str());
            std::cout << "cmd: " << cmd << std::endl;
            std::thread([&] { system(cmd); }).detach();

            return SendTrue();
        }
    }

    int AudioStop() {
        system("killall -9 madplay");
        g_playing_priority = NON_PLAY_PRIORITY;
        return SendTrue();
    }

    int VolumeSet(const std::vector<std::string> &m_str) {
        int volume = std::stoi(m_str[4]);
        if (volume > 7 || volume < 0) {
            //音量值不在范围
            return SendFast("F31");
        } else {
            asns::CVolumeSet volumeSet;
            volumeSet.setVolume(volume);
            volumeSet.addj(volume);
            volumeSet.saveToJson();
            return SendTrue();
        }
    }

    int Reboot() {
        SendTrue();
        system("reboot");
        return 1;
    }

    int GetDeviceStatus() {
        RSDeviceBaseState deviceState;
        deviceState.do_success();
        json res = deviceState;
        std::string str = "01 E1 " + res.dump();
        rs::_uart_write(str.c_str(), str.length());
    }

	int restore_network() {
			char uci[128] = {0};
			sprintf(uci, "uci set network.lan.ipaddr=%s", "192.168.1.100");
			system(uci);
			sprintf(uci, "uci set network.lan.gateway=%s", "192.168.1.1");
			system(uci);
			sprintf(uci, "uci set network.lan.netmask=%s", "255.255.255.0");
			system(uci);
			sprintf(uci, "uci commit network");
			system(uci);
			sprintf(uci, "/etc/init.d/network reload");
			system(uci);

	}
	int clean(const char* prefix) {
		char cmd[128];
		sprintf(cmd,"rm %s/audiodata/*", prefix);
		system(cmd);
		sprintf(cmd,"rm %s/cfg/*.json", prefix);
		system(cmd);
	}

    int Restore() {
        CUtils utils;
        if (utils.is_ros_platform()) {
            asns::CAudioCfgBusiness cfg;
            cfg.load();
            cfg.business[0].password = "Aa123456";
            cfg.business[0].server = "192.168.1.90";
            cfg.business[0].port = 7681;
		//	clean(cfg.business[0].savePrefix.c_str());
			utils.clean_audio_server_file(cfg.business[0].savePrefix.c_str());
            cfg.saveToJson();
            SendTrue();
			utils.ros_restore_allcfg();
			system("reboot");
			
        } else {
            //return SendFast("F35");
			//bugfix shidongxue .2022.12.20 
			asns::CAudioCfgBusiness cfg;
            cfg.load();
            cfg.business[0].password = "Aa123456";
            cfg.business[0].server = "192.168.1.90";
            cfg.business[0].port = 7681;
			utils.clean_audio_server_file(cfg.business[0].savePrefix.c_str());
            cfg.saveToJson();
            SendTrue();

			//network restore must be the last action!
			utils.openwrt_restore_network();
			cout<<"restore success!\n"<<endl;
        }
		return 1;
    }

    int AudioNumberOrTimePlay(const std::vector<std::string> &m_str) {
        std::string name = m_str[4].substr(0, m_str[4].find_first_of('.'));
        asns::CAddColumnCustomAudioFileBusiness busines;
        if (!isNumber(name) || std::stoi(name) > 100 || std::stoi(name) < 0) {
            //音频文件名校验失败
            return SendFast("F21");
        } else if (g_playing_priority != NON_PLAY_PRIORITY) {
            return SendFast("F22");
        } else if (busines.isNameEmpty(name)) {
            //音频文件查询不存在
            return SendFast("F23");
        } else {
            char command[256] = {0};
            int duration = std::stoi(m_str[6]);
            asns::CAudioCfgBusiness cfg;
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
                    sprintf(command, "madplay %s%s -r -t %s &", cfg.getAudioFilePath().c_str(), name.c_str(), buf);
                    break;
                }
                default:
                    return SendFast("F4");
            }
            system(command);
            return SendTrue();
        }
    }

    int GetRSDeviceBaseInfo() {
        RSDeviceBaseInfo devInfo;
        devInfo.do_success();
        json res = devInfo;
        std::string str = "01 E1 " + res.dump();
        std::cout << "GetRSDeviceBaseInfo Res: " << str << std::endl;
        rs::_uart_write(str.c_str(), str.length());
    }

    int NetworkSet(const std::vector<std::string> &m_str) {
        asns::CAudioCfgBusiness cfg;
        cfg.load();
        if (m_str[4].compare("admin") != 0 || m_str[5] != cfg.business[0].password) {
            return SendFast("F6");
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
                sprintf(uci, "uci set network.lan.ipaddr=%s", ipAddress.c_str());
                system(uci);
                sprintf(uci, "uci set network.lan.gateway=%s", gateway.c_str());
                system(uci);
                sprintf(uci, "uci set network.lan.netmask=%s", netMask.c_str());
                system(uci);
                sprintf(uci, "uci commit network");
                system(uci);
                sprintf(uci, "/etc/init.d/network reload");
                system(uci);
            }
            return SendTrue();
        }
    }

    int TCPNetworkSet(const std::vector<std::string> &m_str) {
        asns::CAudioCfgBusiness cfg;
        cfg.load();
        if (m_str[4].compare("admin") != 0 || m_str[5] != cfg.business[0].password) {
            return SendFast("F6");
        } else {
            CUtils utils;
            const std::string &gateway = m_str[7];
            const std::string &ipAddress = m_str[6];
            if (utils.is_ros_platform()) {
                char cm[128] = {0};
                sprintf(cm, "cm set_val WAN1 gateway %s", gateway.c_str());
                system(cm);
                sprintf(cm, "cm set_val WAN1 ipaddress %s", ipAddress.c_str());
                system(cm);
                sprintf(cm, "ifconfig eth0 inet %s up", ipAddress.c_str());
                system(cm);
                sprintf(cm, "ip r add default via %s", gateway.c_str());
                system(cm);
            } else {
                char uci[128] = {0};
                sprintf(uci, "uci set network.lan.ipaddr=%s", ipAddress.c_str());
                system(uci);
                sprintf(uci, "uci set network.lan.gateway=%s", gateway.c_str());
                system(uci);
                sprintf(uci, "uci commit network");
                system(uci);
                sprintf(uci, "/etc/init.d/network reload");
                system(uci);
            }
            return SendTrue();
        }
    }

    int UpdatePwd(const std::vector<std::string> &m_str) {
        asns::CAudioCfgBusiness cfg;
        cfg.load();
        if (m_str[5].length() < 8) {
            return SendFast("F25");
        } else if (m_str[4].compare("admin") != 0 || m_str[5] != cfg.business[0].password) {
            return SendFast("F6");
        } else {
            cfg.business[0].password = m_str[6];
            cfg.saveToJson();
            CUtils utils;
            if (utils.is_ros_platform()) {
                char buf[64] = {0};
                sprintf(buf, "cm set_val sys password %s", m_str[5].c_str());
                system(buf);
            }
            return SendTrue();
        }
    }

    int GetAudioList(const std::vector<std::string> &m_str) {
        asns::CAddColumnCustomAudioFileBusiness busines;
        busines.load();
        json res = busines.business;
        std::string str = "01 E1 " + res.dump();
        std::cout << str << std::endl;
        rs::_uart_write(str.c_str(), str.length());
    }

    // AA 05 01 13 5 http://192.168.85.1:8091/iot/1v1/api/v1/micRecordUpload BB EF
    int Record(const std::vector<std::string> &m_str) {
        int time = std::stod(m_str[4]);
        if (time > 30 || time < 0) {
            return SendFast("F5");
        }
        char buf[128] = {0};
        sprintf(buf, "arecord -d %d -f cd /tmp/record.mp3 &", time);
        system(buf);
        // std::cout << "killall -9 arecord" << std::endl;

        std::string res = m_str[5] + " " + "8000";
        rs::_uart_write(res.c_str(), res.length());
        return 1;
    }

    int RecordBegin(const std::vector<std::string> &m_str) {
        system("arecord -f cd /tmp/record.mp3");
        return SendTrue();
    }

    int RecordEnd(const std::vector<std::string> &m_str) {
        system("killall -9 arecord");
        std::ifstream fs("/tmp/record.mp3", std::fstream::in | std::fstream::binary);
        std::streampos begin, end;
        begin = fs.tellg();
        fs.seekg(0, std::ios::end);
        end = fs.tellg();
        int file_size = end - begin;
        std::string res = "01 E1 " + m_str[4] + " " + std::to_string(file_size) + " 8002";
        rs::_uart_write(res.c_str(), res.length());

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

    int AudioFileUpload(const std::vector<std::string> &m_str) {
        std::string name = m_str[6].substr(0, m_str[6].find_first_of('.'));
        std::string suffix = m_str[6].substr(m_str[6].find_first_of('.') + 1);
        if (!isNumber(name) || std::stoi(name) > 100 || std::stoi(name) < 0) {
            //音频文件名校验失败
            return SendFast("F21");
        }
        if (suffix.compare("mp3") != 0) {
            return SendFast("F27");
        }
        asns::CAddColumnCustomAudioFileData node;
        node.setName(name);
        asns::CAddColumnCustomAudioFileBusiness busines;
        busines.business.push_back(node);
        busines.saveJson();

        asns::CAudioCfgBusiness cfg;
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
        rs::_uart_write(res.c_str(), res.length());
        return 1;
    }

    int RemoteFileUpgrade(const std::vector<std::string> &m_str) {
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
        rs::_uart_write(res.c_str(), res.length());
        return 1;
    }


    void set_send_dir() {
        system("echo 1 > /sys/class/gpio/gpio3/value");
        printf("set send dir! gpio is 1\n");
    }

    void set_receive_dir() {
        system("echo 0 > /sys/class/gpio/gpio3/value");
        printf("set receive dir! gpio is 0\n");
    }


    static int _uart_open(void) {
        int iFd = -1;
        struct termios opt;
        asns::CAudioCfgBusiness cfg;
        cfg.load();
        int iBdVal = cfg.business[0].iBdVal;

		char cmd[64];
		sprintf(cmd,"stty -F /dev/ttyS%d %d", g_tty, iBdVal);
		system(cmd);
        system("echo 3 > /sys/class/gpio/export");
        system("echo out > /sys/class/gpio/gpio3/direction");

        char name[32];
        sprintf(name, "/dev/ttyS%d", g_tty);

        iFd = open(name, O_RDWR);  /* 读写方式打开串口 */

        if (iFd < 0) {
            return -1;
        }

        tcgetattr(iFd, &opt);
        cfmakeraw(&opt);

        cfsetispeed(&opt, iBdVal);
        cfsetospeed(&opt, iBdVal);

        tcsetattr(iFd, TCSANOW, &opt);

        return iFd;
    }


    static int _uart_read(char *pcBuf, int iBufLen) {
        int iFd = g_irs485, iLen = 0;
        int i;

        *pcBuf = '\0';
        printf("reading... from fd:%d\n", iFd);
        iLen = read(iFd, pcBuf, iBufLen);
        if (iLen < 0) {
            close(iFd);
            g_irs485 = -1;
            printf("error read from fd %d\n", iFd);
            return iLen;
        }

        //ignore 0x0
        while (iLen == 1 && pcBuf[0] == 0x0) {
            printf("ignore the 0x0 .\n");
            iLen = read(iFd, pcBuf, iBufLen);
        }

        printf("read success: iLen= %d , hex dump:\n", iLen);
        for (i = 0; i < iLen; i++) {
            printf("%02x ", pcBuf[i]);
        }
        printf("\nhex dump end.\n");


        while (1) {
            if ((iLen > 5) &&
                ('B' == pcBuf[iLen - 5]) &&
                ('B' == pcBuf[iLen - 4]) &&
                (' ' == pcBuf[iLen - 3]) &&
                ('E' == pcBuf[iLen - 2]) && ('F' == pcBuf[iLen - 1])) {
                printf("receive completed.\n");
                break;
            }

            printf("continue to read...\n");
            int next = read(iFd, pcBuf + iLen, iBufLen - iLen);
            iLen += next;
            printf("read next :%d, iLen:%d, buf:%s\n", next, iLen, pcBuf);

        }
        printf("total len = %d\n", iLen);

        for (i = 0; i < iLen; i++) {
            printf("%02x ", pcBuf[i]);
        }
        printf("\nhex dump end.\n");
        return iLen;
    }
}

class RSBusinessManage {
public:

    int do_str_req() {
        int opcode = std::stoi(m_str[3]);
        cout << "opcode: " << opcode << endl;
        switch (opcode) {
            case rs::TCPNETWORKSET:
                std::cout << "TCPNetworkSet" << std::endl;
                rs::TCPNetworkSet(m_str);
                break;
            case rs::UPDATEPWD:
                std::cout << "UpdatePwd" << std::endl;
                rs::UpdatePwd(m_str);
                break;
            case rs::NETWORKSET:
                std::cout << "NetworkSet" << std::endl;
                rs::NetworkSet(m_str);
                break;
            case rs::LOGIN:
                std::cout << "LOGIN" << std::endl;
                rs::Login(m_str);
                break;
            case rs::SETDEVICEADDRRESS:
                std::cout << "设置设备地址" << std::endl;
                rs::SettingDeviceAddress(m_str);
                break;
            case rs::AUDIOPLAY:
                std::cout << "AudioPlay" << std::endl;
                rs::AudioPlay(m_str);
                break;
            case rs::AUDIOSTOP:
                std::cout << "AudioStop" << std::endl;
                rs::AudioStop();
                break;
            case rs::VOLUMSET:
                std::cout << "VolumeSet" << std::endl;
                rs::VolumeSet(m_str);
                break;
            case rs::REBOOT:
                std::cout << "Reboot" << std::endl;
                rs::Reboot();
                break;
            case rs::GETDEVICESTATUS:
                std::cout << "GetDeviceStatus" << std::endl;
                rs::GetDeviceStatus();
                break;
            case rs::TTSPLAY:
                std::cout << "TTS语音播报" << std::endl;
                break;
            case rs::LIGHTSWITCH:
                std::cout << "闪灯开关" << std::endl;
                break;
            case rs::FILEUPLOAD:
                std::cout << "FileUpload" << std::endl;
                break;
            case rs::RESTORE:
                std::cout << "Restore" << std::endl;
                rs::Restore();
                break;
            case rs::AUDIONUMORTPLAY:
                std::cout << "AudioNumberOrTimePlay" << std::endl;
                rs::AudioNumberOrTimePlay(m_str);
                break;
            case rs::TTSNUMORTPLAY:
                std::cout << "文字时长/次数播报" << std::endl;
                break;
            case rs::GETRSDeviceBaseInfo:
                std::cout << "GetRSDeviceBaseInfo" << std::endl;
                rs::GetRSDeviceBaseInfo();
                break;
            case rs::RECORD:
                std::cout << "Record" << std::endl;
                //rs::Record(m_str);
                break;
            case rs::REMOTEUPGRADE:
                std::cout << "远程升级" << std::endl;
                break;
            case rs::GETAUDIOLIST:
                std::cout << "GetAudioList" << std::endl;
                rs::GetAudioList(m_str);
                break;
            case rs::LIGHTCONFIG:
                std::cout << "闪灯配置" << std::endl;
                break;
            case rs::RECORDBEGIN:
                std::cout << "RecordBegin" << std::endl;
                rs::RecordBegin(m_str);
                break;
            case rs::RECORDEND:
                std::cout << "RecordEnd" << std::endl;
                rs::RecordEnd(m_str);
                break;
            case rs::AUDIOFILEUPLOAD:
                std::cout << "AudioFileUpload" << std::endl;
                rs::AudioFileUpload(m_str);
                break;
            case rs::REMOTEFILEUPGRADE:
                std::cout << "RemoteFileUpgrade" << std::endl;
                rs::RemoteFileUpgrade(m_str);
                break;
            default:
                std::cout << "switch F4" << std::endl;
                rs::SendFast("F4");
                break;
        }
        return 1;
    }

    int handle_receive(char *buf, int len) {
        std::istringstream ss(buf);
        std::string token;
        m_str.clear();
        while (std::getline(ss, token, ' ')) {
            m_str.push_back(token);
        }
        if (m_str[0].compare("AA") != 0 || m_str[m_str.size() - 1].compare("EF") != 0) {
            rs::SendFast("F1");
            return 0;
        } else if (std::stoi(m_str[1]) != (m_str.size() - 3)) {
            rs::SendFast("F2");
            return 0;
        } else if (m_str[m_str.size() - 2].compare("BB") != 0) {
            rs::SendFast("F3");
            return 0;
        }
        return do_str_req();
    }

    int worker() {

        int fd = rs::_uart_open();
        if (fd < 0) {
            printf("failed to open ttyS%d to read write.\n", g_tty);
            return -1;
        }
        g_irs485 = fd;


        int readcount = 0;
        char buf[1024] = {0};
        while (1) {
            memset(buf, 0, sizeof(buf));

            rs::set_receive_dir();
            readcount = rs::_uart_read(buf, sizeof(buf));

            if (readcount < 0) {
                printf("failed to read ! errno = %d, strerror=%s\n", errno, strerror(errno));
                return 0;
            }
            printf("recv request:%s, len:%d, handle it...\n", buf, readcount);

            rs::set_send_dir();
            handle_receive(buf, readcount);
        }
    }

private:
    std::vector<std::string> m_str;
};
