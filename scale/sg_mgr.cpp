/*************************************************************************
* Copyright (c) 2008,深圳市元恒丰有限公司
* All rights reserved.

**************************************************************************/

/*!	\file		
 	\brief		定值文件读/写接口的实现
 	
	\author		Chen
	\version	1.0
	\date		2009-07-13
*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <assert.h>
#include "sg_mgr.h"

#if 0
	#define dbg_warning( _argv_ )	{ printf( "line:%-4d file: %s \n",__LINE__,__FILE__ ); printf  _argv_; }
	#define dbg_error( _argv_ )		{ printf( "line:%-4d file: %s \n",__LINE__,__FILE__ ); printf  _argv_; }
#else
	#define dbg_error(_argv_)		
	#define dbg_warning(_argv_)		
#endif

#define ERROR_FREAD		1
#define ERROR_FSEEK		2
#define ERROR_FWRITE	3

static	int fwrite_block( FILE *fp, size_t pos, void* pData, size_t size)
{
	int ret = 0;
	errno = 0;
	while ( 0 != ( ret = fseek( fp, pos ,SEEK_SET) )  && (EINTR == errno) );
	if ( 0 != ret ) 
	{
		dbg_error(( "Error: fseek failed!." ));
		return ERROR_FSEEK;
	}

	errno = 0;
	while( 0 == (ret = fwrite( pData, size, 1, fp)) && EINTR == errno);
	if ( 1 != ret ) 
	{
		dbg_error(( "Error: fwrite failed!." ));
		return ERROR_FWRITE;
	}
	
	return 0;
}

static int fread_block( FILE *fp, size_t pos, void* pData, size_t size )
{
	int ret = 0;
	errno = 0;
	while ( 0 != ( ret = fseek( fp, pos ,SEEK_SET) )  && (EINTR == errno) );
	if ( 0 != ret ) 
	{
		dbg_error(( "Error: fseek failed!." ));
		return ERROR_FSEEK;
	}
	
	errno = 0;
	while ( 0 == (ret = fread( pData, size, 1, fp )) && (EINTR == errno) ) ;
	if ( ret != 1 ) 
	{
        dbg_error(( "Error: fread failed!." ));
		return ERROR_FREAD;
	}
	
	return 0;
}


static bool is_file_existed(const char * pszfname)
{
	int result;

	struct stat buf;
	result = stat( pszfname, &buf ); 

	return( result == 0 );
}

SGManager::SGManager()
{
	m_psg = NULL;
};

SGManager::~SGManager()
{
	destroyDfrSGData(m_psg);
};

int initDfrSg( SHDFR_SG* dfr_sg, void* sg_data, int sg_size, int chkSum )
{
	int   i,pos = 0;
	char* data = (char*)sg_data;
	
	dfr_sg->DataAddr = (INT32U*)data;
	dfr_sg->BufLen   = sg_size;
	dfr_sg->pDFRParm = (YH_DFR_PARM*)(data + pos);
	pos += sizeof( YH_DFR_PARM );
	
	/* 模拟量 */
	dfr_sg->pAnChParm = ( dfr_sg->pDFRParm->DfrCfg.AnChCnt == 0 ) ? 
		NULL : (YH_ANCH_PARM*)(data + pos);
	pos += sizeof( YH_ANCH_PARM ) * dfr_sg->pDFRParm->DfrCfg.AnChCnt;

	/* 开关量 */
	dfr_sg->pBinChParm = ( dfr_sg->pDFRParm->DfrCfg.BinChCnt == 0 ) ? 
		NULL : (YH_BINCH_PARM*)(data + pos);
	pos += sizeof( YH_BINCH_PARM ) * dfr_sg->pDFRParm->DfrCfg.BinChCnt;

	/* 电流量 */
	dfr_sg->pILinParm = ( dfr_sg->pDFRParm->DfrCfg.ILinCnt == 0 ) ? 
		NULL : (YH_ILIN_PARM*)(data + pos);
	pos += sizeof( YH_ILIN_PARM ) * dfr_sg->pDFRParm->DfrCfg.ILinCnt;

	/* 电压量 */
	dfr_sg->pULinParm = ( dfr_sg->pDFRParm->DfrCfg.ULinCnt == 0 ) ? 
		NULL : (YH_ULIN_PARM*)(data + pos);
	pos += sizeof( YH_ULIN_PARM ) * dfr_sg->pDFRParm->DfrCfg.ULinCnt;

	/* 变压器 */
	dfr_sg->pTrnParm = ( dfr_sg->pDFRParm->DfrCfg.TrnCnt == 0 ) ? 
		NULL : (YH_TRN_PARM*)(data + pos);
	pos += sizeof( YH_TRN_PARM ) * dfr_sg->pDFRParm->DfrCfg.TrnCnt;
	
	/* 发电机 */
	dfr_sg->pGnParm = ( dfr_sg->pDFRParm->DfrCfg.GnCnt == 0 ) ? 
		NULL : (YH_GN_PARM*)(data + pos);
	pos += sizeof( YH_GN_PARM ) * dfr_sg->pDFRParm->DfrCfg.GnCnt;
	
	/* 定值数据长度必须等于各个数据段长度之和 */
	if ( pos + 4 != sg_size )
	{
		dbg_error(( "SG data not correct!" ));
		return -1;
	}

	if ( chkSum )
	{
		INT32U chksum = 0;
		INT32U *pVal  = (INT32U*)data;
		for ( i = 0; i < pos/4; i++, pVal++ )
			chksum += (*pVal);
		
		if ( *pVal != chksum )
		{
			dbg_warning(( "SG data check sum not correct!\n" ));
			*pVal = chksum;
		}
	}
	return 0;
}

