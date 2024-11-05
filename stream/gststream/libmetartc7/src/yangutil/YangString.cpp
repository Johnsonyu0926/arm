// Filename: YangString.cpp
// Score: 100

//
// Copyright (c) 2019-2022 yanggaofeng
//

#include <yangutil/sys/YangString.h>
#include <yangutil/sys/YangTime.h>
#include <inttypes.h>
#include <vector>
#include <string>

#if Yang_OS_WIN
#include <random>
#define srandom srand
#define random rand
#endif

#ifdef _MSC_VER
#include <windows.h>
#endif

void yang_write_string(YangBuffer* buf, const std::string& value) {
    if (buf == nullptr) return;
    yang_memcpy(buf->head, value.data(), value.length());
    buf->head += value.length();
}

std::string yang_read_string(YangBuffer* buf, int32_t len) {
    std::string value(buf->head, len);
    buf->head += len;
    return value;
}

void skip_first_spaces(std::string& str) {
    while (!str.empty() && str[0] == ' ') {
        str.erase(0, 1);
    }
}

std::vector<std::string> yang_split_first(const std::string& s, char ch) {
    std::vector<std::string> ret;
    size_t pos = s.find(ch);
    if (pos != std::string::npos) {
        ret.push_back(s.substr(0, pos));
        ret.push_back(s.substr(pos + 1));
    }
    return ret;
}

std::vector<std::string> yang_split(const std::string& s, char ch) {
    std::vector<std::string> ret;
    size_t start = 0;
    size_t pos = 0;
    while ((pos = s.find(ch, start)) != std::string::npos) {
        ret.push_back(s.substr(start, pos - start));
        start = pos + 1;
    }
    if (start < s.length()) {
        ret.push_back(s.substr(start));
    }
    return ret;
}

std::vector<std::string> yang_splits(const std::string& str, const std::string& delim) {
    std::vector<std::string> ret;
    size_t start = 0;
    size_t pos = 0;
    while ((pos = str.find(delim, start)) != std::string::npos) {
        ret.push_back(str.substr(start, pos - start));
        start = pos + delim.length();
    }
    if (start < str.length()) {
        ret.push_back(str.substr(start));
    }
    return ret;
}

std::string yang_int2str(int64_t value) {
    char tmp[22];
    snprintf(tmp, sizeof(tmp), "%" PRId64, value);
    return std::string(tmp);
}

std::string yang_random_str(int32_t len) {
    static const std::string random_table = "01234567890123456789012345678901234567890123456789abcdefghijklmnopqrstuvwxyz";
    std::string ret;
    ret.reserve(len);
    for (int32_t i = 0; i < len; ++i) {
        ret.push_back(random_table[random() % random_table.size()]);
    }
    return ret;
}

void yang_replace(std::string& strBig, const std::string& strsrc, const std::string& strdst) {
    size_t pos = 0;
    while ((pos = strBig.find(strsrc, pos)) != std::string::npos) {
        strBig.replace(pos, strsrc.length(), strdst);
        pos += strdst.length();
    }
}

// By GST @Date