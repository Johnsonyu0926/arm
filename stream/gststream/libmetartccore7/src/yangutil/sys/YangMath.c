// Filename: YangMath.cpp
// Score: 99

#include <yangutil/sys/YangMath.h>
#include <yangutil/sys/YangTime.h>

#if Yang_OS_WIN
#define random rand
#define srandom srand
#endif

int16_t yang_floattoint16(float f) {
    return static_cast<int16_t>(f * 32767 + 0.5);
}

float yang_int16tofloat(int16_t i16) {
    return static_cast<float>(i16) / 32767;
}

uint64_t yang_random() {
    static yangbool _random_contexttialized = yangfalse;
    if (!_random_contexttialized) {
        _random_contexttialized = yangtrue;
        srandom(static_cast<uint64_t>(yang_update_system_time() | (getpid() << 13)));
    }
    return random();
}

void yang_insertSort(int a[], int n) {
    for (int i = 1; i < n; i++) {
        if (a[i] < a[i - 1]) {
            int j = i - 1;
            int x = a[i];
            a[i] = a[i - 1];
            while (x < a[j]) {
                a[j + 1] = a[j];
                j--;
            }
            a[j + 1] = x;
        }
    }
}

int yang_get_uint16_index(uint16_t* a, uint16_t value, int32_t alen) {
    int low = 0, high = alen, mid;
    while (low <= high) {
        mid = (high + low) / 2;
        if (value == a[mid]) return -1;
        if (yang_rtp_seq_distance(value, a[mid]) > 0)
            high = mid - 1;
        else if (yang_rtp_seq_distance(a[mid], value) > 0)
            low = mid + 1;
        else
            return mid;
    }
    return low;
}

int32_t yang_insert_uint16_sort(uint16_t* a, uint16_t value, uint32_t *palen) {
    int32_t alen = *palen;
    if (alen == 0) {
        a[0] = value;
        *palen = 1;
        return Yang_Ok;
    }
    if (yang_rtp_seq_distance(a[alen - 1], value) > 0) {
        a[alen] = value;
        *palen = alen + 1;
        return Yang_Ok;
    }
    int32_t index = yang_get_uint16_index(a, value, alen);
    if (index == -1) return 1;

    yang_memmove(reinterpret_cast<char*>(a) + (index + 1) * sizeof(uint16_t),
                 reinterpret_cast<char*>(a) + index * sizeof(uint16_t),
                 sizeof(uint16_t) * (alen - index));
    a[index] = value;
    *palen = alen + 1;
    return Yang_Ok;
}

// By GST @2024/10/28