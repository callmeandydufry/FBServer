/*$T Share/Debug.h GC 1.140 10/10/07 10:06:26 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#ifndef __DEBUG__H__
#define __DEBUG__H__

#include "KwNetBasics.h"


//////////////////////////////////////////////////////////////////////////
//函数宏
//////////////////////////////////////////////////////////////////////////
#define	IgnoreKCheck()	__assert_state = 1
#define RetryKCheck()	__assert_state = 2
#define KCheckState()	(__assert_state)
#define IgnoreMessage() __message_state = TRUE

//////////////////////////////////////////////////////////////////////////
//全局变量
//////////////////////////////////////////////////////////////////////////
extern int32 __assert_state;
extern int32 __message_state;

//////////////////////////////////////////////////////////////////////////
//功能函数
//////////////////////////////////////////////////////////////////////////
extern void _Check1(const char *file, uint32 line, const char *func, const char *expr);
extern void _Check2(const char *file, uint32 line, const char *func, const char *expr, const char *msg);
extern void _Check3(const char *file, uint32 line, const char *func, const char *expr, const char *msg);
extern void _KMB(const char *msg);
extern BOOL RangeCheck_KCheck(int32 nIndex, int32 nFrom, int32 nTo, char const *szCodeLocation);


//////////////////////////////////////////////////////////////////////////
//接口宏，包括各种平台处理
//////////////////////////////////////////////////////////////////////////
#if defined(NDEBUG)
	#define KCheck(expr)					((void) ((expr) ? 0 : (_Check1(__FILE__, __LINE__, __FUNCTION__, #expr), 0)))
	#define KCheckEx(expr, msg)				((void) ((expr) ? 0 : (_Check2(__FILE__, __LINE__, __FUNCTION__, #expr, msg), 0)))
	#define KCheckSpecial(expr, msg)		((void) ((expr) ? 0 : (_Check3(__FILE__, __LINE__, __FUNCTION__, #expr, msg), 0)))
	#define KDialogBox(msg)					_KMB(msg)
#elif defined(__LINUX__)
	#define KCheck(expr)					{ if(!(expr)) { _Check1(__FILE__, __LINE__, __PRETTY_FUNCTION__, #expr); } }
	#define KCheckEx(expr, msg)				{ if(!(expr)) { _Check2(__FILE__, __LINE__, __PRETTY_FUNCTION__, #expr, msg); } }
	#define KCheckSpecial(expr, msg)		{ if(!(expr)) { _Check3(__FILE__, __LINE__, __PRETTY_FUNCTION__, #expr, msg); } }
	#define KCheckExPass(expr, msg)			{ if(!(expr)) { _Check2(__FILE__, __LINE__, __PRETTY_FUNCTION__, #expr, msg); } }
	#define KDialogBox(msg)					((void) 0)
#elif defined(__WIN_CONSOLE__) || defined(__WIN32__) || defined(__WINDOWS__)
	#define KCheck(expr)					((void) ((expr) ? 0 : (_Check1(__FILE__, __LINE__, __FUNCTION__, #expr), 0)))
	#define KCheckEx(expr, msg)				((void) ((expr) ? 0 : (_Check2(__FILE__, __LINE__, __FUNCTION__, #expr, msg), 0)))
	#define KCheckSpecial(expr, msg)		((void) ((expr) ? 0 : (_Check3(__FILE__, __LINE__, __FUNCTION__, #expr, msg), 0)))
	#define KDialogBox(msg)					_KMB(msg)
#elif defined(__MFC__)
	#define KCheck(expr)					ASSERT(expr)
	#define KCheckEx(expr, msg)				((void) 0)
	#define KCheckSpecial(expr, msg)		((void) 0)
	#define KDialogBox(msg)					((void) 0)
#endif

#endif//__DEBUG__H__
