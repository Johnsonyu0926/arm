#ifndef SHARE_H
#define SHARE_H

#include "doorsbase.h"
#include <stdint.h>
#include <stdbool.h>

#define MAX_ATTACH_COUNT 1024

typedef enum {
    DsCharsetType_None = 1,
    DsCharsetType_GB2312 = 2
} DsCharsetType;

typedef enum {
    DsEncodingMethod_None = 1,
    DsEncodingMethod_Bit8 = 2,
    DsEncodingMethod_Base64 = 3,
    DsEncodingMethod_Bit7 = 4,
    DsEncodingMethod_QP = 5
} DsEncodingMethod;

typedef enum {
    DsContextType_None = 1,
    DsContextType_TextPlain = 2,
    DsContextType_TextHtml = 3,
    DsContextType_AppOct = 4,
    DsContextType_AppMsWord = 5
} DsContextType;

typedef enum {
    DsDisposition_None = 1,
    DsDisposition_Attachment = 2
} DsDisposition;

typedef enum {
    DsAuditStatus_Succeeded = 1,
    DsAuditStatus_Failed = 2,
    DsAuditStatus_FailSubject = 3,
    DsAuditStatus_FailBody = 4,
    DsAuditStatus_FailAttachment = 5
} DsAuditStatus;

typedef struct BodyPart {
    DsEncodingMethod encoding;
    DsContextType contentType;
    int part;
    int bodyLen;
    char* body;
    DsDisposition disposition;
    char* attachFileName;
    struct BodyPart* next;
} BodyPart;

const char* GetEncodingMethodName(DsEncodingMethod method);
const char* GetContextTypeName(DsContextType type);
void PrintBodyPartInfo(const BodyPart* part);

#endif // SHARE_H