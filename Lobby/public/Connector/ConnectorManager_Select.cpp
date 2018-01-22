#include "stdafx.h"
#include "ConnectorManager_Select.h"

#define ACCEPT_ONESTEP	50

ConnectorManager_Select::ConnectorManager_Select()
{
	FD_ZERO(&m_ReadFDs[SELECT_BAK]);
	FD_ZERO(&m_WriteFDs[SELECT_BAK]);
	FD_ZERO(&m_ExceptFDs[SELECT_BAK]);
	FD_ZERO(&m_ReadFDs[SELECT_USE]);
	FD_ZERO(&m_WriteFDs[SELECT_USE]);
	FD_ZERO(&m_ExceptFDs[SELECT_USE]);

	m_Timeout[SELECT_BAK].tv_sec = 0;
	m_Timeout[SELECT_BAK].tv_usec = 0;

	m_MinFD = m_MaxFD = INVALID_SOCKET;
}

ConnectorManager_Select::~ConnectorManager_Select()
{
	__GUARD__
	KSafeDelete(m_pServerSocket);
	__UNGUARD__
}

BOOL ConnectorManager_Select::Init(uint32 uMaxCount,const char* ip,uint16 port, uint32 uBackLog)
{
	__GUARD__	
		
	if( m_bServerMode )
	{
		KCheck(ip!=NULL);
		KCheck(port!=0);
		m_pServerSocket = new Acceptor(port, ip, uBackLog);
		KCheck(m_pServerSocket);
		m_pServerSocket->setNonBlocking();

		m_SocketID = m_pServerSocket->getSOCKET();
		KCheck(m_SocketID != INVALID_SOCKET);

		FD_SET(m_SocketID, &m_ReadFDs[SELECT_BAK]);
		FD_SET(m_SocketID, &m_ExceptFDs[SELECT_BAK]);

		m_MinFD = m_MaxFD = m_SocketID;
	}

	return ConnectorManager::Init(uMaxCount,ip,port);

	__UNGUARD__ 
	return FALSE;
}

BOOL ConnectorManager_Select::AddConnectorSocket(SOCKET fd,ut_id_connector pid)
{
	__GUARD__

	if(fd == INVALID_SOCKET)
	{
		QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager_Select::AddConnectorSocket(): Error ! fd==INVALID pid=%d mgr='%s'", pid, GetManagerName() );
		return FALSE;
	}

	if( m_uFDSize >= FD_SETSIZE )
	{
		QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager_Select::AddConnectorSocket(): Error ! m_uFDSize(%d)>FD_SETSIZE pid=%d mgr='%s'", m_uFDSize, pid, GetManagerName() );
		return FALSE;
	}

	if(FD_ISSET(fd, &m_ReadFDs[SELECT_BAK]))
	{
		QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager_Select::AddConnectorSocket(): Error ! Already Include fd %d pid=%d mgr='%s'", fd, pid, GetManagerName() );
		return FALSE;
	}

	m_MinFD = (m_MinFD == INVALID_KHAN_ID) ? fd : std::min(fd, m_MinFD);
	m_MaxFD = (m_MaxFD == INVALID_KHAN_ID) ? fd : std::max(fd, m_MaxFD);

	FD_SET(fd, &m_ReadFDs[SELECT_BAK]);
	FD_SET(fd, &m_WriteFDs[SELECT_BAK]);
	FD_SET(fd, &m_ExceptFDs[SELECT_BAK]);

	m_uFDSize++;

	return TRUE;
	__UNGUARD__
	return FALSE;
}

