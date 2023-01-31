#pragma once

#include "add_mqtt_custom_audio_file.hpp"
#include "json.hpp"

/**
 * {
            "fileName":"101.mp3", // 要删除的文件名
        "audioUploadRecordId":417, // 该文件上传记录ID
        "storageType":1 // 该文件存储位置
    }

    {
        "audioUploadRecordId":434, // 文件上传记录唯一ID
    }

 */
namespace asns {
    template<typename Quest, typename Result>
    class CReQuest;
    template<typename T>
    class CResult;
    class CFileDeleteResultData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CFileDeleteResultData, audioUploadRecordId)

        template<typename Quest, typename Result,typename T>
        void do_success(const CReQuest<Quest, Result> &c, CResult<T> &r) {
            audioUploadRecordId = c.data.audioUploadRecordId;
        }

    private:
        int audioUploadRecordId;
    };

    class CFileDeleteData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CFileDeleteData, fileName, audioUploadRecordId, storageType)

        int do_req() {
            CAddMqttCustomAudioFileBusiness business;
            return business.deleteData(fileName);
        }

    public:
        std::string fileName;
        int audioUploadRecordId;
        int storageType;
    };
}