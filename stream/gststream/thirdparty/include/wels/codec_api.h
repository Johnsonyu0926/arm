#ifndef WELS_VIDEO_CODEC_SVC_API_H__
#define WELS_VIDEO_CODEC_SVC_API_H__

#ifndef __cplusplus
#if defined(_MSC_VER) && (_MSC_VER < 1800)
typedef unsigned char bool;
#else
#include <stdbool.h>
#endif
#endif

#include "codec_app_def.h"
#include "codec_def.h"

#if defined(_WIN32) || defined(__cdecl)
#define EXTAPI __cdecl
#else
#define EXTAPI
#endif

#ifdef __cplusplus
/**
* @brief Encoder definition
*/
class ISVCEncoder {
 public:
  /**
  * @brief  Initialize the encoder
  * @param  pParam  basic encoder parameter
  * @return CM_RETURN: 0 - success; otherwise - failed;
  */
  virtual int EXTAPI Initialize (const SEncParamBase* pParam) = 0;

  /**
  * @brief  Initialize encoder by using extension parameters.
  * @param  pParam  extension parameter for encoder
  * @return CM_RETURN: 0 - success; otherwise - failed;
  */
  virtual int EXTAPI InitializeExt (const SEncParamExt* pParam) = 0;

  /**
  * @brief   Get the default extension parameters.
  *          If you want to change some parameters of encoder, firstly you need to get the default encoding parameters,
  *          after that you can change part of parameters you want to.
  * @param   pParam  extension parameter for encoder
  * @return  CM_RETURN: 0 - success; otherwise - failed;
  * */
  virtual int EXTAPI GetDefaultParams (SEncParamExt* pParam) = 0;
  /// uninitialize the encoder
  virtual int EXTAPI Uninitialize() = 0;

  /**
  * @brief Encode one frame
  * @param kpSrcPic the pointer to the source luminance plane
  *        chrominance data:
  *        CbData = kpSrc  +  m_iMaxPicWidth * m_iMaxPicHeight;
  *        CrData = CbData + (m_iMaxPicWidth * m_iMaxPicHeight)/4;
  *        the application calling this interface needs to ensure the data validation between the location
  * @param pBsInfo output bit stream
  * @return  0 - success; otherwise -failed;
  */
  virtual int EXTAPI EncodeFrame (const SSourcePicture* kpSrcPic, SFrameBSInfo* pBsInfo) = 0;

  /**
  * @brief  Encode the parameters from output bit stream
  * @param  pBsInfo output bit stream
  * @return 0 - success; otherwise - failed;
  */
  virtual int EXTAPI EncodeParameterSets (SFrameBSInfo* pBsInfo) = 0;

  /**
  * @brief  Force encoder to encode frame as IDR if bIDR set as true
  * @param  bIDR true: force encoder to encode frame as IDR frame; false, return 1 and nothing to do
  * @return 0 - success; otherwise - failed;
  */
  virtual int EXTAPI ForceIntraFrame (bool bIDR, int iLayerId = -1) = 0;

  /**
  * @brief   Set option for encoder, detail option type, please refer to enumurate ENCODER_OPTION.
  * @param   pOption option for encoder such as InDataFormat, IDRInterval, SVC Encode Param, Frame Rate, Bitrate,...
  * @return  CM_RETURN: 0 - success; otherwise - failed;
  */
  virtual int EXTAPI SetOption (ENCODER_OPTION eOptionId, void* pOption) = 0;

  /**
  * @brief   Get option for encoder, detail option type, please refer to enumurate ENCODER_OPTION.
  * @param   pOption option for encoder such as InDataFormat, IDRInterval, SVC Encode Param, Frame Rate, Bitrate,...
  * @return  CM_RETURN: 0 - success; otherwise - failed;
  */
  virtual int EXTAPI GetOption (ENCODER_OPTION eOptionId, void* pOption) = 0;
  virtual ~ISVCEncoder() {}
};

