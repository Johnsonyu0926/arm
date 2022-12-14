/**
****************************************************************************************************
* 
* 版权信息：版权所有 (c) 2010, 杭州海康威视数字技术有限公司, 保留所有权利
* 
* 文件名称：mem_tab.h
* 文件标识：_MEM_TAB_H_
* 摘    要：海康威视 规范内存特征定义 规范文件
*
* 当前版本：1.0.0
* 作    者：俞海 
* 日    期：2010年07月14日
* 备    注：规范所需内存特征定义
*
* 当前版本：1.1.0
* 作    者：武晓阳 
* 日    期：2013年05月27日
* 备    注：根据前端、后端现在使用的情况进行修改
*           1. 去掉3个变量结构体，只采用5个变量的结构体；
*           2. 5个结构体的*base变量在最前面；
*           3. MEM_SPACE增加TILER结构
****************************************************************************************************
*/
#ifndef _HIK_MEM_TAB_H_
#define _HIK_MEM_TAB_H_


#ifdef __cplusplus
extern "C" {
#endif


/**
****************************************************************************************************
* 结构名：MEM_ALIGNMENT
* 说  明：内存对齐属性
* 备 注：
****************************************************************************************************
*/
typedef enum _MEM_ALIGNMENT
{
    MEM_ALIGN_4BYTE     =  4,     // 4 Byte对齐
    MEM_ALIGN_8BYTE     =  8,     // 8 Byte对齐 
    MEM_ALIGN_16BYTE    =  16,    // 16 Byte对齐
    MEM_ALIGN_32BYTE    =  32,    // 32 Byte对齐
    MEM_ALIGN_64BYTE    =  64,    // 64 Byte对齐
    MEM_ALIGN_128BYTE   =  128,   // 128 Byte对齐
    MEM_ALIGN_256BYTE   =  256    // 256 Byte对齐

} MEM_ALIGNMENT;


/**
****************************************************************************************************
* 结构名：MEM_ATTRS
* 说  明：内存属性
* 备 注：
****************************************************************************************************
*/
typedef enum _MEM_ATTRS
{
    MEM_SCRATCH,      // 可复用内存，能在多路切换时有条件复用
    MEM_PERSIST       // 不可复用内存

} MEM_ATTRS;


/**
****************************************************************************************************
* 结构名：MEM_SPACE
* 说  明：内存分配空间
* 备 注：
****************************************************************************************************
*/
typedef enum _MEM_SPACE 
{
    MEM_EXTERNAL_PROG,            // 外部程序存储区
    MEM_INTERNAL_PROG,            // 内部程序存储区

    MEM_EXTERNAL_CACHED_DATA,     // 外部可Cache存储区
    MEM_EXTERNAL_UNCACHED_DATA,   // 外部不可Cache存储区
    MEM_INTERNAL_DATA,            // 内部存储区
    MEM_EXTERNAL_TILERED8 ,       // 外部Tilered数据存储区8bit，Netra/Centaurus特有
    MEM_EXTERNAL_TILERED16,       // 外部Tilered数据存储区16bit，Netra/Centaurus特有
    MEM_EXTERNAL_TILERED32 ,      // 外部Tilered数据存储区32bit，Netra/Centaurus特有
    MEM_EXTERNAL_TILEREDPAGE      // 外部Tilered数据存储区page形式，Netra/Centaurus特有

} MEM_SPACE;


/**
****************************************************************************************************
* 结构名：MEM_TAB
* 说  明：内存分配结构体
* 备 注：
****************************************************************************************************
*/
typedef struct _MEM_TAB
{
    void          *base;       // 分配出的内存指针
    unsigned int  size;       // 以BYTE为单位的内存大小
    unsigned int  alignment;  // 内存对齐属性, 建议为128
    MEM_SPACE     space;      // 内存分配空间
    MEM_ATTRS     attrs;      // 内存属性

} MEM_TAB;


#ifdef __cplusplus
}
#endif

#endif //_HIK_MEM_TAB_H_

