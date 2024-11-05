// Filename: x265.h
// Score: 98

#ifndef X265_H
#define X265_H

#include <stdint.h>
#include <stdio.h>
#include "x265_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#if _MSC_VER
#pragma warning(disable: 4201) // non-standard extension used (nameless struct/union)
#endif

typedef struct x265_encoder x265_encoder;
typedef struct x265_picyuv x265_picyuv;

#ifdef X265_API_IMPORTS
#define X265_API __declspec(dllimport)
#else
#define X265_API
#endif

typedef enum {
    NAL_UNIT_CODED_SLICE_TRAIL_N = 0,
    NAL_UNIT_CODED_SLICE_TRAIL_R,
    NAL_UNIT_CODED_SLICE_TSA_N,
    NAL_UNIT_CODED_SLICE_TLA_R,
    NAL_UNIT_CODED_SLICE_STSA_N,
    NAL_UNIT_CODED_SLICE_STSA_R,
    NAL_UNIT_CODED_SLICE_RADL_N,
    NAL_UNIT_CODED_SLICE_RADL_R,
    NAL_UNIT_CODED_SLICE_RASL_N,
    NAL_UNIT_CODED_SLICE_RASL_R,
    NAL_UNIT_CODED_SLICE_BLA_W_LP = 16,
    NAL_UNIT_CODED_SLICE_BLA_W_RADL,
    NAL_UNIT_CODED_SLICE_BLA_N_LP,
    NAL_UNIT_CODED_SLICE_IDR_W_RADL,
    NAL_UNIT_CODED_SLICE_IDR_N_LP,
    NAL_UNIT_CODED_SLICE_CRA,
    NAL_UNIT_VPS = 32,
    NAL_UNIT_SPS,
    NAL_UNIT_PPS,
    NAL_UNIT_ACCESS_UNIT_DELIMITER,
    NAL_UNIT_EOS,
    NAL_UNIT_EOB,
    NAL_UNIT_FILLER_DATA,
    NAL_UNIT_PREFIX_SEI,
    NAL_UNIT_SUFFIX_SEI,
    NAL_UNIT_UNSPECIFIED = 62,
    NAL_UNIT_INVALID = 64,
} NalUnitType;

typedef struct x265_nal {
    uint32_t type;
    uint32_t sizeBytes;
    uint8_t* payload;
} x265_nal;

#define X265_LOOKAHEAD_MAX 250

typedef struct x265_lookahead_data {
    int64_t plannedSatd[X265_LOOKAHEAD_MAX + 1];
    uint32_t *vbvCost;
    uint32_t *intraVbvCost;
    uint32_t *satdForVbv;
    uint32_t *intraSatdForVbv;
    int keyframe;
    int lastMiniGopBFrame;
    int plannedType[X265_LOOKAHEAD_MAX + 1];
    int64_t dts;
    int64_t reorderedPts;
} x265_lookahead_data;

typedef struct x265_analysis_validate {
    int maxNumReferences;
    int analysisReuseLevel;
    int sourceWidth;
    int sourceHeight;
    int keyframeMax;
    int keyframeMin;
    int openGOP;
    int bframes;
    int bPyramid;
    int maxCUSize;
    int minCUSize;
    int intraRefresh;
    int lookaheadDepth;
    int chunkStart;
    int chunkEnd;
    int cuTree;
    int ctuDistortionRefine;
    int rightOffset;
    int bottomOffset;
    int frameDuplication;
} x265_analysis_validate;

typedef struct x265_analysis_intra_data {
    uint8_t* depth;
    uint8_t* modes;
    char* partSizes;
    uint8_t* chromaModes;
    int8_t* cuQPOff;
} x265_analysis_intra_data;

typedef struct x265_analysis_MV {
    union {
        struct { int32_t x, y; };
        int64_t word;
    };
} x265_analysis_MV;

typedef struct x265_analysis_inter_data {
    int32_t* ref;
    uint8_t* depth;
    uint8_t* modes;
    uint8_t* partSize;
    uint8_t* mergeFlag;
    uint8_t* interDir;
    uint8_t* mvpIdx[2];
    int8_t* refIdx[2];
    x265_analysis_MV* mv[2];
    int64_t* sadCost;
    int8_t* cuQPOff;
} x265_analysis_inter_data;

typedef struct x265_weight_param {
    uint32_t log2WeightDenom;
    int inputWeight;
    int inputOffset;
    int wtPresent;
} x265_weight_param;

#if X265_DEPTH < 10
typedef uint32_t sse_t;
#else
typedef uint64_t sse_t;
#endif

