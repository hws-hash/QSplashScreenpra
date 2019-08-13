/*************************************************************************
* Copyright (c) 2011,深圳市元恒丰电气技术有限公司
* All rights reserved.

**************************************************************************/

/*!	\file
 	\brief		基本算法函数的实现
 	
	\author		chen
	\version	1.0
	\date		2011-12-22
	
	实现一些基本的算法函数
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>

#include "calc_def.h"

#if 1
	#define	dbg_printf(argv)	printf argv; printf("\n");
#else
	#define	dbg_printf(argv)
#endif

#define SQRT2		(1.4142135f)
static FLOAT32		gs_cosf[128];						//余弦系数表，用于频率计算
static FLOAT32		gs_sinf[128];						//正弦系数表，用于频率计算

/*!
	\brief	生成DFT计算所需的正、余弦系数表，用于幅值计算，基于额定频率
	\param	psinf		[out]	正弦系数表
	\param	pcosf		[out]	余弦系数表
	\param	k			[in]	谐波次数
	\param  sampleNum	[in]	每周波的计算点数
*/
void init_coefficient_table( float* pcosf, float* psinf, int k, int sampleNum)
{
	int i;
	float miVar = (float)( 2 * k * PI / sampleNum);

	for ( i = 0; i < sampleNum; i++ )
	{
		*pcosf = (float)cos( i * miVar ) ;	
		*psinf = (float)sin( i * miVar );
		pcosf++;
		psinf++;
	}
}

/*!
	\brief	生成DFT计算所需的正、余弦系数表，用于频率计算
	\param	psinf	[out]	正弦系数表
	\param	pcosf	[out]	余弦系数表
	\param	N		[in]	系数表个数
	\param  sample  [in]    每周的实际采样点数，采样时间间隔固定时，随系统频率变化而变化
*/
void coefficient_table( float* pcosf, float* psinf, int N, float sampleNum )
{
	int i;

	float miVar = (float)( 2 * PI / sampleNum );

	for ( i = 0; i < N; i++ )
	{
		*pcosf = (float)cos( i * miVar ) ;	
		*psinf = (float)sin( i * miVar );
		pcosf++;
		psinf++;
	}
}


/*!
	\brief	初始化DFT计算所需的正、余弦系数表
	\param	pcoeff	[in]	系数表起始地址
	\param	k		[in]	谐波次数
	\param	N		[in]	每周波采样点数
	\param  skew    [in]    角度偏移
*/
void init_coefficient_table_angle( float* pcoeff, int k, int N, float skew )
{
	int i;

	for ( i = 0; i < N; i++, pcoeff++ )
	{
		*pcoeff = (float)sin( k*i*2*PI/N + skew/k);		//正弦系数
	}

	for ( i = 0; i < N; i++, pcoeff++ )
	{
		*pcoeff = (float)cos( k*i*2*PI/N + skew/k) ;	//余弦系数
	}
}

/*!
	\brief	生成DFT计算所需的正、余弦系数表
	\param	k		[in]	谐波次数
	\param	N		[in]	每周波采样点数
	\param  skew    [in]    角度偏移
*/
float* creat_coefficient_table( int k, int N, float skew )
{
	float *pfdata; 
	pfdata = (float*)malloc( 2 * N * sizeof(float) );
	if ( pfdata == NULL )
		return NULL;
			
	init_coefficient_table_angle( pfdata, k, N, skew );
	return pfdata;
}


/*!
	\brief	改进DFT计算，计算基波、5次、9次谐波
	\param	pfSrc		[in]	采样数据起始地址
	\param  pcosf		[in]	余弦系数表
	\param	psinf		[in]	正弦系数表
	\param	quarterNum	[in]	分别对应四分之一周波、半周波、四分之三周波、一周波计算点数
	\param	k			[in]	谐波次数，对应于基波，5，9次谐波
	\param	pComplex	[out]	指向计算结果结构的指针
*/
void improved_DFT159( float* pfSrc, float* pcosf, float* psinf, int* quarterNum, COMPLEX_R* pComplex )
{
	int i = 1;
	
	pComplex->Real = pComplex->Imag = 0;
	
	for ( ; i< *quarterNum; i++)
	{
		pComplex->Imag += *(pcosf+i) * ( *(pfSrc+i) - *(pfSrc+*(quarterNum+1)-i) - *(pfSrc+*(quarterNum+1)+i) + *(pfSrc+*(quarterNum+3)-i) );
		pComplex->Real += *(psinf+i) * ( *(pfSrc+i) + *(pfSrc+*(quarterNum+1)-i) - *(pfSrc+*(quarterNum+1)+i) - *(pfSrc+*(quarterNum+3)-i) );
	}

	//处理位于四分之一周波处的特殊点
	pComplex->Imag += *pfSrc - *(pfSrc+*(quarterNum+1));
	pComplex->Real += *(pfSrc+*quarterNum) - *(pfSrc+*(quarterNum+2));

	pComplex->Real = SQRT2 * pComplex->Real / *(quarterNum+3);
	pComplex->Imag = SQRT2 * pComplex->Imag / *(quarterNum+3);
	pComplex->Rms  = (float)( sqrt( pComplex->Real * pComplex->Real + pComplex->Imag * pComplex->Imag ) );
}

