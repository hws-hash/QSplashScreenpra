/*************************************************************************
* Copyright (c) 2011,深圳市元恒丰电气技术有限公司
* All rights reserved.

**************************************************************************/

/*!	\file
 	\brief		计数录波器中相关数据单元对象的长度
 	
	\author		chen
	\version	1.0
	\date		2011-08-23
*/

#include "rdu_len.h"
#include "rt_ctl.h"
#include "shd_def.h"

/*!
 	\brief	计算一个模拟量采样数点的长度
 */
int rdu_getAnSmpPntLen( int AnChCnt )
{	
	return AnChCnt * RAW_AN_SMP_DATA_LEN;
}

/*!
 	\brief	计算一个开关量数据采样数点的长度
 */
int rdu_getBinSmpPntLen( int BinChCnt )
{	//每个开关量采样数据占用一位，且总的占用空间按4字节对齐
//	int bin_len_per_pnt = BinChCnt / 8 + ( BinChCnt % 8 == 0 ? 0 : 1 );
//	if ( bin_len_per_pnt % 4 != 0 )
//	{
//		bin_len_per_pnt = bin_len_per_pnt + 4 - bin_len_per_pnt % 4;
//	}
//	return bin_len_per_pnt;

  //每个开关量采样数据占用一位，且总的占用空间按2字节对齐
  return ((BinChCnt+15)/16)*2;
}

int rdu_getCalcPntLen( YH_DFR_CFG* dfr_cfg )
{
    return ( sizeof(YHFtimeval)+
                    sizeof(ANCH_VAL)*dfr_cfg->AnChCnt +
                    sizeof(ULIN_VAL)*dfr_cfg->ULinCnt +
                    sizeof(ILIN_VAL)*dfr_cfg->ILinCnt +
                    sizeof(GN_VAL)  *dfr_cfg->GnCnt   +
                    sizeof(TRN_VAL) *dfr_cfg->TrnCnt );
}

//int rdu_getRtFrmLen( YH_DFR_PARM* dfr_parm )
//{
//	int smp_pnt_len = sizeof(YHFtimeval) +
//					  rdu_getAnSmpPntLen( dfr_parm->DfrCfg.AnChCnt ) +
//					  rdu_getBinSmpPntLen( dfr_parm->DfrCfg.BinChCnt );
	
//	return ( sizeof(FRM_HEAD_RT) +	rdu_getCalcPntLen(&dfr_parm->DfrCfg) +
//					  dfr_parm->DfrCfg.RtCycCnt * smp_pnt_len *
//					  dfr_parm->DfrCfg.SmpCntPerCyc + 4);
//}

//int rdu_getContFrmLen( YH_DFR_PARM* dfr_parm )
//{
//	int smp_pnt_len = sizeof(YHFtimeval) +
//					  rdu_getAnSmpPntLen( dfr_parm->DfrCfg.AnChCnt ) +
//					  rdu_getBinSmpPntLen( dfr_parm->DfrCfg.BinChCnt );
	
//	return ( sizeof(FRM_HEAD_CONT) +  rdu_getCalcPntLen(&dfr_parm->DfrCfg) + smp_pnt_len *
//			dfr_parm->DfrContSet.SmpRte + 4 ) ;
//}
