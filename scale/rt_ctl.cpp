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
#include "rt_ctl.h"
//#include "sg_mgr.h"
#include "sg_def.h"
#include "rdu_len.h"
#include <stdlib.h>
#include <malloc.h>
//#include "tmp.h"

//#include "rcd_mem.h"

//#include "rcd_dbg.h"

static int rt_ctl_assoc( RT_CTL* rt_ctl );

RT_CTL* rt_ctl_create( SHDFR_SG* pSg, int cyc_num )
{
    int ret, smp_cnt;
    RT_CTL * rt_ctl;

    if ( pSg == NULL )
    {
        printf( "pSg is NULL!\n" );
        return NULL;
    }

    rt_ctl = (RT_CTL*)calloc( 1, sizeof(RT_CTL) );
    if ( rt_ctl == NULL )
    {
        printf( "Error: No memory!\n" );
        return NULL;
    }

    smp_cnt = pSg->pDFRParm->DfrCfg.SmpCntPerCyc;
    rt_ctl->CycCnt = cyc_num;
    rt_ctl->SmpCntPerCyc = smp_cnt;
    rt_ctl->SmpCntPerSec = smp_cnt * pSg->pDFRParm->DfrCfg.Freq;
    rt_ctl->SmpPntCnt= cyc_num * smp_cnt;
    rt_ctl->SmpIntervalUsec	 = 1000 * 1000 / rt_ctl->SmpCntPerCyc / pSg->pDFRParm->DfrCfg.Freq;
    rt_ctl->CycIntervalUsec  = 1000 * 1000 / pSg->pDFRParm->DfrCfg.Freq;

    ret = rt_ctl_init( rt_ctl, pSg );

    if ( ret != 0 )
    {
        rt_ctl_destory( rt_ctl );
        rt_ctl = NULL;
  }

    return rt_ctl;
}

