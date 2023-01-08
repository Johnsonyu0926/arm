#pragma once

#include "json.hpp"
#include "utils.h"
#include "add_custom_audio_file.hpp"

namespace asns {
    class CFileUploadResult {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CFileUploadResult, cmd, resultId, msg)

        int do_success() {
            cmd = "FileUpload";
            resultId = 1;
            msg = "file upload success";
        }

        int do_fail(std::string str) {
            cmd = "FileUpload";
            resultId = 2;
            msg = str;
        }

    private:
        std::string cmd;
        int resultId;
        std::string msg;
    };

    class CFileUpload {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CFileUpload, downloadUrl, fileName)

        int do_req(CSocket *pClient) {
            CFileUploadResult fileUploadResult;
            CUtils utils;
            CAudioCfgBusiness cfg;
            std::string res = utils.get_upload_result(downloadUrl.c_str(), cfg.getAudioFilePath().c_str(),
                                                      fileName.c_str());
            std::cout << "res:-----" << res << std::endl;
            if (res.empty()) {
                fileUploadResult.do_fail("fail");
            } else if (res.find("error") != std::string::npos) {
                fileUploadResult.do_fail("Connection unreachable");
            } else {
                CAddCustomAudioFileData node;
                node.filePath = downloadUrl;
                node.customAudioName = fileName;
                CAddCustomAudioFileBusiness business;
                if (!business.exist(fileName)) {
                    business.business.push_back(node);
                    business.saveToJson();
                    fileUploadResult.do_success();
                } else {
                    fileUploadResult.do_success();
                }
            }
            json js = fileUploadResult;
            std::string str = js.dump();
            std::cout << "return json: " << str << std::endl;
            pClient->Send(str.c_str(), str.length());
        }

    private:
        std::string cmd;
        std::string downloadUrl;
        std::string fileName;
    };
}