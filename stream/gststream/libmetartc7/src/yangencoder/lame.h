// Filename: LAME_LAME.h
// Score: 97

#ifndef LAME_LAME_H
#define LAME_LAME_H

#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef void (*lame_report_function)(const char *format, va_list ap);

#if defined(WIN32) || defined(_WIN32)
#undef CDECL
#define CDECL __cdecl
#else
#define CDECL
#endif

#define DEPRECATED_OR_OBSOLETE_CODE_REMOVED 1

typedef enum vbr_mode_e {
    vbr_off = 0,
    vbr_mt,
    vbr_rh,
    vbr_abr,
    vbr_mtrh,
    vbr_max_indicator,
    vbr_default = vbr_mtrh
} vbr_mode;

typedef enum MPEG_mode_e {
    STEREO = 0,
    JOINT_STEREO,
    DUAL_CHANNEL,
    MONO,
    NOT_SET,
    MAX_INDICATOR
} MPEG_mode;

typedef enum Padding_type_e {
    PAD_NO = 0,
    PAD_ALL,
    PAD_ADJUST,
    PAD_MAX_INDICATOR
} Padding_type;

typedef enum preset_mode_e {
    ABR_8 = 8,
    ABR_320 = 320,
    V9 = 410,
    VBR_10 = 410,
    V8 = 420,
    VBR_20 = 420,
    V7 = 430,
    VBR_30 = 430,
    V6 = 440,
    VBR_40 = 440,
    V5 = 450,
    VBR_50 = 450,
    V4 = 460,
    VBR_60 = 460,
    V3 = 470,
    VBR_70 = 470,
    V2 = 480,
    VBR_80 = 480,
    V1 = 490,
    VBR_90 = 490,
    V0 = 500,
    VBR_100 = 500,
    R3MIX = 1000,
    STANDARD = 1001,
    EXTREME = 1002,
    INSANE = 1003,
    STANDARD_FAST = 1004,
    EXTREME_FAST = 1005,
    MEDIUM = 1006,
    MEDIUM_FAST = 1007
} preset_mode;

typedef enum asm_optimizations_e {
    MMX = 1,
    AMD_3DNOW = 2,
    SSE = 3
} asm_optimizations;

typedef enum Psy_model_e {
    PSY_GPSYCHO = 1,
    PSY_NSPSYTUNE = 2
} Psy_model;

typedef enum buffer_constraint_e {
    MDB_DEFAULT = 0,
    MDB_STRICT_ISO = 1,
    MDB_MAXIMUM = 2
} buffer_constraint;

struct lame_global_struct;
typedef struct lame_global_struct lame_global_flags;
typedef lame_global_flags *lame_t;

lame_global_flags *CDECL lame_init(void);

#if DEPRECATED_OR_OBSOLETE_CODE_REMOVED
#else
int32_t CDECL lame_init_old(lame_global_flags *);
#endif

int32_t CDECL lame_set_num_samples(lame_global_flags *, unsigned long);
unsigned long CDECL lame_get_num_samples(const lame_global_flags *);

int32_t CDECL lame_set_in_samplerate(lame_global_flags *, int);
int32_t CDECL lame_get_in_samplerate(const lame_global_flags *);

int32_t CDECL lame_set_num_channels(lame_global_flags *, int);
int32_t CDECL lame_get_num_channels(const lame_global_flags *);

int32_t CDECL lame_set_scale(lame_global_flags *, float);
float CDECL lame_get_scale(const lame_global_flags *);

int32_t CDECL lame_set_scale_left(lame_global_flags *, float);
float CDECL lame_get_scale_left(const lame_global_flags *);

int32_t CDECL lame_set_scale_right(lame_global_flags *, float);
float CDECL lame_get_scale_right(const lame_global_flags *);

int32_t CDECL lame_set_out_samplerate(lame_global_flags *, int);
int32_t CDECL lame_get_out_samplerate(const lame_global_flags *);

int32_t CDECL lame_set_analysis(lame_global_flags *, int);
int32_t CDECL lame_get_analysis(const lame_global_flags *);

int32_t CDECL lame_set_bWriteVbrTag(lame_global_flags *, int);
int32_t CDECL lame_get_bWriteVbrTag(const lame_global_flags *);

