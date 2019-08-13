/*************************************************************************
* Copyright (c) 2011,深圳市YHF电气技术有限公司
* All rights reserved.

**************************************************************************/

/*!	\file
 	\brief		计数录波器中相关数据单元对象的长度
 	
	\author		chentm
	\version	1.0
	\date		2011-08-23
*/

#ifndef	__SH_RECORD_DATA_UNIT_LEN_DEFINE_H
#define	__SH_RECORD_DATA_UNIT_LEN_DEFINE_H

#include "sg_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 	\brief	获取一个模拟量采样点的长度
 	\return	返回模拟量采样点的长度
 */
int rdu_getAnSmpPntLen ( int AnChCnt  );

/*!
 	\brief	计算一个开关量采样点的长度
 	\return	返回开关量采样点的长度
 */
int rdu_getBinSmpPntLen( int BinChCnt );


/*!
 	\brief	计算一个计算量点的长度
 	\return	返回计算量点的长度
// */
int rdu_getCalcPntLen( YH_DFR_CFG* dfr_cfg );

///*!
// 	\brief	计算实时数据帧的长度
// 	\return	返回实时数据帧的长度
// */
//int rdu_getRtFrmLen  ( YH_DFR_PARM* dfr_parm );

///*!
// 	\brief	计算稳态数据帧的长度
// 	\return	返回稳态数据帧的长度
// */
//int rdu_getContFrmLen( YH_DFR_PARM* dfr_parm );

#ifdef __cplusplus
}
#endif

#endif	/*__SH_RECORD_DATA_UNIT_LEN_DEFINE_H*/
