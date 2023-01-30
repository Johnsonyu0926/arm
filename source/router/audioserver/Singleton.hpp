#pragma once

class Singleton {
public:
    ~Singleton() = default;

    Singleton(const Singleton &) = delete;

    Singleton &operator=(const Singleton &) = delete;

    static Singleton &getInstance() {
        static Singleton instance;
        return instance;
    }

    int getStatus() const {
        return m_status;
    }

    void setStatus(const int status) {
        m_status = status;
    }

private:
    Singleton() = default;

private:
    int m_status;
};