int SGManager::initDfrSgData( SHDFR_SG* pSg )
{
	int i, j, cnt = 0;
	YH_ANCH_CFG* pAnCfg   = NULL;
	YH_BINCH_CFG* pBinCfg = NULL;
	YH_ULIN_CFG* pULinCfg = NULL;
	YH_ILIN_CFG* pILinCfg = NULL;
	YH_TRN_CFG*  pTrnCfg  = NULL;
	YH_GN_CFG*   pGnCfg   = NULL;
	YH_DFR_CFG*  pDfrCfg  = &pSg->pDFRParm->DfrCfg;

	//YH_ANCH_SET        AnTrnsSet;  /*!< 暂态定值 */
	//YH_ANCH_SET        AnContSet;  /*!< 稳态定值 */
	//YH_PQ_ANCH_SET		AnPQSet;	/*!< 电能质量定值 */ 

	pSg->pDFRParm->DfrCfg.StnID = 66;
	pSg->pDFRParm->DfrCfg.DFRID = 88;    
	pSg->pDFRParm->DfrCfg.ContEna = 0;    
	pSg->pDFRParm->DfrCfg.PQEna = 0; 

	pSg->pDFRParm->DfrTrnsSet.A2NumCyc = 0;
	pSg->pDFRParm->DfrTrnsSet.A1NumCyc = 5;
	pSg->pDFRParm->DfrTrnsSet.BNumCyc = 15;
	pSg->pDFRParm->DfrTrnsSet.CNumCyc = 100;
	pSg->pDFRParm->DfrTrnsSet.DNumCyc = 250;
	pSg->pDFRParm->DfrTrnsSet.A2SmpRte = 50;
  pSg->pDFRParm->DfrTrnsSet.A1SmpRte = 100000/5;
  pSg->pDFRParm->DfrTrnsSet.BSmpRte = 100000/5;
	pSg->pDFRParm->DfrTrnsSet.CSmpRte = 50;
	pSg->pDFRParm->DfrTrnsSet.DSmpRte = 10;
	pSg->pDFRParm->DfrTrnsSet.MaxNumRcd = 1000;

	pSg->pDFRParm->DfrContSet.SmpRte = 1000;
	pSg->pDFRParm->DfrContSet.RcdDur = 5;
	pSg->pDFRParm->DfrContSet.MaxNumRcd = 100;
	pSg->pDFRParm->DfrContSet.RcdFileDur = 120;

	for ( i = 0; i < pDfrCfg->AnChCnt; i++ )
	{
		(pSg->pAnChParm + i)->AnTrnsSet.Index = i + 1;
		(pSg->pAnChParm + i)->AnContSet.Index = i + 1;
		(pSg->pAnChParm + i)->AnPQSet.Index   = i + 1;

		pAnCfg = &(pSg->pAnChParm + i)->AnCfg;
		pAnCfg->Index = i + 1;
		pAnCfg->ChNum = i + 1;
		sprintf( (char*)pAnCfg->Name, "模拟通道%d", i + 1 );

		pAnCfg->Offset = 0.0;
		pAnCfg->Scaler = 0.0015f;
		pAnCfg->Skew   = 0.0;
		pAnCfg->Ratio  = 2200.0f;
		pAnCfg->ChTyp  = ANCH_TYPE_TV;
		pAnCfg->ACSecRtg  = 1;
		pAnCfg->Freq = pSg->pDFRParm->DfrCfg.Freq;
		pAnCfg->BindType = 0;
		pAnCfg->Unit =  29; 
		pAnCfg->UnitMultiplier = 0;
	}

	for ( i = 0; i < pDfrCfg->BinChCnt; i++ )
	{
		(pSg->pBinChParm + i)->BinTrnsSet.Index = i + 1;
		(pSg->pBinChParm + i)->BinContSet.Index = i + 1;

		pBinCfg = &(pSg->pBinChParm + i)->BinCfg;
		pBinCfg->Index = i + 1;
		pBinCfg->ChNum = i + 1;
		sprintf( (char*)pBinCfg->Name, "开关通道%d", i + 1 );
	}

	for ( i = 0; i < pDfrCfg->ULinCnt; i++ )
	{
		(pSg->pULinParm + i)->LinTrnsSet.Index = i + 1;
		(pSg->pULinParm + i)->LinContSet.Index = i + 1;

		pULinCfg = &(pSg->pULinParm + i)->LinCfg;
		pULinCfg->Index = i + 1;
		sprintf( (char*)pULinCfg->Name, "电压%d", i + 1 );

		pULinCfg->ChNum[0] = ++cnt;
		pULinCfg->ChNum[1] = ++cnt;
		pULinCfg->ChNum[2] = ++cnt;
		pULinCfg->ChNum[3] = ++cnt;
	}

	cnt = 0;
	for ( i = 0; i < pDfrCfg->ILinCnt; i++ )
	{
		(pSg->pILinParm + i)->LinTrnsSet.Index = i + 1;
		(pSg->pILinParm + i)->LinContSet.Index = i + 1;
		(pSg->pILinParm + i)->FloSet.Index = i + 1;

		pILinCfg = &(pSg->pILinParm + i)->LinCfg;
		pILinCfg->Index = i + 1;
		sprintf( (char*)pILinCfg->Name, "电流%d", i + 1 );

		pILinCfg->ChNum[0] = ++cnt;
		pILinCfg->ChNum[1] = ++cnt;
		pILinCfg->ChNum[2] = ++cnt;
		pILinCfg->ChNum[3] = ++cnt;
	}

	cnt = 0;
	for ( i = 0; i < pDfrCfg->TrnCnt; i++ )
	{
		(pSg->pTrnParm + i)->TrnTrnsSet.Index = i + 1;
		(pSg->pTrnParm + i)->TrnContSet.Index = i + 1;

		pTrnCfg = &(pSg->pTrnParm + i)->TrnCfg;
		pTrnCfg->Index = i + 1;
		sprintf( (char*)pTrnCfg->Name, "变压器%d", i + 1 );

		pTrnCfg->HiBraCnt = 1;
		pTrnCfg->MiBraCnt = 1;
		pTrnCfg->LoBraCnt = 1;

		for ( j = 0; j < pTrnCfg->HiBraCnt; j++ )
			pTrnCfg->HiBraNum[j] = ++cnt;

		for ( j = 0; j < pTrnCfg->MiBraCnt; j++ )
			pTrnCfg->MiBraNum[j] = ++cnt;

		for ( j = 0; j < pTrnCfg->LoBraCnt; j++ )
			pTrnCfg->LoBraNum[j] = ++cnt;
	}

	for ( i = 0; i < pDfrCfg->GnCnt; i++ )
	{
		(pSg->pGnParm + i)->GnTrnsSet.Index = i + 1;
		(pSg->pGnParm + i)->GnContSet.Index = i + 1;

		pGnCfg = &(pSg->pGnParm + i)->GnCfg;
		pGnCfg->Index = i + 1;
		sprintf( (char*)pGnCfg->Name, "发电机%d", i + 1 );
	}

	return 0;	
}

INT32U SGManager::calc_sg_chksum( SHDFR_SG* dfr_sg )
{
	int i;
	INT32U chksum = 0;
	INT32U *pVal = (INT32U*)dfr_sg->DataAddr;
	for ( i = 0; i < dfr_sg->BufLen/4 -1; i++, pVal++ )
		chksum += (*pVal);
	
	*pVal = chksum;
	return chksum;
}


