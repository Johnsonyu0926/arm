// Filename: faaccfg.h
// Score: 95

#ifndef _FAACCFG_H_
#define _FAACCFG_H_

#define FAAC_CFG_VERSION 105

/* MPEG ID's */
#define MPEG2 1
#define MPEG4 0

/* AAC object types */
#define MAIN 1
#define LOW  2
#define SSR  3
#define LTP  4

/* Input Formats */
#define FAAC_INPUT_NULL    0
#define FAAC_INPUT_16BIT   1
#define FAAC_INPUT_24BIT   2
#define FAAC_INPUT_32BIT   3
#define FAAC_INPUT_FLOAT   4

#define SHORTCTL_NORMAL    0
#define SHORTCTL_NOSHORT   1
#define SHORTCTL_NOLONG    2

enum stream_format {
    RAW_STREAM = 0,
    ADTS_STREAM = 1,
};

enum {JOINT_NONE = 0, JOINT_MS, JOINT_IS};

#pragma pack(push, 1)
typedef struct faacEncConfiguration {
    /* config version */
    int version;

    /* library version */
    char *name;

    /* copyright string */
    char *copyright;

    /* MPEG version, 2 or 4 */
    unsigned int mpegVersion;

    /* AAC object type */
    unsigned int aacObjectType;

    union {
        /* Joint coding mode */
        unsigned int jointmode;
        /* compatibility alias */
        unsigned int allowMidside;
    };

    /* Use one of the channels as LFE channel */
    unsigned int useLfe;

    /* Use Temporal Noise Shaping */
    unsigned int useTns;

    /* bitrate / channel of AAC file */
    unsigned long bitRate;

    /* AAC file frequency bandwidth */
    unsigned int bandWidth;

    /* Quantizer quality */
    unsigned long quantqual;

    /* Bitstream output format (0 = Raw; 1 = ADTS) */
    unsigned int outputFormat;

    /* psychoacoustic model list */
    psymodellist_t *psymodellist;

    /* selected index in psymodellist */
    unsigned int psymodelidx;

    /*
        PCM Sample Input Format
        0   FAAC_INPUT_NULL         invalid, signifies a misconfigured config
        1   FAAC_INPUT_16BIT        native endian 16bit
        2   FAAC_INPUT_24BIT        native endian 24bit in 24 bits     (not implemented)
        3   FAAC_INPUT_32BIT        native endian 24bit in 32 bits     (DEFAULT)
        4   FAAC_INPUT_FLOAT        32bit floating point
    */
    unsigned int inputFormat;

    /* block type enforcing (SHORTCTL_NORMAL/SHORTCTL_NOSHORT/SHORTCTL_NOLONG) */
    int shortctl;

    /*
        Channel Remapping

        Default         0, 1, 2, 3 ... 63  (64 is MAX_CHANNELS in coder.h)

        WAVE 4.0        2, 0, 1, 3
        WAVE 5.0        2, 0, 1, 3, 4
        WAVE 5.1        2, 0, 1, 4, 5, 3
        AIFF 5.1        2, 0, 3, 1, 4, 5 
    */
    int channel_map[64];
    int pnslevel;
} faacEncConfiguration, *faacEncConfigurationPtr;

#pragma pack(pop)

#endif /* _FAACCFG_H_ */

/* warn people for update */
#pragma message("please update faad2 include filename and function names!")

/* Backwards compatible link */
#include "neaacdec.h"

// By GST @Date