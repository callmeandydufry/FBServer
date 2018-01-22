#include "stdafx.h"
#include "ConnectorManager.h"
#include "MessageFactoryManager.h"


ConnectorManager::ConnectorManager()
{
	m_uConnectorCount = 0;
	m_uMaxConnectorCount = 0;
	m_aConnecorID = NULL;
	m_pConnectorPool = NULL;

	m_bServerMode = TRUE;
	m_pServerSocket = NULL;
	m_SocketID = INVALID_SOCKET;
	m_uFDSize = 0;

	m_Head = 0;
	m_Tail = 0;
	m_MessageQue = new ASYNC_MSG[MAX_CACHE_SIZE];
	KCheck(m_MessageQue);
	m_QueSize = MAX_CACHE_SIZE;
}

ConnectorManager::~ConnectorManager()
{
	KSafeDelete( m_pConnectorPool );
	KSafeDelete_ARRAY( m_aConnecorID );
	KSafeDelete_ARRAY( m_MessageQue );
}

BOOL ConnectorManager::Init(uint32 uMaxCount,const char* ip,uint16 port, uint32 uBackLog)
{
	__GUARD__

	m_uMaxConnectorCount = uMaxCount;
	m_aConnecorID = new ut_id_connector[uMaxCount];
	KCheck(m_aConnecorID);
	for( int32 _Idx=0; _Idx<uMaxCount; _Idx++ )
	{
		m_aConnecorID[_Idx] = INVALID_KHAN_ID;
	}

	m_ThreadID = KGetCurrentTID();

	return TRUE;
	__UNGUARD__
	return FALSE;
}

