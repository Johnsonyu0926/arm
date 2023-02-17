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

        template<typename Quest, typename Result, typename T>
        int do_success(const CReQuest<Quest, Result> &c, CResult<T> &r) {
            CUtils utils;
            std::string res = utils.record_upload(c.data.recordDuration,c.data.requestUrl, c.data.imei);
            DS_TRACE("result:" << res.c_str());
            if (res.empty() || res.find("error") != std::string::npos) {
                uploadStatus = 0;
                micRecordId = 0;
                r.resultId = 2;
                r.result = "download error";
                return 2;
            } else if (res.find("uploadStatus") != std::string::npos) {
                res = res.substr(res.find_first_of('{'), res.find_last_of('}') - res.find_first_of('{') + 1);
                json js = json::parse(res);
                uploadStatus = js.at("uploadStatus");
                micRecordId = js.at("micRecordId");
            }
            r.resultId = 1;
            r.result = "success";
            return 1;
        }

    public:
        int uploadStatus;
        long micRecordId;
    };


    class CMicRecordUploadData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CMicRecordUploadData, imei, requestUrl, recordDuration)

    public:
        std::string imei;
        std::string requestUrl;
        int recordDuration;
    };
}