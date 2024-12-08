#pragma once

#include <map>
#include <functional>
#include <string>
#include <unordered_map>

#include "mosquittopp.h"
#include "mqtt_reTemp.hpp"
#include "mqtt_audioPlay.hpp"
#include "mqtt_fileUpload.hpp"
#include "mqtt_ttsPlay.hpp"
#include "mqtt_recordPlay.hpp"
#include "mqtt_volumeSet.hpp"
#include "mqtt_fileDelete.hpp"
#include "mqtt_heartbeat.hpp"
#include "mqtt_boot.hpp"
#include "mqtt_reboot.hpp"
#include "mqtt_audioStop.hpp"
#include "mqtt_planPlay.hpp"
#include "mqtt_audioStreamStart.hpp"
#include "mqtt_operateCmd.hpp"
#include "mqtt_ledShowSet.hpp"
#include "mqtt_micRecordUpload.hpp"
#include "mqtt_gpioSet.hpp"
#include "audiocfg.hpp"
#include "VolumeAdaptSchemaSet.hpp"
#include "mqtt_volumeAdaptSchemaSet.hpp"
#include "mqtt_baudSet.hpp"

class ServiceManage {
public:
    static constexpr int DEL_SIZE = 21;
    static constexpr const char* DEL_STR = "\"data\"";

    using msgHandler = std::function<std::string(const json&)>;

    static ServiceManage& instance() {
        static ServiceManage serviceManage;
        return serviceManage;
    }

    msgHandler getHandler(const std::string& str) {
        auto it = m_fn.find(str);
        if (it != m_fn.end()) {
            return it->second;
        } else {
            return [=](const json& js) -> std::string {
                json res;
                res["result"] = "no such cmd";
                res["resultId"] = 2;
                res["imei"] = js["imei"];
                res["topic"] = asns::REQUEST_TOPIC + js["imei"].get<std::string>();
                res["cmd"] = js["cmd"];
                res["publishId"] = js["publishId"];
                return res.dump();
            };
        }
    }

    std::string heartBeat() {
        asns::CHeartBeatData heartBeat;
        heartBeat.do_success();
        json js = heartBeat;
        return js.dump();
    }

    std::string boot() {
        asns::CBootData bootData;
        bootData.do_success();
        json js = bootData;
        return js.dump();
    }

    ~ServiceManage() = default;

private:
    ServiceManage() {
        m_fn.emplace("audioPlay", [&](const json& js) -> std::string {
            asns::CReQuest<asns::CAudioPlayData, asns::CAudioPlayResultData> req = js;
            std::string reStr = req.do_fail_success();
            reStr.erase(reStr.find(DEL_STR), DEL_SIZE);
            return reStr;
        });
        m_fn.emplace("ttsPlay", [&](const json& js) -> std::string {
            asns::CReQuest<asns::CTtsPlayData, asns::CTtsPlayResultData> req = js;
            std::string reStr = req.do_fail_success();
            reStr.erase(reStr.find(DEL_STR), DEL_SIZE);
            return reStr;
        });
        m_fn.emplace("recordPlay", [&](const json& js) -> std::string {
            asns::CReQuest<asns::CRecordPlayData, asns::CRecordPlayResultData> req = js;
            std::string reStr = req.do_fail_success();
            reStr.erase(reStr.find(DEL_STR), DEL_SIZE);
            return reStr;
        });
        m_fn.emplace("fileUpload", [&](const json& js) -> std::string {
            asns::CReQuest<asns::CFileUploadData, asns::CFileUploadResultData> req = js;
            std::string reStr = req.do_fail_success();
            return reStr;
        });
        m_fn.emplace("volumeSet", [&](const json& js) -> std::string {
            asns::CReQuest<asns::CVolumeSetData, asns::CVolumeSetResultData> req = js;
            std::string reStr = req.do_fail_success();
            return reStr;
        });
        m_fn.emplace("fileDelete", [&](const json& js) -> std::string {
            asns::CReQuest<asns::CFileDeleteData, asns::CFileDeleteResultData> req = js;
            std::string reStr = req.do_fail_success();
            return reStr;
        });
        m_fn.emplace("reboot", [&](const json& js) -> std::string {
            asns::CReQuest<asns::CRebootData, asns::CRebootResultData> req = js;
            std::string reStr = req.do_fail_success();
            reStr.erase(reStr.find(DEL_STR), DEL_SIZE);
            return reStr;
        });
        m_fn.emplace("audioStreamTest", [&](const json& js) -> std::string {
            asns::CReQuest<asns::CPlanPlayData, asns::CPlanPlayResultData> req = js;
            std::string reStr = req.do_fail_success();
            reStr.erase(reStr.find(DEL_STR), DEL_SIZE);
            return reStr;
        });
        m_fn.emplace("audioStop", [&](const json& js) -> std::string {
            asns::CReQuest<asns::CAudioStopData, asns::CAudioStopResultData> req = js;
            std::string reStr = req.do_fail_success();
            reStr.erase(reStr.find(DEL_STR), DEL_SIZE);
            return reStr;
        });
        m_fn.emplace("audioStreamStart", [&](const json& js) -> std::string {
            asns::CReQuest<asns::CAudioStreamStartData, asns::CAudioStreamStartResultData> req = js;
            std::string reStr = req.do_fail_success();
            reStr.erase(reStr.find(DEL_STR), DEL_SIZE);
            return reStr;
        });
        m_fn.emplace("ptzOperate", [&](const json& js) -> std::string {
            asns::CReQuest<asns::CPtzOperateData, asns::CPtzOperateResultData> req = js;
            std::string reStr = req.do_fail_success();
            reStr.erase(reStr.find(DEL_STR), DEL_SIZE);
            return reStr;
        });
        m_fn.emplace("ledShowSet", [&](const json& js) -> std::string {
            asns::CReQuest<asns::CLedShowSetData, asns::CLedShowSetResultData> req = js;
            std::string reStr = req.do_fail_success();
            return reStr;
        });
        m_fn.emplace("micRecordUpload", [&](const json& js) -> std::string {
            asns::CReQuest<asns::CMicRecordUploadData, asns::CMicRecordUploadResultData> req = js;
            std::string reStr = req.do_fail_success();
            return reStr;
        });
        m_fn.emplace("gpioSet", [&](const json& js) -> std::string {
            asns::CReQuest<asns::CGpioSet, asns::CGpioSetResultData> req = js;
            std::string reStr = req.do_fail_success();
            return reStr;
        });
        m_fn.emplace("volumeAdaptSchemaSet", [&](const json& js) -> std::string {
            asns::CReQuest<asns::CVolumeAdaptSchemaData, asns::CVolumeAdaptSchemaSet> req = js;
            std::string reStr = req.do_fail_success();
            reStr.erase(reStr.find(DEL_STR), DEL_SIZE);
            return reStr;
        });
        m_fn.emplace("configurationSet", [&](const json& js) -> std::string {
            asns::CReQuest<asns::CBaudData, asns::CBaudResultData> req = js;
            std::string reStr = req.do_fail_success();
            reStr.erase(reStr.find(DEL_STR), DEL_SIZE);
            return reStr;
        });
    }

    std::unordered_map<std::string, msgHandler> m_fn;
};