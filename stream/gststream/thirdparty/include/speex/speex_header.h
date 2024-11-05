
#ifndef SPEEX_HEADER_H
#define SPEEX_HEADER_H
/** @defgroup SpeexHeader SpeexHeader: Makes it easy to write/parse an Ogg/Speex header
 *  This is the Speex header for the Ogg encapsulation. You don't need that if you just use RTP.
 *  @{
 */

#include "speex_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct SpeexMode;

/** Length of the Speex header identifier */
#define SPEEX_HEADER_STRING_LENGTH 8

/** Maximum number of characters for encoding the Speex version number in the header */
#define SPEEX_HEADER_VERSION_LENGTH 20

/** Speex header info for file-based formats */
typedef struct SpeexHeader {
   char speex_string[SPEEX_HEADER_STRING_LENGTH];   /**< Identifies a Speex bit-stream, always set to "Speex   " */
   char speex_version[SPEEX_HEADER_VERSION_LENGTH]; /**< Speex version */
   spx_int32_t speex_version_id;       /**< Version for Speex (for checking compatibility) */
   spx_int32_t header_size;            /**< Total size of the header ( sizeof(SpeexHeader) ) */
   spx_int32_t rate;                   /**< Sampling rate used */
   spx_int32_t mode;                   /**< Mode used (0 for narrowband, 1 for wideband) */
   spx_int32_t mode_bitstream_version; /**< Version ID of the bit-stream */
   spx_int32_t nb_channels;            /**< Number of channels encoded */
   spx_int32_t bitrate;                /**< Bit-rate used */
   spx_int32_t frame_size;             /**< Size of frames */
   spx_int32_t vbr;                    /**< 1 for a VBR encoding, 0 otherwise */
   spx_int32_t frames_per_packet;      /**< Number of frames stored per Ogg packet */
   spx_int32_t extra_headers;          /**< Number of additional headers after the comments */
   spx_int32_t reserved1;              /**< Reserved for future use, must be zero */
   spx_int32_t reserved2;              /**< Reserved for future use, must be zero */
} SpeexHeader;

/** Initializes a SpeexHeader using basic information */
void speex_init_header(SpeexHeader *header, int rate, int nb_channels, const struct SpeexMode *m);

/** Creates the header packet from the header itself (mostly involves endianness conversion) */
char *speex_header_to_packet(SpeexHeader *header, int *size);

/** Creates a SpeexHeader from a packet */
SpeexHeader *speex_packet_to_header(char *packet, int size);

/** Frees the memory allocated by either speex_header_to_packet() or speex_packet_to_header() */
void speex_header_free(void *ptr);

#ifdef __cplusplus
}
#endif

/** @} */
#endif
//GST
