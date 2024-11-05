// Filename: YangDecoderH264.h
// Score: 95

//
// Copyright (c) 2019-2022 yanggaofeng
//

#ifndef YANGDECODER_SRC_YangDecoderH264_H_
#define YANGDECODER_SRC_YangDecoderH264_H_

#include <yangdecoder/YangH264Dec.h>
#include <yangutil/yangtype.h>

#if !Yang_Enable_H264Decoder_So

class YangDecoderH264 {
public:
    YangDecoderH264();
    ~YangDecoderH264();
    void init(uint8_t *headers, int headerLen);
    int decode(int isIframe, unsigned char *pData, int nSize, enum YangYuvType pyuvType, unsigned char *dest, int *pnFrameReturned);
    void decode_close();

private:
    YangH264DecContext *m_context;
};

#endif

#endif /* YANGDECODER_SRC_YangDecoderH264_H_ */

// By GST @Date