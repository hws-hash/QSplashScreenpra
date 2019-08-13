/*************************************************************************
* Copyright (c) 2011,深圳市元恒丰电气技术有限公司
* All rights reserved.

**************************************************************************/

/*!	\file
 	\brief		实时定值结构相关函数的实现
 	
	\author		chen
	\version	1.0
	\date		2011-05-10
*/

/*! \add    
	\brief      添加频率启动、变压器启动返回值
	\author     chen
	\date		2011-12-28
*/
	


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rt_sg.h"
//#include "rcd_mem.h"

#if 1
#define dbg_warning( _argv_ )	{ printf( "line:%-4d file: %s \n",__LINE__,__FILE__ ); printf  _argv_; }
#define dbg_error( _argv_ )		{ printf( "line:%-4d file: %s \n",__LINE__,__FILE__ ); printf  _argv_; }
#else
#define dbg_error(_argv_)		
#define dbg_warning(_argv_)		
#endif

static int initRtSg( RTDFR_SG* rt_sg, void* sg_data, int sg_size );

static void sg2rt_anch( YH_ANCH_SET* pSgSet, RT_ANCH_SET* pRtSet )
{
	pRtSet->HiTrgRtn  = pSgSet->HiTrgLev  * 0.98f;
	pRtSet->LoTrgRtn  = pSgSet->LoTrgLev  * 1.02f;
	pRtSet->Ha2TrgRtn = pSgSet->Ha2TrgLev * 0.98f;
	pRtSet->Ha3TrgRtn = pSgSet->Ha3TrgLev * 0.98f;
	pRtSet->Ha5TrgRtn = pSgSet->Ha5TrgLev * 0.98f;
	pRtSet->Ha7TrgRtn = pSgSet->Ha7TrgLev * 0.98f;
	pRtSet->Ha9TrgRtn = pSgSet->Ha9TrgLev * 0.98f;
	pRtSet->Threshold = 0.0f;
}

static void sgrt_uline(YH_LIN_SET* uiSgSet, RT_LIN_SET* uiRtSet)
{
	uiRtSet->NSHiTrgRtn = uiSgSet->NSHiTrgLev * 0.98f;
	uiRtSet->PHiTrgRtn  = uiSgSet->PHiTrgLev  * 0.98f;
	uiRtSet->PRteTrgRtn = uiSgSet->PRteTrgLev * 0.98f;
	uiRtSet->PSHiTrgRtn = uiSgSet->PSHiTrgLev * 0.98f;
	uiRtSet->PSLoTrgRtn = uiSgSet->PSLoTrgLev * 1.02f;
	uiRtSet->QHiTrgRtn  = uiSgSet->QHiTrgLev  * 0.98f;
	uiRtSet->QRteTrgRtn = uiSgSet->QRteTrgLev * 0.98f;
	uiRtSet->SwgTrgRtn  = uiSgSet->SwgTrgLev  * 1.00f;
	uiRtSet->ZSHiTrgRtn = uiSgSet->ZSHiTrgLev * 0.98f;
}

void sgrt_trans(YH_TRN_SET* transSet, RT_TRN_SET* transRt)
{
	transRt->DifATrgRtn = transSet->DifATrgLev * 0.98;
	transRt->OvExTrgRtn = transSet->OvExTrgLev * 0.95; 
}

void sgrt_gn(YH_GN_SET* gnSet, RT_GN_SET* gnRt)
{
	gnRt->DifTrgRtn			=  gnSet->DifTrgLev			* 0.98f;
	gnRt->DifInTrgRtn		=  gnSet->DifInTrgLev		* 0.98f;
	gnRt->DifTranTrgRtn		=  gnSet->DifTranTrgLev		* 0.98f;

	gnRt->LossExcTrgRtn		=  gnSet->LossExcTrgLev		* 1.02f;
	gnRt->LowFreOvCurTrgRtn =  gnSet->LowFreOvCurTrgLev * 0.98f;
	gnRt->OvExTrgRtn		=  gnSet->OvExTrgLev		* 0.95f;
	gnRt->ReversePoTrgRtn	=  gnSet->ReversePoTrgLev	* 0.95f;
	gnRt->RotorHar2TrgRtn	=  gnSet->RotorHar2TrgLev	* 0.98f;
	gnRt->StatorHar3TrgRtn  =  gnSet->StatorHar3TrgLev	* 0.98f;
	gnRt->StatorZeroTrgRtn  =  gnSet->StatorZeroTrgLev	* 0.98f;
	gnRt->ZeroSeqTrgRtn		=  gnSet->ZeroSeqTrgLev		* 0.98f;
}

