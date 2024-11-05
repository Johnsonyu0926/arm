// memfile.h
#ifndef MEMFILE_H
#define MEMFILE_H

#include <vector>
#include <span>
#include <cstddef>

class CMemFile {
public:
    CMemFile();
    ~CMemFile();

    CMemFile(const CMemFile&) = delete;
    CMemFile& operator=(const CMemFile&) = delete;
    CMemFile(CMemFile&&) noexcept = default;
    CMemFile& operator=(CMemFile&&) noexcept = default;

    void SeekToBegin() noexcept;
    [[nodiscard]] std::size_t Read(std::span<char> buffer) noexcept;
    void Write(std::span<const char> data);

private:
    std::vector<char> m_buf;
    std::size_t m_nPos;
};

#endif // MEMFILE_H

//BY GST ARMV8 GCC 13.2