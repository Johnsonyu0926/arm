#pragma once

#include <map>
#include <functional>
#include <string>

#include "mosquittopp.h"
#include "mqtt_reTemp.hpp"
#include "mqtt_audioPlay.hpp"
#include "mqtt_fileUpload.hpp"
#include "mqtt_ttsPlay.hpp"
//#include "mqtt_recordPlay.hpp"
#include "mqtt_volumeSet.hpp"
#include "mqtt_fileDelete.hpp"
#include "mqtt_heartbeat.hpp"
#include "mqtt_boot.hpp"
#include "mqtt_reboot.hpp"
#include "mqtt_heartbeat.hpp"
#include "mqtt_audioStop.hpp"
//#include "mqtt_planPlay.hpp"
#include "mqtt_audioStreamStart.hpp"
#include "mqtt_operateCmd.hpp"
#include "mqtt_ledShowSet.hpp"
#include "mqtt_micRecordUpload.hpp"
#include "audiocfg.hpp"

class ServiceManage {
public:
    const int DEL_SIZE = 11;

    explicit ServiceManage() {
        m_fn.insert(std::make_pair("audioPlay", [&](const json &js) -> std::string {
            asns::CReQuest<asns::CAudioPlayData, asns::CAudioPlayResultData> req = js;
            std::string reStr = req.do_req();
            reStr.erase(reStr.find("\"null\""), DEL_SIZE);
            return reStr;
        }));
        m_fn.insert(std::make_pair("ttsPlay", [&](const json &js) -> std::string {
            asns::CReQuest<asns::CTtsPlayData, asns::CTtsPlayResultData> req = js;
            std::string reStr = req.do_req();
            reStr.erase(reStr.find("\"null\""), DEL_SIZE);
            return reStr;
        }));
        /*m_fn.insert(std::make_pair("recordPlay", [&](const json &js) -> std::string {
            asns::CReQuest<asns::CRecordPlayData, asns::CRecordPlayResultData> req = js;
            std::string reStr = req.do_req();
            reStr.erase(reStr.find("\"null\""), DEL_SIZE);
            return reStr;
        }));*/
        m_fn.insert(std::make_pair("fileUpload", [&](const json &js) -> std::string {
            asns::CReQuest<asns::CFileUploadData, asns::CFileUploadResultData> req = js;
            std::string reStr = req.do_req();
            return reStr;
        }));
        m_fn.insert(std::make_pair("volumeSet", [&](const json &js) -> std::string {
            asns::CReQuest<asns::CVolumeSetData, asns::CVolumeSetResultData> req = js;
            std::string reStr = req.do_req();
            reStr.erase(reStr.find("\"null\""), DEL_SIZE);
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
            reStr.erase(reStr.find("\"null\""), DEL_SIZE);
            return reStr;
        }));
        /*m_fn.insert(std::make_pair("planPlay", [&](const json &js) -> std::string {
            asns::CReQuest<asns::CPlanPlayData, asns::CPlanPlayResultData> req = js;
            std::string reStr = req.do_req();
            reStr.erase(reStr.find("\"null\""), 11);
            return reStr;
        }));*/
        m_fn.insert(std::make_pair("audioStop", [&](const json &js) -> std::string {
            asns::CReQuest<asns::CAudioStopData, asns::CAudioStopResultData> req = js;
            std::string reStr = req.do_req();
            reStr.erase(reStr.find("\"null\""), DEL_SIZE);
            return reStr;
        }));
        m_fn.insert(std::make_pair("audioStreamStart", [&](const json &js) -> std::string {
            asns::CReQuest<asns::CAudioStreamStartData, asns::CAudioStreamStartResultData> req = js;
            std::string reStr = req.do_req();
            reStr.erase(reStr.find("\"null\""), DEL_SIZE);
            return reStr;
        }));
        m_fn.insert(std::make_pair("ptzOperate", [&](const json &js) -> std::string {
            asns::CReQuest<asns::CPtzOperateData, asns::CPtzOperateResultData> req = js;
            std::string reStr = req.do_req();
            reStr.erase(reStr.find("\"null\""), DEL_SIZE);
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

    virtual ~ServiceManage() {}

public:
    std::map<std::string, std::function<std::string(const json &js)>> m_fn;
};