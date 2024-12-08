#include "CClientThread.h"

int CClientThread::do_req(char *buf, CSocket *pClient) {
    if (strstr(buf, "Login")) {
        spdlog::info("do login business.");
        CLogin login = m_json;
        login.do_req(pClient);
    } else if (strstr(buf, "NetworkSet")) {
        spdlog::info("do network set business.");
        CNetworkSet networkSet = m_json;
        networkSet.do_req(pClient);
    } else if (strstr(buf, "ServerSet")) {
        spdlog::info("do server set business.");
        CServerSet serverSet = m_json;
        serverSet.do_req(pClient);
    } else if (strstr(buf, "VolumeSet")) {
        spdlog::info("do VolumeSet business.");
        CVolumeSet volumeSet = m_json;
        volumeSet.do_req(pClient);
    } else if (strstr(buf, "GetAudioList")) {
        spdlog::info("GetAudioList cmd.");
        CGetAudioList getAudioList = m_json;
        getAudioList.do_req(pClient);
    } else if (strstr(buf, "GetDeviceBaseInfo")) {
        spdlog::info("GetDeviceBaseInfo cmd");
        CGetDeviceBaseInfo deviceBaseInfo = m_json;
        deviceBaseInfo.do_req(pClient);
    } else if (strstr(buf, "AudioPlay")) {
        spdlog::info("AudioPlay cmd");
        CAudioPlay audioPlay = m_json;
        audioPlay.do_req(pClient);
    } else if (strstr(buf, "TtsPlay")) {
        spdlog::info("TtsPlay cmd");
        CTtsPlay ttsPlay = m_json;
        ttsPlay.do_req(pClient);
    } else if (strstr(buf, "FileUpload")) {
        spdlog::info("FileUpload cmd");
        CFileUpload fileUpload = m_json;
        fileUpload.do_req(pClient);
    } else if (strstr(buf, "AudioStop")) {
        spdlog::info("AudioStop cmd");
        CAudioStop audioStop = m_json;
        audioStop.do_req(pClient);
    } else if (strstr(buf, "Reboot")) {
        spdlog::info("Reboot cmd");
        CReboot reboot = m_json;
        reboot.do_req(pClient);
    } else if (strstr(buf, "Restore")) {
        spdlog::info("Restore cmd");
        CRestore restore = m_json;
        restore.do_req(pClient);
    } else if (strstr(buf, "UpdatePwd")) {
        spdlog::info("UpdatePwd cmd");
        CUpdate update = m_json;
        update.do_req(pClient);
    } else if (strstr(buf, "MicRecord")) {
        spdlog::info("MicRecord cmd");
        CMicRecord res = m_json;
        res.do_req(pClient);
    } else if (strstr(buf, "AudioDelete")) {
        spdlog::info("AudioDelete command.");
        CDeleteAudio res = m_json;
        res.do_req(pClient);
    } else if (strstr(buf, "PtzOperate")) {
        spdlog::info("PtzOperate command.");
        CPtzOperate res = m_json;
        res.do_req(pClient);
    } else if (strstr(buf, "RelaySet")) {
        spdlog::info("RelaySet command.");
        CRelaySet res = m_json;
        res.do_req(pClient);
    } else {
        spdlog::info("unsupport command: {}", buf);
        CUnsupportBusiness business = m_json;
        business.do_req(pClient);
    }
    return 0;
}

int CClientThread::do_verify(char *buf) {
    try {
        m_json = json::parse(buf, nullptr, false);
    } catch (const json::parse_error &ex) {
        spdlog::info("error parse: {} at byte {}", buf, ex.byte);
        return 0;
    }
    spdlog::info("json is parse ok: {}", m_json.dump());
    return 1;
}

