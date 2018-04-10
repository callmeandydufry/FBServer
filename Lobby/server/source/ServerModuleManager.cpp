#include "stdafx.h"
#include "ServerModuleManager.h"
#include "ModuleConnectorManager.h"
#include "ModulePacketMessage.h"
#include "ServerModuleExportInterface.h"
#include "ServerModuleRouter.h"
#include "RegisterPlayerManager.h"
#include "OnlinePlayerManager.h"
#include "IndexAllocManager.h"
#include "DBProxyManager.h"
#include "ClientGateManager.h"
#include "SnidAllocManager.h"
#include "ServerModuleThreadContainer.h"
#include "MailManager.h"

BOOL gIsOnlyLocalServer = FALSE;
BOOL gIsEnableModuleMultiThread = TRUE;

ServerModuleManager::ServerModuleManager()
{
	__GUARD__
		mRegisterModuleCount = 0;
	for (int i = 0; i < MAX_MODULE_COUNT; ++i)
	{
		mModuleRegisterInfos[i].clear();
	}

	mRegisterProcedureCount = 0;
	for (int i = 0; i < MAX_MODULE_PROCEDURE_COUNT; ++i)
	{
		mProcedureRegisterInfos[i].clear();
	}

	mIsWorking = FALSE;
	mIsPaused = FALSE;
	mIsAllModuleRegistered = FALSE;

	mModuleMap.InitTable(MAX_MODULE_COUNT);
	mProcedureMap.InitTable(MAX_MODULE_PROCEDURE_COUNT, MAX_MODULE_NAME_LEN);

	mMyListeningPort = INVALID_ID;

	mConnectCheckTimer.Clear();
	mCoordinatorSyncTimer.Clear();

	mServerRouter = NULL;
	mIsSelfCoordinator = FALSE;
	mModuleFactories.InitTable(MAX_MODULE_COUNT);

	__UNGUARD__
}

ServerModuleManager::~ServerModuleManager()
{
	__GUARD__

	KSafeDelete(mConnectorManager);

	__UNGUARD__
}

