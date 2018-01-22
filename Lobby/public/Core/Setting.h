/*$T MapServer/Server/Base/Config.h GC 1.140 10/10/07 10:07:20 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "TypeDefine.h"
#include "Array_Base.h"

struct SC
{
	char	m_IP[IP_SIZE];
	uint32	m_Port;
	BOOL	m_EnableDB;
};

struct PLATFORM_SETTING
{
	int32	m_AppId;
	int32	m_ChannelId;
	int32	m_ChildChannel;
	int32	m_GameId;
	int32	m_ClientId;
	PLATFORM_SETTING()
	{
		memset(this, 0, sizeof(*this));
	}
};

struct SPECIAL_SETTING
{
	BOOL	m_Enable;
	ut_id	m_MapID;
	uint32	m_ServerCount;
	ut_id	m_WorldID;
	BOOL	m_PWCardEnable;

	char	m_AuServerIP[IP_SIZE];
	char	m_GLServerIP[IP_SIZE];
	char	m_BillingIP[IP_SIZE];
	char	m_DBCenterIP[IP_SIZE];

	uint32	AuServerPort;
	uint32	GLServerPort;
	uint32	BillPort;
	uint32	DBCenterPort;
	BOOL	GLServerEnableDBShare;
	SC	m_SrvInfo[MAX_SERVER];

	SPECIAL_SETTING()
	{
		memset(this, 0, sizeof(*this));
	}
};

struct MODULE_INFO
{
	FixedString<MAX_MODULE_NAME_LEN> m_szType;
	int32 m_nGroup;
	int32 m_nThreadIndex;
};

struct MODULE_PROCEDURE_INFO
{
	FixedString<MAX_MODULE_NAME_LEN> m_szName;
	FixedString<IP_SIZE> m_szIP;
	uint16 m_uPort;
	TDynaArray<MODULE_INFO> m_ModuleList;
};

struct MODULE_STAT
{
	MODULE_STAT()
	{
		mOnlinePlayerModuleNum = 0;
		mClientGateModuleNum = 0;
		mDBProxyModuleNum = 0;
		mRouterModuleNum = 0;
		mRegisterPlayerModuleNum = 0;
		mIndexAllocModuleNum = 0;
		mRoomModuleNum = 0;
		mSnidAllocModuleNum = 0;
	}

	int32 mOnlinePlayerModuleNum;
	int32 mClientGateModuleNum;
	int32 mDBProxyModuleNum;
	int32 mRouterModuleNum;
	int32 mRegisterPlayerModuleNum;
	int32 mIndexAllocModuleNum;
	int32 mRoomModuleNum;
	int32 mSnidAllocModuleNum;
};

struct MODULE_SETTING
{
	MODULE_STAT mModuleStat;

	MODULE_PROCEDURE_INFO mCoordinatorInfo;
	TStaticArray<MODULE_PROCEDURE_INFO, MAX_MODULE_PROCEDURE_COUNT> mProcedures;

	MODULE_SETTING()
	{
		memset(&mCoordinatorInfo, 0, sizeof(mCoordinatorInfo));
	}
};

struct ROBOT_SETTION
{
	int32 mRunRobotNum;

	ROBOT_SETTION()
	{
		memset(this, 0, sizeof(ROBOT_SETTION));
	}
};


struct BASE_SETTING
{
	ut_id	m_ServerID;
	int32	m_SystemMode;
	int32	m_KickUserTime;
	int32	mDisconnectTime;
	BOOL	m_nCompressMessage;
	int32	mSessionReservedTime;	// 网络连接断开后Session保留的时间 [12/23/2017 yz]
	BOOL	mEnableRandEncryptKey; // 是否启用随机消息加密机制 [12/28/2017 yz]

	BASE_SETTING()
	{
		m_ServerID = INVALID_KHAN_ID;
		m_SystemMode = 0;
		m_KickUserTime = 60000;
		mDisconnectTime = 1000;
		m_nCompressMessage = TRUE;
		mSessionReservedTime = 60000;
	};

	~BASE_SETTING()
	{
	};
};

struct SERVER_DATA
{
	ut_id		m_ServerID;
	ut_id		m_MachineID;
	char		m_IP[NET_TYPE_MAX_NUM][IP_SIZE];
	uint32		m_Port;
	int32		m_Type;

	SERVER_DATA()
	{
		Init();
	};

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	void Init()
	{
		m_ServerID = INVALID_KHAN_ID;
		m_MachineID = INVALID_KHAN_ID;
		memset(m_IP, 0, sizeof(m_IP));
		m_Port = 0;
		m_Type = -1;
	}

	const char* GetIpByNetType(NET_TYPE aiType)
	{
		char liType = aiType;
		if (liType <= NET_TYPE_UNKNOW || liType >= NET_TYPE_MAX_NUM )
		{
			liType = NET_TYPE_CNC;
		}

		return m_IP[liType];
	}
};

#define SERVER_CAPACITY 128

struct SERVER_SETTING
{
	SERVER_DATA	*m_pServer;
	uint32		m_ServerCount;
	int32		m_HashServer[SERVER_CAPACITY];

	SERVER_SETTING()
	{
		m_pServer = NULL;
		m_ServerCount = 0;
		for(int32 _Idx = 0; _Idx < SERVER_CAPACITY; _Idx++)
		{
			m_HashServer[_Idx] = -1;
		}
	}
	~SERVER_SETTING()
	{
		KSafeDelete_ARRAY(m_pServer);
		m_ServerCount = 0;
	}
};

class	SettingSystem
{
/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
public:
	SettingSystem();
	~	SettingSystem();

	BOOL	Init();
	BOOL	InitLog(int32 LogIndex);

private:

	void	LoadPlatformServerConfig();
	void	LoadBaseServerConfig();
	void	LoadServerIndexConfig();
	void	LoadGameConfig();

	void	PrintAllConfigInfo();

	// 读取模块配置信息 [11/25/2016 yz]
	void	LoadModuleConfig();

/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
public:
	PLATFORM_SETTING	mPlatformInfo;
	BASE_SETTING		mConfigInfo;
	SERVER_SETTING		mServerInfo;
	
	MODULE_SETTING		mModuleInfo;

	ROBOT_SETTION		mRobotInfo;

	char				mLogPath[255];
	int32				mLogIndex;
};

extern SettingSystem	g_Config;

#endif