int SGManager::readSGFileHead ( const char* sg_fname, DfrSGFileHead* psg_file_head )
{
	int ret = 0;
	struct stat file_stat;

	FILE* fp = fopen( sg_fname, "rb" );
	if ( NULL == fp )
	{
		dbg_error(( "Open sgfile [%s] failed!\n", sg_fname ));
		return -1;
	}
	
	fstat( fileno(fp), &file_stat );
	
	if ( file_stat.st_size < sizeof(DfrSGFileHead) )
	{
		dbg_error(( "Error: File length [%d] less than DfrSGFileHead.\n", file_stat.st_size ));
		fclose( fp );
		return -1;
	}
	
	errno = 0;
	while ( 0 == (ret = fread( psg_file_head, sizeof(DfrSGFileHead), 1, fp )) && (EINTR == errno) ) ;
	if ( ret != 1 )
	{
        dbg_error(( "Error: fread failed!." ));
        fclose( fp );
        return -1;
    }
	
	fclose( fp );
	return 0;
}

int SGManager::writeSGFileHead ( const char* sg_fname, const DfrSGFileHead* sg_file_head )
{
	int ret = 0;
	FILE* fp = fopen( sg_fname, "rb+" );
	if ( NULL == fp )
	{
		dbg_error(( "Open sgfile [%s] failed!\n", sg_fname ));
		return -1;
	}
	
	errno = 0;
	while( 0 == (ret = fwrite( sg_file_head, sizeof(DfrSGFileHead), 1, fp)) && EINTR == errno);
	if ( ret != 1 )
	{
        dbg_error(( "Error: fwrite failed!." ));
        fclose( fp );
        return -1;
    }
	
	fclose(fp);
	return 0;
}

int SGManager::readDfrSGData( const char* sg_fname, SHDFR_SG* dfr_sg, DfrSGFileHead* psg_file_head )
{
	int flg,ret,pos;
	struct stat file_stat;
	unsigned char* data;
	DfrSGFileHead  sg_file_head;

	/* 打开定值文件 */
	FILE * fp = fopen( sg_fname, "rb" );
	if ( NULL == fp )
	{
		dbg_error(( "Open sgfile [%s] failed!\n", sg_fname ));
		return -1;
	}
	
	/* 获取文件长度 */
	fstat( fileno(fp), &file_stat );
	
	if ( file_stat.st_size < sizeof(DfrSGFileHead) )
	{
		dbg_error(( "Error: File length [%d] less than DfrSGFileHead.\n", file_stat.st_size ));
		fclose( fp );
		return -1;
	}
	
	/* 读取定值文件头 */
	memset( &sg_file_head, 0, sizeof(DfrSGFileHead) );
	
	errno = 0;
	while ( 0 == (ret = fread( &sg_file_head, sizeof(DfrSGFileHead), 1, fp )) && (EINTR == errno) ) ;
	if ( ret != 1 )
	{
        dbg_error(( "Error: fread failed!." ));
        fclose( fp );
        return -1;
    }
	
	/* 检测文件长度是否正确 */
	if ( file_stat.st_size != 
		sizeof( DfrSGFileHead ) + sg_file_head.LenOfSG )
	{
		dbg_error(( "Error: SGFile length not correct!." ));
		fclose( fp );
		return -1;
	}
	
	if ( psg_file_head )
		memcpy( psg_file_head, &sg_file_head, sizeof(DfrSGFileHead) );
	
	flg = 0;
	data = NULL;
	if ( dfr_sg->BufLen != 0 )
	{
		if ( dfr_sg->BufLen != sg_file_head.LenOfSG )
		{
			dbg_error(( "Error: sg_len not match!" ));
			fclose( fp );
			return -1;
		}
		
		if ( dfr_sg->DataAddr == NULL )
		{
			dbg_error(( "Unexpected!" ));
			fclose( fp );
			return -1;
		}
		data = (unsigned char*)dfr_sg->DataAddr;
	}
	else
	{
		/* 分配内存 */
		data = (unsigned char*)calloc( 1, sg_file_head.LenOfSG );
		if ( NULL == data )
		{
			dbg_error(( "Error: No memory!" ));
			fclose( fp );
			return -1;
		}
		flg = 1;
	}
	
	/* 读取定值数据 */
	pos = sizeof( DfrSGFileHead );
	ret = fread_block( fp, pos, data, sg_file_head.LenOfSG );
	fclose( fp );
	
	if ( ret != 0 )
	{
		dbg_error(( "fread_block failed!" ));
		if (flg)
			free( data );
		return -1;
	}

	ret = initDfrSg( dfr_sg, data, sg_file_head.LenOfSG, 1 );
	if ( ret != 0 )
	{
		dbg_error(( "initDfrSg failed!" ));
		if (flg)
			free( data );
		return -1;
	}

	return 0;
}

int SGManager::createDfrSGFile( const char* sg_fname, const SHDFR_SG* dfr_sg )
{
	int ret = 0;
	FILE* fp = NULL;
	DfrSGFileHead  sg_file_head;
	DfrSGFileHead* psg_file_head = &sg_file_head;
	memset( &sg_file_head, 0, sizeof(DfrSGFileHead) );
	
	fp = fopen( sg_fname, "wb" );
	if ( NULL == fp )
	{
		dbg_error(( "Open sgfile [%s] failed!\n", sg_fname ));
		return -1;
	}

	strcpy( psg_file_head->ShFlg, "yhf" );
	strcpy( psg_file_head->ShExt, "sg" );
	psg_file_head->FileVer = SG_FILE_VERSION;
	psg_file_head->LenOfSG = dfr_sg->BufLen;
	
	errno = 0;
	while( 0 == (ret = fwrite( psg_file_head, sizeof(DfrSGFileHead), 1, fp)) 
		&& EINTR == errno);
	if ( 1 != ret )
	{
		dbg_error(( "Write SgFile head failed!" ));
		fclose( fp );
		remove(sg_fname);
		return -1;
	}
	
	calc_sg_chksum( (SHDFR_SG*)dfr_sg );
	
	errno = 0;
	while( 0 == (ret = fwrite( dfr_sg->DataAddr, psg_file_head->LenOfSG, 1, fp)) 
		&& EINTR == errno);
	if ( 1 != ret )
	{
		dbg_error(( "Write SgFile data failed!" ));
		fclose( fp );
		remove(sg_fname);
		return -1;
	}

	fclose( fp );
	return 0;
}

