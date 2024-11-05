// scsi.cpp
#include "scsi.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/sg.h>
#include <algorithm>
#include <stdexcept>

CScsi::CScsi() : fd(-1), m_nMem(0) {
    cmd.fill(0);
}

CScsi::~CScsi() {
    CloseDevice();
}

void CScsi::SetMem(int nMem) {
    m_nMem = nMem;
}

std::array<unsigned char, INQUIRY_REPLY_LEN> CScsi::Inquiry() {
    cmd.fill(0);
    cmd[0] = INQUIRY_CMD;
    cmd[4] = INQUIRY_REPLY_LEN;

    std::array<unsigned char, INQUIRY_REPLY_LEN> buffer{};
    int status = handle_SCSI_cmd(INQUIRY_CMDLEN, 0, {}, INQUIRY_REPLY_LEN, buffer);
    if (status < 0) {
        throw std::runtime_error("SCSI Inquiry command failed");
    }
    return buffer;
}

int CScsi::Open(const std::string& device) {
    m_szDevice = device;
    fd = open(device.c_str(), O_RDWR);
    if (fd < 0) {
        throw std::runtime_error("Failed to open SCSI device");
    }
    return 0;
}

void CScsi::CloseDevice() {
    if (fd != -1) {
        close(fd);
        fd = -1;
    }
}

int CScsi::TestUnitReady() {
    cmd.fill(0);
    cmd[0] = TESTUNITREADY_CMD;
    return handle_SCSI_cmd(TESTUNITREADY_CMDLEN, 0, {}, 0, {});
}

int CScsi::Write(std::span<const BYTE> pBuf, int nSectNum, int nSectCount) {
    cmd.fill(0);
    cmd[0] = SCSI_Cmd_Write;
    cmd[2] = (nSectNum >> 24) & 0xFF;
    cmd[3] = (nSectNum >> 16) & 0xFF;
    cmd[4] = (nSectNum >> 8) & 0xFF;
    cmd[5] = nSectNum & 0xFF;
    cmd[8] = nSectCount;

    return handle_SCSI_cmd(SWITCH_CMDLEN, nSectCount * 512, pBuf, 0, {});
}

int CScsi::Read(std::span<BYTE> pBuf, int nSectNum, int nSectCount) {
    cmd.fill(0);
    cmd[0] = SCSI_Cmd_Read;
    cmd[2] = (nSectNum >> 24) & 0xFF;
    cmd[3] = (nSectNum >> 16) & 0xFF;
    cmd[4] = (nSectNum >> 8) & 0xFF;
    cmd[5] = nSectNum & 0xFF;
    cmd[8] = nSectCount;

    return handle_SCSI_cmd(SWITCH_CMDLEN, 0, {}, nSectCount * 512, pBuf);
}

int CScsi::Disconnect() {
    if (fd != -1) {
        int result = close(fd);
        fd = -1;
        return result;
    }
    return 0;
}

int CScsi::Switch() {
    cmd.fill(0);
    cmd[0] = SWITCH_CMD;
    return handle_SCSI_cmd(SWITCH_CMDLEN, 0, {}, 0, {});
}

int CScsi::Reset() {
    if (fd == -1) {
        throw std::runtime_error("Device not opened");
    }
    return ioctl(fd, SG_SCSI_RESET, nullptr);
}

int CScsi::handle_SCSI_cmd(unsigned cmd_len, unsigned in_size, std::span<const unsigned char> i_buff,
                           unsigned out_size, std::span<unsigned char> o_buff) {
    if (fd == -1) {
        throw std::runtime_error("Device not opened");
    }

    sg_io_hdr_t io_hdr{};

    io_hdr.interface_id = 'S';
    io_hdr.cmd_len = cmd_len;
    io_hdr.mx_sb_len = 0;
    io_hdr.dxfer_direction = out_size ? SG_DXFER_FROM_DEV : SG_DXFER_TO_DEV;
    io_hdr.dxfer_len = out_size ? out_size : in_size;
    io_hdr.dxferp = out_size ? o_buff.data() : const_cast<unsigned char*>(i_buff.data());
    io_hdr.cmdp = cmd.data();
    io_hdr.timeout = 5000;

    int result = ioctl(fd, SG_IO, &io_hdr);
    if (result < 0) {
        throw std::runtime_error("SCSI command failed");
    }
    return result;
}

//BY GST ARMV8 GCC 13.2