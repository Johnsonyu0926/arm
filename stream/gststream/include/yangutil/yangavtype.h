// Filename: YangAVType.h
// Score: 95

#ifndef INCLUDE_YANGUTIL_YANGAVTYPE_H_
#define INCLUDE_YANGUTIL_YANGAVTYPE_H_

#include <yangutil/yangtype.h>

// Frame types
#define YangFrameTypeAudio 8
#define YangFrameTypeVideo 9
#define kNalTypeMask 0x1F

// Payload types
#define YangAudioPayloadType 111
#define YangAV1PayloadType 123
#define YangH264PayloadType 106
#define YangH265PayloadType 126
#define YangMjpegPayloadType 26

#define Yang_TWCC_ID 3

#define kDefaultLowLossThreshold 0.02f
#define kDefaultHighLossThreshold 0.1f

// Data channel types
enum {
    YANG_DATA_CHANNEL_DCEP = 50,
    YANG_DATA_CHANNEL_STRING = 51,
    YANG_DATA_CHANNEL_BINARY = 53,
    YANG_DATA_CHANNEL_STRING_EMPTY = 56,
    YANG_DATA_CHANNEL_BINARY_EMPTY = 57
};

// YUV types
typedef enum YangYuvType {
    YangYuy2,
    YangI420,
    YangYv12,
    YangNv12,
    YangNv21,
    YangRgb,
    YangArgb,
    YangBgra,
    YangP010,
    YangP016
} YangColorSpace;

// Rotation types
typedef enum YangRatate {
    Yang_Rotate0 = 0,
    Yang_Rotate90 = 90,
    Yang_Rotate180 = 180,
    Yang_Rotate270 = 270
} YangRatate;

// AVC levels
enum YangAvcLevel {
    YangAvcLevelReserved = 0,
    YangAvcLevel_1 = 10,
    YangAvcLevel_11 = 11,
    YangAvcLevel_12 = 12,
    YangAvcLevel_13 = 13,
    YangAvcLevel_2 = 20,
    YangAvcLevel_21 = 21,
    YangAvcLevel_22 = 22,
    YangAvcLevel_3 = 30,
    YangAvcLevel_31 = 31,
    YangAvcLevel_32 = 32,
    YangAvcLevel_4 = 40,
    YangAvcLevel_41 = 41,
    YangAvcLevel_5 = 50,
    YangAvcLevel_51 = 51
};

// AVC slice types
enum YangAvcSliceType {
    YangAvcSliceTypeP = 0,
    YangAvcSliceTypeB = 1,
    YangAvcSliceTypeI = 2,
    YangAvcSliceTypeSP = 3,
    YangAvcSliceTypeSI = 4,
    YangAvcSliceTypeP1 = 5,
    YangAvcSliceTypeB1 = 6,
    YangAvcSliceTypeI1 = 7,
    YangAvcSliceTypeSP1 = 8,
    YangAvcSliceTypeSI1 = 9
};

// AVC NALU types
typedef enum {
    YangAvcNaluTypeReserved = 0,
    YangAvcNaluTypeForbidden = 0,
    YangAvcNaluTypeNonIDR = 1,
    YangAvcNaluTypeDataPartitionA = 2,
    YangAvcNaluTypeDataPartitionB = 3,
    YangAvcNaluTypeDataPartitionC = 4,
    YangAvcNaluTypeIDR = 5,
    YangAvcNaluTypeSEI = 6,
    YangAvcNaluTypeSPS = 7,
    YangAvcNaluTypePPS = 8,
    YangAvcNaluTypeAccessUnitDelimiter = 9,
    YangAvcNaluTypeEOSequence = 10,
    YangAvcNaluTypeEOStream = 11,
    YangAvcNaluTypeFilterData = 12,
    YangAvcNaluTypeSPSExt = 13,
    YangAvcNaluTypePrefixNALU = 14,
    YangAvcNaluTypeSubsetSPS = 15,
    YangAvcNaluTypeLayerWithoutPartition = 19,
    YangAvcNaluTypeCodedSliceExt = 20
} YangAvcNaluType;

// AVC profiles
enum YangAvcProfile {
    YangAvcProfileReserved = 0,
    YangAvcProfileBaseline = 66,
    YangAvcProfileConstrainedBaseline = 578,
    YangAvcProfileMain = 77,
    YangAvcProfileExtended = 88,
    YangAvcProfileHigh = 100,
    YangAvcProfileHigh10 = 110,
    YangAvcProfileHigh10Intra = 2158,
    YangAvcProfileHigh422 = 122,
    YangAvcProfileHigh422Intra = 2170,
    YangAvcProfileHigh444 = 144,
    YangAvcProfileHigh444Predictive = 244,
    YangAvcProfileHigh444Intra = 2192
};

// Structures
struct YangRect {
    short x;
    short y;
    short w;
    short h;
};

struct YangColor {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

typedef struct {
    int32_t vpsLen;
    int32_t spsLen;
    int32_t ppsLen;
    uint8_t vps[128];
    uint8_t sps[128];
    uint8_t pps[64];
} YangH2645Conf;

typedef struct {
    uint8_t buffer[128];
    int32_t bufLen;
} YangRtmpMeta;

typedef struct {
    int32_t isInit;
    YangH2645Conf mp4Meta;
    YangRtmpMeta livingMeta;
} YangVideoMeta;

enum YangVideoHwType {
    Yang_Hw_Soft,
    YangV_Hw_Intel,
    YangV_Hw_Nvdia,
    YangV_Hw_Android
};

struct YangMessage {
    int32_t mediaType;
    int32_t nb;
    int64_t timestamp;
    char* payload;
};

typedef struct {
    int32_t nb;
    char* bytes;
} YangSample;

YangSample* yang_sample_copy(YangSample* src);

#include <yangutil/yangavctype.h>

#ifdef __cplusplus
extern "C" {
#include <yangutil/yangframebuffer.h>
}
#else
#include <yangutil/yangframebuffer.h>
#endif

#ifdef __cplusplus
class YangBufferManager {
public:
    YangBufferManager();
    YangBufferManager(int32_t num, int32_t bufsize);
    virtual ~YangBufferManager();
    void init(int32_t num, int32_t bufsize);
    uint8_t* getBuffer();
private:
    uint8_t* m_cache;
    int32_t m_curindex;
    int32_t m_unitsize;
    int32_t m_size;
};
#endif

#endif /* INCLUDE_YANGUTIL_YANGAVTYPE_H_ */
// By GST @2024/10/28