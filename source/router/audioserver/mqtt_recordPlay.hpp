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
            char buf[256] = {0};
            std::string fileName = "/tmp/record.mp3";
            sprintf(buf, "curl --location --request GET %s --output %s", downloadUrl.c_str(), fileName.c_str());
            std::cout << "cmd: " << buf << std::endl;
            system(buf);
            switch (timeType) {
                case 0: {
                    char command[256] = {0};
                    int d = playCount / (3600 * 24);
                    int t = playCount % (3600 * 24) / 3600;
                    int m = playCount % (3600 * 24) % 3600 / 60;
                    int s = playCount % (3600 * 24) % 3600 % 60;
                    char buf[64] = {0};
                    sprintf(buf, "%d:%d:%d:%d", d, t, m, s);
                    sprintf(command, "madplay %s -r -t %s &",fileName.c_str(), buf);
                    std::cout << "command: " << command << std::endl;
                    system(command);
                    break;
                }
                case 1: {
                    std::string cmd = "madplay ";
                    for (int i = 0; i < playCount; ++i) {
                        cmd += fileName + ' ';
                    }
                    cmd += "&";
                    std::cout << "cmd: " << cmd << std::endl;
                    system(cmd.c_str());
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