#define CTU_DISTORTION_OFF 0
#define CTU_DISTORTION_INTERNAL 1
#define CTU_DISTORTION_EXTERNAL 2

typedef struct x265_analysis_distortion_data {
    sse_t* ctuDistortion;
    double* scaledDistortion;
    double averageDistortion;
    double sdDistortion;
    uint32_t highDistortionCtuCount;
    uint32_t lowDistortionCtuCount;
    double* offset;
    double* threshold;
} x265_analysis_distortion_data;

#define MAX_NUM_REF 16

typedef struct x265_analysis_data {
    int64_t satdCost;
    uint32_t frameRecordSize;
    uint32_t poc;
    uint32_t sliceType;
    uint32_t numCUsInFrame;
    uint32_t numPartitions;
    uint32_t depthBytes;
    int bScenecut;
    x265_weight_param* wt;
    x265_analysis_inter_data* interData;
    x265_analysis_intra_data* intraData;
    uint32_t numCuInHeight;
    x265_lookahead_data lookahead;
    uint8_t* modeFlag[2];
    x265_analysis_validate saveParam;
    x265_analysis_distortion_data* distortionData;
    uint64_t frameBits;
    int list0POC[MAX_NUM_REF];
    int list1POC[MAX_NUM_REF];
    double totalIntraPercent;
} x265_analysis_data;

typedef struct x265_cu_stats {
    double percentSkipCu[4];
    double percentMergeCu[4];
    double percentIntraDistribution[4][3];
    double percentInterDistribution[4][3];
    double percentIntraNxN;
} x265_cu_stats;

typedef struct x265_pu_stats {
    double percentSkipPu[4];
    double percentIntraPu[4];
    double percentAmpPu[4];
    double percentInterPu[4][3];
    double percentMergePu[4][3];
    double percentNxN;
} x265_pu_stats;

typedef struct x265_frame_stats {
    double qp;
    double rateFactor;
    double psnrY;
    double psnrU;
    double psnrV;
    double psnr;
    double ssim;
    double decideWaitTime;
    double row0WaitTime;
    double wallTime;
    double refWaitWallTime;
    double totalCTUTime;
    double stallTime;
    double avgWPP;
    double avgLumaDistortion;
    double avgChromaDistortion;
    double avgPsyEnergy;
    double avgResEnergy;
    double avgLumaLevel;
    double bufferFill;
    uint64_t bits;
    int encoderOrder;
    int poc;
    int countRowBlocks;
    int list0POC[MAX_NUM_REF];
    int list1POC[MAX_NUM_REF];
    uint16_t maxLumaLevel;
    uint16_t minLumaLevel;
    uint16_t maxChromaULevel;
    uint16_t minChromaULevel;
    double avgChromaULevel;
    uint16_t maxChromaVLevel;
    uint16_t minChromaVLevel;
    double avgChromaVLevel;
    char sliceType;
    int bScenecut;
    double ipCostRatio;
    int frameLatency;
    x265_cu_stats cuStats;
    x265_pu_stats puStats;
    double totalFrameTime;
    double vmafFrameScore;
    double bufferFillFinal;
    double unclippedBufferFillFinal;
} x265_frame_stats;

typedef struct x265_ctu_info_t {
    int32_t ctuAddress;
    int32_t ctuPartitions[64];
    void* ctuInfo;
} x265_ctu_info_t;

typedef enum {
    NO_CTU_INFO = 0,
    HAS_CTU_INFO = 1,
    CTU_INFO_CHANGE = 2,
} CTUInfo;

typedef enum {
    DEFAULT = 0,
    AVC_INFO = 1,
    HEVC_INFO = 2,
} AnalysisRefineType;

typedef enum {
    BUFFERING_PERIOD = 0,
    PICTURE_TIMING = 1,
    PAN_SCAN_RECT = 2,
    FILLER_PAYLOAD = 3,
    USER_DATA_REGISTERED_ITU_T_T35 = 4,
    USER_DATA_UNREGISTERED = 5,
    RECOVERY_POINT = 6,
    SCENE_INFO = 9,
    FULL_FRAME_SNAPSHOT = 15,
    PROGRESSIVE_REFINEMENT_SEGMENT_START = 16,
    PROGRESSIVE_REFINEMENT_SEGMENT_END = 17,
    FILM_GRAIN_CHARACTERISTICS = 19,
    POST_FILTER_HINT = 22,
    TONE_MAPPING_INFO = 23,
    FRAME_PACKING = 45,
    DISPLAY_ORIENTATION = 47,
    SOP_DESCRIPTION = 128,
    ACTIVE_PARAMETER_SETS = 129,
    DECODING_UNIT_INFO = 130,
    TEMPORAL_LEVEL0_INDEX = 131,
    DECODED_PICTURE_HASH = 132,
    SCALABLE_NESTING = 133,
    REGION_REFRESH_INFO = 134,
    MASTERING_DISPLAY_INFO = 137,
    CONTENT_LIGHT_LEVEL_INFO = 144,
    ALTERNATIVE_TRANSFER_CHARACTERISTICS = 147,
} SEIPayloadType;

