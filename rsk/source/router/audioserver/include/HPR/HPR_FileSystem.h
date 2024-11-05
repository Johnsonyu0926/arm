#ifndef __HPR_FS_H__
#define __HPR_FS_H__

#include "HPR_Config.h"
#include "HPR_Types.h"
#include "HPR_Time.h"

#if defined(OS_WINDOWS)
    #if !defined OS_WINCE
        #include <direct.h>
    #endif
    typedef PSID HPR_UID_T;
    typedef PSID HPR_GID_T;
    typedef DWORD HPR_FILETYPE_T;
    typedef HPR_INT64 HPR_INODE_T;
    typedef HPR_INT64 HPR_SIZE_T;
    #define HPR_INVALID_FILE INVALID_HANDLE_VALUE
#elif defined(OS_POSIX)
    typedef uid_t HPR_UID_T;
    typedef gid_t HPR_GID_T;
    typedef mode_t HPR_FILETYPE_T;
    typedef HPR_INT64 HPR_INODE_T;
    typedef HPR_INT64 HPR_SIZE_T;
    #define HPR_INVALID_FILE (HPR_HANDLE)-1 
#else
    #error OS Not Implement Yet.
#endif

#define HPR_MAX_PATH 260

#define HPR_READ 0x0001
#define HPR_WRITE 0x0002
#define HPR_CREATE 0x0004
#define HPR_APPEND 0x0008
#define HPR_TRUNCATE 0x0010
#define HPR_BINARY 0x0020
#define HPR_EXCL 0x0040
#define HPR_BUFFERED 0x0080
#define HPR_SHARELOCK 0x0100
#define HPR_XTHREAD 0x0200
#define HPR_LARGEFILE 0x0400
#define HPR_DIRECT 0x0800

#define HPR_ATTR_READONLY 0x1000
#define HPR_ATTR_WRITE 0x2000
#define HPR_ATTR_EXECUTABLE 0x4000
#define HPR_ATTR_HIDDEN 0x8000
#define HPR_ATTR_SYSTEM 0x0100
#define HPR_DELONCLOSE 0x0200

#define HPR_USETID 0x8000
#define HPR_UREAD 0x0400
#define HPR_UWRITE 0x0200
#define HPR_UEXECUTE 0x0100

#define HPR_GSETID 0x4000
#define HPR_GREAD 0x0040
#define HPR_GWRITE 0x0020
#define HPR_GEXECUTE 0x0010

#define HPR_WSTICKY 0x2000
#define HPR_WREAD 0x0004
#define HPR_WWRITE 0x0002
#define HPR_WEXECUTE 0x0001

#define HPR_OS_DEFAULT 0x0FFF

#define HPR_TYPE_CHAR 0x0001
#define HPR_TYPE_DISK 0x0002
#define HPR_TYPE_PIPE 0x0004
#define HPR_TYPE_UNKNOWN 0x0008
#define HPR_TYPE_SOCK 0x0010
#define HPR_TYPE_FIFO 0x0020
#define HPR_TYPE_LINK 0x0040
#define HPR_TYPE_BLK 0x0080
#define HPR_TYPE_DIR 0x0100
#define HPR_TYPE_REG 0x0200

#define HPR_SEEK_SET SEEK_SET
#define HPR_SEEK_CUR SEEK_CUR
#define HPR_SEEK_END SEEK_END

typedef struct HPR_FINFO_S {
    HPR_FILETYPE_T nFileType;
    HPR_UID_T vUID;
    HPR_GID_T vGID;
    HPR_UINT32 nProtection;
    HPR_UINT32 nDeviceID;
    HPR_UINT32 nHardLink;
    HPR_INODE_T iINode;
    HPR_SIZE_T nSize;
    HPR_TIME_T iAccessTime;
    HPR_TIME_T iCreateTime;
    HPR_TIME_T iWriteTime;
} HPR_FINFO_T;

typedef struct _FILE_FIND_INFO {
    char sFileName[HPR_MAX_PATH];
    HPR_FINFO_T FileInfo;
} HPR_FILEFIND_INFO;

HPR_DECLARE HPR_INT32 CALLBACK HPR_MakeDir(const char* pDir);
HPR_DECLARE HPR_INT32 CALLBACK HPR_RemoveDir(const char* pDir);
HPR_DECLARE HPR_HANDLE CALLBACK HPR_OpenDir(const char *pDirName);
HPR_DECLARE HPR_INT32 CALLBACK HPR_FindFileInDir(HPR_HANDLE hDir, HPR_FILEFIND_INFO *pFileInfo);
HPR_DECLARE HPR_INT32 CALLBACK HPR_CloseDir(HPR_HANDLE hDir);

HPR_DECLARE HPR_HANDLE CALLBACK HPR_OpenFile(const char* pFileName, HPR_UINT32 nFlag, HPR_UINT32 nFileAttr);
HPR_DECLARE HPR_INT32 CALLBACK HPR_CloseFile(HPR_HANDLE hFile);
HPR_DECLARE HPR_INT32 CALLBACK HPR_DeleteFile(const char* pFileName);
HPR_DECLARE HPR_INT32 CALLBACK HPR_ReadFile(HPR_HANDLE hFile, void* pBuf, HPR_UINT32 nNumberOfBytesToRead, HPR_UINT32* pNumberOfBytesRead);
HPR_DECLARE HPR_INT32 CALLBACK HPR_WriteFile(HPR_HANDLE hFile, void* pBuf, HPR_UINT32 nNumberOfBytesToWrite, HPR_UINT32* pNumberOfBytesWrite);
HPR_DECLARE HPR_INT32 CALLBACK HPR_FileSeek(HPR_HANDLE hFile, HPR_INT64 iOffset, HPR_UINT32 nWhence, HPR_INT64* iCurOffset);
HPR_DECLARE HPR_BOOL CALLBACK HPR_FileIsEOF(HPR_HANDLE hFile);
HPR_DECLARE HPR_INT32 CALLBACK HPR_FileStat(HPR_HANDLE hFile, HPR_FINFO_T* pFInfo);
HPR_DECLARE HPR_INT32 CALLBACK HPR_FileFlush(HPR_HANDLE hFile);
HPR_DECLARE HPR_INT32 CALLBACK HPR_GetCurExePath(char* buf, HPR_INT32 size);

#define HPR_R_OK 4
#define HPR_W_OK 2
#define HPR_X_OK 1
#define HPR_F_OK 0
HPR_DECLARE HPR_INT32 CALLBACK HPR_AccessFile(const char *path, int mode);
HPR_DECLARE HPR_INT32 CALLBACK HPR_CopyFile(const char* src, const char* dst, HPR_BOOL bFailIfExists);
HPR_DECLARE HPR_INT32 CALLBACK HPR_RenameFile(const char* oldName, const char* newName);

#endif // __HPR_FS_H__
