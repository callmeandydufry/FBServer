/*$T MapServer/Server/Base/Config.cpp GC 1.140 10/10/07 10:07:20 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#include "stdafx.h"
#include "Setting.h"
#include "BaseTool.h"
#include "FileConfigConst.h"
#include "TAB.h"
#include "SplitStringLevelOne.h"


using namespace TAB;
SettingSystem	g_Config;

#define DIR_STEP	(__PI / 18.f)

#if ( defined(__LINUX__) && (defined(__MAPSERVER__) || defined(__GLSERVER__)) )
#include "udpsender.h"
CUdpSender g_UdpSender;
#endif


BOOL DecodeDBPassword( const char* pIn, char* pOut )
{
	char pOut_Binary[DB_MAX_USER_LEN*2+2] = {0};
	int32 idx = 0;
	uint32 OutLenTmp = 0;
	
	int32 nStartIndex = pIn[0] % (DB_MAX_USER_LEN*2+2);
	if( nStartIndex == 0 )
		nStartIndex = 1;
	
	char szLen_Binary[3] = {0};
	szLen_Binary[0] = pIn[DB_MAX_USER_LEN*2];
	szLen_Binary[1] = pIn[DB_MAX_USER_LEN*2+1];
	char PassLen = 0;
	
	Str2Binary( szLen_Binary, 2, (char*)&PassLen, 2, OutLenTmp );
	
	memset( pOut_Binary, 0, DB_MAX_USER_LEN*2+2 );
	for( int _Idx=nStartIndex; idx<PassLen*2; _Idx++ )
	{
		if( _Idx>=DB_MAX_USER_LEN*2 )
			_Idx = 1;
		pOut_Binary[idx] = pIn[_Idx];
		idx++;
	}

	Str2Binary( pOut_Binary, (uint32)strlen(pOut_Binary), pOut, DB_MAX_USER_LEN*2+2, OutLenTmp );

	return TRUE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
SettingSystem::SettingSystem()
{

	__GUARD__	
	mLogIndex = -1;
	__UNGUARD__
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
SettingSystem::~SettingSystem()
{
	__GUARD__ __UNGUARD__
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL SettingSystem::Init()
{
	__GUARD__	

	LoadServerIndexConfig();
	LoadBaseServerConfig();
	LoadGameConfig();
	PrintAllConfigInfo();
	LoadPlatformServerConfig();
	LoadModuleConfig();

	return TRUE;

	__UNGUARD__ return FALSE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL SettingSystem::InitLog(int32 LogIndex)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__	
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	mLogIndex = LogIndex;
	return TRUE;
	__UNGUARD__ return FALSE;
}

/*
=======================================================================================================================
 加载PlatformServer基本配置信息
=======================================================================================================================
*/
void SettingSystem::LoadPlatformServerConfig()
{
	__GUARD__ 

	Configer	ini(FILE_PLATFORMSERVERCONFIG);

	mPlatformInfo.m_AppId = (int32) (ini.Get_Property_Int("Platform", "AppId"));
	mPlatformInfo.m_ChannelId = (int32) (ini.Get_Property_Int("Platform", "ChannelId"));
	mPlatformInfo.m_ChildChannel = (int32) (ini.Get_Property_Int("Platform", "ChildChannel"));
	mPlatformInfo.m_GameId = (int32) (ini.Get_Property_Int("Platform", "GameId"));
	mPlatformInfo.m_ClientId = (int32) (ini.Get_Property_Int("Platform", "ClientId"));
	

	__UNGUARD__
}

