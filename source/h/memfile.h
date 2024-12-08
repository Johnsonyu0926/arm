#ifndef _MEMFILE_H__
#define _MEMFILE_H__

#include <cstring>
#include <vector>

class CMemFile {
public:
    CMemFile() : m_nPos(0) {}

    ~CMemFile() = default;

    void SeekToBegin() {
        m_nPos = 0;
    }

    int Read(char* szValue, int nSize) {
        if (m_nPos + nSize > m_szBuf.size()) {
            nSize = m_szBuf.size() - m_nPos;
        }
        std::memcpy(szValue, m_szBuf.data() + m_nPos, nSize);
        m_nPos += nSize;
        return nSize;
    }

    void Write(const char* szBuf, int nSize) {
        m_szBuf.insert(m_szBuf.end(), szBuf, szBuf + nSize);
    }

private:
    std::vector<char> m_szBuf;
    int m_nPos;  // 当前指针指向数据的位置
};

#endif // _MEMFILE_H__