/**
* @brief Decoder definition
*/
class ISVCDecoder {
 public:

  /**
  * @brief  Initialize decoder
  * @param  pParam  parameter for decoder
  * @return 0 - success; otherwise - failed;
  */
  virtual long EXTAPI Initialize (const SDecodingParam* pParam) = 0;

  /// Uninitialize the decoder
  virtual long EXTAPI Uninitialize() = 0;

  /**
  * @brief   Decode one frame
  * @param   pSrc the h264 stream to be decoded
  * @param   iSrcLen the length of h264 stream
  * @param   ppDst buffer pointer of decoded data (YUV)
  * @param   pStride output stride
  * @param   iWidth output width
  * @param   iHeight output height
  * @return  0 - success; otherwise -failed;
  */
  virtual DECODING_STATE EXTAPI DecodeFrame (const unsigned char* pSrc,
      const int iSrcLen,
      unsigned char** ppDst,
      int* pStride,
      int* iWidth,
      int* iHeight) = 0;

  virtual DECODING_STATE EXTAPI DecodeFrameNoDelay (const unsigned char* pSrc,
      const int iSrcLen,
      unsigned char** ppDst,
      SBufferInfo* pDstInfo) = 0;

  virtual DECODING_STATE EXTAPI DecodeFrame2 (const unsigned char* pSrc,
      const int iSrcLen,
      unsigned char** ppDst,
      SBufferInfo* pDstInfo) = 0;

  virtual DECODING_STATE EXTAPI FlushFrame (unsigned char** ppDst,
      SBufferInfo* pDstInfo) = 0;

  /**
  * @brief   This function parse input bitstream only, and rewrite possible SVC syntax to AVC syntax
  * @param   pSrc the h264 stream to be decoded
  * @param   iSrcLen the length of h264 stream
  * @param   pDstInfo bit stream info
  * @return  0 - success; otherwise -failed;
  */
  virtual DECODING_STATE EXTAPI DecodeParser (const unsigned char* pSrc,
      const int iSrcLen,
      SParserBsInfo* pDstInfo) = 0;

  virtual DECODING_STATE EXTAPI DecodeFrameEx (const unsigned char* pSrc,
      const int iSrcLen,
      unsigned char* pDst,
      int iDstStride,
      int& iDstLen,
      int& iWidth,
      int& iHeight,
      int& iColorFormat) = 0;

  /**
  * @brief   Set option for decoder, detail option type, please refer to enumurate DECODER_OPTION.
  * @param   pOption  option for decoder such as OutDataFormat, Eos Flag, EC method, ...
  * @return  CM_RETURN: 0 - success; otherwise - failed;
  */
  virtual long EXTAPI SetOption (DECODER_OPTION eOptionId, void* pOption) = 0;

  /**
  * @brief   Get option for decoder, detail option type, please refer to enumurate DECODER_OPTION.
  * @param   pOption  option for decoder such as OutDataFormat, Eos Flag, EC method, ...
  * @return  CM_RETURN: 0 - success; otherwise - failed;
  */
  virtual long EXTAPI GetOption (DECODER_OPTION eOptionId, void* pOption) = 0;
  virtual ~ISVCDecoder() {}
};

