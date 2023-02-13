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
        int do_success(const CReQuest<Quest, Result> &c, CResult<T> &r) {
            CUtils utils;
            if (utils.get_process_status("madplay")) {
                r.resultId = 2;
                r.result = "Already played";
                return 2;
            }
            std::string path = "/tmp/";
            std::string name = "record.mp3";
            std::string res = utils.get_upload_result(c.data.downloadUrl.c_str(), path.c_str(), name.c_str());
            std::cout << "res:-----" << res << std::endl;
            if (res.find("error") != std::string::npos) {
                r.resultId = 2;
                r.result = "download error";
                return 2;
            } else if (res.find("Failed to connect") != std::string::npos) {
                r.resultId = 2;
                r.result = "download Failed to connect error";
                return 2;
            } else if (res.find("Couldn't connect to server") != std::string::npos) {
                r.resultId = 2;
                r.result = "download Couldn't connect to server error";
                return 2;
            }
            utils.bit_rate_32_to_48(path + name);
            switch (c.data.timeType) {
                case 0: {
                    if (c.data.playCount < 1) {
                        r.resultId = 2;
                        r.result = "play time error";
                        return 2;
                    }
                    utils.audio_time_play(c.data.playCount, path + name, ASYNC_START);
                    break;
                }
                case 1: {
                    if (c.data.playCount < 1) {
                        r.resultId = 2;
                        r.result = "play count error";
                        return 2;
                    }
                    utils.audio_num_play(c.data.playCount, path + name, ASYNC_START);
                    break;
                }
            }
            r.resultId = 1;
            r.result = "success";
            return 1;
        }

    private:
        std::nullptr_t null;
    };

    class CRecordPlayData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT (CRecordPlayData, downloadUrl, playCount, playType, storageType,
                                                     timeType)

    public:
        std::string downloadUrl;// 音频文件下载路径
        int playCount;// 播放次数，单位次；当timeType为次数播放时生效
        int playType;// 播放模式：0-音频文件，1-文字播报，2-录音文件播放
        int storageType;// 存储位置：0-内置Flash，1-外置存储, 可不用
        int timeType;// 0-时长播放；1-次数播放
    };
}