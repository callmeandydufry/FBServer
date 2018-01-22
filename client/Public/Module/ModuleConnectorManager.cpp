#include "stdafx.h"
#include "ModuleConnectorManager.h"
#include "ModuleConnector.h"
#include "SSHandShake.h"

ModuleConnectorManager::ModuleConnectorManager()
{
	m_bServerMode = TRUE;
	SetEpollMaxSize(10240);
	InitPoolData();
	mIpConnectorMap.InitTable(MAX_MODULE_CONNECTOR_POOL_SIZE, IP_PORT_SIZE);
}

ModuleConnectorManager::~ModuleConnectorManager()
{
}

BOOL ModuleConnectorManager::InitPoolData(ConnectorPool* pool)
{
	m_pConnectorPool = new ConnectorPool;
	KCheck(m_pConnectorPool);

	m_pConnectorPool->Init(MAX_MODULE_CONNECTOR_POOL_SIZE);
	for (int32 i = 0; i < m_pConnectorPool->GetMaxCount(); i++)
	{
		Connector* pConnector = new ModuleConnector;
		m_pConnectorPool->InitPoolData(i, pConnector);
	}

	return TRUE;
}

BOOL ModuleConnectorManager::RemoveConnector(Connector *pConnector)
{
	__GUARD__

		// 保证不能被其他的Manager错删除了 [3/18/2015 yangzhen]
		if (pConnector->GetOwnerThreadId() != m_ThreadID)
		{
			QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ModuleConnectorManager::RemoveConnector(): Error ! mgr='%s' pid=%d", GetManagerName(), pConnector->GetConnectorID());
			return FALSE;
		}

	FixedString<IP_SIZE> szIP = pConnector->GetSocket()->m_IP;
	uint32 uPort = pConnector->GetSocket()->m_Port;

	BOOL bRet = ConnectorManager::RemoveConnector(pConnector);
	if (bRet)
	{
		ut_id_connector connID = INVALID_ID;

		char ipPortAddr[IP_PORT_SIZE];
		tsnprintf_s(ipPortAddr, IP_PORT_SIZE, "%s:%u", szIP.c_str(), uPort);
		if (mIpConnectorMap.Get(ipPortAddr, connID))
		{
			mIpConnectorMap.Remove(ipPortAddr);
		}
	}
	return bRet;
	__UNGUARD__
		return FALSE;
}

BOOL ModuleConnectorManager::AddConnector(Connector* pConnector)
{
	__GUARD__

		BOOL bRet = ConnectorManager::AddConnector(pConnector);

	return TRUE;
	__UNGUARD__
		return FALSE;
}

