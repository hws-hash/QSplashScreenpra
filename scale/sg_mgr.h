/*************************************************************************
* Copyright (c) 2008,深圳市元恒丰有限公司
* All rights reserved.

**************************************************************************/

/*!	\file		
 	\brief		定值文件读/写接口
	\author		Chentm
	\version	2.0
	\date		2011-05-04
*/

#ifndef DFR_SG_MGR_INCLUDED_
#define DFR_SG_MGR_INCLUDED_

#include <stdio.h>
#include "sg_def.h"

/*!< 定值文件版本号 */
#define	SG_FILE_VERSION		0x0100

/*!
 	\brief	录波器定值组文件头结构
 */
struct DfrSGFileHead
{
	char			ShFlg[4];		/*!< 自定义文件格式标志,固定为"yhf" */
	char			ShExt[4];		/*!< 文件扩展名固定为"sg" */
	INT8			FileType;		/*!< 文件类型 */
	INT8			Endian;			/*!< 文件字节顺序 */
	INT8			Resv1[2];		/*!< 预留，保证4字节对齐 */
	INT32			FileVer;		/*!< 文件版本 */
	INT32			FileLen;		/*!< 文件长度,=0时表示长度未知 */
	INT32			LenOfSG;		/*!< 定值数据的长度 */
	unsigned char	Resv[16];		/*!< 预留18字节,保证文件头总的大小为40字节 */
};

/*!
 	\brief	定值管理器
	
	在设备运行过程中，当定值/配置信息更新时，
	各个运行中的模块均需要重新进行配置或初始化。
	定值管理器用于协同各个模块动态的更新配置信息。
 */
class SGManager
{
public:

/*!
	\brief	构造函数
*/
	SGManager();
	
/*!
	\brief	析构函数
*/
	~SGManager();

/*!
	\brief	初始化定值结构信息
	\param	dfr_sg		[in,out]	定值组文件
	\return	成功返回0,否则返回错误号
*/
	int initDfrSgData( SHDFR_SG* pSg );

/*!
	\brief	读取DFR定值数据
	\param	sg_fname	[in]	定值组文件
	\param	dfr_sg		[out]	存放定值组数据的结构
	\return	成功返回0,否则返回错误号	
*/
	int readDfrSGData( const char* sg_fname, SHDFR_SG* dfr_sg, DfrSGFileHead* sg_file_head );

/*!
	\brief	读取DFR FileHead
	\param	sg_fname	[in]	定值组文件
	\param	psg_file_head	[out]	存放定值文件头
	\return	成功返回0,否则返回错误号	
*/
	int readSGFileHead( const char* sg_fname, DfrSGFileHead* psg_file_head );

/*!
	\brief	写定值文件
	\param	sg_fname	[in]	定值组文件
	\param	dfr_sg		[out]	存放定值组数据的结构
	\return	成功返回0,否则返回错误号
	\sa		readDfrSGData()
	
	如果该定值文件存在，则改写定值文件的数据；
	如果该定值文件不存在，则创建该定值文件；
*/	
	int writeDfrSGData( const char* sg_fname, const SHDFR_SG* dfr_sg );

/*!
	\brief	写定值头
	\param	sg_fname	    [in]	定值组文件
	\param	sg_file_head	[out]	存放定值头数据的结构
	\return	成功返回0,否则返回错误号
	
	如果该定值文件存在，则改写定值文件的数据；
	如果该定值文件不存在，则创建该定值文件；
*/	
	int writeSGFileHead ( const char* sg_fname, const DfrSGFileHead* sg_file_head );

/*!
	\brief	创建一个定值文件
	\param	sg_fname	[in]	定值组文件
	\param	dfr_sg		[out]	存放定值组数据的结构
	\return	成功返回0,否则返回错误号
	\sa		writeDfrSGData()
	
	如果该定值文件存在，则会覆盖该文件；
	如果该定值文件不存在，则创建该定值文件；
*/	
	int createDfrSGFile( const char* sg_fname, const SHDFR_SG* dfr_sg );

/*!
	\brief	 拷贝定值部分数据
	\param	 nUnum	[in]		电压线路
	\param	 nInum	[in,out]	电流线路
	\return	 成功返回0,否则返回错误号
	注意：拷贝过程中为目的定值数据分配了同源定值数据同样大小的内存空间，需要由调用者自己去释放。
*/	
	SHDFR_SG* dfr_sg_pt_cpy(SHDFR_SG* sg_dst, int nUnum, int nInum, const SHDFR_SG* sg_src);

/*!
	\brief	创建定值数据
	\param	anChCnt		[in]	模拟量通道数
	\param	binChCnt	[in]	开关量通道数
	\param	ilinCnt		[in]	电流量数
	\param	ulinCnt		[in]	电压量数
	\param	trnCnt		[in]	变压器数
	\param	gnCnt		[in]	发电机数
	\return	返回创建的定值数据的指针，如果失败则返回NULL
	
	本函数只是为定值数据分配了合适的空间，所有定值数据值均未初始化。
*/	
	SHDFR_SG* createDfrSGData( int anChCnt, int binChCnt, 
						   int ilinCnt, int ulinCnt, 
						   int trnCnt,  int gnCnt );

/*!
	\brief	创建定值数据(从定值文件中读取)
	\param	sg_fname	[in]	定值组文件
	\param	sg_fname	[out]	填充 m_psg
	\return	如果失败则返回false
	\sa		createDfrSGData(),destroyDfrSGData()
*/	
	bool createDfrSGFromFile( const char* sg_fname );

/*!
	\brief	克隆一份定值数据
	\param	sg_src		[in]	定值数据源
	\return	返回创建的定值数据的指针，如果失败则返回NULL
	\sa		createDfrSGData(),destroyDfrSGData()
*/
	SHDFR_SG* cloneDfrSGData( const SHDFR_SG* sg_src );

/*!
	\brief	释放定值数据内存空间
	\param	dfr_sg		[in]	定值数据
	\sa		createDfrSGData(),createDfrSGFromFile()
	
	本函数用于释放通过createDfrSGData(),createDfrSGFromFile(),cloneDfrSGData()生成的定值数据内存空间。
*/	
	void destroyDfrSGData( SHDFR_SG* &dfr_sg );

/*!
	\brief	计算定值数据校验和
*/
	INT32U calc_sg_chksum( SHDFR_SG* psg );

/*!
	\brief	创建默认定值
*/	
	SHDFR_SG* createDefaultDfrSG();