BOOL ConnectorManager::HeartBeat(uint32 uTime)
{
	__GUARD__ 
		
	BOOL	ret;
	if( uTime == 0 )
		uTime = GET_TIME().TickCount();
	uint32	uConnectorCount = GetConnectorCount();
	
	for(uint32 _Idx = 0; _Idx < uConnectorCount; _Idx++)
	{
		if(m_aConnecorID[_Idx] == INVALID_KHAN_ID) 
		{
			QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager::HeartBeat(): Error ! m_aConnecorID[%d] == INVALID_ID", _Idx );
			continue;
		}

		Connector* pConnector = GetConnector(m_aConnecorID[_Idx]);
		if(pConnector == NULL)
		{
			QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager::HeartBeat(): Error ! pConnector==NULL,id=%d", m_aConnecorID[_Idx] );
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

		if(!ret)
		{
			ret = RemoveConnector(pConnector);
			KCheck(ret);
		}
	}

	return TRUE;
	__UNGUARD__
	return FALSE;
}


BOOL ConnectorManager::AddConnectorToMgr(Connector *pConnector)
{
	__GUARD__

	if( pConnector == NULL )
	{
		QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager::AddConnectorToMgr(): Error ! pConnector == NULL" );
		return FALSE;
	}

	if( m_uConnectorCount >= m_uMaxConnectorCount )
	{
		QLogSystem::QUICK_LOG
			(
				QUICK_LOG_CONNECTOR,
				"ConnectorManager::AddConnectorToMgr(): Error ! count(%d) > maxcount(%d)",  
				m_uConnectorCount,
				m_uMaxConnectorCount
			);
		return FALSE;
	}

	if( m_aConnecorID[m_uConnectorCount] != INVALID_KHAN_ID )
	{
		QLogSystem::QUICK_LOG
			( 
				QUICK_LOG_CONNECTOR,
				"ConnectorManager::AddConnectorToMgr(): Error ! m_aConnecorID[%d]==%d", 
				m_uConnectorCount, 
				m_aConnecorID[m_uConnectorCount] 
			);
		return FALSE;
	}

	m_aConnecorID[m_uConnectorCount] = pConnector->GetConnectorID();
	pConnector->SetManagerID(m_uConnectorCount);
	pConnector->SetConnectorMgr(this);

	m_uConnectorCount++;

	return TRUE;
	__UNGUARD__ 
	return FALSE;
}

BOOL ConnectorManager::DelConnectorFromMgr(Connector *pConnector)
{
	__GUARD__	
		
	if( pConnector == NULL )
	{
		QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager::DelConnectorFromMgr(): Error ! pConnector == NULL" );
		return FALSE;
	}

	if( m_uConnectorCount<=0 )
	{
		QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager::DelConnectorFromMgr(): Error ! count(%d)<=0", m_uConnectorCount );
		return FALSE;
	}

	ut_id ManagerID = pConnector->GetManagerID();
	if(ManagerID < 0 || ManagerID >= m_uMaxConnectorCount)
	{
		QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager::DelConnectorFromMgr(): Error ! Invalid ManagerID %d first time", ManagerID );

		for( int32 _Idx=0; _Idx<m_uConnectorCount; _Idx++ )
		{
			if( m_aConnecorID[_Idx] == pConnector->GetConnectorID() )
			{
				ManagerID = _Idx;
				break;
			}
		}

		if(ManagerID < 0 || ManagerID >= m_uMaxConnectorCount)
		{
			QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager::DelConnectorFromMgr(): Error ! Invalid ManagerID %d second time", ManagerID );
			return FALSE;
		}
	}

	Connector* pLastConnector = GetConnector(m_aConnecorID[m_uConnectorCount - 1]);
	if( pLastConnector == NULL )
	{
		QLogSystem::QUICK_LOG
			( 
				QUICK_LOG_CONNECTOR,
				"ConnectorManager::DelConnectorFromMgr(): Error ! pLastConnector==NULL,count=%d,id=%d",
				m_uConnectorCount,
				m_aConnecorID[m_uConnectorCount - 1]
			);
		return FALSE;
	}

	if( pConnector != pLastConnector )
	{
		m_aConnecorID[ManagerID] = m_aConnecorID[m_uConnectorCount - 1];
		m_aConnecorID[m_uConnectorCount - 1] = INVALID_KHAN_ID;
		pLastConnector->SetManagerID(ManagerID);
		pConnector->SetManagerID(INVALID_KHAN_ID);
		pConnector->SetConnectorMgr(NULL);
	}
	else
	{
		m_aConnecorID[ManagerID] = INVALID_KHAN_ID;
		pConnector->SetManagerID(INVALID_KHAN_ID);
		pConnector->SetConnectorMgr(NULL);
	}

	RecieveStream* pReciStream = pConnector->GetRecieveStream();
	if(NULL != pReciStream)
	{
		// 从一个Manager中移除可能会加入另一个Manager，清理ReciStream为了在离开Manager后不再执行后续的消息包，否则可能导致Connector已经在另一个Manager中了，
		// 但后续的消息还在以前的Manager中执行，如果Kcheck后删除Connector还会导致更严重的问题 [yangzhen]
		pReciStream->Clear();
	}

	m_uConnectorCount--;

	return TRUE;
	__UNGUARD__
	return FALSE;
}

BOOL  ConnectorManager::DelInvalidConnectorFromMgr(uint32 Index)
{
	__GUARD__
		KCheck(Index >=0 && Index < m_uConnectorCount);

		if (Index == m_uConnectorCount - 1)
		{
			m_aConnecorID[Index] = INVALID_KHAN_ID;		
		}
		else
		{
			Connector* pLastConnector = GetConnector(m_aConnecorID[m_uConnectorCount - 1]);
			if( pLastConnector == NULL )
			{
				// 如果最后一个也是无效，则先处理最后的无效，当前无效的在下次tick时候清理 [xingzhishan]
				m_aConnecorID[m_uConnectorCount - 1] = INVALID_KHAN_ID;		
			}
			else
			{
				m_aConnecorID[Index] = m_aConnecorID[m_uConnectorCount - 1];
				m_aConnecorID[m_uConnectorCount - 1] = INVALID_KHAN_ID;
				pLastConnector->SetManagerID(Index);
			}
		}
		m_uConnectorCount--;
		return  TRUE;
	__UNGUARD__ return FALSE;
}

BOOL ConnectorManager::AddConnector(Connector *pConnector)
{
	__GUARD__ 

	if( pConnector == NULL )
	{
		QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager::AddConnector(): Error ! mgr='%s' pConnector==NULL", GetManagerName() );
		return FALSE;
	}

	BOOL bRet = FALSE;

	bRet = AddConnectorSocket(pConnector->GetSocket()->getSOCKET(),pConnector->GetConnectorID());
	if( bRet )
	{
		bRet = AddConnectorToMgr(pConnector);
		if(!bRet)
		{
			DelConnectorSocket(pConnector->GetSocket()->getSOCKET(), pConnector->GetConnectorID());
			QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager::AddConnector(): Error ! AddConnectorToMgr Failed! mgr='%s' fd=%d pid=%d", 
				GetManagerName(),
				pConnector->GetSocket()->getSOCKET(), pConnector->GetConnectorID() );
			return FALSE;
		}
	}
	else
	{
		QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager::AddConnector(): Error ! AddConnectorSocket Failed! mgr='%s' fd=%d pid=%d", 
			GetManagerName(),
			pConnector->GetSocket()->getSOCKET(), pConnector->GetConnectorID() );
		return FALSE;
	}

	if(pConnector->GetOwnerThreadId() != 0)
	{
		QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager::AddConnector(): pConnector->m_nCurrThreadId != 0 ! mgr='%s' fd=%d pid=%d", GetManagerName(), pConnector->GetSocket()->getSOCKET(), pConnector->GetConnectorID() );
	}
	pConnector->SetOwnerThreadId(m_ThreadID);

	QLogSystem::QUICK_LOG
		( 
			QUICK_LOG_CONNECTOR,
			"ConnectorManager::AddConnector(): Success ! mgr='%s' fd=%d pid=%d", 
			GetManagerName(),
			pConnector->GetSocket()->getSOCKET(),
			pConnector->GetConnectorID() 
		);

	return TRUE;
	__UNGUARD__
	return FALSE;
}

