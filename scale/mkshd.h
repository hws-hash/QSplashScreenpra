/*************************************************************************
* Copyright (c) 2011,深圳市YHF电气技术有限公司
* All rights reserved.

**************************************************************************/

/*!	\file
 	\brief		填充SHD帧、生成SHD文件访问接口
 	
	\author		chen
	\version	1.0
	\date		2011-06-23
*/

#ifndef	__SH_MAKE_SHD_DEFINE_H
#define	__SH_MAKE_SHD_DEFINE_H

#include "shd_ctl.h"

#ifdef __cplusplus
extern "C" {
#endif

	
#define SHD_FILE_VERSION	0x1000


/*
	+------+----------------+-----------+-----------------------------------------------+
	| 序号 | 内容           |  长度		|	说明	        							|
	+------+----------------+-----------+-----------------------------------------------+
	|	1  | 文件头	        |	40Bytes	|												|
	+------+----------------+-----------+-----------------------------------------------+	
	|   2  | 定值数据		|	        |												|
	+------+----------------+-----------+-----------------------------------------------+		
	|   3  | 录波原始数据帧	|	        |												|
	+------+----------------+-----------+-----------------------------------------------+
*/

/*!
 	\brief	录波器定值组文件头结构
	
 */
typedef struct shd_file_head_t
{
	char			ShFlg[4];		/*!< 公司自定义文件格式标志,固定为"YHF" */
	char			ShExt[4];		/*!< 文件扩展名固定为"shd" */
	INT8			SgDataType;		/*!< 定值数据的类型 0 -- SHDFR_SG定值数据 1 -- 序列化的SGDfrObject对象 */
	INT8			Endian;			/*!< 文件字节顺序 */
	INT8			Resv1[2];		/*!< 预留，保证4字节对齐 */
	INT32			FileVer;		/*!< 文件版本 */
	INT32			FileLen;		/*!< 文件长度,=0时表示长度未知 */

	//INT32			SgDataLen;		/*!< 定值数据的长度 */
	INT8			Resv2[20];		/*!< 预留20字节,保证文件头总的大小为40字节 */
} SHD_FILE_HEAD ;


int create_shd_file( MK_SHD_CTL* pShdCtl );
int write_shd_file ( MK_SHD_CTL* pShdCtl, int shd_pos );
int close_shd_file ( MK_SHD_CTL* pShdCtl );

int fill_shd_start( MK_SHD_CTL* pShdCtl );
/*!
 	\brief	生成SHD原始数据帧
	\param	pShdCtl	生成SHD文件的控制信息
 */
int fill_shd_proc( MK_SHD_CTL* pShdCtl );

int fill_shd_stop( MK_SHD_CTL* pShdCtl );

/*!
 	\brief	生成comtrade数据
	\param	pShdCtl	生成SHD文件的控制信息
 */
int fill_ctd_stop( MK_SHD_CTL* pShdCtl );
int fill_ctd_proc( MK_SHD_CTL* pShdCtl );
int write_ctd_file ( MK_SHD_CTL* pShdCtl, int shd_pos );

#ifdef __cplusplus
}
#endif

#endif	//__SH_MAKE_SHD_DEFINE_H
