// Filename: YangMediaBuffer.h
// Score: 96

#ifndef YANGUTIL_BUFFER_YANGBUFFER_H_
#define YANGUTIL_BUFFER_YANGBUFFER_H_

#include <yangutil/sys/YangThread.h>
#include <yangutil/yangtype.h>
#include <yangutil/yangavtype.h>

#define yang_reindex(p) if (p != NULL) p->resetIndex();

// Media Buffer class
class YangMediaBuffer {
public:
    // Constructor
    YangMediaBuffer();

    // Destructor
    virtual ~YangMediaBuffer();

    // Reset index
    void resetIndex();

    // Get size of buffer
    uint32_t size();

protected:
    // Initialize frames
    void initFrames(int pnum, int unitsize);

    // Put frame into buffer
    void putFrame(YangFrame* pframe);

    // Get frame from buffer
    void getFrame(YangFrame* pframe);

    // Get reference to current frame
    YangFrame* getCurFrameRef();

    // Get timestamp of next frame
    int64_t getNextFrameTimestamp();

    // Get reference to frame
    uint8_t* getFrameRef(YangFrame* pframe);

    // Protected members
    uint32_t m_putIndex;
    uint32_t m_getIndex;
    uint32_t m_cache_num;
    uint32_t m_size;
    uint32_t m_nextIndex;
    int32_t m_ret;
    YangFrame** m_frames;
    YangBufferManager* m_bufferManager;

private:
    // Private members
    yang_thread_mutex_t m_lock;

public:
    // Public members
    int32_t m_mediaType;
    int32_t m_uid;
};

#endif /* YANGUTIL_BUFFER_YANGBUFFER_H_ */
// By GST @2024/10/28