int SGManager::writeDfrSGData( const char* sg_fname, const SHDFR_SG* dfr_sg )
{
	int   ret = 0;
	FILE* fp = NULL;
	DfrSGFileHead  sg_file_head;
	//DfrSGFileHead* psg_file_head = &sg_file_head;
	memset( &sg_file_head, 0, sizeof(DfrSGFileHead) );
	
	if ( !is_file_existed( sg_fname ) )
	{	/* 文件不存在，创建一个新的文件 */
		return createDfrSGFile( sg_fname, dfr_sg );
	}

	/* 改写已经存在的定值文件 */
	fp = fopen( sg_fname, "rb+" );
	if ( NULL == fp )
	{
		dbg_error(( "Open sgfile [%s] failed!\n", sg_fname ));
		return -1;
	}
	
	calc_sg_chksum( (SHDFR_SG*)dfr_sg );
	
	do 
	{
		/* 读取定值文件头 */
		errno = 0;
		while ( 0 == (ret = fread( &sg_file_head, sizeof(DfrSGFileHead), 1, fp )) && (EINTR == errno) ) ;
		if ( ret != 1 )
		{
	        dbg_error(( "Error: fread failed!." ));
	        ret = -1;
	        break;
	    }
	
		if ( sg_file_head.LenOfSG != dfr_sg->BufLen )
		{
			dbg_error(( "Error: SG data length not correct!" ));
	        ret = -1;
	        break;
		}
		
		ret = fwrite_block( fp, sizeof(DfrSGFileHead), dfr_sg->DataAddr, sg_file_head.LenOfSG );
		if ( ret != 0 )
		{
			dbg_error(( "Error: fwrite_block failed!" ));
	        ret = -1;
	        break;
		}
	} while ( 0 );
	
	fclose( fp );	
	return ret;
}

SHDFR_SG* SGManager::createDfrSGData( int anChCnt, int binChCnt, 
						   int ilinCnt, int ulinCnt, 
						   int trnCnt,  int genCnt)
{	
	int ret,sg_size;
	char* data;
	SHDFR_SG* dfr_sg;
	YH_DFR_PARM* pDFRParm;

	sg_size = sizeof(YH_DFR_PARM) + 4
		+ sizeof(YH_ANCH_PARM) * anChCnt
		+ sizeof(YH_BINCH_PARM) * binChCnt
		+ sizeof(YH_ILIN_PARM) * ilinCnt
		+ sizeof(YH_ULIN_PARM) * ulinCnt
		+ sizeof(YH_TRN_PARM) * trnCnt
		+ sizeof(YH_GN_PARM) * genCnt;

	dfr_sg = (SHDFR_SG*) calloc ( 1, sizeof(SHDFR_SG) );
	if ( NULL == dfr_sg )
	{
		dbg_error(( "No memory!" ));
		return NULL;
	}

	data = (char*) calloc ( 1, sg_size );
	if ( NULL == data )
	{
		dbg_error(( "No memory!" ));
		free( dfr_sg );
		return NULL;
	}

	pDFRParm = (YH_DFR_PARM*)data;
	pDFRParm->DfrCfg.AnChCnt	= anChCnt;
	pDFRParm->DfrCfg.BinChCnt	= binChCnt;
	pDFRParm->DfrCfg.ILinCnt	= ilinCnt;
	pDFRParm->DfrCfg.ULinCnt	= ulinCnt;
	pDFRParm->DfrCfg.TrnCnt		= trnCnt;
	pDFRParm->DfrCfg.GnCnt		= genCnt;

	pDFRParm->DfrCfg.Ver = 0x100;
	pDFRParm->DfrCfg.Freq			= 50;
  pDFRParm->DfrCfg.SmpCntPerCyc	= 2000/5;
	pDFRParm->DfrCfg.RtCycCnt		= 5; 
	pDFRParm->DfrCfg.TrnsEna = 1;

	strcpy( pDFRParm->DfrCfg.StnName, "StationName" );
	strcpy( pDFRParm->DfrCfg.DFRName, "HWR-100" );

	ret = initDfrSg( dfr_sg, data, sg_size, 1 );
	if ( ret != 0 )
	{
		dbg_error(( "initDfrSg() failed!" ));
		free( data );
		free( dfr_sg );
		return NULL;
	}

	initDfrSgData( dfr_sg );

	return dfr_sg;
}

bool SGManager::createDfrSGFromFile( const char* sg_fname )
{
	int ret;
	m_psg = (SHDFR_SG*) calloc ( 1, sizeof(SHDFR_SG) );
	if ( NULL == m_psg )
	{
		dbg_error(( "No memory!" ));
		return false;
	}
	
	ret = readDfrSGData( sg_fname, m_psg, NULL );
	if ( ret != 0 )
	{
		dbg_error(( "readDfrSGData failed!" ));
		free( m_psg );
		m_psg = NULL;
		return false;
	}
	
	return true;
}

void SGManager::destroyDfrSGData( SHDFR_SG* &dfr_sg )
{
	if ( NULL == dfr_sg )
		return;

	if ( dfr_sg->DataAddr )
	{
		free( dfr_sg->DataAddr );
		dfr_sg->DataAddr = NULL;
	}
	
	free( dfr_sg );
	dfr_sg = NULL;
}

static void set_vol_name(char *p,int i){
  const int phase=i%4;//相位A,B,C,N
  const int channel=i/4+1;//通道数
  char c_phase;
  switch (phase) {
  case 0:
    c_phase='A';
    break;
  case 1:
    c_phase='B';
    break;
  case 2:
    c_phase='C';
    break;
  case 3:
    c_phase='N';
    break;
  default:
    break;
  }
  sprintf(p, ("AC%d-U%c"), channel, c_phase);
}

static void set_cur_name(char *p,int i){
  const int phase=i%4;//相位A,B,C,N
  const int channel=i/4+1;//通道数
  char c_phase;
  switch (phase) {
  case 0:
    c_phase='A';
    break;
  case 1:
    c_phase='B';
    break;
  case 2:
    c_phase='C';
    break;
  case 3:
    c_phase='N';
    break;
  default:
    break;
  }
  sprintf(p, ("AC%d-I%c"), channel, c_phase);
}

