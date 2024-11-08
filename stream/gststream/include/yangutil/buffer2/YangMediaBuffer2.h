﻿// Filename: YangBuffer2.h
// Score: 98

#ifndef YANGUTIL_BUFFER_YANGBUFFER2_H_
#define YANGUTIL_BUFFER_YANGBUFFER2_H_

#include <yangutil/sys/YangThread.h>
#include <yangutil/yangtype.h>
#include <yangutil/yangavtype.h>

#define yang_reindex2(p)  if(p != NULL) p->resetIndex(&p->mediaBuffer);

// Media Buffer Session structure
typedef struct {
    int32_t mediaType;
    int32_t uid;
    uint32_t putIndex;
    uint32_t getIndex;
    uint32_t cache_num;
    uint32_t size;
    uint32_t nextIndex;
    int32_t ret;
    yang_thread_mutex_t mlock;
    YangFrame** frames;
    uint8_t* bufferManager;
} YangMediaBufferSession2;

// Media Buffer structure
typedef struct {
    YangMediaBufferSession2 session;
    void (*resetIndex)(YangMediaBufferSession2* session);
    void (*initFrames)(YangMediaBufferSession2* session, int pnum, int unitsize);
    void (*putFrame)(YangMediaBufferSession2* session, YangFrame* pframe);
    void (*getFrame)(YangMediaBufferSession2* session, YangFrame* pframe);
    YangFrame* (*getCurFrameRef)(YangMediaBufferSession2* session);
    int64_t (*getNextFrameTimestamp)(YangMediaBufferSession2* session);
    uint8_t* (*getFrameRef)(YangMediaBufferSession2* session, YangFrame* pframe);
} YangMediaBuffer2;

// Function to create media buffer
void yang_create_mediaBuffer2(YangMediaBuffer2* buf);

// Function to destroy media buffer
void yang_destroy_mediaBuffer2(YangMediaBuffer2* buf);

#endif /* YANGUTIL_BUFFER_YANGBUFFER2_H_ */
// By GST @2024/10/28