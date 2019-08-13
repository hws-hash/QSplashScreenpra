/*************************************************************************
* Copyright (c) 2010,深圳市YHF电气技术有限公司
* All rights reserved.

**************************************************************************/

/*!	\file
 	\brief		
 	
	\author		chen
	\version	1.0
	\date		2010-03-18
	
*/

#ifndef	__SH_RECORD_MESSAGE_DEFINE_H
#define	__SH_RECORD_MESSAGE_DEFINE_H

#include "rcd_ctl.h"

#ifdef __cplusplus
extern "C" {
#endif


void send_rcd_msg( FAULT_INFO* flt_info, int status );


#ifdef __cplusplus
}
#endif

#endif	//__SH_RECORD_MESSAGE_DEFINE_H
