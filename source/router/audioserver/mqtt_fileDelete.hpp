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
        int do_success(const CReQuest<Quest, Result> &c, CResult<T> &r) {
            audioUploadRecordId = c.data.audioUploadRecordId;
            CAddMqttCustomAudioFileBusiness business;
            if(business.deleteData(c.data.fileName) == 1){
                r.resultId = 1;
                r.result = "success";
                return 1;
            }else{
                r.resultId = 2;
                r.result = "no file";
                return 2;
            }
        }

    private:
        int audioUploadRecordId;
    };

    class CFileDeleteData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CFileDeleteData, fileName, audioUploadRecordId, storageType)

    public:
        std::string fileName;
        int audioUploadRecordId;
        int storageType;
    };
}