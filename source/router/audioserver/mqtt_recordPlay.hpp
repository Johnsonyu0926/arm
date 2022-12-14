#pragma once

#include "json.hpp"

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

    class CRecordPlayResultData {
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CRecordPlayResultData, null);

    public:
        template<typename Quest, typename Result>
        void do_success(const CReQuest<Quest, Result> &c) {}

    private:
        std::nullptr_t null;
    };

    class CRecordPlayData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CRecordPlayData, downloadUrl, playCount, playType, storageType, timeType)

        int do_req() {
            char command[256] = {0};
            switch (timeType) {
                case 0: {
                    break;
                }
                case 1: {
                    sprintf(command,
                            "ffmpeg -y -r 30 -re -i rtmp:%s -acodec mp3 -b:a 48k -max_delay 100 -g 5 -b 700000 -f mp3 - | madplay -",
                            downloadUrl.substr(5).c_str());
                    break;
                }
            }
            std::cout << "command: " << command << std::endl;
            system(command);
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