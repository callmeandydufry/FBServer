#include "stdafx.h"
#include "ClientGateManager.h"
#include "ServerModuleUtils.h"
#include "IncomingConnectorManager.h"
#include "SessionConnectorManager.h"
#include "ClientSession.h"
#include "ClientConnector.h"

ClientGateManager::ClientGateManager()
	: mIncomingConnectorManager(NULL)
	, mSessionConnectorManager(NULL)
	, mAllocedSessionID(1)
{
	
}

ClientGateManager::~ClientGateManager()
{

}

BOOL ClientGateManager::initServerModule(int32 moduleName, ModuleComponent* defaultComponent, int32 groupID)
{
	__GUARD__
	// listen socket thread init
	ServerModuleBase::initServerModule(moduleName, this, groupID);

	mConnectorPool.Init(MODULE_GATESESSION_NUM);
	for (int32 i = 0; i < mConnectorPool.GetMaxCount(); i++)
	{
		Connector* pConnector = new ClientConnector();
		mConnectorPool.InitPoolData(i, pConnector);
	}

	mIncomingConnectorManager = new IncomingConnectorManager(this);
	mSessionConnectorManager = new SessionConnectorManager(this);

	mIncomingConnectorManager->InitPoolData(&mConnectorPool);
	mSessionConnectorManager->InitPoolData(&mConnectorPool);

	mIncomingPort = 17711;
	mIncomingConnectorManager->Init(MODULE_GATESESSION_NUM, "127.0.0.1", mIncomingPort);
	mIncomingConnectorManager->start();

	mSessionConnectorManager->Init(MODULE_GATESESSION_NUM);

	mSessionPool.initPool(MODULE_ONLINEPLAYER_NUM);
	mAccountSessionMap.InitTable(MODULE_ONLINEPLAYER_NUM, MAX_ACCOUNT);
	mSessionIDMap.InitTable(MODULE_ONLINEPLAYER_NUM);

	return TRUE;
	__UNGUARD__;
	return FALSE;
}

void ClientGateManager::uinitServerModule()
{
	ServerModuleBase::uinitServerModule();

	if (mIncomingConnectorManager)
	{
		mIncomingConnectorManager->stop();
		while (TRUE)
		{
			if (mIncomingConnectorManager->getState() == KThread::THREAD_FINISH)
			{
				break;
			}
			SafeSleep(100);
		}
	}

	KSafeDelete(mIncomingConnectorManager);
	KSafeDelete(mSessionConnectorManager);
}

void ClientGateManager::tickModule(uint32 uTick)
{
	ServerModuleBase::tickModule(uTick);

	TDynaArray<ClientSession*> destroyList;
	// ɾ�����õĻỰ���� [12/29/2017 yz]
	for (int32 i = 0; i < mSessionList.GetSize(); i++)
	{
		if (mSessionList[i]->isNeedDestroy())
		{
			destroyList.AddItem(mSessionList[i]);
		}
	}

	for (int32 i = 0; i < destroyList.GetSize(); i++)
	{
		destroySession(destroyList[i]);
	}

	if (mSessionConnectorManager)
	{
		BOOL ret = TRUE;
		_NET_TRY
		{
			ret = mSessionConnectorManager->Select();
			KCheck(ret);
			ret = mSessionConnectorManager->Handle_Exceptions();
			KCheck(ret);
			ret = mSessionConnectorManager->Handle_Inputs();
			KCheck(ret);
			ret = mSessionConnectorManager->Handle_Outputs();
			KCheck(ret);
		}
		_NET_CATCH
		{
			QLOGGER_CODEHOLDER();
		}

		_NET_TRY
		{
			ret = mSessionConnectorManager->Handle_Commands();
			KCheck(ret);
		}
		_NET_CATCH
		{
			QLOGGER_CODEHOLDER();
		}

		_NET_TRY
		{
			ret = mSessionConnectorManager->Handle_CacheCommands();
			KCheck(ret);
		}
		_NET_CATCH
		{
			QLOGGER_CODEHOLDER();
		}

		_NET_TRY
		{
			ret = mSessionConnectorManager->HeartBeat(uTick);
			KCheck(ret);
		}
		_NET_CATCH
		{
			QLOGGER_CODEHOLDER();
		}
	}
}

int32 ClientGateManager::allocSessionID()
{
	return mAllocedSessionID++;
}

