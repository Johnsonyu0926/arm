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
            std::transform(fileName.begin(), fileName.end(), fileName.begin(), ::tolower);
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
            } else if (res.find("Connection refused") != std::string::npos) {
                fileUploadResult.do_fail("Connection refused");
            } else if (res.find("No route to host") != std::string::npos) {
                fileUploadResult.do_fail("No route to host");
            } else if (res.find("Host is unreachable") != std::string::npos) {
                fileUploadResult.do_fail("Host is unreachable");
            } else if (res.find("Failed to connect") != std::string::npos) {
                fileUploadResult.do_fail("Failed to connect");
            } else if (res.find("Couldn't connect to server") != std::string::npos) {
                fileUploadResult.do_fail("Couldn't connect to server");
            } else {
                CAddCustomAudioFileData node;
                node.filePath = downloadUrl;
                node.customAudioName = fileName;
                CAddCustomAudioFileBusiness business;
                if (!business.exist(fileName)) {
                    std::string cmd = "conv.sh " + cfg.getAudioFilePath() + fileName;
                    system(cmd.c_str());
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