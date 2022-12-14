/*
 * $Id: scsi.h,v 1.28 2004/11/25 11:12:06 billy Exp $
 *
 * Author: Billy Shi (dxshi@yahoo.com)
 *
 * Copyright(C) 2001-2010 Doors Internet Software Co.,Ltd. 
 *
 */
// Scsi.h: interface for the CScsi class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCSI_H__701A166F_809B_47D6_B487_1907DC8ECCCE__INCLUDED_)
#define AFX_SCSI_H__701A166F_809B_47D6_B487_1907DC8ECCCE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string.h>
#include "doorsdefs.h"
#include "doorsbase.h"
//TestUnit Ready  
#define TESTUNITREADY_CMD 0
#define TESTUNITREADY_CMDLEN 6

#define ADD_SENSECODE 12
#define ADD_SC_QUALIFIER 13
#define NO_MEDIA_SC 0x3a
#define NO_MEDIA_SCQ 0x00


#define INQUIRY_CMD     0x12
#define INQUIRY_CMDLEN  6
#define INQUIRY_REPLY_LEN 96
#define INQUIRY_VENDOR  8       /* Offset in reply data to vendor name */
#define SCSI_OFF 36//sizeof(struct sg_header)

#define DEVICE "/dev/sg0"

#define SWITCH_CMD	0x17
#define SWITCH_CMDLEN	6
//added by billy.

#ifndef BYTE
#define BYTE unsigned char
#endif
#ifndef IN
#define IN
#define OUT
#endif

/*
 * SCSI Write
 */

typedef struct SCSI_Cdb_Read_s {
   unsigned int CommandCode      : 8;  /* should be 0x08 */
   unsigned int LBA2             : 5;
   unsigned int Lun              : 3;
   unsigned int LBA1             : 8;
   unsigned int LBA0             : 8;
   unsigned int Length           : 8;
   unsigned int Link             : 1;
   unsigned int Flag             : 1;
   unsigned int                  : 4;
   unsigned int VendorSpecific   : 2;
   } SCSI_Cdb_Read_t;

typedef struct SCSI_Cdb_Write_s {
   unsigned int CommandCode         : 8;  /* should be 0x08 */
   unsigned int LBA2             : 5;
   unsigned int Lun              : 3;
   unsigned int LBA1             : 8;
   unsigned int LBA0             : 8;
   unsigned int Length           : 8;
   unsigned int Link             : 1;
   unsigned int Flag             : 1;
   unsigned int                  : 4;
   unsigned int VendorSpecific         : 2;
   } SCSI_Cdb_Write_t;

#define SCSI_Cmd_Read				0x08	//--> 0x28h READ(10) modified by billy
#define SCSI_Cmd_Write				0x0A	//--> 0x2ah WRITE(10)


#define VERSON	"Prospect EGAP 1"

class CHeadBlock
{
public:
	char 	m_szVerson[16]; //Prospect EGAP 1
	int	m_nBlockCount;	
	BYTE	m_pBufRev[492];
public:
	CHeadBlock()
	{
		strcpy(m_szVerson,VERSON);
		memset(m_pBufRev,0,492);
	}
	~CHeadBlock(){}
};
class CScsi  
{
public:
	CScsi();
	void SetMem(int nMem);
	virtual ~CScsi();
	unsigned char * Inquiry ( void );
	int	Open(char* szDevice);
	void 	CloseDevice();
	int 	TestUnitReady();
	int	Write(BYTE* pBuf,IN int nSectNum,IN int nSectCount);	//return the bytes
	int Read(OUT BYTE* pBuf,IN int nSectNum,IN int nSectCount);

	int 	Disconnect();
	int 	Switch();
	int 	Reset();
private:
	int handle_SCSI_cmd(unsigned cmd_len,         /* command length */
	  unsigned in_size,         /* input data size */
	  unsigned char *i_buff,    /* input buffer */
	  unsigned out_size,        /* output data size */
	  unsigned char *o_buff     /* output buffer */
	  );

	unsigned char cmd[SCSI_OFF + 18];      /* SCSI command buffer */
	int fd;                               /* SCSI device/file descriptor */

	char m_szDevice[128];
	int	m_nMem;
};

#endif // !defined(AFX_SCSI_H__701A166F_809B_47D6_B487_1907DC8ECCCE__INCLUDED_)
