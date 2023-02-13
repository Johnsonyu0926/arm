#pragma once

#include "json.hpp"
#include "public.hpp"
#include "utils.h"

/**
 * {
        "downloadUrl":"http://172.16.1.20:9999/data/cms/record/e2567a91-8271-4011-905a-6f1d94845812.mp3", // 录音文件下载路径
        "playCount":1,
        "playType":3,
        "storageType":1,
        "timeType":1
    }
 */
namespace asns {
    template<typename Quest, typename Result>
    class CReQuest;

    template<typename T>
    class CResult;

    class CRecordPlayResultData {
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CRecordPlayResultData, null);

    public:
        template<typename Quest, typename Result, typename T>
        void do_success(const CReQuest<Quest, Result> &c, CResult<T> &r) {}

    private:
        std::nullptr_t null;
    };

    class CRecordPlayData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT (CRecordPlayData, downloadUrl, playCount, playType, storageType,
                                                     timeType)

        int do_req() {
            CUtils utils;
            if (utils.get_process_status("madplay")) {
                return 5;
            }
            std::string path = "/tmp/";
            std::string name = "record.mp3";
            std::string res = utils.get_upload_result(downloadUrl.c_str(), path.c_str(), name.c_str());
            std::cout << "res:-----" << res << std::endl;
            if (res.find("error") != std::string::npos) {
                return 3;
            } else if (res.find("Failed to connect") != std::string::npos) {
                return 3;
            } else if (res.find("Couldn't connect to server") != std::string::npos) {
                return 3;
            }
            utils.bit_rate_32_to_48(path + name);
            switch (timeType) {
                case 0: {
                    utils.audio_time_play(playCount, path + name, ASYNC_START);
                    break;
                }
                case 1: {
                    utils.audio_num_play(playCount, path + name, ASYNC_START);
                    break;
                }
            }
            return 1;
        }

    private:
        std::string downloadUrl;// 音频文件下载路径
        int playCount;// 播放次数，单位次；当timeType为次数播放时生效
        int playType;// 播放模式：0-音频文件，1-文字播报，2-录音文件播放
        int storageType;// 存储位置：0-内置Flash，1-外置存储, 可不用
        int timeType;// 0-时长播放；1-次数播放
    };
}