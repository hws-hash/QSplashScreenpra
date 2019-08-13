/*************************************************************************
* Copyright (c) 2011,深圳市YHF电气技术有限公司
* All rights reserved.

**************************************************************************/

/*!	\file
 	\brief		
 	
	\author		chentm
	\version	1.0
	\date		2011-03-18
	
*/

#ifndef	__YH_SHD_CONTROL_DEFINE_H
#define	__YH_SHD_CONTROL_DEFINE_H

#include "shd_def.h"
#include "rcd_ctl.h"
#include "../scale/sg_def.h"

#ifdef __cplusplus
extern "C" {
#endif


#define MAX_SHD_FILE_LEN	50*1024*1024


typedef int (*FN_SHD_PROC)(struct mk_shd_ctl_t*);

/*!
 	\brief	生成SHD的控制结构

	本结构用于生成原始录波数据SHD文件
 */
typedef struct mk_shd_ctl_t 
{
	/*以下属性项与 SHD_INFO结构完全一致，以方便处理 */
//	SHDFR_SG			DfrSg;						/*!< 定值信息 */
	INT8U*				BufAddr;					/*!< 缓冲区地址 */
	INT32				BufLen;						/*!< 缓冲区长度 */
	INT32				DataLen;					/*!< 缓冲区中有效数据的长度  */
	FRM_HEAD_TRNS		*FrmHead;					/*!< 录波帧头 */
	FRM_HEAD_TRNS_DATA* FrmData[MAX_SHD_FRM_NUM];	/*!< 录波数据子帧 */
	INT32				ShdFrmCnt;					/*!< 录波数据子帧的个数(当前帧序号) */
	FRM_HEAD_TRNS_DATA* FirstA2Frm;					/*!< 第一个A2帧 */
	FRM_HEAD_TRNS_DATA* FirstA1Frm;					/*!< 第一个A1帧 */		
	FRM_HEAD_TRNS_DATA* FirstBFrm;					/*!< 第一个B帧 */
	TRG_INFO*			TrgInfo;					/*!< 启动信息 */
	//INT8U*			SgDataAddr;					/*!< 序列化的定值数据区起始地址 */	//ctm
	//INT32				SgDataLen;					/*!< 序列化的定值数据区长度 */		//ctm 
	
	INT32				AnChCnt;					/*!< 模拟量通道数 */
	INT32				BinChCnt;					/*!< 开关量通道数 */
	INT32				SmpLenPerPnt;				/*!< 一个原始采样段的数据点的长度 */
	INT32				CompLenPerPnt;				/*!< 一个复数段的数据点的长度 */
	INT32				AnSmpLenPerPnt;				/*!< 一个模拟量采样数据点的长度 */
	INT32				AnCompLenPerPnt;			/*!< 一个模拟量复数采样数据点的长度 */
	INT32				BinLenPerPnt;				/*!< 一个开关量采样数据点的长度 */

	/*
	对于每周波的采样点数，做如下约定：
	>0时，表示为原始采样
	<1时，表示为复数采样，其绝对值表示每几个周波采一个复数数据点
	*/
	INT32				A2SmpPntPerCyc;				/*!< A2段每周波采样点数 */
	INT32				A1SmpPntPerCyc;				/*!< A1段每周波采样点数 */
	INT32				BSmpPntPerCyc;				/*!< B段每周波采样点数 */
	INT32				CSmpPntPerCyc;				/*!< C段每周波采样点数 */
	INT32				DSmpPntPerCyc;				/*!< D段每周波采样点数 */

//	YH_TRNS_RCD_SET		Setting;					/*!< 定值信息 */

	INT32				Status;						/*!< 状态 */
	FAULT_INFO* 		CurFltInfo;					/*!< 当前正在处理的故障信息 */
	FAULT_SEG_INFO*		CurFltSeg;					/*!< 当前正在处理的故障信息段 */

	INT32				ShdFltCycCnt;				/*!< 本次故障已经处理过的周波数 */
	INT32				ShdSegCycCnt;				/*!< 本段内已经处理的总的周波数 */
	
	INT32				FltNum;						/*!< 故障序号 */
	INT32				FlgFWriteShd;				/*!< 标志是否需要写SHD原始文件 */
	char				ShdFileName[128];			/*!< SHD文件名(不包含路径及扩展名) */
	char				ShdFilePath[256];			/*!< SHD文件全路径 */
	FILE*				ShdFp;						/*!< SHD文件对象指针 */
	INT32				ShdFWriteLen;				/*!< 已经写入SHD文件的数据的长度 */

	//INT32				ShdSgType;					/*!< 生成SHD文件时，保存的定值的格式 */

	FN_SHD_PROC			PtrStartProc;				/*!< 录波开始时的处理函数 */
	FN_SHD_PROC			PtrRcdProc;					/*!< 录波过程中的处理函数 */
	FN_SHD_PROC			PtrStopProc;				/*!< 录波结束时的处理函数 */
}MK_SHD_CTL;


/*!
 	\brief	SHD控制结构的创建
 	\return	成功返回创建的SHD控制结构的指针，否则返回NULL
 */
MK_SHD_CTL* shd_ctl_create( int buf_len );

/*!
 	\brief	SHD控制结构的初始化
 	\return	成功返回0，否则返回非0错误号
 */
//int shd_ctl_init( MK_SHD_CTL* pShdCtl, SHDFR_SG* psg, FAULT_INFO* flt_info, void* sgdata, int data_len );

/*!
 	\brief	SHD控制结构的销毁
 	\return	成功返回0，否则返回非0错误号
 */
int shd_ctl_destory( MK_SHD_CTL* pShdCtl );


#ifdef __cplusplus
}
#endif

#endif	//__YH_SHD_CONTROL_DEFINE_H
