#pragma once

#include "json.hpp"
#include "mqtt_reTemp.hpp"
#include "utils.h"
#include <thread>

using json = nlohmann::json;

namespace asns {
    template<typename Quest, typename Result>
    class CReQuest;

    template<typename T>
    class CResult;

    class CMicRecordUploadResultData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CMicRecordUploadResultData, uploadStatus, micRecordId)

        template<typename Quest, typename Result,typename T>
        void do_success(const CReQuest<Quest, Result> &c, CResult<T> &r) {
            system("killall -9 arecord");
            CUtils utils;
            std::string res = utils.get_doupload_result(c.data.requestUrl, c.data.imei);
            std::cout << "result:" << res << std::endl;
            if (res.empty() || res.find("error") != std::string::npos) {
                uploadStatus = 0;
                micRecordId = 0;
            } else if (res.find("uploadStatus") != std::string::npos) {
                json js = json::parse(res);
                uploadStatus = js.at("uploadStatus");
                micRecordId = js.at("micRecordId");
            }
        }

    public:
        int uploadStatus;
        long micRecordId;
    };


    class CMicRecordUploadData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CMicRecordUploadData, imei, requestUrl, recordDuration)

        int do_req() {
            system("arecord -f cd /tmp/record.mp3 &");
            std::this_thread::sleep_for(std::chrono::seconds(recordDuration));
            return 1;
        }

    public:
        std::string imei;
        std::string requestUrl;
        int recordDuration;
    };
}