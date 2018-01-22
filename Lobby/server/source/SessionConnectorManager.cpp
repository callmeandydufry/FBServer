#include "stdafx.h"
#include "SessionConnectorManager.h"
#include "ServerModuleUtils.h"
#include "ClientConnector.h"

SessionConnectorManager::SessionConnectorManager(ClientGateManager* gateManager)
	: mGateManager(gateManager)
{
	m_bServerMode = FALSE;
	SetEpollMaxSize(MODULE_GATESESSION_NUM);
}

SessionConnectorManager::~SessionConnectorManager()
{
}

BOOL SessionConnectorManager::InitPoolData(ConnectorPool* pool)
{
	m_pConnectorPool = pool;
	return TRUE;
}

BOOL SessionConnectorManager::RemoveConnector(Connector *pConnector)
{
	__GUARD__

	((ClientConnector *)pConnector)->FreeMySelf();
	BOOL bRet = ConnectorManager::RemoveConnector(pConnector);

	return bRet;
	__UNGUARD__
		return FALSE;
}

BOOL SessionConnectorManager::AddConnector(Connector* pConnector)
{
	__GUARD__

		BOOL bRet = ConnectorManager::AddConnector(pConnector);

	return TRUE;
	__UNGUARD__
		return FALSE;
}

BOOL SessionConnectorManager::HeartBeat(uint32 uTime)
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

		ClientConnector* pConnector = (ClientConnector*)GetConnector(m_aConnecorID[i]);
		if (pConnector == NULL)
		{
			QLogSystem::QUICK_LOG
			(
				QUICK_LOG_ERROR,
				"SessionConnectorManager::HeartBeat pConnector==NULL. pid=%d, i=%d, cnt1=%d, cnt2=%d ",
				m_aConnecorID[i],
				i,
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


BOOL SessionConnectorManager::Handle_CacheCommands()
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