/*!
	\brief	改进DFT计算，计算3，7次谐波
	\param	pfSrc		[in]	采样数据起始地址
	\param  pcosf		[in]	余弦系数表
	\param	psinf		[in]	正弦系数表
	\param	quarterNum	[in]	分别对应四分之一周波、半周波、四分之三周波、一周波计算点数
	\param	k			[in]	谐波次数，对应于3，7次谐波
	\param	pComplex	[out]	指向计算结果结构的指针
*/
void improved_DFT37( float* pfSrc, float* pcosf, float* psinf, int* quarterNum, COMPLEX_R* pComplex )
{
	int i = 1;
	
	pComplex->Real = pComplex->Imag = 0;
	
	for ( ; i< *quarterNum; i++)
	{
		pComplex->Imag += *(pcosf+i) * ( *(pfSrc+i) - *(pfSrc+*(quarterNum+1)-i) - *(pfSrc+*(quarterNum+1)+i) + *(pfSrc+*(quarterNum+3)-i) );
		pComplex->Real += *(psinf+i) * ( *(pfSrc+i) + *(pfSrc+*(quarterNum+1)-i) - *(pfSrc+*(quarterNum+1)+i) - *(pfSrc+*(quarterNum+3)-i) );
	}

	//处理位于四分之一周波处的特殊点
	pComplex->Imag += *pfSrc - *(pfSrc+*(quarterNum+1));
	pComplex->Real += -*(pfSrc+*quarterNum) + *(pfSrc+*(quarterNum+2));

	pComplex->Real = SQRT2 * pComplex->Real / *(quarterNum+3);
	pComplex->Imag = SQRT2 * pComplex->Imag / *(quarterNum+3);
	pComplex->Rms  = (float)( sqrt( pComplex->Real * pComplex->Real + pComplex->Imag * pComplex->Imag ) );
}


/*!
	\brief	改进DFT，只计算二次谐波
	\param	pfSrc		[in]	采样数据起始地址
	\param  pcosf		[in]	余弦系数表
	\param	psinf		[in]	正弦系数表
	\param	quarterNum	[in]	分别对应四分之一周波、半周波、四分之三周波、一周波计算点数
	\param	pComplex	[out]	指向计算结果结构的指针
*/
void improved_DFT2( float* pfSrc, float* pcosf, float* psinf, int* quarterNum, COMPLEX_R* pComplex )
{
	int i = 1;

	pComplex->Real = pComplex->Imag = 0;

	for ( ; i< *quarterNum; i++)
	{
		pComplex->Imag += *(pcosf+i) * ( *(pfSrc+i) - *(pfSrc+*quarterNum+i) + *(pfSrc+*(quarterNum+1)+i) - *(pfSrc+*(quarterNum+2)+i) );
		pComplex->Real += *(psinf+i) * ( *(pfSrc+i) - *(pfSrc+*quarterNum+i) + *(pfSrc+*(quarterNum+1)+i) - *(pfSrc+*(quarterNum+2)+i) );
	}
	
	pComplex->Imag +=  *pfSrc - *(pfSrc+*quarterNum) + *(pfSrc+*(quarterNum+1)) - *(pfSrc+*(quarterNum+2));

	pComplex->Real = SQRT2 * pComplex->Real / *(quarterNum+3);
	pComplex->Imag = SQRT2 * pComplex->Imag / *(quarterNum+3);
	pComplex->Rms  = (float)( sqrt( pComplex->Real * pComplex->Real + pComplex->Imag * pComplex->Imag ) );
}


