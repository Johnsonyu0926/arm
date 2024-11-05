#pragma once
#include <mutex>
#include <memory>

namespace PublicUtils {

template <typename T>
class Singleton {
public:
    static T& Instance() {
        static std::once_flag s_flag;
        std::call_once(s_flag, [&]() { m_pInstance.reset(new T()); });
        return *m_pInstance;
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

protected:
    Singleton() = default;
    virtual ~Singleton() = default;

private:
    static std::unique_ptr<T> m_pInstance;
};

template <typename T>
std::unique_ptr<T> Singleton<T>::m_pInstance;

}
