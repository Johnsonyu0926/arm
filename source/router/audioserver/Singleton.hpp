#pragma once

class Singleton {
public:
    ~Singleton() {}

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
    Singleton() {}

private:
    int m_status;
};