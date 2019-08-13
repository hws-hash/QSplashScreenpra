/*! \file
    \brief      定义SHDFR配置/定值结构
    \author     Chentm
    \version    2.0
    \date       2011-05-08
	<pre>
	
    本文件中定义了SHDFR电力故障记录装置配置、定值相关结构信息；

    以下为本文件中各变量/属性缩写的说明：
    An      Analogue                模拟量
    Bin     Binary                  开关量(与IEC 61850保持一致)
    Bra     Branch
    Cfg     Config
    Cnt     Count
    Ch      Channels
    ECur    Electric Current
    Hz      Frequency
    Info    Information
    Hi      High
    Mi      Middle
    Lo      Low
    Mrk     Mark
    Trg     Trigger
    Num     Number
    Parm    Parameter
    Vol     Voltage
    Gn      Generator
    Flo     Fault Location
    Lev     Level
    Ena     Enable
    Stn     Station
    SetVal  SettingValue
    Trn     Transformer
    DFR     Digit Fault Recorder
    Rtg     Rating
    Idx     Index
    Glb     Global
    Cont    Continuous
    Smp     Sample
    Dur     Duration
    Resv    Reserve
    Bus     Bus bar         母线    
    Trns    Transient       暂态 
	Cle     clearance       间隙
	Pri     Primary         主要的，一次的
	Scou    self-coupling   自耦
    Comwinding        common winding    公共绕组
    
    定值整体结构：
    +-------------------+---------------+-------+-----------+-------+-----------------------------------+
    |       数据        |  类型         |大小   |数量       |LN     |说明                               |
    +-------------------+---------------+-------+-----------+-------------------------------------------+
    |YH_DFR_PARM        |YH_DFR_PARM    |       |           |       |录波器信息                         |
    | DfrCfg            |YH_DFR_CFG     |       | 1         |       |录波器配置信息                     |
    | DfrTrnsSet        |YH_TRNS_RCD_SET|       | 1         |RDRE_T |暂态RDRE定值                       |
    | DfrContSet        |YH_CONT_RCD_SET|       | 1         |RDRE_C |稳态RDRE定值                       |
    | HzTrnsSet         |YH_HZ_SET      |       | 1         |RFDR   |暂态频率定值                       |
    | HzContSet         |YH_HZ_SET      |       | 1         |RFDR   |稳态频率定值                       |
    +-------------------+---------------+-------+-----------+-------------------------------------------+
    |YH_ANCH_PARM       |YH_ANCH_PARM   |       | AnChCnt   |       |模拟量通道参数                     |
    | AnCfg             |YH_ANCH_CFG    |       | 1         |       |模拟通道基本配置参数(公用信息)     |
    | AnTrnsSet         |YH_ANCH_SET    |       | 1         |RADR   |模拟通道暂态定值                   |
    | AnContSet         |YH_ANCH_SET    |       | 1         |RADR   |模拟通道稳态定值                   |
    | AnPQSet      	    |YH_PQ_ANCH_SET |       | 1         |       |模拟通道电能质量定值               |    
    +-------------------+---------------+-------+-----------+-------------------------------------------+
    |YH_BINCH_PARM      |YH_BINCH_PARM  |       | BinChCnt  |RBDR   |状态量通道参数                     |
    | BinCfg            |YH_BINCH_CFG   |       | 1         |RBDR   |状态量通道参数                     |
    | BinTrnsSet        |YH_BINCH_SET   |       | 1         |RBDR   |状态量通道参数                     |
    | BinContSet        |YH_BINCH_SET   |       | 1         |RBDR   |状态量通道参数                     |
    +-------------------+---------------+-------+-----------+-------------------------------------------+
    |YH_ILIN_PARM       |YH_ILIN_PARM   |       | ILinCnt   |       |电流(线路)参数                     |
    | LinCfg            |YH_ILIN_CFG    |       | 1         |       |电流基本配置参数(公用信息)         |
    | FloSet            |YH_ILIN_FLO_SET|       | 1         |RFLO   |故障测距定值  ******               |
    | LinTrnsSet        |YH_LIN_SET     |       | 1         |RDIS   |电流触发录波定值  *****            |
    | LinContSet        |YH_LIN_SET     |       | 1         |RDIS   |电流标记事件定值   ******          |
    | LinPQSet	        |YH_PQ_LIN_SET  |       |   1       |RDIA   |电能质量定值                       |
    +-------------------+---------------+-------+-----------+-------------------------------------------+
    |YH_ULIN_PARM       |YH_ULIN_PARM   |       | ULinCnt   |       |电压参数                           |
    | LinCfg            |YH_ULIN_CFG    |       | 1         |       |电压基本配置参数(公用信息)         |
    | LinTrnsSet        |YH_LIN_SET     |       | 1         |RDUS   |电压触发录波定值                   |
    | LinContSet        |YH_LIN_SET     |       | 1         |RDUS   |电压标记事件定值                   |
    | LinPQSet	        |YH_PQ_LIN_SET  |       | 1         |RDUA   |电能质量定值                       |
    +-------------------+---------------+-------+-----------+-------------------------------------------+
    |YH_TRN_PARM        |YH_TRN_PARM    |       | TrnCnt    |       |变压器信息                         |
    | TrnCfg            |YH_TRN_CFG     |       | 1         |       |变压器基本配置参数(公用信息)       |
    | TrnTrnsSet        |YH_TRN_SET     |       | 1         |       |变压器触发录波定值                 |
    | TrnContSet        |YH_TRN_SET     |       | 1         |       |变压器标记事件定值                 |
    +-------------------+---------------+-------+-----------+-------------------------------------------+
    |YH_GN_PARM         |YH_GN_PARM     |       | GnCnt     |       |发电机信息                         |
    | GnCfg             |YH_GN_CFG      |       | 1         |       |发电机基本配置参数(公用信息)       |
    | GnTrnsSet         |YH_GN_SET      |       | 1         |       |发电机触发录波定值                 |
    | GnContSet         |YH_GN_SET      |       | 1         |       |发电机标记事件定值                 |
    +-------------------+---------------+-------+-----------+-------------------------------------------+   
    |CheckSum           |INT32U         |       | 1         |       |校验和                             |
    +-------------------+---------------+-------+-----------+-------------------------------------------+
    
    其它说明：
    1.暂态、稳态定值项数据结构完全相同，但意义有所不同，
      对于暂态而言，以Ena结束的属性项标识对应的触发录波是否启用；
      对于稳态而言，以Ena结束的属性项标识是否记录相应的标识事件；

    2.定值结构大小与配置相关。
    
    注意：本文定义的结构中已经为未来的扩充预留了相应的空间，在未来扩充属性时，直接利用这个预留空间。
          则可以保证定值数据向前兼容。在扩展属性时，要特别注意保证字节边界对齐。
    
    结构体空间占用为：
	sizeof( YH_DFR_CFG )           = 600
	sizeof( YH_TRNS_RCD_SET )      = 60
	sizeof( YH_CONT_RCD_SET )      = 40
	sizeof( YH_HZ_SET )            = 40
	sizeof( YH_ANCH_CFG )          = 120
	sizeof( YH_ANCH_SET )          = 80
	sizeof( YH_PQ_ANCH_SET )       = 160
	sizeof( YH_BINCH_CFG )         = 100
	sizeof( YH_BINCH_SET )         = 20
	sizeof( YH_ULIN_CFG )          = 160
	sizeof( YH_ILIN_CFG )          = 180
	sizeof( YH_LIN_SET )           = 80
	sizeof( YH_ILIN_FLO_SET )      = 80
	sizeof( YH_TRN_CFG )           = 240
	sizeof( YH_TRN_SET )           = 40
	sizeof( YH_GN_CFG )            = 100
	sizeof( YH_GN_SET )            = 40
	sizeof( YH_DFR_PARM )          = 780
	sizeof( YH_ANCH_PARM )         = 440
	sizeof( YH_BINCH_PARM )        = 140
	sizeof( YH_ULIN_PARM )         = 320
	sizeof( YH_ILIN_PARM )         = 420
	sizeof( YH_TRN_PARM )          = 320
	sizeof( YH_GN_PARM )           = 180
    </pre>
*/