/*!
	\brief	DFT计算，基波实部 虚部和有效值, 谐波有效值，适用于频率等于额定频率情况
	\param	pfSrc		[in]	采样数据起始地址
	\param	pcoeff		[in]	正余弦系数表首地址
	\param	pComplex	[out]	指向计算结果结构的指针
*/
void calc_DFT( float* pfSrc, float* pCoeff, COMPLEX_R* pComplex )
{
	int     i;
	float	*pfData, *cosAdr, *sinAdr;
	float	ak = 0.0, bk = 0.0;          

	pfData = pfSrc;
	cosAdr = pCoeff;
	sinAdr = pCoeff + DFT_PNT_CNT_PER_CYC;
	for (i = 0; i < DFT_PNT_CNT_PER_CYC; i++)			
	{	
		ak += (*cosAdr) * (*pfData);						/*! DFT计算实部 */
		bk += (*sinAdr) * (*pfData);						/*! DFT计算虚部 */
		cosAdr++;
		sinAdr++;
		pfData++;
	}

	pComplex->Real =   SQRT2 * bk / DFT_PNT_CNT_PER_CYC;
	pComplex->Imag =   SQRT2 * ak / DFT_PNT_CNT_PER_CYC;	
	pComplex->Rms  = (float)( sqrt( pComplex->Real * pComplex->Real + pComplex->Imag * pComplex->Imag ) );	
}

/*!
	\brief	相邻周波正序相角差计算，适用于频率不等于额定频率情况
	\param	prevSrc			[in]	上一周波采样数据
	\param  curSrc			[in]	当前周波采样数据
	\param  nextSrc			[in]	下一周波采样数据
	\param	idxU			[in]	A，B，C三相电压通道号
	\param	sampleNum		[in]	每周波计算点数
	\param	prevFreq		[in]	上一周波频率
	\param  ratedFreq		[in]	额定频率
	\param	curAngle		[out]	指向计算结果的指针
*/
FLOAT32 calc_pos_angle( FLOAT32* prevSrc, FLOAT32* curSrc,FLOAT32* nextSrc, INT16* idxU, INT16 sampleNum, FLOAT32 prevFreq, INT32 ratedFreq)
{
	int			i, j, iCnt, iDif;
	float		*prevData, *curData, *pf_cosf, *pf_sinf, fcnt;
	float		prev_real[3], prev_imag[3], cur_real[3], cur_imag[3], prev_pos_real, prev_pos_imag, cur_pos_real, cur_pos_imag;          
	
	if ( prevFreq >= (0.5*ratedFreq) )
		fcnt = (float)(ratedFreq * sampleNum / prevFreq );	
	else
		fcnt = sampleNum;

	coefficient_table( gs_cosf, gs_sinf, (int)( fcnt + 1 ), fcnt);	
	
	iCnt = (int)fcnt;
	
	for (j=0; j<3; j++)
	{
		prev_real[j] = 0;
		prev_imag[j] = 0;
		cur_real[j] = 0;
		cur_imag[j] = 0;

		pf_cosf = gs_cosf;
		pf_sinf = gs_sinf;
		prevData = prevSrc + *(idxU + j) * sampleNum;
		curData  = curSrc + *(idxU + j) * sampleNum;

		if ( fcnt < sampleNum )										/*! 频率大于额定值，实际点数小于采样点数 */
		{
			for (i=0; i<iCnt; i++)			
			{	
				prev_real[j] += (*pf_sinf) * (*prevData);							
				prev_imag[j] += (*pf_cosf) * (*prevData);	
				cur_real[j]  += (*pf_sinf) * (*curData);							
				cur_imag[j]  += (*pf_cosf) * (*curData);

				pf_cosf++;
				pf_sinf++;
				prevData++;
				curData++;
			}
		}
		else														/*! 频率小于额定值，实际点数大于采样点数 */
		{
			iDif = iCnt - sampleNum;
			for (i=0; i< sampleNum; i++)			
			{	
				prev_real[j] += (*pf_sinf) * (*prevData);							
				prev_imag[j] += (*pf_cosf) * (*prevData);	
				cur_real[j]  += (*pf_sinf) * (*curData);							
				cur_imag[j]  += (*pf_cosf) * (*curData);

				pf_cosf++;
				pf_sinf++;
				prevData++;
				curData++;
			}

			prevData = curSrc + *(idxU + j) * sampleNum;
			curData  = nextSrc + *(idxU + j) * sampleNum;

			for (i=0; i<iDif; i++)
			{
				prev_real[j] += (*pf_sinf) * (*prevData);							
				prev_imag[j] += (*pf_cosf) * (*prevData);	
				cur_real[j]  += (*pf_sinf) * (*curData);							
				cur_imag[j]  += (*pf_cosf) * (*curData);

				pf_cosf++;
				pf_sinf++;
				prevData++;
				curData++;
			}
		}

		//处理最后一个点
		prev_real[j] += (fcnt - iCnt) * (*pf_sinf) * (*prevData) ;
		prev_imag[j] += (fcnt - iCnt) * (*pf_cosf) * (*prevData);
		cur_real[j]  += (fcnt - iCnt) * (*pf_sinf) * (*curData) ;
		cur_imag[j]  += (fcnt - iCnt) * (*pf_cosf) * (*curData);	
	}
	
	prev_pos_real = SEQT0 * prev_real[0] - SEQT1 * (prev_real[1] + prev_real[2]) - SEQT2 * ( prev_imag[1] - prev_imag[2]);
	prev_pos_imag = SEQT0 * prev_imag[0] - SEQT1 * (prev_imag[1] + prev_imag[2]) + SEQT2 * ( prev_real[1] - prev_real[2]);
	cur_pos_real  = SEQT0 * cur_real[0] - SEQT1 * (cur_real[1] + cur_real[2]) - SEQT2 * ( cur_imag[1] - cur_imag[2]);
	cur_pos_imag  = SEQT0 * cur_imag[0] - SEQT1 * (cur_imag[1] + cur_imag[2]) + SEQT2 * ( cur_real[1] - cur_real[2]);

	return atan2(cur_pos_imag, cur_pos_real) - atan2(prev_pos_imag, prev_pos_real);

	//pComplex->Real = 2 * ak / fCnt;
	//pComplex->Imag = - 2 * bk / fCnt;	
	//pComplex->Rms  = (float)( sqrt( pComplex->Real * pComplex->Real + pComplex->Imag * pComplex->Imag ) );	
}



