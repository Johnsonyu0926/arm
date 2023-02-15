#pragma once
#include "directional_sound_column.hpp"

class RSBusinessManage {
public:
    int do_str_req() {
        CBusiness bus;
        int opcode = std::stoi(m_str[3]);
        cout << "opcode: " << opcode << endl;
        switch (opcode) {
            case asns::TCPNETWORKSET:
                std::cout << "TCPNetworkSet" << std::endl;
                bus.TCPNetworkSet(m_str);
                break;
            case asns::UPDATEPWD:
                std::cout << "UpdatePwd" << std::endl;
                bus.UpdatePwd(m_str);
                break;
            case asns::NETWORKSET:
                std::cout << "NetworkSet" << std::endl;
                bus.NetworkSet(m_str);
                break;
            case asns::LOGIN:
                std::cout << "LOGIN" << std::endl;
                bus.Login(m_str);
                break;
            case SETDEVICEADDRRESS:
                std::cout << "设置设备地址" << std::endl;
                break;
            case asns::AUDIOPLAY:
                std::cout << "AudioPlay" << std::endl;
                bus.AudioPlay(m_str);
                break;
            case asns::AUDIOSTOP:
                std::cout << "AudioStop" << std::endl;
                bus.AudioStop();
                break;
            case asns::VOLUMSET:
                std::cout << "VolumeSet" << std::endl;
                bus.VolumeSet(m_str);
                break;
            case asns::REBOOT:
                std::cout << "Reboot" << std::endl;
                bus.Reboot();
                break;
            case asns::GETDEVICESTATUS:
                std::cout << "GetDeviceStatus" << std::endl;
                bus.GetDeviceStatus();
                break;
            case asns::TTSPLAY:
                std::cout << "TtsPlay" << std::endl;
                bus.TtsPlay(m_str);
                break;
            case LIGHTSWITCH:
                std::cout << "闪灯开关" << std::endl;
                break;
            case asns::FILEUPLOAD:
                std::cout << "fileUpload" << std::endl;
                bus.FileUpload(m_str);
                break;
            case asns::RESTORE:
                std::cout << "Restore" << std::endl;
                bus.Restore();
                break;
            case asns::AUDIONUMORTPLAY:
                std::cout << "AudioNumberOrTimePlay" << std::endl;
                bus.AudioNumberOrTimePlay(m_str);
                break;
            case asns::TTSNUMORTPLAY:
                std::cout << "Tts Number or Time Play" << std::endl;
                bus.TtsNumTimePlay(m_str);
                break;
            case asns::GETDEVICEBASEINFO:
                std::cout << "GetDeviceBaseInfo" << std::endl;
                bus.GetDeviceBaseInfo();
                break;
            case asns::RECORD:
                std::cout << "Record" << std::endl;
                bus.Record(m_str);
                break;
            case asns::REMOTEUPGRADE:
                std::cout << "远程升级" << std::endl;
                break;
            case asns::GETAUDIOLIST:
                std::cout << "GetAudioList" << std::endl;
                bus.GetAudioList(m_str);
                break;
            case asns::LIGHTCONFIG:
                std::cout << "FlashConfig" << std::endl;
                bus.FlashConfig(m_str);
                break;
            case asns::RECORDBEGIN:
                std::cout << "RecordBegin" << std::endl;
                bus.RecordBegin(m_str);
                break;
            case asns::RECORDEND:
                std::cout << "RecordEnd" << std::endl;
                bus.RecordEnd(m_str);
                break;
            case asns::AUDIOFILEUPLOAD:
                std::cout << "AudioFileUpload" << std::endl;
                bus.AudioFileUpload(m_str);
                break;
            case asns::REMOTEFILEUPGRADE:
                std::cout << "RemoteFileUpgrade" << std::endl;
                bus.RemoteFileUpgrade(m_str);
                break;
            default:
                std::cout << "switch F4" << std::endl;
                CUtils utils;
                utils.uart_write(asns::NONSUPPORT_ERROR);
                break;
        }
        return 1;
    }

    int handle_receive(const char *buf) {
        CUtils utils;
        m_str = utils.string_split(buf);
        if (m_str[0].compare("AA") != 0 || m_str[m_str.size() - 1].compare("EF") != 0) {
            utils.uart_write(asns::BEGIN_END_CODE_ERROR);
            return 0;
        } else if (std::stoi(m_str[1]) != (m_str.size() - 3)) {
            utils.uart_write(asns::LENGTH_ERROR);
            return 0;
        } else if (m_str[m_str.size() - 2].compare("BB") != 0) {
            utils.uart_write(asns::CHECK_CODE_ERROR);
            return 0;
        }
        return do_str_req();
    }

    int worker() {
        Rs485 rs;
        int fd = rs._uart_open();
        if (fd < 0) {
            return -1;
        }

        int read_count = 0;
        char buf[1024] = {0};
        while (1) {
            memset(buf, 0, sizeof(buf));
            read_count = rs._uart_read(buf, sizeof(buf));

            if (read_count < 0) {
                printf("failed to read ! errno = %d, strerror=%s\n", errno, strerror(errno));
                return 0;
            }
            printf("recv request:%s, len:%d, handle it...\n", buf, read_count);
            handle_receive(buf);
        }
    }

private:
    std::vector<std::string> m_str;
};
