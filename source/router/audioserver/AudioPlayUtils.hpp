#pragma once

#include <string>
#include "AcquisitionNoise.hpp"
#include "AudioPlayStatus.hpp"
#include "utils.h"

class AudioPlayUtil {
public:
    static std::string ttsCmd(const std::string &txt, int speed = 50, int gender = 0) {
        std::string cmd = "tts.sh \"" + txt + "\" " + std::to_string(speed);
        switch (gender) {
            case 0:
                cmd += " xiaoyan";
                break;
            case 1:
                cmd += " xiaofeng";
                break;
            default:
                break;
        }
        return cmd;
    }

    static void tts_loop_play(const std::string &txt, bool async = false, int speed = 50, int gender = 0) {
        std::string cmd = ttsCmd(txt, speed, gender);
        LOG(INFO) << "tts_loop_play cmd :" << cmd;
        auto play_func = [=] {
            PlayStatus::getInstance().setPlayId(asns::TTS_TASK_PLAYING);
            PlayStatus::getInstance().setPlayContent(txt);
            while (PlayStatus::getInstance().getPlayId() == asns::TTS_TASK_PLAYING) {
                if (!CUtils::get_process_status("ffplay") &&
                    PlayStatus::getInstance().getPlayId() == asns::TTS_TASK_PLAYING) {
                    CUtils::cmd_system(cmd);
                } else if (PlayStatus::getInstance().getPlayId() != asns::TTS_TASK_PLAYING) {
                    break;
                }
                usleep(1000 * 100);
            }
        };
        if (async) {
            CUtils::async_wait(1, 0, 0, play_func);
        } else {
            play_func();
        }
    }

    static void tts_num_play(int num, const std::string &txt, bool async = false, int speed = 50, int gender = 0) {
        std::string cmd = ttsCmd(txt, speed, gender);
        LOG(INFO) << "tts_num_play cmd :" << cmd;
        auto play_func = [=] {
            PlayStatus::getInstance().setPlayId(asns::TTS_TASK_PLAYING);
            PlayStatus::getInstance().setPlayContent(txt);
            for (int i = 0; i < num; ++i) {
                while (CUtils::get_process_status("ffplay")) {
                    usleep(1000 * 100);
                }
                if (PlayStatus::getInstance().getPlayId() != asns::TTS_TASK_PLAYING) {
                    break;
                }
                CUtils::cmd_system(cmd);
                if (PlayStatus::getInstance().getPlayId() != asns::TTS_TASK_PLAYING) {
                    break;
                }
            }
            PlayStatus::getInstance().init();
        };
        if (async) {
            CUtils::async_wait(1, 0, 0, play_func);
        } else {
            play_func();
        }
    }

    static void tts_time_play(int time, const std::string &txt, bool async = false, int speed = 50, int gender = 0) {
        std::string cmd = ttsCmd(txt, speed, gender);
        LOG(INFO) << "tts_time_play cmd :" << cmd;
        CUtils::async_wait(1, time, 0, [&] {
            CUtils::cmd_system("killall -9 ffplay");
            PlayStatus::getInstance().init();
        });
        auto play_func = [=] {
            PlayStatus::getInstance().setPlayId(asns::TTS_TASK_PLAYING);
            PlayStatus::getInstance().setPlayContent(txt);
            while (PlayStatus::getInstance().getPlayId() == asns::TTS_TASK_PLAYING) {
                if (!CUtils::get_process_status("ffplay") &&
                    PlayStatus::getInstance().getPlayId() == asns::TTS_TASK_PLAYING) {
                    CUtils::cmd_system(cmd);
                } else if (PlayStatus::getInstance().getPlayId() != asns::TTS_TASK_PLAYING) {
                    break;
                }
                usleep(1000 * 100);
            }
        };
        if (async) {
            CUtils::async_wait(1, 0, 0, play_func);
        } else {
            play_func();
        }
    }