SHDFR_SG* SGManager::createDefaultDfrSG()
{
	 int i = 0;
	 SHDFR_SG* pshdfr;
   int anch=16;
   int dnch=8;

   pshdfr = createDfrSGData( anch, dnch, 4, 4, 0, 0);
	 if(NULL == pshdfr)
	 {
		 return NULL;
	 }

	 pshdfr->pDFRParm->DfrCfg.Ver = 0x100;
	 pshdfr->pDFRParm->DfrTrnsSet.A1NumCyc = 5;
	 pshdfr->pDFRParm->DfrTrnsSet.BNumCyc = 15;
	 pshdfr->pDFRParm->DfrTrnsSet.CNumCyc = 100;
	 pshdfr->pDFRParm->DfrTrnsSet.DNumCyc = 250;
	 pshdfr->pDFRParm->DfrTrnsSet.A2SmpRte = 50;
   pshdfr->pDFRParm->DfrTrnsSet.A1SmpRte = 10000;
   pshdfr->pDFRParm->DfrTrnsSet.BSmpRte = 10000;
	 pshdfr->pDFRParm->DfrTrnsSet.CSmpRte = 50;
	 pshdfr->pDFRParm->DfrTrnsSet.DSmpRte = 10;
	 pshdfr->pDFRParm->DfrTrnsSet.MaxNumRcd = 1000;
   pshdfr->pDFRParm->DfrCfg.SmpCntPerCyc = 10000/50;

	 //通道1-4
   for(i=0; i<8; i++)
	 {
         //sprintf(pshdfr->pAnChParm[i].AnCfg.Name, ("%s%c"), ("AC-U"), 'a'+i);
     set_vol_name(pshdfr->pAnChParm[i].AnCfg.Name,i);
         pshdfr->pAnChParm[i].AnCfg.ChNum = 1+i;
	     pshdfr->pAnChParm[i].AnCfg.Index = i+1;
	     pshdfr->pAnChParm[i].AnCfg.ChTyp = 1;   //交流电压
	     pshdfr->pAnChParm[i].AnCfg.Unit =  29;
	     pshdfr->pAnChParm[i].AnCfg.UnitMultiplier = 0;
       pshdfr->pAnChParm[i].AnCfg.BindNum = i/4+1;
	     pshdfr->pAnChParm[i].AnCfg.Ratio = 1100;
	     pshdfr->pAnChParm[i].AnCfg.BindType = 1;
	     pshdfr->pAnChParm[i].AnCfg.ACSecRtg = 57.74;

		 //pshdfr->pAnChParm[i].AnTrnsSet.SaTrgLev  = 0.8;
	     //pshdfr->pAnChParm[i].AnTrnsSet.HiTrgLev  = 1.9;
	     //pshdfr->pAnChParm[i].AnTrnsSet.LoTrgLev  = 0.6;
	     //pshdfr->pAnChParm[i].AnTrnsSet.Ha2TrgLev = 1.4;
	     //pshdfr->pAnChParm[i].AnTrnsSet.Ha3TrgLev = 1.5;
	     //pshdfr->pAnChParm[i].AnTrnsSet.Ha5TrgLev = 1.6;
	     //pshdfr->pAnChParm[i].AnTrnsSet.Ha7TrgLev = 1.7;
	 }

     //通道5-8
   for(; i < anch; i++)
	 {
         //sprintf(pshdfr->pAnChParm[i].AnCfg.Name, ("%s%c"), ("AC-I"), 'a'+i-4);
     set_cur_name(pshdfr->pAnChParm[i].AnCfg.Name,i);
         pshdfr->pAnChParm[i].AnCfg.ChNum = 1+i; 
	     pshdfr->pAnChParm[i].AnCfg.Index = i+1;
	     pshdfr->pAnChParm[i].AnCfg.ChTyp = 2;    //交流电流      
       pshdfr->pAnChParm[i].AnCfg.BindNum = i/4+1;
	     pshdfr->pAnChParm[i].AnCfg.BindType = 2;
	     pshdfr->pAnChParm[i].AnCfg.Ratio = 120;
	     pshdfr->pAnChParm[i].AnCfg.Unit =  5;       //A
	     pshdfr->pAnChParm[i].AnCfg.UnitMultiplier = 0;
         pshdfr->pAnChParm[i].AnCfg.ACSecRtg = 1;

	     //pshdfr->pAnChParm[i].AnTrnsSet.SaTrgLev  = 0.8;
	     //pshdfr->pAnChParm[i].AnTrnsSet.HiTrgLev  = 1.9;
	     //pshdfr->pAnChParm[i].AnTrnsSet.LoTrgLev  = 0.6;
	     //pshdfr->pAnChParm[i].AnTrnsSet.Ha2TrgLev = 1.4;
	     //pshdfr->pAnChParm[i].AnTrnsSet.Ha3TrgLev = 1.5;
	     //pshdfr->pAnChParm[i].AnTrnsSet.Ha5TrgLev = 1.6;
	     //pshdfr->pAnChParm[i].AnTrnsSet.Ha7TrgLev = 1.7;
	 }

//   //通道1-4
//   for(; i<12; i++)
//   {
//         sprintf(pshdfr->pAnChParm[i].AnCfg.Name, ("%s%c"), ("BC-U"), 'a'+i-8);
//         pshdfr->pAnChParm[i].AnCfg.ChNum = 1+i;
//       pshdfr->pAnChParm[i].AnCfg.Index = i+1;
//       pshdfr->pAnChParm[i].AnCfg.ChTyp = 1;   //交流电压
//       pshdfr->pAnChParm[i].AnCfg.Unit =  29;
//       pshdfr->pAnChParm[i].AnCfg.UnitMultiplier = 0;
//       pshdfr->pAnChParm[i].AnCfg.BindNum = 1;
//       pshdfr->pAnChParm[i].AnCfg.Ratio = 1100;
//       pshdfr->pAnChParm[i].AnCfg.BindType = 1;
//       pshdfr->pAnChParm[i].AnCfg.ACSecRtg = 57.74;

//     //pshdfr->pAnChParm[i].AnTrnsSet.SaTrgLev  = 0.8;
//       //pshdfr->pAnChParm[i].AnTrnsSet.HiTrgLev  = 1.9;
//       //pshdfr->pAnChParm[i].AnTrnsSet.LoTrgLev  = 0.6;
//       //pshdfr->pAnChParm[i].AnTrnsSet.Ha2TrgLev = 1.4;
//       //pshdfr->pAnChParm[i].AnTrnsSet.Ha3TrgLev = 1.5;
//       //pshdfr->pAnChParm[i].AnTrnsSet.Ha5TrgLev = 1.6;
//       //pshdfr->pAnChParm[i].AnTrnsSet.Ha7TrgLev = 1.7;
//   }



//	 //通道9-12
//   for(; i<16; i++)
//	 {
//		 //sprintf(pshdfr->pAnChParm[i].AnCfg.Name, ("%s%d"), ("直流"), i-7);
//	     pshdfr->pAnChParm[i].AnCfg.ChNum = i+1;
//	     pshdfr->pAnChParm[i].AnCfg.Index = i+1;
//	     pshdfr->pAnChParm[i].AnCfg.ChTyp = 3;      ///
//	     pshdfr->pAnChParm[i].AnCfg.BindNum = 0;
//	     pshdfr->pAnChParm[i].AnCfg.BindType = 0;
//	     pshdfr->pAnChParm[i].AnCfg.Ratio = 1;
//	     pshdfr->pAnChParm[i].AnCfg.Unit =  29;
//         pshdfr->pAnChParm[i].AnCfg.UnitMultiplier = 0;
//         pshdfr->pAnChParm[i].AnCfg.ACSecRtg = 1;
//         //pshdfr->pAnChParm[i].AnTrnsSet.SaTrgLev  = 0.8;
//         //pshdfr->pAnChParm[i].AnTrnsSet.HiTrgLev  = 1.9;
//         //pshdfr->pAnChParm[i].AnTrnsSet.LoTrgLev  = 0.6;
//         //pshdfr->pAnChParm[i].AnTrnsSet.Ha2TrgLev = 1.4;
//         //pshdfr->pAnChParm[i].AnTrnsSet.Ha3TrgLev = 1.5;
//         //pshdfr->pAnChParm[i].AnTrnsSet.Ha5TrgLev = 1.6;
//         //pshdfr->pAnChParm[i].AnTrnsSet.Ha7TrgLev = 1.7;
//	 }
//	 sprintf(pshdfr->pAnChParm[3].AnCfg.Name, "AC-Un");
//	 sprintf(pshdfr->pAnChParm[7].AnCfg.Name, "AC-In");

//   sprintf(pshdfr->pAnChParm[12].AnCfg.Name, "DC-I(4-20mA)");
//   sprintf(pshdfr->pAnChParm[13].AnCfg.Name, "DC-U(0-200mV)");
//   sprintf(pshdfr->pAnChParm[14].AnCfg.Name, "DC-U(0-10V)");
//   sprintf(pshdfr->pAnChParm[15].AnCfg.Name, "DC-U(0-300V)");

//   pshdfr->pAnChParm[12].AnCfg.ChTyp = 4;
//   pshdfr->pAnChParm[12].AnCfg.Unit =  5;
//   pshdfr->pAnChParm[12].AnCfg.UnitMultiplier = -3;

//   pshdfr->pAnChParm[13].AnCfg.ChTyp = 3;
//   pshdfr->pAnChParm[13].AnCfg.Unit =  29;
//   pshdfr->pAnChParm[13].AnCfg.UnitMultiplier = -3;

	 //pshdfr->pAnChParm[10].AnCfg.ChTyp = 3;
	 //pshdfr->pAnChParm[10].AnCfg.Unit =  29;
	 //pshdfr->pAnChParm[10].AnCfg.UnitMultiplier = 0;

     //line1 电压线路
   for(int i=0; i<4; ++i){
     sprintf(pshdfr->pULinParm[i].LinCfg.Name, "电压%d",i+1);
     pshdfr->pULinParm[i].LinCfg.Index = i+1;
     //pshdfr->pULinParm[0].LinCfg.VolLev = 0;
     //pshdfr->pULinParm[0].LinCfg.PTType = 1;
       //pshdfr->pULinParm[0].LinCfg.AssNum = 0;
       pshdfr->pULinParm[i].LinCfg.PhTyp = 4;
     pshdfr->pULinParm[i].LinCfg.ChNum[0] = 0;
     pshdfr->pULinParm[i].LinCfg.ChNum[1] = 0;
     pshdfr->pULinParm[i].LinCfg.ChNum[2] = 0;
     pshdfr->pULinParm[i].LinCfg.ChNum[3] = 0;
   }
	 //pshdfr->pULinParm[0].LinCfg.ChHf1Num = 0;
	 //pshdfr->pULinParm[0].LinCfg.ChHf1Num = 0;
     //pshdfr->pULinParm[0].LinCfg.ChHf2Num = 0;
	 //pshdfr->pULinParm[0].LinCfg.ExtrAnCnt = 0;
	 //pshdfr->pULinParm[0].LinCfg.AnNumArr[0] = 0;
	 //pshdfr->pULinParm[0].LinCfg.AnNumArr[1] = 0;
	 //pshdfr->pULinParm[0].LinCfg.AnNumArr[2] = 0;
	 //pshdfr->pULinParm[0].LinCfg.AnNumArr[3] = 0;
	 //pshdfr->pULinParm[0].LinCfg.AnNumArr[4] = 0;
	 //pshdfr->pULinParm[0].LinCfg.AnNumArr[5] = 0;
	 //pshdfr->pULinParm[0].LinCfg.AnNumArr[6] = 0;
	 //pshdfr->pULinParm[0].LinCfg.AnNumArr[7] = 0;
     //pshdfr->pULinParm[0].LinCfg.ExtrBinCnt = 0;

	 //for(int i=0; i<16; i++)
		// pshdfr->pULinParm[0].LinCfg.BinNumArr[i] = 0;
     //  
	 //pshdfr->pULinParm[0].LinCfg.FileRcdEna = 0;
	 //pshdfr->pULinParm[0].LinCfg.PQEnable = 0;
	 //pshdfr->pULinParm[0].LinCfg.LinIdx2KD = 0;
	 //pshdfr->pULinParm[0].LinCfg.Resv[0] = 0;   //Resv[21]*/
    
	 //line2 电流线路
   for(int i=0; i<4; ++i){
     sprintf(pshdfr->pILinParm[i].LinCfg.Name, "电流线%d",i+1);   /*!< 线路名称 */
     pshdfr->pILinParm[i].LinCfg.Index = i+3;       /*!< 线路顺序索引号，从1开始的连续正整数*/
     pshdfr->pILinParm[i].LinCfg.LinTyp = 0;      /*!< 标识该线路类型，0--单回线，1--双回线，2--旁路（旁路开关的电流），3--母联，4--电铁线，5--其他*/
     pshdfr->pILinParm[i].LinCfg.PhTyp = 4;       /*!< 接线方式,4-ABCN,3-ABC,2-AC,1-单通道*/
     //Resv1[2];                                  /*!< 预留1，保证字节边界对齐 */
     //pshdfr->pILinParm[0].LinCfg.ULinNum = 0;     /*!< 线路对应的电压索引号 */
     //pshdfr->pILinParm[0].LinCfg.ParaNum = 0;     /*!< 对应的双回线路号(仅对双回线路有意义) */
     pshdfr->pILinParm[i].LinCfg.ChNum[0] = 0;    /*!< Ia对应的通道号,=0不存在Ia */
     pshdfr->pILinParm[i].LinCfg.ChNum[1] = 0;    /*!< Ib对应的通道号,=0不存在Ia*/
     pshdfr->pILinParm[i].LinCfg.ChNum[2] = 0;    /*!< Ic对应的通道号,=0不存在Ic*/
     pshdfr->pILinParm[i].LinCfg.ChNum[3] = 0;    /*!< 3I0对应的通道号,=0不存在3I0 */
     //pshdfr->pILinParm[0].LinCfg.ChHf1Num = 0;  /*!< 高频通道1对应的通道号,=0不存在Hf1 */
     //pshdfr->pILinParm[0].LinCfg.ChHf2Num = 0;  /*!< 高频通道2对应的通道号,=0不存在Hf2 */
     //pshdfr->pILinParm[0].LinCfg.ExtrAnCnt = 0;   /*!< 与该线路相关的其它模拟量的通道的个数 */
     //pshdfr->pILinParm[0].LinCfg.AnNumArr[8];   /*!< 与该线路相关的其它模拟量的通道号 */
     //pshdfr->pILinParm[0].LinCfg.ExtrBinCnt;    /*!< 与该线路相关的其它状态量的通道的个数 */
     //pshdfr->pILinParm[0].LinCfg.BinNumArr[36]; /*!< 与该线路相关的其它状态量的通道号 */
     //pshdfr->pILinParm[0].LinCfg.CurDir;        /*!< 电流方向 */
     //pshdfr->pILinParm[0].LinCfg.FltLocEna;     /*!< 标识是否启用故障测距 */
     //pshdfr->pILinParm[0].LinCfg.FileRcdEna;    /*!< 标识是否生成该线路的分通道文件 */
     //pshdfr->pILinParm[0].LinCfg.PQEnable;	  /*!< 是否启用电能质量功能：1=启用;0=不启用。*/
     //pshdfr->pILinParm[0].LinCfg.LinIdx2KD;     /*!< 2KD线路索引号（为与2KD定值互相转换而添加） */
     //pshdfr->pILinParm[0].LinCfg.ULinNumBak;    /*!< 备用电压索引号 */
     //pshdfr->Resv[2];        /*!< 预留，
   }

	 return pshdfr;
}