typedef struct x265_sei_payload {
    int payloadSize;
    SEIPayloadType payloadType;
    uint8_t* payload;
} x265_sei_payload;

typedef struct x265_sei {
    int numPayloads;
    x265_sei_payload *payloads;
} x265_sei;

typedef struct x265_dolby_vision_rpu {
    int payloadSize;
    uint8_t* payload;
} x265_dolby_vision_rpu;

typedef struct x265_picture {
    int64_t pts;
    int64_t dts;
    void* userData;
    void* planes[3];
    int stride[3];
    int bitDepth;
    int sliceType;
    int poc;
    int colorSpace;
    int forceqp;
    x265_analysis_data analysisData;
    float *quantOffsets;
    x265_frame_stats frameData;
    x265_sei userSEI;
    void* rcData;
    size_t framesize;
    int height;
    int64_t reorderedPts;
    x265_dolby_vision_rpu rpu;
    int fieldNum;
    uint32_t picStruct;
    int width;
} x265_picture;

typedef enum {
    X265_DIA_SEARCH,
    X265_HEX_SEARCH,
    X265_UMH_SEARCH,
    X265_STAR_SEARCH,
    X265_SEA,
    X265_FULL_SEARCH
} X265_ME_METHODS;

#define X265_CPU_MMX (1 << 0)
#define X265_CPU_MMX2 (1 << 1)
#define X265_CPU_MMXEXT X265_CPU_MMX2
#define X265_CPU_SSE (1 << 2)
#define X265_CPU_SSE2 (1 << 3)
#define X265_CPU_LZCNT (1 << 4)
#define X265_CPU_SSE3 (1 << 5)
#define X265_CPU_SSSE3 (1 << 6)
#define X265_CPU_SSE4 (1 << 7)
#define X265_CPU_SSE42 (1 << 8)
#define X265_CPU_AVX (1 << 9)
#define X265_CPU_XOP (1 << 10)
#define X265_CPU_FMA4 (1 << 11)
#define X265_CPU_FMA3 (1 << 12)
#define X265_CPU_BMI1 (1 << 13)
#define X265_CPU_BMI2 (1 << 14)
#define X265_CPU_AVX2 (1 << 15)
#define X265_CPU_AVX512 (1 << 16)
#define X265_CPU_CACHELINE_32 (1 << 17)
#define X265_CPU_CACHELINE_64 (1 << 18)
#define X265_CPU_SSE2_IS_SLOW (1 << 19)
#define X265_CPU_SSE2_IS_FAST (1 << 20)
#define X265_CPU_SLOW_SHUFFLE (1 << 21)
#define X265_CPU_STACK_MOD4 (1 << 22)
#define X265_CPU_SLOW_ATOM (1 << 23)
#define X265_CPU_SLOW_PSHUFB (1 << 24)
#define X265_CPU_SLOW_PALIGNR (1 << 25)

#define X265_CPU_ARMV6 0x0000001
#define X265_CPU_NEON 0x0000002
#define X265_CPU_FAST_NEON_MRC 0x0000004

#define X265_CPU_ALTIVEC 0x0000001

#define X265_MAX_SUBPEL_LEVEL 7
#define X265_LOG_NONE (-1)
#define X265_LOG_ERROR 0
#define X265_LOG_WARNING 1
#define X265_LOG_INFO 2
#define X265_LOG_DEBUG 3
#define X265_LOG_FULL 4

#define X265_B_ADAPT_NONE 0
#define X265_B_ADAPT_FAST 1
#define X265_B_ADAPT_TRELLIS 2

#define X265_REF_LIMIT_DEPTH 1
#define X265_REF_LIMIT_CU 2
#define X265_TU_LIMIT_BFS 1
#define X265_TU_LIMIT_DFS 2
#define X265_TU_LIMIT_NEIGH 4

#define X265_BFRAME_MAX 16
#define X265_MAX_FRAME_THREADS 16