BOOL ServerModuleManager::init(const char* procedureName)
{
	__GUARD__

	mMyProcedureName = procedureName;

	mConnectorManager = new ModuleConnectorManager;
	KCheck(mConnectorManager);

	QLogSystem::QUICK_LOG(SLOW_LOG_SERVER, "ModuleManager::init self proc name = %s", procedureName);

	REGISTER_SERVER_MODULE_EXPORT_METHODS();

	RouterModuleFactory* pRouterFactory = new RouterModuleFactory();
	mModuleFactories.Add(pRouterFactory->getType(), pRouterFactory);
	RegisterModuleFactory* pRegFactory = new RegisterModuleFactory();
	mModuleFactories.Add(pRegFactory->getType(), pRegFactory);
	OnlinePlayerFactory* pOnlineFactory = new OnlinePlayerFactory();
	mModuleFactories.Add(pOnlineFactory->getType(), pOnlineFactory);
// 	IndexAllocFactory* pIndexFactory = new IndexAllocFactory();
// 	mModuleFactories.Add(pIndexFactory->getType(), pIndexFactory);
	DBProxyModuleFactory* pDBProxyFactory = new DBProxyModuleFactory();
	mModuleFactories.Add(pDBProxyFactory->getType(), pDBProxyFactory);
	ClientGateModuleFactory* pClientGateFactory = new ClientGateModuleFactory();
	mModuleFactories.Add(pClientGateFactory->getType(), pClientGateFactory);
// 	SnidAllocModuleFactory* SnidAllocFactory = new SnidAllocModuleFactory();
// 	mModuleFactories.Add(SnidAllocFactory->getType(), SnidAllocFactory);
	MailManagerFactory* pMailMgrFactory = new MailManagerFactory();
	mModuleFactories.Add(pMailMgrFactory->getType(), pMailMgrFactory);

	for (int i = 0; i < g_Config.mModuleInfo.mProcedures.GetSize(); ++i)
	{
		MODULE_PROCEDURE_INFO& procedureInfo = g_Config.mModuleInfo.mProcedures[i];
		if (procedureInfo.m_szName == mMyProcedureName)
		{
			registerProcedure(
				procedureInfo.m_szName.c_str(),
				procedureInfo.m_szIP.c_str(),
				procedureInfo.m_uPort
			);

			// ע�᱾�����е�ģ�� [12/7/2017 yz]
			for (int j = 0; j < procedureInfo.m_ModuleList.GetSize(); ++j)
			{
				EServerModuleType moduleType = ServerModuleUtil::convertString2Type(procedureInfo.m_ModuleList[j].m_szType.c_str());
				int32 moduleName = ServerModuleUtil::getModuleNameByTypeAndGroup(moduleType, procedureInfo.m_ModuleList[j].m_nGroup);
				
				ServerModuleFactory* factory = mModuleFactories.Get((int32)moduleType);
				if (NULL != factory)
				{
					ServerModuleBase* newModule = factory->createModule(moduleType, procedureInfo.m_ModuleList[j].m_nGroup);
					if (NULL != newModule)
					{
						newModule->initServerModule(moduleName, NULL, NULL);
						newModule->setThreadIndex(procedureInfo.m_ModuleList[j].m_nThreadIndex);

						registerModule(moduleName, mMyProcedureName.c_str());
						ModuleRegisterInfo* moduleInfo = mModuleMap.Get(moduleName);
						if (NULL != moduleInfo)
						{
							KCheck(FALSE == moduleInfo->mIsExternal);
							moduleInfo->mModulePtr = newModule;
						}

						// ·��ģ��������Ҫ��ModuleManager���������ģ���ע����Ϣ�����Ա�����ModuleManager��һ���߳��У����ܷŵ��̳߳���tick [1/13/2018 yz]
						if (moduleType == EServerModuleType_Router)
						{
							// ·��ģ����Ҫ�������浽һ�������У���Ϊ������Ҫ��ModuleServer�е������ĺ���ͬ������ģ���б�Coordinator�� [1/11/2018 yz]
							mServerRouter = static_cast<ServerModuleRouter*>(newModule);
						}
						else if(gIsEnableModuleMultiThread)
						{
							ServerModuleThread* moduleThread = (ServerModuleThread*)g_pModuleThreadContainer->GetThreadContainer().GetThreadByIndex(newModule->getThreadIndex());
							if (moduleThread == NULL)
							{
								moduleThread = new ServerModuleThread();
								g_pModuleThreadContainer->GetThreadContainer().InsertThread(moduleThread, newModule->getThreadIndex());
							}
							moduleThread->insertModule(newModule);
						}
					}
				}
			}
		}
	}

	// ÿ�����̶����봴��Router�������޷�ͬ������ģ�鵽Coordinator [1/11/2018 yz]
	KCheck(mServerRouter != NULL && "Every procedure must create one router when its initialization.");

	// Э������Ψһһ����Ҫ�������ļ�ע����ⲿģ�飬����ģ�鶼�Ǵ�Э������ȡ���ڽ��� [12/7/2017 yz]
	if (mMyProcedureName != "Coordinator") // ����������Coordinator���ǾͲ�����ע��һ���ⲿģ���ˣ�����ı���ģ�鴴����ʱ���ע�� [1/11/2018 yz]
	{
		MODULE_PROCEDURE_INFO& coordinatorInfo = g_Config.mModuleInfo.mCoordinatorInfo;

		registerProcedure(
			coordinatorInfo.m_szName.c_str(),
			coordinatorInfo.m_szIP.c_str(),
			coordinatorInfo.m_uPort
		);

		// ע�᱾�����е�ģ�� [12/7/2017 yz]
		for (int j = 0; j < coordinatorInfo.m_ModuleList.GetSize(); ++j)
		{
			EServerModuleType moduleType = ServerModuleUtil::convertString2Type(coordinatorInfo.m_ModuleList[j].m_szType.c_str());
			int32 moduleName = ServerModuleUtil::getModuleNameByTypeAndGroup(moduleType, coordinatorInfo.m_ModuleList[j].m_nGroup);
			registerModule(moduleName, coordinatorInfo.m_szName.c_str());
		}
	}
	else
	{
		mIsSelfCoordinator = TRUE;
	}

	KCheck(INVALID_ID != mMyListeningPort);

	__UNGUARD__
	return TRUE;
}

