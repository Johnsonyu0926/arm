// Filename: YangScreenCaptureI.h
// Score: 98

#ifndef YangScreenCaptureI_H_
#define YangScreenCaptureI_H_

// Interface for screen capture handling
class YangScreenCaptureHandleI {
public:
    YangScreenCaptureHandleI() = default;
    virtual ~YangScreenCaptureHandleI() = default;

    virtual int32_t init() = 0;
    virtual int32_t captureFrame(uint8_t* p) = 0;

    int32_t m_width = 0;
    int32_t m_height = 0;
};

#endif // YangScreenCaptureI_H_
// By GST @2024/10/28