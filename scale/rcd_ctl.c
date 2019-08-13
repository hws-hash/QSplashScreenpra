/*************************************************************************
* Copyright (c) 2011,深圳市元恒丰电气技术有限公司
* All rights reserved.

**************************************************************************/

/*!	\file
 	\brief		实时数据区相关接口的实现
 	
	\author		chen
	\version	1.0
	\date		2011-03-17
	
*/

#include <stdio.h>
#include <assert.h>
#include "rcd_ctl.h"
#include "shd_ctl.h"
#include "string.h"
//#include "rcd_dbg.h"

#include <stdlib.h>         /* For _MAX_PATH definition */
//#include <malloc.h>

static FAULT_INFO* flt_info_create( int max_seg_num )
{
	int  len;
	FAULT_INFO *flt_info;
	
	len = sizeof( FAULT_INFO ) + max_seg_num * sizeof(FAULT_SEG_INFO);
	
	flt_info = (FAULT_INFO*) calloc( 1, len );
	if ( NULL == flt_info )
		return NULL;
	
	flt_info->MaxSegCnt = max_seg_num;
	flt_info->SegInfo = (FAULT_SEG_INFO*)(flt_info+1);

	flt_info->MkShdCtl = shd_ctl_create( 52 * 1024 *1024 );
	if( NULL == flt_info->MkShdCtl )
	{
		printf( "shd_ctl_create() failed!\n" );
		free( flt_info );
		return NULL;
	}
		
	return flt_info;
}

//int flt_info_init( FAULT_INFO* flt_info, SHDFR_SG* pSg )
//{
//	int ret = 0;
	
//	if ( flt_info->SegInfo )
//		memset( flt_info->SegInfo, 0, flt_info->MaxSegCnt*sizeof(FAULT_SEG_INFO) );

//	flt_info->SegCnt = 0;
//	flt_info->CycCnt = 0;
	
//	flt_info->FlgRcding    = 0;
//	flt_info->FlgShdMading = 0;
//	flt_info->FlgNeedSplit = 0;
//	flt_info->ReTrgCnt     = 0;
//	flt_info->FltFileLen   = 0;

//	memset( &flt_info->FltTime,   0, sizeof(YHFtimeval) );
//	memset( &flt_info->StartTime, 0, sizeof(YHFtimeval) );
	
//	if( pSg )
//		ret = shd_ctl_init( flt_info->MkShdCtl, pSg, NULL, NULL, 0 );
	
//	return ret;
//}

TRNS_RCD_CTL* rcd_ctl_create()
{
	TRNS_RCD_CTL* pRcdCtl = (TRNS_RCD_CTL*) calloc ( 1, sizeof(TRNS_RCD_CTL) );
	if ( NULL == pRcdCtl )
		return NULL;
	
	pRcdCtl->FltInfo1 = flt_info_create( MAX_FAULT_SEG_NUM );
	if ( NULL == pRcdCtl->FltInfo1 )
	{
		rcd_ctl_destory( pRcdCtl );
		return NULL;
	}
	
	pRcdCtl->FltInfo2 = flt_info_create( MAX_FAULT_SEG_NUM );
	if ( NULL == pRcdCtl->FltInfo2 )
	{
		rcd_ctl_destory( pRcdCtl );
		return NULL;
	}
	
	return pRcdCtl;
}

//int  rcd_ctl_init   ( TRNS_RCD_CTL* pRcdCtl, SHDFR_SG* psg )
//{
//	int ret = 0;
//	if ( psg == NULL )
//		return 1;
	
//	pRcdCtl->Mod = 1;	/*正常运行状态*/
//	pRcdCtl->Setting = psg->pDFRParm->DfrTrnsSet;
	
//	ret = flt_info_init( pRcdCtl->FltInfo1, psg );
//	if ( ret != 0 )
//		return ret;
	
//	ret = flt_info_init( pRcdCtl->FltInfo2, psg );
//	if ( ret != 0 )
//		return ret;
	
//	return 0;
//}

int  rcd_ctl_destory( TRNS_RCD_CTL* pRcdCtl )
{
	if ( pRcdCtl == NULL )
		return 0;
	
	if ( pRcdCtl->FltInfo1 )
	{
		if( pRcdCtl->FltInfo1->MkShdCtl )
		{
			free( pRcdCtl->FltInfo1->MkShdCtl );
			pRcdCtl->FltInfo1->MkShdCtl = NULL;
		}
		free( pRcdCtl->FltInfo1 );
		pRcdCtl->FltInfo1 = NULL;
	}

	if ( pRcdCtl->FltInfo2 )
	{
		if( pRcdCtl->FltInfo2->MkShdCtl )
		{
			free( pRcdCtl->FltInfo2->MkShdCtl );
			pRcdCtl->FltInfo2->MkShdCtl = NULL;
		}
		free( pRcdCtl->FltInfo2 );
		pRcdCtl->FltInfo2 = NULL;
	}
	
	free( pRcdCtl );
	return 0;
}


