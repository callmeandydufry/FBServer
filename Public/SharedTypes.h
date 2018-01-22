#ifndef __SHARED_TYPES_H__
#define __SHARED_TYPES_H__

#ifndef __GAMESERVER__
#define NAMESPACE_LOBBY_BEGIN \
namespace LobbyPublic {

#define NAMESPACE_LOBBY_END \
}

#else

#define NAMESPACE_LOBBY_BEGIN
#define NET_IP_SIZE		24
#define NAMESPACE_LOBBY_END

#endif

#ifdef __GAMESERVER__
#include "TypeDefine.h"
#else
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdarg.h>
#include <memory.h>
#include <malloc.h>
#include <string.h>

NAMESPACE_LOBBY_BEGIN

typedef unsigned char	uchar;
typedef char			int8;
typedef uchar			uint8;
typedef unsigned short	uint16;
typedef short			int16;
typedef unsigned int	uint32;
typedef int				int32;
typedef double			float64;

#if defined(WIN32)
typedef unsigned long		TID;
typedef unsigned __int64	uint64;
typedef __int64				int64;
#elif defined(__LINUX__)
typedef pthread_t			TID;
typedef unsigned long long	uint64;
typedef long long int		int64;
#endif

typedef unsigned int		IP_t;
typedef uint16				MSG_ID;
typedef int32				BOOL;

typedef uint64				SNID_t;
#ifndef NULL
#define NULL	0
#endif

#ifndef TRUE
#define TRUE	1
#endif

#ifndef FALSE
#define FALSE	0
#endif

#define MAX_ACCOUNT							(100)
#define MAX_CHARACTER_NAME					(30)
#define DB_MAX_STR_LEN						(128)
#define MAX_TOKEN_LEN						(512)
#define NET_IP_SIZE		24
#define NET_PORT_SIZE	10
#define NET_MAC_ADDRESS_SIZE 32

enum EHAND_SHAKE_RESULT_CODE
{
	EHAND_SHAKE_SUCCESS,
	EHAND_SHAKE_FULL,
	EHAND_SHAKE_STOP_SERVICE,
	EHAND_SHAKE_VERSION_FAILED,
};

NAMESPACE_LOBBY_END

using LobbyPublic::uchar;
using LobbyPublic::MSG_ID;
using LobbyPublic::BOOL;
using LobbyPublic::SNID_t;

#endif //__GAMESERVER__


#endif