// Filename: YangSysMessageHandle.cpp
// Score: 100

#include <yangutil/sys/YangSysMessageHandle.h>
#include <yangutil/sys/YangLog.h>
#include <yangutil/sys/YangThread.h>
#include <yangutil/yangavinfotype.h>

void yang_post_message(int32_t st, int32_t uid, YangSysMessageI* mhandle, void* user) {
    if (YangSysMessageHandle::m_instance)
        YangSysMessageHandle::m_instance->putMessage(mhandle, st, uid, 0, user);
}

void yang_post_state_message(int32_t st, int32_t uid, int32_t handleState, YangSysMessageI* mhandle) {
    if (YangSysMessageHandle::m_instance)
        YangSysMessageHandle::m_instance->putMessage(mhandle, st, uid, handleState);
}

void yang_post_userMessage(int32_t st, int32_t uid, YangSysMessageI* mhandle, void* user) {
    if (YangSysMessageHandle::m_instance)
        YangSysMessageHandle::m_instance->putMessage(mhandle, st, uid, 0);
}

YangSysMessageHandle::YangSysMessageHandle() : m_loop(yangfalse), m_isStart(yangfalse), m_waitState(yangfalse), m_receive(nullptr) {
    yang_thread_mutex_init(&m_mutex, nullptr);
    yang_thread_mutex_init(&m_lock, nullptr);
    yang_thread_cond_init(&m_cond_mess, nullptr);
    if (m_instance == nullptr)
        m_instance = this;
}

YangSysMessageHandle::~YangSysMessageHandle() {
    if (m_isStart) {
        stop();
        while (m_isStart) {
            yang_usleep(1000);
        }
    }
    m_instance = nullptr;
    yang_thread_mutex_destroy(&m_mutex);
    yang_thread_mutex_destroy(&m_lock);
    yang_thread_cond_destroy(&m_cond_mess);
}

YangSysMessageHandle* YangSysMessageHandle::m_instance = nullptr;

void YangSysMessageHandle::run() {
    m_isStart = yangtrue;
    startLoop();
    m_isStart = yangfalse;
}

void YangSysMessageHandle::stop() {
    stopLoop();
}

void YangSysMessageHandle::stopLoop() {
    m_loop = yangfalse;
    if (m_waitState) {
        yang_thread_mutex_lock(&m_lock);
        yang_thread_cond_signal(&m_cond_mess);
        yang_thread_mutex_unlock(&m_lock);
    }
}

void YangSysMessageHandle::putMessage(YangSysMessageI* handle, int32_t pst, int32_t puid, int32_t handleState, void* user) {
    if (!m_loop) return;

    auto mes = std::make_unique<YangSysMessage>();
    mes->uid = puid;
    mes->messageId = pst;
    mes->handleState = handleState;
    mes->handle = handle;
    mes->user = user;

    yang_thread_mutex_lock(&m_mutex);
    m_sysMessages.push_back(std::move(mes));
    yang_thread_mutex_unlock(&m_mutex);

    if (m_waitState) {
        yang_thread_mutex_lock(&m_lock);
        yang_thread_cond_signal(&m_cond_mess);
        yang_thread_mutex_unlock(&m_lock);
    }
}

void YangSysMessageHandle::startLoop() {
    m_loop = 1;
    yang_thread_mutex_unlock(&m_lock);
    yang_thread_mutex_lock(&m_lock);

    while (m_loop) {
        m_waitState = yangtrue;
        yang_thread_cond_wait(&m_cond_mess, &m_lock);
        m_waitState = yangfalse;

        while (!m_sysMessages.empty()) {
            handleMessage(m_sysMessages.front().get());
            m_sysMessages.front()->handle = nullptr;
            yang_thread_mutex_lock(&m_mutex);
            m_sysMessages.pop_front();
            yang_thread_mutex_unlock(&m_mutex);
        }
    }
    yang_thread_mutex_unlock(&m_lock);
}

// By GST @Date