ClientSession* ClientGateManager::createNewSession(const char* account, const char* token)
{
	__GUARD__;

	ClientSession* session = mAccountSessionMap.Get(account);
	// todo �����ظ������Ự [12/23/2017 yz]
	if (NULL != session)
	{
		// token��ͬʱΪ�ͻ��˶������� [1/11/2018 Chief]
		if (!strncmp(session->getToken(), token, sizeof(session->getToken())))  
		{
			session->requestPlayerOnline();
			return session;
		}
		else
		{
			// access token��ÿ���û���¼ʱ���ɣ�����ʱ��Ĭ��Ϊ�����£��û��ٴε�¼ʱ�Զ�ˢ��  
			// http://wiki.open.qq.com/wiki/mobile/OAuth2.0%E5%BC%80%E5%8F%91%E6%96%87%E6%A1%A3#Step3._.E8.8E.B7.E5.8F.96Access_Token
			// �޷�������������µ�¼ ������� ��һ�������¼(������Ҷ����½) todo ����ͳ�����ݣ�����������
			// ��Ҳ�����Ȩ���޷���½��Ϸ�ģ�����˼��������ҵ����
		}
		
	}

	ClientSession* newSession = mSessionPool.allocObject();
	if (NULL == newSession)
	{
		return NULL;
	}

	newSession->init(this);
	newSession->initAuth(account, token);
	
	mSessionList.AddItem(newSession);
	mAccountSessionMap.Add(account, newSession);
	mSessionIDMap.Add(newSession->getSessionID(), newSession);

	return newSession;
	__UNGUARD__
	return NULL;
}

uint64 ClientGateManager::rpcCreateRobotNewSession(const char* account, const char* token)
{
	__GUARD__;

	ClientSession* session = mAccountSessionMap.Get(account);
	if (NULL != session)
	{
		return session->getSessionID();
	}

	ClientSession* newSession = mSessionPool.allocObject();
	if (NULL == newSession)
	{
		return NULL;
	}

	newSession->init(this);
	newSession->initAuth(account, token);

	mSessionList.AddItem(newSession);
	mAccountSessionMap.Add(account, newSession);
	mSessionIDMap.Add(newSession->getSessionID(), newSession);
	newSession->setRobotFlag(TRUE);
	return newSession->getSessionID();
	__UNGUARD__;
	return INVALID_ID;
}

uint64 ClientGateManager::rpcS2CSynWorldChatData(const char* szMsg, uint32 chatID, SNID_t u64Sender)
{
	uint64 nSessionID = 0;
	__GUARD__;

	// todo �������������ݻ��浽ĳ����reg���������ʱ��client������������������

	// todo ���������ݷ������������������

	return nSessionID;
	__UNGUARD__;
	return nSessionID;
}

void ClientGateManager::destroySession(ClientSession* session)
{
	__GUARD__
	KCheck(session);

	BOOL found = FALSE;
	for (int32 i = 0; i < mSessionList.GetSize(); ++i)
	{
		if (mSessionList[i] == session)
		{
			mSessionList.RemoveSwap(i);
			found = TRUE;
		}
	}

	if (found)
	{
		mSessionIDMap.Remove(session->getSessionID());
		mAccountSessionMap.Remove(session->getAccount());

		session->uninit();
		mSessionPool.freeObject(session);
	}
	__UNGUARD__
}

BOOL ClientGateManager::checkSessionOnlineBySnid(SNID_t u64PlayerID)
{
	__GUARD__;

	ClientSession* pSession = mSessionIDMap.Get(u64PlayerID);
	if (NULL == pSession)
	{
		return FALSE;
	}

	return (ESESS_ONLINE == pSession->getSessionState());

	__UNGUARD__;
	return FALSE;
}

uint64 ClientGateManager::getSessionIDBySnid(SNID_t u64PlayerID)
{
	__GUARD__;

	ClientSession* pSession = mSessionIDMap.Get(u64PlayerID);
	if (NULL == pSession)
	{
		return FALSE;
	}

	return pSession->getSessionID();

	__UNGUARD__;
	return FALSE;
}

//----------------------------------------------------------------------
// factory
//----------------------------------------------------------------------

EServerModuleType ClientGateModuleFactory::getType() const
{
	return EServerModuleType_ClientGate;
}

ServerModuleBase* ClientGateModuleFactory::createModule(EServerModuleType type, int32 group)
{
	if (mPtrServerModule)
	{
		delete(mPtrServerModule);
		mPtrServerModule;
	}

	mPtrServerModule = new ClientGateManager;
	return mPtrServerModule;
}

void ClientGateModuleFactory::destroyModule(ModuleBase* module)
{
	if (mPtrServerModule)
	{
		delete(mPtrServerModule);
		mPtrServerModule;
	}
}