/*
=======================================================================================================================
 加载服务器基本配置信息，主要是IP，Port
=======================================================================================================================
*/
void SettingSystem::LoadBaseServerConfig()
{
	__GUARD__ 

	Configer	ini(FILE_BASESERVERCONFIG);

	mServerInfo.m_ServerCount = ini.Get_Property_Int("System", "ServerCount");
	mServerInfo.m_pServer = new SERVER_DATA[mServerInfo.m_ServerCount];
	memset(mServerInfo.m_pServer, 0, sizeof(SERVER_DATA) * mServerInfo.m_ServerCount);

	uint32 _Idx;
	for(_Idx = 0; _Idx < mServerInfo.m_ServerCount; _Idx++)
	{
		char	szSection[256];
		memset(szSection, 0, 256);
		mServerInfo.m_pServer[_Idx].Init();
		sprintf(szSection, "Server%d", _Idx);
		mServerInfo.m_pServer[_Idx].m_ServerID = (ut_id) (ini.Get_Property_Int(szSection, "ServerIndex"));
		mServerInfo.m_pServer[_Idx].m_MachineID = (ut_id) (ini.Get_Property_Int(szSection, "MachineIndex"));
		ini.GetText(szSection, "IP", mServerInfo.m_pServer[_Idx].m_IP[NET_TYPE_CNC], IP_SIZE);
		mServerInfo.m_pServer[_Idx].m_Port = (uint32) (ini.Get_Property_Int(szSection, "Port"));
		mServerInfo.m_pServer[_Idx].m_Type = (uint32) (ini.Get_Property_Int(szSection, "Type"));
	}

	for(_Idx = 0; _Idx < mServerInfo.m_ServerCount; _Idx++)
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		ut_id	ServerID = mServerInfo.m_pServer[_Idx].m_ServerID;
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		KCheck(ServerID != INVALID_KHAN_ID && ServerID < SERVER_CAPACITY);

		KCheck(mServerInfo.m_HashServer[ServerID] == -1);

		mServerInfo.m_HashServer[ServerID] = _Idx;
	}

	__UNGUARD__
}

/*
=======================================================================================================================
 加载MapServer索引信息，
=======================================================================================================================
*/
void SettingSystem::LoadServerIndexConfig()
{
	__GUARD__ 

	Configer	ini(FILE_SERVERINDEX);

	mConfigInfo.m_ServerID = ini.Get_Property_Int("System", "ServerIndex");

	__UNGUARD__

}

void SettingSystem::LoadModuleConfig()
{
	__GUARD__

	Configer ini(FILE_MODULE_CONFIG);
	
	mModuleInfo.mModuleStat.mOnlinePlayerModuleNum = ini.Get_Property_Int("ALL", "OnlinePlayerModuleNum");
	mModuleInfo.mModuleStat.mClientGateModuleNum = ini.Get_Property_Int("ALL", "ClientGateModuleNum");
	mModuleInfo.mModuleStat.mDBProxyModuleNum = ini.Get_Property_Int("ALL", "DBProxyModuleNum");
	mModuleInfo.mModuleStat.mRouterModuleNum = ini.Get_Property_Int("ALL", "RouterModuleNum");
	mModuleInfo.mModuleStat.mRegisterPlayerModuleNum = ini.Get_Property_Int("ALL", "RegisterPlayerModuleNum");
	mModuleInfo.mModuleStat.mIndexAllocModuleNum = ini.Get_Property_Int("ALL", "IndexAllocModuleNum");
	mModuleInfo.mModuleStat.mRoomModuleNum = ini.Get_Property_Int("ALL", "RoomModuleNum");
	mModuleInfo.mModuleStat.mSnidAllocModuleNum = ini.Get_Property_Int("ALL", "SnidAllocModuleNum");

	mModuleInfo.mCoordinatorInfo.m_szName = "Coordinator";
	mModuleInfo.mCoordinatorInfo.m_szIP =		ini.GetText("Coordinator", "ip");
	mModuleInfo.mCoordinatorInfo.m_uPort =	ini.Get_Property_Int("Coordinator", "port");
	int32 moduleCount =							ini.Get_Property_Int("Coordinator", "modulecount");
	for (int32 i = 0; i < moduleCount; ++i)
	{
		String moduleName = StringUtil::printf("module%d", i);
		String moduleInfo = ini.GetText("Coordinator", moduleName.c_str());
		std::vector<String> moduleParams = StringUtil::split(moduleInfo, ",");
		if (moduleParams.size() >= 2)
		{
			mModuleInfo.mCoordinatorInfo.m_ModuleList.Add();
			mModuleInfo.mCoordinatorInfo.m_ModuleList[i].m_szType = StringUtil::trim(moduleParams[0]);
			mModuleInfo.mCoordinatorInfo.m_ModuleList[i].m_nGroup = atoi(StringUtil::trim(moduleParams[1]).c_str());
			mModuleInfo.mCoordinatorInfo.m_ModuleList[i].m_nThreadIndex = 0;
			if (moduleParams.size() >= 3)
			{
				mModuleInfo.mCoordinatorInfo.m_ModuleList[i].m_nThreadIndex = atoi(StringUtil::trim(moduleParams[2]).c_str());
			}
		}
	}

	String procListStr = ini.GetText("AllProc", "Procs");
	std::vector<String> procList = StringUtil::split(procListStr, ",");

	for (int32 i = 0; i < procList.size(); ++i)
	{
		const String& procedureSection = procList[i];
		MODULE_PROCEDURE_INFO* procInfo = mModuleInfo.mProcedures.Add(1);

		procInfo->m_szName = procedureSection;
		procInfo->m_szIP =		ini.GetText(procedureSection.c_str(), "ip");
		procInfo->m_uPort =		ini.Get_Property_Int(procedureSection.c_str(), "port");
		int32 moduleCount =		ini.Get_Property_Int(procedureSection.c_str(), "modulecount");
		for (int32 i = 0; i < moduleCount; ++i)
		{
			String moduleName = StringUtil::printf("module%d", i);
			String moduleInfo = ini.GetText(procedureSection.c_str(), moduleName.c_str());
			std::vector<String> moduleParams = StringUtil::split(moduleInfo, ",");
			if (moduleParams.size() >= 2)
			{
				procInfo->m_ModuleList.Add();
				procInfo->m_ModuleList[i].m_szType = StringUtil::trim(moduleParams[0]);
				procInfo->m_ModuleList[i].m_nGroup = atoi(StringUtil::trim(moduleParams[1]).c_str());
				procInfo->m_ModuleList[i].m_nThreadIndex = 0;
				if (moduleParams.size() >= 3)
				{
					procInfo->m_ModuleList[i].m_nThreadIndex = atoi(StringUtil::trim(moduleParams[2]).c_str());
				}
			}
		}
	}

	__UNGUARD__
}

