#pragma once

#include <thread>
#include <string>
#include "public.hpp"
#include "spdlog/spdlog.h"

class PlayStatus {
public:
    PlayStatus(const PlayStatus &) = delete;
    PlayStatus &operator=(const PlayStatus &) = delete;

    static PlayStatus &getInstance() {
        static PlayStatus instance;
        return instance;
    }

    void init() {
        m_mqttPlayStatus = asns::STOP_TASK_PLAYING;
        m_playId = asns::STOP_TASK_PLAYING;
        m_priority = asns::STOP_TASK_PLAYING;
        m_pId = asns::STOP_TASK_PLAYING;
        playConten.clear();
        spdlog::info("PlayStatus initialized");
    }

    int getPlayState() const {
        return m_playId != asns::STOP_TASK_PLAYING;
    }

    int getPlayId() const {
        return m_playId;
    }

    void setPlayId(const int id) {
        m_playId = id;
        spdlog::info("Play ID set to {}", id);
    }

    int getPriority() const {
        return m_priority;
    }

    void setPriority(const int id) {
        m_priority = id;
        spdlog::info("Priority set to {}", id);
    }

    pid_t getProcessId() const {
        return m_pId;
    }

    void setProcessId(pid_t id) {
        m_pId = id;
        spdlog::info("Process ID set to {}", id);
    }

    std::string getPlayConten() const {
        return playConten;
    }

    void setPlayConten(const std::string &playConten) {
        this->playConten = playConten;
        spdlog::info("Play content set to {}", playConten);
    }

    int getMqttPlayStatus() const {
        return m_mqttPlayStatus;
    }

    void setMqttPlayStatus(const int status) {
        m_mqttPlayStatus = status;
        spdlog::info("MQTT Play Status set to {}", status);
    }

    ~PlayStatus() = default;

private:
    PlayStatus() = default;

private:
    int m_mqttPlayStatus{-1};
    int m_playId{-1};
    int m_priority{-1};
    pid_t m_pId{-1};
    std::string playConten{};
};