#define X265_TYPE_AUTO 0x0000
#define X265_TYPE_IDR 0x0001
#define X265_TYPE_I 0x0002
#define X265_TYPE_P 0x0003
#define X265_TYPE_BREF 0x0004
#define X265_TYPE_B 0x0005
#define IS_X265_TYPE_I(x) ((x) == X265_TYPE_I || (x) == X265_TYPE_IDR)
#define IS_X265_TYPE_B(x) ((x) == X265_TYPE_B || (x) == X265_TYPE_BREF)

#define X265_QP_AUTO 0

#define X265_AQ_NONE 0
#define X265_AQ_VARIANCE 1
#define X265_AQ_AUTO_VARIANCE 2
#define X265_AQ_AUTO_VARIANCE_BIASED 3
#define X265_AQ_EDGE 4

#define x265_ADAPT_RD_STRENGTH 4
#define X265_REFINE_INTER_LEVELS 3
#define X265_CSP_I400 0
#define X265_CSP_I420 1
#define X265_CSP_I422 2
#define X265_CSP_I444 3
#define X265_CSP_COUNT 4
#define X265_CSP_NV12 4
#define X265_CSP_NV16 5
#define X265_CSP_BGR 6
#define X265_CSP_BGRA 7
#define X265_CSP_RGB 8
#define X265_CSP_MAX 9
#define X265_EXTENDED_SAR 255
#define X265_ANALYSIS_OFF 0
#define X265_ANALYSIS_SAVE 1
#define X265_ANALYSIS_LOAD 2

typedef struct x265_cli_csp {
    int planes;
    int width[3];
    int height[3];
} x265_cli_csp;

static const x265_cli_csp x265_cli_csps[] = {
    { 1, { 0, 0, 0 }, { 0, 0, 0 } },
    { 3, { 0, 1, 1 }, { 0, 1, 1 } },
    { 3, { 0, 1, 1 }, { 0, 0, 0 } },
    { 3, { 0, 0, 0 }, { 0, 0, 0 } },
    { 2, { 0, 0 }, { 0, 1 } },
    { 2, { 0, 0 }, { 0, 0 } },
};

typedef enum {
    X265_RC_ABR,
    X265_RC_CQP,
    X265_RC_CRF
} X265_RC_METHODS;

typedef struct x265_sliceType_stats {
    double avgQp;
    double bitrate;
    double psnrY;
    double psnrU;
    double psnrV;
    double ssim;
    uint32_t numPics;
} x265_sliceType_stats;

typedef struct x265_stats {
    double globalPsnrY;
    double globalPsnrU;
    double globalPsnrV;
    double globalPsnr;
    double globalSsim;
    double elapsedEncodeTime;
    double elapsedVideoTime;
    double bitrate;
    double aggregateVmafScore;
    uint64_t accBits;
    uint32_t encodedPictureCount;
    uint32_t totalWPFrames;
    x265_sliceType_stats statsI;
    x265_sliceType_stats statsP;
    x265_sliceType_stats statsB;
    uint16_t maxCLL;
    uint16_t maxFALL;
} x265_stats;

static const char * const x265_motion_est_names[] = { "dia", "hex", "umh", "star", "sea", "full", 0 };
static const char * const x265_source_csp_names[] = { "i400", "i420", "i422", "i444", "nv12", "nv16", 0 };
static const char * const x265_video_format_names[] = { "component", "pal", "ntsc", "secam", "mac", "unknown", 0 };
static const char * const x265_fullrange_names[] = { "limited", "full", 0 };
static const char * const x265_colorprim_names[] = { "reserved", "bt709", "unknown", "reserved", "bt470m", "bt470bg", "smpte170m", "smpte240m", "film", "bt2020", "smpte428", "smpte431", "smpte432", 0 };
static const char * const x265_transfer_names[] = { "reserved", "bt709", "unknown", "reserved", "bt470m", "bt470bg", "smpte170m", "smpte240m", "linear", "log100",
                                                    "log316", "iec61966-2-4", "bt1361e", "iec61966-2-1", "bt2020-10", "bt2020-12",
                                                    "smpte2084", "smpte428", "arib-std-b67", 0 };
static const char * const x265_colmatrix_names[] = { "gbr", "bt709", "unknown", "", "fcc", "bt470bg", "smpte170m", "smpte240m",
                                                     "ycgco", "bt2020nc", "bt2020c", "smpte2085", "chroma-derived-nc", "chroma-derived-c", "ictcp", 0 };
static const char * const x265_sar_names[] = { "unknown", "1:1", "12:11", "10:11", "16:11", "40:33", "24:11", "20:11",
                                               "32:11", "80:33", "18:11", "15:11", "64:33", "160:99", "4:3", "3:2", "2:1", 0 };
