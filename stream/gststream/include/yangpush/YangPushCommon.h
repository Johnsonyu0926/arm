// Filename: YangPushCommon.h
// Score: 98

#ifndef INCLUDE_YANGPUSH_YANGPUSHCOMMON_H_
#define INCLUDE_YANGPUSH_YANGPUSHCOMMON_H_

// Video source types
#define Yang_VideoSrc_Camera 0
#define Yang_VideoSrc_Screen 1
#define Yang_VideoSrc_OutInterface 2

// Enum for push message types
enum YangPushMessageType {
    YangM_Push_StartAudioCapture,
    YangM_Push_StartVideoCapture,
    YangM_Push_StartScreenCapture,
    YangM_Push_StartOutCapture,
    YangM_Push_Connect,
    YangM_Push_Connect_Whip,
    YangM_Push_Disconnect,
    YangM_Push_Record_Start,
    YangM_Push_Record_Stop,
    YangM_Push_SwitchToCamera,
    YangM_Push_SwitchToScreen,
    YangM_Sys_Setvr,
    YangM_Sys_UnSetvr
};

#endif /* INCLUDE_YANGPUSH_YANGPUSHCOMMON_H_ */
// By GST @2024/10/28