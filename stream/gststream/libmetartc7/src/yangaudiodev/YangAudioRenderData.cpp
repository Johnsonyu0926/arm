// Filename: YangAudioRenderData.cpp
// Score: 94


#include <yangaudiodev/YangAudioRenderData.h>
#include <yangavutil/audio/YangAudioUtil.h>
#include <yangutil/sys/YangLog.h>
#include <cstring>

using namespace std;

#define Yang_Mix_BUF_Len 4096

YangAudioRenderData::YangAudioRenderData() {
    m_syns = nullptr;
    m_cacheLen = 1024 * 4 * 10;
    m_cache = new uint8_t[m_cacheLen];
    m_size = 0;
    m_pos = 0;
    m_aecBufLen = 0;
    m_renderLen = 0;
    m_syn = nullptr;
    m_hasAec = false;
    m_aecBuf = nullptr;
    m_mixBuf = new uint8_t[960 * 8];
    m_preProcess = nullptr;
    m_mixPos = 0;
    std::memset(&m_res, 0, sizeof(YangAudioResample));
    yang_create_audioresample(&m_res);
    m_res.init(m_res.context, 48000, 2, 48000, 2, 20);
}

YangAudioRenderData::~YangAudioRenderData() {
    delete[] m_cache;
    delete[] m_mixBuf;
    m_syn = nullptr;
    m_preProcess = nullptr;
    yang_destroy_audioresample(&m_res);
}

void YangAudioRenderData::setContext(YangSynBufferManager* streams) {
    m_syns = streams;
}

void YangAudioRenderData::setInAudioBuffer(YangSynBuffer *pal) {
    m_syn = pal;
}

void YangAudioRenderData::initRender(int psample, int pchannel) {
    m_res.initOut(m_res.context, psample, pchannel);
}

void YangAudioRenderData::initPlay(int psample, int pchannel) {
    m_res.initIn(m_res.context, psample, pchannel);
}

void YangAudioRenderData::setAec() {
    if (m_aecBuf == nullptr) m_aecBuf = new uint8_t[Yang_Mix_BUF_Len];
    m_hasAec = true;
}

uint8_t* YangAudioRenderData::getAudioRef(YangFrame* pframe) {
    if (m_syn) {
        return m_syn->getAudioRef(m_syn->session, pframe);
    }

    if (m_syns && m_syns->getAudioDatas(m_syns->session, m_mixBuf, pframe) == Yang_Ok) {
        return m_mixBuf;
    }

    return nullptr;
}

uint8_t* YangAudioRenderData::getAecAudioData() {
    if (m_renderLen < m_res.context->inBytes || m_aecBufLen < m_res.context->inBytes) return nullptr;

    if ((m_mixPos + m_res.context->inBytes) >= Yang_Mix_BUF_Len) {
        std::memmove(m_aecBuf, m_aecBuf + m_mixPos, m_aecBufLen);
        m_mixPos = 0;
    }

    uint8_t* p = m_aecBuf + m_mixPos;
    m_aecBufLen -= m_res.context->inBytes;
    m_mixPos += m_res.context->inBytes;
    m_renderLen = 0;
    return p;
}

void YangAudioRenderData::setRenderLen(int plen) {
    m_renderLen += (m_res.context->inBytes * plen) / m_res.context->outBytes;
}

uint8_t* YangAudioRenderData::getAudioData(YangFrame* frame) {
    uint8_t* tmp = getAudioRef(frame);

    if (m_hasAec && tmp) {
        if ((m_mixPos + m_aecBufLen + m_res.context->inBytes) >= Yang_Mix_BUF_Len) {
            std::memmove(m_aecBuf, m_aecBuf + m_mixPos, m_aecBufLen);
            m_mixPos = 0;
        }

        std::memcpy(m_aecBuf + m_mixPos + m_aecBufLen, tmp, m_res.context->inBytes);
        m_aecBufLen += m_res.context->inBytes;
    }

    return tmp;
}

void YangAudioRenderData::setAudioData(YangFrame* frame) {
    uint8_t *buf = getAudioData(frame);
    frame->payload = buf;
    m_res.resample(m_res.context, frame);
}

uint8_t* YangAudioRenderData::getRenderAudioData(int len) {
    if ((m_pos + m_size + len) >= m_cacheLen) {
        std::memmove(m_cache, m_cache + m_pos, m_size);
        m_pos = 0;
    }

    m_audioFrame.payload = nullptr;
    m_audioFrame.nb = 0;

    if ((m_size + (len << 1)) < m_cacheLen) setAudioData(&m_audioFrame);

    if (m_audioFrame.payload && m_audioFrame.nb > 0) {
        if ((m_pos + m_size + m_audioFrame.nb) >= m_cacheLen) {
            std::memmove(m_cache, m_cache + m_pos, m_size);
            m_pos = 0;
        }
        std::memcpy(m_cache + m_pos + m_size, m_audioFrame.payload, m_audioFrame.nb);
        m_size += m_audioFrame.nb;
    }

    if (len > m_size) return nullptr;
    uint8_t *p = m_cache + m_pos;

    m_size -= len;
    if (m_size == 0) {
        m_pos = 0;
    } else {
        m_pos += len;
    }
    return p;
}
// By GST @Date