static const char * const x265_interlace_names[] = { "prog", "tff", "bff", 0 };
static const char * const x265_analysis_names[] = { "off", "save", "load", 0 };

struct x265_zone;
struct x265_param;
typedef struct x265_zone {
    int startFrame, endFrame;
    int bForceQp;
    int qp;
    float bitrateFactor;
    struct x265_param* zoneParam;
    double* relativeComplexity;
} x265_zone;

typedef struct x265_vmaf_data {
    int width;
    int height;
    size_t offset;
    int internalBitDepth;
    FILE *reference_file;
    FILE *distorted_file;
} x265_vmaf_data;

typedef struct x265_vmaf_framedata {
    int width;
    int height;
    int frame_set;
    int internalBitDepth;
    void *reference_frame;
    void *distorted_frame;
} x265_vmaf_framedata;

typedef struct x265_vmaf_commondata {
    char *format;
    char *model_path;
    char *log_path;
    char *log_fmt;
    int disable_clip;
    int disable_avx;
    int enable_transform;
    int phone_model;
    int psnr;
    int ssim;
    int ms_ssim;
    char *pool;
    int thread;
    int subsample;
    int enable_conf_interval;
} x265_vmaf_commondata;

static const x265_vmaf_commondata vcd[] = { { NULL, (char *)"/usr/local/share/model/vmaf_v0.6.1.pkl", NULL, NULL, 0, 0, 0, 0, 0, 0, 0, NULL, 0, 1, 0 } };

