#ifndef _AUDIO_CODEC_COMMON_H_
#define _AUDIO_CODEC_COMMON_H_

#ifdef _cplusplus
extern "C" {
#endif

#include "datatypedef.h"
#include "HikCrypt/mem_tab.h"

#define HIK_AUDIOCODEC_LIB_S_OK 1
#define HIK_AUDIOCODEC_LIB_S_FAIL 0
#define HIK_AUDIOCODEC_LIB_E_PARAM_NULL 0x80000000
#define HIK_AUDIOCODEC_LIB_E_HANDLE_NULL 0x80000001
#define HIK_AUDIOCODEC_LIB_E_MEMORY_NULL 0x80000002
#define HIK_AUDIOCODEC_LIB_E_CHANNEL_ERR 0x80000003
#define HIK_AUDIOCODEC_LIB_E_SAMPLERATE_ERR 0x80000004
#define HIK_AUDIOCODEC_LIB_E_MAXBITS_ERR 0x80000005
#define HIK_AUDIOCODEC_LIB_E_BITSCTRL_ERR 0x80000006
#define HIK_AUDIOCODEC_LIB_E_BITRATE_ERR 0x80000007
#define HIK_AUDIOCODEC_LIB_E_INPUT_SIZE_ERR 0x80000008
#define HIK_AUDIOCODEC_LIB_E_MEMTAB_SIZE_ERR 0x80000009
#define HIK_AUDIOCODEC_LIB_E_BUF_TOO_SMALL 0x8000000a

#define AUDIO_CODEC_BUF_SIZE 8192
#define AUDIO_DEC_DATA_SIZE 2048

typedef enum _BITCTRLLEVEL_ {
    LEVEL1 = 1,
    LEVEL2 = 2,
    LEVEL3 = 3
} BITCTRLLEVEL;

typedef enum _ENC_MODE_ {
    AMR_MR475 = 0,
    AMR_MR515,
    AMR_MR59,
    AMR_MR67,
    AMR_MR74,
    AMR_MR795,
    AMR_MR102,
    AMR_MR122,
    AMR_MRDTX,
    AMR_N_MODES
} AUDIO_ENC_MODE;

#ifndef _HRESULT_DEFINED
#define _HRESULT_DEFINED
    typedef S32 HRESULT;
#endif

typedef struct _AUDIOENCInfo {
    U32 in_frame_size;
    S32 reserved[16];
} AUDIOENC_INFO;

typedef struct _AUDIOENC_PARAM_ {
    U32 sample_rate;
    U32 num_channels;
    U32 bitrate;
    U32 vad_dtx_enable;
    BITCTRLLEVEL bit_ctrl_level;
    S32 reserved[16];
} AUDIOENC_PARAM;

typedef struct _AUDIOENC_PROCESS_PARAM_ {
    U08 *in_buf;
    U08 *out_buf;
    U32 out_frame_size;
    S32 g726enc_reset;
    S32 g711_type;
    AUDIO_ENC_MODE enc_mode;
    S32 reserved[16];
} AUDIOENC_PROCESS_PARAM;

typedef struct _AUDIODECInfo {
    S32 nchans;
    S32 sample_rate;
    S32 aacdec_profile;
    S32 reserved[16];
} AUDIODEC_INFO;

typedef struct _AUDIODEC_PARAM {
    S32 gdec_bitrate;
    S32 reserved[16];
} AUDIODEC_PARAM;

typedef struct _AUDIODEC_PROCESS_PARAM {
    U08 *in_buf;
    U08 *out_buf;
    U32 in_data_size;
    U32 proc_data_size;
    U32 out_frame_size;
    AUDIODEC_INFO dec_info;
    S32 g726dec_reset;
    S32 g711_type;
    S32 reserved[16];
} AUDIODEC_PROCESS_PARAM;

#ifdef _cplusplus
}
#endif

#endif
