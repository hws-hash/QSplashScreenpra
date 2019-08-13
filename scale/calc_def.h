/*************************************************************************
* Copyright (c) 2010,深圳市YHF电气技术有限公司
* All rights reserved.

**************************************************************************/

/*!	\file
 	\brief		计算结果数据结构的定义
 	
	\author		Chen
	\version	1.0
	\date		2010-03-11
	
*/

#ifndef	__SH_CALC_DATA_STRUCT_DEFINE_H
#define	__SH_CALC_DATA_STRUCT_DEFINE_H

#include "yhftypes.h"
//#include "xtime.h"

#define	DFT_PNT_CNT_PER_CYC		20		/*!< 进行DFT计算时，每周波数据的采样点数 */

#define SEQT0			(0.3333333f)
#define SEQT1		    (0.1666667f)
#define SEQT2			(0.2886751f)

#ifdef __cplusplus
extern "C" {
#endif

#if 0
/*!
    \brief  原始采样时标    16Bytes
    
    本结构与FPGA中原始采样数据的时标完全一致．
    在DSP与监控板通讯过程中，所有的数据时标也采用此时标．
    DSP不做任何与时间转换相关操作．
 */
typedef struct
{
    INT16U   day;        /*!< 日 */
    INT16U   hour;       /*!< 时 */
    INT16U   min;        /*!< 分 */
    INT16U   sec;        /*!< 秒 */
    INT16U   msec;       /*!< 毫秒 */
    INT16U   usec;       /*!< 微秒 */
    INT16U   year;       /*!< 年 */
    INT16U   month;      /*!< 月 */
} SMP_TIMESTAMP;
#else
typedef struct timeval	SMP_TIMESTAMP;
#endif



/*!
    \brief  复数结构(实虚部表示)
 */
typedef struct
{
    FLOAT32		Real;   /*!< 实部 */
    FLOAT32		Imag;   /*!< 虚部 */
} COMPLEX;


/*!
    \brief  复数结构(实虚部、有效值表示)
 */
typedef struct
{
    FLOAT32		Real;   /*!< 实部 */
    FLOAT32		Imag;   /*!< 虚部 */
    FLOAT32		Rms;	/*!< 有效值 */
} COMPLEX_R;

/*!
    \brief  复数结构(实虚部、有效值、相位表示)
 */
typedef struct
{
    FLOAT32		Real;   /*!< 实部 */
    FLOAT32		Imag;   /*!< 虚部 */
    FLOAT32		Rms;	/*!< 有效值 */
    FLOAT32		Phs;	/*!< 相位 */
} COMPLEX_RP;


/*!
    \brief  序分量
 */
typedef struct
{
    FLOAT32		c1;		/*!< 正序 */
    FLOAT32		c2;		/*!< 负序 */
    FLOAT32		c0;		/*!< 零序 */
} SEQ_VAL;

/*!
    \brief  功率分量
 */
typedef struct
{
    FLOAT32         PPower;         /*!< 有功功率*/
    FLOAT32         QPower;         /*!< 无功功率*/
    FLOAT32         ApparentPower;  /*!< 视在功率*/
} POWER_VAL;

/*!
    \brief  模拟通道计算结果
 */
typedef  struct
{
    INT32U			IsValid;        /*!< 是否有效 */
    COMPLEX_R       ChVal;          /*!< 通道计算结果 */    
    COMPLEX_R       Ha2Val;         /*!< 2次谐波值 */
    COMPLEX_R       Ha3Val;         /*!< 3次谐波值 */
    COMPLEX_R       Ha5Val;         /*!< 5次谐波值 */
    COMPLEX_R       Ha7Val;         /*!< 7次谐波值 */
    COMPLEX_R       Ha9Val;         /*!< 9次谐波值 */
}ANCH_VAL;


/*!
    \brief  电压计算结果
 */
typedef struct
{
    INT32U			IsValid;    /*!< 是否有效 0 = 有效； 1 = 频率计算无效； 2 = 序分量计算无效； 3 = 频率、序分量计算均无效*/
    FLOAT32         Hz;         /*!< 频率*/
    FLOAT32         HzRte;      /*!< 频率变化率 */
    FLOAT32         PsSeq;      /*!< 正序 */
    FLOAT32         NgSeq;      /*!< 负序 */
    FLOAT32         ZerSeq;     /*!< 零序 */
}ULIN_VAL;


/*!
    \brief  电流计算结果
 */
typedef struct
{
    INT32U			IsValid;        /*!< 是否有效 0 = 有效；1 = 功率计算无效；2 = 序分量计算无效；3 = 功率、序分量计算均无效 */
    FLOAT32         PPower;         /*!< 有功功率*/
    FLOAT32         QPower;         /*!< 无功功率*/
    FLOAT32         ApparentPower;  /*!< 视在功率*/
    FLOAT32         PF;             /*!< 平均功率因数*/
    FLOAT32         PsSeq;          /*!< 正序*/
    FLOAT32         NgSeq;          /*!< 负序*/
    FLOAT32         ZerSeq;         /*!< 零序*/
}ILIN_VAL;


/*!
    \brief  变压器计算结果
 */
typedef struct 
{
    INT32U			IsValid;            /*!< 是否有效 0：有效， 1：过激磁倍数无效， 2：差流无效， 3：过激磁、差流均无效*/
    FLOAT32         OverExcition;       /*!< 变压器过激磁倍数*/
    FLOAT32         DiffAClc[3];        /*!< 变压器各相差流*/
    FLOAT32         ThrATimes[3];       /*!< 变压器穿越电流倍数*/
    FLOAT32         AperComponent[3];   /*!< 变压器非周期分量*/ 
    FLOAT32         RstA[3];            /*!< 制动电流*/
    FLOAT32         Resv[1];            /*!< 为以后扩展使用*/
}TRN_VAL;


/*********************************2011.3.10修改**********************************************************/
/*!
    \brief  发电机计算结果
*/
typedef struct
{
    INT32U			IsValid;			/*!< 是否有效 0：有效， 1：励磁计算结果无效， 2：机端计算结果无效， 3：励磁与机端计算结果均无效 */
	FLOAT32			PPower;				/*!< 有功功率 */
	FLOAT32			QPower;				/*!< 无功功率 */
	FLOAT32			ApparentPower;		/*!< 视在功率 */
	FLOAT32			P2Power;			/*!< 负序功率 */
	FLOAT32			OverExcition;		/*!< 发电机过激磁倍数 */
	FLOAT32			ExcVol;				/*!< 发电机励磁电压 */
	FLOAT32			StatorHar3;			/*!< 发电机定子三次谐波电压比 */
	FLOAT32			RotorHar2;			/*!< 转子二次谐波 */
    FLOAT32			DifClc[3];			/*!< 发电机各相完全纵差电流 */
	FLOAT32			DifInClc[3];		/*!< 发电机各相不完全纵差电流 */
	FLOAT32			DifTranClc[3];		/*!< 发电机各相横差电流 */
    FLOAT32			LowFreOvCur[3];		/*!< 低频电流最大值 */
}GN_VAL;

/**********************************************************************************************/

/*!
    \brief  周波计算结果

    <pre>
    一周波计算结果数据排列如下：
    序号    长度（字节）                说明
    1       sizeof(YHFtimeval)       时标
    2       sizeof(ANCH_VAL)*AnChCnt    模拟量通道计算数据
    3       sizeof(ULIN_VAL)*ULinCnt    电压计算数据
    4       sizeof(ILIN_VAL)*ILinCnt    电流计算数据
    5       sizeof(GN_VAL)*GnCnt        发电机计算数据
    6       sizeof(TRN_VAL)*TrnCnt      变压器计算数据
    </pre>
*/
typedef struct 
{
    YHFtimeval*      TimeStamp;  /*!< 时标*/
    ANCH_VAL*           AnChVal;    /*!< 模拟量通道计算数据起始位置*/
    ULIN_VAL*           ULinVal;    /*!< 电压计算数据起始位置*/
    ILIN_VAL*           ILinVal;    /*!< 电流计算数据起始位置*/
    GN_VAL*             GnVal;      /*!< 发电机计算数据起始位置*/
    TRN_VAL*            TrnVal;     /*!< 变压器计算数据起始位置*/
}CALC_DATA_INFO;


/*************************************************************************
**************************************************************************/

//#define	PI			(3.1415926535897932f)
#define EPSINON 	(0.000001)

/*!
	\brief	生成DFT计算所需的正、余弦系数表，用于幅值计算，基于额定频率
	\param	psinf		[out]	正弦系数表
	\param	pcosf		[out]	余弦系数表
	\param	k			[in]	谐波次数
	\param  sampleNum	[in]	每周波的计算点数
*/
void init_coefficient_table( float* pcosf, float* psinf, int k, int sampleNum);

/*!
	\brief	生成DFT计算所需的正、余弦系数表，用于频率计算
	\param	psinf	[out]	正弦系数表
	\param	pcosf	[out]	余弦系数表
	\param	N		[in]	系数表个数
	\param  sample  [in]    每周的实际采样点数，采样时间间隔固定时，随系统频率变化而变化
*/
void coefficient_table( float* pcosf, float* psinf, int N, float sampleNum );

/*!
	\brief	生成DFT计算所需的正、余弦系数表
	\param	k				[in]	谐波次数
	\param	N				[in]	系数表点数
	\param  sampleNum		[in]    每周波实际采样点数
*/
float* creat_coefficient_table( int k, int N, float sampleNum );


/*!
	\brief	改进DFT计算，计算基波、5次、9次谐波
	\param	pfSrc		[in]	采样数据起始地址
	\param  pcosf		[in]	余弦系数表
	\param	psinf		[in]	正弦系数表
	\param	quarterNum	[in]	分别对应四分之一周波、半周波、四分之三周波、一周波计算点数
	\param	k			[in]	谐波次数，对应于基波，5，9次谐波
	\param	pComplex	[out]	指向计算结果结构的指针
*/
void improved_DFT159( float* pfSrc, float* pcosf, float* psinf, int* quarterNum, COMPLEX_R* pComplex );

/*!
	\brief	改进DFT计算，计算3，7次谐波
	\param	pfSrc		[in]	采样数据起始地址
	\param  pcosf		[in]	余弦系数表
	\param	psinf		[in]	正弦系数表
	\param	quarterNum	[in]	分别对应四分之一周波、半周波、四分之三周波、一周波计算点数
	\param	k			[in]	谐波次数，对应于3，7次谐波
	\param	pComplex	[out]	指向计算结果结构的指针
*/
void improved_DFT37( float* pfSrc, float* pcosf, float* psinf, int* quarterNum, COMPLEX_R* pComplex );

/*!
	\brief	改进DFT，只计算二次谐波
	\param	pfSrc		[in]	采样数据起始地址
	\param  pcosf		[in]	余弦系数表
	\param	psinf		[in]	正弦系数表
	\param	quarterNum	[in]	分别对应四分之一周波、半周波、四分之三周波、一周波计算点数
	\param	pComplex	[out]	指向计算结果结构的指针
*/
void improved_DFT2( float* pfSrc, float* pcosf, float* psinf, int* quarterNum, COMPLEX_R* pComplex );

/*!
	\brief	初始化DFT计算所需的正、余弦系数表
	\param	pcoeff	[in]	系数表起始地址
	\param	k		[in]	谐波次数
	\param	N		[in]	每周波采样点数
	\param  skew    [in]    角度偏移
*/
void init_coefficient_table_angle( float* pcoeff, int k, int N, float skew );

/*!
	\brief	DFT计算，基波实部 虚部和有效值, 谐波有效值，适用于频率等于额定频率情况
	\param	pfSrc		[in]	采样数据起始地址
	\param	pcoeff		[in]	正余弦系数表首地址
	\param	pComplex	[out]	指向计算结果结构的指针
*/
void calc_DFT( float* pfSrc, float* pCoeff, COMPLEX_R* pComplex );

/*!
	\brief	相邻周波正序相角差计算，适用于频率不等于额定频率情况
	\param	prevSrc			[in]	上一周波采样数据
	\param  curSrc			[in]	当前周波采样数据
	\param  nextSrc			[in]	下一周波采样数据
	\param	idxU			[in]	A，B，C三相电压通道号
	\param	sampleNum		[in]	每周波计算点数
	\param	prevFreq		[in]	上一周波频率
	\param  ratedFreq		[in]	额定频率
	\param	curAngle		[out]	指向计算结果的指针
*/
FLOAT32 calc_pos_angle( FLOAT32* prevSrc, FLOAT32* curSrc,FLOAT32* nextSrc, INT16* idxU, INT16 sampleNum, FLOAT32 prevFreq, INT32 ratedFreq);

/*!
	\brief	DFT计算，基波实部 虚部和有效值, 谐波有效值，适用于频率不等于额定频率情况，
			小于额定频率时需用到下一周波数据
	\param	pfSrc		[in]	当前周波采样数据
	\param  nextSrc		[in]	下一周波采样数据
	\param	psinf		[in]	正弦系数表
	\param	pcosf		[in]	余弦系数表
	\param	fCnt		[in]	每周波的实际点数
	\param  sampleNum	[in]	每周波采样点数（抽点后的每周波点数，20点或32点）
*/
float calc_Hz_DFT( float* pfSrc, float* nextSrc, float* pcosf, float* psinf, float fCnt, int sampleNum );

/*!
	\brief	直流计算
	\param	pfdata		[in]	原始采样数据起始地址
	\param	fcnt		[in]	原始采样点的个数
	\param	pComplex	[out]	指向计算结果结构的指针
*/
void calc_DC( float* pfsrc, int fcnt, COMPLEX_R* pComplex );

/*!
	\brief	频率计算
	\param	pFront		[in]	前一周波采样数据起始地址
	\param	pCur		[in]	当前原始采样数据起始地址
	\param  pNext       [in]	下一周波采样值起始地址
	\param  currentHz	[in]    当前采用的系统频率
	\param	sampleNum	[in]	每周波采样率（抽点后的每周波点数，20点或32点 ）
	\param  ratedFreq	[in]	额定频率
	\return 返回计算结果
*/
float calc_freq( float* pPrev, float* pCur, float* pNext, float currentHz, int sampleNum, INT32 ratedFreq );

/*!
	\brief	序量计算
	\param	phsA	[in]	A相计算结果
	\param	phsB	[in]	B相计算结果
	\param	phsC	[in]	C相计算结果
	\param	pseq	[out]	序量计算结果输出
*/
void calc_seq( COMPLEX_R* phsA, COMPLEX_R* phsB, COMPLEX_R* phsC, SEQ_VAL* pseq );

/*!
	\brief	序量计算：用于电压量序分量计算，其中正序需求得矢量值，用于频率计算
	\param	phsA	[in]	A相计算结果
	\param	phsB	[in]	B相计算结果
	\param	phsC	[in]	C相计算结果
	\param	pseq	[out]	序量计算结果输出
	\param  ps
*/
void calc_useq( COMPLEX_R* phsA, COMPLEX_R* phsB, COMPLEX_R* phsC, SEQ_VAL* pseq, FLOAT32* psangle );

/*!
	\brief	单相功率计算
	\brief  pU	  [in]	电压
	\brief	pI	  [in]	电流
	\brief  power [out] 单相功率
*/
POWER_VAL cal_power( COMPLEX_R* pU, COMPLEX_R* pI);

/*!
	\brief	三相功率计算
	\param	phsA	[in]	A相计算结果
	\param	phsB	[in]	B相计算结果
	\param	phsC	[in]	C相计算结果
	\param	power3	[out]	功率计算结果输出
*/
POWER_VAL cal_power3( COMPLEX_R* pUa, COMPLEX_R* pUb, COMPLEX_R* pUc, COMPLEX_R* pIa, COMPLEX_R* pIb, COMPLEX_R* pIc);

#ifdef __cplusplus
}
#endif


#endif	//__SH_CALC_DATA_STRUCT_DEFINE_H