int32_t CDECL lame_set_decode_only(lame_global_flags *, int);
int32_t CDECL lame_get_decode_only(const lame_global_flags *);

#if DEPRECATED_OR_OBSOLETE_CODE_REMOVED
#else
int32_t CDECL lame_set_ogg(lame_global_flags *, int);
int32_t CDECL lame_get_ogg(const lame_global_flags *);
#endif

int32_t CDECL lame_set_quality(lame_global_flags *, int);
int32_t CDECL lame_get_quality(const lame_global_flags *);

int32_t CDECL lame_set_mode(lame_global_flags *, MPEG_mode);
MPEG_mode CDECL lame_get_mode(const lame_global_flags *);

#if DEPRECATED_OR_OBSOLETE_CODE_REMOVED
#else
int32_t CDECL lame_set_mode_automs(lame_global_flags *, int);
int32_t CDECL lame_get_mode_automs(const lame_global_flags *);
#endif

int32_t CDECL lame_set_force_ms(lame_global_flags *, int);
int32_t CDECL lame_get_force_ms(const lame_global_flags *);

int32_t CDECL lame_set_free_format(lame_global_flags *, int);
int32_t CDECL lame_get_free_format(const lame_global_flags *);

int32_t CDECL lame_set_findReplayGain(lame_global_flags *, int);
int32_t CDECL lame_get_findReplayGain(const lame_global_flags *);

int32_t CDECL lame_set_decode_on_the_fly(lame_global_flags *, int);
int32_t CDECL lame_get_decode_on_the_fly(const lame_global_flags *);

#if DEPRECATED_OR_OBSOLETE_CODE_REMOVED
#else
int32_t CDECL lame_set_ReplayGain_input(lame_global_flags *, int);
int32_t CDECL lame_get_ReplayGain_input(const lame_global_flags *);

int32_t CDECL lame_set_ReplayGain_decode(lame_global_flags *, int);
int32_t CDECL lame_get_ReplayGain_decode(const lame_global_flags *);

int32_t CDECL lame_set_findPeakSample(lame_global_flags *, int);
int32_t CDECL lame_get_findPeakSample(const lame_global_flags *);
#endif

int32_t CDECL lame_set_nogap_total(lame_global_flags *, int);
int32_t CDECL lame_get_nogap_total(const lame_global_flags *);

int32_t CDECL lame_set_nogap_currentindex(lame_global_flags *, int);
int32_t CDECL lame_get_nogap_currentindex(const lame_global_flags *);

int32_t CDECL lame_set_errorf(lame_global_flags *, lame_report_function);
int32_t CDECL lame_set_debugf(lame_global_flags *, lame_report_function);
int32_t CDECL lame_set_msgf(lame_global_flags *, lame_report_function);

int32_t CDECL lame_set_brate(lame_global_flags *, int);
int32_t CDECL lame_get_brate(const lame_global_flags *);
int32_t CDECL lame_set_compression_ratio(lame_global_flags *, float);
float CDECL lame_get_compression_ratio(const lame_global_flags *);

int32_t CDECL lame_set_preset(lame_global_flags *, int32_t);
int32_t CDECL lame_set_asm_optimizations(lame_global_flags *, int, int32_t);

int32_t CDECL lame_set_copyright(lame_global_flags *, int);
int32_t CDECL lame_get_copyright(const lame_global_flags *);

int32_t CDECL lame_set_original(lame_global_flags *, int);
int32_t CDECL lame_get_original(const lame_global_flags *);

int32_t CDECL lame_set_error_protection(lame_global_flags *, int);
int32_t CDECL lame_get_error_protection(const lame_global_flags *);

#if DEPRECATED_OR_OBSOLETE_CODE_REMOVED
#else
int32_t CDECL lame_set_padding_type(lame_global_flags *, Padding_type);
Padding_type CDECL lame_get_padding_type(const lame_global_flags *);
#endif

int32_t CDECL lame_set_extension(lame_global_flags *, int);
int32_t CDECL lame_get_extension(const lame_global_flags *);

int32_t CDECL lame_set_strict_ISO(lame_global_flags *, int);
int32_t CDECL lame_get_strict_ISO(const lame_global_flags *);