int sg_comp_dfrcfg( const SHDFR_SG* pdfr_sg1, const SHDFR_SG* pdfr_sg2 )
{
	if ( pdfr_sg1 == pdfr_sg2 )
		return 0;
	
	if ( pdfr_sg1 == NULL || pdfr_sg2 == NULL )
		return 1;
	
	if ( pdfr_sg1->pDFRParm == pdfr_sg2->pDFRParm )
		return 0;

	if ( pdfr_sg1->pDFRParm == NULL || pdfr_sg2->pDFRParm == NULL )
		return 1;
	
	if ( pdfr_sg1->pDFRParm->DfrCfg.AnChCnt  != pdfr_sg2->pDFRParm->DfrCfg.AnChCnt ||
		 pdfr_sg1->pDFRParm->DfrCfg.BinChCnt != pdfr_sg2->pDFRParm->DfrCfg.BinChCnt ||
		 pdfr_sg1->pDFRParm->DfrCfg.ULinCnt  != pdfr_sg2->pDFRParm->DfrCfg.ULinCnt ||
		 pdfr_sg1->pDFRParm->DfrCfg.ILinCnt  != pdfr_sg2->pDFRParm->DfrCfg.ILinCnt ||
		 pdfr_sg1->pDFRParm->DfrCfg.FLinCnt  != pdfr_sg2->pDFRParm->DfrCfg.FLinCnt ||
		 pdfr_sg1->pDFRParm->DfrCfg.TrnCnt   != pdfr_sg2->pDFRParm->DfrCfg.TrnCnt ||
		 pdfr_sg1->pDFRParm->DfrCfg.GnCnt    != pdfr_sg2->pDFRParm->DfrCfg.GnCnt
		 )
	{
		return 1;
	}
	return 0;
}