/*!
	\brief	DFT计算，基波实部 虚部和有效值, 谐波有效值，适用于频率不等于额定频率情况，
			小于额定频率时需用到下一周波数据
	\param	pfSrc		[in]	当前周波采样数据
	\param  nextSrc		[in]	下一周波采样数据
	\param	psinf		[in]	正弦系数表
	\param	pcosf		[in]	余弦系数表
	\param	fCnt		[in]	每周波的实际点数
	\param  sampleNum	[in]	每周波采样点数（抽点后的每周波点数，20点或32点）
*/
float calc_Hz_DFT( float* pfSrc, float* nextSrc, float* pcosf, float* psinf, float fCnt, int sampleNum )
{
	int     i, iCnt, iDif;
	float	*pfData;
	float	ak = 0.0, bk = 0.0;          
	
	iCnt = (int)fCnt;
	pfData = pfSrc;
	
	if ( fCnt < sampleNum )										/*! 频率大于额定值，实际点数小于采样点数 */
	{
		for (i = 0; i < iCnt; i++)			
		{	
			ak += (*psinf) * (*pfData);							
			bk += (*pcosf) * (*pfData);				
			pcosf++;
			psinf++;
			pfData++;
		}
	}
	else														/*! 频率小于额定值，实际点数大于采样点数 */
	{
		iDif = iCnt - sampleNum;
		for (i = 0; i < sampleNum; i++)			
		{	
			ak += (*psinf) * (*pfData);							
			bk += (*pcosf) * (*pfData);				
			pcosf++;
			psinf++;
			pfData++;
		}

		pfData = nextSrc;
		for (i = 0; i < iDif; i++)
		{
			ak += (*psinf) * (*pfData);							
			bk += (*pcosf) * (*pfData);				
			pcosf++;
			psinf++;
			pfData++;
		}
	}

	//处理最后一个点
	ak += (fCnt - iCnt) * (*psinf) * (*pfData) ;						
	bk += (fCnt - iCnt) * (*pcosf) * (*pfData);					

	return ( (float)atan2( bk, ak ) );
	
	//pComplex->Real = 2 * ak / fCnt;
	//pComplex->Imag = - 2 * bk / fCnt;	
	//pComplex->Rms  = (float)( sqrt( pComplex->Real * pComplex->Real + pComplex->Imag * pComplex->Imag ) );	
}

