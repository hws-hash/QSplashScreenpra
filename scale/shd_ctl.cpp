/*************************************************************************
* Copyright (c) 2011,深圳市元恒丰电气技术有限公司
* All rights reserved.

**************************************************************************/

/*!	\file
 	\brief		
 	
	\author		chen
	\version	1.0
	\date		2011-03-25
	
*/

#include <stdio.h>
#include <assert.h>
#include "shd_ctl.h"
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "rdu_len.h"
//#include "rcd_mem.h"
//#include "rcd_dbg.h"
//#include "sg_mgr.h"
//#include "dfrserver_config.h"
//extern SGManager g_sgMgr;

MK_SHD_CTL* shd_ctl_create( int buf_len )
{
	MK_SHD_CTL* pShdCtl;
	pShdCtl = (MK_SHD_CTL*) calloc( 1, sizeof(MK_SHD_CTL) + buf_len );
	if ( NULL == pShdCtl )
		return NULL;
	
	pShdCtl->BufAddr = (INT8U*)(pShdCtl + 1);
	pShdCtl->BufLen  = buf_len;
	return pShdCtl;
}

static int get_smp_pnt_per_cyc( int rate, int freq )
{
	int pnt = 0;
	if ( rate > freq )
		pnt = rate / freq; 
	else
		pnt = -(freq / rate);
	
	return pnt;
}

//static int  my_dfr_sg_cpy( SHDFR_SG* sg_dst, const SHDFR_SG* sg_src, int flg )
//{
//	int  ret;
//	void *data;

//	if ( sg_src == NULL || sg_dst == NULL )
//	{
//		printf( "sg_src == NULL || sg_dst == NULL\n" );
//		return -1;
//	}

//	if ( sg_src == sg_dst )
//	{
//		printf( "sg_src == sg_dst.\n" );
//		return 0;
//	}
	
//	if ( sg_src->BufLen == 0 || sg_src->DataAddr == NULL )
//	{
//		printf( "source sg data is NULL!\n" );
//		return -1;
//	}

//	if ( sg_src->BufLen == sg_dst->BufLen &&
//		sg_comp_dfrcfg( sg_dst, sg_src ) == 0 )
//	{
//		memcpy( sg_dst->DataAddr, sg_src->DataAddr, sg_src->BufLen );
//		return 0;
//	}

//	if ( flg == 0 )
//	{
//		printf( "sg data not match!\n" );
//		return -1;
//	}

//	data = calloc( 1, sg_src->BufLen );
//	if ( NULL == data )
//	{
//		printf( "Error: no memory!\n" );
//		return -1;
//	}
	
//	if ( sg_dst->DataAddr )
//	{
//		free( sg_dst->DataAddr );
//		sg_dst->DataAddr = NULL;
//		sg_dst->BufLen = 0;
//	}
	
//	memcpy( data, sg_src->DataAddr, sg_src->BufLen );
//	ret = initDfrSg( sg_dst, data, sg_src->BufLen, 1 );
//	if ( ret != 0 )
//	{
//		printf( "initDfrSg() failed!\n" );
//		free( data );
//		return -1;
//	}
	
//	return 0;
//}

//static int read_conf_FltNum()
//{
//	FILE* fp;
//	char  szLine[256] = {0};
//	fp = fopen( CONF_FNAME_FLTNUM, "r" );
//	if ( NULL == fp )
//		return 0;
	
//	fgets ( szLine, 255, fp );
//	fclose( fp );
	
//	return atoi( szLine );
//}


//int shd_ctl_init( MK_SHD_CTL* pShdCtl, SHDFR_SG* psg, FAULT_INFO* flt_info, void* sg_data, int data_len )
//{
//	YH_DFR_CFG *pDfrCfg = &psg->pDFRParm->DfrCfg;
	
//	pShdCtl->AnChCnt	= pDfrCfg->AnChCnt;
//	pShdCtl->BinChCnt	= pDfrCfg->BinChCnt;
	
//	pShdCtl->AnSmpLenPerPnt = rdu_getAnSmpPntLen ( pShdCtl->AnChCnt );
//	pShdCtl->BinLenPerPnt   = rdu_getBinSmpPntLen( pShdCtl->BinChCnt );
//	pShdCtl->AnCompLenPerPnt= pShdCtl->AnChCnt * sizeof(COMPLEX);

//	pShdCtl->SmpLenPerPnt   = sizeof(YHFtimeval) +
//							  pShdCtl->AnSmpLenPerPnt +
//							  pShdCtl->BinLenPerPnt;

//	pShdCtl->CompLenPerPnt  = sizeof(SMP_TIMESTAMP) +
//							  pShdCtl->AnCompLenPerPnt +
//							  pShdCtl->BinLenPerPnt;

//	pShdCtl->A2SmpPntPerCyc	= get_smp_pnt_per_cyc( psg->pDFRParm->DfrTrnsSet.A2SmpRte, pDfrCfg->Freq);
//	pShdCtl->A1SmpPntPerCyc	= get_smp_pnt_per_cyc( psg->pDFRParm->DfrTrnsSet.A1SmpRte, pDfrCfg->Freq);
//	pShdCtl->BSmpPntPerCyc	= get_smp_pnt_per_cyc( psg->pDFRParm->DfrTrnsSet.BSmpRte , pDfrCfg->Freq);
//	pShdCtl->CSmpPntPerCyc	= get_smp_pnt_per_cyc( psg->pDFRParm->DfrTrnsSet.CSmpRte , pDfrCfg->Freq);
//	pShdCtl->DSmpPntPerCyc	= get_smp_pnt_per_cyc( psg->pDFRParm->DfrTrnsSet.DSmpRte , pDfrCfg->Freq);

//	pShdCtl->FlgFWriteShd	= 0;	//ctm
	
//	/* Copy一份定值 */
//	dfr_sg_cpy( &pShdCtl->DfrSg, psg, 1 );

//	/* 初始化故障信息 */
//	pShdCtl->CurFltInfo = flt_info;
//	pShdCtl->CurFltSeg  = NULL;
//	pShdCtl->DataLen	= 0;

//	pShdCtl->ShdFrmCnt		= 0;
//	pShdCtl->ShdFltCycCnt	= 0;
//	pShdCtl->ShdSegCycCnt	= 0;
	
//	if ( pShdCtl->ShdFp )
//		fclose( pShdCtl->ShdFp );
	
//	pShdCtl->ShdFp			= 0;
//	pShdCtl->ShdFileName[0] = 0;
//	pShdCtl->ShdFWriteLen	= 0;
	
//	pShdCtl->FltNum = read_conf_FltNum();
	
//	return 0;
//}


//int shd_ctl_destory( MK_SHD_CTL* pShdCtl )
//{
//	if ( pShdCtl != NULL )
//	{
//		/* 释放定值数据空间 */
//		if ( pShdCtl->DfrSg.DataAddr )
//		{
//			free( pShdCtl->DfrSg.DataAddr );
//			memset( &pShdCtl->DfrSg, 0, sizeof(SHDFR_SG) );
//		}
		
//		if ( pShdCtl->ShdFp )
//			fclose( pShdCtl->ShdFp );
		
//		free( pShdCtl );
//	}
//	return 0;
//}