typedef struct x265_param {
    int cpuid;
    int frameNumThreads;
    const char* numaPools;
    int bEnableWavefront;
    int bDistributeModeAnalysis;
    int bDistributeMotionEstimation;
    int bLogCuStats;
    int bEnablePsnr;
    int bEnableSsim;
    int logLevel;
    int csvLogLevel;
    const char* csvfn;
    int internalBitDepth;
    int internalCsp;
    uint32_t fpsNum;
    uint32_t fpsDenom;
    int sourceWidth;
    int sourceHeight;
    int interlaceMode;
    int totalFrames;
    int levelIdc;
    int bHighTier;
    int uhdBluray;
    int maxNumReferences;
    int bAllowNonConformance;
    int bRepeatHeaders;
    int bAnnexB;
    int bEnableAccessUnitDelimiters;
    int bEmitHRDSEI;
    int bEmitInfoSEI;
    int decodedPictureHashSEI;
    int bEnableTemporalSubLayers;
    int bOpenGOP;
    int keyframeMin;
    int keyframeMax;
    int bframes;
    int bFrameAdaptive;
    int bBPyramid;
    int bFrameBias;
    int lookaheadDepth;
    int lookaheadSlices;
    int scenecutThreshold;
    int bIntraRefresh;
    uint32_t maxCUSize;
    uint32_t minCUSize;
    int bEnableRectInter;
    int bEnableAMP;
    uint32_t maxTUSize;
    uint32_t tuQTMaxInterDepth;
    uint32_t tuQTMaxIntraDepth;
    uint32_t limitTU;
    int rdoqLevel;
    int bEnableSignHiding;
    int bEnableTransformSkip;
    int noiseReductionIntra;
    int noiseReductionInter;
    const char *scalingLists;
    int bEnableConstrainedIntra;
    int bEnableStrongIntraSmoothing;
    uint32_t maxNumMergeCand;
    uint32_t limitReferences;
    uint32_t limitModes;
    int searchMethod;
    int subpelRefine;
    int searchRange;
    int bEnableTemporalMvp;
    int bEnableHME;
    int hmeSearchMethod[3];
    int bEnableWeightedPred;
    int bEnableWeightedBiPred;
    int bSourceReferenceEstimation;
    int bEnableLoopFilter;
    int deblockingFilterTCOffset;
    int deblockingFilterBetaOffset;
    int bEnableSAO;
    int bSaoNonDeblocked;
    int selectiveSAO;
    int rdLevel;
    int bEnableEarlySkip;
    int recursionSkipMode;
    int bEnableFastIntra;
    int bEnableTSkipFast;
    int bCULossless;
    int bIntraInBFrames;
    int rdPenalty;
    double psyRd;
    double psyRdoq;
    double rfConstant;
    double rfConstantMax;
    double rfConstantMin;
    double rfConstantDelta;
    double rfConstantStep;
    int bEnableAdaptiveQuant;
    int rateControlMode;
    int targetBitrate;
    int maxBitrate;
    int minBitrate;
    int bEnable1PassVBR;
    int maxQPDelta;
    int minQPDelta;
    int bEnableVisualQualityAdaptation;
    int bEnableFastFirstPass;
    int bEnableAdaptiveBFramePlacement;
    int bEnableAdaptiveIFramePlacement;
    int bEnableAdaptivePFramePlacement;
    int bEnableAdaptiveQuantizationDistortion;
    int bEnableAdaptiveQuantizationStrength;
    int bEnableAspectRatioSignaling;
    int bEnableBufferingPeriodSEI;
    int bEnableChromaResidualScaling;
    int bEnableConstrainedIntraPrediction;
    int bEnableDeblockFiltering;
    int bEnableDeblocking;
    int bEnableFastIntraDecision;
    int bEnableFastIntra;
    int bEnableFastSkipDetection;
    int bEnableFastTransformSkip;
    int bEnableFastSearchRange;
    int bEnableFastSearch;
    int bEnableFastIntraSearch;
    int bEnableFastInterSearch;
    int bEnableFastInter;
    int bEnableFastInterDecision;
    int bEnableFastInterSkip;
    int bEnableFastInterPrediction;
    int bEnableRdRefine;
    int analysisReuseMode;
    const char* analysisReuseFileName;
    int bLossless;
    int cbQpOffset;
    int crQpOffset;
    int preferredTransferCharacteristics;
    int pictureStructure;
    struct {
        int rateControlMode;
        int qp;
        int bitrate;
        double qCompress;
        double ipFactor;
        double pbFactor;
        double rfConstant;
        int qpStep;
        int aqMode;
        int hevcAq;
        double aqStrength;
        double qpAdaptationRange;
        int vbvMaxBitrate;
        int vbvBufferSize;
        double vbvBufferInit;
        int cuTree;
        double rfConstantMax;
        double rfConstantMin;
        int bStatWrite;
        int bStatRead;
        const char* statFileName;
        double qblur;
        double complexityBlur;
        int bEnableSlowFirstPass;
        int zoneCount;
        x265_zone* zones;
        int zonefileCount;
        const char* lambdaFileName;
        int bStrictCbr;
        uint32_t qgSize;
        int bEnableGrain;
        int qpMax;
        int qpMin;
        int bEnableConstVbv;
    } rc;
    struct {
        int aspectRatioIdc;
        int sarWidth;
        int sarHeight;
        int bEnableOverscanInfoPresentFlag;
        int bEnableOverscanAppropriateFlag;
        int bEnableVideoSignalTypePresentFlag;
        int videoFormat;
        int bEnableVideoFullRangeFlag;
        int bEnableColorDescriptionPresentFlag;
        int colorPrimaries;
        int transferCharacteristics;
        int matrixCoeffs;
        int bEnableChromaLocInfoPresentFlag;
        int chromaSampleLocTypeTopField;
        int chromaSampleLocTypeBottomField;
        int bEnableDefaultDisplayWindowFlag;
        int defDispWinLeftOffset;
        int defDispWinRightOffset;
        int defDispWinTopOffset;
        int defDispWinBottomOffset;
    } vui;
    const char* masteringDisplayColorVolume;
    uint16_t maxCLL;
    uint16_t maxFALL;
    uint16_t minLuma;
    uint16_t maxLuma;
    int log2MaxPocLsb;
    int bEmitVUITimingInfo;
    int bEmitVUIHRDInfo;
    unsigned int maxSlices;
    int bOptQpPPS;
    int bOptRefListLengthPPS;
    int bMultiPassOptRPS;
    double scenecutBias;
    int lookaheadThreads;
    int bOptCUDeltaQP;
    int analysisMultiPassRefine;
    int analysisMultiPassDistortion;
    int bAQMotion;
    int bSsimRd;
    double dynamicRd;
    int bEmitHDRSEI;
    int bHDROpt;
    int analysisReuseLevel;
    int bLimitSAO;
    const char* toneMapFile;
    int bDhdr10opt;
    int bCTUInfo;
    int bUseRcStats;
    int scaleFactor;
    int intraRefine;
    int interRefine;
    int mvRefine;
    uint32_t maxLog2CUSize;
    uint32_t maxCUDepth;
    uint32_t unitSizeDepth;
    uint32_t num4x4Partitions;
    int bUseAnalysisFile;
    FILE* csvfpt;
    int forceFlush;
    int bEnableSplitRdSkip;
    int bDisableLookahead;
    int bLowPassDct;
    double vbvBufferEnd;
    double vbvEndFrameAdjust;
    int bAnalysisType;
    int bCopyPicToFrame;
    int gopLookahead;
    const char* analysisSave;
    const char* analysisLoad;
    int radl;
    double maxAUSizeFactor;
    int bEmitIDRRecoverySEI;
    int bDynamicRefine;
    int bSingleSeiNal;
    int chunkStart;
    int chunkEnd;
    const char* naluFile;
    int dolbyProfile;
    int bEnableHRDConcatFlag;
    int ctuDistortionRefine;
    int bEnableSvtHevc;
    void* svtHevcParam;
    int bEnableFades;
    int bField;
    int bEmitCLL;
    int bEnableFrameDuplication;
    int dupThreshold;
    int sourceBitDepth;
    uint32_t reconfigWindowSize;
    int bResetZoneConfig;
    int bEnableSceneCutAwareQp;
    int scenecutWindow;
    int maxQpDelta;
    double edgeTransitionThreshold;
    int bHistBasedSceneCut;
    int hmeRange[3];
    int bHDR10Opt;
    int bEmitHDR10SEI;
    int analysisSaveReuseLevel;
    int analysisLoadReuseLevel;
    int confWinRightOffset;
    int confWinBottomOffset;
    float edgeVarThreshold;
    int decoderVbvMaxRate;
} x265_param;

