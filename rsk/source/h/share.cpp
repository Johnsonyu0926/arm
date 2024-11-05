// share.cpp
#include "share.h"
#include <iostream>

// 如果需要，可以在这里定义全局变量
// 例如：std::array<BodyPart, MAX_ATTACH_COUNT> g_bodyParts;

// 实用函数示例
std::string_view GetEncodingMethodName(DsEncodingMethod method) {
    switch (method) {
        case DsEncodingMethod::None: return "None";
        case DsEncodingMethod::Bit8: return "8-bit";
        case DsEncodingMethod::Base64: return "Base64";
        case DsEncodingMethod::Bit7: return "7-bit";
        case DsEncodingMethod::QP: return "Quoted-Printable";
        default: return "Unknown";
    }
}

std::string_view GetContextTypeName(DsContextType type) {
    switch (type) {
        case DsContextType::None: return "None";
        case DsContextType::TextPlain: return "Text/Plain";
        case DsContextType::TextHtml: return "Text/HTML";
        case DsContextType::AppOct: return "Application/Octet-Stream";
        case DsContextType::AppMsWord: return "Application/MS-Word";
        default: return "Unknown";
    }
}

void PrintBodyPartInfo(const BodyPart& part) {
    std::cout << "Encoding: " << GetEncodingMethodName(part.encoding) << '\n'
              << "Content Type: " << GetContextTypeName(part.contentType) << '\n'
              << "Part Number: " << part.part << '\n'
              << "Body Length: " << part.bodyLen << '\n'
              << "Disposition: " << (part.disposition == DsDisposition::Attachment ? "Attachment" : "Inline") << '\n';
    
    if (part.attachFileName) {
        std::cout << "Attachment Filename: " << part.attachFileName.get() << '\n';
    }
}



//BY GST ARMV8 GCC 13.2