int rt_ctl_init( RT_CTL* rt_ctl, SHDFR_SG* pSg )
{
    int		ret, len, cfg_chg, dataLen;
    INT8U *pdata;

//    /* 检测定值中的基本配置信息是否改变 */
//    cfg_chg = sg_comp_dfrcfg( &rt_ctl->DfrSg, pSg );
//    printf( "rt_ctl_init(), cfg_chg = %d\n", cfg_chg );

    /* Copy一份定值数据 */
//    ret = dfr_sg_cpy( &rt_ctl->DfrSg, pSg, 1 );
//    if ( ret != 0 )
//        return ret;

    /* 初始化实时分析定值参数信息 */
    ret = init_rt_sg( &rt_ctl->RtSg, pSg );
    if ( ret != 0 )
        return ret;

    rt_ctl->AnChCnt  = pSg->pDFRParm->DfrCfg.AnChCnt;
    rt_ctl->BinChCnt = pSg->pDFRParm->DfrCfg.BinChCnt;
    rt_ctl->ULinCnt  = pSg->pDFRParm->DfrCfg.ULinCnt;
    rt_ctl->ILinCnt  = pSg->pDFRParm->DfrCfg.ILinCnt;
    rt_ctl->TrnCnt   = pSg->pDFRParm->DfrCfg.TrnCnt;
    rt_ctl->GnCnt    = pSg->pDFRParm->DfrCfg.GnCnt;

    rt_ctl->AnSmpLenPerPnt   = rdu_getAnSmpPntLen(rt_ctl->AnChCnt);
    rt_ctl->BinLenPerPnt     = rdu_getBinSmpPntLen(rt_ctl->BinChCnt);

  rt_ctl->SmpDataLenPerPnt = rt_ctl->AnSmpLenPerPnt +
                                rt_ctl->BinLenPerPnt +
                sizeof(YHFtimeval);
    rt_ctl->SmpDataLenPerCyc = rt_ctl->SmpDataLenPerPnt * rt_ctl->SmpCntPerCyc;
    rt_ctl->CalDataLenPerCyc = rdu_getCalcPntLen( &pSg->pDFRParm->DfrCfg );

//	for (int i=4; i<8; i++)
//	{
//		rt_ctl->swFlgCh[i] = i;	//AC_I
//	}

    //rt_ctl->SgBufLen = 1024*1024;

    /* 如果基本配置信息改变，需要重新调整内存空间的分配 */
    if ( cfg_chg )
    {
        if (rt_ctl->SmpBufAddr)
        {
            free( rt_ctl->SmpBufAddr );
            rt_ctl->SmpBufAddr = NULL;
        }

        if (rt_ctl->CalBufAddr)
        {
            free( rt_ctl->CalBufAddr );
            rt_ctl->CalBufAddr = NULL;
        }

        if (rt_ctl->DftCalDataPrev)
        {
            free( rt_ctl->DftCalDataPrev );
            rt_ctl->DftCalDataPrev = NULL;
        }

        if (rt_ctl->DftCalDataCur)
        {
            free( rt_ctl->DftCalDataCur );
            rt_ctl->DftCalDataCur = NULL;
        }

        if (rt_ctl->DftCalDataNext)
        {
            free( rt_ctl->DftCalDataNext );
            rt_ctl->DftCalDataNext = NULL;
        }
    }

    /* 原始采样数据区 */
    if ( rt_ctl->SmpBufAddr == NULL )
    {
        len = rt_ctl->CycCnt * rt_ctl->SmpDataLenPerCyc;
    pdata = (INT8U*) calloc ( 2, len );
        if ( pdata == NULL ) {
            return -1;
        }
        rt_ctl->SmpBufAddr = pdata;
        rt_ctl->SmpBufLen = len;
    }

    /* 计算结果数据区 */
    if ( rt_ctl->CalBufAddr == NULL )
    {
        len = rt_ctl->CycCnt * rt_ctl->CalDataLenPerCyc;
        pdata = (INT8U*) calloc ( 1, len );
        if ( pdata == NULL ) {
            return -1;
        }
        rt_ctl->CalBufAddr = pdata;
        rt_ctl->CalBufLen  = len;
    }


    if ( rt_ctl->SmpCntPerCyc % 20 == 0 )		dataLen = 20;
    else										dataLen = 32;

    /* Dft计算数据区 上上周期*/
    if ( rt_ctl->DftCalDataPrev == NULL )
    {
        len = dataLen * rt_ctl->AnChCnt;
        pdata = (INT8U*) calloc ( 1, len * sizeof(FLOAT32)  );
        if ( pdata == NULL ) {
            return -1;
        }
        rt_ctl->DftCalDataPrev    = (FLOAT32*)pdata;
        rt_ctl->DftCalDataArrLen = len;
    }

    /* Dft计算数据区 上周期*/
    if ( rt_ctl->DftCalDataCur == NULL )
    {
        len = dataLen * rt_ctl->AnChCnt;
        pdata = (INT8U*) calloc ( 1, len * sizeof(FLOAT32)  );
        if ( pdata == NULL ) {
            return -1;
        }
        rt_ctl->DftCalDataCur  = (FLOAT32*)pdata;
    }

    /* Dft计算数据区 本周期*/
    if ( rt_ctl->DftCalDataNext == NULL )
    {
        len = dataLen * rt_ctl->AnChCnt;
        pdata = (INT8U*) calloc ( 1, len * sizeof(FLOAT32)  );
        if ( pdata == NULL ) {
            return -1;
        }
        rt_ctl->DftCalDataNext  = (FLOAT32*)pdata;
    }

    /* 索引区 */
    if ( rt_ctl->IdxBufAddr == NULL )
    {
        len = rt_ctl->CycCnt * sizeof(CYC_DATA_IDX);
        pdata = (INT8U*) calloc ( 1, len );
        if ( pdata == NULL ) {
            return -1;
        }
        rt_ctl->IdxBufAddr = pdata;
        rt_ctl->IdxBufLen  = len;
        rt_ctl->CycIdxArr	= (CYC_DATA_IDX*)pdata;
    }

    /* 开关变位表 */
    rt_ctl->MaxBinChStateNum = 4096;
    if ( rt_ctl->BinChStateArr == NULL )
    {
        len = rt_ctl->MaxBinChStateNum * sizeof(BIN_CH_STATE_ITEM);
        pdata = (INT8U*) calloc ( 1, len );
        if ( pdata == NULL ) {
            return -1;
        }
        rt_ctl->BinChStateArr = (BIN_CH_STATE_ITEM*)pdata;
    }

    /* 暂态启动信息区 */
    if ( rt_ctl->TrnsTrgArr == NULL )
    {
        len = rt_ctl->CycCnt * sizeof(TRG_INFO);
        pdata = (INT8U*) calloc ( 1, len );
        if ( pdata == NULL ) {
            return  -1;
        }
        rt_ctl->TrnsTrgArr = (TRG_INFO*)pdata;
    }

    /* 稳态触发信息区 */
    if ( rt_ctl->ContTrgArr == NULL )
    {
        len = rt_ctl->CycCnt * sizeof(TRG_INFO);
        pdata = (INT8U*) calloc ( 1, len );
        if ( pdata == NULL ) {
            return -1;
        }
        rt_ctl->ContTrgArr = (TRG_INFO*)pdata;
    }

    if ( cfg_chg )
    {
        ret = rt_ctl_assoc( rt_ctl );
    }

    rt_ctl->InitCnt = 120;
    return ret;
}

