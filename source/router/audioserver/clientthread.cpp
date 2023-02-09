#include <stdio.h>
#include <iostream>
#include "doorsbase.h"
#include <sys/ioctl.h>
#include <fcntl.h>
#include <openssl/md5.h>
#include "json.hpp"

#include "clientthread.h"
#include "login.hpp"
#include "network.hpp"
#include "serverset.hpp"
#include "volume.hpp"
#include "getaudiolist.hpp"
#include "delaudio.hpp"
#include "unsupport.hpp"

#include "getDeviceBaseInfo.hpp"
#include "reboot.hpp"
#include "updatePwd.hpp"
#include "restore.hpp"
#include "audioStop.hpp"
#include "audioPlay.hpp"
#include "ttsPlay.hpp"
#include "fileUpload.hpp"
#include "record.hpp"
#include "ptzOperate.hpp"
#include "directional_sound_column.hpp"
#include "relaySet.hpp"
#include "audio_del.hpp"
#include "micRecord.hpp"

using namespace asns;

extern DWORD total_kilo;
#define BUF_LEN 2048 // FOR fragment.
using json = nlohmann::json;

int CClientThread::do_req(char *buf, CSocket *pClient) {
    if (strstr(buf, "Login")) {
        cout << "do login business." << endl;
        CLogin login = m_json;
        login.do_req(pClient);
    } else if (strstr(buf, "NetworkSet")) {
        cout << "do network set business." << endl;
        CNetworkSet networkSet = m_json;
        networkSet.do_req(pClient);
    } else if (strstr(buf, "ServerSet")) {
        cout << "do server set business." << endl;
        CServerSet serverSet = m_json;
        serverSet.do_req(pClient);
    } else if (strstr(buf, "VolumeSet")) {
        cout << "do VolumeSet business." << endl;
        CVolumeSet volumeSet = m_json;
        volumeSet.do_req(pClient);
    }
        // else if (strstr(buf, "AudioDelete"))
        // {
        //     cout << "AudioDelete command." << endl;
        //     CDelAudio delAudio = m_json;
        //     delAudio.do_req(pClient);
        // }
    else if (strstr(buf, "GetAudioList")) {
        cout << "GetAudioList cmd." << endl;
        CGetAudioList getAudioList = m_json;
        getAudioList.do_req(pClient);
    } else if (strstr(buf, "GetDeviceBaseInfo")) {
        std::cout << "GetDeviceBaseInfo cmd" << std::endl;
        CGetDeviceBaseInfo deviceBaseInfo = m_json;
        deviceBaseInfo.do_req(pClient);
    } else if (strstr(buf, "AudioPlay")) {
        std::cout << "AudioPlay cmd" << std::endl;
        CAudioPlay audioPlay = m_json;
        audioPlay.do_req(pClient);
    } else if (strstr(buf, "TtsPlay")) {
        std::cout << "TtsPlay cmd" << std::endl;
        CTtsPlay ttsPlay = m_json;
        ttsPlay.do_req(pClient);
    } else if (strstr(buf, "FileUpload")) {
        std::cout << "FileUpload cmd" << std::endl;
        CFileUpload fileUpload = m_json;
        fileUpload.do_req(pClient);
    } else if (strstr(buf, "AudioStop")) {
        std::cout << "AudioStop cmd" << std::endl;
        CAudioStop audioStop = m_json;
        audioStop.do_req(pClient);
    } else if (strstr(buf, "Reboot")) {
        std::cout << "Reboot cmd" << std::endl;
        CReboot reboot = m_json;
        reboot.do_req(pClient);
    } else if (strstr(buf, "Restore")) {
        std::cout << "Restore cmd" << std::endl;
        CRestore restore = m_json;
        restore.do_req(pClient);
    } else if (strstr(buf, "UpdatePwd")) {
        std::cout << "UpdatePwd cmd" << std::endl;
        CUpdate update = m_json;
        update.do_req(pClient);
    }/* else if (strstr(buf, "record")) {
        std::cout << "record cmd" << std::endl;
        CRecord recod = m_json;
        recod.do_req(pClient);
    }*/
        //{"duration":"5","uploadUrl":"http://192.168.85.1:8091/iot/1v1/api/v1/micRecordUpload","cmd":"MicRecord"}
    else if (strstr(buf, "MicRecord")) {
        std::cout << "MicRecord cmd" << std::endl;
        CMicRecord res = m_json;
        res.do_req(pClient);
    }
        //{"cmd":"AudioDelete","deleteName":"23.mp3","storageType":1}
    else if (strstr(buf, "AudioDelete")) {
        cout << "AudioDelete command." << endl;
        CDeleteAudio res = m_json;
        res.do_req(pClient);
    } else if (strstr(buf, "PtzOperate")) {
        cout << "PtzOperate command." << endl;
        CPtzOperate res = m_json;
        res.do_req();
    } else if (strstr(buf, "RelaySet")) {
        cout << "RelaySet command." << endl;
        CRelaySet res = m_json;
        res.do_req(pClient);
    } else {
        cout << "unsupport command:" << buf << endl;
        CUnsupportBusiness business = m_json;
        business.do_req(pClient);
    }
    return 0;
}