int  dfr_sg_cpy( SHDFR_SG* sg_dst, const SHDFR_SG* sg_src, int flg )
{
	int  ret;
	void *data;

	if ( sg_src == NULL || sg_dst == NULL )
	{
		dbg_error(( "sg_src == NULL || sg_dst == NULL" ));
		return -1;
	}

	if ( sg_src == sg_dst )
	{
		dbg_warning(( "sg_src == sg_dst." ));
		return 0;
	}
	
	if ( sg_src->BufLen == 0 || sg_src->DataAddr == NULL )
	{
		dbg_error(( "source sg data is NULL!" ));
		return -1;
	}

	if ( sg_src->BufLen == sg_dst->BufLen &&
		 sg_comp_dfrcfg( sg_dst, sg_src ) == 0 )
	{
		memcpy( sg_dst->DataAddr, sg_src->DataAddr, sg_src->BufLen );
		return 0;
	}

	if ( flg == 0 )
	{
		dbg_error(( "sg data not match!" ));
		return -1;
	}

	data = calloc( 1, sg_src->BufLen );
	if ( NULL == data )
	{
		dbg_error(( "Error: no memory!" ));
		return -1;
	}
	
	if ( sg_dst->DataAddr )
	{
		free( sg_dst->DataAddr );
		sg_dst->DataAddr = NULL;
		sg_dst->BufLen = 0;
	}
	
	memcpy( data, sg_src->DataAddr, sg_src->BufLen );
	ret = initDfrSg( sg_dst, data, sg_src->BufLen, 1 );
	if ( ret != 0 )
	{
		dbg_error(( "initDfrSg() failed!" ));
		free( data );
		return -1;
	}
	
	return 0;
}

SHDFR_SG* SGManager::cloneDfrSGData( const SHDFR_SG* sg_src )
{
	int ret;
	SHDFR_SG* dfr_sg = (SHDFR_SG*) calloc ( 1, sizeof(SHDFR_SG) );
	if ( NULL == dfr_sg )
	{
		dbg_error(( "No memory!" ));
		return NULL;
	}
	
	ret = dfr_sg_cpy( dfr_sg, sg_src, 1 );
	if ( ret != 0 )
	{
		dbg_error(( "dfr_sg_cpy failed!" ));
		free( dfr_sg );
		return NULL;
	}
	
	return dfr_sg;
}

const char* getAnChPhase( const SHDFR_SG* srcSG, int nChannelID)
{
	int i,nULineCount,nILineCount;

    if (srcSG->pDFRParm == NULL)
    {
        return "";
    }
    
    /* 线路数 */
    nULineCount = srcSG->pDFRParm->DfrCfg.ULinCnt;
    nILineCount = srcSG->pDFRParm->DfrCfg.ILinCnt;
    
    /* 在母线中查找 */
    for ( i=0; i<nULineCount; i++)
    {
        YH_ULIN_CFG* cfg = &srcSG->pULinParm[i].LinCfg;
        
        if (nChannelID == cfg->ChNum[0]) return "A";
        if (nChannelID == cfg->ChNum[1]) return "B";
        if (nChannelID == cfg->ChNum[2]) return "C";
        if (nChannelID == cfg->ChNum[3]) return "N";
    }
    
    /* 在线路中查找 */
    for ( i=0; i<nILineCount; i++)
    {
        YH_ILIN_CFG* cfg = &srcSG->pILinParm[i].LinCfg;
        
        if (nChannelID == cfg->ChNum[0]) return "A";
        if (nChannelID == cfg->ChNum[1]) return "B";
        if (nChannelID == cfg->ChNum[2]) return "C";
        if (nChannelID == cfg->ChNum[3]) return "N";
    }
    
    return "";
}