/*
=======================================================================================================================
加载游戏全局信息
=======================================================================================================================
*/
void SettingSystem::LoadGameConfig()
{
	__GUARD__ 

	Configer	ini(FILE_GAMECONFIG);

	mConfigInfo.m_SystemMode = ini.Get_Property_Int("System", "SystemMode");

	mRobotInfo.mRunRobotNum = ini.Get_Property_Int("Robot", "RunRobotNum");
	if (mRobotInfo.mRunRobotNum > MAX_ROBOT_NUM)
	{
		KCheck(0);
	}

	__UNGUARD__
}


/*
 =======================================================================================================================
 =======================================================================================================================
 */
void SettingSystem::PrintAllConfigInfo()
{
	QLogSystem::QUICK_LOG(mLogIndex, "################################################################################################################################");
	QLogSystem::QUICK_LOG(mLogIndex, "#				ALL SERVER CONFIG				");
	QLogSystem::QUICK_LOG(mLogIndex, "################################################################################################################################");
	//////////////////////////////////////////////////////////////////////////
	// Print ServerIndex
	QLogSystem::QUICK_LOG(mLogIndex, "# ServerID=%d", mConfigInfo.m_ServerID);
	//////////////////////////////////////////////////////////////////////////
	// Print ServersConfig
	QLogSystem::QUICK_LOG(mLogIndex, "# ServerCount=%d", mServerInfo.m_ServerCount);

	uint32 _Idx;
	for(_Idx = 0; _Idx < mServerInfo.m_ServerCount; _Idx++)
	{
		QLogSystem::QUICK_LOG(
			mLogIndex, 
			"# [%u] CNC IP=%s, CHINANET IP=%s Port=%d, ServerID=%d, Type=%d, MachineID=%d",
			_Idx, 
			mServerInfo.m_pServer[_Idx].m_IP[NET_TYPE_CNC], 
			mServerInfo.m_pServer[_Idx].m_IP[NET_TYPE_CHINANET],
			mServerInfo.m_pServer[_Idx].m_Port, 
			mServerInfo.m_pServer[_Idx].m_ServerID,
			mServerInfo.m_pServer[_Idx].m_Type,
			mServerInfo.m_pServer[_Idx].m_MachineID
			);
	}
	QLogSystem::QUICK_LOG(mLogIndex, "################################################################################################################################");
}