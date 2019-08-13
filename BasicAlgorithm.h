#ifndef BasicAlgorithm_H
#define BasicAlgorithm_H

#ifdef PRAGMA
#pragma interface "BasicAlgorithm.h"
#endif

/* {{{USR
******************************************************************************
* Copyright (c) 2008,深圳市元恒丰电气技术有限公司研发部
* All rights reserved.
* 
* 文件名称：BasicAlgorithm.h
* 文件标识：见配置管理计划书
* 摘    要：基础算法类
* 
* 当前版本：1.1
* 作    者：陈铁明
* 完成日期：2008年5月12日
*
* 取代版本：1.0
* 原作者  ：陈铁明
* 完成日期：2007年5月10日
******************************************************************************
   }}}USR */

//#include <Experts.h>
//#include <ComplexEx.h>
#include"yhftypes.h"
#pragma pack(1)

struct SHComplex
{
	FLOAT64 re;
	FLOAT64 im;
	FLOAT64 mag; 
	FLOAT64 arg;
}; 

#pragma pack()

class BasicAlgorithm
{
public:
	/* {{{USR
	******************************************************************************
	  Function:     //BOOL SHGetSequence( FLOAT32 * pData[3], INT32 perCycPoints, FLOAT32 * pSequence )
	  Description:  //计算序分量和三相不平衡度
	  Input:        //输入pData[3]三相数据、perCycPoints每周期点数
	  Output:       //FLOAT32 *pSequence：pSequence[0、2、4]为正负零序有效值
					//pSequence[1、3、5]为正负零序相角（度0->360）
					//pSequence[6]为三相不平衡度
	  Return:       //BOOL：计算成功返回True，失败返回False
	  Others:		//pSequence需要调用者事先申请空间FLOAT32[7]
	******************************************************************************
	   }}}USR */
	BOOL SHGetSequence( FLOAT32 * pData[3], INT32 perCycPoints, FLOAT32 * pSequence );
	/* {{{USR
	******************************************************************************
	  Function:     //BOOL SHGetSequence( INT16 * pData[3], INT32 perCycPoints, FLOAT32 * pSequence )
	  Description:  //计算序分量和三相不平衡度
	  Input:        //输入pData[3]三相数据、perCycPoints每周期点数
	  Output:       //FLOAT32 *pSequence：pSequence[0、2、4]为正负零序有效值
					//pSequence[1、3、5]为正负零序相角（度0->360）
					//pSequence[6]为三相不平衡度
	  Return:       //BOOL：计算成功返回True，失败返回False
	  Others:		//pSequence需要调用者事先申请空间FLOAT32[7]
	******************************************************************************
	   }}}USR */
	BOOL SHGetSequence( INT16 * pData[3], FLOAT32 * multiplier, INT32 perCycPoints, FLOAT32 * pSequence );

	/* {{{USR
	******************************************************************************
	  Function:		//BOOL SHGetHarmonic( FLOAT32 * pData, INT32 maxNum, INT32 perCycPoints, FLOAT32 * pHarmonic )
	  Description:	//谐波计算，计算0-到maxNum次谐波。（0-直流量，1-基波）
	  Input:		//FLOAT32 *pData：一周波数据开始地址
					//INT32 maxNum：最大计算的谐波次数，即计算0'maxNum次谐波和相角
					//INT32 perCycPoints：一周波点数
	  Output:		//FLOAT32 *pHarmonic：pHarmonic[2n]第n次谐波有效值；pHarmonic[2n+1]第n次谐波相位角，n从0到maxNum
					//pHarmonic[0]直流，pHarmonic[1]THD值，pHarmonic[2]基波有效值，pHarmonic[3] 基波相位角(0->360度)
	  Return:		//BOOL：计算成功返回True，失败返回False
	  Others:		//pHarmonic需要调用者事先申请空间FLOAT32[2*maxNum+2]
	******************************************************************************
	   }}}USR */
	BOOL SHGetHarmonic( FLOAT32 * pData, INT32 maxNum, INT32 perCycPoints, FLOAT32 * pHarmonic );

