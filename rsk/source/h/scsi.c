#include "scsi.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/sg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// 初始化CScsi对象
void scsi_init(CScsi* scsi) {
    memset(scsi->cmd, 0, sizeof(scsi->cmd));
    scsi->fd = -1;
    scsi->m_nMem = 0;
    memset(scsi->m_szDevice, 0, sizeof(scsi->m_szDevice));
}

// 设置内存
void scsi_set_mem(CScsi* scsi, int nMem) {
    scsi->m_nMem = nMem;
}

// 打开设备
int scsi_open(CScsi* scsi, const char* device) {
    strncpy(scsi->m_szDevice, device, sizeof(scsi->m_szDevice) - 1);
    scsi->fd = open(device, O_RDWR);
    if (scsi->fd < 0) {
        perror("Failed to open SCSI device");
        return -1;
    }
    return 0;
}

// 关闭设备
void scsi_close_device(CScsi* scsi) {
    if (scsi->fd != -1) {
        close(scsi->fd);
        scsi->fd = -1;
    }
}

// 测试设备是否准备好
int scsi_test_unit_ready(CScsi* scsi) {
    memset(scsi->cmd, 0, sizeof(scsi->cmd));
    scsi->cmd[0] = TESTUNITREADY_CMD;
    return scsi_handle_cmd(scsi, TESTUNITREADY_CMDLEN, 0, NULL, 0, NULL);
}

// 写数据
int scsi_write(CScsi* scsi, const uint8_t* pBuf, int nSectNum, int nSectCount) {
    memset(scsi->cmd, 0, sizeof(scsi->cmd));
    scsi->cmd[0] = SCSI_Cmd_Write;
    scsi->cmd[2] = (nSectNum >> 24) & 0xFF;
    scsi->cmd[3] = (nSectNum >> 16) & 0xFF;
    scsi->cmd[4] = (nSectNum >> 8) & 0xFF;
    scsi->cmd[5] = nSectNum & 0xFF;
    scsi->cmd[8] = nSectCount;

    return scsi_handle_cmd(scsi, SWITCH_CMDLEN, nSectCount * 512, pBuf, 0, NULL);
}

// 读数据
int scsi_read(CScsi* scsi, uint8_t* pBuf, int nSectNum, int nSectCount) {
    memset(scsi->cmd, 0, sizeof(scsi->cmd));
    scsi->cmd[0] = SCSI_Cmd_Read;
    scsi->cmd[2] = (nSectNum >> 24) & 0xFF;
    scsi->cmd[3] = (nSectNum >> 16) & 0xFF;
    scsi->cmd[4] = (nSectNum >> 8) & 0xFF;
    scsi->cmd[5] = nSectNum & 0xFF;
    scsi->cmd[8] = nSectCount;

    return scsi_handle_cmd(scsi, SWITCH_CMDLEN, 0, NULL, nSectCount * 512, pBuf);
}

// 断开连接
int scsi_disconnect(CScsi* scsi) {
    if (scsi->fd != -1) {
        int result = close(scsi->fd);
        scsi->fd = -1;
        return result;
    }
    return 0;
}

// 切换
int scsi_switch(CScsi* scsi) {
    memset(scsi->cmd, 0, sizeof(scsi->cmd));
    scsi->cmd[0] = SWITCH_CMD;
    return scsi_handle_cmd(scsi, SWITCH_CMDLEN, 0, NULL, 0, NULL);
}

// 重置
int scsi_reset(CScsi* scsi) {
    if (scsi->fd == -1) {
        perror("Device not opened");
        return -1;
    }
    return ioctl(scsi->fd, SG_SCSI_RESET, NULL);
}

// 处理SCSI命令
int scsi_handle_cmd(CScsi* scsi, unsigned cmd_len, unsigned in_size, const uint8_t* i_buff, unsigned out_size, uint8_t* o_buff) {
    if (scsi->fd == -1) {
        perror("Device not opened");
        return -1;
    }

    sg_io_hdr_t io_hdr;
    memset(&io_hdr, 0, sizeof(sg_io_hdr_t));

    io_hdr.interface_id = 'S';
    io_hdr.cmd_len = cmd_len;
    io_hdr.mx_sb_len = 0;
    io_hdr.dxfer_direction = out_size ? SG_DXFER_FROM_DEV : SG_DXFER_TO_DEV;
    io_hdr.dxfer_len = out_size ? out_size : in_size;
    io_hdr.dxferp = out_size ? o_buff : (uint8_t*)i_buff;
    io_hdr.cmdp = scsi->cmd;
    io_hdr.timeout = 5000;

    int result = ioctl(scsi->fd, SG_IO, &io_hdr);
    if (result < 0) {
        perror("SCSI command failed");
        return -1;
    }
    return result;
}

// 查询
void scsi_inquiry(CScsi* scsi, uint8_t* buffer) {
    memset(scsi->cmd, 0, sizeof(scsi->cmd));
    scsi->cmd[0] = INQUIRY_CMD;
    scsi->cmd[4] = INQUIRY_REPLY_LEN;

    int status = scsi_handle_cmd(scsi, INQUIRY_CMDLEN, 0, NULL, INQUIRY_REPLY_LEN, buffer);
    if (status < 0) {
        perror("SCSI Inquiry command failed");
    }
}