    static void ffplay_audio_num_play(int num, const std::string &path, bool async = false) {
        const std::string cmd = "ffplay -autoexit -nodisp " + path;
        LOG(INFO) << "ffplay_audio_num_play cmd :" << cmd;
        auto play_func = [=] {
            PlayStatus::getInstance().setPlayId(asns::AUDIO_TASK_PLAYING);
            PlayStatus::getInstance().setPlayContent(path.substr(path.find_last_of('/') + 1));
            for (int i = 0; i < num; ++i) {
                while (CUtils::get_process_status("ffplay")) {
                    usleep(1000 * 100);
                }
                if (PlayStatus::getInstance().getPlayId() != asns::AUDIO_TASK_PLAYING) {
                    break;
                }
                CUtils::cmd_system(cmd);
                if (PlayStatus::getInstance().getPlayId() != asns::AUDIO_TASK_PLAYING) {
                    break;
                }
            }
            PlayStatus::getInstance().init();
        };
        if (async) {
            CUtils::async_wait(1, 0, 0, play_func);
        } else {
            play_func();
        }
    }

    static void ffplay_audio_time_play(int time, const std::string &path, bool async = false) {
        const std::string cmd = "ffplay -autoexit -nodisp " + path;
        LOG(INFO) << "ffplay_audio_time_play cmd :" << cmd;
        CUtils::async_wait(1, time, 0, [&] {
            CUtils::cmd_system("killall -9 ffplay");
            PlayStatus::getInstance().init();
        });
        auto play_func = [=] {
            PlayStatus::getInstance().setPlayId(asns::AUDIO_TASK_PLAYING);
            PlayStatus::getInstance().setPlayContent(path.substr(path.find_last_of('/') + 1));
            while (PlayStatus::getInstance().getPlayId() == asns::AUDIO_TASK_PLAYING) {
                if (!CUtils::get_process_status("ffplay") &&
                    PlayStatus::getInstance().getPlayId() == asns::AUDIO_TASK_PLAYING) {
                    CUtils::cmd_system(cmd);
                } else if (PlayStatus::getInstance().getPlayId() != asns::AUDIO_TASK_PLAYING) {
                    break;
                }
                usleep(1000 * 100);
            }
        };
        if (async) {
            CUtils::async_wait(1, 0, 0, play_func);
        } else {
            play_func();
        }
    }

    static void audio_loop_play(const std::string &path, bool async = false) {
        auto play_func = [=] {
            std::string cmd = "madplay " + path + " -r";
            PlayStatus::getInstance().setPlayId(asns::AUDIO_TASK_PLAYING);
            PlayStatus::getInstance().setPlayContent(path.substr(path.find_last_of('/') + 1));
            CUtils::cmd_system(cmd);
            PlayStatus::getInstance().init();
        };
        if (async) {
            CUtils::async_wait(1, 0, 0, play_func);
        } else {
            play_func();
        }
    }

    static void audio_num_play(int num, const std::string &path, bool async = false) {
        auto play_func = [=] {
            std::string cmd = "madplay ";
            for (int i = 0; i < num; ++i) {
                cmd += path + ' ';
            }
            PlayStatus::getInstance().setPlayId(asns::AUDIO_TASK_PLAYING);
            PlayStatus::getInstance().setPlayContent(path.substr(path.find_last_of('/') + 1));
            CUtils::cmd_system(cmd);
            PlayStatus::getInstance().init();
        };
        if (async) {
            CUtils::async_wait(1, 0, 0, play_func);
        } else {
            play_func();
        }
    }

    static void audio_time_play(int time, const std::string &path, bool async = false) {
        int d = time / (3600 * 24);
        int t = time % (3600 * 24) / 3600;
        int m = time % (3600 * 24) % 3600 / 60;
        int s = time % (3600 * 24) % 3600 % 60;
        char buf[64] = {0};
        char cmd[128] = {0};
        snprintf(buf, sizeof(buf), "%d:%d:%d:%d", d, t, m, s);
        snprintf(cmd, sizeof(cmd), "madplay %s -r -t %s", path.c_str(), buf);
        auto play_func = [=] {
            PlayStatus::getInstance().setPlayId(asns::AUDIO_TASK_PLAYING);
            PlayStatus::getInstance().setPlayContent(path.substr(path.find_last_of('/') + 1));
            CUtils::cmd_system(cmd);
            PlayStatus::getInstance().init();
        };
        if (async) {
            CUtils::async_wait(1, 0, 0, play_func);
        } else {
            play_func();
        }
    }

    static void audio_stop() {
        PlayStatus::getInstance().init();
        CUtils::cmd_system("killall -9 ffplay");
        CUtils::cmd_system("killall -9 madplay");
    }
};