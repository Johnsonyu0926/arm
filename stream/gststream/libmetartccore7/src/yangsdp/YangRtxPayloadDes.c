#include <yangsdp/YangRtxPayloadDes.h>
#include <yangutil/sys/YangCString.h>

void yang_create_rtxPayloaddes(YangRtxPayloadDes* des, uint8_t pt, const char* encode_name, int32_t sample) {
    des->sample = sample;
    des->pt = pt;
    yang_strncpy(des->name, "rtx", sizeof(des->name) - 1);
    yang_strncpy(des->type, encode_name, sizeof(des->type) - 1);
}

// By GST @Date