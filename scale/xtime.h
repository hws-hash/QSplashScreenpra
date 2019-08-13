/*************************************************************************
* Copyright (c) 2008,深圳市YHF电气技术有限公司
* All rights reserved.

**************************************************************************/

/*!	\file
 	\brief	实现与OS无关的系统时间函数
	\author	chen
	\version	1.0
	\date	2008-04-23
	
	在进行跨平台的系统编程时，会遇到时间结构、时间函数不一致的情况；
	本文件中针对不同的操作系统平台，实现相同的时间结构、时间函数，以实现跨平台的高精度时间目的。
	实现方法：
	1.在非Windows平台下，定义了SYSTEMTIME时间结构，该结构与Windows操作系统下的SYSTEMTIME结构完全一致；
	2.在非Windows平台下，定义为SetLocalTime和GetLocalTime函数；
	3.在Windows/VxWorks平台下，定义了gettimeofday和settimeofday函数，其接口与Linux系统下的gettimeofday/settimeofday完全一致;
	4.定义了timeval结构与SYSTEMTIME结构之间进行相互转换的函数；
*/

#ifndef	__XTIME_H__
#define	__XTIME_H__

#include "yhftypes.h"

#if defined(WIN32)
#include <time.h>
//#include <winsock2.h>
#include <windows.h>
#endif


#if defined(linux)
#include <time.h>
#include <sys/time.h>
#endif

#if !defined( ACSI_DLL )
#define ACSI_DLL
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(WIN32)
/*!
*	\brief 关于时区信息的结构定义
*/
//struct timezone
//{
//    int  tz_minuteswest; /*!< minutes W of Greenwich */
//    int  tz_dsttime;     /*!< type of dst correction */
//};
#endif


#if defined(WIN32)
//ACSI_DLL struct tm* gmtime_r (const time_t *timer, struct tm *tp);
ACSI_DLL void  FILETIME_2_timeval ( FILETIME*  pft, struct timeval* ptv );
ACSI_DLL int   gettimeofday(struct timeval *tv, struct timezone *tz);
ACSI_DLL int   settimeofday(const struct timeval *tv, const struct timezone *tz);
#elif defined(VXWORKS)
ACSI_DLL int   gettimeofday(struct timeval *tv, struct timezone *tz);
ACSI_DLL int   settimeofday(const struct timeval *tv, const struct timezone *tz);
#endif	//VXWORKS

/*!
* 	\brief 定义所在的时区
*/
#ifndef __TIMEZONE__
	#define	__TIMEZONE__			8
#endif

/*!
*	\brief 定义time_t数据类型表示的日期的起始日期 1970/01/01 00:00:00
*
*	自定义的自1970/01/01 00:00:00以来的time_t时间转换所需要的宏
*	详见 \ref systemtime_2_timeval 及\ref ndatetime_2_timeval
 */
#define	TIMET_START_YEAR		1970

/*!
* 	\brief 非闰年的年份中总共的天数
 */
#define	DAYS_IN_YEAR			365


/*!
*	\brief 一天的秒数		= 24 * 3600
*/
#define	SECONDS_IN_DAY			86400

/*!
*	\brief 一小时的秒数	= 60 * 60
*/
#define	SECONDS_IN_HOUR			3600

/*!
*	\brief 一分钟的秒数	= 60
*/
#define	SECONDS_IN_MINUTE		60

/*!	
*	\brief 一小时内的分钟数 
*/
#define	MINUTES_IN_HOUR			60

/*!	
*	\brief 一周中的天数
*/
#define DAYS_IN_WEEK	        7

/*!	
*	\brief 一年中的月数
*/
#define MONTHS_IN_YEAR			12

/*!	
*	\brief 一个月可能的最多天数
*/
#define MAX_DAYS_IN_MONTH       31

/*!	
*	\brief 一年可能的最多天数
*/
#define MAX_DAYS_IN_YEAR		366


/*!
* 	\brief 判断某年是否为闰年的表达式宏定义
*
*	\param year	--[in]	参加闰年判断地年份
 */
#define	EXP_IS_LEAP_YEAR(year)		\
		(((0 == (year) % 400) || (0 == (year) % 4 && 0 != (year) % 100))  \
			? 1 : 0) \


