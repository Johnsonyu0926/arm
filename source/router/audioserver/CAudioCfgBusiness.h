#ifndef __CLIENT_THREAD_H__
#define __CLIENT_THREAD_H__

#include "json.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <openssl/md5.h>
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
#include "spdlog/spdlog.h"

using json = nlohmann::json;

class CSThread;
class CSocket;

class CClientThread : public CSThread {
public:
    virtual BOOL InitInstance() override;
    virtual BOOL ExitInstance() override;

    void SetClient(CSocket *pClient) { m_pClient = pClient; }

private:
    CSocket *m_pClient{nullptr};
    json m_json;
    std::vector<std::string> m_str;

    BOOL Check(const unsigned char *szBuf);
    BOOL Gen(BYTE *szBuf); // make sure the buf size is 1024 Bytes.
    int do_req(char *buf, CSocket *pClient);
    int do_verify(char *buf);
    int do_str_req(CSocket *pClient);
    int do_str_verify(char *buf, CSocket *pClient);
};

#endif // __CLIENT_THREAD_H__