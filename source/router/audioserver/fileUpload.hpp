#pragma once

#include <algorithm>
#include <string>
#include "json.hpp"
#include "utils.h"
#include "add_custom_audio_file.hpp"
#include "spdlog/spdlog.h"

namespace asns {
    class CFileUploadResult {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CFileUploadResult, cmd, resultId, msg)

        void do_success() {
            cmd = "FileUpload";
            resultId = 1;
            msg = "file upload success";
        }

        void do_fail(const std::string& str) {
            cmd = "FileUpload";
            resultId = 2;
            msg = str;
        }

    private:
        std::string cmd;
        int resultId{0};
        std::string msg;
    };

    class CFileUpload {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CFileUpload, downloadUrl, fileName)

        int do_req(CSocket *pClient) {
            std::transform(fileName.begin(), fileName.end(), fileName.begin(), ::tolower);
            CFileUploadResult fileUploadResult;
            CUtils utils;

            if (utils.get_available_Disk("/mnt") < asns::DISK_SIZE) {
                return send_failure(pClient, "Lack of space");
            }

            CAudioCfgBusiness cfg;
            std::string res = utils.get_upload_result(downloadUrl.c_str(), cfg.getAudioFilePath().c_str(), fileName.c_str());
            if (res != "success") {
                return send_failure(pClient, res);
            }

            CAddCustomAudioFileData node;
            node.filePath = downloadUrl;
            node.customAudioName = fileName;
            CAddCustomAudioFileBusiness business;
            utils.bit_rate_32_to_48(cfg.getAudioFilePath() + fileName);

            if (!business.exist(fileName)) {
                business.business.push_back(node);
                business.saveToJson();
            }

            fileUploadResult.do_success();
            return send_response(pClient, fileUploadResult);
        }

    private:
        std::string cmd;
        std::string downloadUrl;
        std::string fileName;

        int send_failure(CSocket *pClient, const std::string& msg) {
            CFileUploadResult fileUploadResult;
            fileUploadResult.do_fail(msg);
            return send_response(pClient, fileUploadResult);
        }

        int send_response(CSocket *pClient, const CFileUploadResult& result) {
            json js = result;
            std::string str = js.dump();
            spdlog::info("return json: {}", str);
            return pClient->Send(str.c_str(), str.length());
        }
    };
} // namespace asns