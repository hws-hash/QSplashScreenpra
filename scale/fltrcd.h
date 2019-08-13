/*************************************************************************
* Copyright (c) 2011,深圳市元恒丰电气技术有限公司
* All rights reserved.

**************************************************************************/

/*!	\file
    \brief
    \author		chen
    \version	1.0
    \date		2011-03-18
*/

#ifndef	__YH_DIGIT_FAULT_RECOED_H
#define	__YH_DIGIT_FAULT_RECOED_H

//#include "dbg_ctl.h"
#include "rt_ctl.h"
//#include "rt_analyse.h"
#include "rcd_ctl.h"
#include "shd_ctl.h"
//#include "mkshd.h"
//#include "wave_smp_ctl.h"

#ifdef __cplusplus
extern "C" {
#endif

extern RT_CTL*			g_rtCtl;		/*!< 实时数据控制 */
extern TRNS_RCD_CTL*	g_rcdCtl;		/*!< 录波控制 */
//extern WAVE_SMP_CTL*	g_waveCtl;		/*!< 实时波形数据抽取控制 */

int fltrcd_init( SHDFR_SG* pSg );
int fltrcd_reinit();
int fltrcd_destory();

/*!
    \brief	实时处理
    \param	cyc_cnt		周波数
    \return	标志是否重新初始化。如果系统已经重新初始化（如重新加载定值），返回1.否则返回0.
*/
int rt_proc( int cyc_cnt );


void rt_calc( CYC_DATA_IDX* pCycData );

/*!
    \brief	从原始采样缓冲区中再抽取数据
    \param	dst_addr	目的地址
    \param	start_pos	抽取数据的起始位置
    \param	pnt_cnt		抽取点的个数
    \param	step		抽取的步长（即使隔step个点抽取一个点）
*/
void smpdata_from_rtbuf( INT8U* dst_addr, int start_pos, int pnt_cnt, int step );


/*!
    \brief	暂态录波处理
    实时暂态录波处理，每处理一周波的数据，调用一次。
 */
void trns_rcd_proc( int cur_pos );

void cont_rcd_proc( int cur_pos );

void pq_rcd_proc( int cur_pos );

//char* FltSegFlg_itoa( int SegFlg );
void  print_rt_ctl  ( RT_CTL* prtCtl, FILE* fp );
void  print_shd_ctl ( MK_SHD_CTL* pShdCtl, FILE* fp );
void  print_rcd_ctl ( TRNS_RCD_CTL* prcdCtl, FILE* fp );
//void  print_flt_info( FAULT_INFO* pfltInfo, FILE* fp );

#ifdef __cplusplus
}
#endif

#endif	//__YH_DIGIT_FAULT_RECOED_H
