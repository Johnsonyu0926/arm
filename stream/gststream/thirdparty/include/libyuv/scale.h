#ifndef INCLUDE_LIBYUV_SCALE_H_
#define INCLUDE_LIBYUV_SCALE_H_

#include "libyuv/basic_types.h"

#ifdef __cplusplus
namespace libyuv {
extern "C" {
#endif

// Supported filtering modes.
typedef enum FilterMode {
  kFilterNone = 0,
  kFilterLinear = 1,
  kFilterBilinear = 2,
  kFilterBox = 3
} FilterModeEnum;

// Function declarations for scaling YUV planes.
LIBYUV_API void ScalePlane(const uint8_t* src, int src_stride, int src_width, int src_height, uint8_t* dst, int dst_stride, int dst_width, int dst_height, enum FilterMode filtering);
LIBYUV_API void ScalePlane_16(const uint16_t* src, int src_stride, int src_width, int src_height, uint16_t* dst, int dst_stride, int dst_width, int dst_height, enum FilterMode filtering);

// Function declarations for scaling YUV 4:2:0 images.
LIBYUV_API int I420Scale(const uint8_t* src_y, int src_stride_y, const uint8_t* src_u, int src_stride_u, const uint8_t* src_v, int src_stride_v, int src_width, int src_height, uint8_t* dst_y, int dst_stride_y, uint8_t* dst_u, int dst_stride_u, uint8_t* dst_v, int dst_stride_v, int dst_width, int dst_height, enum FilterMode filtering);
LIBYUV_API int I420Scale_16(const uint16_t* src_y, int src_stride_y, const uint16_t* src_u, int src_stride_u, const uint16_t* src_v, int src_stride_v, int src_width, int src_height, uint16_t* dst_y, int dst_stride_y, uint16_t* dst_u, int dst_stride_u, uint16_t* dst_v, int dst_stride_v, int dst_width, int dst_height, enum FilterMode filtering);

// Function declarations for scaling YUV 4:4:4 images.
LIBYUV_API int I444Scale(const uint8_t* src_y, int src_stride_y, const uint8_t* src_u, int src_stride_u, const uint8_t* src_v, int src_stride_v, int src_width, int src_height, uint8_t* dst_y, int dst_stride_y, uint8_t* dst_u, int dst_stride_u, uint8_t* dst_v, int dst_stride_v, int dst_width, int dst_height, enum FilterMode filtering);
LIBYUV_API int I444Scale_16(const uint16_t* src_y, int src_stride_y, const uint16_t* src_u, int src_stride_u, const uint16_t* src_v, int src_stride_v, int src_width, int src_height, uint16_t* dst_y, int dst_stride_y, uint16_t* dst_u, int dst_stride_u, uint16_t* dst_v, int dst_stride_v, int dst_width, int dst_height, enum FilterMode filtering);

// Function declarations for scaling NV12 images.
LIBYUV_API int NV12Scale(const uint8_t* src_y, int src_stride_y, const uint8_t* src_uv, int src_stride_uv, int src_width, int src_height, uint8_t* dst_y, int dst_stride_y, uint8_t* dst_uv, int dst_stride_uv, int dst_width, int dst_height, enum FilterMode filtering);

#ifdef __cplusplus
// Legacy API. Deprecated.
LIBYUV_API int Scale(const uint8_t* src_y, const uint8_t* src_u, const uint8_t* src_v, int src_stride_y, int src_stride_u, int src_stride_v, int src_width, int src_height, uint8_t* dst_y, uint8_t* dst_u, uint8_t* dst_v, int dst_stride_y, int dst_stride_u, int dst_stride_v, int dst_width, int dst_height, LIBYUV_BOOL interpolate);

// For testing, allow disabling of specialized scalers.
LIBYUV_API void SetUseReferenceImpl(LIBYUV_BOOL use);
#endif  // __cplusplus

#ifdef __cplusplus
}  // extern "C"
}  // namespace libyuv
#endif

#endif  // INCLUDE_LIBYUV_SCALE_H_