BOOL ConnectorManager_Select::DelConnectorSocket(SOCKET fd, ut_id_connector pid)
{
	__GUARD__

	if(fd == INVALID_SOCKET)
	{
		QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager_Select::DelConnectorSocket(): Error ! fd==INVALID pid=%d mgr='%s'", pid, GetManagerName() );
		return FALSE;
	}

	if( m_MinFD==INVALID_SOCKET || m_MaxFD==INVALID_SOCKET )
	{
		QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager_Select::DelConnectorSocket(): Error ! minFD=%d,maxFD=%d pid=%d mgr='%s'", m_MinFD, m_MaxFD, pid, GetManagerName() );
		return FALSE;
	}

	if( m_uFDSize <= 0 )
	{
		QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager_Select::DelConnectorSocket(): Error ! fdSize(%d)<=0 pid=%d mgr='%s'", m_uFDSize, pid, GetManagerName() );
		return FALSE;
	}

	if(fd == m_MinFD)
	{
		SOCKET	MinFD = m_MaxFD;
		uint32	uConnectorCount = GetConnectorCount();
		for(uint32 _Idx = 0; _Idx < uConnectorCount; _Idx++)
		{
			if(m_aConnecorID[_Idx] == INVALID_KHAN_ID) 
				continue;

			Connector *pConn = GetConnector(m_aConnecorID[_Idx]);
			if(pConn == NULL)
				continue;

			SOCKET sockid = pConn->GetSocket()->getSOCKET();			
			if(sockid == fd) 
				continue;
			if(sockid == INVALID_SOCKET) 
				continue;

			if(sockid < MinFD)
				MinFD = sockid;
		}

		if(m_MinFD == m_MaxFD)
		{
			m_MinFD = m_MaxFD = INVALID_SOCKET;
		}
		else
		{
			if( (m_SocketID!=INVALID_SOCKET) && (MinFD>m_SocketID) )
			{
				m_MinFD = m_SocketID;
			}
			else
			{
				m_MinFD = MinFD;
			}
		}
	}
	else if(fd == m_MaxFD)
	{
		SOCKET	MaxFD = m_MinFD;
		uint32	uConnectorCount = GetConnectorCount();
		for(uint32 _Idx = 0; _Idx < uConnectorCount; _Idx++)
		{
			if(m_aConnecorID[_Idx] == INVALID_KHAN_ID) 
				continue;

			Connector *pConn = GetConnector(m_aConnecorID[_Idx]);
			if(pConn == NULL)
				continue;

			SOCKET sockid = pConn->GetSocket()->getSOCKET();	
			if(sockid == fd) 
				continue;
			if(sockid == INVALID_SOCKET) 
				continue;

			if(sockid > MaxFD)
				MaxFD = sockid;
		}

		if(m_MaxFD == m_MinFD)
		{
			m_MinFD = m_MaxFD = INVALID_SOCKET;
		}
		else
		{
			if( (m_SocketID!=INVALID_SOCKET) && (MaxFD<m_SocketID) )
			{
				m_MaxFD = m_SocketID;
			}
			else
			{
				m_MaxFD = MaxFD;
			}
		}
	}

	FD_CLR(fd, &m_ReadFDs[SELECT_BAK]);
	FD_CLR(fd, &m_ReadFDs[SELECT_USE]);
	FD_CLR(fd, &m_WriteFDs[SELECT_BAK]);
	FD_CLR(fd, &m_WriteFDs[SELECT_USE]);
	FD_CLR(fd, &m_ExceptFDs[SELECT_BAK]);
	FD_CLR(fd, &m_ExceptFDs[SELECT_USE]);

	m_uFDSize--;

	return TRUE;
	__UNGUARD__
	return FALSE;
}


BOOL ConnectorManager_Select::Select()
{
	__GUARD__

	if(m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET)
		return TRUE;
		
	m_Timeout[SELECT_USE].tv_sec = m_Timeout[SELECT_BAK].tv_sec;
	m_Timeout[SELECT_USE].tv_usec = m_Timeout[SELECT_BAK].tv_usec;

	m_ReadFDs[SELECT_USE] = m_ReadFDs[SELECT_BAK];
	m_WriteFDs[SELECT_USE] = m_WriteFDs[SELECT_BAK];
	m_ExceptFDs[SELECT_USE] = m_ExceptFDs[SELECT_BAK];

	_NET_TRY
	{
		int32	iRet = SOCKET_BASIC::select_ex
			(
				(int32) m_MaxFD + 1,
				&m_ReadFDs[SELECT_USE],
				&m_WriteFDs[SELECT_USE],
				&m_ExceptFDs[SELECT_USE],
				&m_Timeout[SELECT_USE]
			);

		KCheck(iRet != SOCKET_ERROR);
	}
	_NET_CATCH
	{
		QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager_Select::Select(): Error !" );
	}

	return TRUE;
	__UNGUARD__
	return FALSE;
}

