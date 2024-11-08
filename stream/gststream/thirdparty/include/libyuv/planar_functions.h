#ifndef INCLUDE_LIBYUV_PLANAR_FUNCTIONS_H_
#define INCLUDE_LIBYUV_PLANAR_FUNCTIONS_H_

#include "libyuv/basic_types.h"
#include "libyuv/convert.h"
#include "libyuv/convert_argb.h"

#ifdef __cplusplus
namespace libyuv {
extern "C" {
#endif

#if defined(__pnacl__) || defined(__CLR_VER) || \
    (defined(__native_client__) && defined(__x86_64__)) || \
    (defined(__i386__) && !defined(__SSE__) && !defined(__clang__))
#define LIBYUV_DISABLE_X86
#endif

#if defined(__has_feature)
#if __has_feature(memory_sanitizer)
#define LIBYUV_DISABLE_X86
#endif
#endif

#if !defined(LIBYUV_DISABLE_X86) && \
    (defined(_M_IX86) || defined(__x86_64__) || defined(__i386__))
#define HAS_ARGBAFFINEROW_SSE2
#endif

// Copy a plane of data.
LIBYUV_API
void CopyPlane(const uint8_t* src_y,
               int src_stride_y,
               uint8_t* dst_y,
               int dst_stride_y,
               int width,
               int height);

LIBYUV_API
void CopyPlane_16(const uint16_t* src_y,
                  int src_stride_y,
                  uint16_t* dst_y,
                  int dst_stride_y,
                  int width,
                  int height);

LIBYUV_API
void Convert16To8Plane(const uint16_t* src_y,
                       int src_stride_y,
                       uint8_t* dst_y,
                       int dst_stride_y,
                       int scale,
                       int width,
                       int height);

LIBYUV_API
void Convert8To16Plane(const uint8_t* src_y,
                       int src_stride_y,
                       uint16_t* dst_y,
                       int dst_stride_y,
                       int scale,
                       int width,
                       int height);

// Set a plane of data to a 32 bit value.
LIBYUV_API
void SetPlane(uint8_t* dst_y,
              int dst_stride_y,
              int width,
              int height,
              uint32_t value);

// Split interleaved UV plane into separate U and V planes.
LIBYUV_API
void SplitUVPlane(const uint8_t* src_uv,
                  int src_stride_uv,
                  uint8_t* dst_u,
                  int dst_stride_u,
                  uint8_t* dst_v,
                  int dst_stride_v,
                  int width,
                  int height);

// Merge separate U and V planes into one interleaved UV plane.
LIBYUV_API
void MergeUVPlane(const uint8_t* src_u,
                  int src_stride_u,
                  const uint8_t* src_v,
                  int src_stride_v,
                  uint8_t* dst_uv,
                  int dst_stride_uv,
                  int width,
                  int height);

// Scale U and V to half width and height and merge into interleaved UV plane.
LIBYUV_API
void HalfMergeUVPlane(const uint8_t* src_u,
                      int src_stride_u,
                      const uint8_t* src_v,
                      int src_stride_v,
                      uint8_t* dst_uv,
                      int dst_stride_uv,
                      int width,
                      int height);

// Swap U and V channels in interleaved UV plane.
LIBYUV_API
void SwapUVPlane(const uint8_t* src_uv,
                 int src_stride_uv,
                 uint8_t* dst_vu,
                 int dst_stride_vu,
                 int width,
                 int height);

// Split interleaved RGB plane into separate R, G and B planes.
LIBYUV_API
void SplitRGBPlane(const uint8_t* src_rgb,
                   int src_stride_rgb,
                   uint8_t* dst_r,
                   int dst_stride_r,
                   uint8_t* dst_g,
                   int dst_stride_g,
                   uint8_t* dst_b,
                   int dst_stride_b,
                   int width,
                   int height);

// Merge separate R, G and B planes into one interleaved RGB plane.
LIBYUV_API
void MergeRGBPlane(const uint8_t* src_r,
                   int src_stride_r,
                   const uint8_t* src_g,
                   int src_stride_g,
                   const uint8_t* src_b,
                   int src_stride_b,
                   uint8_t* dst_rgb,
                   int dst_stride_rgb,
                   int width,
                   int height);

// Copy I400. Supports inverting.
LIBYUV_API
int I400ToI400(const uint8_t* src_y,
               int src_stride_y,
               uint8_t* dst_y,
               int dst_stride_y,
               int width,
               int height);

#define J400ToJ400 I400ToI400

// Copy I422 to I422.
#define I422ToI422 I422Copy
LIBYUV_API
int I422Copy(const uint8_t* src_y,
             int src_stride_y,
             const uint8_t* src_u,
             int src_stride_u,
             const uint8_t* src_v,
             int src_stride_v,
             uint8_t* dst_y,
             int dst_stride_y,
             uint8_t* dst_u,
             int dst_stride_u,
             uint8_t* dst_v,
             int dst_stride_v,
             int width,
             int height);

// Copy I444 to I444.
#define I444ToI444 I444Copy
LIBYUV_API
int I444Copy(const uint8_t* src_y,
             int src_stride_y,
             const uint8_t* src_u,
             int src_stride_u,
             const uint8_t* src_v,
             int src_stride_v,
             uint8_t* dst_y,
             int dst_stride_y,
             uint8_t* dst_u,
             int dst_stride_u,
             uint8_t* dst_v,
             int dst_stride_v,
             int width,
             int height);

// Convert YUY2 to I422.
LIBYUV_API
int YUY2ToI422(const uint8_t* src_yuy2,
               int src_stride_yuy2,
               uint8_t* dst_y,
               int dst_stride_y,
               uint8_t* dst_u,
               int dst_stride_u,
               uint8_t* dst_v,
               int dst_stride_v,
               int width,
               int height);

// Convert UYVY to I422.
LIBYUV_API
int UYVYToI422(const uint8_t* src_uyvy,
               int src_stride_uyvy,
               uint8_t* dst_y,
               int dst_stride_y,
               uint8_t* dst_u,
               int dst_stride_u,
               uint8_t* dst_v,
               int dst_stride_v,
               int width,
               int height);

LIBYUV_API
int YUY2ToNV12(const uint8_t* src_yuy2,
               int src_stride_yuy2,
               uint8_t* dst_y,
               int dst_stride_y,
               uint8_t* dst_uv,
               int dst_stride_uv,
               int width,
               int height);

LIBYUV_API
int UYVYToNV12(const uint8_t* src_uyvy,
               int src_stride_uyvy,
               uint8_t* dst_y,
               int dst_stride_y,
               uint8_t* dst_uv,
               int dst_stride_uv,
               int width,
               int height);

// Convert NV21 to NV12.
LIBYUV_API
int NV21ToNV12(const uint8_t* src_y,
               int src_stride_y,
               const uint8_t* src_vu,
               int src_stride_vu,
               uint8_t* dst_y,
               int dst_stride_y,
               uint8_t* dst_uv,
               int dst_stride_uv,
               int width,
               int height);

LIBYUV_API
int YUY2ToY(const uint8_t* src_yuy2,
            int src_stride_yuy2,
            uint8_t* dst_y,
            int dst_stride_y,
            int width,
            int height);

// Convert I420 to I400. (calls CopyPlane ignoring u/v).
LIBYUV_API
int I420ToI400(const uint8_t* src_y,
               int src_stride_y,
               const uint8_t* src_u,
               int src_stride_u,
               const uint8_t* src_v,
               int src_stride_v,
               uint8_t* dst_y,
               int dst_stride_y,
               int width,
               int height);

#define J420ToJ400 I420ToI400
#define I420ToI420Mirror I420Mirror

// I420 mirror.
LIBYUV_API
int I420Mirror(const uint8_t* src_y,
               int src_stride_y,
               const uint8_t* src_u,
               int src_stride_u,
               const uint8_t* src_v,
               int src_stride_v,
               uint8_t* dst_y,
               int dst_stride_y,
               uint8_t* dst_u,
               int dst_stride_u,
               uint8_t* dst_v,
               int dst_stride_v,
               int width,
               int height);

#define I400ToI400Mirror I400Mirror

// I400 mirror. A single plane is mirrored horizontally.
LIBYUV_API
int I400Mirror(const uint8_t* src_y,
               int src_stride_y,
               uint8_t* dst_y,
               int dst_stride_y,
               int width,
               int height);

#define NV12ToNV12Mirror NV12Mirror

// NV12 mirror.
LIBYUV_API
int NV12Mirror(const uint8_t* src_y,
               int src_stride_y,
               const uint8_t* src_uv,
               int src_stride_uv,
               uint8_t* dst_y,
               int dst_stride_y,
               uint8_t* dst_uv,
               int dst_stride_uv,
               int width,
               int height);

#define ARGBToARGBMirror ARGBMirror

// ARGB mirror.
LIBYUV_API
int ARGBMirror(const uint8_t* src_argb,
               int src_stride_argb,
               uint8_t* dst_argb,
               int dst_stride_argb,
               int width,
               int height);

#define RGB24ToRGB24Mirror RGB24Mirror

// RGB24 mirror.
LIBYUV_API
int RGB24Mirror(const uint8_t* src_rgb24,
                int src_stride_rgb24,
                uint8_t* dst_rgb24,
                int dst_stride_rgb24,
                int width,
                int height);

// Mirror a plane of data.
LIBYUV_API
void MirrorPlane(const uint8_t* src_y,
                 int src_stride_y,
                 uint8_t* dst_y,
                 int dst_stride_y,
                 int width,
                 int height);

// Mirror a plane of UV data.
LIBYUV_API
void MirrorUVPlane(const uint8_t* src_uv,
                   int src_stride_uv,
                   uint8_t* dst_uv,
                   int dst_stride_uv,
                   int width,
                   int height);

#define RGB24ToRAW RAWToRGB24

LIBYUV_API
int RAWToRGB24(const uint8_t* src_raw,
               int src_stride_raw,
               uint8_t* dst_rgb24,
               int dst_stride_rgb24,
               int width,
               int height);

// Draw a rectangle into I420.
LIBYUV_API
int I420Rect(uint8_t* dst_y,
             int dst_stride_y,
             uint8_t* dst_u,
             int dst_stride_u,
             uint8_t* dst_v,
             int dst_stride_v,
             int x,
             int y,
             int width,
             int height,
             int value_y,
             int value_u,
             int value_v);

// Draw a rectangle into ARGB.
LIBYUV_API
int ARGBRect(uint8_t* dst_argb,
             int dst_stride_argb,
             int dst_x,
             int dst_y,
             int width,
             int height,
             uint32_t value);

// Convert ARGB to gray scale ARGB.
LIBYUV_API
int ARGBGrayTo(const uint8_t* src_argb,
               int src_stride_argb,
               uint8_t* dst_argb,
               int dst_stride_argb,
               int width,
               int height);

// Make a rectangle of ARGB gray scale.
LIBYUV_API
int ARGBGray(uint8_t* dst_argb,
             int dst_stride_argb,
             int dst_x,
             int dst_y,
             int width,
             int height);

// Make a rectangle of ARGB Sepia tone.
LIBYUV_API
int ARGBSepia(uint8_t* dst_argb,
              int dst_stride_argb,
              int dst_x,
              int dst_y,
              int width,
              int height);

// Apply a matrix rotation to each ARGB pixel.
LIBYUV_API
int ARGBColorMatrix(const uint8_t* src_argb,
                    int src_stride_argb,
                    uint8_t* dst_argb,
                    int dst_stride_argb,
                    const int8_t* matrix_argb,
                    int width,
                    int height);

// Deprecated. Use ARGBColorMatrix instead.
LIBYUV_API
int RGBColorMatrix(uint8_t* dst_argb,
                   int dst_stride_argb,
                   const int8_t* matrix_rgb,
                   int dst_x,
                   int dst_y,
                   int width,
                   int height);

// Apply a color table each ARGB pixel.
LIBYUV_API
int ARGBColorTable(uint8_t* dst_argb,
                   int dst_stride_argb,
                   const uint8_t* table_argb,
                   int dst_x,
                   int dst_y,
                   int width,
                   int height);

// Apply a color table each ARGB pixel but preserve destination alpha.
LIBYUV_API
int RGBColorTable(uint8_t* dst_argb,
                  int dst_stride_argb,
                  const uint8_t* table_argb,
                  int dst_x,
                  int dst_y,
                  int width,
                  int height);

// Apply a luma/color table each ARGB pixel but preserve destination alpha.
LIBYUV_API
int ARGBLumaColorTable(const uint8_t* src_argb,
                       int src_stride_argb,
                       uint8_t* dst_argb,
                       int dst_stride_argb,
                       const uint8_t* luma,
                       int width,
                       int height);

// Apply a 3 term polynomial to ARGB values.
LIBYUV_API
int ARGBPolynomial(const uint8_t* src_argb,
                   int src_stride_argb,
                   uint8_t* dst_argb,
                   int dst_stride_argb,
                   const float* poly,
                   int width,
                   int height);

// Convert plane of 16 bit shorts to half floats.
LIBYUV_API
int HalfFloatPlane(const uint16_t* src_y,
                   int src_stride_y,
                   uint16_t* dst_y,
                   int dst_stride_y,
                   float scale,
                   int width,
                   int height);

// Convert a buffer of bytes to floats, scale the values and store as floats.
LIBYUV_API
int ByteToFloat(const uint8_t* src_y, float* dst_y, float scale, int width);

// Quantize a rectangle of ARGB. Alpha unaffected.
LIBYUV_API
int ARGBQuantize(uint8_t* dst_argb,
                 int dst_stride_argb,
                 int scale,
                 int interval_size,
                 int interval_offset,
                 int dst_x,
                 int dst_y,
                 int width,
                 int height);

// Copy ARGB to ARGB.
LIBYUV_API
int ARGBCopy(const uint8_t* src_argb,
             int src_stride_argb,
             uint8_t* dst_argb,
             int dst_stride_argb,
             int width,
             int height);

// Copy Alpha channel of ARGB to alpha of ARGB.
LIBYUV_API
int ARGBCopyAlpha(const uint8_t* src_argb,
                  int src_stride_argb,
                  uint8_t* dst_argb,
                  int dst_stride_argb,
                  int width,
                  int height);

// Extract the alpha channel from ARGB.
LIBYUV_API
int ARGBExtractAlpha(const uint8_t* src_argb,
                     int src_stride_argb,
                     uint8_t* dst_a,
                     int dst_stride_a,
                     int width,
                     int height);

// Copy Y channel to Alpha of ARGB.
LIBYUV_API
int ARGBCopyYToAlpha(const uint8_t* src_y,
                     int src_stride_y,
                     uint8_t* dst_argb,
                     int dst_stride_argb,
                     int width,
                     int height);

typedef void (*ARGBBlendRow)(const uint8_t* src_argb0,
                             const uint8_t* src_argb1,
                             uint8_t* dst_argb,
                             int width);

// Get function to Alpha Blend ARGB pixels and store to destination.
LIBYUV_API
ARGBBlendRow GetARGBBlend();

// Alpha Blend ARGB images and store to destination.
LIBYUV_API
int ARGBBlend(const uint8_t* src_argb0,
              int src_stride_argb0,
              const uint8_t* src_argb1,
              int src_stride_argb1,
              uint8_t* dst_argb,
              int dst_stride_argb,
              int width,
              int height);

// Alpha Blend plane and store to destination.
LIBYUV_API
int BlendPlane(const uint8_t* src_y0,
               int src_stride_y0,
               const uint8_t* src_y1,
               int src_stride_y1,
               const uint8_t* alpha,
               int alpha_stride,
               uint8_t* dst_y,
               int dst_stride_y,
               int width,
               int height);

// Alpha Blend YUV images and store to destination.
LIBYUV_API
int I420Blend(const uint8_t* src_y0,
              int src_stride_y0,
              const uint8_t* src_u0,
              int src_stride_u0,
              const uint8_t* src_v0,
              int src_stride_v0,
              const uint8_t* src_y1,
              int src_stride_y1,
              const uint8_t* src_u1,
              int src_stride_u1,
              const uint8_t* src_v1,
              int src_stride_v1,
              const uint8_t* alpha,
              int alpha_stride,
              uint8_t* dst_y,
              int dst_stride_y,
              uint8_t* dst_u,
              int dst_stride_u,
              uint8_t* dst_v,
              int dst_stride_v,
              int width,
              int height);

// Multiply ARGB image by ARGB image. Shifted down by 8. Saturates to 255.
LIBYUV_API
int ARGBMultiply(const uint8_t* src_argb0,
                 int src_stride_argb0,
                 const uint8_t* src_argb1,
                 int src_stride_argb1,
                 uint8_t* dst_argb,
                 int dst_stride_argb,
                 int width,
                 int height);

// Add ARGB image with ARGB image. Saturates to 255.
LIBYUV_API
int ARGBAdd(const uint8_t* src_argb0,
            int src_stride_argb0,
            const uint8_t* src_argb1,
            int src_stride_argb1,
            uint8_t* dst_argb,
            int dst_stride_argb,
            int width,
            int height);

// Subtract ARGB image (argb1) from ARGB image (argb0). Saturates to 0.
LIBYUV_API
int ARGBSubtract(const uint8_t* src_argb0,
                 int src_stride_argb0,
                 const uint8_t* src_argb1,
                 int src_stride_argb1,
                 uint8_t* dst_argb,
                 int dst_stride_argb,
                 int width,
                 int height);

// Convert I422 to YUY2.
LIBYUV_API
int I422ToYUY2(const uint8_t* src_y,
               int src_stride_y,
               const uint8_t* src_u,
               int src_stride_u,
               const uint8_t* src_v,
               int src_stride_v,
               uint8_t* dst_yuy2,
               int dst_stride_yuy2,
               int width,
               int height);

// Convert I422 to UYVY.
LIBYUV_API
int I422ToUYVY(const uint8_t* src_y,
               int src_stride_y,
               const uint8_t* src_u,
               int src_stride_u,
               const uint8_t* src_v,
               int src_stride_v,
               uint8_t* dst_uyvy,
               int dst_stride_uyvy,
               int width,
               int height);

// Convert unattentuated ARGB to preattenuated ARGB.
LIBYUV_API
int ARGBAttenuate(const uint8_t* src_argb,
                  int src_stride_argb,
                  uint8_t* dst_argb,
                  int dst_stride_argb,
                  int width,
                  int height);

// Convert preattentuated ARGB to unattenuated ARGB.
LIBYUV_API
int ARGBUnattenuate(const uint8_t* src_argb,
                    int src_stride_argb,
                    uint8_t* dst_argb,
                    int dst_stride_argb,
                    int width,
                    int height);

// Internal function - do not call directly.
// Computes table of cumulative sum for image where the value is the sum
// of all values above and to the left of the entry. Used by ARGBBlur.
LIBYUV_API
int ARGBComputeCumulativeSum(const uint8_t* src_argb,
                             int src_stride_argb,
                             int32_t* dst_cumsum,
                             int dst_stride32_cumsum,
                             int width,
                             int height);

// Blur ARGB image.
// dst_cumsum table of width * (height + 1) * 16 bytes aligned to
//   16 byte boundary.
// dst_stride32_cumsum is number of ints in a row (width * 4).
// radius is number of pixels around the center.  e.g. 1 = 3x3. 2=5x5.
// Blur is optimized for radius of 5 (11x11) or less.
LIBYUV_API
int ARGBBlur(const uint8_t* src_argb,
             int src_stride_argb,
             uint8_t* dst_argb,
             int dst_stride_argb,
             int32_t* dst_cumsum,
             int dst_stride32_cumsum,
             int width,
             int height,
             int radius);

// Gaussian 5x5 blur a float plane.
// Coefficients of 1, 4, 6, 4, 1.
// Each destination pixel is a blur of the 5x5
// pixels from the source.
// Source edges are clamped.
LIBYUV_API
int GaussPlane_F32(const float* src,
                   int src_stride,
                   float* dst,
                   int dst_stride,
                   int width,
                   int height);

// Multiply ARGB image by ARGB value.
LIBYUV_API
int ARGBShade(const uint8_t* src_argb,
              int src_stride_argb,
              uint8_t* dst_argb,
              int dst_stride_argb,
              int width,
              int height,
              uint32_t value);

// Interpolate between two images using specified amount of interpolation
// (0 to 255) and store to destination.
// 'interpolation' is specified as 8 bit fraction where 0 means 100% src0
// and 255 means 1% src0 and 99% src1.
LIBYUV_API
int InterpolatePlane(const uint8_t* src0,
                     int src_stride0,
                     const uint8_t* src1,
                     int src_stride1,
                     uint8_t* dst,
                     int dst_stride,
                     int width,
                     int height,
                     int interpolation);

// Interpolate between two ARGB images using specified amount of interpolation
// Internally calls InterpolatePlane with width * 4 (bpp).
LIBYUV_API
int ARGBInterpolate(const uint8_t* src_argb0,
                    int src_stride_argb0,
                    const uint8_t* src_argb1,
                    int src_stride_argb1,
                    uint8_t* dst_argb,
                    int dst_stride_argb,
                    int width,
                    int height,
                    int interpolation);

// Interpolate between two YUV images using specified amount of interpolation
// Internally calls InterpolatePlane on each plane where the U and V planes
// are half width and half height.
LIBYUV_API
int I420Interpolate(const uint8_t* src0_y,
                    int src0_stride_y,
                    const uint8_t* src0_u,
                    int src0_stride_u,
                    const uint8_t* src0_v,
                    int src0_stride_v,
                    const uint8_t* src1_y,
                    int src1_stride_y,
                    const uint8_t* src1_u,
                    int src1_stride_u,
                    const uint8_t* src1_v,
                    int src1_stride_v,
                    uint8_t* dst_y,
                    int dst_stride_y,
                    uint8_t* dst_u,
                    int dst_stride_u,
                    uint8_t* dst_v,
                    int dst_stride_v,
                    int width,
                    int height,
                    int interpolation);

// Row function for copying pixels from a source with a slope to a row
// of destination. Useful for scaling, rotation, mirror, texture mapping.
LIBYUV_API
void ARGBAffineRow_C(const uint8_t* src_argb,
                     int src_argb_stride,
                     uint8_t* dst_argb,
                     const float* uv_dudv,
                     int width);
// TODO: Move ARGBAffineRow_SSE2 to row.h
LIBYUV_API
void ARGBAffineRow_SSE2(const uint8_t* src_argb,
                        int src_argb_stride,
                        uint8_t* dst_argb,
                        const float* uv_dudv,
                        int width);

// Shuffle ARGB channel order.  e.g. BGRA to ARGB.
// shuffler is 16 bytes and must be aligned.
LIBYUV_API
int ARGBShuffle(const uint8_t* src_bgra,
                int src_stride_bgra,
                uint8_t* dst_argb,
                int dst_stride_argb,
                const uint8_t* shuffler,
                int width,
                int height);

// Sobel ARGB effect with planar output.
LIBYUV_API
int ARGBSobelToPlane(const uint8_t* src_argb,
                     int src_stride_argb,
                     uint8_t* dst_y,
                     int dst_stride_y,
                     int width,
                     int height);

// Sobel ARGB effect.
LIBYUV_API
int ARGBSobel(const uint8_t* src_argb,
              int src_stride_argb,
              uint8_t* dst_argb,
              int dst_stride_argb,
              int width,
              int height);

// Sobel ARGB effect w/ Sobel X, Sobel, Sobel Y in ARGB.
LIBYUV_API
int ARGBSobelXY(const uint8_t* src_argb,
                int src_stride_argb,
                uint8_t* dst_argb,
                int dst_stride_argb,
                int width,
                int height);

#ifdef __cplusplus
}  // extern "C"
}  // namespace libyuv
#endif

#endif  // INCLUDE_LIBYUV_PLANAR_FUNCTIONS_H_

// By GST @Date