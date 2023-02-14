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
#include "volume.hpp"
#include "audiocfg.hpp"
#include "add_column_custom_audio_file.hpp"
#include "talk.h"
#include "login.hpp"
#include "utils.h"
#include "gpio.hpp"
#include "playStatus.hpp"
#include "rsTcpTransferThread.hpp"


extern int g_playing_priority;
static int g_irs485 = -1;
static int g_tty = 1; //ttyUSB1 as default.
#define MAX_SEND 12

class RSDeviceBaseInfo {
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
        asns::CAudioCfgBusiness cfg;
        cfg.load();
        CUtils util;
        codeVersion = cfg.business[0].codeVersion; //"2.1.01"; //"1.2";
        coreVersion = util.get_by_cmd_res("webs -V");
        relayMode = CGpio::getInstance().getGpioModel();
        ip = util.get_lan_addr();
        storageType = util.is_ros_platform() ? 0 : 1;
        port = 34508;
        playStatus = PlayStatus::getInstance().getPlayId() == asns::STOP_TASK_PLAYING ? 0 : 1;
        g_volumeSet.load();
        volume = g_volumeSet.getVolume();
        relayStatus = CGpio::getInstance().getGpioStatus();
        hardwareReleaseTime = util.get_by_cmd_res("uname -a");
        spiFreeSpace = storageType ? 9752500 : 0;
        flashFreeSpace = util.get_available_Disk("/mnt");
        hardwareVersion = "7621";
        hardwareModelId = 2;
        password = cfg.business[0].serverPassword;
        temperature = 12;
        netmask = util.get_lan_netmask();
        address = "01";
        gateway = util.get_lan_gateway();
        userName = "admin";
        imei = cfg.business[0].deviceID;
        functionVersion = "COMMON";
        deviceCode = cfg.business[0].deviceID;
        serverAddress = cfg.business[0].server;
        serverPort = to_string(cfg.business[0].port);
    }

public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(RSDeviceBaseInfo, codeVersion, coreVersion, relayMode, ip, storageType, port,
                                   playStatus, volume, relayStatus, hardwareReleaseTime, spiFreeSpace,
                                   flashFreeSpace, hardwareVersion, password, temperature, netmask, address,
                                   gateway, userName, imei, functionVersion, deviceCode, serverAddress, serverPort,
                                   hardwareModelId)
};

class RSDeviceBaseState {
public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(RSDeviceBaseState, relayStatus, volume, storageType)

    int do_success() {
        CUtils utils;
        relayStatus = utils.get_process_status("madplay");
        g_volumeSet.load();
        volume = g_volumeSet.getVolume();
        storageType = 1;
    }

private:
    int relayStatus;
    int volume;
    int storageType;
};


