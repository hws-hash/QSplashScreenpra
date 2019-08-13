/*************************************************************************
* Copyright (c) 2011,深圳市元恒丰电气技术有限公司
* All rights reserved.

**************************************************************************/

/*!	\file
    \brief		实时数据区相关数据结构及接口的定义

    \author		chentm
    \version	1.0
    \date		2011-03-11

*/

#ifndef	__SH_REAL_TIME_CONTROL_DEFINE_H
#define	__SH_REAL_TIME_CONTROL_DEFINE_H

#include <stdio.h>
#include "calc_def.h"
#include "../scale/rt_sg.h"
#include "xtime.h"

#define		DFT_PNT_CNT_PER_CYC		20		/*!< 进行DFT计算时，每周波数据的采样点数 */
#define		RT_BUFF_CYC_NUM		600			/*!< 采样缓冲区原始数据周波数 */
#define		RAW_AN_SMP_DATA_LEN		2		/*!< 一个原始采样数据点的长度 */

#ifdef __cplusplus
extern "C" {
#endif

/*
实时数据的物理存储方式

<pre>
1.实时数据分块存储：分为原始采样数据区、计算结果数据区
2.计算结果按类型分块存储；分为模拟量通道、电压量、电流量、变压器、发电机等；
3.每一类型数据块内按周波顺序循环存储；
4.每一周波内的数据按序号顺序存储；
5.计算结果索引区存储着每个周波的索引信息；
6.每周波索引信息结构的各个属性项，分别指向对应周波的相应计算区的存储地址；
7.计算索引区在初始化后运行时保持不变，主要是为了方便在算法中快速定位相应数据地址，提高效率，并避免浪费存储空间
8.这种顺序循环存储+索引的方式，也可以高效、方便的适应算法的应用。
9.计算原始模拟通道采样值为每周波20点，从原始数据中抽取。

+---------------+---------------+---------------+---------------+---------------+
| 实时周波      |    周波1      |    周波2      |      ...      |    周波n      |
| 索引信息      |Smp|Cal|Trg|...|Smp|Cal|Trg|...|Smp|Cal|Trg|...|Smp|Cal|Trg|...|
+---------------+---------------+---------------+---------------+---------------+
|                        原始采样数据存储区                                     |
+---------------+---------------+---------------+---------------+---------------+
| 原始模拟      |    周波1      |    周波2      |      ...      |    周波n      |
| 通道采样值    | 1 | 2 |...| N | 1 | 2 |...| N |      ...      | 1 | 2 |...| N |
+---------------+---------------+---------------+---------------+---------------+
| 原始开关量    |    周波1      |    周波2      |      ...      |    周波n      |
| 通道采样值    | 1 | 2 |...| N | 1 | 2 |...| N |      ...      | 1 | 2 |...| N |
+---------------+---------------+---------------+---------------+---------------+
|                        计算结果数据存储区                                     |
+---------------+---------------+---------------+---------------+---------------+
| 模拟量通道    |    周波1      |    周波2      |      ...      |    周波n      |
| 计算结果      | 1 | 2 |...| N | 1 | 2 |...| N |      ...      | 1 | 2 |...| N |
+---------------+---------------+---------------+---------------+---------------+
| 电压量        |    周波1      |    周波2      |      ...      |    周波n      |
| 计算结果      | 1 | 2 |...| N | 1 | 2 |...| N |      ...      | 1 | 2 |...| N |
+---------------+---------------+---------------+---------------+---------------+
| 电流量        |    周波1      |    周波2      |      ...      |    周波n      |
| 计算结果      | 1 | 2 |...| N | 1 | 2 |...| N |      ...      | 1 | 2 |...| N |
+---------------+---------------+---------------+---------------+---------------+
| 变压器        |    周波1      |    周波2      |      ...      |    周波n      |
| 计算结果      | 1 | 2 |...| N | 1 | 2 |...| N |      ...      | 1 | 2 |...| N |
+---------------+---------------+---------------+---------------+---------------+
| 发电机        |    周波1      |    周波2      |      ...      |    周波n      |
| 计算结果      | 1 | 2 |...| N | 1 | 2 |...| N |      ...      | 1 | 2 |...| N |
+---------------+---------------+---------------+---------------+---------------+
|                        启动信息数据存储区                                     |
+---------------+---------------+---------------+---------------+---------------+
| 暂态录波      |    周波1      |    周波2      |      ...      |    周波n      |
| 触发信息      | 1 | 2 |...| N | 1 | 2 |...| N |      ...      | 1 | 2 |...| N |
+---------------+---------------+---------------+---------------+---------------+
| 稳态录波      |    周波1      |    周波2      |      ...      |    周波n      |
| 触发信息      | 1 | 2 |...| N | 1 | 2 |...| N |      ...      | 1 | 2 |...| N |
+---------------+---------------+---------------+---------------+---------------+

</pre>

*/

typedef struct
{
    INT32	Smping: 1;			/*!< 采样标志,标志采样中 */
    INT32   SmpValid: 1;     	/*!< 采样值有效标志,标志着该周波所有采样点数据已经采样 */
    INT32	CalValid: 1;		/*!< 计算值有效标志 */
    INT32	TrnTrgValid: 1;		/*!< 暂态启动有效标志 */
    INT32	ContTrgValid: 1;	/*!< 稳态启动有效标志 */

    INT32	TimeJump: 1;		/*!< 标志本周波的原始采样点中，数据时标有明显的跳变 */
} CYC_DATA_STATUS;

/*!
    \brief	周波数据索引
 */
typedef struct cycle_data_index_t
{
    INT32						CycSeqNo;		/*!< 周波顺序号 */
    CYC_DATA_STATUS				Status;			/*!< 数据状态 */

    INT32						BinStateCnt;	/*!< 本周波内，开关变位点的个数 */
    INT32						BinStatePos;	/*!< 本周波起始时标对应的开关变位点(即本周波起始时所对应的开关状态) */

    INT8U*						SmpDataAddr;	/*!< 原始采样数据起始地址 */

    INT8U*						CalDataAddr;	/*!< 计算结果数据起始地址 */

    YHFtimeval *     		TimeStamp;  	/*!< 时标，指向计算数据空间的时标 */
    ANCH_VAL*           		AnChVal;    	/*!< 模拟量通道计算数据起始位置*/
    ULIN_VAL*           		ULinVal;    	/*!< 电压计算数据起始位置*/
    ILIN_VAL*           		ILinVal;    	/*!< 电流计算数据起始位置*/
    GN_VAL*             		GnVal;      	/*!< 发电机计算数据起始位置*/
    TRN_VAL*            		TrnVal;     	/*!< 变压器计算数据起始位置*/

    TRG_INFO*					TrnsTrgInfo;	/*!< 暂态触发信息*/
    TRG_INFO*					ContTrgInfo;	/*!< 稳态触发信息*/

    struct cycle_data_index_t*	pprev;			/*!< 前一周波数据索引 */
} CYC_DATA_IDX;


/*!
    \brief	开关量通道状态信息条目
 */
typedef struct
{
    YHFtimeval		t;								/*!< 时标 */
    INT32			ChgCnt;							/*!< 包含的开关变位点的个数 */
    INT16U			CycIdx;							/*!< 时标对应的周波索引号 */
    INT16U			SmpIdx;							/*!< 时标对应的周波内的采样点序号 */
    INT16U			Status[MAX_BINCH_WORD_NUM];		/*!< 状态 */
    INT16U			ChgFlg[MAX_BINCH_WORD_NUM];		/*!< 变位标志 */
}BIN_CH_STATE_ITEM;

/*!
    \brief	实时数据控制
 */
typedef struct rt_data_area_t
{
    INT32				AnChCnt;			/*!< 模拟量通道数 */
    INT32				BinChCnt;			/*!< 状态量通道数 */
    INT32				ULinCnt;			/*!< 电压量数 */
    INT32				ILinCnt;			/*!< 线路数	*/
    INT32				TrnCnt;				/*!< 变压器数 */
    INT32				GnCnt;				/*!< 发电机数 */

    INT32				CycCnt;				/*!< 总的周波数 */
    INT32				SmpCntPerCyc;		/*!< 每周波采样点数 */
    INT32				SmpCntPerSec;		/*!< 每秒钟的采样点数 */
    INT32				SmpIntervalUsec;	/*!< 采样间隔时间(微秒) = 1s/系统频率/SmpCntPerCyc */
    INT32				CycIntervalUsec;	/*!< 每周波的时间间隔 */
    INT32				SmpPntCnt;			/*!< 总的采样点数 */

    INT32				AnSmpLenPerPnt;		/*!< 每个模拟量采样点数据总的长度 */
    INT32				BinLenPerPnt;		/*!< 每个开关量采样点数据总的长度 */
    INT32				SmpDataLenPerPnt;	/*!< 每个原始采样数据点的长度 */
    INT32				SmpDataLenPerCyc;	/*!< 每个周波的原始采样数据的总长 */
    INT32				CalDataLenPerCyc;	/*!< 每个周波的计算数据的总长 */

    //INT8U*				SgDataAddr;			/*!< 定值数据起始地址 */
    //INT32U				SgDataLen;			/*!< 定值数据区长度 */
    //INT32U				SgBufLen;			/*!< 定值数据缓冲区长度 */

    INT8U*				IdxBufAddr;			/*!< 数据索引区起始地址 */
    INT32U				IdxBufLen;			/*!< 数据索引区长度 */

    INT8U*				SmpBufAddr;			/*!< 原始采样数据起始地址 */
    INT32U				SmpBufLen;			/*!< 原始采样数据区长度 */

    INT8U*				CalBufAddr;			/*!< 计算数据缓冲区起始地址 */
    INT32U				CalBufLen;			/*!< 计算数据缓冲区长度 */

    TRG_INFO*			TrnsTrgArr;			/*!< 暂态启动信息 */
    TRG_INFO*			ContTrgArr;			/*!< 稳态触发信息 */

    FLOAT32*			DftCalDataPrev;		/*!< DFT计算的临时数组：上上周波数据 */
    FLOAT32*			DftCalDataCur;		/*!< DFT计算的临时数组：上一周波数据 */
    FLOAT32*			DftCalDataNext;		/*!< DFT计算的临时数组：当前周波数据 */

    INT32				DftCalDataArrLen;	/*!< DFT计算的临时数组的长度 */

    CYC_DATA_IDX*		CycIdxArr;			/*!< 索引数组起始地址 */

    /*开关变位表中保存着从系统初始化以前的开关状态信息，只有当有状态改变时，才会向开关变位表中添加一条信息*/
    INT32				MaxBinChStateNum;	/*!< 开关变位表的最大条目数 */
    BIN_CH_STATE_ITEM*	BinChStateArr;		/*!< 开关变位表 */
    INT32				StateFirstPos;		/*!< 第一个状态的位置 */
    INT32				StateLastPos;		/*!< 最后一个状态的位置 */

    SHDFR_SG			DownLoadSg;			/*!< 下发的定值信息 */
    SHDFR_SG			DfrSg;				/*!< 定值信息 */
    RTDFR_SG			RtSg;				/*!< 实时分析处理定值参数信息，包括返回值*/

    int					CurSmpPntPos;		/*!< 当前最新一个采样点的位置 */
    INT8U*				CurSmpPntAddr;		/*!< 当前最新一个采样点的数据存储地址 */

    INT8U*				CurSmpCycAddr;		/*!< 当前最新一个计算周波数据的起始位置（未使用） */

    int					CurCycPos;			/*!< 当前最新一个计算周波数据的位置 */

    int					RunningState;		/*!< 当前系统运行状态 0正常 1调试*/

    unsigned int		swFlg;				/*!< 旋钮状态 */
  //int					swFlgCh[12];		/*!< 旋钮对应通道 */
    int					swFlgReInit;		/*!< 旋钮状态重新初始化标志 */

    int					FlgReloadSg;		/*!< 系统重新加载定值的标志 */

    int					contReInit;			/*!< 稳态重新加载定值的标志 */

    int					FlgReInit;			/*!< 系统重新初始化标志 */

    int					InitCnt;			/*!< 重新初始化一定数量周波内，不判断故障 */

} RT_CTL;

/*!
    \brief	实时数据区的创建
    \param	pSg		[in]	定值信息
    \param	cyc_num	[in]	实时数据缓冲周波数
    \return	成功返回创建的实时数据区的指针，否则返回NULL
 */
RT_CTL* rt_ctl_create( SHDFR_SG* pSg, int cyc_num );

/*!
    \brief	实时数据区的初始化
    \param	pSg		[in]	定值信息
    \return	成功返回0，否则返回非0错误号

    根据定值重新初始化实时数据区。
    当更新系统定值时，调用本函数。
    注意：当新的定值与原定值不匹配时，会导致内存空间的重新分配。
 */
int rt_ctl_init( RT_CTL* prtCtl, SHDFR_SG* pSg );

/*!
    \brief	实时数据区的销毁
    \return	成功返回0，否则返回非0错误号
 */
int rt_ctl_destory( RT_CTL* prtCtl );

/*!
    \brief	打印实时数据区的信息
 */
void print_rt_ctl( RT_CTL* prtCtl, FILE* fp );

/*!
    \brief	打印开关变位条目信息
 */
void sprint_BIN_CH_STATE_ITEM( char* buf, BIN_CH_STATE_ITEM* item,
    int binChCnt, int idx );

#ifdef __cplusplus
}
#endif

#endif	//__SH_REAL_TIME_CONTROL_DEFINE_H
