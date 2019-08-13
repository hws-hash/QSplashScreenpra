
#if !defined(AFX_CH_AJUST_MGR_H_)
#define AFX_CH_AJUST_MGR_H_

#if _MSC_VER > 1000
#pragma once
#endif

//#include "sg_def.h"
#include "rt_ctl.h"

//最大模拟量数
#define AN_CH_MAX_VIRTUAL 16

//最大开关量数
#define DN_CH_MAX 16

class ChAdjustMgr  
{
public:
    bool	m_bDataValid;				/*!< 数据是否有效 */
	int		m_nAnChCnt;					/*!< 模拟量通道数 */
	int		m_nPtCount;					/*!< 每个通道数据长度 */
	int		m_nPerCycPoint;				/*!< 每周波采样点数 */
	float	m_rateFreq;					/*!< 额定工作频率(50\60 Hz) */
	char	m_AnChName[AN_CH_MAX_VIRTUAL][64];	/*!< 模拟量通道名称 */
	short	*m_pChData[AN_CH_MAX_VIRTUAL];		/*!< 模拟量数据 */


	//通道类型和单位
	char	m_ChTyp[AN_CH_MAX_VIRTUAL];			    /*!< 模拟量通道的类型，0=UnKnown 1=交流电压(TV)；2=交流电流(TA)；3=直流电压(DV)；4=直流电流(DA); 5=高频(HF); 6=频率(FQ) */
	float	m_BackupOffset0[AN_CH_MAX_VIRTUAL];     /*!< 备份原始零漂，没有乘比例系数 */
	float	m_BackupOffset[AN_CH_MAX_VIRTUAL];     	/*!< 备份零漂 */
    float	m_BackupScaler[AN_CH_MAX_VIRTUAL];     	/*!< 备份比例系数 */
    float	m_BackupSkew[AN_CH_MAX_VIRTUAL];       	/*!< 备份通道角差 */

	float	m_Offset0[AN_CH_MAX_VIRTUAL];     	/*!< 原始零漂，没有乘比例系数 */
	float	m_Offset[AN_CH_MAX_VIRTUAL];     	/*!< 零漂 */
    float	m_Scaler[AN_CH_MAX_VIRTUAL];     	/*!< 比例系数 */
    float	m_Skew[AN_CH_MAX_VIRTUAL];       	/*!< 通道角差 */

	float	m_ChInputVal[AN_CH_MAX_VIRTUAL];	/*!< 输入值 */    
    float	m_ChOutputVal[AN_CH_MAX_VIRTUAL];	/*!< 输出值 */

	char	m_ChSelected[AN_CH_MAX_VIRTUAL];	/*!< 标记要调节的通道 */	

	bool m_bDataChanged;				/*!< 数据是否已经更新 */

	char ErrStr[64];					/*!< 出错信息 */

	bool GetRealtimeData(RT_CTL* rtCtl);	//取采集数据

	bool GetSG();							//初始化时候取定值数据

    void Init( );

/*! 
	 *  \fn			void CalcOffset()
	 *  \brief		计算零漂
	 *  \param[in]	pChData:原始采样数据
	 *  \param[in]	pScaler:比例系数
	 *  \param[in]	pChSelected:要调节的通道=1
	 *  \param[in]	nAnChCnt:通道数
	 *  \param[in]	nPtCount:每通道采样数据点数
	 *  \param[in]	nPerCycPoint:每周波采样数据点数
	 *  \param[out]	pOffset:零漂
	 *  \param[out]	pOffset0:原始零漂
	 *  \param[out]	pOutputVal:输出值，用来界面显示零漂有效值
	 *  \return		none
	 *  \note		none
*/
	void CalcOffset(short **pChData, float *pScaler, char *pChSelected, int nAnChCnt, int nPtCount, int nPerCycPoint, float *pOffset, float *pOffset0, float *pOutputVal);

/*! 
	 *  \fn			void CalcOffset()
	 *  \brief		计算比例系数
	 *  \param[in]	pChData:原始采样数据
	 *  \param[in]	pInputVal:实际输入值
	 *  \param[in]	pChSelected:要调节的通道=1
	 *  \param[in]	pOffset0:原始零漂
	 *  \param[in]	nAnChCnt:通道数
	 *  \param[in]	nPtCount:每通道采样数据点数
	 *  \param[in]	nPerCycPoint:每周波采样数据点数
	 *  \param[out]	pScaler:比例系数
	 *  \param[out]	pOffset:零漂
	 *  \param[out]	pOutputVal:输出值，用来界面显示零漂有效值
	 *  \return		none
	 *  \note		A/D < 80 不调节
*/
	void CalcScaler(short **pChData, float *pInputVal, char *pChSelected, float *pOffset0, int nAnChCnt, int nPtCount, int nPerCycPoint, float *pScaler,float *pOffset, float *pOutputVal);		// 计算比例系数

/*! 
	 *  \fn			void CalcSkew()
	 *  \brief		计算采样同步性(角差)
	 *  \param[in]	pChData:原始采样数据
	 *  \param[in]	pChSelected:要调节的通道=1
	 *  \param[in]	nAnChCnt:通道数
	 *  \param[in]	nPtCount:每通道采样数据点数
	 *  \param[in]	nPerCycPoint:每周波采样数据点数
	 *  \param[out]	pSkew:采样同步差
	 *  \param[out]	pOutputVal:输出值，用来界面显示通道角差
	 *  \return		none
	 *  \note		要输入同一相位交流数据
*/
    //void CalcSkew(short **pChData, char *pChSelected, int nAnChCnt, int nPtCount, int nPerCycPoint, float *pSkew, float *pOutputVal);

	ChAdjustMgr();
	~ChAdjustMgr();

private:
	char *m_pDataBuff;				/*!< 采样数据缓冲区 */
};

#endif 
