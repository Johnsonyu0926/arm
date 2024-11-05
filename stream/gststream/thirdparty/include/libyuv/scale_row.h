// Filename: scale_row.h
// Score: 95

#ifndef INCLUDE_LIBYUV_SCALE_ROW_H_
#define INCLUDE_LIBYUV_SCALE_ROW_H_

#include "libyuv/basic_types.h"
#include "libyuv/scale.h"

#ifdef __cplusplus
namespace libyuv {
extern "C" {
#endif

// Platform-specific optimizations
#if defined(__pnacl__) || defined(__CLR_VER) ||            \
    (defined(__native_client__) && defined(__x86_64__)) || \
    (defined(__i386__) && !defined(__SSE__) && !defined(__clang__))
#define LIBYUV_DISABLE_X86
#endif
#if defined(__native_client__)
#define LIBYUV_DISABLE_NEON
#endif
#if defined(__has_feature)
#if __has_feature(memory_sanitizer)
#define LIBYUV_DISABLE_X86
#endif
#endif
#if defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#if (__GNUC__ > 4) || (__GNUC__ == 4 && (__GNUC_MINOR__ >= 7))
#define GCC_HAS_AVX2 1
#endif
#endif
#if defined(__clang__) && (defined(__x86_64__) || defined(__i386__))
#if (__clang_major__ > 3) || (__clang_major__ == 3 && (__clang_minor__ >= 4))
#define CLANG_HAS_AVX2 1
#endif
#endif
#if defined(_M_IX86) && !defined(__clang__) && defined(_MSC_VER) && \
    _MSC_VER >= 1700
#define VISUALC_HAS_AVX2 1
#endif

// Available on all x86 platforms
#if !defined(LIBYUV_DISABLE_X86) && \
    (defined(_M_IX86) || defined(__x86_64__) || defined(__i386__))
#define HAS_FIXEDDIV1_X86
#define HAS_FIXEDDIV_X86
#define HAS_SCALEADDROW_SSE2
#define HAS_SCALEARGBCOLS_SSE2
#define HAS_SCALEARGBCOLSUP2_SSE2
#define HAS_SCALEARGBFILTERCOLS_SSSE3
#define HAS_SCALEARGBROWDOWN2_SSE2
#define HAS_SCALEARGBROWDOWNEVEN_SSE2
#define HAS_SCALECOLSUP2_SSE2
#define HAS_SCALEFILTERCOLS_SSSE3
#define HAS_SCALEROWDOWN2_SSSE3
#define HAS_SCALEROWDOWN34_SSSE3
#define HAS_SCALEROWDOWN38_SSSE3
#define HAS_SCALEROWDOWN4_SSSE3
#endif

// Available for gcc/clang x86 platforms
#if !defined(LIBYUV_DISABLE_X86) && \
    (defined(__x86_64__) || (defined(__i386__) && !defined(_MSC_VER)))
#define HAS_SCALEUVROWDOWN2BOX_SSSE3
#endif

// Available for gcc/clang x86 platforms, but require clang 3.4 or gcc 4.7
#if !defined(LIBYUV_DISABLE_X86) && \
    (defined(__x86_64__) || defined(__i386__)) && !defined(_MSC_VER) && \
    (defined(CLANG_HAS_AVX2) || defined(GCC_HAS_AVX2))
#define HAS_SCALEUVROWDOWN2BOX_AVX2
#endif

// Available on all x86 platforms, but require VS2012, clang 3.4 or gcc 4.7
#if !defined(LIBYUV_DISABLE_X86) &&                          \
    (defined(VISUALC_HAS_AVX2) || defined(CLANG_HAS_AVX2) || \
     defined(GCC_HAS_AVX2))
#define HAS_SCALEADDROW_AVX2
#define HAS_SCALEROWDOWN2_AVX2
#define HAS_SCALEROWDOWN4_AVX2
#endif

// Available on Neon platforms
#if !defined(LIBYUV_DISABLE_NEON) && \
    (defined(__ARM_NEON__) || defined(LIBYUV_NEON) || defined(__aarch64__))
#define HAS_SCALEADDROW_NEON
#define HAS_SCALEARGBCOLS_NEON
#define HAS_SCALEARGBFILTERCOLS_NEON
#define HAS_SCALEARGBROWDOWN2_NEON
#define HAS_SCALEARGBROWDOWNEVEN_NEON
#define HAS_SCALEFILTERCOLS_NEON
#define HAS_SCALEROWDOWN2_NEON
#define HAS_SCALEROWDOWN34_NEON
#define HAS_SCALEROWDOWN38_NEON
#define HAS_SCALEROWDOWN4_NEON
#define HAS_SCALEUVROWDOWN2BOX_NEON
#endif

#if !defined(LIBYUV_DISABLE_MSA) && defined(__mips_msa)
#define HAS_SCALEADDROW_MSA
#define HAS_SCALEARGBCOLS_MSA
#define HAS_SCALEARGBFILTERCOLS_MSA
#define HAS_SCALEARGBROWDOWN2_MSA
#define HAS_SCALEARGBROWDOWNEVEN_MSA
#define HAS_SCALEFILTERCOLS_MSA
#define HAS_SCALEROWDOWN2_MSA
#define HAS_SCALEROWDOWN34_MSA
#define HAS_SCALEROWDOWN38_MSA
#define HAS_SCALEROWDOWN4_MSA
#endif

#if !defined(LIBYUV_DISABLE_MMI) && defined(_MIPS_ARCH_LOONGSON3A)
#define HAS_FIXEDDIV1_MIPS
#define HAS_FIXEDDIV_MIPS
#define HAS_SCALEADDROW_16_MMI
#define HAS_SCALEADDROW_MMI
#define HAS_SCALEARGBCOLS_MMI
#define HAS_SCALEARGBCOLSUP2_MMI
#define HAS_SCALEARGBROWDOWN2_MMI
#define HAS_SCALEARGBROWDOWNEVEN_MMI
#define HAS_SCALECOLS_16_MMI
#define HAS_SCALECOLS_MMI
#define HAS_SCALEROWDOWN2_16_MMI
#define HAS_SCALEROWDOWN2_MMI
#define HAS_SCALEROWDOWN4_16_MMI
#define HAS_SCALEROWDOWN4_MMI
#define HAS_SCALEROWDOWN34_MMI
#endif

// Function declarations for scaling rows
void ScalePlaneVertical(int src_height,
                        int dst_width,
                        int dst_height,
                        int src_stride,
                        int dst_stride,
                        const uint8_t* src_argb,
                        uint8_t* dst_argb,
                        int x,
                        int y,
                        int dy,
                        int bpp,
                        enum FilterMode filtering);

void ScalePlaneVertical_16(int src_height,
                           int dst_width,
                           int dst_height,
                           int src_stride,
                           int dst_stride,
                           const uint16_t* src_argb,
                           uint16_t* dst_argb,
                           int x,
                           int y,
                           int dy,
                           int wpp,
                           enum FilterMode filtering);

enum FilterMode ScaleFilterReduce(int src_width,
                                  int src_height,
                                  int dst_width,
                                  int dst_height,
                                  enum FilterMode filtering);

int FixedDiv_C(int num, int div);
int FixedDiv_X86(int num, int div);
int FixedDiv_MIPS(int num, int div);
int FixedDiv1_C(int num, int div);
int FixedDiv1_X86(int num, int div);
int FixedDiv1_MIPS(int num, int div);

#ifdef HAS_FIXEDDIV_X86
#define FixedDiv FixedDiv_X86
#define FixedDiv1 FixedDiv1_X86
#elif defined HAS_FIXEDDIV_MIPS
#define FixedDiv FixedDiv_MIPS
#define FixedDiv1 FixedDiv1_MIPS
#else
#define FixedDiv FixedDiv_C
#define FixedDiv1 FixedDiv1_C
#endif

void ScaleSlope(int src_width,
                int src_height,
                int dst_width,
                int dst_height,
                enum FilterMode filtering,
                int* x,
                int* y,
                int* dx,
                int* dy);

void ScaleRowDown2_C(const uint8_t* src_ptr,
                     ptrdiff_t src_stride,
                     uint8_t* dst,
                     int dst_width);
void ScaleRowDown2_16_C(const uint16_t* src_ptr,
                        ptrdiff_t src_stride,
                        uint16_t* dst,
                        int dst_width);
void ScaleRowDown2Linear_C(const uint8_t* src_ptr,
                           ptrdiff_t src_stride,
                           uint8_t* dst,
                           int dst_width);
void ScaleRowDown2Linear_16_C(const uint16_t* src_ptr,
                              ptrdiff_t src_stride,
                              uint16_t* dst,
                              int dst_width);
void ScaleRowDown2Box_C(const uint8_t* src_ptr,
                        ptrdiff_t src_stride,
                        uint8_t* dst,
                        int dst_width);
void ScaleRowDown2Box_Odd_C(const uint8_t* src_ptr,
                            ptrdiff_t src_stride,
                            uint8_t* dst,
                            int dst_width);
void ScaleRowDown2Box_16_C(const uint16_t* src_ptr,
                           ptrdiff_t src_stride,
                           uint16_t* dst,
                           int dst_width);
void ScaleRowDown4_C(const uint8_t* src_ptr,
                     ptrdiff_t src_stride,
                     uint8_t* dst,
                     int dst_width);
void ScaleRowDown4_16_C(const uint16_t* src_ptr,
                        ptrdiff_t src_stride,
                        uint16_t* dst,
                        int dst_width);
void ScaleRowDown4Box_C(const uint8_t* src_ptr,
                        ptrdiff_t src_stride,
                        uint8_t* dst,
                        int dst_width);
void ScaleRowDown4Box_16_C(const uint16_t* src_ptr,
                           ptrdiff_t src_stride,
                           uint16_t* dst,
                           int dst_width);
void ScaleRowDown34_C(const uint8_t* src_ptr,
                      ptrdiff_t src_stride,
                      uint8_t* dst,
                      int dst_width);
void ScaleRowDown34_16_C(const uint16_t* src_ptr,
                         ptrdiff_t src_stride,
                         uint16_t* dst,
                         int dst_width);
void ScaleRowDown34_0_Box_C(const uint8_t* src_ptr,
                            ptrdiff_t src_stride,
                            uint8_t* d,
                            int dst_width);
void ScaleRowDown34_0_Box_16_C(const uint16_t* src_ptr,
                               ptrdiff_t src_stride,
                               uint16_t* d,
                               int dst_width);
void ScaleRowDown34_1_Box_C(const uint8_t* src_ptr,
                            ptrdiff_t src_stride,
                            uint8_t* d,
                            int dst_width);
void ScaleRowDown34_1_Box_16_C(const uint16_t* src_ptr,
                               ptrdiff_t src_stride,
                               uint16_t* d,
                               int dst_width);
void ScaleCols_C(uint8_t* dst_ptr,
                 const uint8_t* src_ptr,
                 int dst_width,
                 int x,
                 int dx);
void ScaleCols_16_C(uint16_t* dst_ptr,
                    const uint16_t* src_ptr,
                    int dst_width,
                    int x,
                    int dx);
void ScaleColsUp2_C(uint8_t* dst_ptr,
                    const uint8_t* src_ptr,
                    int dst_width,
                    int,
                    int);
void ScaleColsUp2_16_C(uint16_t* dst_ptr,
                       const uint16_t* src_ptr,
                       int dst_width,
                       int,
                       int);
void ScaleFilterCols_C(uint8_t* dst_ptr,
                       const uint8_t* src_ptr,
                       int dst_width,
                       int x,
                       int dx);
void ScaleFilterCols_16_C(uint16_t* dst_ptr,
                          const uint16_t* src_ptr,
                          int dst_width,
                          int x,
                          int dx);
void ScaleFilterCols64_C(uint8_t* dst_ptr,
                         const uint8_t* src_ptr,
                         int dst_width,
                         int x32,
                         int dx);
void ScaleFilterCols64_16_C(uint16_t* dst_ptr,
                            const uint16_t* src_ptr,
                            int dst_width,
                            int x32,
                            int dx);
void ScaleRowDown38_C(const uint8_t* src_ptr,
                      ptrdiff_t src_stride,
                      uint8_t* dst,
                      int dst_width);
void ScaleRowDown38_16_C(const uint16_t* src_ptr,
                         ptrdiff_t src_stride,
                         uint16_t* dst,
                         int dst_width);
void ScaleRowDown38_3_Box_C(const uint8_t* src_ptr,
                            ptrdiff_t src_stride,
                            uint8_t* dst_ptr,
                            int dst_width);
void ScaleRowDown38_3_Box_16_C(const uint16_t* src_ptr,
                               ptrdiff_t src_stride,
                               uint16_t* dst_ptr,
                               int dst_width);
void ScaleRowDown38_2_Box_C(const uint8_t* src_ptr,
                            ptrdiff_t src_stride,
                            uint8_t* dst_ptr,
                            int dst_width);
void ScaleRowDown38_2_Box_16_C(const uint16_t* src_ptr,
                               ptrdiff_t src_stride,
                               uint16_t* dst_ptr,
                               int dst_width);
void ScaleAddRow_C(const uint8_t* src_ptr, uint16_t* dst_ptr, int src_width);
void ScaleAddRow_16_C(const uint16_t* src_ptr,
                      uint32_t* dst_ptr,
                      int src_width);
void ScaleARGBRowDown2_C(const uint8_t* src_argb,
                         ptrdiff_t src_stride,
                         uint8_t* dst_argb,
                         int dst_width);
void ScaleARGBRowDown2Linear_C(const uint8_t* src_argb,
                               ptrdiff_t src_stride,
                               uint8_t* dst_argb,
                               int dst_width);
void ScaleARGBRowDown2Box_C(const uint8_t* src_argb,
                            ptrdiff_t src_stride,
                            uint8_t* dst_argb,
                            int dst_width);
void ScaleARGBRowDownEven_C(const uint8_t* src_argb,
                            ptrdiff_t src_stride,
                            int src_stepx,
                            uint8_t* dst_argb,
                            int dst_width);
void ScaleARGBRowDownEvenBox_C(const uint8_t* src_argb,
                               ptrdiff_t src_stride,
                               int src_stepx,
                               uint8_t* dst_argb,
                               int dst_width);
void ScaleARGBCols_C(uint8_t* dst_argb,
                     const uint8_t* src_argb,
                     int dst_width,
                     int x,
                     int dx);
void ScaleARGBCols64_C(uint8_t* dst_argb,
                       const uint8_t* src_argb,
                       int dst_width,
                       int x32,
                       int dx);
void ScaleARGBColsUp2_C(uint8_t* dst_argb,
                        const uint8_t* src_argb,
                        int dst_width,
                        int,
                        int);
void ScaleARGBFilterCols_C(uint8_t* dst_argb,
                           const uint8_t* src_argb,
                           int dst_width,
                           int x,
                           int dx);
void ScaleARGBFilterCols64_C(uint8_t* dst_argb,
                             const uint8_t* src_argb,
                             int dst_width,
                             int x32,
                             int dx);
void ScaleUVRowDown2_C(const uint8_t* src_uv,
                       ptrdiff_t src_stride,
                       uint8_t* dst_uv,
                       int dst_width);
void ScaleUVRowDown2Linear_C(const uint8_t* src_uv,
                             ptrdiff_t src_stride,
                             uint8_t* dst_uv,
                             int dst_width);
void ScaleUVRowDown2Box_C(const uint8_t* src_uv,
                          ptrdiff_t src_stride,
                          uint8_t* dst_uv,
                          int dst_width);
void ScaleUVRowDownEven_C(const uint8_t* src_uv,
                          ptrdiff_t src_stride,
                          int src_stepx,
                          uint8_t* dst_uv,
                          int dst_width);
void ScaleUVRowDownEvenBox_C(const uint8_t* src_uv,
                             ptrdiff_t src_stride,
                             int src_stepx,
                             uint8_t* dst_uv,
                             int dst_width);
void ScaleUVCols_C(uint8_t* dst_uv,
                   const uint8_t* src_uv,
                   int dst_width,
                   int x,
                   int dx);
void ScaleUVCols64_C(uint8_t* dst_uv,
                     const uint8_t* src_uv,
                     int dst_width,
                     int x32,
                     int dx);
void ScaleUVColsUp2_C(uint8_t* dst_uv,
                      const uint8_t* src_uv,
                      int dst_width,
                      int,
                      int);
void ScaleUVFilterCols_C(uint8_t* dst_uv,
                         const uint8_t* src_uv,
                         int dst_width,
                         int x,
                         int dx);
void ScaleUVFilterCols64_C(uint8_t* dst_uv,
                           const uint8_t* src_uv,
                           int dst_width,
                           int x32,
                           int dx);

// Specialized scalers for x86
void ScaleRowDown2_SSSE3(const uint8_t* src_ptr,
                         ptrdiff_t src_stride,
                         uint8_t* dst_ptr,
                         int dst_width);
void ScaleRowDown2Linear_SSSE3(const uint8_t* src_ptr,
                               ptrdiff_t src_stride,
                               uint8_t* dst_ptr,
                               int dst_width);
void ScaleRowDown2Box_SSSE3(const uint8_t* src_ptr,
                            ptrdiff_t src_stride,
                            uint8_t* dst_ptr,
                            int dst_width);
void ScaleRowDown2_AVX2(const uint8_t* src_ptr,
                        ptrdiff_t src_stride,
                        uint8_t* dst_ptr,
                        int dst_width);
void ScaleRowDown2Linear_AVX2(const uint8_t* src_ptr,
                              ptrdiff_t src_stride,
                              uint8_t* dst_ptr,
                              int dst_width);
void ScaleRowDown2Box_AVX2(const uint8_t* src_ptr,
                           ptrdiff_t src_stride,
                           uint8_t* dst_ptr,
                           int dst_width);
void ScaleRowDown4_SSSE3(const uint8_t* src_ptr,
                         ptrdiff_t src_stride,
                         uint8_t* dst_ptr,
                         int dst_width);
void ScaleRowDown4Box_SSSE3(const uint8_t* src_ptr,
                            ptrdiff_t src_stride,
                            uint8_t* dst_ptr,
                            int dst_width);
void ScaleRowDown4_AVX2(const uint8_t* src_ptr,
                        ptrdiff_t src_stride,
                        uint8_t* dst_ptr,
                        int dst_width);
void ScaleRowDown4Box_AVX2(const uint8_t* src_ptr,
                           ptrdiff_t src_stride,
                           uint8_t* dst_ptr,
                           int dst_width);

void ScaleRowDown34_SSSE3(const uint8_t* src_ptr,
                          ptrdiff_t src_stride,
                          uint8_t* dst_ptr,
                          int dst_width);
void ScaleRowDown34_1_Box_SSSE3(const uint8_t* src_ptr,
                                ptrdiff_t src_stride,
                                uint8_t* dst_ptr,
                                int dst_width);
void ScaleRowDown34_0_Box_SSSE3(const uint8_t* src_ptr,
                                ptrdiff_t src_stride,
                                uint8_t* dst_ptr,
                                int dst_width);
void ScaleRowDown38_SSSE3(const uint8_t* src_ptr,
                          ptrdiff_t src_stride,
                          uint8_t* dst_ptr,
                          int dst_width);
void ScaleRowDown38_3_Box_SSSE3(const uint8_t* src_ptr,
                                ptrdiff_t src_stride,
                                uint8_t* dst_ptr,
                                int dst_width);
void ScaleRowDown