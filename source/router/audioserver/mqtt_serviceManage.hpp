#pragma once

#include <map>
#include <functional>
#include <string>

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
#include "mqtt_heartbeat.hpp"
#include "mqtt_audioStop.hpp"
#include "mqtt_planPlay.hpp"
#include "mqtt_audioStreamStart.hpp"
#include "mqtt_operateCmd.hpp"
#include "mqtt_ledShowSet.hpp"
#include "mqtt_micRecordUpload.hpp"
#include "mqtt_gpioSet.hpp"
#include "audiocfg.hpp"

class ServiceManage {
public:
    const int DEL_SIZE = 21;
    const std::string DEL_STR = "\"data\"";
    using msgHandler = std::function<std::string(const json &)>;

    static ServiceManage &instance() {
        static ServiceManage serviceManage;
        return serviceManage;
    }

    msgHandler getHandler(std::string str) {
        auto it = m_fn.find(str);
        if (it != m_fn.end()) {
            //如果找到了消息id对应的处理函数就把函数返回
            return m_fn[str];
        } else {
            //没找到就返回一个空操作的处理函数
            return [=](const json &js) -> std::string {
                json res;
                res["result"] = "no such cmd";
                res["resultId"] = 2;
                res["imei"] = js["imei"];
                res["topic"] = "TaDiao/device/report/test/" + js["imei"].get<std::string>();
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
    explicit ServiceManage() {
        m_fn.insert(std::make_pair("audioPlay", [&](const json &js) -> std::string {
            asns::CReQuest<asns::CAudioPlayData, asns::CAudioPlayResultData> req = js;
            std::string reStr = req.do_req();
            reStr.erase(reStr.find(DEL_STR), DEL_SIZE);
            return reStr;
        }));
        m_fn.insert(std::make_pair("ttsPlay", [&](const json &js) -> std::string {
            asns::CReQuest<asns::CTtsPlayData, asns::CTtsPlayResultData> req = js;
            std::string reStr = req.do_req();
            reStr.erase(reStr.find(DEL_STR), DEL_SIZE);
            return reStr;
        }));
        m_fn.insert(std::make_pair("recordPlay", [&](const json &js) -> std::string {
            asns::CReQuest<asns::CRecordPlayData, asns::CRecordPlayResultData> req = js;
            std::string reStr = req.do_req();
            reStr.erase(reStr.find(DEL_STR), DEL_SIZE);
            return reStr;
        }));
        m_fn.insert(std::make_pair("fileUpload", [&](const json &js) -> std::string {
            asns::CReQuest<asns::CFileUploadData, asns::CFileUploadResultData> req = js;
            std::string reStr = req.do_req();
            return reStr;
        }));
        m_fn.insert(std::make_pair("volumeSet", [&](const json &js) -> std::string {
            asns::CReQuest<asns::CVolumeSetData, asns::CVolumeSetResultData> req = js;
            std::string reStr = req.do_req();
            return reStr;
        }));
        m_fn.insert(std::make_pair("fileDelete", [&](const json &js) -> std::string {
            asns::CReQuest<asns::CFileDeleteData, asns::CFileDeleteResultData> req = js;
            std::string reStr = req.do_req();
            return reStr;
        }));
        m_fn.insert(std::make_pair("reboot", [&](const json &js) -> std::string {
            asns::CReQuest<asns::CRebootData, asns::CRebootResultData> req = js;
            std::string reStr = req.do_req();
            reStr.erase(reStr.find(DEL_STR), DEL_SIZE);
            return reStr;
        }));
        m_fn.insert(std::make_pair("audioStreamTest", [&](const json &js) -> std::string {
            asns::CReQuest<asns::CPlanPlayData, asns::CPlanPlayResultData> req = js;
            std::string reStr = req.do_req();
            reStr.erase(reStr.find(DEL_STR), DEL_SIZE);
            return reStr;
        }));
        m_fn.insert(std::make_pair("audioStop", [&](const json &js) -> std::string {
            asns::CReQuest<asns::CAudioStopData, asns::CAudioStopResultData> req = js;
            std::string reStr = req.do_req();
            reStr.erase(reStr.find(DEL_STR), DEL_SIZE);
            return reStr;
        }));
        m_fn.insert(std::make_pair("audioStreamStart", [&](const json &js) -> std::string {
            asns::CReQuest<asns::CAudioStreamStartData, asns::CAudioStreamStartResultData> req = js;
            std::string reStr = req.do_req();
            reStr.erase(reStr.find(DEL_STR), DEL_SIZE);
            return reStr;
        }));
        m_fn.insert(std::make_pair("ptzOperate", [&](const json &js) -> std::string {
            asns::CReQuest<asns::CPtzOperateData, asns::CPtzOperateResultData> req = js;
            std::string reStr = req.do_req();
            reStr.erase(reStr.find(DEL_STR), DEL_SIZE);
            return reStr;
        }));
        m_fn.insert(std::make_pair("ledShowSet", [&](const json &js) -> std::string {
            asns::CReQuest<asns::CLedShowSetData, asns::CLedShowSetResultData> req = js;
            std::string reStr = req.do_req();
            return reStr;
        }));
        m_fn.insert(std::make_pair("micRecordUpload", [&](const json &js) -> std::string {
            asns::CReQuest<asns::CMicRecordUploadData, asns::CMicRecordUploadResultData> req = js;
            std::string reStr = req.do_req();
            return reStr;
        }));
        m_fn.insert(std::make_pair("gpioSet", [&](const json &js) -> std::string {
            asns::CReQuest<asns::CGpioSet, asns::CGpioSetResultData> req = js;
            std::string reStr = req.do_req();
            return reStr;
        }));
    }

public:
    std::unordered_map<std::string, std::function<std::string(const json &js)>> m_fn;
};