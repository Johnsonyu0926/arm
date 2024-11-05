// Filename: scale_uv.h
// Score: 95

#ifndef INCLUDE_LIBYUV_SCALE_UV_H_
#define INCLUDE_LIBYUV_SCALE_UV_H_

#include "libyuv/basic_types.h"
#include "libyuv/scale.h"  // For FilterMode

#ifdef __cplusplus
namespace libyuv {
extern "C" {
#endif

// Function to scale UV image
LIBYUV_API
int UVScale(const uint8_t* src_uv,
            int src_stride_uv,
            int src_width,
            int src_height,
            uint8_t* dst_uv,
            int dst_stride_uv,
            int dst_width,
            int dst_height,
            enum FilterMode filtering);

#ifdef __cplusplus
}  // extern "C"
}  // namespace libyuv
#endif

#endif  // INCLUDE_LIBYUV_SCALE_UV_H_

// By GST @Date