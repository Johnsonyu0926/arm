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
#include "unsupport.hpp"

#include "getDeviceBaseInfo.hpp"
#include "reboot.hpp"
#include "updatePwd.hpp"
#include "restore.hpp"
#include "audioStop.hpp"
#include "audioPlay.hpp"
#include "ttsPlay.hpp"
#include "fileUpload.hpp"
#include "ptzOperate.hpp"
#include "directional_sound_column.hpp"
#include "relaySet.hpp"
#include "audio_del.hpp"
#include "micRecord.hpp"
#include "public.hpp"

using namespace asns;

extern DWORD total_kilo;
#define BUF_LEN 2048 // FOR fragment.
using json = nlohmann::json;

int CClientThread::do_req(char *buf, CSocket *pClient) {
    if (strstr(buf, "Login")) {
        DS_TRACE("do login business.");
        CLogin login = m_json;
        login.do_req(pClient);
    } else if (strstr(buf, "NetworkSet")) {
        DS_TRACE("do network set business.");
        CNetworkSet networkSet = m_json;
        networkSet.do_req(pClient);
    } else if (strstr(buf, "ServerSet")) {
        DS_TRACE("do server set business.");
        CServerSet serverSet = m_json;
        serverSet.do_req(pClient);
    } else if (strstr(buf, "VolumeSet")) {
        DS_TRACE("do VolumeSet business.");
        CVolumeSet volumeSet = m_json;
        volumeSet.do_req(pClient);
    } else if (strstr(buf, "GetAudioList")) {
        DS_TRACE("GetAudioList cmd.");
        CGetAudioList getAudioList = m_json;
        getAudioList.do_req(pClient);
    } else if (strstr(buf, "GetDeviceBaseInfo")) {
        DS_TRACE("GetDeviceBaseInfo cmd");
        CGetDeviceBaseInfo deviceBaseInfo = m_json;
        deviceBaseInfo.do_req(pClient);
    } else if (strstr(buf, "AudioPlay")) {
        DS_TRACE("AudioPlay cmd");
        CAudioPlay audioPlay = m_json;
        audioPlay.do_req(pClient);
    } else if (strstr(buf, "TtsPlay")) {
        DS_TRACE("TtsPlay cmd");
        CTtsPlay ttsPlay = m_json;
        ttsPlay.do_req(pClient);
    } else if (strstr(buf, "FileUpload")) {
        DS_TRACE("FileUpload cmd");
        CFileUpload fileUpload = m_json;
        fileUpload.do_req(pClient);
    } else if (strstr(buf, "AudioStop")) {
        DS_TRACE("AudioStop cmd");
        CAudioStop audioStop = m_json;
        audioStop.do_req(pClient);
    } else if (strstr(buf, "Reboot")) {
        DS_TRACE("Reboot cmd");
        CReboot reboot = m_json;
        reboot.do_req(pClient);
    } else if (strstr(buf, "Restore")) {
        DS_TRACE("Restore cmd");
        CRestore restore = m_json;
        restore.do_req(pClient);
    } else if (strstr(buf, "UpdatePwd")) {
        DS_TRACE("UpdatePwd cmd");
        CUpdate update = m_json;
        update.do_req(pClient);
    }//{"duration":"5","uploadUrl":"http://192.168.85.1:8091/iot/1v1/api/v1/micRecordUpload","cmd":"MicRecord"}
    else if (strstr(buf, "MicRecord")) {
        DS_TRACE("MicRecord cmd");
        CMicRecord res = m_json;
        res.do_req(pClient);
    }
        //{"cmd":"AudioDelete","deleteName":"23.mp3","storageType":1}
    else if (strstr(buf, "AudioDelete")) {
        DS_TRACE("AudioDelete command.");
        CDeleteAudio res = m_json;
        res.do_req(pClient);
    } else if (strstr(buf, "PtzOperate")) {
        DS_TRACE("PtzOperate command.");
        CPtzOperate res = m_json;
        res.do_req();
    } else if (strstr(buf, "RelaySet")) {
        DS_TRACE("RelaySet command.");
        CRelaySet res = m_json;
        res.do_req(pClient);
    } else {
        DS_TRACE("unsupport command:" << buf);
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
        DS_TRACE("error parse:" << buf);
        return 0;
    }
    DS_TRACE("json is parse ok:" << m_json.dump().c_str());
    return 1;
}

