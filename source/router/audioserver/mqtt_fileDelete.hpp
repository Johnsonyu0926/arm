#pragma once

#include <string>
#include "add_mqtt_custom_audio_file.hpp"
#include "json.hpp"

namespace asns {

    template<typename Quest, typename Result>
    class CReQuest;

    template<typename T>
    class CResult;

    class CFileDeleteResultData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CFileDeleteResultData, audioUploadRecordId)

        template<typename Quest, typename Result, typename T>
        int do_success(const CReQuest<Quest, Result> &c, CResult<T> &r) {
            audioUploadRecordId = c.data.audioUploadRecordId;
            CAddMqttCustomAudioFileBusiness business;
            business.deleteData(c.data.fileName);
            r.resultId = 1;
            r.result = "success";
            return 1;
        }

    private:
        int audioUploadRecordId{0};
    };

    class CFileDeleteData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CFileDeleteData, fileName, audioUploadRecordId, storageType)

        std::string fileName;
        int audioUploadRecordId{0};
        int storageType{0};
    };
}