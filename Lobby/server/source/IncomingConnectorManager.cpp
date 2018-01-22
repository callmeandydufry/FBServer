#include "stdafx.h"
#include "IncomingConnectorManager.h"
#include "ServerModuleUtils.h"
#include "ClientConnector.h"

#define ACCEPT_ONESTEP	50

IncomingConnectorManager::IncomingConnectorManager(ClientGateManager* gateManager)
	: mActive(TRUE)
	, mGateManager(gateManager)
{
	SetEpollMaxSize(MODULE_GATESESSION_NUM);
}

IncomingConnectorManager::~IncomingConnectorManager()
{
}

BOOL IncomingConnectorManager::InitPoolData(ConnectorPool* pool)
{
	m_pConnectorPool = pool;
	return TRUE;
}

Connector* IncomingConnectorManager::RecvNewConnector()
{
	__GUARD__

	Connector* pConnector = ConnectorManager::RecvNewConnector();

// 	if (pConnector)
// 	{
// 		pConnector->SetStatus(PS_KHAN_LOGIN_CONNECT);
// 
// 		((GameConnector*)pConnector)->SetDefaultEncrptorKey();
// 	}

	return pConnector;

	__UNGUARD__
		return NULL;
}


BOOL IncomingConnectorManager::RemoveConnector(Connector *pConnector)
{
	__GUARD__

		// 保证不能被其他的Manager错删除了 [3/18/2015 yangzhen]
		if (pConnector->GetOwnerThreadId() != m_ThreadID)
		{
			QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "IncomingConnectorManager::RemoveConnector(): Error ! mgr='%s' pid=%d", GetManagerName(), pConnector->GetConnectorID());
			return FALSE;
		}

	((ClientConnector*)pConnector)->FreeMySelf();

	BOOL bRet = ConnectorManager::RemoveConnector(pConnector);

	return bRet;
	__UNGUARD__
		return FALSE;
}

BOOL IncomingConnectorManager::AddConnector(Connector* pConnector)
{
	__GUARD__

	BOOL bRet = ConnectorManager::AddConnector(pConnector);

	if (bRet)
	{
		if (g_Config.mConfigInfo.m_nCompressMessage)
			((ClientConnector*)pConnector)->EnableCompress(TRUE, KGetCurrentTID());
	}

	return TRUE;
	__UNGUARD__
		return FALSE;
}

BOOL IncomingConnectorManager::HeartBeat(uint32 uTime)
{
	__GUARD__

		BOOL	ret;
	if (uTime == 0)
		uTime = GET_TIME().TickCount();
	uint32	uConnectorCount = GetConnectorCount();

	for (uint32 _Idx = 0; _Idx < uConnectorCount; _Idx++)
	{
		if (m_aConnecorID[_Idx] == INVALID_KHAN_ID)
			continue;

		ClientConnector* pConnector = (ClientConnector*)GetConnector(m_aConnecorID[_Idx]);
		if (pConnector == NULL)
		{
			QLogSystem::QUICK_LOG
			(
				QUICK_LOG_ERROR,
				"IncomingConnectorManager::HeartBeat pConnector==NULL. pid=%d, _Idx=%d, cnt1=%d, cnt2=%d ",
				m_aConnecorID[_Idx],
				_Idx,
				uConnectorCount,
				GetConnectorCount()
			);
			continue;
		}

		_NET_TRY
		{
			ret = pConnector->HeartBeat(uTime,1);
		}
			_NET_CATCH
		{
			ret = FALSE;
		}

			if (!ret)
			{
				ret = RemoveConnector(pConnector);
			}
	}

	return TRUE;
	__UNGUARD__
		return FALSE;
}

void IncomingConnectorManager::run()
{
	__GUARD__	_NET_TRY
	{
		QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "IncomingConnectorManager::run() Thread ID:%d...",getTID());

		while (IsActive())
		{
			m_TickCount++;
			SafeSleep(100);

			BOOL ret = TRUE;

			_NET_TRY
			{
				m_Steps = ts_0;
				ret = Select();
				KCheck(ret);
				m_Steps = ts_1;
				ret = Handle_Exceptions();
				KCheck(ret);
				m_Steps = ts_2;
				ret = Handle_Inputs();
				KCheck(ret);
				m_Steps = ts_3;
				ret = Handle_Outputs();
				KCheck(ret);
			}
			_NET_CATCH
			{
				QLOGGER_CODEHOLDER();
			}

			_NET_TRY
			{
				ret = Handle_Commands();
				KCheck(ret);
				m_Steps = ts_4;
			}
			_NET_CATCH
			{
				QLOGGER_CODEHOLDER();
			}

			_NET_TRY
			{
				ret = Handle_CacheCommands();
				KCheck(ret);
				m_Steps = ts_5;
			}
			_NET_CATCH
			{
				QLOGGER_CODEHOLDER();
			}

			_NET_TRY
			{
				ret = HeartBeat();
				KCheck(ret);
			}
			_NET_CATCH
			{
				QLOGGER_CODEHOLDER();
			}

			m_TickCount++;

		}
	}
	_NET_CATCH
	{
		QLOGGER_CODEHOLDER();
	}

	QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "IncomingConnectorManager Exit!!!! Thread ID:%d...", getTID());

	__UNGUARD__
}