void ServerModuleManager::clear()
{
	TIntegerHashMap<ModuleRegisterInfo *>::IDTableIterator modIt = mModuleMap.Begin();
	while (modIt != mModuleMap.End())
	{
		ServerModuleBase* module = (ServerModuleBase*)modIt->second->mModulePtr;
		if (NULL != module)
		{
			if (gIsEnableModuleMultiThread)
			{
				// �Ƚ�Module���̳߳����Ƴ� [1/13/2018 yz]
				ServerModuleThread* moduleThread = (ServerModuleThread*)g_pModuleThreadContainer->GetThreadContainer().GetThreadByIndex(module->getThreadIndex());
				if (moduleThread != NULL)
				{
					moduleThread->removeModule(module);
					if (moduleThread->isEmpty())
					{
						g_pModuleThreadContainer->GetThreadContainer().RemoveThread(moduleThread->getTID());
						delete moduleThread;
					}
				}
			}

			// Ȼ��ɾ��Module���� [1/13/2018 yz]
			EServerModuleType modType;
			int32 group;
			ServerModuleUtil::getTypeAndGroupByModuleName(module->getModuleName(), modType, group);
			ServerModuleFactory* factory = mModuleFactories.Get((int32)modType);
			if (NULL != factory)
			{
				factory->destroyModule(module);
				if (modType == EServerModuleType_Router)
				{
					mServerRouter = NULL;
				}
			}
		}
		
		modIt++;
	}

	mServerRouter = NULL;
}

void ServerModuleManager::run()
{
	__GUARD__

	if (mConnectorManager)
		mConnectorManager->m_ThreadID = getTID();

	uint32 uTickCount = GET_TIME().TickCount();

	// ÿ����һ���������� [11/23/2016 yz]
	mConnectCheckTimer.BeginTimer(5000, uTickCount);

	if (!mIsSelfCoordinator)
	{
		mCoordinatorSyncTimer.BeginTimer(10000, uTickCount);
		// ��Coordinatorͬ������ģ���б���ȡԶ��ģ���б� [1/11/2018 yz]
		syncRemoteModuleRegInfo();
	}

	while (mIsWorking)
	{
		/*~~~~~~~~~~~~~~~~*/
		BOOL	ret = FALSE;
		/*~~~~~~~~~~~~~~~~*/

		SafeSleep(10);

		if (mIsPaused)
		{
			continue;
		}

		uTickCount = GET_TIME().TickCount();

		_NET_TRY
		{
			if (mConnectCheckTimer.CountingTimer(uTickCount))
			{
				checkExternalSocket();
			}
		}
			_NET_CATCH
		{
			QLOGGER_CODEHOLDER();
		}

		_NET_TRY
		{
			if (!mIsSelfCoordinator && mCoordinatorSyncTimer.CountingTimer(uTickCount))
			{
				// ��Coordinatorͬ������ģ���б���ȡԶ��ģ���б� [1/11/2018 yz]
				syncRemoteModuleRegInfo();
			}
		}
		_NET_CATCH
		{
			QLOGGER_CODEHOLDER();
		}

			_NET_TRY
		{
			translatePacket2Socket();
		}
			_NET_CATCH
		{
			QLOGGER_CODEHOLDER();
		}

			if (mConnectorManager)
			{
				_NET_TRY
				{
					m_Steps = ts_0;
					ret = mConnectorManager->Select();
					KCheck(ret);
					m_Steps = ts_1;
					ret = mConnectorManager->Handle_Exceptions();
					KCheck(ret);
					m_Steps = ts_2;
					ret = mConnectorManager->Handle_Inputs();
					KCheck(ret);
					m_Steps = ts_3;
					ret = mConnectorManager->Handle_Outputs();
					KCheck(ret);
				}
					_NET_CATCH
				{
					QLOGGER_CODEHOLDER();
				}

					_NET_TRY
				{
					ret = mConnectorManager->Handle_Commands();
					KCheck(ret);
					m_Steps = ts_4;
				}
					_NET_CATCH
				{
					QLOGGER_CODEHOLDER();
				}

					_NET_TRY
				{
					ret = mConnectorManager->Handle_CacheCommands();
					KCheck(ret);
					m_Steps = ts_5;
				}
					_NET_CATCH
				{
					QLOGGER_CODEHOLDER();
				}

					_NET_TRY
				{
					ret = mConnectorManager->HeartBeat();
					KCheck(ret);
					m_Steps = ts_6;
				}
					_NET_CATCH
				{
					QLOGGER_CODEHOLDER();
				}
			}

		m_Steps = ts_7;

		m_TickCount++;

		if (m_Init == FALSE && m_TickCount > 10)
		{
			m_Init = TRUE;
		}

		// ֻ��RouterModule��ModuleManager��tick������Module�ڸ��Ե��̳߳ض�����tick [1/13/2018 yz]
		if (mServerRouter)
		{
			mServerRouter->tickModule(uTickCount);
		}

		if (!gIsEnableModuleMultiThread)
		{
			if (mIsAllModuleRegistered || gIsOnlyLocalServer)
			{
				for (int32 i = 0; i < mRegisterModuleCount; ++i)
				{
					if (mModuleRegisterInfos[i].mModulePtr != NULL
						&& mModuleRegisterInfos[i].mModulePtr != mServerRouter)
					{
						mModuleRegisterInfos[i].mModulePtr->tickModule(uTickCount);
					}
				}
			}
		}
	};

	quit();

	__UNGUARD__
}

