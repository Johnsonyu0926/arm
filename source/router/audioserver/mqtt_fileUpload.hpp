#pragma once

#include <string>
#include "add_mqtt_custom_audio_file.hpp"
#include "json.hpp"
#include "audiocfg.hpp"
#include "utils.h"

namespace asns {

    template<typename Quest, typename Result>
    class CReQuest;

    template<typename T>
    class CResult;

    class CFileUploadResultData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CFileUploadResultData, audioUploadRecordId)

        template<typename Quest, typename Result, typename T>
        int do_success(const CReQuest<Quest, Result> &c, CResult<T> &r) {
            audioUploadRecordId = c.data.audioUploadRecordId;
            CUtils utils;
            CAudioCfgBusiness cfg;
            unsigned long long availableDisk = utils.get_available_Disk(cfg.getAudioFilePath());
            LOG(INFO) << "disk size: " << availableDisk << "kb";
            int disk = availableDisk - 500;
            if (!c.data.fileSize.empty()) {
                int size = std::atoll(c.data.fileSize.c_str());
                if (size != 0 && size > disk) {
                    r.resultId = 2;
                    r.result = "Lack of space";
                    return 2;
                }
            }
            std::string res = utils.get_upload_result(c.data.downloadUrl.c_str(), cfg.getAudioFilePath().c_str(), c.data.fileName.c_str());
            if (res == "success") {
                CAddMqttCustomAudioFileData data;
                data.setName(c.data.fileName);
                data.setAudioUploadRecordId(c.data.audioUploadRecordId);
                CAddMqttCustomAudioFileBusiness business;
                utils.bit_rate_32_to_48(cfg.getAudioFilePath() + c.data.fileName);
                if (!business.exist(c.data.fileName)) {
                    business.business.push_back(data);
                    business.saveJson();
                }
                r.resultId = 1;
                r.result = "success";
                return 1;
            } else {
                r.resultId = 2;
                r.result = res;
                return 2;
            }
        }

    private:
        int audioUploadRecordId{0};
    };

    class CFileUploadData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CFileUploadData, downloadUrl, fileName, audioUploadRecordId, storageType, fileSize)

        std::string downloadUrl;
        std::string fileName;
        int audioUploadRecordId{0};
        int storageType{0};
        std::string fileSize{""};
    };
}