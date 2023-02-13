#pragma once

#include "add_mqtt_custom_audio_file.hpp"
#include "json.hpp"
#include "audiocfg.hpp"
#include "utils.h"

/*
 * {
    "downloadUrl":"http://172.16.1.20:9999/data/voice/cd5710aa-aef4-4e86-b0a0-2e430c1a2dc9.mp3", // 音频文件下载路径
    "fileName":"103.mp3", // 文件存储名称
    "audioUploadRecordId":434, // 文件上传记录唯一ID
    "storageType":1
}
curl --location --request GET 'http://218.108.199.238:9999/data/test01.mp3' --output 1.mp3
 {
    "audioUploadRecordId":434, // 文件上传记录唯一ID
}
 */
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
            std::cout << "disk size:" << availableDisk << "kb" << std::endl;
            int disk = availableDisk - 500;
            if (!c.data.fileSize.empty()) {
                int size = std::atoll(c.data.fileSize.c_str());
                if (size != 0 && size > disk) {
                    r.resultId = 2;
                    r.result = "Lack of space";
                    return 2;
                }
            }
            std::string res = utils.get_upload_result(c.data.downloadUrl.c_str(), cfg.getAudioFilePath().c_str(),
                                                      c.data.fileName.c_str());
            std::cout << "res:-----" << res << std::endl;
            if (res.find("error") != std::string::npos) {
                r.resultId = 2;
                r.result = "file upload error";
                return 2;
            } else if (res.find("Failed writing body") != std::string::npos) {
                r.resultId = 2;
                r.result = "file upload Failed writing body error";
                return 2;
            } else {
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
            }
        }

    private:
        int audioUploadRecordId;
    };

    class CFileUploadData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CFileUploadData, downloadUrl, fileName, audioUploadRecordId,
                                                    storageType, fileSize)

    public:
        std::string downloadUrl;
        std::string fileName;
        int audioUploadRecordId;
        int storageType;
        std::string fileSize{""};
    };
}