BOOL ConnectorManager_Select::Handle_Inputs()
{
	__GUARD__
	
	BOOL ret = FALSE;

	if(m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET)
		return TRUE;

	if( m_bServerMode )
	{
		if(FD_ISSET(m_SocketID, &m_ReadFDs[SELECT_USE]))
		{
			for(int32 _Idx = 0; _Idx < ACCEPT_ONESTEP; _Idx++)
			{
				if(RecvNewConnector()==NULL) 
					break;
			}
		}
	}

	uint32	uConnectorCount = GetConnectorCount();
	for(uint32 _Idx = 0; _Idx < uConnectorCount; _Idx++)
	{
		if(m_aConnecorID[_Idx] == INVALID_KHAN_ID)
		{
			QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager_Select::ProcessInputs(): Error ! m_aConnecorID[%d] == INVALID_ID", _Idx );
			DelInvalidConnectorFromMgr(_Idx);
			_Idx--;
			uConnectorCount = GetConnectorCount();
			continue;
		}

		Connector *pConnector = GetConnector(m_aConnecorID[_Idx]);
		if( pConnector == NULL )
		{
			QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager_Select::ProcessInputs(): Error ! pConnector==NULL,id=%d", m_aConnecorID[_Idx] );
			DelInvalidConnectorFromMgr(_Idx);
			_Idx--;
			uConnectorCount = GetConnectorCount();
			continue;
		}

		if( pConnector->IsDisconnect() )
			continue;

		SOCKET s = pConnector->GetSocket()->getSOCKET();
		if(s == INVALID_SOCKET)
		{
			QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager_Select::ProcessOutputs(): Error ! socket==INVALID" );
			continue;
		}

		if(s == m_SocketID)
			continue;

		if(FD_ISSET(s, &m_ReadFDs[SELECT_USE]))
		{
			if(pConnector->GetSocket()->isSockError())
			{
				RemoveConnector(pConnector);
			}
			else
			{
				_NET_TRY
				{
					ret = pConnector->Handle_Input();
					if(!ret)
					{
						RemoveConnector(pConnector);
					}
				}
				_NET_CATCH
				{
					RemoveConnector(pConnector);
				}
			}
		}

		// 如果连接删除了，则总数也应该减少 [yangzhen]
		if(uConnectorCount > GetConnectorCount())
		{
			// Connector的处理函数中只能删除自己，不可能删除别的Connector [yangzhen]
			KCheck((uConnectorCount - GetConnectorCount()) == 1);
			_Idx--;
			uConnectorCount = GetConnectorCount();
		}
	}

	return TRUE;
	__UNGUARD__
	return FALSE;
}

BOOL ConnectorManager_Select::Handle_Outputs()
{
	__GUARD__
	
	BOOL ret = FALSE;

	if(m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET)
		return TRUE;

	uint32	uConnectorCount = GetConnectorCount();
	for(uint32 _Idx = 0; _Idx < uConnectorCount; _Idx++)
	{
		if(m_aConnecorID[_Idx] == INVALID_KHAN_ID)
		{
			QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager_Select::ProcessOutputs(): Error ! m_aConnecorID[%d] == INVALID_ID", _Idx );
			DelInvalidConnectorFromMgr(_Idx);
			// 每次清理一个，清理后需要从新执行，所欲做_Idx--处理 [xingzhishan]
			_Idx--;
			uConnectorCount = GetConnectorCount();
			continue;
		}

		Connector *pConnector = GetConnector(m_aConnecorID[_Idx]);
		if( pConnector == NULL )
		{
			QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager_Select::ProcessOutputs(): Error ! pConnector==NULL,id=%d", m_aConnecorID[_Idx] );
			DelInvalidConnectorFromMgr(_Idx);
			_Idx--;
			uConnectorCount = GetConnectorCount();
			continue;
		}

		if( pConnector->IsDisconnect() )
			continue;

		SOCKET s = pConnector->GetSocket()->getSOCKET();
		if(s == INVALID_SOCKET)
		{
			QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager_Select::ProcessOutputs(): Error ! socket==INVALID" );
			continue;
		}

		if(s == m_SocketID) 
			continue;

		if(FD_ISSET(s, &m_WriteFDs[SELECT_USE]))
		{
			if(pConnector->GetSocket()->isSockError())
			{
				RemoveConnector(pConnector);
			}
			else
			{
				_NET_TRY
				{
					ret = pConnector->Handle_Output();
					if(!ret)
					{
						RemoveConnector(pConnector);
					}
				}
				_NET_CATCH
				{
					RemoveConnector(pConnector);
				}
			}
		}

		// 如果连接删除了，则总数也应该减少 [yangzhen]
		if(uConnectorCount > GetConnectorCount())
		{
			// Connector的处理函数中只能删除自己，不可能删除别的Connector [yangzhen]
			KCheck((uConnectorCount - GetConnectorCount()) == 1);
			_Idx--;
			uConnectorCount = GetConnectorCount();
		}
	}

	return TRUE;

	__UNGUARD__ return FALSE;
}

