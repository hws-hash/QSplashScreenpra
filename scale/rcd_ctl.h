/*************************************************************************
* Copyright (c) 2010,深圳市YHF电气技术有限公司
* All rights reserved.

**************************************************************************/

/*!	\file
 	\brief		
 	
	\author		Chen
	\version	1.0
	\date		2010-03-18
	
*/

#ifndef	__SH_RECORD_CONTROL_DEFINE_H
#define	__SH_RECORD_CONTROL_DEFINE_H

#include "rt_ctl.h"
//#include "sg_def.h"
#include "calc_def.h"
#include "yhftypes.h"

#define		FAULT_A2SEG		1
#define		FAULT_A1SEG		2
#define		FAULT_BSEG		3
#define		FAULT_CSEG		4
#define		FAULT_DSEG		5
#define		FAULT_END_SEG	0

#define		MAX_RETRG_NUM_PER_FLT	100			/*!< 每个故障最大重入次数*/
#define		MAX_CYC_NUM_PER_FLT		50*60*30	/*!< 每个故障最大记录周波数*/

#define		MAX_FAULT_SEG_NUM		500			/*!< 最大故障段数*/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 	\brief	故障数据段

	录波处理时，A2/A1/C/D段，每段对应于一个FAULT_SEG结构
	对于B段，每重入一次，对应一个新的FAULT_SEG结构

	故障数据段信息结构并不保存真正的录波数据，只是保存录波数据的索引信息。
	当系统实时缓冲区足够大的时候，可根据这些信息，追溯整个录波的过程。

 */
typedef struct fault_seg_info_t
{
	INT32				SegFlg;     	/*!< 本段标志，依次为A2=1、A1=2、B=3、C=4、D=5, END=0*/
	INT32				CycNum;			/*!< 本段总的周波数*/
	INT32				LeftCycNum;		/*!< 本段剩余周波数*/
	INT32				StartPos;		/*!< 本段起始位置 */
	INT32				TrgOffset;		/*!< 启动信息的偏移周波数(因为启动判断可以滞后于起始位置,仅对B段有意义...) */
} FAULT_SEG_INFO;


typedef struct	fault_info_t
{
	FAULT_SEG_INFO*		SegInfo;		/*!< 故障段的首地址 */
	INT32				MaxSegCnt;		/*!< 最大故障段的个数 */
	INT32				SegCnt;			/*!< 故障段的总的个数 */
	INT32				CycCnt;			/*!< 所有故障数据总的周波数 */

	INT32				FlgRcding;		/*!< 标志是否处于录波过程中... */
	INT32				FlgShdMading;	/*!< 标志是否处于SHD的生成过程中... */
	INT32				FlgNeedSplit;	/*!< 标志是否需要进行分文件的处理... */

	YHFtimeval			FltTime;		/*!< 故障发生时间 */
	YHFtimeval			StartTime;		/*!< 起始时间 */

//	TRG_FLG				LastTrgFlg;		/*!< 上次触发标志，用于重入判断及录波分段处理 */

	INT32				ReTrgCnt;		/*!< 重入次数 */
	INT32				FltFileLen;		/*!< 故障文件长度(字节数) */
	
	struct mk_shd_ctl_t*	MkShdCtl;	/*!< SHD生成对象 */
} FAULT_INFO;

#define		TRNS_RCD_STATUS_NO_RCD		0		/*!< 无录波 */
#define		TRNS_RCD_STATUS_START_RCD	1		/*!< 开始录波 */
#define		TRNS_RCD_STATUS_RCDING		2		/*!< 录波过程中 */
#define		TRNS_RCD_STATUS_STOP_RCD	3		/*!< 结束录波 */

/*!
 	\brief	暂态录波控制
 */
typedef struct trns_rcd_ctl_t
{
	int					Mod;				/*!< 当前系统运行状态 0调试 1正常 */
	
	int					FltNum;				/*!< 电网故障序号 */
	
	int					FlgReInit;			/*!< 系统重新初始化标志 0 1 */
	int					FlgStopRec;			/*!< 系统停止录波标志 0 1 */
	int					FlgRcdTrg;			/*!< 触发录波(手动录波)标志 0 1*/
	int					FlgRcdStr;			/*!< 录波启动标志 */
	int					FlgRcdMade;			/*!< 生成录波文件的标志 */

	int					FlgSplit;			/*!< 分文件的标志 */

	FAULT_INFO*			FltInfo1;			/*!< 故障信息1 */
	FAULT_INFO*			FltInfo2;			/*!< 故障信息2 */

//	YH_TRNS_RCD_SET		Setting;			/*!< 定值信息 */

	FAULT_INFO*			CurFltInfo;			/*!< 当前故障录波信息 */
}TRNS_RCD_CTL;


//TRNS_RCD_CTL * rcd_ctl_create();
//int  rcd_ctl_init( TRNS_RCD_CTL* prcdCtl, SHDFR_SG* psg );
//int  rcd_ctl_destory( TRNS_RCD_CTL* prcdCtl );
//int  flt_info_init( FAULT_INFO* flt_info, SHDFR_SG* psg );

#ifdef __cplusplus
}
#endif

#endif	//__SH_RECORD_CONTROL_DEFINE_H
