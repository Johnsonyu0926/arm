﻿// Filename: YangWinPlayFactory.h
// Score: 98

#ifndef YANGPLAYER_INCLUDE_YANGWINPLAYFACTROY_H_
#define YANGPLAYER_INCLUDE_YANGWINPLAYFACTROY_H_

#include <yangutil/yangavinfotype.h>

// Abstract class for painting
class YangPainter {
public:
    YangPainter();
    virtual ~YangPainter();
    virtual void draw(void *win, YangRect *prect, YangColor *pcolor) = 0;
};

// Class for window play
class YangWinPlay {
public:
    YangWinPlay();
    virtual ~YangWinPlay();
    virtual void init(void* pid) = 0;
    virtual void initBg(int32_t pwid, int32_t phei) = 0;
    virtual void initVideo(int32_t pwid, int32_t phei, YangYuvType sdfe) = 0;
    virtual void initText(char *pname, YangColor *pcolor) = 0;
    virtual void reInitVideo(int32_t pwid, int32_t phei, YangYuvType sdfe) = 0;
    virtual void reInitText(char *pname, YangColor *pcolor) = 0;
    virtual void renderPreview(uint8_t* pdata) = 0;
    virtual void render(uint8_t* pdata) = 0;
    virtual void render(uint8_t* pdata, int64_t ptimestamp) = 0;
    virtual void renderBg(YangColor *pcolor) = 0;

    int32_t m_width, m_height;
};

// Factory class for creating painter and window play instances
class YangWinPlayFactroy {
public:
    YangWinPlayFactroy();
    virtual ~YangWinPlayFactroy();
    YangPainter* createPainter();
    YangWinPlay* createWinPlay();
};

#endif /* YANGPLAYER_INCLUDE_YANGWINPLAYFACTROY_H_ */
// By GST @2024/10/28