x265_param *x265_param_alloc(void);
void x265_param_free(x265_param *);
void x265_param_default(x265_param *param);

#define X265_PARAM_BAD_NAME (-1)
#define X265_PARAM_BAD_VALUE (-2)
int x265_param_parse(x265_param *p, const char *name, const char *value);

x265_zone *x265_zone_alloc(int zoneCount, int isZoneFile);
void x265_zone_free(x265_param *param);

int x265_zone_param_parse(x265_param* p, const char* name, const char* value);

static const char * const x265_profile_names[] = {
    "main", "main10", "mainstillpicture", "msp",
    "main-intra", "main10-intra",
    "main444-8", "main444-intra", "main444-stillpicture",
    "main422-10", "main422-10-intra",
    "main444-10", "main444-10-intra",
    "main12", "main12-intra",
    "main422-12", "main422-12-intra",
    "main444-12", "main444-12-intra",
    "main444-16-intra", "main444-16-stillpicture",
    0
};

int x265_param_apply_profile(x265_param *, const char *profile);

static const char * const x265_preset_names[] = { "ultrafast", "superfast", "veryfast", "faster", "fast", "medium", "slow", "slower", "veryslow", "placebo", 0 };
static const char * const x265_tune_names[] = { "psnr", "ssim", "grain", "zerolatency", "fastdecode", "animation", 0 };

int x265_param_default_preset(x265_param *, const char *preset, const char *tune);

x265_picture *x265_picture_alloc(void);
void x265_picture_free(x265_picture *);

X265_API extern const int x265_max_bit_depth;
X265_API extern const char *x265_version_str;
X265_API extern const char *x265_build_info_str;

void x265_alloc_analysis_data(x265_param *param, x265_analysis_data* analysis);
void x265_free_analysis_data(x265_param *param, x265_analysis_data* analysis);

#define x265_encoder_glue1(x, y) x ## y
#define x265_encoder_glue2(x, y) x265_encoder_glue1(x, y)
#define x265_encoder_open x265_encoder_glue2(x265_encoder_open_, X265_BUILD)

x265_encoder* x265_encoder_open(x265_param *);
void x265_encoder_parameters(x265_encoder *, x265_param *);
int x265_encoder_headers(x265_encoder *, x265_nal **pp_nal, uint32_t *pi_nal);
int x265_encoder_encode(x265_encoder *encoder, x265_nal **pp_nal, uint32_t *pi_nal, x265_picture *pic_in, x265_picture *pic_out);
int x265_encoder_reconfig(x265_encoder *, x265_param *);
int x265_encoder_reconfig_zone(x265_encoder *, x265_zone *);
void x265_encoder_get_stats(x265_encoder *encoder, x265_stats *, uint32_t statsSizeBytes);
void x265_encoder_log(x265_encoder *encoder, int argc, char **argv);
void x265_encoder_close(x265_encoder *);
int x265_encoder_intra_refresh(x265_encoder *);
int x265_encoder_ctu_info(x265_encoder *, int poc, x265_ctu_info_t** ctu);
int x265_get_slicetype_poc_and_scenecut(x265_encoder *encoder, int *slicetype, int *poc, int* sceneCut);
int x265_get_ref_frame_list(x265_encoder *encoder, x265_picyuv**, x265_picyuv**, int, int, int*, int*);
int x265_set_analysis_data(x265_encoder *encoder, x265_analysis_data *analysis_data, int poc, uint32_t cuBytes);
void x265_cleanup(void);
FILE* x265_csvlog_open(const x265_param *);
void x265_csvlog_frame(const x265_param *, const x265_picture *);
void x265_csvlog_encode(const x265_param*, const x265_stats *, int padx, int pady, int argc, char** argv);
void x265_dither_image(x265_picture *, int picWidth, int picHeight, int16_t *errorBuf, int bitDepth);

