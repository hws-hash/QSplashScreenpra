/*************************************************************************
* Copyright (c) 2010,深圳市YHF电气技术有限公司
* All rights reserved.

**************************************************************************/

/*!	\file
 	\brief		实现一些最基本的基础的算法工具函数
 	
	\author		Chen
	\version	1.0
	\date		2010-09-20
	
*/

#ifndef	__SH_CALC_BASIC_UTILITY_DEFINE_H
#define	__SH_CALC_BASIC_UTILITY_DEFINE_H

#ifdef __cplusplus
extern "C" {
#endif

/*!
	\brief	初始化DFT计算所需的正、余弦系数表
	\param	pcoeff	[in]	系数表起始地址
	\param	k		[in]	谐波次数
	\param	N		[in]	每周波采样点数
	\param  skew    [in]    角度偏移
*/
void init_coefficient_table( float* pcoeff, int k, int N, float skew );

/*!
	\brief	生成DFT计算所需的正、余弦系数青
	\param	k		[in]	谐波次数
	\param	N		[in]	每周波采样点数
	\param  skew    [in]    角度偏移	
*/
float* creat_coefficient_table( int k, int N, float skew );


/*!
	\brief	DFT计算，基波实部 虚部和有效值, 谐波有效值
	\param	pfsrc		[in]	原始采样数据起始地址
	\param	fcnt		[in]	原始采样点的个数
	\param	pcoeff		[in]	正余弦系数表首地址
	\param	pComplex	[out]	指向计算结果结构的指针
*/
void calc_DFT( float* pfsrc, int fcnt, float* pcoeff, COMPLEX_R* pComplex );


/*!
	\brief	直流计算
	\param	pfdata		[in]	原始采样数据起始地址
	\param	fcnt		[in]	原始采样点的个数
	\param	pComplex	[out]	指向计算结果结构的指针
*/
void calc_DC( float* pfdata, int fcnt, COMPLEX_R* pComplex );


/*!
	\brief	频率计算
	\param	pCur	[in]	当前周波实虚部
	\param	pPrev	[in]	前一周波实虚部
	\return 返回计算结果
*/
float calc_freq( COMPLEX* pCur, COMPLEX* pPrev );


#ifdef __cplusplus
}
#endif


#endif	/* __SH_CALC_BASIC_UTILITY_DEFINE_H */