	/* {{{USR
	******************************************************************************
	  Function:		//BOOL SHGetHarmonic( INT16 * pData, FLOAT32 multiplier, INT32 perCycPoints, INT32 beginNum, INT32 maxNum, FLOAT32 * pHarmonic )
	  Description:	//谐波计算，计算beginNum到maxNum次谐波。（0-直流量，1-基波）
	  Input:		//FLOAT32 *pData：一周波数据开始地址
					//multiplier:比例系数
					//INT32 beginNum：开始计算的谐波次数，即计算0'maxNum次谐波和相角
					//INT32 maxNum：最大计算的谐波次数，即计算0'maxNum次谐波和相角
					//INT32 perCycPoints：一周波点数
	  Output:		//FLOAT32 *pHarmonic：pHarmonic[2n]第（n+beginNum）次谐波有效值；pHarmonic[2n+1]第（n+beginNum）次谐波相位角(0->360度)，n从0到maxNum
	  Return:		//BOOL：计算成功返回True，失败返回False
	  Others:		//pHarmonic需要调用者事先申请空间FLOAT32[2*（maxNum-beginNum）+2]
	******************************************************************************
	   }}}USR */
        BOOL SHGetHarmonic( /*INT16 **/unsigned short * pData, FLOAT32 multiplier, INT32 perCycPoints, INT32 beginNum, INT32 maxNum, FLOAT32 * pHarmonic );

	/* {{{USR
	******************************************************************************
	  Function:		//BOOL SHGetHarmonic( INT16 * pData, FLOAT32 multiplier, INT32 perCycPoints, INT32 beginNum, INT32 maxNum, FLOAT32 * pHarmonic )
	  Description:	//谐波计算，计算0-到maxNum次谐波。（0-直流量，1-基波）
	  Input:		//FLOAT32 *pData：一周波数据开始地址
					//INT32 beginNum：开始计算的谐波次数，即计算0'maxNum次谐波和相角
					//INT32 maxNum：最大计算的谐波次数，即计算0'maxNum次谐波和相角
					//INT32 perCycPoints：一周波点数
	  Output:		//FLOAT32 *pHarmonic：pHarmonic[2n]第（n+beginNum）次谐波有效值；pHarmonic[2n+1]第（n+beginNum）次谐波相位角(0->360度)，n从0到maxNum
	  Return:		//BOOL：计算成功返回True，失败返回False
	  Others:		//pHarmonic需要调用者事先申请空间FLOAT32[2*（maxNum-beginNum）+2]
	******************************************************************************
	   }}}USR */
	BOOL SHGetHarmonic( FLOAT32 * pData, INT32 perCycPoints, INT32 beginNum, INT32 maxNum, FLOAT32 * pHarmonic );

