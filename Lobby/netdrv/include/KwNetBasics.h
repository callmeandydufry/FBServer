/*$T Common/Basics.h GC 1.140 08/23/09 03:08:17 */


/*$6
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/


#ifndef __NETBASICS_H__
#define __NETBASICS_H__

#ifndef GAME_CLIENT
#ifndef FD_SETSIZE
#define FD_SETSIZE	2048
#endif /* FD_SETSIZE */
#endif

#if defined(WIN32)
#   if !defined(__WINDOWS__)
#       define __WINDOWS__
#   endif
#endif

#if defined(__WINDOWS__)
#pragma warning(disable : 4786)
#pragma warning(disable : 4018)
#define NOMINMAX
#include <WinSock2.h>
#include <Windows.h>
#include "crtdbg.h"
#elif defined(__LINUX__)
#include <sys/types.h>
#include <pthread.h>
#include <string.h>
#endif
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <math.h>
#include <stdarg.h>
#include <memory.h>
#include <algorithm>

using namespace			std;

typedef unsigned char	uchar;
typedef char			int8;
typedef uchar			uint8;
typedef unsigned short	uint16;
typedef short			int16;
typedef unsigned int	uint32;
typedef int				int32;
typedef double			float64;
typedef std::string		String;

#if defined(__WINDOWS__)
typedef unsigned long			TID;
typedef unsigned __int64	uint64;
typedef __int64			int64;
#elif defined(__LINUX__)
typedef pthread_t				TID;
typedef unsigned long long	uint64;
typedef long long int		int64;
#endif

typedef unsigned int		IP_t;
typedef uint16			MSG_ID;
typedef int32			BOOL;

#if defined(_70_CLIENT)
typedef double			FLOAT64;
#endif

#define IP_SIZE		24
#define PORT_SIZE	10
#define IP_PORT_SIZE 36
#define MAC_ADDRESS_SIZE 32
#define INVALID_KHAN_HANDLE	- 1
#define INVALID_KHAN_ID	- 1
// Fixme:为了兼容书剑代码
#define INVALID_ID 	- 1
#define INVALID -1

#ifndef TRUE
#define TRUE	1
#endif

#ifndef FALSE
#define FALSE	0
#endif

#ifndef _MAX_PATH
#define _MAX_PATH	260
#endif

#if defined(NDEBUG)
#define GUARD	if(1) \
{
#define UNGUARD \
}
#else
#define GUARD	if(1) \
{
#define UNGUARD \
}
#endif

#if defined(NDEBUG)
#define _NET_TRY	try
#define _NET_CATCH	catch(...)
#else
#define _NET_TRY	try
#define _NET_CATCH	catch(...)
#endif

#ifndef KSafeDelete
#if defined(__WINDOWS__)
#define KSafeDelete(x)							\
				if((x) != NULL)							\
				{										\
					/*KCheck(_CrtIsValidHeapPointer(x));*/	\
					delete(x);							\
					(x) = NULL;							\
				}
#elif defined(__LINUX__)
#define KSafeDelete(x)							\
			if((x) != NULL)								\
			{											\
				delete(x);								\
				(x) = NULL;								\
			}
#endif
#endif

#ifndef KSafeDelete_ARRAY
#if defined(__WINDOWS__)
#define KSafeDelete_ARRAY(x)					\
		if((x) != NULL)							\
		{										\
		/*KCheck(_CrtIsValidHeapPointer(x));*/	\
			delete[](x);						\
			(x) = NULL;							\
		}
#elif defined(__LINUX__)
#define KSafeDelete_ARRAY(x)					\
		if((x) != NULL)							\
		{										\
			delete[](x);						\
			(x) = NULL;							\
		}
#endif
#endif

#ifndef KSafeFree
#define KSafeFree(x)				\
		if((x) != NULL)				\
		{							\
			free(x);				\
			(x) = NULL;				\
		}
#endif

#ifndef KSafeRelease
#define KSafeRelease(x)				\
		if((x) != NULL)				\
		{							\
			(x)->Release();			\
			(x) = NULL;				\
		}
#endif
#endif
