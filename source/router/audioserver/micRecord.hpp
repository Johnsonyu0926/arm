#pragma once

#include "json.hpp"
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

    class CMicRecord {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CMicRecord, cmd, duration, uploadUrl)

        int do_req(CSocket *pClient) {
            std::string imei = "11111";
            std::string filePath = "/tmp/record.mp3";
            system("killall -9 arecord");
            CUtils utils;
            if (utils.get_process_status("madplay")) {
                CMicRecordResult result;
                result.do_fail("Currently playing");
                json js = result;
                std::string res = js.dump();
                return pClient->Send(res.c_str(), res.length());
            }
            //curl --location --request POST 'http://192.168.10.2:8091/iot/1v1/api/v1/micRecordUpload' --form 'FormDataUploadFile=@"/tmp/record.mp3"' --form 'imei="11111"'
            system("arecord -f cd /tmp/record.mp3 &");
            std::thread([&] {
                std::this_thread::sleep_for(std::chrono::seconds(std::atoi(duration.c_str())));
                system("killall -9 arecord");
            }).join();
            std::string res = utils.get_doupload_result(uploadUrl, imei);
            std::cout << "result:" << res << std::endl;
            if (res.empty() || res.find("error") != std::string::npos) {
                CMicRecordResult result;
                result.do_fail("post error");
                json js = result;
                std::string res = js.dump();
                return pClient->Send(res.c_str(), res.length());
            } else if (res.find("true") != std::string::npos) {
                CMicRecordResult result;
                result.do_success();
                json js = result;
                std::string res = js.dump();
                return pClient->Send(res.c_str(), res.length());
            }else{
                CMicRecordResult result;
                result.do_fail("post error");
                json js = result;
                std::string res = js.dump();
                return pClient->Send(res.c_str(), res.length());
            }
        }

    private:
        std::string cmd;
        std::string duration;
        std::string uploadUrl;
    };
}
