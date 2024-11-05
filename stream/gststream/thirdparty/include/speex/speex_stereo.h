#ifndef STEREO_H
#define STEREO_H
/** @defgroup SpeexStereoState SpeexStereoState: Handling Speex stereo files
 *  This describes the Speex intensity stereo encoding/decoding
 *  @{
 */

#include "speex_types.h"
#include "speex_bits.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SpeexStereoState {
   float balance;      /**< Left/right balance info */
   float e_ratio;      /**< Ratio of energies: E(left+right)/[E(left)+E(right)]  */
   float smooth_left;  /**< Smoothed left channel gain */
   float smooth_right; /**< Smoothed right channel gain */
   float reserved1;    /**< Reserved for future use */
   float reserved2;    /**< Reserved for future use */
} SpeexStereoState;

#define SPEEX_STEREO_STATE_INIT {1,.5,1,1,0,0}

SpeexStereoState *speex_stereo_state_init();

void speex_stereo_state_reset(SpeexStereoState *stereo);

void speex_stereo_state_destroy(SpeexStereoState *stereo);

void speex_encode_stereo(float *data, int frame_size, SpeexBits *bits);

void speex_encode_stereo_int(spx_int16_t *data, int frame_size, SpeexBits *bits);

void speex_decode_stereo(float *data, int frame_size, SpeexStereoState *stereo);

void speex_decode_stereo_int(spx_int16_t *data, int frame_size, SpeexStereoState *stereo);

int speex_std_stereo_request_handler(SpeexBits *bits, void *state, void *data);

#ifdef __cplusplus
}
#endif

/** @} */
#endif
// By GST @Date