#if ENABLE_LIBVMAF
double x265_calculate_vmafscore(x265_param*, x265_vmaf_data*);
double x265_calculate_vmaf_framelevelscore(x265_vmaf_framedata*);
void x265_vmaf_encoder_log(x265_encoder *encoder, int argc, char **argv, x265_param*, x265_vmaf_data*);
#endif

#define X265_MAJOR_VERSION 1

typedef struct x265_api {
    int api_major_version;
    int api_build_number;
    int sizeof_param;
    int sizeof_picture;
    int sizeof_analysis_data;
    int sizeof_zone;
    int sizeof_stats;
    int bit_depth;
    const char* version_str;
    const char* build_info_str;
    x265_param* (*param_alloc)(void);
    void (*param_free)(x265_param*);
    void (*param_default)(x265_param*);
    int (*param_parse)(x265_param*, const char*, const char*);
    int (*param_apply_profile)(x265_param*, const char*);
    int (*param_default_preset)(x265_param*, const char*, const char *);
    x265_picture* (*picture_alloc)(void);
    void (*picture_free)(x265_picture*);
    void (*picture_init)(x265_param*, x265_picture*);
    x265_encoder* (*encoder_open)(x265_param*);
    void (*encoder_parameters)(x265_encoder*, x265_param*);
    int (*encoder_reconfig)(x265_encoder*, x265_param*);
    int (*encoder_reconfig_zone)(x265_encoder*, x265_zone*);
    int (*encoder_headers)(x265_encoder*, x265_nal**, uint32_t*);
    int (*encoder_encode)(x265_encoder*, x265_nal**, uint32_t*, x265_picture*, x265_picture*);
    void (*encoder_get_stats)(x265_encoder*, x265_stats*, uint32_t);
    void (*encoder_log)(x265_encoder*, int, char**);
    void (*encoder_close)(x265_encoder*);
    void (*cleanup)(void);
    int sizeof_frame_stats;
int (*encoder_intra_refresh)(x265_encoder*);
int (*encoder_ctu_info)(x265_encoder*, int, x265_ctu_info_t**);
int (*get_slicetype_poc_and_scenecut)(x265_encoder*, int*, int*, int*);
int (*get_ref_frame_list)(x265_encoder*, x265_picyuv**, x265_picyuv**, int, int, int*, int*);
FILE* (*csvlog_open)(const x265_param*);
void (*csvlog_frame)(const x265_param*, const x265_picture*);
void (*csvlog_encode)(const x265_param*, const x265_stats *, int, int, int, char**);
void (*dither_image)(x265_picture*, int, int, int16_t*, int);
int (*set_analysis_data)(x265_encoder *encoder, x265_analysis_data *analysis_data, int poc, uint32_t cuBytes);
#if ENABLE_LIBVMAF
double (*calculate_vmafscore)(x265_param *, x265_vmaf_data *);
double (*calculate_vmaf_framelevelscore)(x265_vmaf_framedata *);
void (*vmaf_encoder_log)(x265_encoder*, int, char**, x265_param *, x265_vmaf_data *);
#endif
int (*zone_param_parse)(x265_param*, const char*, const char*);
} x265_api;

#define x265_api_glue1(x, y) x ## y
#define x265_api_glue2(x, y) x265_api_glue1(x, y)
#define x265_api_get x265_api_glue2(x265_api_get_, X265_BUILD)

const x265_api* x265_api_get(int bitDepth);
const x265_api* x265_api_query(int bitDepth, int apiVersion, int* err);

#define X265_API_QUERY_ERR_NONE 0
#define X265_API_QUERY_ERR_VER_REFUSED 1
#define X265_API_QUERY_ERR_LIB_NOT_FOUND 2
#define X265_API_QUERY_ERR_FUNC_NOT_FOUND 3
#define X265_API_QUERY_ERR_WRONG_BITDEPTH 4

static const char * const x265_api_query_errnames[] = {
    "api queried from libx265",
    "libx265 version is not compatible with this application",
    "unable to bind a libx265 with requested bit depth",
    "unable to bind x265_api_query from libx265",
    "libx265 has an invalid bitdepth"
};

#ifdef __cplusplus
}
#endif

#endif // X265_H
// By GST @Date