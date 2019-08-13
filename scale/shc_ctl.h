/*************************************************************************
* Copyright (c) 2010,深圳市YHF电气技术有限公司
* All rights reserved.

**************************************************************************/

/*!	\file
 	\brief		稳态录波控制
 	
	\author		Chen
	\version	1.0
	\date		2010-03-18
	
*/

#ifndef	__SH_SHC_CONTROL_DEFINE_H
#define	__SH_SHC_CONTROL_DEFINE_H

#include "shd_def.h"
#include "rcd_ctl.h"
//#include "sg_def.h"

#if defined( WIN32 )
#define		CONT_TEMP_DIR			"temp_cont"
#define		CONF_FNAME_SHC_TEMP		"dfrserver/conf/shctemp.conf"
#else
#define		CONT_TEMP_DIR			"/temp_cont"
#define		CONF_FNAME_SHC_TEMP		"/dfrserver/conf/shctemp.conf"
#endif


typedef int (*FN_SHC_PROC)(struct mk_shc_ctl_t*);

/*!
 	\brief	稳态录波控制结构

	本结构用于生成原始录波数据SHC文件
 */
typedef struct mk_shc_ctl_t 
{
	/*以下属性项与 SHC_INFO结构完全一致，以方便处理 */
	SHDFR_SG			DfrSg;						/*!< 定值信息 */
	INT8U*				BufAddr;					/*!< 缓冲区地址 */
	INT32				BufLen;						/*!< 缓冲区长度 */
	INT32				DataLen;					/*!< 缓冲区中有效数据的长度  */
	FRM_HEAD_CONT* 		FrmData[1024];				/*!< 录波数据子帧 */
	INT32				ShcFrmCnt;					/*!< 录波数据子帧的个数(当前帧序号) */
	TRG_INFO*			TrgInfo;					/*!< 启动信息 */
	INT8U*				SgDataAddr;					/*!< 序列化的定值数据区起始地址 */
	INT32				SgDataLen;					/*!< 序列化的定值数据区长度 */
	
	INT32				AnChCnt;					/*!< 模拟量通道数 */
	INT32				BinChCnt;					/*!< 开关量通道数 */
	INT32				SmpLenPerPnt;				/*!< 一个原始采样段的数据点的长度 */
	INT32				CompLenPerPnt;				/*!< 一个复数段的数据点的长度 */
	INT32				AnSmpLenPerPnt;				/*!< 一个模拟量采样数据点的长度 */
	INT32				AnCompLenPerPnt;			/*!< 一个模拟量复数采样数据点的长度 */
	INT32				BinLenPerPnt;				/*!< 一个开关量采样数据点的长度 */
	
	INT32				FltNum;						/*!< 故障序号 */
	INT32				FlgFWriteShc;				/*!< 标志是否需要写SHC原始文件 */
	char				ShcFileName[128];			/*!< SHC文件名(不包含路径及扩展名) */
	char				ShcFilePath[256];			/*!< SHC文件全路径 */
	FILE*				ShcFp;						/*!< SHC文件对象指针 */
	INT32				ShcFWriteLen;				/*!< 已经写入SHC文件的数据的长度 */

	INT32				ShcSgType;					/*!< 生成SHC文件时，保存的定值的格式 */

	FN_SHC_PROC			PtrRcdProc;					/*!< 录波过程中的处理函数 */
}MK_SHC_CTL;


#endif //__SH_SHC_CONTROL_DEFINE_H