BOOL ServerModuleManager::registerModule(int32 moduleName, const char* procedureName, BOOL force)
{
	__GUARD__;
	// �Ѿ���ʼ����֮����ע���µ�ģ�� [11/22/2016 yz]
	if (isWorking() && !force)
	{
		KCheck(FALSE);
		return FALSE;
	}

	lock_guard<FastLock> autolock(mModuleRegisterLock);

	BOOL bIsNew = FALSE;
	ModuleRegisterInfo* moduleInfo = mModuleMap.Get(moduleName);
	if (NULL == moduleInfo)
	{
		if (mRegisterModuleCount >= MAX_MODULE_COUNT)
		{
			return FALSE;
		}
		moduleInfo = &mModuleRegisterInfos[mRegisterModuleCount];
		bIsNew = TRUE;
	}
	else
	{
		if (moduleInfo->mProcedureInfo)
		{
			moduleInfo->mProcedureInfo->mModuleCount--;
		}
	}

	ProcedureRegisterInfo* procInfo = mProcedureMap.Get(procedureName);
	if (NULL == procInfo)
		return FALSE;

	moduleInfo->mIsValid = TRUE;
	moduleInfo->mName = moduleName;
	moduleInfo->mProcedureName = procedureName;
	moduleInfo->mProcedureInfo = procInfo;
	procInfo->mModuleCount++;
	if (bIsNew)
	{
		mRegisterModuleCount++;
	}
	
	if (procInfo->mIsLocal)
	{
		moduleInfo->mIsExternal = FALSE;
	}
	else
	{
		moduleInfo->mIsExternal = TRUE;
		// ������ⲿģ�飬�������ص�ģ��İ󶨣�Ҫ�����ص�ģ��󶨻Ḳ���ⲿģ������� [12/6/2017 yz]
		moduleInfo->mModulePtr = NULL;
	}

	mModuleMap.Add(moduleName, moduleInfo);
	return TRUE;

	__UNGUARD__ return FALSE;
}