namespace rs {

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
        std::string code = err_code;
        std::string buf = "01 " + code;
        _uart_write(buf.c_str(), buf.length());
        return 0;
    }

    int Login(const std::vector<std::string> &m_str) {
        CAudioCfgBusiness cfg;
        cfg.load();
        std::cout << m_str[4] << " " << cfg.business[0].serverPassword << " " << m_str[5] << std::endl;
        if (m_str[4].compare("admin") == 0 && m_str[5] == cfg.business[0].serverPassword) {
            std::cout << "return login ok" << std::endl;
            return SendTrue();
        } else {
            std::cout << "return login error" << std::endl;
            return SendFast(asns::USER_PWD_ERROR);
        }
    }

    // AA 04 01 01 23.mp3 BB EF
    int AudioPlay(std::vector<std::string> &m_str) {
        CUtils utils;
        if (utils.get_process_status("madplay")) {
            return SendFast(asns::ALREADY_PLAYED);
        }
        std::string name = m_str[4].substr(0, m_str[4].find_first_of('.'));
        CAddColumnCustomAudioFileBusiness busines;
        if (!utils.str_is_all_num(name) || std::stoi(name) > 100 || std::stoi(name) < 0) {
            //音频文件名校验失败
            return SendFast(asns::OPERATION_NEW_AUDIO_FILE_NAME_ERROR);
        } else if (!busines.isNameEmpty(m_str[4])) {
            //音频文件查询不存在
            return SendFast(asns::AUDIO_FILE_NOT_EXITS);
        } else if (g_playing_priority != NON_PLAY_PRIORITY) {
            /*设备正在播放中，触发失败*/
            return SendFast(asns::ALREADY_PLAYED);
        } else {
            if (utils.get_process_status("aplay")) {
                utils.audio_stop();
            }
            char cmd[256] = {0};
            CAudioCfgBusiness cfg;
            cfg.load();
            utils.audio_loop_play(cfg.getAudioFilePath() + m_str[4], asns::ASYNC_START);
            return SendTrue();
        }
    }

    int AudioStop() {
        CUtils utils;
        utils.audio_stop();
        return SendTrue();
    }

    int VolumeSet(const std::vector<std::string> &m_str) {
        int volume = std::stoi(m_str[4]);
        if (volume > 7 || volume < 0) {
            //音量值不在范围
            return SendFast(asns::VOLUME_RANGE_ERROR);
        } else {
            CVolumeSet volumeSet;
            volumeSet.setVolume(volume);
            volumeSet.addj(volume);
            volumeSet.saveToJson();
            return SendTrue();
        }
    }

    int Reboot() {
        SendTrue();
        CUtils utils;
        utils.reboot();
        return 1;
    }

    int GetDeviceStatus() {
        RSDeviceBaseState deviceState;
        deviceState.do_success();
        json res = deviceState;
        std::string str = "01 E1 " + res.dump();
        rs::_uart_write(str.c_str(), str.length());
    }

    int Restore() {
        CUtils utils;
        CAudioCfgBusiness cfg;
        cfg.load();
        cfg.business[0].serverPassword = "Aa123456";
        cfg.business[0].server = "192.168.1.90";
        cfg.business[0].port = 7681;
        cfg.saveToJson();
        SendTrue();
        utils.restore(cfg.business[0].savePrefix);
        return 1;
    }

    int FlashConfig(const std::vector<std::string> &m_str) {
        CUtils utils;
        utils.gpio_set(std::stoi(m_str[4].c_str()), std::stoi(m_str[5].c_str()));
        return SendTrue();
    }

    int TtsPlay(const std::vector<std::string> &m_str) {
        CUtils utils;
        if (utils.get_process_status("madplay") || utils.get_process_status("aplay")) {
            return SendFast(asns::ALREADY_PLAYED);
        }
        std::string txt = utils.hex_to_string(m_str[4]);
        if (utils.statistical_character_count(txt) > 100) {
            return SendFast(asns::TTS_TXT_LENGTH_ERROR);
        }
        utils.txt_to_audio(txt);
        SendTrue();
        utils.tts_loop_play(asns::ASYNC_START);
    }

    int tts(const int playType, const int duration, const std::string &txt) {
        CUtils utils;
        utils.txt_to_audio(txt);
        switch (playType) {
            case 0: {
                SendTrue();
                utils.tts_loop_play();
                return 0;
            }
            case 1: {
                if (duration < 1) {
                    return SendFast(asns::NONSUPPORT_ERROR);
                }
                utils.tts_num_play(duration);
                break;
            }
            case 2: {
                if (duration < 1) {
                    return SendFast(asns::NONSUPPORT_ERROR);
                }
                utils.tts_time_play(duration);
                break;
            }
            default:
                return SendFast(asns::NONSUPPORT_ERROR);
        }
        SendTrue();
    }

    int TtsNumTimePlay(const std::vector<std::string> &m_str) {
        CUtils utils;
        if (utils.get_process_status("madplay") || utils.get_process_status("aplay") ||
            utils.get_process_status("tts")) {
            return SendFast(asns::ALREADY_PLAYED);
        }
        std::string txt = utils.hex_to_string(m_str[4]);
        if (utils.statistical_character_count(txt) > 100) {
            return SendFast(asns::TTS_TXT_LENGTH_ERROR);
        }
        std::cout << "tts size:" << txt.length() << "txt: " << txt << std::endl;
        int playType = std::stoi(m_str[5]);
        int duration = std::stoi(m_str[6]);
        utils.async_wait(1, 0, 0, tts, playType, duration, txt);
    }

    int AudioNumberOrTimePlay(const std::vector<std::string> &m_str) {
        CUtils utils;
        if (utils.get_process_status("madplay")) {
            return SendFast(asns::ALREADY_PLAYED);
        }
        std::string name = m_str[4].substr(0, m_str[4].find_first_of('.'));
        CAddColumnCustomAudioFileBusiness busines;
        if (!utils.str_is_all_num(name) || std::stoi(name) > 100 || std::stoi(name) < 0) {
            //音频文件名校验失败
            return SendFast(asns::OPERATION_NEW_AUDIO_FILE_NAME_ERROR);
        } else if (g_playing_priority != NON_PLAY_PRIORITY) {
            return SendFast(asns::ALREADY_PLAYED);
        } else if (!busines.isNameEmpty(m_str[4])) {
            return SendFast(asns::AUDIO_FILE_NOT_EXITS);
        } else {
            int duration = std::stoi(m_str[6]);
            CAudioCfgBusiness cfg;
            cfg.load();
            if (utils.get_process_status("aplay")) {
                utils.audio_stop();
            }
            switch (std::stoi(m_str[5])) {
                case 0: {
                    utils.audio_loop_play(cfg.getAudioFilePath() + m_str[4], asns::ASYNC_START);
                    break;
                }
                case 1: {
                    if (duration < 1) {
                        return SendFast(asns::NONSUPPORT_ERROR);
                    }
                    utils.audio_num_play(duration, cfg.getAudioFilePath() + m_str[4], asns::ASYNC_START);
                    break;
                }
                case 2: {
                    if (duration < 1) {
                        return SendFast(asns::NONSUPPORT_ERROR);
                    }
                    utils.audio_time_play(duration, cfg.getAudioFilePath() + m_str[4], asns::ASYNC_START);
                    break;
                }
                default:
                    return SendFast(asns::NONSUPPORT_ERROR);
            }
            return SendTrue();
        }
    }

    int GetDeviceBaseInfo() {
        RSDeviceBaseInfo devInfo;
        devInfo.do_success();
        json res = devInfo;
        std::string str = "01 E1 " + res.dump();
        std::cout << "GetRSDeviceBaseInfo Res: " << str << std::endl;
        rs::_uart_write(str.c_str(), str.length());
    }

    int NetworkSet(const std::vector<std::string> &m_str) {
        CAudioCfgBusiness cfg;
        cfg.load();
        if (m_str[4].compare("admin") != 0 || m_str[5] != cfg.business[0].serverPassword) {
            return SendFast(asns::USER_PWD_ERROR);
        } else {
            CUtils utils;
            SendTrue();
            utils.network_set(m_str[7], m_str[6], m_str[8]);
        }
    }

    int TCPNetworkSet(const std::vector<std::string> &m_str) {
        CAudioCfgBusiness cfg;
        cfg.load();
        if (m_str[4].compare("admin") != 0 || m_str[5] != cfg.business[0].serverPassword) {
            return SendFast(asns::USER_PWD_ERROR);
        } else {
            CUtils utils;
            SendTrue();
            utils.network_set(m_str[7], m_str[6]);
            return 1;
        }
    }

    int UpdatePwd(const std::vector<std::string> &m_str) {
        CAudioCfgBusiness cfg;
        cfg.load();
        if (m_str[6].length() < 8) {
            return SendFast(asns::OPERATION_NEW_PWD_COMPLEXITY_ERROR);
        } else if (m_str[4].compare("admin") != 0 || m_str[5] != cfg.business[0].serverPassword) {
            return SendFast(asns::USER_PWD_ERROR);
        } else {
            cfg.business[0].serverPassword = m_str[6];
            cfg.saveToJson();
            CUtils utils;
            utils.change_password(m_str[6].c_str());
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
        CUtils utils;
        CAudioCfgBusiness bus;
        bus.load();
        std::string imei = bus.business[0].deviceID;
        if (utils.get_process_status("madplay") || utils.get_process_status("aplay")) {
            return SendFast(asns::ALREADY_PLAYED);
        }
        int time = std::stod(m_str[4]);
        if (time > 30 || time < 0) {
            return SendFast(asns::RECORD_TIME_ERROR);
        }
        std::string res = utils.record_upload(time, m_str[5], imei);
        std::cout << "result:" << res << std::endl;
        if (res.find("true") != std::string::npos) {
            return SendTrue();
        } else {
            return SendFast(asns::POST_ERROR);
        }
    }

    int RecordBegin(const std::vector<std::string> &m_str) {
        CUtils utils;
        if (utils.get_process_status("madplay") || utils.get_process_status("aplay")) {
            return SendFast(asns::ALREADY_PLAYED);
        }
        utils.record_start(ASYNC_START);
        return SendTrue();
    }

    int RecordEnd(const std::vector<std::string> &m_str) {
        CUtils utils;
        utils.record_stop();
        utils.volume_gain(asns::RECORD_PATH, "mp3");
        int file_size = utils.get_file_size(asns::RECORD_PATH);
        if (file_size <= 0) {
            return SendFast(asns::RECORD_SIZE_ERROR);
        }
        RsTcpTransferThread *pServer = new RsTcpTransferThread();
        pServer->SetPort(asns::BEGINPORT);
        pServer->SetVecStr(m_str);
        pServer->CreateThread();
    }

    int FileUpload(std::vector<std::string> &m_str) {
        CUtils utils;
        std::string name = m_str[5].substr(0, m_str[5].find_first_of('.'));
        std::string suffix = m_str[5].substr(m_str[5].find_first_of('.') + 1);
        std::transform(suffix.begin(), suffix.end(), suffix.begin(), ::tolower);
        if (!utils.str_is_all_num(name) || std::stoi(name) > 100 || std::stoi(name) < 0) {
            //音频文件名校验失败
            return SendFast(asns::OPERATION_NEW_AUDIO_FILE_NAME_ERROR);
        }
        if (suffix.compare("mp3") != 0) {
            return SendFast(asns::AUDIO_SUFFIX_ERROR);
        }
        std::string temp = name + "." + suffix;
        CAudioCfgBusiness cfg;
        cfg.load();
        std::string path = cfg.getAudioFilePath() + temp;
        std::string res = utils.get_upload_result(m_str[4].c_str(), cfg.getAudioFilePath().c_str(), temp.c_str());
        std::cout << "res:-----" << res << std::endl;
        if (res.find("error") != std::string::npos) {
            return SendFast(asns::OPERATION_FAIL_ERROR);
        } else if (res.find("Failed to connect") != std::string::npos) {
            return SendFast(asns::OPERATION_FAIL_ERROR);
        } else if (res.find("Couldn't connect to server") != std::string::npos) {
            return SendFast(asns::OPERATION_FAIL_ERROR);
        } else {
            utils.bit_rate_32_to_48(path);
            CAddColumnCustomAudioFileBusiness business;
            if (!business.exist(temp)) {
                CAddColumnCustomAudioFileData node;
                node.type = 32;
                node.setName(temp);
                node.size = utils.get_file_size(path);
                business.business.push_back(node);
                business.saveJson();
            } else {
                int size = utils.get_file_size(path);
                business.updateSize(temp, size);
            }
            SendTrue();
            return 1;
        }
    }

    int AudioFileUpload(std::vector<std::string> &m_str) {
        CUtils utils;
        std::string name = m_str[6].substr(0, m_str[6].find_first_of('.'));
        std::string suffix = m_str[6].substr(m_str[6].find_first_of('.') + 1);
        std::transform(suffix.begin(), suffix.end(), suffix.begin(), ::tolower);
        if (!utils.str_is_all_num(name) || std::stoi(name) > 100 || std::stoi(name) < 0) {
            //音频文件名校验失败
            return SendFast(asns::OPERATION_NEW_AUDIO_FILE_NAME_ERROR);
        }
        if (suffix.compare("mp3") != 0) {
            return SendFast(asns::AUDIO_SUFFIX_ERROR);
        }

        RsTcpTransferThread *pServer = new RsTcpTransferThread();
        pServer->SetPort(asns::BEGINPORT);
        pServer->SetVecStr(m_str);
        pServer->CreateThread();
        std::cout << "AudioFileUpload end....." << std::endl;
        return 1;
    }

    int RemoteFileUpgrade(std::vector<std::string> &m_str) {
        RsTcpTransferThread *pServer = new RsTcpTransferThread();
        pServer->SetPort(asns::BEGINPORT);
        pServer->SetVecStr(m_str);
        pServer->CreateThread();
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
        sprintf(cmd, "stty -F /dev/ttyS%d %d", g_tty, iBdVal);
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

    int _uart_work(const char *buf, int len) {
        int fd = rs::_uart_open();
        if (fd < 0) {
            printf("failed to open ttyS%d to read write.\n", g_tty);
            return 2;
        }
        g_irs485 = fd;
        rs::set_send_dir();
        _uart_write(buf, len);
        return 1;
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
            case asns::TCPNETWORKSET:
                std::cout << "TCPNetworkSet" << std::endl;
                rs::TCPNetworkSet(m_str);
                break;
            case asns::UPDATEPWD:
                std::cout << "UpdatePwd" << std::endl;
                rs::UpdatePwd(m_str);
                break;
            case asns::NETWORKSET:
                std::cout << "NetworkSet" << std::endl;
                rs::NetworkSet(m_str);
                break;
            case asns::LOGIN:
                std::cout << "LOGIN" << std::endl;
                rs::Login(m_str);
                break;
            case SETDEVICEADDRRESS:
                std::cout << "设置设备地址" << std::endl;
                break;
            case asns::AUDIOPLAY:
                std::cout << "AudioPlay" << std::endl;
                rs::AudioPlay(m_str);
                break;
            case asns::AUDIOSTOP:
                std::cout << "AudioStop" << std::endl;
                rs::AudioStop();
                break;
            case asns::VOLUMSET:
                std::cout << "VolumeSet" << std::endl;
                rs::VolumeSet(m_str);
                break;
            case asns::REBOOT:
                std::cout << "Reboot" << std::endl;
                rs::Reboot();
                break;
            case asns::GETDEVICESTATUS:
                std::cout << "GetDeviceStatus" << std::endl;
                rs::GetDeviceStatus();
                break;
            case asns::TTSPLAY:
                std::cout << "TtsPlay" << std::endl;
                rs::TtsPlay(m_str);
                break;
            case LIGHTSWITCH:
                std::cout << "闪灯开关" << std::endl;
                break;
            case asns::FILEUPLOAD:
                std::cout << "fileUpload" << std::endl;
                rs::FileUpload(m_str);
                break;
            case asns::RESTORE:
                std::cout << "Restore" << std::endl;
                rs::Restore();
                break;
            case asns::AUDIONUMORTPLAY:
                std::cout << "AudioNumberOrTimePlay" << std::endl;
                rs::AudioNumberOrTimePlay(m_str);
                break;
            case asns::TTSNUMORTPLAY:
                std::cout << "Tts Number or Time Play" << std::endl;
                rs::TtsNumTimePlay(m_str);
                break;
            case asns::GETDEVICEBASEINFO:
                std::cout << "GetDeviceBaseInfo" << std::endl;
                rs::GetDeviceBaseInfo();
                break;
            case asns::RECORD:
                std::cout << "Record" << std::endl;
                rs::Record(m_str);
                break;
            case asns::REMOTEUPGRADE:
                std::cout << "远程升级" << std::endl;
                break;
            case asns::GETAUDIOLIST:
                std::cout << "GetAudioList" << std::endl;
                rs::GetAudioList(m_str);
                break;
            case asns::LIGHTCONFIG:
                std::cout << "FlashConfig" << std::endl;
                rs::FlashConfig(m_str);
                break;
            case asns::RECORDBEGIN:
                std::cout << "RecordBegin" << std::endl;
                rs::RecordBegin(m_str);
                break;
            case asns::RECORDEND:
                std::cout << "RecordEnd" << std::endl;
                rs::RecordEnd(m_str);
                break;
            case asns::AUDIOFILEUPLOAD:
                std::cout << "AudioFileUpload" << std::endl;
                rs::AudioFileUpload(m_str);
                break;
            case asns::REMOTEFILEUPGRADE:
                std::cout << "RemoteFileUpgrade" << std::endl;
                CUtils utils;
                rs::RemoteFileUpgrade(m_str);
                break;
            default:
                std::cout << "switch F4" << std::endl;
                rs::SendFast(asns::NONSUPPORT_ERROR);
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
            rs::SendFast(asns::BEGIN_END_CODE_ERROR);
            return 0;
        } else if (std::stoi(m_str[1]) != (m_str.size() - 3)) {
            rs::SendFast(asns::LENGTH_ERROR);
            return 0;
        } else if (m_str[m_str.size() - 2].compare("BB") != 0) {
            rs::SendFast(asns::CHECK_CODE_ERROR);
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