int init_rt_sg( RTDFR_SG* rt_sg, SHDFR_SG* dfr_sg )
{
	int 			i,j,k,ret,sg_size;
	char			*data;
	INT16U			tmp;
	YH_DFR_CFG		*pDfrCfg;
	
	if ( rt_sg->pDFRParm == NULL ||
		 rt_sg->pDFRParm->DfrCfg.AnChCnt  != dfr_sg->pDFRParm->DfrCfg.AnChCnt ||
		 rt_sg->pDFRParm->DfrCfg.BinChCnt != dfr_sg->pDFRParm->DfrCfg.BinChCnt ||
		 rt_sg->pDFRParm->DfrCfg.ULinCnt  != dfr_sg->pDFRParm->DfrCfg.ULinCnt ||
		 rt_sg->pDFRParm->DfrCfg.ILinCnt  != dfr_sg->pDFRParm->DfrCfg.ILinCnt ||
		 rt_sg->pDFRParm->DfrCfg.FLinCnt  != dfr_sg->pDFRParm->DfrCfg.FLinCnt ||
		 rt_sg->pDFRParm->DfrCfg.TrnCnt   != dfr_sg->pDFRParm->DfrCfg.TrnCnt ||
		 rt_sg->pDFRParm->DfrCfg.GnCnt    != dfr_sg->pDFRParm->DfrCfg.GnCnt )
	{
		if ( rt_sg->DataAddr )
		{	/* 释放原来的空间 */
			free( rt_sg->DataAddr );
			memset( rt_sg, 0, sizeof( RTDFR_SG ) );
		}
		
		pDfrCfg = &dfr_sg->pDFRParm->DfrCfg;
		sg_size = sizeof(YH_DFR_PARM)
					+ sizeof(RT_ANCH_PARM) * pDfrCfg->AnChCnt
					+ sizeof(RT_BINCH_PARM)* pDfrCfg->BinChCnt
					+ sizeof(RT_ILIN_PARM) * pDfrCfg->ILinCnt
					+ sizeof(RT_ULIN_PARM) * pDfrCfg->ULinCnt
					+ sizeof(RT_TRN_PARM)  * pDfrCfg->TrnCnt
					+ sizeof(RT_GN_PARM)   * pDfrCfg->GnCnt;
	
		data = (char*) calloc ( 1, sg_size );
		if ( NULL == data )
		{
			dbg_error(( "No memory!" ));
			return -1;
		}
		
		memcpy( data, dfr_sg->pDFRParm, sizeof(YH_DFR_PARM) );
		
		ret = initRtSg( rt_sg, data, sg_size );
		if ( ret != 0 )
		{
			dbg_error(( "initRtSg() failed!" ));
			free( data );
			return -1;
		}
	}
	
	/*! 频率返回值 */
	rt_sg->pDFRParm->HzTrnsSet.HzDfTrgRtn = (float)(0.95 * dfr_sg->pDFRParm->HzTrnsSet.HzDfTrgLev);
	rt_sg->pDFRParm->HzTrnsSet.HzHiTrgRtn = (float)(0.9 * dfr_sg->pDFRParm->HzTrnsSet.HzHiTrgLev + 0.1 * dfr_sg->pDFRParm->DfrCfg.Freq );
	rt_sg->pDFRParm->HzTrnsSet.HzLoTrgRtn = (float)(0.9 * dfr_sg->pDFRParm->HzTrnsSet.HzLoTrgLev + 0.1 * dfr_sg->pDFRParm->DfrCfg.Freq );
	rt_sg->pDFRParm->HzContSet.HzDfTrgRtn = (float)(0.95 * dfr_sg->pDFRParm->HzContSet.HzDfTrgLev);
	rt_sg->pDFRParm->HzContSet.HzHiTrgRtn = (float)(0.9 * dfr_sg->pDFRParm->HzContSet.HzHiTrgLev + 0.1 * dfr_sg->pDFRParm->DfrCfg.Freq );
	rt_sg->pDFRParm->HzContSet.HzLoTrgRtn = (float)(0.9 * dfr_sg->pDFRParm->HzContSet.HzLoTrgLev + 0.1 * dfr_sg->pDFRParm->DfrCfg.Freq );


	//模拟量通道
	for( i = 0; i < rt_sg->pDFRParm->DfrCfg.AnChCnt; i++ )
	{
		(rt_sg->pAnChParm + i)->AnCfg     = (dfr_sg->pAnChParm + i)->AnCfg;
		(rt_sg->pAnChParm + i)->AnTrnsSet = (dfr_sg->pAnChParm + i)->AnTrnsSet;
		(rt_sg->pAnChParm + i)->AnContSet = (dfr_sg->pAnChParm + i)->AnContSet;
		(rt_sg->pAnChParm + i)->AnPQSet   = (dfr_sg->pAnChParm + i)->AnPQSet;
		
		sg2rt_anch( &(rt_sg->pAnChParm + i)->AnTrnsSet, &(rt_sg->pAnChParm + i)->RtTrnsSet );
		sg2rt_anch( &(rt_sg->pAnChParm + i)->AnContSet, &(rt_sg->pAnChParm + i)->RtContSet );
	}
	
	//开关量通道
	for( i = 0; i < rt_sg->pDFRParm->DfrCfg.BinChCnt; i++ )
	{
		(rt_sg->pBinChParm + i)->BinCfg     = (dfr_sg->pBinChParm + i)->BinCfg;
		(rt_sg->pBinChParm + i)->BinTrnsSet = (dfr_sg->pBinChParm + i)->BinTrnsSet;
		(rt_sg->pBinChParm + i)->BinContSet = (dfr_sg->pBinChParm + i)->BinContSet;
	}
	
	//电压量
	for( i = 0; i < rt_sg->pDFRParm->DfrCfg.ULinCnt; i++ )
	{
		(rt_sg->pULinParm + i)->LinCfg     = (dfr_sg->pULinParm + i)->LinCfg;
		(rt_sg->pULinParm + i)->LinTrnsSet = (dfr_sg->pULinParm + i)->LinTrnsSet;
		(rt_sg->pULinParm + i)->LinContSet = (dfr_sg->pULinParm + i)->LinContSet;

		sgrt_uline(&(rt_sg->pULinParm + i)->LinTrnsSet, &(rt_sg->pULinParm + i)->RtTrnsSet);
		sgrt_uline(&(rt_sg->pULinParm + i)->LinContSet, &(rt_sg->pULinParm + i)->RtContSet);
	}
	
	//电流量
	for( i = 0; i < rt_sg->pDFRParm->DfrCfg.ILinCnt; i++ )
	{
		(rt_sg->pILinParm + i)->LinCfg     = (dfr_sg->pILinParm + i)->LinCfg;
		(rt_sg->pILinParm + i)->LinTrnsSet = (dfr_sg->pILinParm + i)->LinTrnsSet;
		(rt_sg->pILinParm + i)->LinContSet = (dfr_sg->pILinParm + i)->LinContSet;

		sgrt_uline(&(rt_sg->pILinParm + i)->LinTrnsSet, &(rt_sg->pILinParm + i)->RtTrnsSet);
		sgrt_uline(&(rt_sg->pILinParm + i)->LinContSet, &(rt_sg->pILinParm + i)->RtContSet);
	}
	
	//变压器
	for( i = 0; i < rt_sg->pDFRParm->DfrCfg.TrnCnt; i++ )
	{
		(rt_sg->pTrnParm + i)->TrnCfg     = (dfr_sg->pTrnParm + i)->TrnCfg;
		(rt_sg->pTrnParm + i)->TrnTrnsSet = (dfr_sg->pTrnParm + i)->TrnTrnsSet;
		(rt_sg->pTrnParm + i)->TrnContSet = (dfr_sg->pTrnParm + i)->TrnContSet;

		sgrt_trans(&(rt_sg->pTrnParm + i)->TrnContSet, &(rt_sg->pTrnParm + i)->RtContSet);
		sgrt_trans(&(rt_sg->pTrnParm + i)->TrnTrnsSet, &(rt_sg->pTrnParm + i)->RtTrnsSet);
	}
	
	//发电机
	for( i = 0; i < rt_sg->pDFRParm->DfrCfg.GnCnt; i++ )
	{
		(rt_sg->pGnParm + i)->GnCfg     = (dfr_sg->pGnParm + i)->GnCfg;
		(rt_sg->pGnParm + i)->GnTrnsSet = (dfr_sg->pGnParm + i)->GnTrnsSet;
		(rt_sg->pGnParm + i)->GnContSet = (dfr_sg->pGnParm + i)->GnContSet;

		sgrt_gn( &(rt_sg->pGnParm + i)->GnTrnsSet, &(rt_sg->pGnParm + i)->RtTrnsSet);
		sgrt_gn( &(rt_sg->pGnParm + i)->GnContSet, &(rt_sg->pGnParm + i)->RtContSet);
	}
  memset( rt_sg->BinTrgTrnsMsk, 0, sizeof(rt_sg->BinTrgTrnsMsk) );
	//计算开关量启动掩码
//	memset( rt_sg->BinTrgTrnsMsk, 0xFF, sizeof(rt_sg->BinTrgTrnsMsk) );
//	memset( rt_sg->BinTrgContMsk, 0xFF, sizeof(rt_sg->BinTrgContMsk) );
//	for( i = 0; i < rt_sg->pDFRParm->DfrCfg.BinChCnt; i++ )
//	{
//		j = i / 16;
//		k = i % 16;
//		tmp = 0x01 << k;
		
//		if ( (rt_sg->pBinChParm+i)->BinTrnsSet.TrgEna == 0 )
//			rt_sg->BinTrgTrnsMsk[j] &= ~tmp;
		
//		if ( (rt_sg->pBinChParm+i)->BinContSet.TrgEna == 0 )
//			rt_sg->BinTrgContMsk[j] &= ~tmp;
		
//	}
	return 0;
}

