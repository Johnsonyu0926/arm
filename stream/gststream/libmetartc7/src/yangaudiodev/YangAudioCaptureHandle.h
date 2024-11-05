// Filename: YangAudioCaptureHandle.cpp
// Score: 94



#include <yangaudiodev/YangAudioCaptureHandle.h>
#include <yangutil/sys/YangLog.h>
#include <cstring>

YangAudioCaptureHandle::YangAudioCaptureHandle(YangAVInfo *avinfo) {
    if (avinfo == nullptr) return;

    m_enableBuf = false;
    m_audioList = nullptr;
    m_aec = nullptr;
    m_aecPlayBuffer = nullptr;
    pcm = new short[2048];
    m_audioLen = avinfo->audio.sample * avinfo->audio.channel * 2 / 50;

    hasPlayData = 1;
    isFirst = 1;
    m_aecBufferFrames = avinfo->audio.aecBufferFrames;
    std::memset(&m_audioFrame, 0, sizeof(YangFrame));
    std::memset(&m_resample, 0, sizeof(YangAudioResample));
    yang_create_audioresample(&m_resample);
    m_resample.init(m_resample.context, 16000, 1, avinfo->audio.sample, avinfo->audio.channel, 20);
}

YangAudioCaptureHandle::~YangAudioCaptureHandle() {
    m_aec = nullptr;
    m_aecPlayBuffer = nullptr;
    m_audioList = nullptr;
    delete[] pcm;
    yang_destroy_audioresample(&m_resample);
}

void YangAudioCaptureHandle::startRecordWave(char* filename) {
    // Implementation needed
}

void YangAudioCaptureHandle::stopRecordWave() {
    // Implementation needed
}

void YangAudioCaptureHandle::setOutAudioBuffer(YangAudioBuffer *pbuf) {
    m_audioList = pbuf;
}

void YangAudioCaptureHandle::putBuffer(uint8_t *pBuffer, int32_t plen) {
    if (!m_enableBuf) return;

    if (m_aec) {
        if (hasPlayData) {
            m_aec->echoCapture(m_aec->session, reinterpret_cast<short*>(pBuffer), pcm);
            m_aec->preprocessRun(m_aec->session, pcm);
        }

        if (m_aecPlayBuffer && m_aecPlayBuffer->size() > m_aecBufferFrames) {
            uint8_t* tmp = m_aecPlayBuffer->getAudioRef(&m_audioFrame);
            if (tmp) m_aec->echoPlayback(m_aec->session, reinterpret_cast<short*>(tmp));

            if (isFirst) {
                if (m_audioList) m_audioList->resetIndex();
                isFirst = 0;
            }
            hasPlayData = 1;
        } else {
            hasPlayData = 0;
        }

        if (hasPlayData) {
            if (m_audioList) {
                m_audioFrame.payload = reinterpret_cast<uint8_t*>(pcm);
                m_audioFrame.nb = plen;
                m_audioList->putAudio(&m_audioFrame);
            }
        } else {
            if (m_audioList) {
                m_audioFrame.payload = pBuffer;
                m_audioFrame.nb = plen;
                m_audioList->putAudio(&m_audioFrame);
            }
        }
    } else {
        if (m_audioList) {
            m_audioFrame.payload = pBuffer;
            m_audioFrame.nb = plen;
            m_audioList->putAudio(&m_audioFrame);
        }
    }
}

void YangAudioCaptureHandle::putEchoPlay(short* pbuf, int32_t plen) {
    if (!m_enableBuf) return;
    if (m_aec) m_aec->echoPlayback(m_aec->session, pbuf);
}

void YangAudioCaptureHandle::putEchoBuffer(uint8_t *pBuffer, int32_t plen) {
    if (!m_enableBuf) return;
    if (m_aec) {
        m_aec->echoCapture(m_aec->session, reinterpret_cast<short*>(pBuffer), pcm);
        m_aec->preprocessRun(m_aec->session, pcm);
        m_audioFrame.payload = reinterpret_cast<uint8_t*>(pcm);
        m_audioFrame.nb = plen;
        m_resample.resample(m_resample.context, &m_audioFrame);
        if (m_audioList) {
            m_audioList->putAudio(&m_audioFrame);
        }
    }
}

void YangAudioCaptureHandle::putEchoBuffer2(uint8_t *pBuffer, int32_t plen) {
    if (!m_enableBuf) return;
    m_audioFrame.payload = pBuffer;
    m_audioFrame.nb = plen;
    m_resample.resample(m_resample.context, &m_audioFrame);
    if (m_audioList) {
        m_audioList->putAudio(&m_audioFrame);
    }
}

void YangAudioCaptureHandle::putBuffer2(uint8_t *pBuffer, int32_t plen) {
    if (!m_enableBuf) return;
    if (m_audioList) {
        m_audioFrame.payload = pBuffer;
        m_audioFrame.nb = plen;
        m_audioList->putAudio(&m_audioFrame);
    }
}
// By GST @Date