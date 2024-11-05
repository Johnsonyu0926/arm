#ifndef SRC_YANGRTC_YANGDATACHANNEL_H_
#define SRC_YANGRTC_YANGDATACHANNEL_H_

#include <yangutil/yangtype.h>
#include <yangrtc/YangRtcContextH.h>

#if Yang_Enable_Datachannel

typedef struct {
    void *context;
    void (*on_message)(void *context, char* data, int32_t nb);
    void (*send_message)(void *context, YangFrame* msgFrame);
} YangDatachannel;

void yang_create_datachannel(YangDatachannel *datachannel, YangRtcContext* ctx) {
    if (datachannel == NULL || ctx == NULL) return;
    datachannel->context = ctx;
    datachannel->on_message = NULL;
    datachannel->send_message = NULL;
}

void yang_destroy_datachannel(YangDatachannel *datachannel) {
    if (datachannel == NULL) return;
    datachannel->context = NULL;
    datachannel->on_message = NULL;
    datachannel->send_message = NULL;
}

#endif // Yang_Enable_Datachannel

#endif /* SRC_YANGRTC_YANGDATACHANNEL_H_ */

// By GST @Date