BOOL ConnectorManager::DelConnector(Connector *pConnector)
{
	__GUARD__

	BOOL bRet = FALSE;

	SOCKET fd = pConnector->GetSocket()->getSOCKET();
	ut_id_connector ConnectorID = pConnector->GetConnectorID();

	if(pConnector->GetOwnerThreadId() != m_ThreadID)
	{
		QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager::DelConnector(): pConnector->m_nCurrThreadId != m_ThreadID ! mgr='%s' fd=%d pid=%d", GetManagerName(), fd, ConnectorID );
	}
	pConnector->SetOwnerThreadId(0);

	bRet = DelConnectorSocket(pConnector->GetSocket()->getSOCKET(), pConnector->GetConnectorID());
	if( !bRet )
	{
		QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager::DelConnector(): Error ! DelConnectorSocket Failed! mgr='%s' fd=%d pid=%d", GetManagerName(), fd, ConnectorID );
	}

	bRet = DelConnectorFromMgr(pConnector);
	if(!bRet)
	{
		QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager::DelConnector(): Error ! DelConnectorFromMgr Failed! mgr='%s' fd=%d pid=%d", GetManagerName(), fd, ConnectorID );
	}

	if(bRet)
	{
		QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager::DelConnector(): Success ! mgr='%s' fd=%d pid=%d", GetManagerName(), fd, ConnectorID );
	}

	return bRet;
	__UNGUARD__ 
	return FALSE;
}

