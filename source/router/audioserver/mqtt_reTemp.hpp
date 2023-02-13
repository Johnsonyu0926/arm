#pragma once

#include "json.hpp"

/*
 * publishId: 172684, // Long,本次下发唯一ID
    cmd: "audioPlay", // String,本次下发功能标识；详见功能标识
    imei: "869298057401606" // String,设备唯一序列号
    operatorId: 1, // Long,操作者id
    data: "";


    {
    "result":"播放成功", // 处理结果
    "resultId":1, // 处理结果标记；0-表示未处理；1-表示处理成功；2-表示处理失败；
    "imei":"869298057401606", // 设备唯一序列号
    "topic":"TaDiao/device/report/test/869298057401606", // topic
    "cmd":"audioPlay", // 当次处理的功能标识
    "publishId":172684, // 平台下发的本次请求id
    "data": "" // 处理完成上传数据，如果有则需要上传，详情参照详细功能通信指令，已JSON格式体展现
}
 */

namespace asns {
    template<typename Quest, typename Result>
    class CReQuest;

    template<typename T>
    class CResult {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CResult, result, resultId, imei, topic, cmd, publishId, data)

        template<typename Quest, typename Result>
        void do_success(const CReQuest<Quest, Result> &c) {
            this->data.template do_success<Quest, Result, T>(c, *this);
            imei = c.imei;
            topic = "TaDiao/device/report/test/" + imei;
            cmd = c.cmd;
            publishId = c.publishId;
        }

    public:
        std::string result;
        int resultId{};
        std::string imei;
        std::string topic;
        std::string cmd;
        long publishId{};
        T data;
    };

    template<typename Quest, typename Result>
    class CReQuest {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CReQuest, publishId, cmd, imei, operatorId, data)

        std::string do_fail_success() {
            CResult<Result> res;
            res.template do_success<Quest, Result>(*this);
            json js = res;
            return js.dump();
        }

    public:
        long publishId{};
        std::string cmd;
        std::string imei;
        long operatorId{};
        Quest data;
    };

}