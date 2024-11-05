// audioplaystatus.hpp
#pragma once

#include <string>
#include <atomic>
#include "public.hpp"

class PlayStatus {
public:
    static PlayStatus& getInstance() {
        static PlayStatus instance;
        return instance;
    }

    PlayStatus(const PlayStatus&) = delete;
    PlayStatus& operator=(const PlayStatus&) = delete;
    PlayStatus(PlayStatus&&) = delete;
    PlayStatus& operator=(PlayStatus&&) = delete;

    void init() {
        m_mqttPlayStatus.store(asns::STOP_TASK_PLAYING);
        m_playId.store(asns::STOP_TASK_PLAYING);
        m_priority.store(asns::STOP_TASK_PLAYING);
        m_pId.store(asns::STOP_TASK_PLAYING);
        playContent.clear();
    }

    [[nodiscard]] bool getPlayState() const { return m_playId.load() != asns::STOP_TASK_PLAYING; }
    [[nodiscard]] int getPlayId() const { return m_playId.load(); }
    void setPlayId(int id) { m_playId.store(id); }
    [[nodiscard]] int getPriority() const { return m_priority.load(); }
    void setPriority(int id) { m_priority.store(id); }
    [[nodiscard]] pid_t getProcessId() const { return m_pId.load(); }
    void setProcessId(pid_t id) { m_pId.store(id); }
    [[nodiscard]] std::string getPlayContent() const { return playContent; }
    void setPlayContent(std::string content) { playContent = std::move(content); }
    [[nodiscard]] int getMqttPlayStatus() const { return m_mqttPlayStatus.load(); }
    void setMqttPlayStatus(int status) { m_mqttPlayStatus.store(status); }

private:
    PlayStatus() = default;

    std::atomic<int> m_mqttPlayStatus{asns::STOP_TASK_PLAYING};
    std::atomic<int> m_playId{asns::STOP_TASK_PLAYING};
    std::atomic<int> m_priority{asns::STOP_TASK_PLAYING};
    std::atomic<pid_t> m_pId{asns::STOP_TASK_PLAYING};
    std::string playContent;
};

//By GST ARMV8 GCC13.2 audioplaystatus.hpp