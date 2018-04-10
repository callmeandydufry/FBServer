#include "stdafx.h"
#include "ServerModuleUtils.h"

const char* g_szModuleName_Unknown			= "Unknown";
const char* g_szModuleName_Router			= "Router";
const char* g_szModuleName_IndexAlloc		= "IndexAlloc";
const char* g_szModuleName_OnlinePlayer		= "OnlinePlayer";
const char* g_szModuleName_RegisterPlayer	= "RegPlayer";
const char* g_szModuleName_ClientGate		= "Gate";
const char* g_szModuleName_DBProxy			= "DBProxy";
const char* g_szModuleName_Room				= "Room";
const char* g_szModuleName_SnidAlloc		= "SnidAlloc";
const char* g_szModuleName_Mail				= "Mail";

#define MAX_GROUP_MODULE_NUM (10000)

// todo Ų�������ļ���ȥ,һ���ʼ�mod��ӦN��server [2/22/2018 Chief]
#define ConfigMatchServerNum (10)

const char* ServerModuleUtil::convertType2String(EServerModuleType type)
{
	switch (type)
	{
	case EServerModuleType_Router:
		return g_szModuleName_Router;
	case EServerModuleType_IndexAlloc:
		return g_szModuleName_IndexAlloc;
	case EServerModuleType_OnlinePlayer:
		return g_szModuleName_OnlinePlayer;
	case EServerModuleType_RegisterPlayer:
		return g_szModuleName_RegisterPlayer;
	case EServerModuleType_ClientGate:
		return g_szModuleName_ClientGate;
	case EServerModuleType_DBProxy:
		return g_szModuleName_DBProxy;
	case EServerModuleType_Room:
		return g_szModuleName_Room;
	case EServerModuleType_SnidAlloc:
		return g_szModuleName_SnidAlloc;
	case EServerModuleType_Mail:
		return g_szModuleName_Mail;
	}

	return g_szModuleName_Unknown;
}

EServerModuleType ServerModuleUtil::convertString2Type(const char* str)
{
	if (strncmp(str, g_szModuleName_Router, 256) == 0)
	{
		return EServerModuleType_Router;
	}
	else if (strncmp(str, g_szModuleName_IndexAlloc, 256) == 0)
	{
		return EServerModuleType_IndexAlloc;
	}
	else if (strncmp(str, g_szModuleName_OnlinePlayer, 256) == 0)
	{
		return EServerModuleType_OnlinePlayer;
	}
	else if (strncmp(str, g_szModuleName_RegisterPlayer, 256) == 0)
	{
		return EServerModuleType_RegisterPlayer;
	}
	else if (strncmp(str, g_szModuleName_ClientGate, 256) == 0)
	{
		return EServerModuleType_ClientGate;
	}
	else if (strncmp(str, g_szModuleName_DBProxy, 256) == 0)
	{
		return EServerModuleType_DBProxy;
	}
	else if (strncmp(str, g_szModuleName_Room, 256) == 0)
	{
		return EServerModuleType_Room;
	}
	else if (strncmp(str, g_szModuleName_SnidAlloc, 256) == 0)
	{
		return EServerModuleType_SnidAlloc;
	}
	else if (strncmp(str, g_szModuleName_Mail, 256) == 0)
	{
		return EServerModuleType_Mail;
	}
	return EServerModuleType_Unknown;
}

int32 ServerModuleUtil::getModuleNameByTypeAndGroup(EServerModuleType type, int32 group)
{
	return (int32)type * MAX_GROUP_MODULE_NUM + group;
}

void ServerModuleUtil::getTypeAndGroupByModuleName(int32 moduleName, EServerModuleType& type, int32& group)
{
	type = (EServerModuleType)(moduleName / MAX_GROUP_MODULE_NUM);
	group = moduleName % MAX_GROUP_MODULE_NUM;
}

int32 ServerModuleUtil::getCoordinatorModule()
{
	return getModuleNameByTypeAndGroup(EServerModuleType_Router, 0);
}

int32 ServerModuleUtil::getOnlineModuleByPlayerSnid(SNID_t snid)
{
	// ��SNID��β������ȷ�����������ҷ��䵽�Ǹ�ģ�飬�������Ա�֤ [12/16/2017 yz]
	 int32 group = snid % g_Config.mModuleInfo.mModuleStat.mOnlinePlayerModuleNum;
	 return getModuleNameByTypeAndGroup(EServerModuleType_OnlinePlayer, group);
}

int32 ServerModuleUtil::getRegisterModuleByPlayerSnid(SNID_t snid)
{
	// һ��RegisterPlayerModule����һ��� [1/16/2018 yz]
	int32 serverID = (int32)SnidUtil::getPlayerServerID(snid);
	return getModuleNameByTypeAndGroup(EServerModuleType_OnlinePlayer, serverID);
}

int32 ServerModuleUtil::getDBProxyModuleByPlayerSnid(SNID_t snid)
{
	int32 group = snid % g_Config.mModuleInfo.mModuleStat.mDBProxyModuleNum;
	return getModuleNameByTypeAndGroup(EServerModuleType_DBProxy, group);
}

int32 ServerModuleUtil::getSessionModuleBySessionID(int64 sessionID)
{
	int32 group = (int32)(sessionID >> 32);
	return getModuleNameByTypeAndGroup(EServerModuleType_ClientGate, group);
}

int32 ServerModuleUtil::getSnidAllocModuleBySnid(SNID_t n64Snid)
{
	int32 group = 0;
	return getModuleNameByTypeAndGroup(EServerModuleType_SnidAlloc, group);
}

int32 ServerModuleUtil::getMailModuleByPlayerSnid(SNID_t snid)
{
	// todo ֮�����ַ��������ļ�,�������Ƿ��ܽ���״̬��Ϊ��״̬?��������ע��mod,�ʼ�modһ�Զ�
	int32 serverID = (int32)SnidUtil::getPlayerServerID(snid);
	int32 nGroupID = serverID / ConfigMatchServerNum;
	return getModuleNameByTypeAndGroup(EServerModuleType_OnlinePlayer, nGroupID);
	// todo ��config�ļ��,�����Ӧ���������.ÿʮ��regmod��Ӧ1��mailmgr
}

int32 ServerModuleUtil::getDBProxyModuleForMailMgrByGroup(int32 group)
{
	// ��״̬��ģ��,��DBһ��һ[2/2/2018 Chief]
	return getModuleNameByTypeAndGroup(EServerModuleType_DBProxy, group);
}