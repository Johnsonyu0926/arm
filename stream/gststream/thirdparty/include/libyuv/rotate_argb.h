#ifndef INCLUDE_LIBYUV_ROTATE_ARGB_H_
#define INCLUDE_LIBYUV_ROTATE_ARGB_H_

#include "libyuv/basic_types.h"
#include "libyuv/rotate.h"  // For RotationMode.

#ifdef __cplusplus
namespace libyuv {
extern "C" {
#endif

// Function declaration for rotating ARGB frames.
LIBYUV_API int ARGBRotate(const uint8_t* src_argb, int src_stride_argb, uint8_t* dst_argb, int dst_stride_argb, int src_width, int src_height, enum RotationMode mode);

#ifdef __cplusplus
}  // extern "C"
}  // namespace libyuv
#endif

#endif  // INCLUDE_LIBYUV_ROTATE_ARGB_H_