/*!
	\brief	直流计算
	\param	pfData		[in]	原始采样数据起始地址
	\param	fcnt		[in]	原始采样点的个数
	\param	pComplex	[out]	指向计算结果结构的指针
*/
void calc_DC( float* pfsrc, int fcnt, COMPLEX_R* pComplex )
{
	float	DC_value = 0;
	float	*pfData = pfsrc;
	float	*pfData_end = pfsrc + fcnt;

	for ( ; pfData < pfData_end; pfData++ )
	{
		DC_value += (*pfData) * (*pfData);
	}
	
	DC_value = (float) sqrt(DC_value / fcnt);
	pComplex->Real = DC_value;
	pComplex->Imag = 0.0;
	pComplex->Rms  = pComplex->Real;
}

/*!
	\brief	频率计算
	\param	pFront		[in]	前一周波采样数据起始地址
	\param	pCur		[in]	当前原始采样数据起始地址
	\param  pNext       [in]	下一周波采样值起始地址
	\param  currentHz	[in]    当前采用的系统频率
	\param	sampleNum	[in]	每周波采样率（抽点后的每周波点数，20点或32点 ）
	\param  ratedFreq	[in]	额定频率
	\return 返回计算结果
*/
float calc_freq( float* pPrev, float* pCur, float* pNext, float currentHz, int sampleNum, INT32 ratedFreq )
{
	float freq, fcnt, curAngle, prevAngle, difAngle;
	
	if (currentHz <= (0.5 * ratedFreq) )
	{
		currentHz = (float)ratedFreq;
	}

	//每周实际采样点数
	fcnt = (float)(ratedFreq * sampleNum / currentHz );	
	coefficient_table( gs_cosf, gs_sinf, (int)( fcnt + 1 ), fcnt);				
	
	//计算相邻两周波相位差
	curAngle  = calc_Hz_DFT(pCur, pNext, gs_cosf, gs_sinf, fcnt, sampleNum);
	prevAngle = calc_Hz_DFT(pPrev, pCur, gs_cosf, gs_sinf, fcnt, sampleNum);
	difAngle  = curAngle - prevAngle;

	//消除相角在正负PI时可能导致的错误
	if( difAngle > PI )
	{
		difAngle -= 2 * PI;
	}
	else if ( difAngle < -PI )
	{
		difAngle += 2 * PI;
	}

	freq = (float)(ratedFreq + 0.5 * difAngle * ratedFreq  / PI);

	/*! 计算频率与原频率差异过大时，再计算一次 */
	if ( fabs( currentHz  - freq ) > 0.5 )		
	{
		if (freq <= (0.5 * ratedFreq) )
		{
			freq = (float)ratedFreq;
		}

		//每周实际采样点数
		fcnt = (float)(ratedFreq * sampleNum / freq );	
		coefficient_table( gs_cosf, gs_sinf, (int)( fcnt + 1 ), fcnt);					

		//计算相邻两周波相位差
		curAngle  = calc_Hz_DFT(pCur, pNext, gs_cosf, gs_sinf, fcnt, sampleNum);
		prevAngle = calc_Hz_DFT(pPrev, pCur, gs_cosf, gs_sinf, fcnt, sampleNum);
		difAngle  = curAngle - prevAngle;

		//消除相角在正负PI时可能导致的错误
		if( difAngle > PI )
		{
			difAngle -= 2 * PI;
		}
		else if ( difAngle < -PI )
		{
			difAngle += 2 * PI;
		}

		freq = (float)(ratedFreq + 0.5 * difAngle * ratedFreq  / PI);
	}
	

	if((freq > (1.5 * ratedFreq) ) || (freq < (0.5 * ratedFreq) ))
	{
		freq = (float)ratedFreq;
	}
	return freq;
}


