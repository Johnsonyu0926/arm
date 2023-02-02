#pragma once
#include <thread>
/*
class PlayStatus {
public:
    PlayStatus(const PlayStatus &) = delete;

    PlayStatus &operator=(const PlayStatus &) = delete;

    static PlayStatus &getInstance() {
        static PlayStatus instance;
        return instance;
    }

    void init() {
        m_playId = -1;
        m_priority = -1;
        m_pId = -1;
        m_tId = 0;
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

    std::thread::id getThreadId() const {
        return m_tId;
    }

    void setThreadId(const std::thread::id id) {
        m_tId = id;
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
    std::thread::id m_tId{0};
    pid_t m_pId{-1};
};
*/