	SHDFR_SG * m_psg; /*!< 定值数据 */ 
};

/*!
	\brief	初始化定值结构信息
	\param	dfr_sg		[in,out]	定值组文件
	\param	sg_data		[in]		定值数据区
	\param	sg_size		[in]		定值数据区长度
	\param	chkSum		[in]		是否重新计算校验和
	\return	成功返回0,否则返回错误号
*/
	int initDfrSg( SHDFR_SG* dfr_sg, void* sg_data, int sg_size, int chkSum );

/*!
	\brief	比较两个定值结构配置信息是否相同
	\param	pdfr_sg1	[in]	定值结构1
	\param	pdfr_sg2	[in]	定值结构2
	\return	相同返回0,否则非0值
	
	只有当通道数、线路数、等所有基本配置信息完全相同时，才判断为相同。
*/	
	int sg_comp_dfrcfg( const SHDFR_SG* pdfr_sg1, const SHDFR_SG* pdfr_sg2 );

/*!
	\brief	拷贝定值数据
	\param	sg_src	[in]		定值数据源
	\param	sg_dst	[in,out]	目的定值数据
	\param  flg		[in]		定值配置不匹配时的处理方式
	\return	成功返回0,否则返回错误号
	
	当flg为0时,若定值不匹配，则返回错误;
	当flg非0时,若定值不匹配，则会为sg_dst重新分配了内存空间;
	注意：拷贝过程中为目的定值数据分配了同源定值数据同样大小的内存空间，需要由调用者自己去释放。
*/	
	int dfr_sg_cpy( SHDFR_SG* sg_dst, const SHDFR_SG* sg_src, int flg );

/*!
	\brief	得到模拟量通道的相位
	\param	psg			定值数据
	\param	nChannelID	通道号
*/	
	const char* getAnChPhase( const SHDFR_SG* psg, int nChannelID);

/*!
	\brief	得到单位字符串
	\param	nMultiplier	倍数
	\param	nUnitType	单位
	
	定值结构中，单位的表示是根据IEC 61850中单位表示方式一致的。
	本函数实现将其转换为常规的单位字符串。
*/
	const char* getUnitStr(int nMultiplier, int nUnitType, char* buf );

/*!
	\brief	比较两个定值各参数的值
	\param	pdfr_sg1	[in]	定值结构1
	\param	pdfr_sg2	[in]	定值结构2
	\return	相同返回0,否则-1值
	
	只有当通道数、线路数、等所有值完全相同时，才判断为相同。
*/	
	int sg_comp_dfr_val( const SHDFR_SG* pdfr_sg1, const SHDFR_SG* pdfr_sg2 );

#endif	/* DFR_SG_MGR_INCLUDED */