#ifndef __YH_DFR_PARM_DEFINE_H
#define __YH_DFR_PARM_DEFINE_H

#include "yhftypes.h"

#define MAX_TRN_BRA 3

#ifdef __cplusplus
extern "C" {
#endif

/* 本文件中定义的所有结构体均按1字节对齐 */
#pragma pack (1)

typedef struct runtime_type RUNTIME_TYPE;

typedef struct
{
    char    ip[16];         /*!< IP地址 */
    char    netmask[16];    /*!< 子网掩码   */
}NET_CFG_INFO;

/*!
    \brief  录波器配置信息
 */
typedef struct  yh_dfr_cfg
{
    INT32U          Ver;            /*!< 版本号 */
    INT32U          StnID;          /*!< 变电站号 */
    INT32U          DFRID;          /*!< 录波器号 */
    char            StnName[64];    /*!< 变电站名 */
    char            DFRName[64];    /*!< 录波器名 */
    
    INT32           AnChCnt;        /*!< 模拟量通道数 */
    INT32           BinChCnt;       /*!< 状态量通道数 */
    INT32           ULinCnt;        /*!< 电压量数 */
    INT32           ILinCnt;        /*!< 线路数 */
    INT32           FLinCnt;        /*!< Feeder 线路数  */
    INT32           TrnCnt;         /*!< 变压器数 */
    INT32           GnCnt;          /*!< 发电机数 */

	INT32			AnBoardCnt;		/*!< 传统采样，模拟量板卡数 */
	INT32			BinBoardCnt;	/*!< 传统采样，开关量板卡数 */

    INT32           Freq;           /*!< 工作频率，50或60Hz */

    NET_CFG_INFO    Mon1NetCfg[4];  /*!< 监控板1设备Ip地址 */
    NET_CFG_INFO    Mon2NetCfg[4];  /*!< 监控板2设备Ip地址 */

	/*
		1<<0 :启动暂态业务
		1<<1 :启动稳态业务
		1<<2 :启动电能质量业务
		1<<3 :启动PMU业务
		为0：表示此监控板不启用
		通过或运算可以启动多个业务
	*/

	INT32			DevBusiness;	/*!< 整个设备包含的业务（为界面开发方便而定义） */
	INT32			MonBusiness[2];	/*!< 监控板启动的业务功能 */

	INT8			MonitorNo;		/*!< 本监控板编号，=1 或 =2 */

    INT8            TrnsEna;        /*!< 是否启用暂态功能 */
    INT8            ContEna;        /*!< 是否启用稳态功能 */
    INT8            PQEna;			/*!< 是否启用电能质量功能 */
    
    INT8            DirMod;         /*!< 列目录方式;0=IEC 61850标准目录,1=广东标准,2=浙江标准,3=其它 */
    INT8            FileNaming;     /*!< 文件命名方式:0=COMTRADE　标准，1＝广东标准，2＝浙江标准，3＝其它 */
    INT8            NasEna;         /*!< 是否启用网络存储器,稳态有效 */

	INT8			SntpEna;		/*!< 是否启用SNTP对时 */
	INT8			Protocol103Ena;	/*!< 是否启用103通讯 */

    INT8            Resv1[3];      	/*!< 预留，保证字节对齐 */

	INT32			SmpCntPerCyc;	/*!< 系统每周波采样点数(原始采样率) */
	INT32			RtCycCnt;		/*!< 实时波形显示时的周波数 */

	INT8            Resv2[120];      /*!< 预留，结构大小为 600 字节 */

} YH_DFR_CFG;


/*!
    \brief  录波器触发录波定值数据
    
    本结构中数据属性与RDRE_Trns逻辑节点存在对应的关系；
 */
typedef struct  yh_trns_rcd_set
{
    INT32       A2NumCyc;       /*!< A2 段的周波数 0 */
    INT32       A1NumCyc;       /*!< A1 段的周波数 5 */
    INT32       BNumCyc;        /*!< B  段的周波数 15 */
    INT32       CNumCyc;        /*!< C  段的周波数 100 */
    INT32       DNumCyc;        /*!< D  段的周波数 250 */
    
    INT32       A2SmpRte;       /*!< A2 段记录频率 50 */
    INT32       A1SmpRte;       /*!< A1 段记录频率 100000 */
    INT32       BSmpRte;        /*!< B  段记录频率 100000 */
    INT32       CSmpRte;        /*!< C  段记录频率 50 */
    INT32       DSmpRte;        /*!< D  段记录频率 10 */
    INT32       MaxNumRcd;      /*!< 最大记录文件个数 1000 */
    INT8        Resv[16];       /*!< 预留，保证整个结构体大小为 60 字节 */
} YH_TRNS_RCD_SET;


/*!
    \brief  录波器标记事件定值数据
    
    本结构中数据属性与RDRE_Cont逻辑节点存在对应的关系；
 */
typedef struct  yh_cont_rcd_set
{
    INT32U      SmpRte;         /*!< 连续记录采样率 1000 */
    INT32U      RcdDur;         /*!< 连续记录小时数 (小时) */
    INT32U      MaxNumRcd;      /*!< 最大记录文件个数 */
    INT32       RcdFileDur;     /*!< 每个文件的记录时长 120s */
    INT8        Resv[24];       /*!< 预留，保证整个结构体大小为 40 字节 */
} YH_CONT_RCD_SET;


/*!
    \brief  频率定值
    
    本结构中数据属性与RFDR逻辑节点存在对应的关系；
    所有与频率相关的定值项存储在本结构中，暂态/稳态各存在一个实例。
 */
typedef struct  yh_hz_set
{
    FLOAT32     HzDfTrgLev;     /*!< 频率变化率启动值 */
    FLOAT32     HzHiTrgLev;     /*!< 频率上限值 */
    FLOAT32     HzLoTrgLev;     /*!< 频率下限值 */
	FLOAT32     HzDfTrgRtn;		/*!< 频率变化率返回值 */
	FLOAT32     HzHiTrgRtn;		/*!< 频率上限返回值 */
	FLOAT32     HzLoTrgRtn;		/*!< 频率下限返回值 */
 // FLOAT32     ResvVal[3];     /*!< 预留 */
    
    INT8        HzDfTrgEna;     /*!< 频率变化率启动使能 */
    INT8        HzHiTrgEna;     /*!< 频率上限启动使能 */
    INT8        HzLoTrgEna;     /*!< 频率下限启动使能 */
    INT8        ResvEna[13];    /*!< 预留, 结构体大小为 40 字节 */
} YH_HZ_SET;


#define		ANCH_TYPE_UNKNOWN	0	/*!< 模拟量通道类型 UnKnown */
#define		ANCH_TYPE_TV		1	/*!< 模拟量通道类型 交流电压 */
#define		ANCH_TYPE_TA		2	/*!< 模拟量通道类型 交流电流 */
#define		ANCH_TYPE_DV		3	/*!< 模拟量通道类型 直流电压 */
#define		ANCH_TYPE_DA		4	/*!< 模拟量通道类型 直流电流 */
#define		ANCH_TYPE_HF		5	/*!< 模拟量通道类型 高频 */
#define		ANCH_TYPE_FQ		6	/*!< 模拟量通道类型 频率 */

#define		PHS_TYPE_NULL		0	/*!< 相别 UnKnown */
#define		PHS_TYPE_UNKNOWN	0	/*!< 相别 UnKnown */
#define		PHS_TYPE_A			1	/*!< 相别 A相 */
#define		PHS_TYPE_B			2	/*!< 相别 B相 */
#define		PHS_TYPE_C			3	/*!< 相别 C相 */
#define		PHS_TYPE_N			4	/*!< 相别 N相 */

/*! \struct YH_ANCH_CFG
    \brief  模拟量通道参数
 */
typedef struct  yh_anch_cfg
{
    char        Name[64];       /*!< 通道名称 */
    INT32       Index;          /*!< 通道索引号，对应于物理通道号(与ChnNum含义不同，一般情况下应将其设为相等的值) */
    INT32       ChNum;          /*!< 通道编号，对应于COMTRADE文件中的通道号 */
       
    INT8        ChTyp;          /*!< 模拟量通道的类型，0=UnKnown 1=交流电压(TV)；2=交流电流(TA)；3=直流电压(DV)；4=直流电流(DA); 5=高频(HF); 6=频率(FQ) */
    INT8        Unit;           /*!< 单位,值与IEC 61850中SIUnitEnum枚举值保持一致 */
    INT8        UnitMultiplier; /*!< 单位倍数,值与IEC 61850中multiplierEnum枚举值保持一致 */
    INT8        BindType;       /*!< 绑定类型,0--无绑定,1--电压,2--电流 */
    
    INT8		Resv1[2];		/*!< 预留，保证字节对齐 */
    INT16       BindNum;        /*!< 绑定序号,标识本通道绑定哪一个电压量或电流量*/
    
    FLOAT32     Freq;           /*!< 该通道的额定频率，仅对交流通道有效 */
	FLOAT32     ACSecRtg;       /*!< 该通道的二次额定值，仅对交流通道有效 */

    /* 2011.05 将以下属性项从定值项中移到了配置项中，因为无论对于暂态、稳态、电能质量的应用，这些参数都是一致的 */
    FLOAT32     Offset;     	/*!< 偏移量 */
    FLOAT32     Scaler;     	/*!< 比例系数 */
    FLOAT32     Skew;       	/*!< 通道角差 */
    FLOAT32     Ratio;      	/*!< 变比 */
    
    INT8U       SmpTyp;         /*!< 采集类型，0 -- 传统采样类型 1 -- 60044-8采样 2 -- IEC61850-9-1 3 -- IEC61850-9-2LE 4 -- IEC61850-9-2   */
    INT8U       DsIdx;          /*!< 该通道所对应的数据集的索引号,对于数字采样有意义   */
    INT16U      FcdaIdx;        /*!< 该通道所对应的数据集内的FCDA的索引号   */
    
    INT8        Resv[12];       /*!< 预留，用于将来可能的扩展应用，这样整个结构体大小为 120 字节 */
} YH_ANCH_CFG;


/*!
    \brief  模拟量通道定值
    
    本结构中数据属性与RADR逻辑节点存在对应的关系；
    所有与模拟量通道相关的定值项存储在本结构中，暂态/稳态每个模拟量通道存在一个实例。
 */
typedef struct  yh_anch_set
{
    INT32       Index;		/*!< 顺序索引号，从1开始的连续正整数 */
    FLOAT32     HiTrgLev;   /*!< 上限启动定值 */
    FLOAT32     LoTrgLev;   /*!< 下限启动定值 */
    FLOAT32     SaTrgLev;   /*!< 突变启动定值，注：Sa为salient的缩写(突出的) */
    FLOAT32     Ha2TrgLev;  /*!< 2次谐波启动定值 */
    FLOAT32     Ha3TrgLev;  /*!< 3次谐波启动定值 */
    FLOAT32     Ha5TrgLev;  /*!< 5次谐波启动定值 */
    FLOAT32     Ha7TrgLev;  /*!< 7次谐波启动定值 */
    FLOAT32     Ha9TrgLev;  /*!< 9次谐波启动定值 */

    FLOAT32     ResvLev[7]; /*!< 预留，用于将来可能的扩展应用 */
    
    INT8        HiTrgEna;   /*!< 上限启动使能 */
    INT8        LoTrgEna;   /*!< 下限启动使能 */
    INT8        SaTrgEna;   /*!< 突变启动使能 */
    INT8        Ha2TrgEna;  /*!< 2次谐波启动使能 */
    INT8        Ha3TrgEna;  /*!< 3次谐波启动使能 */
    INT8        Ha5TrgEna;  /*!< 5次谐波启动使能 */
    INT8        Ha7TrgEna;  /*!< 7次谐波启动使能 */
    INT8        Ha9TrgEna;  /*!< 9次谐波启动使能 */
    
    INT8        ResvEna[8]; /*!< 预留，用于将来可能的扩展应用，并保证整个结构体占用 80 字节存储空间 */
} YH_ANCH_SET;

/*!
 	\brief	电能质量模拟量通道定值
 	
 */
typedef struct  yh_PQ_anch_set
{
	INT32		Index;			/*!< 顺序索引号，从1开始的连续正整数 */
	FLOAT32		HiTrgLev;		/*!< 电压上限定值 */
	FLOAT32		LoTrgLev;		/*!< 电压下限定值 */
	FLOAT32		VFTrgLev;		/*!< 电压波动上限定值 VF=Voltage Fluctuation */
	FLOAT32		SSeverityTrgLev;/*!< 短时闪变限定值 */
	FLOAT32		LSeverityTrgLev;/*!< 长时闪变限定值 */
	FLOAT32		THDHiTrgLev;	/*!< 总畸变率限定值（百分比） */
	FLOAT32		OddTrgLev;		/*!< 奇次谐波限定值（电压通道，百分比） */
	FLOAT32		EvenTrgLev;		/*!< 偶次谐波限定值（电压通道，百分比） */
	FLOAT32		HaTrgLev[24];	/*!< 2~25次谐波限值(仅电流通道) */
	
	/*暂态指标*/
	FLOAT32		VTranRisingTrgLev;	/*!< 暂升，(百分比，电压有效) */
	FLOAT32		VTranfallingTrgLev;	/*!< 暂降，(百分比，电压有效) */

	INT8		VTranRisingTrgEna;	/*!< 上限启动使能 */
	INT8		VTranfallingTrgEna;	/*!< 下限启动使能 */
	INT8		VIntTrgEna;	/*!< 电压中断启动使能 */

	INT8		ResvEna[1];	/*!< 预留，用于将来可能的扩展应用，并保证整个结构体占用  字节存储空间 */
} YH_PQ_ANCH_SET;


#define		BINCH_LEVEL_OTHERS			0	/*!< 开关量通道分类 其它开关量 */
#define		BINCH_LEVEL_RELAY_ACT		1	/*!< 开关量通道分类 保护动作 */
#define		BINCH_LEVEL_BREAKER_POS		2	/*!< 开关量通道分类 开关位置 */
#define		BINCH_LEVEL_SWITCH_POS		3	/*!< 开关量通道分类 刀闸位置 */
#define		BINCH_LEVEL_DEVICE_ALARM	4	/*!< 开关量通道分类 装置告警 */

/*!
    \brief  开关量通道配置项
    
    <pre>
	ChLevel: 开关量信号的分类：
	         1=保护动作；2=开关位置；3=刀闸位置；4=装置故障告警；0=其它类型。

	ChTyp:   开关量信号的类型，定义如下：
	         保护动作（Relay_Act）：
	                 1=跳A；2=跳B；3=跳C；4=三跳；5=重合闸；6=单跳；7=跳零；
                     8=永跳；9=相间跳；10=加速跳；11=接地跳；0=其它。
	         开关位置（Breaker_Pos）：
	                 1=A合；2=A跳；3=B合；4=B跳；5=C合；6=C跳；0=其它。
	         装置告警（Device_Alarm）：
	                 1=TA断线；2=TV断线；3=通道故障；0=其它。
	         其它开关量：
	                 1=发信；2=收信；0=其它。
    
    对于同时存在传统开关量采样，GOOSE开关量采样的应用来说。
    约定如下：传统开关量采样排列在前。
    
    </pre>
 */
typedef struct  yh_binch_cfg
{
    char        Name[64];       /*!< 通道名称 */
    INT32       Index;          /*!< 通道索引号，对应于物理通道号 */
    INT32       ChNum;          /*!< 通道编号，对应于COMTRADE文件中的通道号 */

    INT8        BindType;       /*!< 绑定类型,0--无绑定,1--电压,2--电流 */
    INT8        ChLevel;        /*!< 开关量信号的分类 */    
	INT8        ChTyp;          /*!< 开关量的类型 */    
	INT8		Resv1[3];		/*!< 预留，保证字节边界对齐 */  

	INT16       BindNum;        /*!< 绑定序号,标识本通道绑定哪一个电压量或电流量，参见BindType属性*/

    /*!
    开关组编号
    开关或者保护/后备保护基于1的编号， 0为不确定
    当type为Jump_A、Jump_B、Jump_C、Jump_ABC时对应的开关编号，如m为1、2分别对应开关1、开关2；
    当type为Jump_ ACT、Send、Recv、Jump_BK_ACT时分别对应的保护/后备保护编号，
    如m为1、2分别对应保护1动作、保护2动作或者后备保护1动作、后备保护2动作（Jump_ BK_ACT）。
    */
    INT32       GrID;
    
    INT8U       SmpTyp;         /*!< 采集类型，0 -- 传统采样类型 1 -- GOOSE采样 */
    INT8U       DsIdx;          /*!< 该通道所对应的数据集的索引号,当SmpTyp为GOOSE时才有意义   */
    INT16U      FcdaIdx;        /*!< 该通道所对应的数据集内的FCDA的索引号   */
    
    INT8        Resv[12];        /*!< 预留，用于将来可能的扩展应用,整个结构体大小为  100 字节 */
}YH_BINCH_CFG;


/*!
    \brief  开关量通道定值项    
 */
typedef struct  yh_binch_set
{
    INT32       Index;			/*!< 开关量顺序索引号，从1开始的连续正整数 */
    
    INT8        StVal;          /*!< 标识该通道是常开还是常闭 0--常开 1--常闭 */
    INT8        TrgEna;         /*!< 状态量触发使能 */
    
    INT8        Resv[14];       /*!< 预留，用于将来可能的扩展应用,整个结构体大小为 20 字节 */
} YH_BINCH_SET;


/*!
    \brief  电压量配置参数  
 */
typedef struct  yh_ulin_cfg
{
    char        Name[64];       /*!< 电压名称 */
    INT32       Index;          /*!< 顺序索引号，从1开始的连续正整数 */
    
    INT8        VolLev;         /*!< 电压等级 -1 = unknown */
    INT8        PTType;         /*!< PT类型 0--母线PT 1--线路PT(电力系统中存在母线PT,不过一般不接入到录波器) */
    INT8        AssNum;         /*!< 母线PT：对应的母线号(母线号相同的两个PT可认为互为备用的关系) 线路PT：对应的线路号 */
    INT8        PhTyp;          /*!< 接线方式,4-ABCN,3-ABC,2-AC,1-单通道 0 未接 */

    INT16       ChNum[4];        /*!< 对应的通道号,=0不存在 */
    INT16       ChHfNum[2];      /*!< 高频通道对应的通道号,=0不存在Hf */

    INT16       ExtrAnCnt;      /*!< 与该线路相关的其它模拟量的通道的个数 */
    INT16       AnNumArr[8];    /*!< 与该线路相关的其它模拟量的通道号 */
    
    INT16       ExtrBinCnt;     /*!< 与该线路相关的其它状态量的通道的个数 */
    INT16       BinNumArr[16];  /*!< 与该线路相关的其它状态量的通道号 */

	INT8        FileRcdEna;     /*!< 标识是否生成该线路的分通道文件 */
	INT8		PQEnable;		/*!< 是否启用电能质量功能：1=启用;0=不启用 */
    
    INT8        LinIdx2KD;      /*!< 2KD线路索引号（为与2KD定值互相转换而添加） */
    INT8        Resv[21];        /*!< 预留，用于将来可能的扩展应用,整个结构体大小为  160 字节 */
} YH_ULIN_CFG;

#define		LINE_TYPE_NORMAL			0	/*!< 普通单回线路 */
#define		LINE_TYPE_PARA				1	/*!< 普通双回线路 */
#define		LINE_TYPE_BYPASS			2	/*!< 旁路 */
#define		LINE_TYPE_BUS_CONNECTION	3	/*!< 母联 */
#define		LINE_TYPE_BUS_RAILWAY		4	/*!< 电铁 */
#define		LINE_TYPE_OTHERS			5	/*!< 其他 */

#define		PHASE_TYPE_ABCN				4	/*!< ABCN接线 */
#define		PHASE_TYPE_ABC				3	/*!< ABC接线 */
#define		PHASE_TYPE_AC				2	/*!< AC接线 */
#define		PHASE_TYPE_SINGLE			1	/*!< 单通道接线 */
#define		PHASE_TYPE_UNKNOWN			0	/*!< 未知接线 */

/*!
    \brief  线路(电流量)配置参数    
 */
typedef struct  yh_ilin_cfg
{
    char        Name[64];       /*!< 线路名称 */
    INT32       Index;          /*!< 线路顺序索引号，从1开始的连续正整数*/
    
    INT8U       LinTyp;         /*!< 标识该线路类型，0--单回线，1--双回线，2--旁路（旁路开关的电流），3--母联，4--电铁线，5--其他*/
    INT8U       PhTyp;          /*!< 接线方式,4-ABCN,3-ABC,2-AC,1-单通道*/
    INT8U		Resv1[2];		/*!< 预留1，保证字节边界对齐 */
    
    INT16       ULinNum;        /*!< 线路对应的电压索引号 */
    INT16       ParaNum;        /*!< 对应的双回线路号(仅对双回线路有意义) */
    
    INT16       ChNum[4];		/*!< 对应的通道号,=0不存在 */
    
    INT16       ChHfNum[2];    /*!< 高频通道对应的通道号,=0不存在Hf */
   
    INT16       ExtrAnCnt;      /*!< 与该线路相关的其它模拟量的通道的个数 */
    INT16       AnNumArr[8];    /*!< 与该线路相关的其它模拟量的通道号 */
    
    INT16       ExtrBinCnt;     /*!< 与该线路相关的其它状态量的通道的个数 */
    INT16       BinNumArr[36];  /*!< 与该线路相关的其它状态量的通道号 */

    INT8        CurDir;         /*!< 电流方向 */
    INT8        FltLocEna;      /*!< 标识是否启用故障测距 */
    INT8        FileRcdEna;     /*!< 标识是否生成该线路的分通道文件 */
    INT8		PQEnable;		/*!< 是否启用电能质量功能：1=启用;0=不启用。*/
    
    INT8        LinIdx2KD;      /*!< 2KD线路索引号（为与2KD定值互相转换而添加） */
    
    INT8        ULinNumBak;     /*!< 备用电压索引号 */
    
    INT8        Resv[2];        /*!< 预留，用于将来可能的扩展应用,整个结构体大小为  180 字节 */        
} YH_ILIN_CFG;


/*!
    \brief  三相电气量定值数据
    
    用于描述电力系统中三相电气量相关的定值数据;
    这里没有区分电流/电压、暂态/稳态定值,所有定值项均包含在其中；
    部分属性项仅对电压有意义，部分属性项仅对电流有意义,由具体应用来决定；
 */
typedef struct  yh_lin_set
{
    INT32       Index;			/*!< 顺序索引号，从1开始的连续正整数 */
    FLOAT32     PSHiTrgLev;     /*!< 正序上限定值 */
    FLOAT32     PSLoTrgLev;     /*!< 正序下限定值 */
    FLOAT32     NSHiTrgLev;     /*!< 负序上限定值 */
    FLOAT32     ZSHiTrgLev;     /*!< 零序上限定值 */
    FLOAT32     ZSSaTrgLev;     /*!< 零序突变定值 */
    FLOAT32     SwgTrgLev;      /*!< 振荡定值 */

    FLOAT32     PHiTrgLev;      /*!< 有功功率上限定值(稳态、电流有效) */
    FLOAT32     PRteTrgLev;     /*!< 有功变化率定值(稳态、电流有效) */
    FLOAT32     QHiTrgLev;      /*!< 无功功率上限定值(稳态、电流有效) */
    FLOAT32     QRteTrgLev;     /*!< 无功变化率定值(稳态、电流有效) */

    FLOAT32     ResvLev[5];     /*!< 预留，用于将来可能的扩展应用 */

    INT8        PSHiTrgEna;     /*!< 正序上限使能 */
    INT8        PSLoTrgEna;     /*!< 正序下限使能 */
    INT8        NSHiTrgEna;     /*!< 负序上限使能 */
    INT8        ZSHiTrgEna;     /*!< 零序上限使能 */
    INT8        ZSSaTrgEna;     /*!< 零序突变使能 */
    
    INT8        SwgTrgEna;      /*!< 振荡启动使能 */
    
    INT8        PHiTrgEna;      /*!< 有功上限使能 */
    INT8        PRteTrgEna;     /*!< 有功变化率使能 */
    INT8        QHiTrgEna;      /*!< 无功上限使能 */
    INT8        QRteTrgEna;     /*!< 无功变化率使能 */
    INT8        ResvEna[6];     /*!< 预留(保证整个结构体大小为 80 bytes)，用于将来可能的扩展应用 */
} YH_LIN_SET;


/*!	\struct	SHPQLinSet
 	\brief	三相电气量电能质量定值数据
 	
 	
 	用于描述电力系统中三相电气量相关的电能质量定值数据;
 	这里没有区分电流/电压、所有定值项均包含在其中；
 	部分属性项仅对电压有意义，部分属性项仅对电流有意义,由具体应用来决定；
 */
typedef struct  yh_PQ_lin_set
{
    INT32	Index;
    FLOAT32	FreqHiTrgLev;		/*!< 频率上限定值(电压有效) */
    FLOAT32	FreqLoTrgLev;		/*!< 频率下限定值(电压有效) */	
    FLOAT32	UBTrgLev;		/*!< 三相不平衡度的限值 */
    INT8	ResvEna[4];		/*!< 预留(保证整个结构体大小为    bytes)，用于将来可能的扩展应用 */
} YH_PQ_LIN_SET;

/*!
    \brief  故障测距定值
    
    注：测距参数中的电阻/电抗、电导/电纳值为实测值
    他们之间不存在电路意义上互为倒数的关系
 */
typedef struct  yh_ilin_flo_set
{
    INT32       Index;		/*!< 顺序索引号，从1开始的连续正整数 */
    FLOAT32     R0;         /*!< 线路零序电阻 */  
    FLOAT32     X0;         /*!< 线路零序电抗 */
    FLOAT32     R1;     	/*!< 线路正序电阻 */
    FLOAT32     X1;         /*!< 线路正序电抗 */
    FLOAT32     G0;			/*!< 零序电导 */
    FLOAT32     B0;			/*!< 零序电纳 */
    FLOAT32     G1; 		/*!< 正序电导 */
    FLOAT32     B1;         /*!< 正序电纳 */    
    FLOAT32     NR0;        /*!< 对端零序电阻 */
    FLOAT32     NX0;        /*!< 对端零序电抗 */
    FLOAT32     NR1;    	/*!< 对端正序电阻 */
    FLOAT32     NX1;        /*!< 对端正序电抗 */
    FLOAT32     MR; 		/*!< 双回线路零序互感电阻，仅对双回线路有意义 */
	FLOAT32     MX; 		/*!< 双回线路零序互感电抗，仅对双回线路有意义 */
    FLOAT32     LinLenKm;   /*!< 线路长度，单位：km */

    FLOAT32		Reactor;	/*!< 并联电抗器补偿电抗 */  
    INT8        Resv[12];   /*!< 预留(保证整个结构体大小为 80 bytes)，用于将来可能的扩展应用 */  
} YH_ILIN_FLO_SET;


/*!
    \brief  变压器绕组信息
*/
typedef struct 
{
	INT8		Coiling;	/*!< 绕组接线方式，  0 -- Y星型接线 1 -- D三角形接线 */
	INT8		PhsHour;	/*!< 绕组相角偏移(0--12) */
	INT8		Ground;		/*!< 中性点接地方式，0 -- 不接地    1 -- 直接接地 2 -- 间隙接地 */
	INT8		Resv;		/*!< 预留 */
} YH_TRN_COIL ;

/*!
    \brief  变压器配置信息
    
    关于变压器分支：<br>
    在SH2000D及以前的录波器产品中均未曾考虑过变压器可能"T"接的应用场合；
    实际应用中，变压器是可能"T"接的，此时该侧电流应该是各个分支电流之和。
    (此种接线方式不常见，据了解，广州市110KV配网较多的采用了主变"T"接的方式)。
    在《广东电网公司录波装置技术规范》5.1.3.4节中，涉及到了分支的相关内容。
    变压器参数结构参照该部分内容设计。
 */
typedef struct  yh_trn_cfg
{
    char        Name[64];               /*!< 变压器名称 */
    INT32       Index;                  /*!< 变压器顺序索引号，从1开始的连续正整数 */

	FLOAT32		Capacity;				/*!< 变压器额定容量 */

    FLOAT32     HiPriRVol;              /*!< 高压侧一次额定电压（KV）*/
    FLOAT32     MiPriRVol;              /*!< 中压侧一次额定电压（KV）*/ 
    FLOAT32     LoPriRVol;              /*!< 低压侧一次额定电压（KV）*/

    YH_TRN_COIL HiCoiling;              /*!< 高压侧绕组接线方式 */
    YH_TRN_COIL MiCoiling;              /*!< 中压侧绕组接线方式 */
    YH_TRN_COIL LoCoiling;              /*!< 低压侧绕组接线方式 */

	INT8        TrnTyp;         		/*!< 变压器类型，MAIN=主变；PLANT=厂用变；EXCITATION=励磁变 */
	INT8		WindingNum;				/*!< 绕组数, 1=自耦变, 2=两卷变, 3=三卷变 */

    INT8        HiBraCnt;               /*!< 高压侧分支数,取值范围1--MAX_TRN_BRA */
    INT8        MiBraCnt;               /*!< 中压侧分支数,取值范围0--MAX_TRN_BRA(注，仅当WindingNum为3时，才有意义) */
    INT8        LoBraCnt;               /*!< 低压侧分支数,取值范围1--MAX_TRN_BRA */
	INT8		Resv1[3];				/*!< 预留1，保证字节边界对齐 */
    
    INT16       HiBraNum[MAX_TRN_BRA];  /*!< 高压侧分支线路号 */
    INT16       MiBraNum[MAX_TRN_BRA];  /*!< 中压侧分支线路号 */
    INT16       LoBraNum[MAX_TRN_BRA];  /*!< 低压侧分支线路号 */

    INT16       HiVolNum;  				/*!< 高压侧电压顺序号 */
    INT16       MiVolNum;  				/*!< 中压侧电压顺序号 */
    INT16       LoVolNum;  				/*!< 低压侧电压顺序号 */

	INT16		HiZsChNum;				/*!< 高压侧零序电流通道号 */
	INT16		MiZsChNum;				/*!< 中压侧零序电流通道号 */
	INT16		LoZsChNum;				/*!< 低压侧零序电流通道号 */

	INT16		HiCleChNum;				/*!< 高压侧间隙零序电流通道号 */
    INT16		MiCleChNum;				/*!< 中压侧间隙零序电流通道号 */
    INT16		LoCleChNum;				/*!< 低压侧间隙零序电流通道号 */

	INT16		ExtrAnCnt;				/*!< 与主变相关的其它模拟量的通道的个数 */
	INT16		AnNumArr[8];			/*!< 与主变相关的其它模拟量的通道号 */

    INT16       BinChCnt;               /*!< 与主变相关开关量的个数 */
    INT16       BinNumArr[16];          /*!< 与主变相关开关量的通道号,最多允许16个 */

    INT16       ScouComwinding;         /*!< 自耦变公共绕组中性点线路号 */
    
    INT8        Resv[46];               /*!< 预留，保证整个结构体大小为 240 字节 */
} YH_TRN_CFG;

/*!
    \brief  变压器定值数据
 */
typedef struct  yh_trn_set
{
    INT32       Index;				/*!< 变压器顺序索引号，从1开始的连续正整数 */
    FLOAT32     OvExTrgLev;     	/*!< 过激磁定值(励磁涌流相对于额定电流的倍数) */
    FLOAT32     ThrITrgLev;     	/*!< 穿越电流定值(穿越电流相对于额定电流的倍数，对于暂态而言无意义) */
    FLOAT32     DifATrgLev;     	/*!< 差流定值 */
    FLOAT32     ResvLev[4];     	/*!< 预留 */
    
    INT8        OvExTrgEna;     	/*!< 过激磁启动使能 */
    INT8        ThrITrgEna;     	/*!< 穿越电流启动使能(对于暂态而言无意义) */
    INT8        DifATrgEna;     	/*!< 差流启动使能 */
    INT8        ResvEna[5];     	/*!< 预留，整个结构体占用  40 字节空间 */
} YH_TRN_SET;

/************************************2011.3.10修改***************************************/
/*
	\brief  发电机差流信息
	\date   2011.3.10
*/
typedef struct
{
	INT16		DifChNumG;			/*!< 机端侧电流通道号，0为不存在 */
	INT16		DifChNumN1;			/*!< 中性点侧电流通道号1，0为不存在 */	
	INT16		DifChNumN2;			/*!< 中性点侧电流通道号2，0为不存在 */
	INT16		inDifChNum;			/*!< 不完全纵差类型， 0：不完全纵差不存在； 1：中性点侧对应通道1； 2：中性点侧对应通道2 */
	FLOAT32		DifKtr;				/*!< 横差比例系数 */
	FLOAT32		DifKin;				/*!< 不完全纵差比例系数 */
}YH_GN_DIF;


/*!
    \brief  发电机配置信息
 */
typedef struct
{
	char        Name[64];           /*!< 发电机名称 */
	INT32       Index;              /*!< 发电机顺序索引号，从1开始的连续正整数 */

	INT16       VolNum;             /*!< 发电机机端电压顺序号 */
	INT16		CurNum;				/*!< 发电机机端电流顺序号 */
	INT16		NAIChNum;			/*!< 发电机中性点电流通道号 */
	INT16		NAUChNum;			/*!< 发电机中性点电压通道号 */
	INT16       UePosChNum;         /*!< 发电机励磁电压Ue+通道号 */
	INT16       UeNegChNum;         /*!< 发电机励磁电压Ue-通道号 */
	INT16       ExitIChNum;         /*!< 发电机励磁电流通道号 */
	INT16		ZeroSeqVNum;		/*!< 发电机纵向零序电压通道号 */
	
	YH_GN_DIF	DifCfg[3];			/*!< 发电机差流信息，分别为A相差流信息，B相差流信息， C相差流信息 */
	
	INT8        GnIDir;             /*!< 发电机机端电流方向，从发电机机端流出为1，流入-1*/
	INT8        NIDir;              /*!< 发电机中性点侧电流方向，从中性点侧流出为1，流入为-1*/

	INT8		Resv[32];			/*!< 预留， 整个结构体占用160字节空间 */
}YH_GN_CFG;


/*!
    \brief  发电机定值信息
 */
typedef struct  yh_gn_set
{
	 INT32      Index;				/*!< 发电机顺序索引号，从1开始的连续正整数 */
	
	 FLOAT32	DifTrgLev;			/*!< 纵差定值，分别对应A相完全纵差定值、B相完全纵差定值、C相完全纵差定值*/
	 FLOAT32	DifInTrgLev;		/*!< 不完全纵差定值，分别对应A相不完全纵差定值、B相不完全纵差定值、C相不完全纵差定值*/
	 FLOAT32	DifTranTrgLev;		/*!< 横差定值，分别对应A相横差定值、B相横差定值、C相横差定值*/
	 FLOAT32	OvExTrgLev;     	/*!< 过激磁定值 */
	 FLOAT32	LossExcTrgLev;		/*!< 失磁启动定值 */
	 FLOAT32	StatorZeroTrgLev;	/*!< 定子接地之零序电压定值 */
	 FLOAT32	StatorHar3TrgLev;	/*!< 定子接地之三次谐波电压比定值 */
	 FLOAT32	ZeroSeqTrgLev;		/*!< 纵向零序电压式定子绕组匝间短路定值*/

	 FLOAT32	RotorHar2TrgLev;	/*!< 转子二次谐波定值*/
	 FLOAT32	ReversePoTrgLev;	/*!< 逆功率定值*/
	 FLOAT32	LowFreOvCurTrgLev;	/*!< 低频过流定值 */

	 FLOAT32	ResvLev[4];			/*!< 预留 */

	 INT8		DifTrgEna;			/*!< 完全纵差启动使能 */
	 INT8		DifInTrgEna;		/*!< 不完全纵差启动使能 */
	 INT8		DifTranTrgEna;		/*!< 横差启动使能 */
	 INT8		OvExTrgEna;			/*!< 过激磁启动使能 */
	 INT8		LossExcTrgEna;		/*!< 失磁启动使能 */
	 INT8		StatorEarthTrgEna;	/*!< 定子绕组单相接地启动使能 */
	 INT8		ZeroSeqTrgEna;		/*!< 纵向零序电压式定子绕组匝间短路启动使能 */
	 INT8		RotorHar2TrgEna;	/*!< 转子二次谐波启动使能 */

	 INT8		ReversePoTrgEna;	/*!< 逆功率使能 */
	 INT8		LowFreOvCurTrgEna;	/*!< 低频过流使能 */

	 INT8		ResvEna[6];			/*!< 预留，整个结构体占用80字节 */
}YH_GN_SET;


/*! \struct SHDFRParm
    \brief  DFR全局性参数
 */
typedef struct  yh_dfr_parm
{
     YH_DFR_CFG         DfrCfg;         /*!< 录波器全局性配置信息 */
     YH_TRNS_RCD_SET    DfrTrnsSet;     /*!< 暂态RDRE定值 */
     YH_HZ_SET          HzTrnsSet;      /*!< 暂态频率定值 */
     YH_CONT_RCD_SET    DfrContSet;     /*!< 稳态RDRE定值 */
     YH_HZ_SET          HzContSet;      /*!< 稳态频率定值 */
} YH_DFR_PARM;


/*!
    \brief  模拟量通道参数
 */
typedef struct  yh_anch_parm
{
     YH_ANCH_CFG        AnCfg;      /*!< 模拟通道基本配置参数(公用信息) */
     YH_ANCH_SET        AnTrnsSet;  /*!< 暂态定值 */
     YH_ANCH_SET        AnContSet;  /*!< 稳态定值 */
     YH_PQ_ANCH_SET 	AnPQSet;	/*!< 电能质量定值 */
} YH_ANCH_PARM;

/*!
    \brief  开关量通道参数
 */
typedef struct  yh_binch_parm
{
     YH_BINCH_CFG       BinCfg;     /*!< 开关量通道基本配置参数(公用信息)   */
     YH_BINCH_SET       BinTrnsSet; /*!< 暂态定值 */
     YH_BINCH_SET       BinContSet; /*!< 稳态定值 */
} YH_BINCH_PARM;

/*!
    \brief  电压量参数
 */
typedef struct  yh_ulin_parm
{
     YH_ULIN_CFG        LinCfg;         /*!< 电压基本配置参数(公用信息) */
     YH_LIN_SET         LinTrnsSet;     /*!< 线路暂态启动定值 */
     YH_LIN_SET         LinContSet;     /*!< 线路稳态标记事件定值 */
     YH_PQ_LIN_SET  	LinPQSet;		/*!< 电能质量定值 */
} YH_ULIN_PARM;

/*!
    \brief  线路参数
 */
typedef struct  yh_ilin_parm
{
     YH_ILIN_CFG        LinCfg;         /*!< 线路基本配置参数(公用信息，其中包含了分通道文件信息) */
     YH_ILIN_FLO_SET    FloSet;         /*!< 测距定值参数 */
     YH_LIN_SET         LinTrnsSet;     /*!< 线路暂态启动定值 */
     YH_LIN_SET         LinContSet;     /*!< 线路稳态标记事件定值 */
     YH_PQ_LIN_SET  	LinPQSet;		/*!< 电能质量定值 */
} YH_ILIN_PARM;

/*!
    \brief  变压器参数
 */
typedef struct  yh_trn_parm
{
     YH_TRN_CFG     TrnCfg;         /*!< 变压器基本配置参数(公用信息) */
     YH_TRN_SET     TrnTrnsSet;     /*!< 变压器暂态启动定值 */
     YH_TRN_SET     TrnContSet;     /*!< 变压器稳态标记事件定值 */
} YH_TRN_PARM;


/*!
    \brief  发电机参数
 */
typedef struct yh_gn_parm 
{
    YH_GN_CFG       GnCfg;              /*!< 发电机基本配置参数(公用信息) */
    YH_GN_SET       GnTrnsSet;          /*!< 发电机暂态启动定值 */
    YH_GN_SET       GnContSet;          /*!< 发电机暂态启动定值 */
}YH_GN_PARM;


typedef struct
{
	INT8U				dstMac[6];		/*!< 目的Mac地址 */
	INT8U				srcMac[6];		/*!< 源Mac地址 */
	INT16U				AppID;			/*!< AppID */
	INT8U               Resv[2];        /*!< 预留 */
}SG_LPDU_CFG;

/*!
    \brief  定值结构信息
 */
typedef struct yh_sg_info
{
    YH_DFR_PARM*        pDFRParm;       /*!< 全局参数 */
    YH_ANCH_PARM*       pAnChParm;      /*!< 模拟量通道信息 */
    YH_BINCH_PARM*      pBinChParm;     /*!< 状态量通道信息 */
    YH_ULIN_PARM*       pULinParm;      /*!< 电压量信息 */
    YH_ILIN_PARM*       pILinParm;      /*!< 线路信息 */
    YH_TRN_PARM*        pTrnParm;       /*!< 变压器信息 */
    YH_GN_PARM*         pGnParm;        /*!< 发电机信息 */
    
    INT32U*             DataAddr;       /*!< 数据区起始地址 */
    INT32U              BufLen;         /*!< 数据区长度 */
} SHDFR_SG;


#pragma pack () /* 恢复字节对齐设定*/
#ifdef __cplusplus
}
#endif

#endif  /*__YH_DFR_PARM_DEFINE_H*/