int CClientThread::do_str_req(CSocket *pClient) {
    CBusiness bus;
    int opcode = std::stoi(m_str[3]);
    DS_TRACE("opcode: " << opcode);
    switch (opcode) {
        case TCPNETWORKSET:
            DS_TRACE("TCPNetworkSet");
            bus.TCPNetworkSet(m_str, pClient);
            break;
        case UPDATEPWD:
            DS_TRACE("UpdatePwd");
            bus.UpdatePwd(m_str, pClient);
            break;
        case NETWORKSET:
            DS_TRACE("NetworkSet");
            bus.NetworkSet(m_str, pClient);
            break;
        case LOGIN:
            DS_TRACE("LOGIN");
            bus.Login(m_str, pClient);
            break;
        case SETDEVICEADDRRESS:
            DS_TRACE("设置设备地址");
            break;
        case AUDIOPLAY:
            DS_TRACE("AudioPlay");
            bus.AudioPlay(m_str, pClient);
            break;
        case AUDIOSTOP:
            DS_TRACE("AudioStop");
            bus.AudioStop(pClient);
            break;
        case VOLUMSET:
            DS_TRACE("VolumeSet");
            bus.VolumeSet(m_str, pClient);
            break;
        case REBOOT:
            DS_TRACE("Reboot");
            bus.Reboot(pClient);
            break;
        case GETDEVICESTATUS:
            DS_TRACE("GetDeviceStatus");
            bus.GetDeviceStatus(pClient);
            break;
        case TTSPLAY:
            DS_TRACE("TtsPlay");
            bus.TtsPlay(m_str, pClient);
            break;
        case LIGHTSWITCH:
            DS_TRACE("闪灯开关");
            break;
        case FILEUPLOAD:
            DS_TRACE("fileUpload");
            bus.FileUpload(m_str, pClient);
            break;
        case RESTORE:
            DS_TRACE("Restore");
            bus.Restore(pClient);
            break;
        case AUDIONUMORTPLAY:
            DS_TRACE("AudioNumberOrTimePlay");
            bus.AudioNumberOrTimePlay(m_str, pClient);
            break;
        case TTSNUMORTPLAY:
            DS_TRACE("Tts Number or Time Play");
            bus.TtsNumTimePlay(m_str, pClient);
            break;
        case GETDEVICEBASEINFO:
            DS_TRACE("GetDeviceBaseInfo");
            bus.GetDeviceBaseInfo(pClient);
            break;
        case RECORD:
            DS_TRACE("Record");
            bus.Record(m_str, pClient);
            break;
        case REMOTEUPGRADE:
            DS_TRACE("远程升级");
            break;
        case GETAUDIOLIST:
            DS_TRACE("GetAudioList");
            bus.GetAudioList(m_str, pClient);
            break;
        case LIGHTCONFIG:
            DS_TRACE("FlashConfig");
            bus.FlashConfig(m_str, pClient);
            break;
        case RECORDBEGIN:
            DS_TRACE("RecordBegin");
            bus.RecordBegin(m_str, pClient);
            break;
        case RECORDEND:
            DS_TRACE("RecordEnd");
            bus.RecordEnd(m_str, pClient);
            break;
        case AUDIOFILEUPLOAD:
            DS_TRACE("AudioFileUpload");
            bus.AudioFileUpload(m_str, pClient);
            DS_TRACE("do_str_req end ...");
            break;
        case REMOTEFILEUPGRADE:
            DS_TRACE("RemoteFileUpgrade");
            bus.RemoteFileUpgrade(m_str, pClient);
            break;
        default:
            DS_TRACE("switch F4");
            bus.SendFast(asns::NONSUPPORT_ERROR, pClient);
            break;
    }
    return 1;
}

int CClientThread::do_str_verify(char *buf, CSocket *pClient) {
    CBusiness bus;
    CUtils utils;
    m_str = utils.string_split(buf);
    if (m_str[0].compare("AA") != 0 || m_str[m_str.size() - 1].compare("EF") != 0) {
        bus.SendFast(asns::BEGIN_END_CODE_ERROR, pClient);
        return 0;
    } else if (std::stoi(m_str[1]) != (m_str.size() - 3)) {
        bus.SendFast(asns::LENGTH_ERROR, pClient);
        return 0;
    } else if (m_str[m_str.size() - 2].compare("BB") != 0) {
        bus.SendFast(asns::CHECK_CODE_ERROR, pClient);
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
        DS_TRACE("audioserver begin ...");
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
                DS_TRACE("read buf:" << buf);
                if (strstr(buf, "AA") && strstr(buf, "EF")) {
                    if (do_str_verify(buf, m_pClient)) {
                        p = buf;
                        offset = 0;
                        do_str_req(m_pClient);
                        DS_TRACE("InitInstance do_str_req end");
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
