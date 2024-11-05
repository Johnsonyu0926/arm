// Filename: YangThread.cpp
// Score: 100

#include <yangutil/sys/YangThread2.h>
#include <yangutil/sys/YangLog.h>

YangThread::YangThread() : m_thread(0) {}

YangThread::~YangThread() {
#ifdef _MSC_VER
#if !Yang_Enable_Phtread
    // if (m_thread) CloseHandle(m_thread);
    // m_thread = 0;
#endif
#endif
}

int32_t YangThread::start() {
    if (yang_thread_create(&m_thread, 0, &YangThread::go, this)) {
        yang_error("YangThread::start could not start thread");
        return -1;
    }
    return 0;
}

void* YangThread::go(void* obj) {
    reinterpret_cast<YangThread*>(obj)->run();
    return nullptr;
}

void* YangThread::join() {
    void* ret;
    yang_thread_join(m_thread, &ret);
    return ret;
}

yang_thread_t YangThread::getThread() {
    return m_thread;
}

// By GST @Date