BOOL ConnectorManager::RemoveConnector(Connector *pConnector)
{
	__GUARD__

	if( pConnector == NULL )
	{
		QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager::RemoveConnector(): Error ! pConnector==NULL mgr='%s'", GetManagerName() );
		return FALSE;
	}

	BOOL ret = DelConnector(pConnector);
	m_pConnectorPool->DelConnector(pConnector->GetConnectorID());
	MoveMessage(pConnector->GetConnectorID());

	return ret;
	__UNGUARD__ 
	return FALSE;
}

void ConnectorManager::RemoveAllConnector()
{
	__GUARD__ 
		
	uint32 uConnectorCount = GetConnectorCount();
	for(uint32 _Idx = 0; _Idx < uConnectorCount; _Idx++)
	{
		// Connector被删除后，数组采取了紧缩的处理，所以每次要删除的索引必须是0 [xingzhishan]
		if(m_aConnecorID[0] == INVALID_KHAN_ID)
		{
			QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager::RemoveAllConnector(): Error ! m_aConnecorID[%d] == INVALID_ID", 0 );
			continue;
		}

		Connector* pConnector = GetConnector(m_aConnecorID[0]);
		if(pConnector == NULL)
		{
			QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager::RemoveAllConnector(): Error ! pConnector==NULL, id=%d ", m_aConnecorID[0] );
			continue;
		}

		RemoveConnector(pConnector);
	}

	__UNGUARD__
}

Connector* ConnectorManager::GetConnector(ut_id_connector pid)
{
	return m_pConnectorPool->GetConnector(pid);
}

Connector* ConnectorManager::FindConnectorInManager(ut_id_connector pid)
{
	__GUARD__
	for( int32 _Idx=0; _Idx<m_uConnectorCount; _Idx++ )
	{
		if( m_aConnecorID[_Idx] == pid )
		{
			return m_pConnectorPool->GetConnector(pid);
		}
	}
	return NULL;
	__UNGUARD__
	return NULL;
}

Connector* ConnectorManager::GetConnectorByIndex(int32 nIndex)
{
	__GUARD__
	if(nIndex >= 0 && nIndex < m_uConnectorCount)
	{
		if(m_aConnecorID[nIndex] != INVALID_ID)
		{
			return m_pConnectorPool->GetConnector(m_aConnecorID[nIndex]);
		}
	}
	return NULL;
	__UNGUARD__
	return NULL;
}

// 以跨服方式取得连接
Connector *ConnectorManager::GetConnectorForGroupMode(ut_id_connector pid)
{
	return m_pConnectorPool->GetConnectorForGroupMode(pid);
}

uint32 ConnectorManager::GetConnectorCount()
{
	return m_uConnectorCount;
}

BOOL ConnectorManager::SendMessage(IMessage *pKMessage, ut_id_connector ServerID, uint32 Flag)
{
	__GUARD__ 
		
	lock_guard<FastLock> autolock(m_Lock);
	
	if(m_MessageQue[m_Tail].m_pMessage)
	{
		BOOL ret = ResizeCache();
		KCheck(ret);
	}

	m_MessageQue[m_Tail].m_pMessage = pKMessage;
	m_MessageQue[m_Tail].m_PlayerID = ServerID;
	m_MessageQue[m_Tail].m_Flag = Flag;

	m_Tail++;
	if(m_Tail >= m_QueSize) 
		m_Tail = 0;

	return TRUE;
	__UNGUARD__ 
	return FALSE;
}

