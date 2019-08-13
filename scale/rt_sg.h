/*************************************************************************
* Copyright (c) 2008,深圳市YHF电气技术有限公司
* All rights reserved.

**************************************************************************/

/*! \file
    \brief      定义实时处理定值参数信息
    \author     chentm
    \version    1.0
    \date       2011-05-10
    
	实时处理过程中可能使用到一些参数、设定值，这些参数项在原始定值结构中不存在，
	或者是需要通过原始定值结构计算后得到。因此，定义这样的结构用于描述这些信息。
	这样做的优点：
	1.整个系统框架结构比较合理清晰，有利于系统的扩展；
	2.可以减少一些中间计算过程，有利于提高系统的整体性能；
	  (例如：对于越限返回值，如不利用结构存储起来，则每周波判断时均要有一次乘法运算)
*/

#ifndef __YH_DFR_RT_PARM_DEFINE_H
#define __YH_DFR_RT_PARM_DEFINE_H

#include "sg_def.h"
#include "trg_def.h"


#ifdef __cplusplus
extern "C" {
#endif

/*!
    \brief  模拟量通道实时处理附加参数
 */
typedef struct
{
    FLOAT32     HiTrgRtn;   /*!< 上限启动返回值 */
    FLOAT32     LoTrgRtn;   /*!< 下限启动返回值 */
	FLOAT32     SaTrgRrn;	/*!< 突变启动返回值 */
    FLOAT32     Ha2TrgRtn;  /*!< 2次谐波启动返回值 */
    FLOAT32     Ha3TrgRtn;  /*!< 3次谐波启动返回值 */
    FLOAT32     Ha5TrgRtn;  /*!< 5次谐波启动返回值 */
    FLOAT32     Ha7TrgRtn;  /*!< 7次谐波启动返回值 */
    FLOAT32     Ha9TrgRtn;  /*!< 9次谐波启动返回值 */
//	FLOAT32     SwgTrgRtn;  /*!< 振荡返回值 */
//	FLOAT32		OscSaTrgLev;/*!< 振荡突变启动值 */

	FLOAT32		Threshold;	/*!< 有效阀值 */

} RT_ANCH_SET;


/*!
    \brief  模拟量通道实时处理附加参数
 */
typedef struct
{
    FLOAT32     PSHiTrgRtn;     /*!< 正序上限返回值 */
    FLOAT32     PSLoTrgRtn;     /*!< 正序下限返回值 */
    FLOAT32     NSHiTrgRtn;     /*!< 负序上限返回值 */
    FLOAT32     ZSHiTrgRtn;     /*!< 零序上限返回值 */
  //  FLOAT32     ZSSaTrgRtn;     /*!< 零序突变返回值 */
    FLOAT32     SwgTrgRtn;      /*!< 振荡返回值 */

    FLOAT32     PHiTrgRtn;      /*!< 有功功率上限返回值(稳态、电流有效) */
    FLOAT32     PRteTrgRtn;     /*!< 有功变化率返回值(稳态、电流有效) */
    FLOAT32     QHiTrgRtn;      /*!< 无功功率上限返回值(稳态、电流有效) */
    FLOAT32     QRteTrgRtn;     /*!< 无功变化率返回值(稳态、电流有效) */
} RT_LIN_SET;


/*!
    \brief  变压器实时处理附加参数
 */
typedef struct
{
    FLOAT32     OvExTrgRtn;     	/*!< 过激磁返回值(励磁涌流相对于额定电流的倍数) */
    FLOAT32     ThrITrgRtn;     	/*!< 穿越电流返回值(穿越电流相对于额定电流的倍数，对于暂态而言无意义) */
    FLOAT32     DifATrgRtn;     	/*!< 差流返回值 */
    FLOAT32     ResvLev[4];     	/*!< 预留 */
    
} RT_TRN_SET;

/*!
	\brief 发电机实时处理附加参数
*/
typedef struct
{
	 FLOAT32	DifTrgRtn;			/*!< 完全纵差返回值，分别对应A相完全纵差返回值、B相完全纵差返回值、C相完全纵差返回值 */
	 FLOAT32	DifInTrgRtn;		/*!< 不完全纵差返回值，分别对应A相不完全纵差返回值、B相不完全纵差返回值、C相不完全纵差返回值 */
	 FLOAT32	DifTranTrgRtn;		/*!< 横差返回值，分别对应A相横差返回值、B相横差返回值、C相横差返回值 */

	 FLOAT32	OvExTrgRtn;     	/*!< 过激磁返回值 */
	 FLOAT32	LossExcTrgRtn;		/*!< 失磁启动定值 */
	 FLOAT32	StatorZeroTrgRtn;	/*!< 定子接地之零序电压返回值 */
	 FLOAT32	StatorHar3TrgRtn;	/*!< 定子接地之三次谐波电压比返回值 */
	 FLOAT32	ZeroSeqTrgRtn;		/*!< 纵向零序电压式定子绕组匝间短路返回值 */
	 FLOAT32	RotorHar2TrgRtn;	/*!< 转子二次谐波返回值 */
	 FLOAT32	ReversePoTrgRtn;	/*!< 逆功率返回值 */
	 FLOAT32	LowFreOvCurTrgRtn;	/*!< 低频过流返回值 */
}RT_GN_SET;

/*!
    \brief  模拟量通道参数，返回值
 */
typedef struct
{
     YH_ANCH_CFG        AnCfg;      /*!< 模拟通道基本配置参数(公用信息) */
     YH_ANCH_SET        AnTrnsSet;  /*!< 暂态定值 */
     YH_ANCH_SET        AnContSet;  /*!< 稳态定值 */
     YH_PQ_ANCH_SET		AnPQSet;	/*!< 电能质量定值 add by libo */
     
     RT_ANCH_SET		RtTrnsSet;	/*!< 暂态实时处理参数 */
     RT_ANCH_SET		RtContSet;	/*!< 稳态实时处理参数 */
} RT_ANCH_PARM;

/*!
    \brief  电压通道参数，返回值
 */
typedef struct
{
     YH_ULIN_CFG        LinCfg;         /*!< 电压基本配置参数(公用信息) */
     YH_LIN_SET         LinTrnsSet;     /*!< 线路暂态启动定值 */
     YH_LIN_SET         LinContSet;     /*!< 线路稳态标记事件定值 */
     
     RT_LIN_SET			RtTrnsSet;		/*!< 暂态实时处理参数 */
     RT_LIN_SET			RtContSet;		/*!< 稳态实时处理参数 */
} RT_ULIN_PARM;

/*!
    \brief  电流通道参数，返回值
 */
typedef struct
{
     YH_ILIN_CFG        LinCfg;         /*!< 线路基本配置参数(公用信息，其中包含了分通道文件信息) */
     YH_ILIN_FLO_SET    FloSet;         /*!< 测距定值参数 */
     YH_LIN_SET         LinTrnsSet;     /*!< 线路暂态启动定值 */
     YH_LIN_SET         LinContSet;     /*!< 线路稳态标记事件定值 */

     RT_LIN_SET			RtTrnsSet;		/*!< 暂态实时处理参数 */
     RT_LIN_SET			RtContSet;		/*!< 稳态实时处理参数 */
} RT_ILIN_PARM;

/*!
    \brief  变压器参数，返回值
 */
typedef struct
{
    YH_TRN_CFG     TrnCfg;         /*!< 变压器基本配置参数(公用信息) */
    YH_TRN_SET     TrnTrnsSet;     /*!< 变压器暂态启动定值 */
    YH_TRN_SET     TrnContSet;     /*!< 变压器稳态标记事件定值 */

	RT_TRN_SET     RtTrnsSet;     /*!< 变压器暂态启动返回定值 */
    RT_TRN_SET     RtContSet;     /*!< 变压器稳态标记事件返回定值 */
} RT_TRN_PARM;

/*!
	\brief	发电机参数， 返回值
*/
typedef struct
{
	YH_GN_CFG		GnCfg;		 /*!< 发电机基本配置参数 */
	YH_GN_SET		GnTrnsSet;	 /*!< 发电机暂态启动定值 */
	YH_GN_SET		GnContSet;	 /*!< 发电机稳态标记事件定值 */

	RT_GN_SET		RtTrnsSet;	 /*!< 发电机暂态启动返回定值 */
	RT_GN_SET		RtContSet;	 /*!< 发电机稳态标记事件返回定值*/
}RT_GN_PARM;


typedef struct yh_binch_parm	RT_BINCH_PARM;
//typedef struct sh_trn_parm		RT_TRN_PARM;
//typedef struct sh_gn_parm		RT_GN_PARM;

typedef struct
{
    YH_DFR_PARM*        pDFRParm;      						/*!< 全局参数 */
    RT_ANCH_PARM*       pAnChParm;      					/*!< 模拟量通道信息 */
    RT_BINCH_PARM*      pBinChParm;     					/*!< 状态量通道信息 */
    RT_ULIN_PARM*       pULinParm;      					/*!< 电压量信息 */
    RT_ILIN_PARM*       pILinParm;      					/*!< 线路信息 */
    RT_TRN_PARM*        pTrnParm;       					/*!< 变压器信息 */
    RT_GN_PARM*         pGnParm;        					/*!< 发电机信息 */

    INT16U				BinTrgTrnsMsk[MAX_BINCH_WORD_NUM];	/*!< 开关量通道暂态启动掩码    */
    INT16U				BinTrgContMsk[MAX_BINCH_WORD_NUM];	/*!< 开关量通道稳态启动掩码    */
 
    INT32U              BufLen;         					/*!< 数据区长度 */
    INT32U*             DataAddr;       					/*!< 数据区起始地址 */
} RTDFR_SG;

/*!
	\brief	初始化定值结构信息
	\param	rt_sg		[in,out]	定值组文件
	\param	dfr_sg		[in]		定值数据区
	\return	成功返回0,否则返回错误号
*/
int init_rt_sg( RTDFR_SG* rt_sg, SHDFR_SG* dfr_sg );

RTDFR_SG* create_rt_sg( SHDFR_SG* dfr_sg );

void destory_rt_sg( RTDFR_SG* rt_sg );

#ifdef __cplusplus
}
#endif

#endif	/*__YH_DFR_RT_PARM_DEFINE_H*/