BOOL ModuleConnectorManager::HeartBeat(uint32 uTime)
{
	__GUARD__

		BOOL	ret;
	if (uTime == 0)
		uTime = GET_TIME().TickCount();
	uint32	uConnectorCount = GetConnectorCount();

	for (uint32 i = 0; i < uConnectorCount; i++)
	{
		if (m_aConnecorID[i] == INVALID_ID)
			continue;

		ModuleConnector* pConnector = (ModuleConnector*)GetConnector(m_aConnecorID[i]);
		if (pConnector == NULL)
		{
			QLogSystem::QUICK_LOG
			(
				QUICK_LOG_ERROR,
				"ModuleConnectorManager::HeartBeat pConnector==NULL. pid=%d, i=%d, cnt1=%d, cnt2=%d ",
				m_aConnecorID[i],
				i,
				uConnectorCount,
				GetConnectorCount()
			);
			continue;
		}

		_NET_TRY
		{
			ret = pConnector->HeartBeat(uTime);
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


BOOL ModuleConnectorManager::Handle_CacheCommands()
{
	__GUARD__

		BOOL ret = FALSE;
	uint32 uret;

	for (uint32 _Idx = 0; _Idx < m_QueSize; _Idx++)
	{
		IMessage*		pKMessage = NULL;
		ut_id_connector	ConnectorID;
		uint32			Flag;
		BOOL			bNeedRemove = TRUE;

		ret = RecvMessage(pKMessage, ConnectorID, Flag);
		if (!ret)
			break;

		if (pKMessage == NULL)
		{
			QLOGGER_CODEHOLDER();
			break;
		}

		if (pKMessage == NULL)
		{
			QLOGGER_CODEHOLDER();
			break;
		}

		if (Flag == PF_REMOVE)
		{
			DESTROY_MESSAGE_OBJ(pKMessage);
			continue;
		}

		if ((ConnectorID == INVALID_KHAN_ID_EX) || (ConnectorID == INVALID_KHAN_ID))
		{
			_NET_TRY
			{
				uret = pKMessage->Process(NULL);
			}
				_NET_CATCH
			{
				QLOGGER_CODEHOLDER();
				uret = MP_ERROR;
			}

				if (uret == MP_ERROR)
				{
				}
				else if (uret == MP_BREAK)
				{
				}
				else if (uret == MP_CONTINUE)
				{
				}
				else if (uret == MP_NOTREMOVE)
				{
					bNeedRemove = FALSE;
				}
				else if (uret == MP_NOTREMOVE_ERROR)
				{
					bNeedRemove = FALSE;
				}
		}
		else
		{
			_NET_TRY
			{
				Connector *pConnector = GetConnector(ConnectorID);
				if (pConnector != NULL)
				{
					_NET_TRY
					{
						uret = pKMessage->Process(pConnector);
					}
						_NET_CATCH
					{
						QLOGGER_CODEHOLDER();
						uret = MP_ERROR;
					}

						if (uret == MP_ERROR)
						{
							RemoveConnector(pConnector);
						}
						else if (uret == MP_BREAK)
						{
						}
						else if (uret == MP_CONTINUE)
						{
						}
						else if (uret == MP_NOTREMOVE)
						{
							bNeedRemove = FALSE;
						}
						else if (uret == MP_NOTREMOVE_ERROR)
						{
							bNeedRemove = FALSE;
							RemoveConnector(pConnector);
						}
				}
				else
				{
					QLogSystem::QUICK_LOG
						(
						QUICK_LOG_CONNECTOR,
						"Handle_CacheCommands Error: pConnector==NULL. ConnectorID=%d, MsgID=%u",
						ConnectorID,
						pKMessage->GetMsgID()
						);

					// 专门为Login线程去掉这个KCheck，因为无论是正常登陆时或是断线重连时都会去主线程申请数据，这是如果断线会导致Connector被删除 [yangzhen]
					// 所以这里找不到Connector是很可能发生的，不是什么严重错误。 [yangzhen]
					// 而MapConnectorManager需要加这个，因为Map线程有断线重连的保护机制，保证Connector不会立即被删除 [yangzhen]
					//KCheck(FALSE);
				}
			}
				_NET_CATCH
			{
				QLOGGER_CODEHOLDER();
			}
		}

		if (bNeedRemove)
			DESTROY_MESSAGE_OBJ(pKMessage);
	}

	return TRUE;
	__UNGUARD__
		return FALSE;
}

ut_id_connector ModuleConnectorManager::ConnectServer(const char* ip, uint16 port)
{
	__GUARD__
		ut_id_connector connId = INVALID_ID;

	char ipPortAddr[IP_PORT_SIZE];
	tsnprintf_s(ipPortAddr, IP_PORT_SIZE, "%s:%u", ip, port);
	if (mIpConnectorMap.Get(ipPortAddr, connId))
	{
		return connId;
	}
	BOOL bRet = ConnectToServer(ip, port, 0, connId, TRUE, TRUE);
	if (bRet)
	{
		mIpConnectorMap.Add(ipPortAddr, connId);
	}
	return connId;
	__UNGUARD__
		return INVALID_ID;
}

ut_id_connector ModuleConnectorManager::GetConnectIDByIP(const char* ip, uint16 port)
{
	__GUARD__
		ut_id_connector connId = INVALID_ID;

	char ipPortAddr[IP_PORT_SIZE];
	tsnprintf_s(ipPortAddr, IP_PORT_SIZE, "%s:%u", ip, port);
	mIpConnectorMap.Get(ipPortAddr, connId);

	return connId;
	__UNGUARD__
		return INVALID_ID;
}

BOOL ModuleConnectorManager::ConnectToServer(const char* ip, uint16 port, ut_id serverid, ut_id_connector& connID, BOOL bCreate/*=TRUE*/, BOOL bSendSS/*=TRUE*/)
{
	__GUARD__

		int32				step = 0;
	BOOL				ret;
	uint32				_Idx = 0;
	BOOL				bRemove = FALSE;
	SSHandShake*			pMsg = NULL;
	Connector*			pConnector = m_pConnectorPool->NewConnector(bCreate);
	if (pConnector == NULL)
		return FALSE;
	Socket*				pSocket = pConnector->GetSocket();

	_NET_TRY
	{
		if (bCreate)
		{
			ret = pSocket->create();
			if (!ret)
			{
				step = 1;
				goto EXCEPTION;
			}
		}

		ret = pSocket->connect(ip, port);
		if (!ret)
		{
			step = 2;
			goto EXCEPTION;
		}

		ret = pSocket->setNonBlocking();
		if (!ret)
		{
			step = 3;
			goto EXCEPTION;
		}

		ret = pSocket->setLinger(0);
		if (!ret)
		{
			step = 4;
			goto EXCEPTION;
		}
	}
		_NET_CATCH
	{
		step = 5;
		goto EXCEPTION;
	}

	ret = AddConnector(pConnector);
	if (!ret)
	{
		step = 6;
		goto EXCEPTION;
	}
	bRemove = TRUE;

	if (bSendSS)
	{
		pMsg = new SSHandShake;
		pMsg->SetMapID(serverid);
		ret = pConnector->SendMessage(pMsg);
		KSafeDelete(pMsg);
		if (!ret)
		{
			step = 7;
			goto EXCEPTION;
		}
	}

	QLogSystem::QUICK_LOG
	(
		QUICK_LOG_CONNECTOR,
		"ServerConnectorManager::ConnectToServer(): Success ! IP:%s Port:%d ServerID:%d",
		ip,
		port,
		serverid
	);

	connID = pConnector->GetConnectorID();

	return TRUE;

EXCEPTION:
	QLogSystem::QUICK_LOG
	(
		QUICK_LOG_CONNECTOR,
		"ServerConnectorManager::ConnectToServer(): Fail ! IP:%s Port:%d ServerID:%d,Step:%d",
		ip,
		port,
		serverid,
		step
	);

	if (bRemove)
	{
		RemoveConnector(pConnector);
	}
	else
	{
		m_pConnectorPool->DelConnector(pConnector->GetConnectorID());
	}

	return FALSE;
	__UNGUARD__
		return FALSE;
}