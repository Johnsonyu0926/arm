// mailcoder.cpp
#include "mailcoder.h"
#include <stdexcept>
#include <array>
#include <span>

// 静态成员初始化
unsigned int CMailCoder::m_LineWidth = 76;
constexpr std::array<char, 64> CMailCoder::BASE64_ENCODE_TABLE = {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"};
constexpr std::array<unsigned int, 256> CMailCoder::BASE64_DECODE_TABLE = {
    /* 0-255 初始化解码表 */
};
constexpr std::array<unsigned char, 256> CMailCoder::QUOTED_ENCODE_TABLE = {
    /* 0-255 初始化编码表 */
};

unsigned char CMailCoder::DecToHex(unsigned char B) {
    if (B < 10) return '0' + B;
    return 'A' + (B - 10);
}

unsigned char CMailCoder::HexToDec(unsigned char C) {
    if (C >= '0' && C <= '9') return C - '0';
    if (C >= 'A' && C <= 'F') return C - 'A' + 10;
    if (C >= 'a' && C <= 'f') return C - 'a' + 10;
    throw std::runtime_error("Invalid hex character");
}

int CMailCoder::Base64EncodeSize(int iSize) {
    return ((iSize + 2) / 3) * 4;
}

int CMailCoder::Base64DecodeSize(int iSize) {
    return (iSize / 4) * 3;
}

int CMailCoder::UUEncodeSize(int iSize) {
    return ((iSize + 2) / 3) * 4;
}

int CMailCoder::UUDecodeSize(int iSize) {
    return (iSize / 4) * 3;
}

int CMailCoder::QuotedEncodeSize(int iSize) {
    return iSize * 3;
}

int CMailCoder::base64_encode(std::span<const char> src, std::span<char> dest) {
    size_t i = 0, j = 0;
    while (i < src.size()) {
        std::array<unsigned char, 3> input = {0, 0, 0};
        std::array<unsigned char, 4> output;
        int value = 0;
        for (int n = 0; n < 3; ++n) {
            if (i < src.size()) {
                input[n] = src[i++];
            } else {
                value = 1;  // padding
            }
        }
        output[0] = BASE64_ENCODE_TABLE[input[0] >> 2];
        output[1] = BASE64_ENCODE_TABLE[((input[0] & 0x03) << 4) | (input[1] >> 4)];
        output[2] = (value == 1) ? '=' : BASE64_ENCODE_TABLE[((input[1] & 0x0F) << 2) | (input[2] >> 6)];
        output[3] = (value == 1 || value == 2) ? '=' : BASE64_ENCODE_TABLE[input[2] & 0x3F];
        for (int n = 0; n < 4; ++n) {
            if (j < dest.size()) {
                dest[j++] = output[n];
            }
        }
    }
    return static_cast<int>(j);
}

int CMailCoder::base64_decode(std::span<const char> src, std::span<char> dest) {
    size_t i = 0, j = 0;
    while (i < src.size()) {
        std::array<unsigned char, 4> input = {0, 0, 0, 0};
        std::array<unsigned char, 3> output;
        for (int n = 0; n < 4; ++n) {
            if (i < src.size()) {
                input[n] = BASE64_DECODE_TABLE[static_cast<unsigned char>(src[i++])];
            }
        }
        output[0] = (input[0] << 2) | (input[1] >> 4);
        output[1] = (input[1] << 4) | (input[2] >> 2);
        output[2] = (input[2] << 6) | input[3];
        for (int n = 0; n < 3; ++n) {
            if (input[n + 1] != 64 && j < dest.size()) {  // padding
                dest[j++] = output[n];
            }
        }
    }
    return static_cast<int>(j);
}

int CMailCoder::UU_encode(std::span<const char> src, std::span<char> dest) {
    // 实现UUE编码
    return 0;
}

int CMailCoder::UU_decode(std::span<const char> src, std::span<char> dest) {
    // 实现UUE解码
    return 0;
}

int CMailCoder::quoted_encode(std::span<const char> src, std::span<char> dest) {
    // 实现Quoted-Printable编码
    return 0;
}

int CMailCoder::quoted_decode(std::span<const char> src, std::span<char> dest) {
    // 实现Quoted-Printable解码
    return 0;
}

//BY GST ARMV8 GCC 13.2