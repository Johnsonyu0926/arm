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
                DS_TRACE("TCPNetworkSet");
                bus.TCPNetworkSet(m_str);
                break;
            case asns::UPDATEPWD:
                DS_TRACE("UpdatePwd");
                bus.UpdatePwd(m_str);
                break;
            case asns::NETWORKSET:
                DS_TRACE("NetworkSet");
                bus.NetworkSet(m_str);
                break;
            case asns::LOGIN:
                DS_TRACE("LOGIN");
                bus.Login(m_str);
                break;
            case SETDEVICEADDRRESS:
                DS_TRACE("设置设备地址");
                break;
            case asns::AUDIOPLAY:
                DS_TRACE("AudioPlay");
                bus.AudioPlay(m_str);
                break;
            case asns::AUDIOSTOP:
                DS_TRACE("AudioStop");
                bus.AudioStop();
                break;
            case asns::VOLUMSET:
                DS_TRACE("VolumeSet");
                bus.VolumeSet(m_str);
                break;
            case asns::REBOOT:
                DS_TRACE("Reboot");
                bus.Reboot();
                break;
            case asns::GETDEVICESTATUS:
                DS_TRACE("GetDeviceStatus");
                bus.GetDeviceStatus();
                break;
            case asns::TTSPLAY:
                DS_TRACE("TtsPlay");
                bus.TtsPlay(m_str);
                break;
            case LIGHTSWITCH:
                DS_TRACE("闪灯开关");
                break;
            case asns::FILEUPLOAD:
                DS_TRACE("fileUpload");
                bus.FileUpload(m_str);
                break;
            case asns::RESTORE:
                DS_TRACE("Restore");
                bus.Restore();
                break;
            case asns::AUDIONUMORTPLAY:
                DS_TRACE("AudioNumberOrTimePlay");
                bus.AudioNumberOrTimePlay(m_str);
                break;
            case asns::TTSNUMORTPLAY:
                DS_TRACE("Tts Number or Time Play");
                bus.TtsNumTimePlay(m_str);
                break;
            case asns::GETDEVICEBASEINFO:
                DS_TRACE("GetDeviceBaseInfo");
                bus.GetDeviceBaseInfo();
                break;
            case asns::RECORD:
                DS_TRACE("Record");
                bus.Record(m_str);
                break;
            case asns::REMOTEUPGRADE:
                DS_TRACE("远程升级");
                break;
            case asns::GETAUDIOLIST:
                DS_TRACE("GetAudioList");
                bus.GetAudioList(m_str);
                break;
            case asns::LIGHTCONFIG:
                DS_TRACE("FlashConfig");
                bus.FlashConfig(m_str);
                break;
            case asns::RECORDBEGIN:
                DS_TRACE("RecordBegin");
                bus.RecordBegin(m_str);
                break;
            case asns::RECORDEND:
                DS_TRACE("RecordEnd");
                bus.RecordEnd(m_str);
                break;
            case asns::AUDIOFILEUPLOAD:
                DS_TRACE("AudioFileUpload");
                bus.AudioFileUpload(m_str);
                break;
            case asns::REMOTEFILEUPGRADE:
                DS_TRACE("RemoteFileUpgrade");
                bus.RemoteFileUpgrade(m_str);
                break;
            default:
                DS_TRACE("switch F4");
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