int CClientThread::do_verify(char *buf) {
    try {
        m_json = json::parse(buf, nullptr, false);
    }
    catch (...) {
        cout << "error parse:" << buf << endl;
        return 0;
    }
    cout << "json is parse ok:" << m_json.dump() << endl;
    return 1;
}

int CClientThread::do_str_req(CSocket *pClient) {
    int opcode = std::stoi(m_str[3]);
    cout << "opcode: " << opcode << endl;
    switch (opcode) {
        case TCPNETWORKSET:
            std::cout << "TCPNetworkSet" << std::endl;
            asns::TCPNetworkSet(m_str, pClient);
            break;
        case UPDATEPWD:
            std::cout << "UpdatePwd" << std::endl;
            asns::UpdatePwd(m_str, pClient);
            break;
        case NETWORKSET:
            std::cout << "NetworkSet" << std::endl;
            asns::NetworkSet(m_str, pClient);
            break;
        case LOGIN:
            std::cout << "LOGIN" << std::endl;
            asns::Login(m_str, pClient);
            break;
        case SETDEVICEADDRRESS:
            std::cout << "设置设备地址" << std::endl;
            //SettingDeviceAddress(m_str, pClient);
            break;
        case AUDIOPLAY:
            std::cout << "AudioPlay" << std::endl;
            asns::AudioPlay(m_str, pClient);
            break;
        case AUDIOSTOP:
            std::cout << "AudioStop" << std::endl;
            asns::AudioStop(pClient);
            break;
        case VOLUMSET:
            std::cout << "VolumeSet" << std::endl;
            asns::VolumeSet(m_str, pClient);
            break;
        case REBOOT:
            std::cout << "Reboot" << std::endl;
            asns::Reboot(pClient);
            break;
        case GETDEVICESTATUS:
            std::cout << "GetDeviceStatus" << std::endl;
            asns::GetDeviceStatus(pClient);
            break;
        case TTSPLAY:
            std::cout << "TtsPlay" << std::endl;
            asns::TtsPlay(m_str, pClient);
            break;
        case LIGHTSWITCH:
            std::cout << "闪灯开关" << std::endl;
            break;
        case FILEUPLOAD:
            std::cout << "fileUpload" << std::endl;
            asns::FileUpload(m_str, pClient);
            break;
        case RESTORE:
            std::cout << "Restore" << std::endl;
            asns::Restore(pClient);
            break;
        case AUDIONUMORTPLAY:
            std::cout << "AudioNumberOrTimePlay" << std::endl;
            asns::AudioNumberOrTimePlay(m_str, pClient);
            break;
        case TTSNUMORTPLAY:
            std::cout << "Tts Number or Time Play" << std::endl;
            asns::TtsNumTimePlay(m_str, pClient);
            break;
        case GETDEVICEBASEINFO:
            std::cout << "GetDeviceBaseInfo" << std::endl;
            asns::GetDeviceBaseInfo(pClient);
            break;
        case RECORD:
            std::cout << "Record" << std::endl;
            asns::Record(m_str, pClient);
            break;
        case REMOTEUPGRADE:
            std::cout << "远程升级" << std::endl;
            break;
        case GETAUDIOLIST:
            std::cout << "GetAudioList" << std::endl;
            asns::GetAudioList(m_str, pClient);
            break;
        case LIGHTCONFIG:
            std::cout << "FlashConfig" << std::endl;
            asns::FlashConfig(m_str,pClient);
            break;
        case RECORDBEGIN:
            std::cout << "RecordBegin" << std::endl;
            asns::RecordBegin(m_str, pClient);
            break;
        case RECORDEND:
            std::cout << "RecordEnd" << std::endl;
            asns::RecordEnd(m_str, pClient);
            break;
        case AUDIOFILEUPLOAD:
            std::cout << "AudioFileUpload" << std::endl;
            asns::AudioFileUpload(m_str, pClient);
            std::cout << "do_str_req end ..." << std::endl;
            break;
        case REMOTEFILEUPGRADE:
            std::cout << "RemoteFileUpgrade" << std::endl;
            CUtils utils;
            asns::RemoteFileUpgrade(m_str, pClient);
            break;
        default:
            std::cout << "switch F4" << std::endl;
            asns::SendFast("F4", pClient);
            break;
    }
    return 1;
}