BOOL ConnectorManager::Handle_CacheCommands()
{
	__GUARD__ 
		
	BOOL ret = FALSE;
	uint32 uret;

	for(uint32 _Idx = 0; _Idx < m_QueSize; _Idx++)
	{
		IMessage*		pKMessage = NULL;
		ut_id_connector	ConnectorID;
		uint32			Flag;
		BOOL			bNeedRemove = TRUE;

		ret = RecvMessage(pKMessage, ConnectorID, Flag);
		if( !ret )
			break;

		if( pKMessage == NULL )
		{
			QLOGGER_CODEHOLDER();
			break;
		}

		if( pKMessage == NULL )
		{
			QLOGGER_CODEHOLDER();
			break;
		}

		if(Flag == PF_REMOVE)
		{
			DESTROY_MESSAGE_OBJ(pKMessage);
			continue;
		}

		if( (ConnectorID==INVALID_KHAN_ID_EX) || (ConnectorID == INVALID_KHAN_ID) )
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

			if(uret == MP_ERROR)
			{
			}
			else if(uret == MP_BREAK)
			{
			}
			else if(uret == MP_CONTINUE)
			{
			}
			else if(uret == MP_NOTREMOVE)
			{
				bNeedRemove = FALSE;
			}
			else if(uret == MP_NOTREMOVE_ERROR)
			{
				bNeedRemove = FALSE;
			}
		}
		else
		{
			_NET_TRY
			{
				Connector *pConnector = GetConnector(ConnectorID);
				if( pConnector != NULL )
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

					if(uret == MP_ERROR)
					{
						RemoveConnector(pConnector);
					}
					else if(uret == MP_BREAK)
					{
					}
					else if(uret == MP_CONTINUE)
					{
					}
					else if(uret == MP_NOTREMOVE)
					{
						bNeedRemove = FALSE;
					}
					else if(uret == MP_NOTREMOVE_ERROR)
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
					KCheck(FALSE);
				}
			}
			_NET_CATCH
			{
				QLOGGER_CODEHOLDER();
			}
		}

		if(bNeedRemove) 
			DESTROY_MESSAGE_OBJ(pKMessage);
	}

	return TRUE;
	__UNGUARD__ 
	return FALSE;
}

BOOL ConnectorManager::RecvMessage(IMessage * &pKMessage, ut_id_connector &ConnectorID, uint32 &Flag)
{
	__GUARD__ 
		
	lock_guard<FastLock> autolock(m_Lock);

	if(m_MessageQue[m_Head].m_pMessage == NULL)
	{
		return FALSE;
	}

	pKMessage = m_MessageQue[m_Head].m_pMessage;
	ConnectorID = m_MessageQue[m_Head].m_PlayerID;
	Flag = m_MessageQue[m_Head].m_Flag;

	m_MessageQue[m_Head].m_pMessage = NULL;
	m_MessageQue[m_Head].m_PlayerID = INVALID_KHAN_ID;
	m_MessageQue[m_Head].m_Flag = PF_NONE;

	m_Head++;
	if(m_Head >= m_QueSize) m_Head = 0;

	return TRUE;
	__UNGUARD__ 
	return FALSE;
}

BOOL ConnectorManager::ResizeCache()
{
	__GUARD__ 
		
	ASYNC_MSG* pNew = new ASYNC_MSG[m_QueSize + MAX_CACHE_SIZE];
	if(pNew == NULL) 
		return FALSE;

	if(m_Head < m_Tail)
		memcpy(pNew, &(m_MessageQue[m_Head]), sizeof(ASYNC_MSG) * (m_Tail - m_Head));
	else if(m_Head >= m_Tail)
	{
		memcpy(pNew, &(m_MessageQue[m_Head]), sizeof(ASYNC_MSG) * (m_QueSize - m_Head));
		memcpy(&pNew[m_QueSize - m_Head], m_MessageQue, sizeof(ASYNC_MSG) * m_Tail);
	}

	memset(m_MessageQue, 0, sizeof(ASYNC_MSG) * m_QueSize);
	KSafeDelete_ARRAY(m_MessageQue);
	m_MessageQue = pNew;

	m_Head = 0;
	m_Tail = m_QueSize;
	m_QueSize = m_QueSize + MAX_CACHE_SIZE;

	QLogSystem::QUICK_LOG
		(
			QUICK_LOG_CONNECTOR,
			"ConnectorManager::ResizeCache() From=%d, To=%d",
			m_QueSize - MAX_CACHE_SIZE,
			m_QueSize
		);

	return TRUE;
	__UNGUARD__ 
	return FALSE;
}