int32_t CDECL lame_set_disable_reservoir(lame_global_flags *, int);
int32_t CDECL lame_get_disable_reservoir(const lame_global_flags *);

int32_t CDECL lame_set_quant_comp(lame_global_flags *, int);
int32_t CDECL lame_get_quant_comp(const lame_global_flags *);
int32_t CDECL lame_set_quant_comp_short(lame_global_flags *, int);
int32_t CDECL lame_get_quant_comp_short(const lame_global_flags *);

int32_t CDECL lame_set_experimentalX(lame_global_flags *, int);
int32_t CDECL lame_get_experimentalX(const lame_global_flags *);

int32_t CDECL lame_set_experimentalY(lame_global_flags *, int);
int32_t CDECL lame_get_experimentalY(const lame_global_flags *);

int32_t CDECL lame_set_experimentalZ(lame_global_flags *, int);
int32_t CDECL lame_get_experimentalZ(const lame_global_flags *);

int32_t CDECL lame_set_exp_nspsytune(lame_global_flags *, int);
int32_t CDECL lame_get_exp_nspsytune(const lame_global_flags *);

void CDECL lame_set_msfix(lame_global_flags *, double);
float CDECL lame_get_msfix(const lame_global_flags *);

int32_t CDECL lame_set_VBR(lame_global_flags *, vbr_mode);
vbr_mode CDECL lame_get_VBR(const lame_global_flags *);

int32_t CDECL lame_set_VBR_q(lame_global_flags *, int);
int32_t CDECL lame_get_VBR_q(const lame_global_flags *);

int32_t CDECL lame_set_VBR_quality(lame_global_flags *, float);
float CDECL lame_get_VBR_quality(const lame_global_flags *);

int32_t CDECL lame_set_VBR_mean_bitrate_kbps(lame_global_flags *, int);
int32_t CDECL lame_get_VBR_mean_bitrate_kbps(const lame_global_flags *);

int32_t CDECL lame_set_VBR_min_bitrate_kbps(lame_global_flags *, int);
int32_t CDECL lame_get_VBR_min_bitrate_kbps(const lame_global_flags *);

int32_t CDECL lame_set_VBR_max_bitrate_kbps(lame_global_flags *, int);
int32_t CDECL lame_get_VBR_max_bitrate_kbps(const lame_global_flags *);

int32_t CDECL lame_set_VBR_hard_min(lame_global_flags *, int);
int32_t CDECL lame_get_VBR_hard_min(const lame_global_flags *);

#if DEPRECATED_OR_OBSOLETE_CODE_REMOVED
#else
int32_t CDECL lame_set_preset_expopts(lame_global_flags *, int);
#endif

int32_t CDECL lame_set_lowpassfreq(lame_global_flags *, int);
int32_t CDECL lame_get_lowpassfreq(const lame_global_flags *);
int32_t CDECL lame_set_lowpasswidth(lame_global_flags *, int);
int32_t CDECL lame_get_lowpasswidth(const lame_global_flags *);

int32_t CDECL lame_set_highpassfreq(lame_global_flags *, int);
int32_t CDECL lame_get_highpassfreq(const lame_global_flags *);
int32_t CDECL lame_set_highpasswidth(lame_global_flags *, int);
int32_t CDECL lame_get_highpasswidth(const lame_global_flags *);

int32_t CDECL lame_set_ATHonly(lame_global_flags *, int);
int32_t CDECL lame_get_ATHonly(const lame_global_flags *);

int32_t CDECL lame_set_ATHshort(lame_global_flags *, int);
int32_t CDECL lame_get_ATHshort(const lame_global_flags *);

int32_t CDECL lame_set_noATH(lame_global_flags *, int);
int32_t CDECL lame_get_noATH(const lame_global_flags *);

int32_t CDECL lame_set_ATHtype(lame_global_flags *, int);
int32_t CDECL lame_get_ATHtype(const lame_global_flags *);

int32_t CDECL lame_set_ATHlower(lame_global_flags *, float);
float CDECL lame_get_ATHlower(const lame_global_flags *);

int32_t CDECL lame_set_athaa_type(lame_global_flags *, int);
int32_t CDECL lame_get_athaa_type(const lame_global_flags *);