BOOL ServerModuleManager::registerProcedure(const char* procedureName, const char* ip, uint16 port, BOOL force)
{
	__GUARD__;
	// �Ѿ���ʼ����֮����ע���µĽ��� [11/22/2016 yz]
	if (isWorking() && !force)
	{
		KCheck(FALSE);
		return FALSE;
	}

	lock_guard<FastLock> autolock(mModuleRegisterLock);

	ProcedureRegisterInfo* procInfo = mProcedureMap.Get(procedureName);
	if (NULL != procInfo)
	{
		// ���ؽ��̼����˿ڲ����ڳ�ʼ��֮��Ķ� [12/6/2017 yz]
		if (mMyProcedureName == procedureName)
			return FALSE;

		procInfo->mIp = ip;
		procInfo->mPort = port;
		return TRUE;
	}

	if (mRegisterProcedureCount >= MAX_MODULE_PROCEDURE_COUNT)
	{
		return FALSE;
	}

	ProcedureRegisterInfo& newProc = mProcedureRegisterInfos[mRegisterProcedureCount];
	mRegisterProcedureCount++;
	newProc.mIsValid = TRUE;
	newProc.mProcedureName = procedureName;
	newProc.mIp = ip;
	newProc.mPort = port;

	// ˵�����Ǳ����̶��� [11/24/2016 yz]
	if (mMyProcedureName == procedureName)
	{
		mMyListeningPort = port;
		newProc.mIsLocal = TRUE;
	}

	mProcedureMap.Add(procedureName, &newProc);

	return TRUE;

	__UNGUARD__ return FALSE;
}

void ServerModuleManager::getLocalModuleRegInfos(ModuleRegList& moduleRegList) const
{
	for (int i = 0; i < mRegisterModuleCount; ++i)
	{
		if (!mModuleRegisterInfos[i].mIsExternal && mModuleRegisterInfos[i].mProcedureName == mMyProcedureName)
		{
			moduleRegList.mAllModules[moduleRegList.mModuleCount].mModuleName = mModuleRegisterInfos[i].mName;
			moduleRegList.mAllModules[moduleRegList.mModuleCount].mProcName = mModuleRegisterInfos[i].mProcedureName;
			if (NULL == mModuleRegisterInfos[i].mProcedureInfo)
				continue;
			moduleRegList.mAllModules[moduleRegList.mModuleCount].mIP = mModuleRegisterInfos[i].mProcedureInfo->mIp;
			moduleRegList.mAllModules[moduleRegList.mModuleCount].mPort = mModuleRegisterInfos[i].mProcedureInfo->mPort;
			moduleRegList.mModuleCount++;
		}
	}
}

void ServerModuleManager::getAllModuleRegInfos(ModuleRegList& moduleRegList) const
{
	for (int i = 0; i < mRegisterModuleCount; ++i)
	{
		moduleRegList.mAllModules[moduleRegList.mModuleCount].mModuleName = mModuleRegisterInfos[i].mName;
		moduleRegList.mAllModules[moduleRegList.mModuleCount].mProcName = mModuleRegisterInfos[i].mProcedureName;
		if(NULL == mModuleRegisterInfos[i].mProcedureInfo)
			continue;
		moduleRegList.mAllModules[moduleRegList.mModuleCount].mIP = mModuleRegisterInfos[i].mProcedureInfo->mIp;
		moduleRegList.mAllModules[moduleRegList.mModuleCount].mPort = mModuleRegisterInfos[i].mProcedureInfo->mPort;
		moduleRegList.mModuleCount++;
	}
}

void ServerModuleManager::syncRemoteModuleRegInfo()
{
	// ����Ա��ص����������������������Ҫ����������ͬ��ģ����Ϣ������ģ�鶼�ڱ����� [1/13/2018 yz]
	if (gIsOnlyLocalServer)
		return;

	if (mServerRouter)
	{
		mServerRouter->syncRemoteModuleRegInfo();
		// ���һ��ʲôʱ�����з�������ģ��ȫ��ע����� [1/12/2018 yz]
		if (!mIsAllModuleRegistered)
		{
			mIsAllModuleRegistered = mServerRouter->checkAllModuleRegistered();
		}
	}
}

void ServerModuleManager::startWork()
{
	__GUARD__;

	KCheck(mMyListeningPort != INVALID_ID);

	mIsWorking = TRUE;

	if (mConnectorManager)
	{
		BOOL ret = mConnectorManager->Init(
			MAX_MODULE_PROCEDURE_COUNT,
			"127.0.0.1",	// Ϊ����˫������,Map�й̶�����127, ʹ��any��������������
			mMyListeningPort
			);

		KCheck(ret);
	}

	start();
	__UNGUARD__
}

void ServerModuleManager::stopWork()
{
	mIsWorking = FALSE;
}

