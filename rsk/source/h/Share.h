// share.h
#ifndef _SHARE_H__
#define _SHARE_H__

#include "doorsbase.h"
#include <string>
#include <memory>

inline constexpr int MAX_ATTACH_COUNT = 1024;

enum class DsCharsetType {
    None = 1,
    GB2312 = 2
};

enum class DsEncodingMethod {
    None = 1,
    Bit8 = 2,
    Base64 = 3,
    Bit7 = 4,
    QP = 5
};

enum class DsContextType {
    None = 1,
    TextPlain = 2,
    TextHtml = 3,
    AppOct = 4,
    AppMsWord = 5
};

enum class DsDisposition {
    None = 1,
    Attachment = 2
};

enum class DsAuditStatus {
    Succeeded = 1,
    Failed = 2,
    FailSubject = 3,
    FailBody = 4,
    FailAttachment = 5
};

struct BodyPart {
    DsEncodingMethod encoding = DsEncodingMethod::None;
    DsContextType contentType = DsContextType::None;
    int part = 0;
    int bodyLen = 0;
    std::unique_ptr<char[]> body;
    DsDisposition disposition = DsDisposition::None;
    std::unique_ptr<char[]> attachFileName;
    std::unique_ptr<BodyPart> next;

    BodyPart() = default;
    BodyPart(const BodyPart&) = delete;
    BodyPart& operator=(const BodyPart&) = delete;
    BodyPart(BodyPart&&) noexcept = default;
    BodyPart& operator=(BodyPart&&) noexcept = default;
};

using Gap_BodyPart_t = std::unique_ptr<BodyPart>;

#endif // _SHARE_H__

//BY GST ARMV8 GCC 13.2