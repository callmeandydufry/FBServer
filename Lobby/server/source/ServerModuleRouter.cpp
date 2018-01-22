#include "stdafx.h"
#include "ServerModuleRouter.h"
#include "ServerModuleManager.h"

BOOL ServerModuleRouter::initServerModule(int32 moduleName, ModuleComponent* defaultComponent, int32 groupID)
{
	ServerModuleBase::initServerModule(moduleName, this, groupID);

	return TRUE;
}

void ServerModuleRouter::uinitServerModule()
{
	ServerModuleBase::uinitServerModule();
}

void ServerModuleRouter::tickModule(uint32 uTick)
{
	ServerModuleBase::tickModule(uTick);
}

void ServerModuleRouter::onRpcSyncLocal2RemoteModuleRegListSuccess(ModuleRegList& regList)
{
	// �ر�ע�⣺��Ϊ ModuleServerDefaultModule �� ModuleServerManager ��ͬһ���߳���tick�����Բ��ܵ���ģ��ע�ắ���������̲߳��ܵ��ã����������̳߳�ͻ [12/5/2017 yz]
	for (uint32 i = 0; i < regList.mModuleCount; ++i)
	{
		ModuleRegInfo& moduleInfo = regList.mAllModules[i];
		// ���ص�ģ��Ͳ���Զ�˷����Ľ��и����� [12/5/2017 yz]
		if (g_pModuleManager->getLocalProcedureName() == moduleInfo.mProcName)
		{
			continue;
		}

		g_pModuleManager->registerProcedure(moduleInfo.mProcName.c_str(), moduleInfo.mIP.c_str(), moduleInfo.mPort, TRUE);
		g_pModuleManager->registerModule(moduleInfo.mModuleName, moduleInfo.mProcName.c_str(), TRUE);
	}

	QLogSystem::QUICK_LOG(
		SLOW_LOG_SERVER,
		"ServerModuleRouter successfully syncronize remote module infomations, reply has been received."
	);
}

void ServerModuleRouter::onRpcSyncLocal2RemoteModuleRegListOvertime()
{
	QLogSystem::QUICK_LOG(
		SLOW_LOG_SERVER,
		"ServerModuleRouter failed syncronize remote module infomations, request overtime."
	);
}

// ��ͨ���������ô˺�����Э��������ͬ�����ص�ģ���б� [12/5/2017 yz]
void ServerModuleRouter::syncRemoteModuleRegInfo()
{
	ModuleRegList retRegList;
	static_cast<ServerModuleManager*>(g_pModuleManager)->getLocalModuleRegInfos(retRegList);

	// ��ȡ���ص�ģ�鲢�ϴ���Э������ȥ [11/30/2016 yz]
	g_pModuleManager->getExportCallable< IServerModuleRouterExportCallable<ServerModuleRouter> >(
		ServerModuleUtil::getCoordinatorModule(),
		MODULE_DEFAULT_ID,
		this
		)
		->rpcSyncLocal2RemoteModuleRegList(retRegList)
		->bindCallback(
			&ServerModuleRouter::onRpcSyncLocal2RemoteModuleRegListSuccess,
			&ServerModuleRouter::onRpcSyncLocal2RemoteModuleRegListOvertime,
			2000
		);

	QLogSystem::QUICK_LOG(
		SLOW_LOG_SERVER,
		"ServerModuleRouter start syncronize remote module infomations, and wait for remote coordinator's reply."
	);
}

BOOL ServerModuleRouter::checkAllModuleRegistered()
{
	__GUARD__

	MODULE_STAT moduleStat;
	ModuleRegList moduleRegList;
	ServerModuleManager* serverModuleManager = static_cast<ServerModuleManager*>(g_pModuleManager);
	serverModuleManager->getAllModuleRegInfos(moduleRegList);

	for (int32 i = 0; i < moduleRegList.mModuleCount; ++i)
	{
		EServerModuleType type = EServerModuleType_Unknown;
		int32 group = 0;
		ServerModuleUtil::getTypeAndGroupByModuleName(moduleRegList.mAllModules[i].mModuleName, type, group);

		if (type == EServerModuleType_Router)
		{
			moduleStat.mRouterModuleNum++;
		}
		else if (type == EServerModuleType_IndexAlloc)
		{
			moduleStat.mIndexAllocModuleNum++;
		}
		else if (type == EServerModuleType_OnlinePlayer)
		{
			moduleStat.mOnlinePlayerModuleNum++;
		}
		else if (type == EServerModuleType_RegisterPlayer)
		{
			moduleStat.mRegisterPlayerModuleNum++;
		}
		else if (type == EServerModuleType_ClientGate)
		{
			moduleStat.mClientGateModuleNum++;
		}
		else if (type == EServerModuleType_DBProxy)
		{
			moduleStat.mDBProxyModuleNum++;
		}
		else if (type == EServerModuleType_Room)
		{
			moduleStat.mRoomModuleNum++;
		}
		else if (type == EServerModuleType_SnidAlloc)
		{
			moduleStat.mSnidAllocModuleNum++;
		}
	}

	if (memcmp(&moduleStat, &g_Config.mModuleInfo.mModuleStat, sizeof(MODULE_STAT)) == 0)
	{
		QLogSystem::QUICK_LOG(
			SLOW_LOG_SERVER,
			"ServerModuleRouter all remote module information has been confirmed."
		);

		return TRUE;
	}
	return FALSE;

	__UNGUARD__
		return FALSE;
}

//----------------------------------------------------------------------
//RPC���ýӿ� START
//----------------------------------------------------------------------
ModuleRegList ServerModuleRouter::rpcSyncLocal2RemoteModuleRegList(ModuleRegList& regList)
{
	// �ر�ע�⣺��Ϊ ModuleServerDefaultModule �� ModuleServerManager ��ͬһ���߳���tick�����Բ��ܵ���ģ��ע�ắ���������̲߳��ܵ��ã����������̳߳�ͻ [12/5/2017 yz]

	for (uint32 i = 0; i < regList.mModuleCount; ++i)
	{
		ModuleRegInfo& moduleInfo = regList.mAllModules[i];
		if (i == 0)
		{
			// ���һ���Ƿ��и����صĽ������� [12/5/2017 yz]
			if (g_pModuleManager->getLocalProcedureName() == moduleInfo.mProcName)
			{
				QLogSystem::QUICK_LOG(
					SLOW_LOG_SERVER,
					"ERROR: rpcUploadRemoteModuleRegList remote procedure name conflict with local procedure name!"
				);
			}
		}
		g_pModuleManager->registerProcedure(moduleInfo.mProcName.c_str(), moduleInfo.mIP.c_str(), moduleInfo.mPort, TRUE);
		g_pModuleManager->registerModule(moduleInfo.mModuleName, moduleInfo.mProcName.c_str(), TRUE);
	}

	ModuleRegList retRegList;
	static_cast<ServerModuleManager*>(g_pModuleManager)->getAllModuleRegInfos(retRegList);

	return retRegList;
}

//----------------------------------------------------------------------
// factory
//----------------------------------------------------------------------

EServerModuleType RouterModuleFactory::getType() const
{
	return EServerModuleType_Router;
}

ServerModuleBase* RouterModuleFactory::createModule(EServerModuleType type, int32 group)
{
	if (mPtrServerModule)
	{
		delete(mPtrServerModule);
		mPtrServerModule;
	}

	mPtrServerModule = new ServerModuleRouter;
	return mPtrServerModule;
}

void RouterModuleFactory::destroyModule(ModuleBase* module)
{
	if (mPtrServerModule)
	{
		delete(mPtrServerModule);
		mPtrServerModule;
	}
}