#pragma once

#include "json.hpp"
#include "audiocfg.hpp"
#include "add_mqtt_custom_audio_file.hpp"
#include "Singleton.hpp"

using json = nlohmann::json;

/*
 *  // mqtt发送体（即详细的功能参数，详细参数已JSON格式体展现）
            {
                "fileName":"65.mp3", // 播放音频文件名
                "audioUploadRecordId":389, // 播放音频文件上传id
                "playCount":1, // 播放次数，单位次；当timeType为次数播放时生效
                "playDuration":60, // 播放时长，单位秒，当timeType为时长播放时生效
                "playStatus":1, // 播放状态，即1代表音频播放，详细参照"播放状态枚举"
                "playType":1, // 播放模式：0-音频文件，1-文字播报，2-录音文件播放
                "storageType":1, // 存储位置：0-内置Flash，1-外置存储, 可不用
                "timeType":0 // 0-时长播放；1-次数播放
            }


        {
            "result":"播放成功", // 处理结果
            "resultId":1, // 处理结果标记；0-表示未处理；1-表示处理成功；2-表示处理失败；
            "imei":"869298057401606", // 设备唯一序列号
            "topic":"TaDiao/device/report/test/869298057401606", // topic
            "cmd":"audioPlay", // 当次处理的功能标识
            "publishId":172684, // 平台下发的本次请求id
            "data": "" // 处理完成上传数据，如果有则需要上传，详情参照详细功能通信指令，已JSON格式体展现
        }
 */



namespace asns {
    template<typename Quest, typename Result>
    class CReQuest;

    class CAudioPlayResultData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CAudioPlayResultData, null)

        template<typename Quest, typename Result>
        void do_success(const CReQuest<Quest, Result> &c) {}

    public:
        std::nullptr_t null;
    };


    class CAudioPlayData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CAudioPlayData, fileName, audioUploadRecordId, playCount,
                                                    playDuration,
                                                    playStatus, playType, storageType, timeType
        )

        int do_req() {
            CAddMqttCustomAudioFileBusiness business;
            if (!business.exist(fileName)) {
                return 0;
            }
            CAudioCfgBusiness cfg;
            cfg.load();

            switch (timeType) {
                case 0: {
                    char command[256] = {0};
                    int d = playDuration / (3600 * 24);
                    int t = playDuration % (3600 * 24) / 3600;
                    int m = playDuration % (3600 * 24) % 3600 / 60;
                    int s = playDuration % (3600 * 24) % 3600 % 60;
                    char buf[64] = {0};
                    sprintf(buf, "%d:%d:%d:%d", d, t, m, s);
                    sprintf(command, "madplay %s%s -r -t %s &", cfg.getAudioFilePath().c_str(),
                            fileName.c_str(), buf);
                    std::cout << "command: " << command << std::endl;
                    system(command);
                    Singleton::getInstance().setStatus(1);
                    break;
                }
                case 1: {
                    std::string cmd = "madplay " + cfg.getAudioFilePath();
                    for (int i = 0; i < playCount; ++i) {
                        cmd += fileName + ' ';
                    }
                    cmd += "&";
                    std::cout << "cmd: " << cmd << std::endl;
                    system(cmd.c_str());
                    Singleton::getInstance().setStatus(1);
                    break;
                }
            }
            return 1;
        }

    public:
        std::string fileName;
        int audioUploadRecordId;
        int playCount;
        int playDuration;
        int playStatus;
        int playType;
        int storageType;
        int timeType;
    };
}