extern "C" {
#else

typedef struct ISVCEncoderVtbl ISVCEncoderVtbl;
typedef const ISVCEncoderVtbl* ISVCEncoder;
struct ISVCEncoderVtbl {
  int (*Initialize) (ISVCEncoder*, const SEncParamBase* pParam);
  int (*InitializeExt) (ISVCEncoder*, const SEncParamExt* pParam);
  int (*GetDefaultParams) (ISVCEncoder*, SEncParamExt* pParam);
  int (*Uninitialize) (ISVCEncoder*);
  int (*EncodeFrame) (ISVCEncoder*, const SSourcePicture* kpSrcPic, SFrameBSInfo* pBsInfo);
  int (*EncodeParameterSets) (ISVCEncoder*, SFrameBSInfo* pBsInfo);
  int (*ForceIntraFrame) (ISVCEncoder*, bool bIDR);
  int (*SetOption) (ISVCEncoder*, ENCODER_OPTION eOptionId, void* pOption);
  int (*GetOption) (ISVCEncoder*, ENCODER_OPTION eOptionId, void* pOption);
};

typedef struct ISVCDecoderVtbl ISVCDecoderVtbl;
typedef const ISVCDecoderVtbl* ISVCDecoder;
struct ISVCDecoderVtbl {
  long (*Initialize) (ISVCDecoder*, const SDecodingParam* pParam);
  long (*Uninitialize) (ISVCDecoder*);
  DECODING_STATE (*DecodeFrame) (ISVCDecoder*, const unsigned char* pSrc,
                                 const int iSrcLen,
                                 unsigned char** ppDst,
                                 int* pStride,
                                 int* iWidth,
                                 int* iHeight);
  DECODING_STATE (*DecodeFrameNoDelay) (ISVCDecoder*, const unsigned char* pSrc,
                                        const int iSrcLen,
                                        unsigned char** ppDst,
                                        SBufferInfo* pDstInfo);
  DECODING_STATE (*DecodeFrame2) (ISVCDecoder*, const unsigned char* pSrc,
                                  const int iSrcLen,
                                  unsigned char** ppDst,
                                  SBufferInfo* pDstInfo);
  DECODING_STATE (*FlushFrame) (ISVCDecoder*, unsigned char** ppDst,
                                SBufferInfo* pDstInfo);
  DECODING_STATE (*DecodeParser) (ISVCDecoder*, const unsigned char* pSrc,
                                  const int iSrcLen,
                                  SParserBsInfo* pDstInfo);
  DECODING_STATE (*DecodeFrameEx) (ISVCDecoder*, const unsigned char* pSrc,
                                   const int iSrcLen,
                                   unsigned char* pDst,
                                   int iDstStride,
                                   int* iDstLen,
                                   int* iWidth,
                                   int* iHeight,
                                   int* iColorFormat);
  long (*SetOption) (ISVCDecoder*, DECODER_OPTION eOptionId, void* pOption);
  long (*GetOption) (ISVCDecoder*, DECODER_OPTION eOptionId, void* pOption);
};
#endif

typedef void (*WelsTraceCallback) (void* ctx, int level, const char* string);

/** @brief   Create encoder
 *  @param   ppEncoder encoder
 *  @return  0 - success; otherwise - failed;
*/
int  WelsCreateSVCEncoder (ISVCEncoder** ppEncoder);

/** @brief   Destroy encoder
*   @param   pEncoder encoder
 *  @return  void
*/
void WelsDestroySVCEncoder (ISVCEncoder* pEncoder);

/** @brief   Get the capability of decoder
 *  @param   pDecCapability  decoder capability
 *  @return  0 - success; otherwise - failed;
*/
int WelsGetDecoderCapability (SDecoderCapability* pDecCapability);

/** @brief   Create decoder
 *  @param   ppDecoder decoder
 *  @return  0 - success; otherwise - failed;
*/
long WelsCreateDecoder (ISVCDecoder** ppDecoder);

/** @brief   Destroy decoder
 *  @param   pDecoder  decoder
 *  @return  void
*/
void WelsDestroyDecoder (ISVCDecoder* pDecoder);

/** @brief   Get codec version
 *           Note, old versions of Mingw (GCC < 4.7) are buggy and use an
 *           incorrect/different ABI for calling this function, making it
 *           incompatible with MSVC builds.
 *  @return  The linked codec version
*/
OpenH264Version WelsGetCodecVersion (void);

/** @brief   Get codec version
 *  @param   pVersion  struct to fill in with the version
*/
void WelsGetCodecVersionEx (OpenH264Version* pVersion);

#ifdef __cplusplus
}
#endif

#endif // WELS_VIDEO_CODEC_SVC_API_H__
// By GST @Date