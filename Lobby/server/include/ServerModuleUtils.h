#ifndef __SERVER_MODULE_UTILS_H__
#define __SERVER_MODULE_UTILS_H__

#include "TypeDefine.h"
#include "ModulePacket.h"

#ifdef __WINDOWS__
#ifdef __RELEASE_SERVER__
#define MODULE_REGISTERPLAYER_NUM				 (100000)
#define MODULE_ONLINEPLAYER_NUM					(10000)		
#define MODULE_BATTLEROOM_NUM					(1000)
#define MODULE_GATESESSION_NUM					(10000)
#else
#define MODULE_REGISTERPLAYER_NUM				(10000)
#define MODULE_ONLINEPLAYER_NUM					(1000)
#define MODULE_BATTLEROOM_NUM					(10)
#define MODULE_GATESESSION_NUM					(1000)
#endif
#else
#ifdef __RELEASE_SERVER__
#define MODULE_REGISTERPLAYER_NUM				(100000)
#define MODULE_ONLINEPLAYER_NUM					(10000)
#define MODULE_BATTLEROOM_NUM					(1000)
#define MODULE_GATESESSION_NUM					(10000)
#else
#define MODULE_REGISTERPLAYER_NUM				 (10000)
#define MODULE_ONLINEPLAYER_NUM					(1000)
#define MODULE_BATTLEROOM_NUM					(10)
#define MODULE_GATESESSION_NUM					(1000)
#endif
#endif

#define SNID_TYPE_RANGE		1000000000000
#define SNID_SUBTYPE_RANGE	10000000000
#define SNID_SERVERID_RANGE 100000000

enum ESnidType
{
	ESnidType_Player = 1,
	ESnidType_Guild = 2,
	ESnidType_Room = 3,
};

class SnidUtil
{
public:
	static SNID_t genPlayerSnid(int32 serverID, uint64 playerID)
	{
		SNID_t snid = (SNID_t)ESnidType_Player * SNID_TYPE_RANGE;
		snid += (SNID_t)serverID * SNID_SERVERID_RANGE;
		snid += playerID;
		return snid;
	}

	static BOOL isPlayerSnid(SNID_t snid)
	{
		int32 type = (int32)(snid / SNID_TYPE_RANGE);
		if (type == ESnidType_Player)
		{
			return TRUE;
		}
		return FALSE;
	}

	static int32 getPlayerServerID(SNID_t snid)
	{
		if (isPlayerSnid(snid))
		{
			return (int32)((snid % SNID_TYPE_RANGE) / SNID_SERVERID_RANGE);
		}
		return INVALID_ID;
	}

	static SNID_t getPlayerID(SNID_t snid)
	{
		return snid % SNID_SERVERID_RANGE;
	}
};

// 定义模块名称 [12/8/2017 yz]
extern const char* g_szModuleName_Unknown;
extern const char* g_szModuleName_Router;
extern const char* g_szModuleName_IndexAlloc;
extern const char* g_szModuleName_OnlinePlayer;
extern const char* g_szModuleName_RegisterPlayer;
extern const char* g_szModuleName_ClientGate;
extern const char* g_szModuleName_DBProxy;
extern const char* g_szModuleName_Room;

// 模块所在的进程定义 [11/24/2016 yz]
struct ProcedureRegisterInfo
{
	BOOL mIsValid;
	FixedString<MAX_MODULE_NAME_LEN> mProcedureName;
	FixedString<IP_SIZE> mIp;
	uint16 mPort;
	uint16 mModuleCount;
	BOOL mIsLocal;

	void clear()
	{
		mIsValid = FALSE;
		mProcedureName.clear();
		mIp.clear();
		mPort = INVALID_ID;
		mModuleCount = 0;
		mIsLocal = FALSE;
	}
};

class ModuleBase;

// 模块信息定义 [11/24/2016 yz]
struct ModuleRegisterInfo
{
	BOOL mIsValid;
	BOOL mIsExternal;
	int32 mName;
	FixedString<MAX_MODULE_NAME_LEN> mProcedureName;
	ProcedureRegisterInfo* mProcedureInfo;
	ModuleBase* mModulePtr;
	// 将要发送到模块的消息队列，消息现在这里缓存，最终要发到模块内部的队列中 [11/22/2016 yz]
	DataPacketQueue mSendPacketQueue;

	void clear()
	{
		mIsValid = FALSE;
		mIsExternal = FALSE;
		mName = INVALID_ID;
		mProcedureName.clear();
		mProcedureInfo = NULL;
		mModulePtr = NULL;
		mSendPacketQueue.clear();
	}
};

enum EServerModuleType
{
	EServerModuleType_Unknown = 0,
	EServerModuleType_Router = 1,
	EServerModuleType_IndexAlloc = 2,
	EServerModuleType_OnlinePlayer = 3,
	EServerModuleType_RegisterPlayer = 4,
	EServerModuleType_ClientGate = 5,
	EServerModuleType_DBProxy = 6,
	EServerModuleType_Room = 7,
	EServerModuleType_SnidAlloc = 8,

	EServerModuleType_MAX,
};

class ServerModuleUtil
{
public:
	static const char* convertType2String(EServerModuleType type);
	static EServerModuleType convertString2Type(const char* str);

	static int32 getModuleNameByTypeAndGroup(EServerModuleType type, int32 group);
	static void getTypeAndGroupByModuleName(int32 moduleName, EServerModuleType& type, int32& group);

	static int32 getCoordinatorModule();
	static int32 getOnlineModuleByPlayerSnid(SNID_t snid);
	static int32 getRegisterModuleByPlayerSnid(SNID_t snid);
	static int32 getDBProxyModuleByPlayerSnid(SNID_t snid);
	static int32 getSessionModuleBySessionID(int64 sessionID);
	static int32 getSnidAllocModuleBySnid(SNID_t snid);
};

#endif