RTDFR_SG* create_rt_sg( SHDFR_SG* dfr_sg )
{
	int			ret;
	RTDFR_SG*   rt_sg;
	
	rt_sg = (RTDFR_SG*) calloc ( 1, sizeof(RTDFR_SG) );
	if ( NULL == rt_sg )
	{
		dbg_error(( "No memory!" ));
		return NULL;
	}
	
	ret = init_rt_sg( rt_sg, dfr_sg );
	if ( ret != 0 )
	{
		dbg_error(( "init_rt_sg() failed!" ));
		destory_rt_sg( rt_sg );
		return NULL;
	}
	return rt_sg;
}

void destory_rt_sg( RTDFR_SG* rt_sg )
{
	if ( NULL == rt_sg )
		return;

	if ( rt_sg->DataAddr )
	{
		free( rt_sg->DataAddr );
		rt_sg->DataAddr = NULL;
	}
	
	free( rt_sg );
	rt_sg = NULL;
}

static int initRtSg( RTDFR_SG* rt_sg, void* sg_data, int sg_size )
{
	int   pos = 0;
	char* data = (char*)sg_data;
	
	rt_sg->DataAddr = (INT32U*)data;
	rt_sg->BufLen   = sg_size;
	rt_sg->pDFRParm = (YH_DFR_PARM*)(data + pos);
	pos += sizeof( YH_DFR_PARM );
	
	//模拟量
	rt_sg->pAnChParm = ( rt_sg->pDFRParm->DfrCfg.AnChCnt == 0 ) ? 
		NULL : (RT_ANCH_PARM*)(data + pos);
	pos += sizeof( RT_ANCH_PARM ) * rt_sg->pDFRParm->DfrCfg.AnChCnt;

	//开关量
	rt_sg->pBinChParm = ( rt_sg->pDFRParm->DfrCfg.BinChCnt == 0 ) ? 
		NULL : (RT_BINCH_PARM*)(data + pos);
	pos += sizeof( RT_BINCH_PARM ) * rt_sg->pDFRParm->DfrCfg.BinChCnt;

	//电流量
	rt_sg->pILinParm = ( rt_sg->pDFRParm->DfrCfg.ILinCnt == 0 ) ? 
		NULL : (RT_ILIN_PARM*)(data + pos);
	pos += sizeof( RT_ILIN_PARM ) * rt_sg->pDFRParm->DfrCfg.ILinCnt;

	//电压量
	rt_sg->pULinParm = ( rt_sg->pDFRParm->DfrCfg.ULinCnt == 0 ) ? 
		NULL : (RT_ULIN_PARM*)(data + pos);
	pos += sizeof( RT_ULIN_PARM ) * rt_sg->pDFRParm->DfrCfg.ULinCnt;

	//变压器
	rt_sg->pTrnParm = ( rt_sg->pDFRParm->DfrCfg.TrnCnt == 0 ) ? 
		NULL : (RT_TRN_PARM*)(data + pos);
	pos += sizeof( RT_TRN_PARM ) * rt_sg->pDFRParm->DfrCfg.TrnCnt;
	
	//发电机
	rt_sg->pGnParm = ( rt_sg->pDFRParm->DfrCfg.GnCnt == 0 ) ? 
		NULL : (RT_GN_PARM*)(data + pos);
	pos += sizeof( RT_GN_PARM ) * rt_sg->pDFRParm->DfrCfg.GnCnt;
	
	
	//定值数据长度必须等于各个数据段长度之和
	if ( pos != sg_size )
	{
		dbg_error(( "RTSG data not correct!" ));
		return -1;
	}
	return 0;
}