/*!
	\brief	序量计算
	\param	phsA	[in]	A相计算结果
	\param	phsB	[in]	B相计算结果
	\param	phsC	[in]	C相计算结果
	\param	pseq	[out]	序量计算结果输出
*/
void calc_seq( COMPLEX_R* phsA, COMPLEX_R* phsB, COMPLEX_R* phsC, SEQ_VAL* pseq )
{	
	float real,imag, tmp1, tmp2, tmp3, tmp4;
	
	//正序、负序计算中存在相同的计算过程，在此用临时变量保存起来，以提高总体效率
	tmp1 = SEQT0 * phsA->Real - SEQT1 * (phsB->Real + phsC->Real);
	tmp2 = SEQT2 * (phsB->Imag - phsC->Imag);
	tmp3 = SEQT0 * phsA->Imag - SEQT1 * (phsB->Imag + phsC->Imag);
	tmp4 = SEQT2 * (phsB->Real - phsC->Real);
	
	//正序计算
	real = tmp1 - tmp2;
	imag = tmp3 + tmp4;
    pseq->c1 = (float)sqrt((real*real + imag*imag));
    
    //负序计算
	real = tmp1 + tmp2;
	imag = tmp3 - tmp4;
    pseq->c2 = (float)sqrt((real*real + imag*imag));
    
    //零序计算 3U0/3I0
    real = phsA->Real + phsB->Real + phsC->Real;
    imag = phsA->Imag + phsB->Imag + phsC->Imag;
    pseq->c0 = (float)sqrt((real*real + imag*imag));
}

/*!
	\brief	序量计算：用于电压量序分量计算，其中正序需求得矢量值，用于频率计算
	\param	phsA	[in]	A相计算结果
	\param	phsB	[in]	B相计算结果
	\param	phsC	[in]	C相计算结果
	\param	pseq	[out]	序量计算结果输出
*/
void calc_useq( COMPLEX_R* phsA, COMPLEX_R* phsB, COMPLEX_R* phsC, SEQ_VAL* pseq, FLOAT32* psangle )
{	
	float real,imag, tmp1, tmp2, tmp3, tmp4;
	
	//正序、负序计算中存在相同的计算过程，在此用临时变量保存起来，以提高总体效率
	tmp1 = SEQT0 * phsA->Real - SEQT1 * (phsB->Real + phsC->Real);
	tmp2 = SEQT2 * (phsB->Imag - phsC->Imag);
	tmp3 = SEQT0 * phsA->Imag - SEQT1 * (phsB->Imag + phsC->Imag);
	tmp4 = SEQT2 * (phsB->Real - phsC->Real);
	
	//正序计算
	real = tmp1 - tmp2;
	imag = tmp3 + tmp4;
	*psangle = atan2(imag,real);
    pseq->c1 = (float)sqrt((real*real + imag*imag));
    
    //负序计算
	real = tmp1 + tmp2;
	imag = tmp3 - tmp4;
    pseq->c2 = (float)sqrt((real*real + imag*imag));
    
    //零序计算 3U0/3I0
    real = phsA->Real + phsB->Real + phsC->Real;
    imag = phsA->Imag + phsB->Imag + phsC->Imag;
    pseq->c0 = (float)sqrt((real*real + imag*imag));
}


/*!
	\brief	单相功率计算
	\brief  pU	  [in]	电压
	\brief	pI	  [in]	电流
	\brief  power [out] 单相功率
*/
POWER_VAL cal_power( COMPLEX_R* pU, COMPLEX_R* pI)
{
	POWER_VAL power; 
	power.PPower = pU->Real * pI->Real + pU->Imag * pI->Imag;
	power.QPower = pU->Imag * pI->Real - pU->Real * pI->Imag;
	power.ApparentPower = pU->Rms * pI->Rms;
	
	return power;
}

/*!
	\brief	三相功率计算
	\param	phsA	[in]	A相计算结果
	\param	phsB	[in]	B相计算结果
	\param	phsC	[in]	C相计算结果
	\param	power3	[out]	功率计算结果输出
*/
POWER_VAL cal_power3( COMPLEX_R* pUa, COMPLEX_R* pUb, COMPLEX_R* pUc, COMPLEX_R* pIa, COMPLEX_R* pIb, COMPLEX_R* pIc)  
{
	POWER_VAL power3;

	power3.PPower = pUa->Real * pIa->Real + pUb->Real * pIb->Real + pUc->Real * pIc->Real +
					pUa->Imag * pIa->Imag + pUb->Imag * pIb->Imag + pUc->Imag * pIc->Imag;

	power3.QPower = -pUa->Real * pIa->Imag - pUb->Real * pIb->Imag - pUc->Real * pIc->Imag +
					pUa->Imag * pIa->Real + pUb->Imag * pIb->Real + pUc->Imag * pIc->Real;

	power3.ApparentPower = pUa->Rms * pIa->Rms + pUb->Rms * pIb->Rms + pUc->Rms * pIc->Rms;
										
	
	return power3;
}