BOOL ConnectorManager_Select::Handle_Exceptions()
{
	__GUARD__ 
		
	if(m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET)
		return TRUE;

	uint32 uConnectorCount = GetConnectorCount();
	for(uint32 _Idx = 0; _Idx < uConnectorCount; _Idx++)
	{
		if(m_aConnecorID[_Idx] == INVALID_KHAN_ID) 
		{
			QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager_Select::Procs_Exceptions(): Error ! m_aConnecorID[%d] == INVALID_ID", _Idx );
			DelInvalidConnectorFromMgr(_Idx);
			_Idx--;
			uConnectorCount = GetConnectorCount();
			continue;
		}

		Connector* pConnector = GetConnector(m_aConnecorID[_Idx]);
		if( pConnector == NULL )
		{
			QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager_Select::Procs_Exceptions(): Error ! pConnector==NULL,id=%d", m_aConnecorID[_Idx] );
			DelInvalidConnectorFromMgr(_Idx);
			_Idx--;
			uConnectorCount = GetConnectorCount();
			continue;
		}

		if( pConnector->IsDisconnect() )
			continue;

		SOCKET s = pConnector->GetSocket()->getSOCKET();
		if(s == m_SocketID)
		{
			continue;
		}

		if(FD_ISSET(s, &m_ExceptFDs[SELECT_USE]))
		{
			RemoveConnector(pConnector);
		}

		// 如果连接删除了，则总数也应该减少 [yangzhen]
		if(uConnectorCount > GetConnectorCount())
		{
			// Connector的处理函数中只能删除自己，不可能删除别的Connector [yangzhen]
			KCheck((uConnectorCount - GetConnectorCount()) == 1);
			_Idx--;
			uConnectorCount = GetConnectorCount();
		}
	}

	return TRUE;
	__UNGUARD__ 
	return FALSE;
}

BOOL ConnectorManager_Select::Handle_Commands()
{
	__GUARD__ 
		
	BOOL ret;

	if(m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET)
		return TRUE;

	uint32 uConnectorCount = GetConnectorCount();
	for(uint32 _Idx = 0; _Idx < uConnectorCount; _Idx++)
	{
		if(m_aConnecorID[_Idx] == INVALID_KHAN_ID) 
		{
			QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager_Select::Handle_Commands(): Error ! m_aConnecorID[%d] == INVALID_ID", _Idx );
			DelInvalidConnectorFromMgr(_Idx);
			_Idx--;
			uConnectorCount = GetConnectorCount();
			continue;
		}

		Connector* pConnector = GetConnector(m_aConnecorID[_Idx]);
		if( pConnector == NULL )
		{
			QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager_Select::Handle_Commands(): Error ! pConnector==NULL,id=%d", m_aConnecorID[_Idx] );
			DelInvalidConnectorFromMgr(_Idx);
			_Idx--;
			uConnectorCount = GetConnectorCount();
			continue;
		}

		if( pConnector->IsDisconnect() )
			continue;

		SOCKET s = pConnector->GetSocket()->getSOCKET();
		if(s == INVALID_SOCKET)
		{
			QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager_Select::ProcessOutputs(): Error ! socket==INVALID" );
			continue;
		}

		if(s == m_SocketID) 
			continue;

		if(pConnector->GetSocket()->isSockError())
		{
			RemoveConnector(pConnector);
		}
		else
		{
			_NET_TRY
			{
				ret = pConnector->Handle_Command();
				if(!ret)
				{
					if( pConnector->IsGameConnector() )
						RemoveConnector(pConnector);
					else
						QLOGGER_CODEHOLDER();
				}
			}
			_NET_CATCH
			{
				if( pConnector->IsGameConnector() )
					RemoveConnector(pConnector);
				else
					QLOGGER_CODEHOLDER();
			}
		}

		// 如果连接删除了，则总数也应该减少 [yangzhen]
		if(uConnectorCount > GetConnectorCount())
		{
			// Connector的处理函数中只能删除自己，不可能删除别的Connector [yangzhen]
			KCheck((uConnectorCount - GetConnectorCount()) == 1);
			_Idx--;
			uConnectorCount = GetConnectorCount();
		}
	}

	return TRUE;
	__UNGUARD__
	return FALSE;
}

