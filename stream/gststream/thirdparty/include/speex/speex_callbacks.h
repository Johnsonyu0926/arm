

#ifndef SPEEX_CALLBACKS_H
#define SPEEX_CALLBACKS_H
/** @defgroup SpeexCallbacks Various definitions for Speex callbacks supported by the decoder.
 *  @{
 */

#include "speex.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Total number of callbacks */
#define SPEEX_MAX_CALLBACKS 16

/* Describes all the in-band requests */

/*These are 1-bit requests*/
/** Request for perceptual enhancement (1 for on, 0 for off) */
#define SPEEX_INBAND_ENH_REQUEST         0
/** Reserved */
#define SPEEX_INBAND_RESERVED1           1

/*These are 4-bit requests*/
/** Request for a mode change */
#define SPEEX_INBAND_MODE_REQUEST        2
/** Request for a low mode change */
#define SPEEX_INBAND_LOW_MODE_REQUEST    3
/** Request for a high mode change */
#define SPEEX_INBAND_HIGH_MODE_REQUEST   4
/** Request for VBR (1 on, 0 off) */
#define SPEEX_INBAND_VBR_QUALITY_REQUEST 5
/** Request to be sent acknowledge */
#define SPEEX_INBAND_ACKNOWLEDGE_REQUEST 6
/** Request for VBR (1 for on, 0 for off) */
#define SPEEX_INBAND_VBR_REQUEST         7

/*These are 8-bit requests*/
/** Send a character in-band */
#define SPEEX_INBAND_CHAR                8
/** Intensity stereo information */
#define SPEEX_INBAND_STEREO              9

/*These are 16-bit requests*/
/** Transmit max bit-rate allowed */
#define SPEEX_INBAND_MAX_BITRATE         10

/*These are 32-bit requests*/
/** Acknowledge packet reception */
#define SPEEX_INBAND_ACKNOWLEDGE         12

/** Callback function type */
typedef int (*speex_callback_func)(SpeexBits *bits, void *state, void *data);

/** Callback information */
typedef struct SpeexCallback {
   int callback_id;             /**< ID associated to the callback */
   speex_callback_func func;    /**< Callback handler function */
   void *data;                  /**< Data that will be sent to the handler */
   void *reserved1;             /**< Reserved for future use */
   int   reserved2;             /**< Reserved for future use */
} SpeexCallback;

/** Handle in-band request */
int speex_inband_handler(SpeexBits *bits, SpeexCallback *callback_list, void *state);

/** Standard handler for mode request (change mode, no questions asked) */
int speex_std_mode_request_handler(SpeexBits *bits, void *state, void *data);

/** Standard handler for high mode request (change high mode, no questions asked) */
int speex_std_high_mode_request_handler(SpeexBits *bits, void *state, void *data);

/** Standard handler for in-band characters (write to stderr) */
int speex_std_char_handler(SpeexBits *bits, void *state, void *data);

/** Default handler for user-defined requests: in this case, just ignore */
int speex_default_user_handler(SpeexBits *bits, void *state, void *data);



/** Standard handler for low mode request (change low mode, no questions asked) */
int speex_std_low_mode_request_handler(SpeexBits *bits, void *state, void *data);

/** Standard handler for VBR request (Set VBR, no questions asked) */
int speex_std_vbr_request_handler(SpeexBits *bits, void *state, void *data);

/** Standard handler for enhancer request (Turn enhancer on/off, no questions asked) */
int speex_std_enh_request_handler(SpeexBits *bits, void *state, void *data);

/** Standard handler for VBR quality request (Set VBR quality, no questions asked) */
int speex_std_vbr_quality_request_handler(SpeexBits *bits, void *state, void *data);


#ifdef __cplusplus
}
#endif

/** @} */
#endif
//@GST