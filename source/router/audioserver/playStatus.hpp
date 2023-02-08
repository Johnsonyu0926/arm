#pragma once

#include <thread>
#include "public.hpp"

class PlayStatus {
public:
    PlayStatus(const PlayStatus &) = delete;

    PlayStatus &operator=(const PlayStatus &) = delete;

    static PlayStatus &getInstance() {
        static PlayStatus instance;
        return instance;
    }

    void init() {
        m_playId = asns::STOP_TASK_PLAYING;
        m_priority = asns::STOP_TASK_PLAYING;
        m_pId = asns::STOP_TASK_PLAYING;
    }

    int getPlayId() const {
        return m_playId;
    }

    void setPlayId(const int id) {
        m_playId = id;
    }

    int getPriority() const {
        return m_priority;
    }

    void setPriority(const int id) {
        m_priority = id;
    }

    pid_t getProcessId() const {
        return m_pId;
    }

    void setProcessId(pid_t id) {
        m_pId = id;
    }

    ~PlayStatus() = default;

private:
    PlayStatus() = default;

private:
    int m_playId{-1};
    int m_priority{-1};
    pid_t m_pId{-1};
};