const char* getUnitStr(int nMultiplier, int nUnitType, char* buf )
{
    if ( buf == NULL )
    	return NULL;
    
    switch (nMultiplier)
    {
		case -24:   strcpy( buf, "y" ); break;
        case -21:   strcpy( buf, "z" ); break;
        case -18:   strcpy( buf, "a" ); break;
        case -15:   strcpy( buf, "f" ); break;
        case -12:   strcpy( buf, "p" ); break;
        case -9:    strcpy( buf, "n" ); break;
        case -6:    strcpy( buf, "μ" ); break;
        case -3:    strcpy( buf, "m" ); break;
        case -2:    strcpy( buf, "c" ); break;
        case -1:    strcpy( buf, "d" ); break;
        case 0:	    strcpy( buf, ""  ); break;
        case 1:	    strcpy( buf, "da"); break;
        case 2:	    strcpy( buf, "h" ); break;
        case 3:	    strcpy( buf, "k" ); break;
        case 6:	    strcpy( buf, "M" ); break;
        case 9:	    strcpy( buf, "G" ); break;
        case 12:    strcpy( buf, "T" ); break;
        case 15:    strcpy( buf, "P" ); break;
        case 18:    strcpy( buf, "E" ); break;
        case 21:    strcpy( buf, "Z" ); break;
        case 24:    strcpy( buf, "Y" ); break;
        default :   strcpy( buf, ""  ); break;
    }
    
    switch (nUnitType)
    {
        case 0: 
        case 1:  break;
        case 2:  strcat( buf, "m" );	break;
        case 3:  strcat( buf, "kg" );	break;
        case 4:  strcat( buf, "s" );	break;
        case 5:  strcat( buf, "A" );	break;
        case 6:  strcat( buf, "K" );	break;
        case 7:  strcat( buf, "mol" );	break;
        case 8:  strcat( buf, "cd" );	break;
        case 9:  strcat( buf, "deg" );	break;
        case 10: strcat( buf, "rad" );	break;
        case 11: strcat( buf, "sr" );	break;
        case 21: strcat( buf, "Gy" );	break;
        case 22: strcat( buf, "q" );	break;
        case 23: strcat( buf, "℃" );	break;
        case 24: strcat( buf, "Sv" );	break;
        case 25: strcat( buf, "F" );	break;
        case 26: strcat( buf, "C" );	break;
        case 27: strcat( buf, "S" );	break;
        case 28: strcat( buf, "H" );	break;
        case 29: strcat( buf, "V" );	break;
        case 30: strcat( buf, "ohm" );	break;
        case 31: strcat( buf, "J" );	break;
        case 32: strcat( buf, "N" );	break;
        case 33: strcat( buf, "Hz" );	break;
        case 34: strcat( buf, "lx" );	break;
        case 35: strcat( buf, "Lm" );	break;
        case 36: strcat( buf, "Wb" );	break;
        case 37: strcat( buf, "T" );	break;
        case 38: strcat( buf, "W" );	break;
        case 39: strcat( buf, "Pa" );	break;
        case 41: strcat( buf, "m2" );	break;
        case 42: strcat( buf, "m3" );	break;
        case 43: strcat( buf, "m/s" );	break;
        case 44: strcat( buf, "m/s2" );	break;
        case 45: strcat( buf, "m3/s" );	break;
        case 46: strcat( buf, "m/m3" );	break;
        case 47: strcat( buf, "M" );	break;
        case 48: strcat( buf, "kg/m3" );break;
        case 49: strcat( buf, "m2/s" ); break;
        case 50: strcat( buf, "W/m K" );break;
        case 51: strcat( buf, "J/K" );	break;
        case 52: strcat( buf, "ppm" );	break;
        case 53: strcat( buf, "1/s" );	break;
        case 54: strcat( buf, "rad/s" );break;
        case 61: strcat( buf, "VA" );	break;
        case 62: strcat( buf, "W" );	break;
        case 63: strcat( buf, "VAr" );	break;
        case 64: strcat( buf, "theta" );break;
        case 65: strcat( buf, "cos(theta)" );	break;
        case 66: strcat( buf, "Vs" );	break;
        case 67: strcat( buf, "V2" );	break;
        case 68: strcat( buf, "As" );	break;
        case 69: strcat( buf, "A2" );	break;
        case 70: strcat( buf, "A2t" );	break;
        case 71: strcat( buf, "VAh" );	break;
        case 72: strcat( buf, "Wh" );	break;
        case 73: strcat( buf, "VArh" );	break;
        case 74: strcat( buf, "V/Hz" ); break;

        default: 
        	break;
    }
    
    return buf;
}

SHDFR_SG* SGManager::dfr_sg_pt_cpy(SHDFR_SG* sg_dst, int nUnum, int nInum, const SHDFR_SG* sg_src)
{
	assert(sg_src!=NULL && sg_dst!=NULL);

	////new mem
	//SHDFR_SG* sg_dst = (SHDFR_SG*)calloc(1, sizeof(SHDFR_SG));
	//if ( NULL == sg_dst )
	//{
	//	dbg_error(( "No memory!" ));
	//	return NULL;
	//}
	
	memcpy(sg_dst->pDFRParm, sg_src->pDFRParm, 
		sizeof(YH_DFR_PARM) + 
		sizeof(YH_BINCH_PARM)*sg_src->pDFRParm->DfrCfg.BinChCnt + 
		sizeof(YH_ANCH_PARM)*sg_src->pDFRParm->DfrCfg.AnChCnt);

	sg_dst->pDFRParm->DfrCfg.ULinCnt = nUnum;
    sg_dst->pDFRParm->DfrCfg.ILinCnt = nInum; 
	
	memcpy(sg_dst->pTrnParm, sg_src->pTrnParm, 
		sizeof(YH_TRN_PARM) * sg_src->pDFRParm->DfrCfg.TrnCnt +
		sizeof(YH_GN_PARM) * sg_src->pDFRParm->DfrCfg.GnCnt); 

	return sg_dst;
}

int sg_comp_dfr_val( const SHDFR_SG* pdfr_sg1, const SHDFR_SG* pdfr_sg2 )
{
	assert(pdfr_sg1 != NULL && pdfr_sg2 != NULL);

	if(pdfr_sg1->BufLen != pdfr_sg2->BufLen)
	{
		return -1;
	}

#ifdef WIN32
	if( memcmp(pdfr_sg1->DataAddr, pdfr_sg2->DataAddr, pdfr_sg1->BufLen) )
	{
		return -1;
	}

#else
	if( bcmp(pdfr_sg1->DataAddr, pdfr_sg2->DataAddr, pdfr_sg1->BufLen) )
	{
		return -1;
	}

#endif

	return 0;
}

