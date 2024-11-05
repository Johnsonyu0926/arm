// audioplayutils.hpp
#pragma once

#include "AcquisitionNoise.hpp"
#include "AudioPlayStatus.hpp"
#include "utils.h"
#include <thread>
#include <chrono>
#include <string_view>
#include <functional>

class AudioPlayUtil {
public:
    static std::string ttsCmd(std::string_view txt, int speed = 50, int gender = 0) {
        std::string cmd = "tts.sh \"" + std::string(txt) + "\" " + std::to_string(speed);
        cmd += (gender == 0) ? " xiaoyan" : " xiaofeng";
        return cmd;
    }

    static void tts_loop_play(std::string_view txt, bool async = false, int speed = 50, int gender = 0) {
        playAudio(ttsCmd(txt, speed, gender), txt, asns::TTS_TASK_PLAYING, async);
    }

    static void tts_num_play(int num, std::string_view txt, bool async = false, int speed = 50, int gender = 0) {
        playAudioMultipleTimes(ttsCmd(txt, speed, gender), txt, num, asns::TTS_TASK_PLAYING, async);
    }

    static void tts_time_play(int time, std::string_view txt, bool async = false, int speed = 50, int gender = 0) {
        playAudioForDuration(ttsCmd(txt, speed, gender), txt, time, asns::TTS_TASK_PLAYING, async);
    }

    static void ffplay_audio_num_play(int num, std::string_view path, bool async = false) {
        std::string cmd = "ffplay -autoexit -nodisp " + std::string(path);
        playAudioMultipleTimes(cmd, path, num, asns::AUDIO_TASK_PLAYING, async);
    }

    static void ffplay_audio_time_play(int time, std::string_view path, bool async = false) {
        std::string cmd = "ffplay -autoexit -nodisp " + std::string(path);
        playAudioForDuration(cmd, path, time, asns::AUDIO_TASK_PLAYING, async);
    }

    static void audio_loop_play(std::string_view path, bool async = false) {
        std::string cmd = "madplay " + std::string(path) + " -r";
        playAudio(cmd, path, asns::AUDIO_TASK_PLAYING, async);
    }

    static void audio_num_play(int num, std::string_view path, bool async = false) {
        std::string cmd = "madplay " + std::string(path) + " ";
        cmd = std::string(num, cmd);
        playAudio(cmd, path, asns::AUDIO_TASK_PLAYING, async);
    }

    static void audio_time_play(int time, std::string_view path, bool async = false) {
        char buf[64];
        snprintf(buf, sizeof(buf), "%d:%02d:%02d:%02d", 
                 time / 86400, (time % 86400) / 3600, (time % 3600) / 60, time % 60);
        std::string cmd = "madplay " + std::string(path) + " -r -t " + buf;
        playAudio(cmd, path, asns::AUDIO_TASK_PLAYING, async);
    }

    static void audio_stop() {
        PlayStatus::getInstance().init();
        CUtils::cmd_system("killall -9 ffplay");
        CUtils::cmd_system("killall -9 madplay");
    }

private:
    static void playAudio(const std::string& cmd, std::string_view content, int playId, bool async) {
        auto play_func = [cmd, content, playId]() {
            PlayStatus::getInstance().setPlayId(playId);
            PlayStatus::getInstance().setPlayContent(getFileName(content));
            CUtils::cmd_system(cmd);
            PlayStatus::getInstance().init();
        };

        executeAsync(play_func, async);
    }

    static void playAudioMultipleTimes(const std::string& cmd, std::string_view content, int num, int playId, bool async) {
        auto play_func = [cmd, content, num, playId]() {
            PlayStatus::getInstance().setPlayId(playId);
            PlayStatus::getInstance().setPlayContent(getFileName(content));
            for (int i = 0; i < num && PlayStatus::getInstance().getPlayId() == playId; ++i) {
                while (CUtils::get_process_status("ffplay")) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
                if (PlayStatus::getInstance().getPlayId() != playId) break;
                CUtils::cmd_system(cmd);
            }
            PlayStatus::getInstance().init();
        };

        executeAsync(play_func, async);
    }

    static void playAudioForDuration(const std::string& cmd, std::string_view content, int time, int playId, bool async) {
        CUtils::async_wait(1, time, 0, [] {
            CUtils::cmd_system("killall -9 ffplay");
            PlayStatus::getInstance().init();
        });

        auto play_func = [cmd, content, playId]() {
            PlayStatus::getInstance().setPlayId(playId);
            PlayStatus::getInstance().setPlayContent(getFileName(content));
            while (PlayStatus::getInstance().getPlayId() == playId) {
                if (!CUtils::get_process_status("ffplay") && PlayStatus::getInstance().getPlayId() == playId) {
                    CUtils::cmd_system(cmd);
                } else if (PlayStatus::getInstance().getPlayId() != playId) {
                    break;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        };

        executeAsync(play_func, async);
    }

    static void executeAsync(const std::function<void()>& func, bool async) {
        if (async) {
            std::thread(func).detach();
        } else {
            func();
        }
    }

    static std::string getFileName(std::string_view path) {
        auto pos = path.find_last_of('/');
        return std::string(pos != std::string_view::npos ? path.substr(pos + 1) : path);
    }
};

//By GST ARMV8 GCC13.2 audioplayutils.hpp