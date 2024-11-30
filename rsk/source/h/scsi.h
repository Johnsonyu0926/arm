#ifndef SCSI_H
#define SCSI_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define INQUIRY_REPLY_LEN 96
#define TESTUNITREADY_CMD 0x00
#define TESTUNITREADY_CMDLEN 6
#define INQUIRY_CMD 0x12
#define INQUIRY_CMDLEN 6
#define SCSI_Cmd_Read 0x08
#define SCSI_Cmd_Write 0x0A
#define SWITCH_CMD 0x17
#define SWITCH_CMDLEN 6
#define SCSI_OFF 36
#define DEVICE "/dev/sg0"

typedef struct {
    char m_szVersion[16];
    int m_nBlockCount;
    uint8_t m_pBufRev[492];
} CHeadBlock;

typedef struct {
    uint8_t cmd[SCSI_OFF + 18];
    int fd;
    char m_szDevice[256];
    int m_nMem;
} CScsi;

void scsi_init(CScsi* scsi);
void scsi_set_mem(CScsi* scsi, int nMem);
int scsi_open(CScsi* scsi, const char* device);
void scsi_close_device(CScsi* scsi);
int scsi_test_unit_ready(CScsi* scsi);
int scsi_write(CScsi* scsi, const uint8_t* pBuf, int nSectNum, int nSectCount);
int scsi_read(CScsi* scsi, uint8_t* pBuf, int nSectNum, int nSectCount);
int scsi_disconnect(CScsi* scsi);
int scsi_switch(CScsi* scsi);
int scsi_reset(CScsi* scsi);
int scsi_handle_cmd(CScsi* scsi, unsigned cmd_len, unsigned in_size, const uint8_t* i_buff, unsigned out_size, uint8_t* o_buff);
void scsi_inquiry(CScsi* scsi, uint8_t* buffer);

#endif // SCSI_H