int CClientThread::do_str_verify(char *buf, CSocket *pClient) {
    std::istringstream ss(buf);
    std::string token;
    m_str.clear();
    while (std::getline(ss, token, ' ')) {
        m_str.push_back(token);
    }
    if (m_str[0].compare("AA") != 0 || m_str[m_str.size() - 1].compare("EF") != 0) {
        asns::SendFast("F1", pClient);
        return 0;
    } else if (std::stoi(m_str[1]) != (m_str.size() - 3)) {
        asns::SendFast("F2", pClient);
        return 0;
    } else if (m_str[m_str.size() - 2].compare("BB") != 0) {
        asns::SendFast("F3", pClient);
        return 0;
    }
    return 1;
}

BOOL CClientThread::InitInstance() {
    //m_login_status = 0;

    fd_set rset;
    int n;
    char buf[8192];
    char *p = buf;
    int offset = 0;
    while (1) {
        std::cout<<"audioserver begin ..."<<std::endl;
        FD_ZERO(&rset);
        FD_SET(m_pClient->m_hSocket, &rset);

        n = select(m_pClient->m_hSocket + 1, &rset, NULL, NULL, NULL);
        if (n > 0) {
            int n1 = m_pClient->Recv(p, sizeof(buf) - 1 - offset);
            if (n1 == 0) {
                printf("recv finish!\n");
                m_pClient->Close();
                return 0;
            } else if (n1 < 0) {
                printf("recv failed.\n");
                m_pClient->Close();
                return 0;
            } else {
                std::cout << "read buf:" << buf << std::endl;
                if (strstr(buf, "AA") && strstr(buf, "EF")) {
                    if (do_str_verify(buf, m_pClient)) {
                        p = buf;
                        offset = 0;
                        do_str_req(m_pClient);
                        std::cout<<"InitInstance do_str_req end"<<std::endl;
                        memset(buf, 0, sizeof(buf));
                    } else {
                        offset += n1;
                        p += offset;
                    }
                } else {
                    if (do_verify(buf)) {
                        p = buf;
                        offset = 0;
                        do_req(buf, m_pClient);
                        memset(buf, 0, sizeof(buf));
                    } else {
                        offset += n1;
                        p += offset;
                    }
                }
            }
        } else {
            if (n == 0) {
            }

            if (n < 0) {
                printf("select <0\n");
                return 0;
            }
        }
    }
    return TRUE;
}

BOOL CClientThread::Check(const unsigned char *szBuf) {
    unsigned char szHash[16];
    MD5(szBuf, 1008, szHash);
    if (memcmp(szHash, &(szBuf[1008]), 16) == 0)
        return TRUE;

    return FALSE;
}

BOOL CClientThread::ExitInstance() {
    if (m_pClient) {
        delete m_pClient;
        m_pClient = NULL;
    }
    return TRUE;
}

BOOL CClientThread::Gen(BYTE *szBuf) {
    static unsigned long long key;
    for (int i = 0; i < 1008; i += 8) {
        key = key * (unsigned long long) (3141592621) + (unsigned long long) (663896637);
        memcpy(&(szBuf[i]), &key, 8);
    }
    MD5((const BYTE *) szBuf, 1008, &(szBuf[1008]));
    return TRUE;
}
