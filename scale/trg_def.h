/*************************************************************************
* Copyright (c) 2010,深圳市YHF电气技术有限公司
* All rights reserved.

**************************************************************************/

/*!	\file
 	\brief		录波启动信息数据结构的定义
 	
	\author		chen
	\version	1.0
	\date		2010-03-11
*/

#ifndef	__SH_TRG_INFO_DATA_STRUCT_DEFINE_H
#define	__SH_TRG_INFO_DATA_STRUCT_DEFINE_H

#include "yhftypes.h"
#include "calc_def.h"
//#include "sg_def.h"


//SHDFR_B后台中，定义的启动原因信息

#define SHDFR_B_RcdTrg			0	/*!< 手动录波 */

#define	SHDFR_B_HzHiAct			101	/*!< 频率越上限 */
#define	SHDFR_B_HzLoAct			102	/*!< 频率越下限 */
#define	SHDFR_B_HzRteAct		103	/*!< 频率变化率 */

#define	SHDFR_B_PsSeqHiAct		104	/*!< 正序越上限 */
#define	SHDFR_B_PsSeqLoAct		105	/*!< 正序越下限 */
#define	SHDFR_B_NgSeqHiAct		106	/*!< 负序越上限 */
#define	SHDFR_B_ZerSeqHiAct		107	/*!< 零序越上限 */
#define	SHDFR_B_ZerSeqDelAct	108	/*!< 零序突变 */

#define	SHDFR_B_SwgAct			109	/*!< 振荡 */
#define	SHDFR_B_PHiAct			110	/*!< 有功功率上限 */
#define	SHDFR_B_PRteAct			111	/*!< 有功功率变化率 */
#define	SHDFR_B_QHiAct			112	/*!< 无功功率上限 */
#define	SHDFR_B_QRteAct			113	/*!< 无功功率变化率 */

#define	SHDFR_B_HiAct			201	/*!< 通道越上限 */
#define	SHDFR_B_LoAct			202	/*!< 通道越下限 */
#define	SHDFR_B_DelAct			203	/*!< 通道突变 */
#define	SHDFR_B_Ha2Act			204	/*!< 通道2次谐波 */
#define	SHDFR_B_Ha3Act			205	/*!< 通道3次谐波 */
#define	SHDFR_B_Ha5Act			207	/*!< 通道5次谐波 */
#define	SHDFR_B_Ha7Act			208	/*!< 通道9次谐波 */
#define	SHDFR_B_Ha9Act			209	/*!< 通道9次谐波 */

#define	SHDFR_B_BinAct			206	/*!< 开关量启动 */

#define SHDFR_B_OvExAct			304	/*!< 变压器过激磁启动 */
#define SHDFR_B_DifAct			306	/*!< 变压器差流启动定值启动 */

#define	MAX_ANCH_NUM	512		/*!< 最大模拟量通道数 */
#define MAX_BINCH_NUM	1024	/*!< 最大开关量通道数(必须设为8的倍数) */
#define MAX_ULIN_NUM	64		/*!< 最大电压(三相)数 */
#define MAX_ILIN_NUM	128		/*!< 最大电流(三相)数 */
#define MAX_TRN_NUM		16		/*!< 最大主变数 */
#define MAX_GN_NUM		16		/*!< 最大发电机数 */

#define	MAX_BINCH_WORD_NUM		MAX_BINCH_NUM/8/2

