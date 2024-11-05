#ifndef CCONV_H
#define CCONV_H

#include <string>
#include "doorsbase.h"

constexpr int BUFLEN = 2000;

class CConv {
public:
    CConv() = default;
    ~CConv() = default;

    bool Utf8Encoding(const std::string& src, std::string& dest);

private:
    bool outbin(const std::string& src, std::string& dest);
    // Helper function for UTF-8 encoding
    std::vector utf8Encode(uint32_t codepoint);
};

inline bool CConv::Utf8Encoding(const std::string& src, std::string& dest) {
    dest.clear();
    for (char c : src) {
        auto encoded = utf8Encode(static_cast(c));
        dest.append(encoded.begin(), encoded.end());
    }
    return true;
}

inline bool CConv::outbin(const std::string& src, std::string& dest) {
    dest = src;
    return true;
}

inline std::vector CConv::utf8Encode(uint32_t codepoint) {
    std::vector result;
    if (codepoint <= 0x7F) {
        result.push_back(static_cast(codepoint));
    } else if (codepoint <= 0x7FF) {
        result.push_back(static_cast(0xC0 | ((codepoint >> 6) & 0x1F)));
        result.push_back(static_cast(0x80 | (codepoint & 0x3F)));
    } else if (codepoint <= 0xFFFF) {
        result.push_back(static_cast(0xE0 | ((codepoint >> 12) & 0x0F)));
        result.push_back(static_cast(0x80 | ((codepoint >> 6) & 0x3F)));
        result.push_back(static_cast(0x80 | (codepoint & 0x3F)));
    } else if (codepoint <= 0x10FFFF) {
        result.push_back(static_cast(0xF0 | ((codepoint >> 18) & 0x07)));
        result.push_back(static_cast(0x80 | ((codepoint >> 12) & 0x3F)));
        result.push_back(static_cast(0x80 | ((codepoint >> 6) & 0x3F)));
        result.push_back(static_cast(0x80 | (codepoint & 0x3F)));
    }
    return result;
}

#endif // CCONV_H

//BY GST ARMV8 GCC142.