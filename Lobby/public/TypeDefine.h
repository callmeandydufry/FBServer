/*$T Share/Type.h GC 1.140 10/10/07 10:06:57 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#ifndef __TYPE_H__
#define __TYPE_H__

#include <vector>
#include "KwNetBasics.h"

typedef int16		ut_id;
typedef uint16		ut_id_mk;
typedef uint32		MK_VALUE;
typedef uint32		ut_id_obj;

typedef int32		ut_id_pres;
typedef ut_id		ut_id_lifeskill;
typedef ut_id		ut_id_team;
typedef ut_id		ut_id_map;
typedef ut_id		ut_id_connector;
typedef ut_id		ut_id_grid;
typedef ut_id		ut_id_spell;
typedef ut_id		ut_id_action;
typedef ut_id		ut_id_effect;
typedef ut_id		ut_id_coord;
typedef int32		ut_id_script;
typedef int32		ut_id_quest;
typedef ut_id		ut_id_group;
typedef ut_id		ut_id_channel;
typedef ut_id		ut_id_cooldown;
typedef uchar		ut_id_camp;
typedef int32		ut_id_guild;
typedef ut_id		ut_id_city;
typedef ut_id		ut_id_toplist;
typedef ut_id		ut_id_bus;
typedef ut_id		ut_id_country;
typedef ut_id		ut_id_family;
typedef int32		ut_id_time;
typedef int32		BOOL;
typedef unsigned long	ulong;
typedef int32		ut_id_teamgroup;
typedef ut_id		ut_id_room;
typedef ut_id		ut_id_signup;
typedef uint32		ut_id_gardenres;
typedef uint32		ut_id_garden;

//typedef struct _Int64
//{
//	long		a;
//	unsigned long	b;
//} Int64;

typedef uint64			SNID_t;

// 为了兼容书剑代码中类型声明 [JiangWei]
typedef SNID_t			GUID_t;
typedef ut_id_obj		ObjID_t;
typedef ut_id			ID_t;
#define INVALID_GUID    ((SNID_t) (-1))

#define INVALID_SNID	((SNID_t) (-1))
#define INVALID_KHAN_INDEX	(-1)
#define INVALID_KHAN_ID_EX	(-2)
#define INVALID_KHAN_ID_OBJ		(-3)   //所有使用CREATE_PACKET_OBJ_WORLD创建的消息，发送给map线程都需要使用这个值

#ifndef MIN
#define MIN(x,y) (((x) > (y)) ? (y) : (x))
#endif  // MIN

#ifndef MAX
#define MAX(x,y) (((x) < (y)) ? (y) : (x))
#endif  // MAX

#define IN
#define OUT 

#define CONVERT_CU_TO_ITEM(CU_Value) (52010000 + CU_Value)
#define EXTREMELY_RATIO_MULTIPLE (10000.0f)
#define EXTREMELY_RATIO(ratio) ((ratio)/EXTREMELY_RATIO_MULTIPLE)
#define PERCENT_RATIO(ratio) ((ratio)/100.0f)
#define ONE_LEVEL_PROPERTY_RATIO(ratio) ((ratio)/100.0f)
#define NOT_NEGATIVE_NUMBER(number) ((number < 0) ? 0 : number)
#define FILL_NUMERAL(x,value,lengh) \
	for (int32 i = 0; i < lengh; i++)\
	{\
		x[i] = value;\
	}

#define CLEAR_OBJECT(x,lengh)\
	for (int32 i = 0; i < lengh; i++)\
	{\
	x[i].Clear();\
	}

#if defined(__LINUX__)

// 64位数据输出
#define OUT64D								"%lld"
#define OUT64U								"%llu"
#define OUT64X								"%llX"
#define OUT64x								"%llx"

#elif defined(__WINDOWS__)

// 64位数据输出
#define OUT64D	"%I64d"
#define OUT64U	"%I64u"
#define OUT64X	"%I64X"
#define OUT64x	"%I64x"

#endif

#if defined(__WINDOWS__)
typedef ulong			TID;
typedef unsigned __int64	MODEL_PART;
typedef unsigned __int64	uint64;
#elif defined(__LINUX__)
typedef void			VOID;
typedef pthread_t		TID;
//typedef unsigned long long	uint64;
typedef unsigned long long	MODEL_PART;
typedef const char		*LPCSTR;
#endif
#if defined(__LINUX__)
typedef int32			MKHandle;
typedef struct
{
	volatile int32	counter;
} atomic_t;
typedef atomic_t	atword;
#define initword(v, _Idx)	(((v).counter) = (_Idx))
#elif defined(__WINDOWS__)
typedef void		*MKHandle;
typedef volatile long	atword;
#define initword(v, _Idx)	((v) = (_Idx))
#endif
typedef void	*DBHandle;

#define ISSET0(x)	((x) & 0x1)
#define ISSET1(x)	((x) & 0x2)
#define ISSET2(x)	((x) & 0x4)
#define ISSET3(x)	((x) & 0x8)
#define ISSET4(x)	((x) & 0x10)
#define ISSET5(x)	((x) & 0x20)
#define ISSET6(x)	((x) & 0x40)
#define ISSET7(x)	((x) & 0x80)
#define ISSET8(x)	((x) & 0x100)
#define ISSET9(x)	((x) & 0x200)
#define ISSET10(x)	((x) & 0x400)
#define ISSET11(x)	((x) & 0x800)
#define ISSET12(x)	((x) & 0x1000)
#define ISSET13(x)	((x) & 0x2000)
#define ISSET14(x)	((x) & 0x4000)
#define ISSET15(x)	((x) & 0x8000)
#include "Debug.h"
#include "Define.h"
#include "Enum.h"
#include "DefineResult.h"
#include "ModuleDefine.h"
#include "FixedString.h"
#include "Struct.h"
#include "Util.h"

#if defined(GAME_CLIENT)
#define __GUARD__
#define __UNGUARD__
#elif 1
#if defined(__WINDOWS__)
#if defined(NDEBUG)
#define __GUARD__ \
	{ \
		try \
		{
#define __UNGUARD__ \
} \
catch(...) \
{ \
	KCheckSpecial(FALSE, __FUNCTION__); \
} \
}

typedef void	*DBHandle;
#else
#define __GUARD__ \
	{ \
		try \
		{
#define __UNGUARD__ \
} \
catch(...) \
{ \
	KCheckSpecial(FALSE, __FUNCTION__); \
} \
}
#endif
#else
#define __GUARD__ \
	{ \
		try \
		{
#define __UNGUARD__ \
} \
catch(...) \
{ \
	KCheckSpecial(FALSE, __PRETTY_FUNCTION__); \
} \
}
#endif
#endif
#endif