	/* {{{USR
	******************************************************************************
	/* {{{USR
	******************************************************************************
	  Function:		//BOOL SHGetPower( FLOAT32 * pDataU, FLOAT32 * pDataI, INT32 perCycPoints, FLOAT32 * pPower )
	  Description:	//计算有功、无功、视在、功率因素、功角
	  Input:		//FLOAT32 *pDataU, *pDataI：一周波电压电流采样值地址
					//INT32 perCycPoints：每周波采样点数
					//FLOAT32 Channel_Skew: 同步采样时间差（电压-电流）
	  Output:		//FLOAT32 *pPower：依次存放功率（power[0]有功、power[1]无功、power[2]视在功率、power[3]功率因素、power[4]以度表示的功角）
	  Return:		//BOOL：计算成功返回True，失败返回False
	  Others:		//pPower需要调用者事先申请空间FLOAT32[5]
	******************************************************************************
	   }}}USR */
	BOOL SHGetPower( FLOAT32 * pDataU, FLOAT32 * pDataI, FLOAT32 Channel_Skew, INT32 perCycPoints, FLOAT32 * pPower );
	/* {{{USR
	******************************************************************************
	  Function:		//BOOL SHGetPower(  INT16 * pDataU, INT16 * pDataI, FLOAT32 Channel_Skew, INT32 perCycPoints, FLOAT32 * pPower  )
	  Description:	//计算有功、无功、视在、功率因素、功角
	  Input:		//INT16 *pDataU, *pDataI：一周波电压电流采样值地址
					//INT32 perCycPoints：每周波采样点数
					//FLOAT32 Channel_Skew: 同步采样时间差（电压-电流）
	  Output:		//FLOAT32 *pPower：依次存放功率（power[0]有功、power[1]无功、power[2]视在功率、power[3]功率因素、power[4]以度表示的功角）
	  Return:		//BOOL：计算成功返回True，失败返回False
	  Others:		//pPower需要调用者事先申请空间FLOAT32[5]
	******************************************************************************
	   }}}USR */
	BOOL SHGetPower( INT16 * pDataU, INT16 * pDataI, FLOAT32 Channel_Skew, INT32 perCycPoints, FLOAT32 * pPower );
	/* {{{USR
	******************************************************************************
	  Function:     //FLOAT32 SHGetFrequency( FLOAT32 * pData, INT32 smpRate )
	  Description:  //计算频率
	  Input:        //pData采样数据、smpRate采样频率
	  Output:       //无
	  Return:       //输出 frequency，测量频率结果
	  Others:		//pData采样数据必须给足3个周波  
	******************************************************************************
	   }}}USR */
	FLOAT32 SHGetFrequency( FLOAT32 * pData, INT32 smpRate );
	/* {{{USR
	******************************************************************************
	  Function:     //FLOAT32 SHGetFrequency( INT16 * pData, INT32 smpRate )
	  Description:  //计算频率
	  Input:        //pData采样数据、smpRate采样频率
	  Output:       //无
	  Return:       //输出 frequency，测量频率结果
	  Others:		//pData采样数据必须给足3个周波  
	******************************************************************************
	   }}}USR */
	FLOAT32 SHGetFrequency( INT16 * pData, INT32 smpRate );
	/* {{{USR
	******************************************************************************
	  Function:     //FLOAT32 SHGetRmsAmp( INT16 * pData, FLOAT32 multiplier, FLOAT32 offset, INT32 perCycPoints )
	  Description:  //计算有效值
	  Input:        //pDATA采样数据、multiplier比例系数，offset Comtrade格式零漂，perCycPoints每周波点数
	  Output:       //无 
	  Return:       //有效值
	  Others:		//pData 1个周期采样数据
	******************************************************************************
	   }}}USR */
        FLOAT32 SHGetRmsAmp(INT16 * pData, FLOAT32 multiplier, FLOAT32 offset, INT32 perCycPoints );
	/* {{{USR
	******************************************************************************
	  Function:     //FLOAT32 SHGetRmsAmp( FLOAT32 * pData, INT32 perCycPoints, int nType )
	  Description:  //计算有效值
	  Input:        //pDATA采样数据, perCycPoints每周波点数;
					//int nType:通道类型
					//1表示交流电压, 2 表示交流电流, 3表示高频, 4 表示直流，5 其它类型
	  Output:       //无 
	  Return:       //有效值
	  Others:		//pData 1个周期采样数据，交流0.07 直流0.2以下返回(直流和基波分量)
	******************************************************************************
	   }}}USR */
	FLOAT32 SHGetRmsAmp( FLOAT32 * pData, INT32 perCycPoints, INT32 nChType );
	/* {{{USR
	******************************************************************************
	  Function:     //void SHGetAnotherTime( YHFTmStruct srcTime, FLOAT64 relativeTime, YHFTmStruct & destTime )
	  Description:  //计算绝对时间
	  Input:        //srcTime: 开始时间, relativeTime:相对时间(单位:ms)
	  Output:       //destTime: 绝对时间(srcTime与relativeTime之和) 
	  Return:       //无
	  Others:		//无
	******************************************************************************
	   }}}USR */
	void SHGetAnotherTime( YHFTmStruct srcTime, FLOAT64 relativeTime, YHFTmStruct & destTime );
	/* {{{USR
	******************************************************************************
	  Function:     //FLOAT32 SHGetMeanValue( INT16 * pData, FLOAT32 multiplier, FLOAT32 offset, INT32 perCycPoints )
	  Description:  //计算平均值
	  Input:        //pDATA采样数据、multiplier比例系数，offset Comtrade格式零漂，perCycPoints每周波点数
	  Output:       //无 
	  Return:       //平均值
	  Others:		//pData 1个周期采样数据
	******************************************************************************
	   }}}USR */
        FLOAT32 SHGetMeanValue( /*INT16 **/unsigned short * pData, FLOAT32 multiplier, FLOAT32 offset, INT32 perCycPoints );
	/* {{{USR
	******************************************************************************
	  Function:     //FLOAT32 SHGetMeanValue( FLOAT32 * pData, INT32 perCycPoints )
	  Description:  //计算平均值
	  Input:        //pDATA采样数据、perCycPoints每周波点数
	  Output:       //无 
	  Return:       //平均值
	  Others:		//pData 1个周期采样数据
	******************************************************************************
	   }}}USR */
	FLOAT32 SHGetMeanValue( FLOAT32 * pData, INT32 perCycPoints );
	/******************************************************************************
	Function:		//FLOAT32 SHGetDecayTimeConstant ( FLOAT32 * pData, INT32 length, INT32 smpRate)
	Description:	//求非周期分量衰减时间常数
	Input:			//FLOAT32 * pData: 通道数据开始地址
					//INT32 length: 数据点长度
					//INT32 smpRate: 采样频率
	Output:			//无
	Return:			//FLOAT32: 非周期分量衰减时间常数 (-1为无效)
	Others:			//数据长度为3.5 - 15.5周波，数据应该为非周期衰减分量数据，无效返回-1
	******************************************************************************/
	FLOAT32 SHGetDecayTimeConstant ( FLOAT32 * pData, INT32 length, INT32 smpRate);
};

#endif /* BasicAlgorithm_H */
