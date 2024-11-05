// memfile.cpp
#include "memfile.h"
#include <algorithm>
#include <cstring>

CMemFile::CMemFile() : m_nPos(0) {}

CMemFile::~CMemFile() = default;

void CMemFile::SeekToBegin() noexcept {
    m_nPos = 0;
}

std::size_t CMemFile::Read(std::span<char> buffer) noexcept {
    const auto available = m_buf.size() - m_nPos;
    const auto to_read = std::min(available, buffer.size());
    
    std::copy_n(m_buf.begin() + m_nPos, to_read, buffer.begin());
    m_nPos += to_read;
    
    return to_read;
}

void CMemFile::Write(std::span<const char> data) {
    const auto new_size = m_nPos + data.size();
    if (new_size > m_buf.size()) {
        m_buf.resize(new_size);
    }
    
    std::copy(data.begin(), data.end(), m_buf.begin() + m_nPos);
    m_nPos += data.size();
}

//BY GST ARMV8 GCC 13.2