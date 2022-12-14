#pragma once

#include "json.hpp"
#include "add_custom_audio_file.hpp"

namespace asns
{
    class CFileUploadResult
    {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CFileUploadResult, cmd, resultId, msg)

        int do_success()
        {
            cmd = "FileUpload";
            resultId = 1;
            msg = "音频文件上传成功";
        }

    private:
        std::string cmd;
        int resultId;
        std::string msg;
    };

    class CFileUpload
    {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CFileUpload, downloadUrl, fileName)

        int do_req(CSocket *pClient)
        {
            CAddCustomAudioFileData node;
            node.filePath = downloadUrl;
            strncpy(node.filename, fileName.c_str(), fileName.length());

            CAudioCfgBusiness audioCfgBusiness;
            audioCfgBusiness.load();

            CAddCustomAudioFileBusiness addCustomAudioFileBusiness;
            addCustomAudioFileBusiness.savePrefix = audioCfgBusiness.business[0].savePrefix;
            addCustomAudioFileBusiness.append(node);

            CFileUploadResult fileUploadResult;
            fileUploadResult.do_success();
            json js = fileUploadResult;
            std::string str = js.dump();
            pClient->Send(str.c_str(), str.length());
        }

    private:
        std::string cmd;
        std::string downloadUrl;
        std::string fileName;
    };
}