#if DEPRECATED_OR_OBSOLETE_CODE_REMOVED
#else
int32_t CDECL lame_set_athaa_loudapprox(lame_global_flags *, int);
int32_t CDECL lame_get_athaa_loudapprox(const lame_global_flags *);
#endif

int32_t CDECL lame_set_athaa_sensitivity(lame_global_flags *, float);
float CDECL lame_get_athaa_sensitivity(const lame_global_flags *);

#if DEPRECATED_OR_OBSOLETE_CODE_REMOVED
#else
int32_t CDECL lame_set_cwlimit(lame_global_flags *, int);
int32_t CDECL lame_get_cwlimit(const lame_global_flags *);
#endif

int32_t CDECL lame_set_allow_diff_short(lame_global_flags *, int);
int32_t CDECL lame_get_allow_diff_short(const lame_global_flags *);

int32_t CDECL lame_set_useTemporal(lame_global_flags *, int);
int32_t CDECL lame_get_useTemporal(const lame_global_flags *);

int32_t CDECL lame_set_interChRatio(lame_global_flags *, float);
float CDECL lame_get_interChRatio(const lame_global_flags *);

int32_t CDECL lame_set_no_short_blocks(lame_global_flags *, int);
int32_t CDECL lame_get_no_short_blocks(const lame_global_flags *);

int32_t CDECL lame_set_force_short_blocks(lame_global_flags *, int);
int32_t CDECL lame_get_force_short_blocks(const lame_global_flags *);

int32_t CDECL lame_set_emphasis(lame_global_flags *, int);
int32_t CDECL lame_get_emphasis(const lame_global_flags *);

int32_t CDECL lame_get_version(const lame_global_flags *);
int32_t CDECL lame_get_encoder_delay(const lame_global_flags *);
int32_t CDECL lame_get_encoder_padding(const lame_global_flags *);
int32_t CDECL lame_get_framesize(const lame_global_flags *);
int32_t CDECL lame_get_mf_samples_to_encode(const lame_global_flags *);

int32_t CDECL lame_get_size_mp3buffer(const lame_global_flags *);
int32_t CDECL lame_get_frameNum(const lame_global_flags *);
int32_t CDECL lame_get_totalframes(const lame_global_flags *);
int32_t CDECL lame_get_RadioGain(const lame_global_flags *);
int32_t CDECL lame_get_AudiophileGain(const lame_global_flags *);
float CDECL lame_get_PeakSample(const lame_global_flags *);
int32_t CDECL lame_get_noclipGainChange(const lame_global_flags *);
float CDECL lame_get_noclipScale(const lame_global_flags *);
int32_t CDECL lame_get_maximum_number_of_samples(lame_t gfp, size_t buffer_size);

int32_t CDECL lame_init_params(lame_global_flags *);

const char* CDECL get_lame_version(void);
const char* CDECL get_lame_short_version(void);
const char* CDECL get_lame_very_short_version(void);
const char* CDECL get_psy_version(void);
const char* CDECL get_lame_url(void);
const char* CDECL get_lame_os_bitness(void);

typedef struct {
    int32_t major;
    int32_t minor;
    int32_t alpha;
    int32_t beta;
    int32_t psy_major;
    int32_t psy_minor;
    int32_t psy_alpha;
    int32_t psy_beta;
    const char *features;
} lame_version_t;

void CDECL get_lame_version_numerical(lame_version_t *);

void CDECL lame_print_config(const lame_global_flags *);
void CDECL lame_print_internals(const lame_global_flags *);

int32_t CDECL lame_encode_buffer(lame_global_flags *, const int16_t [], const int16_t [], const int32_t, uint8_t *, const int32_t);
int32_t CDECL lame_encode_buffer_interleaved(lame_global_flags *, int16_t [], int32_t, uint8_t *, int32_t);
int32_t CDECL lame_encode_buffer_float(lame_global_flags *, const float [], const float [], const int32_t, uint8_t *, const int32_t);
int32_t CDECL lame_encode_buffer_ieee_float(lame_t, const float [], const float [], const int32_t, uint8_t *, const int32_t);
int32_t CDECL lame_encode_buffer_interleaved_ieee_float(lame_t, const float [], const int32_t, uint8_t *, const int32_t);
int32_t CDECL lame_encode_buffer_ieee_double(lame_t, const double [], const double [], const int32_t, uint8_t *, const int32_t);
int32_t CDECL lame_encode_buffer_interleaved_ieee_double(lame_t, const double [], const int32_t, uint8_t *, const int32_t);
int32_t CDECL lame_encode_buffer_long(lame_global_flags *, const long [], const long [], const int32_t, uint8_t *, const int32_t);
int32_t CDECL lame_encode_buffer_long2(lame_global_flags *, const long [], const long [], const int32_t, uint8_t *, const int32_t);
int32_t CDECL lame_encode_buffer_int(lame_global_flags *, const int32_t [], const int32_t [], const int32_t, uint8_t *, const int32_t);
int32_t CDECL lame_encode_buffer_interleaved_int(lame_t, const int32_t [], const int32_t, uint8_t *, const int32_t);

