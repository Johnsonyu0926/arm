// Filename: YangVector.cpp
// Score: 95

#include <yangutil/sys/YangVector.h>
#include <yangutil/sys/YangMath.h>

// Insert a string into the vector
void yang_insert_stringVector(YangStringVector* vec, const char* str) {
    if (vec == nullptr || str == nullptr) return;
    yang_insert_stringVector2(vec, str, yang_strlen(str) + 1);
}

// Insert a string with specified length into the vector
void yang_insert_stringVector2(YangStringVector* vec, const char* str, int plen) {
    if (vec == nullptr || str == nullptr) return;
    if (vec->vsize >= vec->capacity) {
        char** tmp = (char**)yang_malloc((vec->capacity + 5) * sizeof(char*));
        yang_memcpy(tmp, vec->payload, vec->vsize * sizeof(char*));
        yang_free(vec->payload);
        vec->payload = tmp;
        vec->capacity += 5;
    }
    vec->payload[vec->vsize] = (char*)yang_calloc(plen, 1);
    yang_memcpy(vec->payload[vec->vsize], str, plen);
    vec->vsize++;
}

// Create a string vector
void yang_create_stringVector(YangStringVector* vec) {
    if (vec == nullptr) return;
    vec->capacity = 5;
    vec->payload = (char**)yang_calloc(vec->capacity * sizeof(char*), 1);
    vec->vsize = 0;
}

// Destroy a string vector
void yang_destroy_stringVector(YangStringVector* vec) {
    if (vec == nullptr) return;
    for (int i = 0; i < vec->vsize; i++) {
        yang_free(vec->payload[i]);
    }
    vec->vsize = 0;
    vec->capacity = 0;
    yang_free(vec->payload);
}

// Clear a string vector
void yang_clear_stringVector(YangStringVector* vec) {
    if (vec == nullptr) return;
    yang_memset(vec->payload, 0, sizeof(char*) * vec->capacity);
    vec->vsize = 0;
}

// Insert a uint16_t value into the sorted vector
void yang_insert_uint16Sort(YangUint16Sort* psort, uint16_t val) {
    if (psort->vsize >= psort->capacity - 1) {
        uint16_t* tmp = (uint16_t*)yang_calloc((psort->capacity + 50) * sizeof(uint16_t), 1);
        yang_memcpy(tmp, psort->payload, psort->vsize * sizeof(uint16_t));
        yang_free(psort->payload);
        psort->payload = tmp;
        psort->capacity += 50;
    }
    yang_insert_uint16_sort(psort->payload, val, &psort->vsize);
}

// Create a uint16_t sorted vector with default capacity
void yang_create_uint16Sort(YangUint16Sort* psort) {
    yang_create_uint16Sort2(psort, 50);
}

// Create a uint16_t sorted vector with specified capacity
void yang_create_uint16Sort2(YangUint16Sort* psort, int32_t pcapacity) {
    if (psort == nullptr) return;
    psort->capacity = pcapacity;
    if (psort->payload == nullptr) {
        psort->payload = (uint16_t*)yang_calloc(sizeof(uint16_t) * psort->capacity, 1);
    }
}

// Destroy a uint16_t sorted vector
void yang_destroy_uint16Sort(YangUint16Sort* psort) {
    if (psort == nullptr) return;
    yang_free(psort->payload);
    psort->capacity = 0;
    psort->vsize = 0;
}

// Clear a uint16_t sorted vector
void yang_clear_uint16Sort(YangUint16Sort* psort) {
    psort->vsize = 0;
}

// Clear a uint16_t sorted vector from a specified index
void yang_clear_uint16Sort2(YangUint16Sort* psort, int32_t index) {
    if (psort->vsize == 0) return;
    int32_t vsize = psort->vsize - index - 1;
    yang_memmove((char*)psort->payload, (char*)psort->payload + index * sizeof(uint16_t), vsize * sizeof(uint16_t));
    psort->vsize = vsize;
}

// By GST @2024/10/28