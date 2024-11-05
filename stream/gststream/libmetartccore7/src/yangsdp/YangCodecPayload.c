#include <yangsdp/YangCodecPayload.h>
#include <yangutil/sys/YangMemory.h>
#include <yangutil/sys/YangCString.h>

void yang_insert_fbsVector(YangFbsVector* vec, const char* str) {
    if (vec->vlen >= vec->capacity) {
        vec->capacity += 10;
        char** tmp = (char**)yang_malloc(vec->capacity * sizeof(char*));
        yang_memcpy(tmp, vec->groups, vec->vlen * sizeof(char*));
        yang_free(vec->groups);
        vec->groups = tmp;
    }
    vec->groups[vec->vlen] = (char*)yang_calloc(yang_strlen(str) + 1, 1);
    yang_strncpy(vec->groups[vec->vlen], str, yang_strlen(str));
    vec->vlen++;
}

void yang_init_sdpVector(YangFbsVector* vec) {
    vec->capacity = 10;
    vec->groups = (char**)yang_calloc(vec->capacity, sizeof(char*));
    vec->vlen = 0;
}

// By GST @Date