BOOL ConnectorManager::MoveMessage(ut_id_connector ConnectorID)
{
	__GUARD__ 
		
	lock_guard<FastLock>	autolock(m_Lock);
	uint32					Cur = m_Head;
	
	for(uint32 _Idx = 0; _Idx < m_QueSize; _Idx++)
	{
		if(m_MessageQue[Cur].m_pMessage == NULL) 
			break;

		if(m_MessageQue[Cur].m_PlayerID == ConnectorID)
		{
			m_MessageQue[Cur].m_Flag = PF_REMOVE;
		}

		Cur++;
		if(Cur >= m_QueSize) 
			Cur = 0;
	}

	return TRUE;
	__UNGUARD__ 
	return FALSE;
}

Connector* ConnectorManager::RecvNewConnector()
{
	__GUARD__ 

	if( m_bServerMode == FALSE )
		return NULL;
		
	int32		iStep = 0;
	BOOL		ret = FALSE;
	BOOL		bRemove = FALSE;
	Connector*	client = NULL;

	_NET_TRY
	{
		//Fixme： 问题登录的时候会走两遍，第一次成功了，第二次失败了 
		client = m_pConnectorPool->NewConnector();
		if( client == NULL )
		{
			QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager::RecvNewConnector(): Fail ! New Connector Fail" );
			return NULL;
		}

		ret = m_pServerSocket->accept(client->GetSocket());
		if(!ret)
		{
			m_pConnectorPool->DelConnector(client->GetConnectorID());
			return NULL;
		}
	}
	_NET_CATCH
	{
		iStep += 1000;
		goto EXCEPTION;
	}

	_NET_TRY
	{
		iStep = 3;
		if( client->GetSocket()->getSOCKET() == INVALID_SOCKET )
		{
			goto EXCEPTION;
		}

		iStep = 4;
		ret = client->GetSocket()->setNonBlocking();
		if(!ret)
		{
			goto EXCEPTION;
		}

		iStep = 5;
		if(client->GetSocket()->getSockError())
		{
			goto EXCEPTION;
		}

		iStep = 6;
		ret = client->GetSocket()->setLinger(0);
		if(!ret)
		{
			goto EXCEPTION;
		}

		iStep = 7;
		_NET_TRY
		{
			ret = AddConnector(client);
			if(!ret)
			{
				goto EXCEPTION;
			}
			bRemove = TRUE;
		}
		_NET_CATCH
		{
			iStep += 10000;
			goto EXCEPTION;
		}
	}
	_NET_CATCH
	{
		iStep += 100000;
	}

	QLogSystem::QUICK_LOG
		(
			QUICK_LOG_CONNECTOR,
			"ConnectorManager::RecvNewConnector(): Success ! sock=%d,pid=%d,ip=%s",
			client->GetSocket()->getSOCKET(),
			client->GetConnectorID(),
			client->GetSocket()->m_IP
		);

	return client;

EXCEPTION:

	QLogSystem::QUICK_LOG
		(
			QUICK_LOG_CONNECTOR,
			"ConnectorManager::RecvNewConnector(): Fail ! step=%d,sock=%d,pid=%d,ip=%s",
			iStep,
			client->GetSocket()->getSOCKET(),
			client->GetConnectorID(),
			client->GetSocket()->m_IP
		);

	if( client != NULL )
	{
		if( bRemove )
			RemoveConnector(client);
		else
			m_pConnectorPool->DelConnector(client->GetConnectorID());
	}

	return NULL;
	__UNGUARD__ 
	return NULL;
}

BOOL ConnectorManager::CheckAndReconnectToServer()
{
	__GUARD__;
	return TRUE;
	__UNGUARD__ return FALSE;
}

const char* ConnectorManager::GetManagerName()
{
	return "";
}