void ServerModuleManager::pauseWork()
{
	mIsPaused = TRUE;
}

void ServerModuleManager::resumeWork()
{
	mIsPaused = FALSE;
}

BOOL ServerModuleManager::waitThreadExit()
{
	while (getState() != KThread::THREAD_FINISH)
	{
		SafeSleep(100);
	}
	return TRUE;
}

uint32 ServerModuleManager::getCurrentTickCount()
{
	__GUARD__;
	return GET_TIME().TickCount();
	__UNGUARD__
		return 0;
}

void ServerModuleManager::sendPacket(int32 moduleName, DataPacket* packet)
{
	__GUARD__
		if (NULL == packet)
			return;

	if (!isWorking())
	{
		KSafeDelete(packet);
		return;
	}
	packet->mTargetModule = moduleName;

	ModuleRegisterInfo* moduleInfo = mModuleMap.Get(moduleName);
	if (NULL != moduleInfo && moduleInfo->mIsValid)
	{
		if (moduleInfo->mIsExternal)
		{
			moduleInfo->mSendPacketQueue.pushPacket(packet);
		}
		else if (NULL != moduleInfo->mModulePtr)
		{
			moduleInfo->mModulePtr->recvPacket(packet);
		}
		else
		{
			KSafeDelete(packet);
		}
	}
	else
	{
		KSafeDelete(packet);
	}
	__UNGUARD__
}

void ServerModuleManager::receivePacket(int32 moduleName, DataPacket* packet)
{
	__GUARD__
		if (NULL == packet)
			return;

	if (!isWorking())
	{
		KSafeDelete(packet);
		return;
	}
	ModuleRegisterInfo* moduleInfo = mModuleMap.Get(moduleName);
	if (NULL != moduleInfo && moduleInfo->mIsValid && !moduleInfo->mIsExternal && NULL != moduleInfo->mModulePtr)
	{
		moduleInfo->mModulePtr->recvPacket(packet);
	}
	else
	{
		KSafeDelete(packet);
	}
	__UNGUARD__
}

void ServerModuleManager::quit()
{
	__GUARD__;
	
	if (mConnectorManager)
	{
		mConnectorManager->RemoveAllConnector();
	}

	__UNGUARD__
}

void ServerModuleManager::checkExternalSocket()
{
	__GUARD__;
	if (!isWorking())
	{
		return;
	}

	if (mConnectorManager)
	{
		for (int32 i = 0; i < mRegisterProcedureCount; ++i)
		{
			ProcedureRegisterInfo& procInfo = mProcedureRegisterInfos[i];
			if (procInfo.mIsValid && procInfo.mModuleCount > 0 && !procInfo.mIsLocal)
			{
				mConnectorManager->ConnectServer(procInfo.mIp.c_str(), procInfo.mPort);
			}
		}
	}

	__UNGUARD__
}

void ServerModuleManager::translatePacket2Socket()
{
	__GUARD__;

	if (!isWorking())
		return;

	if (NULL == mConnectorManager)
		return;

	for (int32 i = 0; i < mRegisterModuleCount; ++i)
	{
		ModuleRegisterInfo& moduleInfo = mModuleRegisterInfos[i];
		if (moduleInfo.mIsValid && moduleInfo.mIsExternal)
		{
			if (NULL != moduleInfo.mProcedureInfo && !moduleInfo.mProcedureInfo->mIsLocal)
			{
				ut_id_connector connID = mConnectorManager->GetConnectIDByIP(moduleInfo.mProcedureInfo->mIp.c_str(), moduleInfo.mProcedureInfo->mPort);
				Connector* connector = mConnectorManager->GetConnector(connID);
				if (NULL != connector)
				{
					DataPacket* packet = NULL;
					while (moduleInfo.mSendPacketQueue.recvPacket(packet))
					{
						XXModulePacket msg;
						msg.exchangeDataPacket(packet);
						connector->SendMessage(&msg);
					}
				}
			}
			else
			{
				DataPacket* packet = NULL;
				while (moduleInfo.mSendPacketQueue.recvPacket(packet))
				{
					delete packet;
				}
			}
		}
	}

	__UNGUARD__
}
