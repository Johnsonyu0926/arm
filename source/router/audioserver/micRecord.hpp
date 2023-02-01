#pragma once

#include "json.hpp"
#include "audiocfg.hpp"
//{"duration":"5","uploadUrl":"http://192.168.85.1:8091/iot/1v1/api/v1/micRecordUpload","cmd":"MicRecord"}
namespace asns {

    class CMicRecordResult {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CMicRecordResult, cmd, resultId, msg)

        int do_success() {
            cmd = "MicRecord";
            resultId = 1;
            msg = "success";
        }

        int do_fail(const std::string str) {
            cmd = "MicRecord";
            resultId = 2;
            msg = str;
        }

    private:
        std::string cmd;
        int resultId;
        std::string msg;
    };
    class CMicRecordThread : public CSThread {
    public:
        virtual BOOL InitInstance() {
            CUtils utils;
            sleep(std::atoi(duration.c_str()));
            system("killall -9 arecord");
            std::string res = utils.get_doupload_result(uploadUrl, imei);
            std::cout << "result:" << res << std::endl;
            system("rm /tmp/record.mp3");
            if (res.find("true") != std::string::npos) {
                CMicRecordResult result;
                result.do_success();
                json js = result;
                std::string res = js.dump();
                return pClient->Send(res.c_str(), res.length());
            } else {
                CMicRecordResult result;
                result.do_fail("post error");
                json js = result;
                std::string res = js.dump();
                return pClient->Send(res.c_str(), res.length());
            }
        }

        virtual BOOL ExitInstance() {
            if (pClient) {
                delete pClient;
                pClient = NULL;
            }
            return TRUE;
        }

        void setClient(CSocket *pClient) { this->pClient = pClient; }
        void setUploadUrl(std::string uploadUrl) { this->uploadUrl = uploadUrl; }
        void setImei(std::string imei) { this->imei = imei; }
        void setDuration(std::string duration) { this->duration = duration; }

    private:
        CSocket *pClient;
        std::string uploadUrl;
        std::string imei;
        std::string duration;
    };
    class CMicRecord {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CMicRecord, cmd, duration, uploadUrl)

        int do_req(CSocket *pClient) {
            CAudioCfgBusiness bus;
            std::string imei = bus.business[0].deviceID;
            std::string filePath = "/tmp/record.mp3";
            system("killall -9 arecord");
            CUtils utils;
            if (utils.get_process_status("madplay") || utils.get_process_status("aplay")) {
                CMicRecordResult result;
                result.do_fail("Currently playing");
                json js = result;
                std::string res = js.dump();
                return pClient->Send(res.c_str(), res.length());
            }
            //curl --location --request POST 'http://192.168.10.2:8091/iot/1v1/api/v1/micRecordUpload' --form 'FormDataUploadFile=@"/tmp/record.mp3"' --form 'imei="11111"'
            system("arecord -f cd /tmp/record.mp3 &");
            CMicRecordThread *pThread = new CMicRecordThread();
            pThread->setUploadUrl(uploadUrl);
            pThread->setImei(imei);
            pThread->setDuration(duration);
            pThread->setClient(pClient);
            pThread->CreateThread();
        }

    private:
        std::string cmd;
        std::string duration;
        std::string uploadUrl;
    };

    class CSThread;


}