/*!
*	\brief 从公元元年到当年的开始一共经历了多少个闰年
*
*	\param year	--[in]	计算中的起始年份，要求 \c year 大于400，否则计算将不准确。
 */
#define	EXP_HOW_MANY_LEAP_YEAR(year)		\
			(  ((year) - 1) / 4				\
			-  ((year) - 1) / 100			\
			+  ((year) - 1) / 400 + 1 )		\


/*!
*	\brief 在指定的年份之间发生了多少个闰年
*
*	\param start	--[in]	起始年份，要求 \c start 大于400。
*	\param end		--[in]	结束年份，要求 \c end 大于400，在计算时，不包括结束年份 \c end 本身。
 */
#define	EXP_HOW_MANY_LEAP_YEAR_BETWEEN(start, end)		\
		( EXP_HOW_MANY_LEAP_YEAR (end) - \
			EXP_HOW_MANY_LEAP_YEAR (start))

/*!
*	\brief 在指定的年份之间的经历的天数
* 
*	即从start/01/01到end/01/01期间所经历的天数
*
*	\param start	--[in]	起始年份，要求 \c start 大于400。
*	\param end		--[in]	结束年份，要求 \c end 大于400，在计算时，不包括结束年份 \c end 本身。
 */
#define	HOW_MANY_DAYS_BETWEEN(start, end)						\
			((end - start) * DAYS_IN_YEAR						\
				+ EXP_HOW_MANY_LEAP_YEAR_BETWEEN (start, end))

/*!
* 	\brief 自 TIMET_START_YEAR 所定义年份的1月1日起到 \c year 年的1月1日, 经历了多少天，
*			不包括 \c year 年的1月1日这一天。
*	
*	\param year	--[in]	起始年份，要求 \c year 大于400。
 */
#define	HOW_MANY_DAYS_SINCE_TIMET_START_YEAR(year)			\
			HOW_MANY_DAYS_BETWEEN ( TIMET_START_YEAR,		\
									year)

#if !defined( WIN32 )

/*!
*	\brief 在非Windows平台上的定义Windows系统日期时间结构定义
 */
typedef struct _SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;

/*!
 *	\brief 设置系统时间
 *
 *	\param	pstm	--[in]	使用该参数中所包含的时间去设置系统的当前时间
 *
 *	\note
 *		参数pstm中所包含的时间是当地时间, 且没有夏时制。
 *		在调用本函数时, 会遇到权限检查的问题.
 */
ACSI_DLL void SetLocalTime(SYSTEMTIME * pstm);

/*!
 *	\brief 取系统当前时间.
 *
 *	\param	pstm	--[out]	取得的系统时间放置到该参数中.
 *
 *	\note
 *		取得的时间必须是当地时间, 且没有夏时制。
 */
ACSI_DLL void GetLocalTime(SYSTEMTIME * pstm);

#endif // !defined( WIN32 )

/*!
 *	\brief timeval时间转换为SYSTEMTIME类型的时间
 *
 *	\param	ptv 	--[in]	要转换的时间
 *	\param	pstm	--[out]	被转换后的时间
 */
ACSI_DLL void timeval_2_SYSTEMTIME(YHFtimeval * ptv, SYSTEMTIME * pstm);

ACSI_DLL void timeval_2_YHFTmStru(YHFtimeval * ptv, struct YHFTmStruct * pstm);

/*!
 *	\brief timeval时间转换为SYSTEMTIME类型的时间
 *
 *	\param	pstm	--[in]	被转换的时间
 *	\param	ptv 	--[out]	转换后的时间
 */
ACSI_DLL void SYSTEMTIME_2_timeval(SYSTEMTIME * pstm, struct timeval * ptv);
/*!
 *	\brief timeval时间转换为SYSTEMTIME类型的时间
 *
 *	\param	pstm	--[in]	被转换的时间
 *	\param	ptv 	--[out]	转换后的时间
 */