int CClientThread::do_str_req(CSocket *pClient) {
    CBusiness bus;
    int opcode = std::stoi(m_str[3]);
    spdlog::info("opcode: {}", opcode);
    switch (opcode) {
        case TCPNETWORKSET:
            spdlog::info("TCPNetworkSet");
            bus.TCPNetworkSet(m_str, pClient);
            break;
        case UPDATEPWD:
            spdlog::info("UpdatePwd");
            bus.UpdatePwd(m_str, pClient);
            break;
        case NETWORKSET:
            spdlog::info("NetworkSet");
            bus.NetworkSet(m_str, pClient);
            break;
        case LOGIN:
            spdlog::info("LOGIN");
            bus.Login(m_str, pClient);
            break;
        case SETDEVICEADDRRESS:
            spdlog::info("设置设备地址");
            break;
        case AUDIOPLAY:
            spdlog::info("AudioPlay");
            bus.AudioPlay(m_str, pClient);
            break;
        case AUDIOSTOP:
            spdlog::info("AudioStop");
            bus.AudioStop(pClient);
            break;
        case VOLUMSET:
            spdlog::info("VolumeSet");
            bus.VolumeSet(m_str, pClient);
            break;
        case REBOOT:
            spdlog::info("Reboot");
            bus.Reboot(pClient);
            break;
        case GETDEVICESTATUS:
            spdlog::info("GetDeviceStatus");
            bus.GetDeviceStatus(pClient);
            break;
        case TTSPLAY:
            spdlog::info("TtsPlay");
            bus.TtsPlay(m_str, pClient);
            break;
        case LIGHTSWITCH:
            spdlog::info("闪灯开关");
            break;
        case FILEUPLOAD:
            spdlog::info("fileUpload");
            bus.FileUpload(m_str, pClient);
            break;
        case RESTORE:
            spdlog::info("Restore");
            bus.Restore(pClient);
            break;
        case AUDIONUMORTPLAY:
            spdlog::info("AudioNumberOrTimePlay");
            bus.AudioNumberOrTimePlay(m_str, pClient);
            break;
        case TTSNUMORTPLAY:
            spdlog::info("Tts Number or Time Play");
            bus.TtsNumTimePlay(m_str, pClient);
            break;
        case GETDEVICEBASEINFO:
            spdlog::info("GetDeviceBaseInfo");
            bus.GetDeviceBaseInfo(pClient);
            break;
        case RECORD:
            spdlog::info("Record");
            bus.Record(m_str, pClient);
            break;
        case REMOTEUPGRADE:
            spdlog::info("远程升级");
            break;
        case GETAUDIOLIST:
            spdlog::info("GetAudioList");
            bus.GetAudioList(m_str, pClient);
            break;
        case LIGHTCONFIG:
            spdlog::info("FlashConfig");
            bus.FlashConfig(m_str, pClient);
            break;
        case RECORDBEGIN:
            spdlog::info("RecordBegin");
            bus.RecordBegin(m_str, pClient);
            break;
        case RECORDEND:
            spdlog::info("RecordEnd");
            bus.RecordEnd(m_str, pClient);
            break;
        case AUDIOFILEUPLOAD:
            spdlog::info("AudioFileUpload");
            bus.AudioFileUpload(m_str, pClient);
            spdlog::info("do_str_req end ...");
            break;
        case REMOTEFILEUPGRADE:
            spdlog::info("RemoteFileUpgrade");
            bus.RemoteFileUpgrade(m_str, pClient);
            break;
        case DSPMANAGE:
            spdlog::info("DSP Management");
            bus.DspManagement(m_str, pClient);
            break;
        default:
            spdlog::info("switch F4");
            bus.SendFast(asns::NONSUPPORT_ERROR, pClient);
            break;
    }
    return 1;
}

#define DONE 0
#define UNCOMPLETE -1
#define LEN_ERR -2
#define CODE_ERR -3

int CClientThread::do_str_verify(char *buf, CSocket *pClient) {
    CBusiness bus;
    CUtils utils;
    m_str = utils.string_split(buf);
    if (m_str[0] != "AA" || m_str.back() != "EF") {
        bus.SendFast(asns::BEGIN_END_CODE_ERROR, pClient);
        return UNCOMPLETE;
    } else if (std::stoi(m_str[1]) != static_cast<int>(m_str.size() - 3)) {
        bus.SendFast(asns::LENGTH_ERROR, pClient);
        return LEN_ERR;
    } else if (m_str[m_str.size() - 2] != "BB") {
        bus.SendFast(asns::CHECK_CODE_ERROR, pClient);
        return CODE_ERR;
    }
    return DONE;
}

BOOL CClientThread::InitInstance() {
    fd_set rset;
    int n;
    char buf[8192] = {0};
    char *p = buf;
    int offset = 0;
    while (true) {
        spdlog::info("audioserver begin ...");
        FD_ZERO(&rset);
        FD_SET(m_pClient->m_hSocket, &rset);

        n = select(m_pClient->m_hSocket + 1, &rset, NULL, NULL, NULL);
        if (n > 0) {
            int n1 = m_pClient->Recv(p, sizeof(buf) - 1 - offset);
            if (n1 == 0) {
                spdlog::info("recv finish!");
                m_pClient->Close();
                return 0;
            } else if (n1 < 0) {
                spdlog::info("recv failed.");
                m_pClient->Close();
                return 0;
            } else {
                spdlog::info("read buf: {}", buf);
                if (strstr(buf, "AA") && strstr(buf, "EF")) {
                    int ret = do_str_verify(buf, m_pClient);
                    switch (ret) {
                        case DONE:
                            p = buf;
                            offset = 0;
                            do_str_req(m_pClient);
                            spdlog::info("InitInstance do_str_req end");
                            memset(buf, 0, sizeof(buf));
                            break;
                        case UNCOMPLETE:
                            offset += n1;
                            p += offset;
                            break;
                        default:
                            offset = 0;
                            memset(buf, 0, sizeof(buf));
                            spdlog::info("Ignore this message, ret = {}", ret);
                            break;
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
            if (n < 0) {
                spdlog::warn("select < 0");
                return 0;
            }
        }
    }
    return TRUE;
}

BOOL CClientThread::Check(const unsigned char *szBuf) {
    unsigned char szHash[16];
    MD5(szBuf, 1008, szHash);
    return memcmp(szHash, &(szBuf[1008]), 16) == 0;
}

BOOL CClientThread::ExitInstance() {
    if (m_pClient) {
        delete m_pClient;
        m_pClient = nullptr;
    }
    return TRUE;
}

BOOL CClientThread::Gen(BYTE *szBuf) {
    static unsigned long long key;
    for (int i = 0; i < 1008; i += 8) {
        key = key * static_cast<unsigned long long>(3141592621) + static_cast<unsigned long long>(663896637);
        memcpy(&(szBuf[i]), &key, 8);
    }
    MD5(szBuf, 1008, &(szBuf[1008]));
    return TRUE;
}