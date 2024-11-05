// Filename: YangAVSyn.h
// Score: 98

#ifndef YANGPLAYER_INCLUDE_YANGAVSYN_H_
#define YANGPLAYER_INCLUDE_YANGAVSYN_H_

#include <yangutil/sys/YangTime.h>
#include <yangutil/yangavtype.h>
#include <yangutil/buffer/YangAudioPlayBuffer.h>
#include <yangavutil/audio/YangAudioMix.h>
#include <yangutil/buffer/YangVideoBuffer.h>

// Synchronization types
enum YangSynType {
    YANG_SYNC_AUDIO_MASTER,     // Default choice
    YANG_SYNC_VIDEO_MASTER,
    YANG_SYNC_EXTERNAL_CLOCK,   // Synchronize to an external clock
};

// Constants for synchronization intervals and cache times
#define Yang_Max_Audio_Intervaltime 25
#define Yang_Max_Video_Intervaltime 35
#define Yang_Video_Base_Update_Interval 600000 // 10 minutes
#define Yang_Audio_Base_Update_Interval 5000   // 5 seconds
#define Yang_Video_Cache_time 35
#define Yang_Audio_Cache_time 5

// Structure for synchronization buffer
typedef struct {
    void* session;

    // Function pointers for buffer operations
    uint8_t* (*getVideoRef)(void* session, YangFrame* pframe);
    uint8_t* (*getAudioRef)(void* session, YangFrame* audioFrame);
    void (*setInAudioBuffer)(void* session, YangAudioPlayBuffer* paudioList);
    void (*setInVideoBuffer)(void* session, YangVideoBuffer* pbuf);
    void (*setAudioClock)(void* session, int32_t paudioclock);
    void (*setVideoClock)(void* session, int32_t pvideoclock);
    void (*setUid)(void* session, int32_t uid);
    int32_t (*getUid)(void* session);
    int32_t (*getAudioSize)(void* session);
    int32_t (*getVideoSize)(void* session);
    void (*resetVideoClock)(void* psession);
    void (*resetAudioClock)(void* psession);
    int32_t (*width)(void* psession);
    int32_t (*height)(void* psession);
} YangSynBuffer;

// Function to create synchronization buffer
void yang_create_synBuffer(YangSynBuffer* syn);

// Function to destroy synchronization buffer
void yang_destroy_synBuffer(YangSynBuffer* syn);

#endif /* YANGPLAYER_INCLUDE_YANGAVSYN_H_ */
// By GST @2024/10/28