#ifdef __cplusplus
extern "C" {
#endif


/*!
    \brief  通道启动信息
    
    描述一个通道的启动状态信息
 */
typedef struct ANCH_TRG_STRU
{
    /* 第一个字节 */
    INT8U    DelAct : 1;     /*!< 突变动作 */
    INT8U    HiAct : 1;      /*!< 越上限动作 */
    INT8U    LoAct : 1;      /*!< 越下限动作 */
    INT8U    Ha2Act : 1;     /*!< 二次谐波动作 */
    INT8U    Ha3Act : 1;     /*!< 三次谐波动作 */
    INT8U    Ha5Act : 1;     /*!< 五次谐波动作 */
    INT8U    Ha7Act : 1;     /*!< 七次谐波动作 */
    INT8U    Ha9Act : 1;     /*!< 九次谐波动作 */
    
    /* 第二个字节 */
	INT8U	 DelStr : 1;	 /*!< 突变启动 */
    INT8U    HiStr : 1;      /*!< 越上限启动 */
    INT8U    LoStr : 1;      /*!< 越下限启动 */
    INT8U    Ha2Str : 1;     /*!< 二次谐波启动 */
    INT8U    Ha3Str : 1;     /*!< 三次谐波启动 */
    INT8U    Ha5Str : 1;     /*!< 五次谐波启动 */
    INT8U    Ha7Str : 1;     /*!< 七次谐波启动 */
    INT8U    Ha9Str : 1;     /*!< 九次谐波启动 */
    
	INT8U	 ReAct;			 /*!< 重入标志 */
	INT8U    Resv;			 /*!< 预留 */
}ANCH_TRG;

/*!
    \brief  电压启动信息
    
    描述一个电压量的启动信息
 */
typedef struct ULIN_TRG_STRU
{
    /* 第一个字节 */
    INT8U    PsSeqHiAct : 1;     /*!< 正序越上限动作 */
    INT8U    PsSeqLoAct : 1;     /*!< 正序越下限动作 */
    INT8U    NgSeqHiAct : 1;     /*!< 负序越上限动作 */
    INT8U    ZerSeqHiAct: 1;     /*!< 零序越上限动作 */
    INT8U    ZerSeqDelAct:1;     /*!< 零序突变动作 */
    INT8U    HzHiAct : 1;        /*!< 频率越上限动作 */
    INT8U    HzLoAct : 1;        /*!< 频率越下限动作 */
    INT8U    HzRteAct: 1;        /*!< 频率变化率(突变)动作 */
    
    /* 第二个字节 */
    INT8U    PsSeqHiStr : 1;     /*!< 正序越上限启动 */
    INT8U    PsSeqLoStr : 1;     /*!< 正序越下限启动 */
    INT8U    NgSeqHiStr : 1;     /*!< 负序越上限启动 */
    INT8U    ZerSeqHiStr: 1;     /*!< 零序越上限启动 */
    INT8U    ZerSeqDelStr:1;     /*!< 零序突变启动 */
    INT8U    HzHiStr : 1;        /*!< 频率越上限启动 */
    INT8U    HzLoStr : 1;        /*!< 频率越上限启动 */
    INT8U    HzRteStr: 1;        /*!< 频率变化率(突变)启动 */
    
	INT8U	 ReAct;				 /*!< 重入标志 */
    INT8U    Resv;			 	 /*!< 预留 */
} ULIN_TRG;


/*!
    \brief  电流量启动信息
    
    描述一个电流量的启动信息
 */
typedef struct ILIN_TRG_STRU
{
    /* 第一个字节 */
    INT8U    NgSeqHiAct : 1;     /*!< 负序越上限动作 */
    INT8U    ZerSeqHiAct: 1;     /*!< 零序越上限动作 */
    INT8U    ZerSeqDelAct:1;     /*!< 零序突变动作 */
    INT8U    SwgAct : 1;         /*!< 振荡动作 */
    INT8U    PHiAct : 1;         /*!< 有功功率越上限动作 */
    INT8U    PRteAct: 1;         /*!< 有功功率变化率动作 */
    INT8U    QHiAct : 1;         /*!< 无功功率越上限动作 */
    INT8U    QRteAct: 1;         /*!< 无功功率变化率动作 */
    
    /* 第二个字节 */
    INT8U    NgSeqHiStr : 1;     /*!< 负序越上限启动 */
    INT8U    ZerSeqHiStr: 1;     /*!< 零序越上限启动 */
    INT8U    ZerSeqDelStr:1;     /*!< 零序突变启动 */
	INT8U    SwgStr : 1;		 /*!< 振荡启动 */
    INT8U    PHiStr : 1;         /*!< 有功功率越上限启动 */
    INT8U    PRteStr: 1;         /*!< 有功功率变化率启动 */
    INT8U    QHiStr : 1;         /*!< 无功功率越上限启动 */
    INT8U    QRteStr: 1;         /*!< 无功功率变化率启动 */
	
	INT8U	 ReAct;				 /*<  重入标志 */
    INT8U    Resv;				 /*!< 预留 */
} ILIN_TRG;

/************************2011.3.10修改**************************/
/*!
	\brief	发电机启动信息
	
	描述一个发电机的启动信息

	date	2011.3.10
*/
typedef struct GN_TRG_STRU
{
	INT8U	DifAAct: 1;			/*!< A相纵差动作 */
	INT8U	DifBAct: 1;			/*!< B相纵差动作 */
	INT8U	DifCAct: 1;			/*!< C相纵差动作 */
	INT8U	DifInAAct: 1;		/*!< A相不完全纵差动作 */
	INT8U	DifInBAct: 1;		/*!< B相不完全纵差动作 */
	INT8U	DifInCAct: 1;		/*!< C相不完全纵差动作 */
	INT8U	DifTranAAct: 1;		/*!< A相横差动作 */
	INT8U	DifTranBAct: 1;		/*!< B相横差动作 */

	INT8U	DifTranCAct: 1;		/*!< C相横差动作 */
	INT8U	OvExAct: 1;			/*!< 过励磁动作 */
	INT8U	LossExcAct: 1;		/*!< 失磁动作 */
	INT8U	StatorEarthAct: 1;	/*!< 定子接地动作 */
	INT8U	ZeroSeqAct: 1;		/*!< 纵向零序电压动作 */
	INT8U	RotorHar2Act: 1;	/*!< 转子二次谐波动作 */
	INT8U	ReversePoAct: 1;	/*!< 逆功率动作 */
	INT8U	LowFreOvCurAct: 1;	/*!< 低频过流动作 */

	INT8U	DifAStr: 1;			/*!< A相纵差启动 */
	INT8U	DifBStr: 1;			/*!< B相纵差启动 */
	INT8U	DifCStr: 1;			/*!< C相纵差启动 */
	INT8U	DifInAStr: 1;		/*!< A相不完全纵差启动 */
	INT8U	DifInBStr: 1;		/*!< B相不完全纵差启动 */
	INT8U	DifInCStr: 1;		/*!< C相不完全纵差启动 */
	INT8U	DifTranAStr: 1;		/*!< A相横差启动 */
	INT8U	DifTranBStr: 1;		/*!< B相横差启动 */

	INT8U	DifTranCStr: 1;		/*!< C相横差启动 */
	INT8U	OvExStr: 1;			/*!< 过励磁启动 */
	INT8U	LossExcStr: 1;		/*!< 失磁启动 */
	INT8U	StatorEarthStr: 1;	/*!< 定子接地启动 */
	INT8U	ZeroSeqStr: 1;		/*!< 纵向零序电压启动 */
	INT8U	RotorHar2Str: 1;	/*!< 转子二次谐波启动 */
	INT8U	ReversePoStr: 1;	/*!< 逆功率启动 */
	INT8U	LowFreOvCurStr: 1;	/*!< 低频过流启动 */

	INT8U	ReAct;				/*!< 重入标志 >*/
	INT8U   Resv[3];			/*!< 预留 */
} GN_TRG ;

/*****************************************************************************/

/*!
    \brief  变压器启动信息
    
    描述一个变压器的启动信息
 */
typedef struct TRN_TRG_STRU
{
    /* 第一个字节 */
    INT8U    OvExAct: 1;     /*!< 过激磁动作 */
    INT8U    ThrIAct: 1;     /*!< 穿越电流动作 */
    INT8U    DifAAct: 1;     /*!< A相差动电流动作 */
    INT8U    DifBAct: 1;     /*!< B相差动电流动作 */
    INT8U    DifCAct: 1;     /*!< C相差动电流动作 */
    INT8U    bit3: 1;        /*!< 预留 */
    INT8U    bit2: 1;        /*!< 预留 */
    INT8U    bit1: 1;        /*!< 预留 */

	INT8U    OvExStr: 1;     /*!< 过激磁动作 */
    INT8U    ThrIStr: 1;     /*!< 穿越电流动作 */
    INT8U    DifAStr: 1;     /*!< A相差动电流动作 */
    INT8U    DifBStr: 1;     /*!< B相差动电流动作 */
    INT8U    DifCStr: 1;     /*!< C相差动电流动作 */
    INT8U    bit6: 1;        /*!< 预留 */
    INT8U    bit5: 1;        /*!< 预留 */
    INT8U    bit4: 1;        /*!< 预留 */

    /* 第三个字节 */
	INT8U	 ReAct;			 /*<  重入标志 */
    INT8U    Resv;			 /*!< 预留 */
} TRN_TRG;

/*!
	\brief	启动信息

	2011.1.17 Yuan Mingjun修改：将动作标志细分，新添突变启动标志位用于长期越限处理 
*/
typedef struct TRG_FLG_STRU
{
    /* 第一个字节 */
    INT8U    Act: 1;     	 /*!< 动作标志 */
    INT8U    ReAct: 1;     	 /*!< 重入标志 */
	INT8U    OscAct: 1;      /*!< 震荡动作标志 */
	INT8U    DelTrg: 1;      /*!< 突变启动（置此标志，重新进入ABCD段） */
	INT8U    HzRteAct: 1;    /*!< 频率变化率动作标志 */    	
	INT8U    bit3: 1;        /*!< 预留 */
    INT8U    bit2: 1;        /*!< 预留 */
    INT8U    bit1: 1;        /*!< 预留 */
    
    /* 第二个字节，启动标志 */	
    INT8U    BinChAct: 1;    /*!< 开关量动作标志 */
    INT8U    AnChAct: 1;     /*!< 模拟量通道动作标志 */
	INT8U    ULineAct: 1;    /*!< 线路电压项动作标志 */
	INT8U	 ILineAct: 1;	 /*!< 线路电流项动作标志 */
    INT8U    TrnAct: 1;      /*!< 变压器动作标志 */
    INT8U    GnAct: 1;       /*!< 发电机动作标志 */
    INT8U    bit10: 1;       /*!< 预留 */
    INT8U    bit9: 1;        /*!< 预留 */

    /*  */
	INT16	 OscNum;		 /*!< 震荡提前周期 */
}TRG_FLG;

typedef struct RESULT_FLG_STRU
{
	INT8U    Str: 1;		 /*!< 启动标志 */
	INT8U    Act: 1;		 /*!< 动作标志 */
	INT8U    bit6: 1;		 /*!< 预留 */
    INT8U    bit5: 1;        /*!< 预留 */
    INT8U    bit4: 1;        /*!< 预留 */
    INT8U    bit3: 1;        /*!< 预留 */
    INT8U    bit2: 1;        /*!< 预留 */
    INT8U    bit1: 1;        /*!< 预留 */
} RESULT_FLG;

/*!
    \brief  启动结构
 */
typedef struct TRG_INFO_STRU
{
    TRG_FLG			TrgFlg;							/*!< 启动标志 */
    ANCH_TRG    	AnChTrg[MAX_ANCH_NUM];  		/*!< 模拟量通道启动信息 */
    ULIN_TRG    	ULinTrg[MAX_ULIN_NUM];    		/*!< 电压启动信息 */
    ILIN_TRG    	ILinTrg[MAX_ILIN_NUM];    		/*!< 电流启动信息 */
    GN_TRG      	GnTrg[MAX_GN_NUM];        		/*!< 发电机启动信息 */
    TRN_TRG     	TrnTrg[MAX_TRN_NUM];      		/*!< 变压器启动信息 */

	/* 对于全局性的启动信息，只要有任何一个元素的的启动标志为1，则全局的标志为1。*/
	ANCH_TRG		GlbAnChTrg;						/*!< 全局的模拟量通道启动信息 */
	ULIN_TRG		GlbULinTrg;						/*!< 全局的电压启动信息 */
	ILIN_TRG		GlbILinTrg;						/*!< 全局的电流启动信息 */
	GN_TRG			GlbGnTrg;						/*!< 全局的发电机启动信息 */
	TRN_TRG			GlbTrnTrg;						/*!< 全局的变压器启动信息 */

	/* 若有多个启动信息，以下索引记录的是第一个启动信息索引号 */
	INT16			IdxAnChTrg;						/*!< 索引号 模拟量通道启动信息 */
	INT16			IdxULinTrg;						/*!< 索引号 电压启动信息 */
	INT16			IdxILinTrg;						/*!< 索引号 电流启动信息 */
	INT16			IdxGnTrg;						/*!< 索引号 发电机启动信息 */
	INT16			IdxTrnTrg;						/*!< 索引号 变压器启动信息 */
	INT16			IdxBinChTrg;					/*!< 索引号 开关量通道启动信息 */

	INT32			BinTrgSmpIdx;					/*!< 开关量动作时(首个开关量变位)的周波内的采样序号 */
	YHFtimeval		BinTrgTimeStamp;				/*!< 开关量动作时(首个开关量变位)的绝对时标 */
	INT16U			BinChAct[MAX_BINCH_WORD_NUM];	/*!< 开关量通道触发动作信息 */
}TRG_INFO; 

/*!
    \brief  SHDFR_B的启动原因结构
 */
typedef struct SHDFR_B_TRG_REASON_STRU
{
	INT32			TrgType;		/*!< 启动类型 */
	INT32			ObjIndex;		/*!< 对应通道序号或线路序号 */
	INT32			RetrgTv;		/*!< 重入时间 */
	INT32			RetrgNo;		/*!< 重入编号 */
}SHDFR_B_TRG_REASON;

/*!
 	\brief	启动信息分析
 	\param	trg_info	录波数据子帧
 	\param	dfr_cfg		该参数指定了分析时的通道数、线路数等信息。如果为NULL，则按缺省的最大值进行分析。
 */
void trginfo_analyse( TRG_INFO* trg_info, struct yh_dfr_cfg* dfr_cfg );

/*!
 	\brief	将启动信息转换为SHDFR_B中的启动原因信息。
 */
void trginfo_2_shdfr_b( TRG_INFO* trg_info, SHDFR_B_TRG_REASON* trg_reson, int ulin_cnt );

#ifdef __cplusplus
}
#endif


#endif	/* __SH_TRG_INFO_DATA_STRUCT_DEFINE_H */