int32_t CDECL lame_encode_flush(lame_global_flags *, uint8_t *, int32_t);
int32_t CDECL lame_encode_flush_nogap(lame_global_flags *, uint8_t *, int32_t);
int32_t CDECL lame_init_bitstream(lame_global_flags *);

void CDECL lame_bitrate_hist(const lame_global_flags *, int32_t [14]);
void CDECL lame_bitrate_kbps(const lame_global_flags *, int32_t [14]);
void CDECL lame_stereo_mode_hist(const lame_global_flags *, int32_t [4]);
void CDECL lame_bitrate_stereo_mode_hist(const lame_global_flags *, int32_t [14][4]);
void CDECL lame_block_type_hist(const lame_global_flags *, int32_t [6]);
void CDECL lame_bitrate_block_type_hist(const lame_global_flags *, int32_t [14][6]);

#if (DEPRECATED_OR_OBSOLETE_CODE_REMOVED && 0)
#else
void CDECL lame_mp3_tags_fid(lame_global_flags *, FILE *);
#endif

size_t CDECL lame_get_lametag_frame(const lame_global_flags *, uint8_t *, size_t);
int32_t CDECL lame_close(lame_global_flags *);

#if DEPRECATED_OR_OBSOLETE_CODE_REMOVED
#else
int32_t CDECL lame_encode_finish(lame_global_flags *, uint8_t *, int32_t);
#endif

struct hip_global_struct;
typedef struct hip_global_struct hip_global_flags;
typedef hip_global_flags *hip_t;

typedef struct {
    int32_t header_parsed;
    int32_t stereo;
    int32_t samplerate；
    int32_t bitrate;
} mp3data_struct;

hip_t CDECL hip_decode_init(void);
int32_t CDECL hip_decode_exit(hip_t gfp);
void CDECL hip_set_errorf(hip_t gfp, lame_report_function f);
void CDECL hip_set_debugf(hip_t gfp, lame_report_function f);
void CDECL hip_set_msgf(hip_t gfp, lame_report_function f);

int32_t CDECL hip_decode(hip_t gfp, uint8_t *mp3buf, size_t len, short pcm_l[], short pcm_r[]);
int32_t CDECL hip_decode_headers(hip_t gfp, uint8_t *mp3buf, size_t len, short pcm_l[], short pcm_r[], mp3data_struct *mp3data);
int32_t CDECL hip_decode1(hip_t gfp, uint8_t *mp3buf, size_t len, short pcm_l[], short pcm_r[]);
int32_t CDECL hip_decode1_headers(hip_t gfp, uint8_t *mp3buf, size_t len, short pcm_l[], short pcm_r[], mp3data_struct *mp3data);
int32_t CDECL hip_decode1_headersB(hip_t gfp, uint8_t *mp3buf, size_t len, short pcm_l[], short pcm_r[], mp3data_struct *mp3data, int32_t *enc_delay, int32_t *enc_padding);

#if DEPRECATED_OR_OBSOLETE_CODE_REMOVED
#else
int32_t CDECL lame_decode_init(void);
int32_t CDECL lame_decode(uint8_t *mp3buf, int32_t len, short pcm_l[], short pcm_r[]);
int32_t CDECL lame_decode_headers(uint8_t *mp3buf, int32_t len, short pcm_l[], short pcm_r[], mp3data_struct *mp3data);
int32_t CDECL lame_decode1(uint8_t *mp3buf, int32_t len, short pcm_l[], short pcm_r[]);
int32_t CDECL lame_decode1_headers(uint8_t *mp3buf, int32_t len, short pcm_l[], short pcm_r[], mp3data_struct *mp3data);
int32_t CDECL lame_decode1_headersB(uint8_t *mp3buf, int32_t len, short pcm_l[], short pcm_r[], mp3data_struct *mp3data, int32_t *enc_delay, int32_t *enc_padding);
int32_t CDECL lame_decode_exit(void);
#endif

