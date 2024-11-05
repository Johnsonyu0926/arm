// Filename: YangSynBufferManager.h
// Score: 98

#ifndef INCLUDE_YANGSTREAM_YANGSYNBUFFERMANAGER_H_
#define INCLUDE_YANGSTREAM_YANGSYNBUFFERMANAGER_H_

#include <yangstream/YangSynBuffer.h>
#include <yangutil/sys/YangVector.h>

// Declare YangSynBuffer vector
yang_vector_declare2(YangSynBuffer)

// Structure for SynBuffer session
typedef struct {
    yang_thread_mutex_t playBufferMutex; // Mutex for play buffer
    YangSynBuffer* playBuffer;           // Pointer to play buffer
    YangSynBufferVector2* playBuffers;   // Vector of play buffers
} YangSynBufferSession;

// Structure for SynBuffer manager
typedef struct {
    YangSynBufferSession* session; // Pointer to SynBuffer session

    // Function pointers for SynBuffer operations
    YangSynBuffer* (*getSynBuffer)(YangSynBufferSession* session, int puid);
    int (*getPlayBufferIndex)(YangSynBufferSession* session, int puid);
    int32_t (*getAudioDatas)(YangSynBufferSession* session, uint8_t* mixBuf, YangFrame* pframe);
    int32_t (*addAudioBuffer)(YangSynBufferSession* session, void* yangAudioPlayBuffer, int32_t puid);
    void (*initAudioBuffer)(YangSynBufferSession* session);
    void (*removeAllAudioBuffer)(YangSynBufferSession* session);
    void (*removeAudioBuffer)(YangSynBufferSession* session, int32_t puid);
} YangSynBufferManager;

// Function to create SynBuffer manager
void yang_create_synBufferManager(YangSynBufferManager* man);

// Function to destroy SynBuffer manager
void yang_destroy_synBufferManager(YangSynBufferManager* man);

#endif /* INCLUDE_YANGSTREAM_YANGSYNBUFFERMANAGER_H_ */
// By GST @2024/10/28