int rt_ctl_destory( RT_CTL* rt_ctl )
{
    if ( rt_ctl == NULL )
        return -1;

    if (rt_ctl->SmpBufAddr)
    {
        free( rt_ctl->SmpBufAddr );
        rt_ctl->SmpBufAddr = NULL;
    }

    if (rt_ctl->CalBufAddr)
    {
        free( rt_ctl->CalBufAddr );
        rt_ctl->CalBufAddr = NULL;
    }

    if (rt_ctl->DftCalDataPrev)
    {
        free( rt_ctl->DftCalDataPrev );
        rt_ctl->DftCalDataPrev = NULL;
    }

    if (rt_ctl->DftCalDataCur)
    {
        free( rt_ctl->DftCalDataCur );
        rt_ctl->DftCalDataCur = NULL;
    }

    if (rt_ctl->DftCalDataNext)
    {
        free( rt_ctl->DftCalDataNext );
        rt_ctl->DftCalDataNext = NULL;
    }

    if (rt_ctl->IdxBufAddr)
    {
        free( rt_ctl->IdxBufAddr );
        rt_ctl->IdxBufAddr = NULL;
    }

    if ( rt_ctl->BinChStateArr )
    {
        free( rt_ctl->BinChStateArr );
        rt_ctl->BinChStateArr = NULL;
    }

    if (rt_ctl->TrnsTrgArr)
    {
        free( rt_ctl->TrnsTrgArr );
        rt_ctl->TrnsTrgArr = NULL;
    }

    if (rt_ctl->ContTrgArr)
    {
        free( rt_ctl->ContTrgArr );
        rt_ctl->ContTrgArr = NULL;
    }

    free( rt_ctl );
    return 0;
}


static int rt_ctl_assoc( RT_CTL* rt_ctl )
{
    int 			i;
    INT8U			*pdata;
    TRG_INFO		*pTrnsTrg, *pContTrg;
    CYC_DATA_IDX	*pCycDataIdx = NULL;

    //初始化采样数据区指针
    pdata  = rt_ctl->SmpBufAddr;
    pCycDataIdx = (CYC_DATA_IDX*)rt_ctl->IdxBufAddr;
    for ( i = 0; i < rt_ctl->CycCnt; i++, pCycDataIdx++ )
    {
        pCycDataIdx->SmpDataAddr = pdata;
        pdata += rt_ctl->SmpDataLenPerCyc;
    }

    //初始化计算结果数据指针
    pdata    = rt_ctl->CalBufAddr;
    pTrnsTrg = rt_ctl->TrnsTrgArr;
    pContTrg = rt_ctl->ContTrgArr;
    pCycDataIdx = (CYC_DATA_IDX*)rt_ctl->IdxBufAddr;
    for ( i = 0; i < rt_ctl->CycCnt;
        i++, pCycDataIdx++, pTrnsTrg++, pContTrg++ )
    {
        pCycDataIdx->CalDataAddr = pdata;
        pCycDataIdx->TimeStamp	 = (YHFtimeval*)pdata;
        pdata += sizeof( YHFtimeval );

        if ( rt_ctl->AnChCnt > 0 )
        {
            pCycDataIdx->AnChVal  = (ANCH_VAL*) pdata;
            pdata += rt_ctl->AnChCnt * sizeof(ANCH_VAL);
        }

        if ( rt_ctl->ULinCnt > 0 )
        {
            pCycDataIdx->ULinVal  = (ULIN_VAL*) pdata;
            pdata += rt_ctl->ULinCnt * sizeof(ULIN_VAL);
        }

        if ( rt_ctl->ILinCnt > 0 )
        {
            pCycDataIdx->ILinVal  = (ILIN_VAL*) pdata;
            pdata += rt_ctl->ILinCnt * sizeof(ILIN_VAL);
        }

        if ( rt_ctl->GnCnt > 0 )
        {
            pCycDataIdx->GnVal  = (GN_VAL*) pdata;
            pdata += rt_ctl->GnCnt * sizeof(GN_VAL);
        }

        if ( rt_ctl->TrnCnt > 0 )
        {
            pCycDataIdx->TrnVal  = (TRN_VAL*) pdata;
            pdata += rt_ctl->TrnCnt * sizeof(TRN_VAL);
        }

        pCycDataIdx->TrnsTrgInfo = pTrnsTrg;
        pCycDataIdx->ContTrgInfo = pContTrg;
    }
    assert( pdata == rt_ctl->CalBufAddr + rt_ctl->CalBufLen );

    //为方便处理，串成一个单链表...
    pCycDataIdx = (CYC_DATA_IDX*)rt_ctl->IdxBufAddr;
    pCycDataIdx->pprev = pCycDataIdx + rt_ctl->CycCnt - 1;
    for ( i = 0; i < rt_ctl->CycCnt-1; i++ )
    {
        pCycDataIdx++;
        pCycDataIdx->pprev = pCycDataIdx - 1;
    }

    rt_ctl->CurSmpPntAddr = rt_ctl->SmpBufAddr;
    rt_ctl->CurSmpPntPos = 0;
    rt_ctl->CurCycPos = 0;

    return 0;
}

