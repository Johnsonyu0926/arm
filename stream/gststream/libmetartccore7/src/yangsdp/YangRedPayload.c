#include <yangsdp/YangRedPayload.h>
#include <yangutil/sys/YangCString.h>

void yang_create_redPayloaddes(YangRedPayload* red, uint8_t pt, const char* encode_name, int32_t sample, int32_t channel) {
    if (red == NULL || encode_name == NULL) return;

    red->sample = sample;
    red->pt = pt;
    yang_strncpy(red->name, encode_name, sizeof(red->name) - 1);
    red->name[sizeof(red->name) - 1] = '\0'; // Ensure null-termination
    red->channel = channel;
}

// By GST @Date