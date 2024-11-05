// Filename: YangVideoEncoder.cpp
// Score: 100


#include <yangencoder/YangVideoEncoder.h>

YangVideoEncoder::YangVideoEncoder() : m_isInit(0), m_vbuffer(new uint8_t[YANG_VIDEO_ENCODE_BUFFER_LEN]) {
    memset(&m_videoInfo, 0, sizeof(YangVideoInfo));
}

YangVideoEncoder::~YangVideoEncoder() {
    delete[] m_vbuffer;
    m_vbuffer = nullptr;
}

void YangVideoEncoder::setVideoPara(YangVideoInfo *pap, YangVideoEncInfo *penc) {
    memcpy(&m_videoInfo, pap, sizeof(YangVideoInfo));
    memcpy(&m_enc, penc, sizeof(YangVideoEncInfo));
}

// By GST @Date