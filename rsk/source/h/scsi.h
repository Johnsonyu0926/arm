// scsi.h
#ifndef AFX_SCSI_H__701A166F_809B_47D6_B487_1907DC8ECCCE__INCLUDED_
#define AFX_SCSI_H__701A166F_809B_47D6_B487_1907DC8ECCCE__INCLUDED_

#include <string>
#include <array>
#include <cstdint>
#include "doorsdefs.h"
#include "doorsbase.h"

// SCSI Command Definitions
inline constexpr uint8_t TESTUNITREADY_CMD = 0;
inline constexpr uint8_t TESTUNITREADY_CMDLEN = 6;
inline constexpr uint8_t ADD_SENSECODE = 12;
inline constexpr uint8_t ADD_SC_QUALIFIER = 13;
inline constexpr uint8_t NO_MEDIA_SC = 0x3a;
inline constexpr uint8_t NO_MEDIA_SCQ = 0x00;
inline constexpr uint8_t INQUIRY_CMD = 0x12;
inline constexpr uint8_t INQUIRY_CMDLEN = 6;
inline constexpr uint8_t INQUIRY_REPLY_LEN = 96;
inline constexpr uint8_t INQUIRY_VENDOR = 8;
inline constexpr uint8_t SCSI_OFF = 36;
inline constexpr const char* DEVICE = "/dev/sg0";
inline constexpr uint8_t SWITCH_CMD = 0x17;
inline constexpr uint8_t SWITCH_CMDLEN = 6;

// SCSI Read/Write
struct SCSI_Cdb_Read {
    uint8_t CommandCode;
    uint8_t LBA2 : 5;
    uint8_t Lun : 3;
    uint8_t LBA1;
    uint8_t LBA0;
    uint8_t Length;
    uint8_t Link : 1;
    uint8_t Flag : 1;
    uint8_t : 4;
    uint8_t VendorSpecific : 2;
};

struct SCSI_Cdb_Write {
    uint8_t CommandCode;
    uint8_t LBA2 : 5;
    uint8_t Lun : 3;
    uint8_t LBA1;
    uint8_t LBA0;
    uint8_t Length;
    uint8_t Link : 1;
    uint8_t Flag : 1;
    uint8_t : 4;
    uint8_t VendorSpecific : 2;
};

inline constexpr uint8_t SCSI_Cmd_Read = 0x08;
inline constexpr uint8_t SCSI_Cmd_Write = 0x0A;

inline constexpr const char* VERSION = "Prospect EGAP 1";

class CHeadBlock {
public:
    CHeadBlock() : m_szVersion{}, m_nBlockCount{}, m_pBufRev{} {
        std::copy_n(VERSION, std::min(sizeof(m_szVersion) - 1, std::string_view(VERSION).length()), m_szVersion);
    }

    std::array<char, 16> m_szVersion;
    int m_nBlockCount;
    std::array<BYTE, 492> m_pBufRev;
};

class CScsi {
public:
    CScsi();
    virtual ~CScsi();

    CScsi(const CScsi&) = delete;
    CScsi& operator=(const CScsi&) = delete;
    CScsi(CScsi&&) noexcept = default;
    CScsi& operator=(CScsi&&) noexcept = default;

    void SetMem(int nMem);
    [[nodiscard]] std::array<unsigned char, INQUIRY_REPLY_LEN> Inquiry();
    [[nodiscard]] int Open(const std::string& device);
    void CloseDevice();
    [[nodiscard]] int TestUnitReady();
    [[nodiscard]] int Write(std::span<const BYTE> pBuf, int nSectNum, int nSectCount);
    [[nodiscard]] int Read(std::span<BYTE> pBuf, int nSectNum, int nSectCount);
    [[nodiscard]] int Disconnect();
    [[nodiscard]] int Switch();
    [[nodiscard]] int Reset();

private:
    [[nodiscard]] int handle_SCSI_cmd(unsigned cmd_len, unsigned in_size, std::span<const unsigned char> i_buff,
                                      unsigned out_size, std::span<unsigned char> o_buff);

    std::array<unsigned char, SCSI_OFF + 18> cmd;
    int fd;
    std::string m_szDevice;
    int m_nMem;
};

#endif // AFX_SCSI_H__701A166F_809B_47D6_B487_1907DC8ECCCE__INCLUDED_

//BY GST ARMV8 GCC 13.2