/*************************************************************************
* Copyright (c) 2008,深圳市YHF电气技术有限公司
* All rights reserved.

**************************************************************************/

/*! \file
    \brief      定义SHD原始录波文件相关数据结构
    \author     chen
    \version    1.0
    \date       2010-03-25
*/

#ifndef __SH_DFR_STRUCT_DEFINE_H
#define __SH_DFR_STRUCT_DEFINE_H

#include <stdio.h>
#include "yhftypes.h"
//#include "sg_def.h"
#include "calc_def.h"
#include "trg_def.h"

#define	SHD_FRM_TYPE_RT_DATA		0x33333333		/*!< 实时数据帧 */
#define	SHD_FRM_TYPE_TRNS_HEAD		0x44444444		/*!< 暂态数据帧 */
#define	SHD_FRM_TYPE_TRNS_DATA		0x55555555		/*!< 暂态数据子帧 */
#define	SHD_FRM_TYPE_CONT_DATA		0x66666666		/*!< 稳态数据帧 */

#ifdef __cplusplus
extern "C" {
#endif


/* 本文件中定义的所有结构体均按1字节对齐 */
#pragma pack (1)

/*
    FPGA中原始采样数据点数据排列如下：
    序号    长度（字节）                说明
    1       sizeof(SMP_TIMESTAMP)=16    采样时标
    2       2*32                        1号模拟量板原始采样数据
    3       2*32                        2号模拟量板原始采样数据（当模拟量板数量>=2时出现）
    4       2*32                        3号模拟量板原始采样数据（当模拟量板数量>=3时出现）
    5       8                           1号开关量板状态（每一位表示一个开关量的状态）
    6       8                           2号开关量板状态（每一位表示一个开关量的状态）
    7       8                           3号开关量板状态（每一位表示一个开关量的状态）
    8       2                           采样脉冲计数器
    9       2                           采样点序号高位
    10      2                           采样点序号低位
    11      2*2                         1号模拟量板33、34采样值
    12      2*2                         2号模拟量板33、34采样值
    13      2*2                         3号模拟量板33、34采样值
    14      6(96路)|10(64路)|14(32路)   预留，保证FPGA数据长度为64的整数倍
*/


/*!
    \brief  实时数据帧头
    
 */
typedef struct
{
    INT32U       FrmType;        /*!< 帧类型标识，实时数据帧头约定为SHD_FRM_TYPE_RT_DATA */
    INT32U       FrmLen;         /*!< 帧长度，此长度指的是数据长度，不包括帧头及帧校验和 */
    INT32U       FrmIdx;         /*!< 帧序号，第一帧从1开始记数，每发送一帧，帧序应加1 */
    
    INT16U       AnChCnt;        /*!< 模拟量通道数 */
    INT16U       BinChCnt;       /*!< 状态量通道数 */
    
    INT16U       ULinCnt;        /*!< 电压数 */
    INT16U       ILinCnt;        /*!< 电流数 */
    INT16U       TrnCnt;         /*!< 变压器数 */
    INT16U       GnCnt;          /*!< 发电机数 */

    INT16U       SmpRate;        /*!< 采样率，每周波采样点数 */
    INT16U       CycleCnt;       /*!< 周波总数（为5的整倍数）*/
    INT16U       CalDataLen;     /*!< 计算数据的长度 */
    INT16U       Resv[1];        /*!< 预留，数据帧头共32字节　*/
} FRM_HEAD_RT;


/*!
    \brief  稳态数据帧头
    
 */
typedef struct 
{
    INT32U       FrmType;        /*!< 帧类型标识，稳态帧类型应为SHD_FRM_TYPE_CONT_DATA */
    INT32U       FrmLen;         /*!< 帧长度，此长度指的是数据长度，不包括帧头及帧校验和 */
    INT32U       FrmIdx;         /*!< 帧序号，第一帧从1开始记数，每发送一帧，帧序应加 */

    INT16U       AnChCnt;        /*!< 模拟量通道数 */
    INT16U       BinChCnt;       /*!< 状态量通道数 */
    
    INT16U       ULinCnt;        /*!< 电压数 */
    INT16U       ILinCnt;        /*!< 电流数 */
    INT16U       TrnCnt;         /*!< 变压器数 */
    INT16U       GnCnt;          /*!< 发电机数 */

    INT16U       SmpRte;         /*!< 采样率，稳态为每秒采样率 */
    INT16U	 	 BinStateCnt;	 /*!< 开关量变位点的个数　*/
    INT8U    	 IsTrg;      	 /*!< 本帧数据是否包含启动信息。是为1否则为0 */
    INT8U        Resv[3];        /*!< 预留，数据帧头共32字节　*/
} FRM_HEAD_CONT;


/*!
    \brief  暂态数据帧头
    
 */
typedef struct 
{
    INT32U       	FrmType;        /*!< 帧类型标识,暂态帧类型标识应为SHD_FRM_TYPE_TRNS_HEAD */
    INT32U       	FrmLen;         /*!< 帧长度，暂态帧长不确定，因此始终为0 */
    INT32U       	FrmIdx;         /*!< 帧序号，从1开始计数，对于暂态而言，此序号即为录波文件编号 */
    
    INT16U       	AnChCnt;        /*!< 模拟量通道数 */
    INT16U       	BinChCnt;       /*!< 状态量通道数 */

	INT16U			SmpLenPerPnt;	/*!< 一个原始采样数据段的数据点的长度 */
	INT16U			CompLenPerPnt;	/*!< 一个复数数据段的数据点的长度 */
	INT16U			AnSmpLenPerPnt;	/*!< 一个模拟量采样数据点的长度 */
	INT16U			AnCompLenPerPnt;/*!< 一个模拟量复数采样数据点的长度 */
	INT16U			BinLenPerPnt;	/*!< 一个开关量采样数据点的长度 */
  INT16U			BinStateItemLen;/*!< 一个开关变位点的长度 */
	
	INT16U			EndSmpRte;		/*!< 固定填充0 */
    INT16U       	A2SmpRte;       /*!< A2 段记录频率 */
    INT16U       	A1SmpRte;       /*!< A1 段记录频率 */
    INT16U       	BSmpRte;        /*!< B  段记录频率 */
    INT16U       	CSmpRte;        /*!< C  段记录频率 */
    INT16U       	DSmpRte;        /*!< D  段记录频率 */
    
    YHFtimeval		FltTime;   		/*!< 时标，故障触发时间(第一个B段起始时间) */
    YHFtimeval		StartTime; 		/*!< 时标，起始时间(第一个A2段起始时间) */

    INT16U			FltNum;			/*!< 电网故障序号 */
    INT16U			TrgType;		/*!< 启动类型 */
    INT16U			TrgInfoLen;		/*!< 启动信息的长度 */
    
    INT8U			FlgFragment;	/*!< 分文件标识，指明本录波文件是上一个录波文件的延续(不应进行启动点零点校正) */
    INT8U			CompDataType;	/*!< 复数数据存储类型 0--存储为浮点数 1--存储为INT16(与DAT相同，精度有损失) */
    
    INT8U       	Resv[20]; 		/*!< 预留，使数据帧头共80字节　*/
} FRM_HEAD_TRNS;



/*!
    \brief  暂态数据子帧帧头
    
 */
typedef struct
{
    INT32U	 FrmType;    /*!< 帧类型标识，暂态数据子帧类型应为SHD_FRM_TYPE_TRNS_DATA */
    INT32U   FrmLen;	 /*!< 帧长度 */
    INT32U   FrmIdx;     /*!< 帧编号，每个文件的帧编号从1开始, 0表示文件结束 */

	INT32U	 SmpPntIdx;	 /*!< 采样点索引号，标志本帧的第一个采样点相对于整个录波过程的采样序号(实质上就是DAT文件中的采样点序号) */
    INT16U   SmpNum;     /*!< 本帧中采样点数*/
    INT16U   ReTrgNum;   /*!< 重入次数，即重新进入A1段的次数，，从0开始，如果C、D段没有重入，则=1 */
    
    INT16U	 BinStateCnt;/*!< 本帧中开关量变位点的个数　*/
    INT8U    SegFlg;     /*!< 本节中段标志，依次为A2=1、A1=2、B=3、C=4、D=5、０表示结束帧 */
    INT8U    DataType;   /*!< 数据的类型 0--原始采样数据 1--复数采样数据 */

    INT8U    IsTrg;      /*!< 本帧数据是否包含启动原因，即本次重入是否是第一次进入B 段。是为1， 否则为0*/
    INT8U    Resv1[3];   /*!< 预留，暂态数据子帧帧头长度为32字节　*/
    INT32U	 ChkSum;	 /*!< 帧校验和，计算数据区的校验和　*/
} FRM_HEAD_TRNS_DATA;


/*! 一个复数段数据点的长度
    一个数据点的排列如下：
    序号    长度（字节）            说明
    1       sizeof(SMP_TIMESTAMP)   采样时标
    2       sizeof(COMPLEX)*32      1号模拟量板实虚部数据（当模拟量板数量>=1时出现）
    3       sizeof(COMPLEX)*32      2号模拟量板实虚部数据（当模拟量板数量>=2时出现）
    4       sizeof(COMPLEX)*32      3号模拟量板实虚部数据（当模拟量板数量>=3时出现）
    5       8                       1号开关量板状态（每一位表示一个开关量的状态,当开关量板数>=1时出现）
    6       8                       2号开关量板状态（每一位表示一个开关量的状态,当开关量板数>=2时出现）
    7       8                       3号开关量板状态（每一位表示一个开关量的状态,当开关量板数>=3时出现）
*/


/*!
    \brief  实时数据帧

    每一周波的原始采样数据与FPGA上送的采样数据结构发完全相同
	其长度为FPGA_SMP_DATA_LEN
*/
typedef struct
{
    FRM_HEAD_RT*        FrmHead;        /*!< 帧头*/
    CALC_DATA_INFO      CalcDataInfo;  	/*!< 1周波计算结果数据*/
    INT8U*           	SmpData;		/*!< 原始采样数据起始位置（5周波采样数据，采样率为暂态B段采样率）*/
    INT32U*           	ChkSum;         /*!< 帧校验和*/
} FRM_RT_INFO;


/*!
    \brief  暂态A1/B段数据帧信息
 */
typedef struct 
{
    FRM_HEAD_TRNS_DATA* FrmHead;    /*!< 帧头*/
    TRG_INFO*           TrgInfo;    /*!< 启动信息，若帧头中的IsTrg标志非0，则包含启动信息*/
    INT8U*           	A1BData;    /*!< A1、B段数据起始位置*/
    INT32U*           	ChkSum;     /*!< 帧校验和*/
} FRM_TRNS_A1B_INFO;


/*!
    \brief  暂态A2/C/D段数据帧信息
 */
typedef struct
{
    FRM_HEAD_TRNS_DATA* FrmHead;    /*!< 帧头*/
    TRG_INFO*           TrgInfo;    /*!< 启动信息，始终＝0*/
    INT8U*           	A2CDData;   /*!< A2、C、D段数据起始位置*/
    INT32U*           	ChkSum;     /*!< 帧校验和*/
} FRM_TRNS_A2CD_INFO;

/*!
    \brief  稳态数据帧

    每一周波的稳态数据排列
    序号    长度（字节）                说明
    1       SMP_DATA_LEN_PER_PNT        1周波采样数据,其中SmpPnt为每周波采样点数
    2       CALC_DATA_LEN_PER_PNT       1周波计算数据   
*/
typedef struct
{
    FRM_HEAD_CONT*      FrmHead;    /*!< 帧头*/
    INT8U*           	ContData;   /*!< 稳态数据，包含多个周波的稳态数据（依据帧头而定）*/
    INT32U*           	ChkSum;     /*!< 帧校验和*/
} FRM_CONT_INFO;

#define	MAX_SHD_FRM_NUM		8192*5 /* 8192*5 */

/*!
    \brief  SHD数据信息 
*/
typedef struct
{
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
	INT8U*				SgDataAddr;					/*!< 序列化的定值数据区起始地址 */
	INT32				SgDataLen;					/*!< 序列化的定值数据区长度 */
	
}SHD_INFO;

#pragma pack () /* 恢复字节对齐设定*/

void print_FRM_HEAD_TRNS_DATA( FRM_HEAD_TRNS_DATA* pframe, FILE* fp );

#ifdef __cplusplus
}
#endif


#endif  /* __SH_DFR_STRUCT_DEFINE_H */