void CDECL id3tag_genre_list(void (*handler)(int, const char *, void *), void *cookie);
void CDECL id3tag_init(lame_t gfp);
void CDECL id3tag_add_v2(lame_t gfp);
void CDECL id3tag_v1_only(lame_t gfp);
void CDECL id3tag_v2_only(lame_t gfp);
void CDECL id3tag_space_v1(lame_t gfp);
void CDECL id3tag_pad_v2(lame_t gfp);
void CDECL id3tag_set_pad(lame_t gfp, size_t n);
void CDECL id3tag_set_title(lame_t gfp, const char *title);
void CDECL id3tag_set_artist(lame_t gfp, const char *artist);
void CDECL id3tag_set_album(lame_t gfp, const char *album);
void CDECL id3tag_set_year(lame_t gfp, const char *year);
void CDECL id3tag_set_comment(lame_t gfp, const char *comment);
int32_t CDECL id3tag_set_track(lame_t gfp, const char *track);
int32_t CDECL id3tag_set_genre(lame_t gfp, const char *genre);
int32_t CDECL id3tag_set_fieldvalue(lame_t gfp, const char *fieldvalue);
int32_t CDECL id3tag_set_albumart(lame_t gfp, const char *image, size_t size);
size_t CDECL lame_get_id3v1_tag(lame_t gfp, uint8_t *buffer, size_t size);
size_t CDECL lame_get_id3v2_tag(lame_t gfp, uint8_t *buffer, size_t size);
void CDECL lame_set_write_id3tag_automatic(lame_global_flags *gfp, int);
int32_t CDECL lame_get_write_id3tag_automatic(const lame_global_flags *gfp);
int32_t CDECL id3tag_set_textinfo_latin1(lame_t gfp, const char *id, const char *text);
int32_t CDECL id3tag_set_comment_latin1(lame_t gfp, const char *lang, const char *desc, const char *text);

#if DEPRECATED_OR_OBSOLETE_CODE_REMOVED
#else
int32_t CDECL id3tag_set_textinfo_ucs2(lame_t gfp, const char *id, const unsigned short *text);
int32_t CDECL id3tag_set_comment_ucs2(lame_t gfp, const char *lang, const unsigned short *desc, const unsigned short *text);
int32_t CDECL id3tag_set_fieldvalue_ucs2(lame_t gfp, const unsigned short *fieldvalue);
#endif

int32_t CDECL id3tag_set_fieldvalue_utf16(lame_t gfp, const unsigned short *fieldvalue);
int32_t CDECL id3tag_set_textinfo_utf16(lame_t gfp, const char *id, const unsigned short *text);
int32_t CDECL id3tag_set_comment_utf16(lame_t gfp, const char *lang, const unsigned short *desc, const unsigned short *text);

extern const int32_t bitrate_table[3][16];
extern const int32_t samplerate_table[3][4];

int32_t CDECL lame_get_bitrate(int32_t mpeg_version, int32_t table_index);
int32_t CDECL lame_get_samplerate(int32_t mpeg_version, int32_t table_index);

#define LAME_MAXALBUMART (128 * 1024)
#define LAME_MAXMP3BUFFER (16384 + LAME_MAXALBUMART)

typedef enum {
    LAME_OKAY = 0,
    LAME_NOERROR = 0,
    LAME_GENERICERROR = -1,
    LAME_NOMEM = -10,
    LAME_BADBITRATE = -11,
    LAME_BADSAMPFREQ = -12,
    LAME_INTERNALERROR = -13,
    FRONTEND_READERROR = -80,
    FRONTEND_WRITEERROR = -81,
    FRONTEND_FILETOOLARGE = -82
} lame_errorcodes_t;

#if defined(__cplusplus)
}
#endif

#endif /* LAME_LAME_H */

// By GST @Date