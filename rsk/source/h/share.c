#include "share.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 获取编码方法名称
const char* GetEncodingMethodName(DsEncodingMethod method) {
    switch (method) {
        case DsEncodingMethod_None: return "None";
        case DsEncodingMethod_Bit8: return "8-bit";
        case DsEncodingMethod_Base64: return "Base64";
        case DsEncodingMethod_Bit7: return "7-bit";
        case DsEncodingMethod_QP: return "Quoted-Printable";
        default: return "Unknown";
    }
}

// 获取内容类型名称
const char* GetContextTypeName(DsContextType type) {
    switch (type) {
        case DsContextType_None: return "None";
        case DsContextType_TextPlain: return "Text/Plain";
        case DsContextType_TextHtml: return "Text/HTML";
        case DsContextType_AppOct: return "Application/Octet-Stream";
        case DsContextType_AppMsWord: return "Application/MS-Word";
        default: return "Unknown";
    }
}

// 打印BodyPart信息
void PrintBodyPartInfo(const BodyPart* part) {
    printf("Encoding: %s\n", GetEncodingMethodName(part->encoding));
    printf("Content Type: %s\n", GetContextTypeName(part->contentType));
    printf("Part Number: %d\n", part->part);
    printf("Body Length: %d\n", part->bodyLen);
    printf("Disposition: %s\n", (part->disposition == DsDisposition_Attachment ? "Attachment" : "Inline"));

    if (part->attachFileName) {
        printf("Attachment Filename: %s\n", part->attachFileName);
    }
}