/*!
 *	\brief 取指定时间(SYSTEMTIME)的日期时间字符串函数
 *
 *	\param	pstm		--[in]	SYSTEMTIME类型的时间
 *	\param	pszdt_str	--[out]	返回日期时间字符串
 *
 *  \return
 *		格式化完成后的字符串, 实际上等于pszdt_str
 *
 *	\note
 *		时间字符串的格式为(%04d-%02d-%02d %02d:%02d:%02d.%03d), 因此, 时间字符串最长不
 *		超过32字符(包括null字符), 因此定义MAX_DATATIME_STRING_L=32
 *
 */
ACSI_DLL char* get_SYSTEMTIME_str(SYSTEMTIME * pstm, char * pszdt_str);

/*!
 *	\brief 取指定时间(timeval)的日期时间字符串函数
 *
 *	\param	ptv			--[in]	timeval类型的时间
 *	\param	pszdt_str	--[out]	返回日期时间字符串
 *
 *	\return
 *		格式化完成后的字符串, 实际上等于pszdt_str
 *		
 *	\note
 *		时间字符串的格式为(%04d-%02d-%02d %02d:%02d:%02d.%03d), 因此, 时间字符串最长不
 *		超过32字符(包括null字符), 因此定义 MAX_DATATIME_STRING_L=32
 *
 */
ACSI_DLL char* get_timeval_str(YHFtimeval * ptv, char * pszdt_str);//

/*!
 *	\brief 取指定时间(tm)的日期时间字符串函数
 *
 *	\param	ptm			--[in]	tm类型的时间
 *	\param	pszdt_str	--[out]	返回日期时间字符串
 *
 *	\return
 *		格式化完成后的字符串, 实际上等于pszdt_str
 *		
 *	\note
 *		时间字符串的格式为(%04d-%02d-%02d %02d:%02d:%02d.%03d), 因此, 时间字符串最长不
 *		超过32字符(包括null字符), 因此定义 MAX_DATATIME_STRING_L=32
 *
 */
ACSI_DLL char* get_tm_str(struct tm * ptv, char * pszdt_str);

/*
 *	取当前的日期时间字符串
 *
 *	参数
 *		pszdt_str	--	返回日期时间字符串, 是一个MAX_DATATIME_STRING_L长度的字符串指针
 *
 *	返回值
 *		格式化完成后的字符串, 实际上等于pszdt_str
 *
 *	备注
 *		时间字符串的格式为(%04d-%02d-%02d %02d:%02d:%02d.%03d), 因此, 时间字符串最长不
 *		超过32字符(包括null字符), 因此定义MAX_DATATIME_STRING_L=32
 *
 */
ACSI_DLL char* get_cur_dt_str(char * pszdt_str);

/*!
 *	\brief 时间比较, 并返回其差值的绝对值
 *
 *	\param	ptv1			--[in]	时间1
 *	\param	ptv2			--[in]	时间2
 *	\param	ptv_abs_delta	--[out]	返回差值的绝对值, 如果该参数不为NULL.
 *
 *	\retval	小于0	--	ptv1 小于(早于) ptv2
 *	\retval	等于0	--	ptv1 等于(等于) ptv2
 *	\retval	大于0	--	ptv1 大于(晚于) ptv2
 */
ACSI_DLL int timeval_compare(YHFtimeval * ptv1, YHFtimeval * ptv2, YHFtimeval * ptv_abs_delta);

/*!
 *	\brief 取周中的星期
 *
 *	\param	ptmt	--[in]	time_t格式的日期时间
 *	
 *	\return
 *		星期几	--	0 -- 6; 其中0 -- 星期日, 1 -- 星期一 ... 6 -- 星期六
 */
ACSI_DLL int get_wday(time_t * ptmt);

/*!
*	\brief 取某日期属于当年的那一天
*
*	\param	ptmt	--[in]	time_t格式的赋
*
*	\return
*		返回值范围：[0, 365] \n
*			- 0 代表1月1日
*			- 364 在非闰年代表12月31日，在闰年，代表12月30日。
*			- 365 在闰年代表12月31日，在非闰年，不可能返回该值。
*/
ACSI_DLL int get_yday(time_t * ptmt);


void timeval_add( struct timeval * ptv_dst, struct timeval * ptv_add );
void timeval_sub( struct timeval * ptv_dst, struct timeval * ptv_sub );
void timeval_add_usec( struct timeval * ptv_dst, int u_sec );

#ifdef __cplusplus
}
#endif

#endif	//__XTIME_H__

