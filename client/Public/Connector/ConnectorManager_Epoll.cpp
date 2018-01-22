#include "stdafx.h"
#include "ConnectorManager_Epoll.h"

#ifdef __EPOLL__

#include <errno.h>
#include <unistd.h>
#define ACCEPT_ONESTEP			50

ConnectorManager_Epoll::ConnectorManager_Epoll()
{
	m_uEventsMaxSize = 0;
	events = NULL;
	m_nEvents = 0;	
}

ConnectorManager_Epoll::~ConnectorManager_Epoll()
{
	KSafeDelete(m_pServerSocket);
	KSafeDelete_ARRAY(events);
	close(m_Epollfd);
}

BOOL ConnectorManager_Epoll::Init(uint32 uMaxCount,const char* ip,uint16 port, uint32 uBackLog)
{
	__GUARD__ 

	if( m_bServerMode )
	{
		m_pServerSocket = new Acceptor(port, ip, uBackLog);
		KCheck(m_pServerSocket);

		m_pServerSocket->setNonBlocking();

		m_SocketID = m_pServerSocket->getSOCKET();
		KCheck(m_SocketID != INVALID_SOCKET);

		struct epoll_event	ev;
		ev.data.u64 = ToUint64( (uint32)m_SocketID, INVALID_KHAN_ID);
		ev.events = EPOLLIN;
		epoll_ctl(m_Epollfd, EPOLL_CTL_ADD, m_SocketID, &ev);
	}

	return ConnectorManager::Init(uMaxCount,ip,port);

	__UNGUARD__
	return FALSE;
}

void ConnectorManager_Epoll::SetEpollMaxSize(uint32 uMaxSize)
{
	events = new struct epoll_event[uMaxSize];
	KCheck(events);
	m_Epollfd = epoll_create(uMaxSize);
	m_uEventsMaxSize = uMaxSize;
}


BOOL ConnectorManager_Epoll::AddConnectorSocket(SOCKET fd,ut_id_connector pid)
{
	__GUARD__

	if(fd == INVALID_SOCKET)
	{
		QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager_Epoll::AddConnectorSocket(): Error ! fd==INVALID pid=%d mgr='%s'", pid, GetManagerName() );
		return FALSE;
	}

	if( m_uFDSize >= m_uEventsMaxSize )
	{
		QLogSystem::QUICK_LOG
			( 
				QUICK_LOG_CONNECTOR,
				"ConnectorManager_Epoll::AddConnectorSocket(): Error ! m_uFDSize(%d)>m_uEventsMaxSize(%d) pid=%d mgr='%s'", 
				m_uFDSize,
				m_uEventsMaxSize,
				pid, GetManagerName()
			);
		return FALSE;
	}

	struct epoll_event ev;
	ev.data.u64 = ToUint64( (uint32)fd, (uint32)pid );
	ev.events = EPOLLIN | EPOLLET;
	if( 0 != epoll_ctl(m_Epollfd, EPOLL_CTL_ADD, fd, &ev) )
	{
		int err = errno;
		QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager_Epoll::AddConnectorSocket(): epoll_ctl Error %s! pid=%d mgr='%s'", strerror(err), pid, GetManagerName() );
		return FALSE;
	}

	m_uFDSize++;

	return TRUE;
	__UNGUARD__
	return FALSE;
}

BOOL ConnectorManager_Epoll::DelConnectorSocket(SOCKET fd, ut_id_connector pid)
{
	__GUARD__

	if(fd == INVALID_SOCKET)
	{
		QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager_Epoll::DelConnectorSocket(): Error ! fd==INVALID pid=%d mgr='%s'", pid, GetManagerName() );
		return FALSE;
	}

	if( m_uFDSize<=0 )
	{
		QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager_Epoll::DelConnectorSocket(): Error ! m_uFDSize(%d)<=0 pid=%d mgr='%s'", m_uFDSize, pid, GetManagerName() );
		return FALSE;
	}

	struct epoll_event	ev;
	memset(&ev, 0, sizeof(epoll_event));
	ev.data.fd = fd;
	ev.events = EPOLLIN | EPOLLET;
	if( 0 != epoll_ctl(m_Epollfd, EPOLL_CTL_DEL, ev.data.fd, &ev) )
	{
		int err = errno;
		QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager_Epoll::DelConnectorSocket(): epoll_ctl Error ! m_uFDSize(%d)<=0, error=%d, %s, pid=%d mgr='%s'", m_uFDSize, err, strerror(err), pid, GetManagerName() );
		return FALSE;
	}
	
	m_uFDSize--;

	return TRUE;
	__UNGUARD__
	return FALSE;
}


BOOL ConnectorManager_Epoll::Select()
{
	__GUARD__	
		
	_NET_TRY
	{
		m_nEvents = epoll_wait(m_Epollfd, events, m_uEventsMaxSize, 0);
		if(m_nEvents > m_uEventsMaxSize || m_nEvents < 0)
		{
			char szMsg[128] = { 0 };
			tsnprintf_s(szMsg, 128, "Epoll Select Error ! m_nEvents=%d", m_nEvents);
			KCheckEx(FALSE, szMsg);
		}
	}
	_NET_CATCH
	{
		QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager_Epoll::Select(): Error ! m_nEvents=%d", m_nEvents );
	}

	return TRUE;
	__UNGUARD__ 
	return FALSE;
}

BOOL ConnectorManager_Epoll::Handle_Inputs()
{
	__GUARD__
		
	BOOL ret = FALSE;

	for(uint32 _Idx = 0; _Idx < m_nEvents; _Idx++)
	{
		int32 SockFD = GetHighSection(events[_Idx].data.u64);
		ut_id_connector PID = GetLowSection(events[_Idx].data.u64);

		if(SockFD == m_SocketID)
		{
			if( m_bServerMode )
			{
				for(int32 _Idx = 0; _Idx < ACCEPT_ONESTEP; _Idx++)
				{
					if(RecvNewConnector()==NULL) 
						break;
				}
			}
		}
		else if( events[_Idx].events & EPOLLIN )
		{
			Connector* pConnector = GetConnector(PID);
			if( pConnector == NULL )
			{
				QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager_Epoll::ProcessInputs(): Error ! pConnector==NULL,id=%d", PID );
				continue;
			}

			if( pConnector->IsDisconnect() )
				continue;

			SOCKET s = pConnector->GetSocket()->getSOCKET();
			if(s == INVALID_SOCKET)
			{
				QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager_Epoll::ProcessInputs(): Error ! socket==INVALID" );
				continue;
			}

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
	}

	return TRUE;
	__UNGUARD__ 
	return FALSE;
}

BOOL ConnectorManager_Epoll::Handle_Outputs()
{
	__GUARD__ 
		
	BOOL	ret = FALSE;
	uint32	uConnectorCount = GetConnectorCount();

	for(uint32 _Idx = 0; _Idx < uConnectorCount; _Idx++)
	{
		if(m_aConnecorID[_Idx] == INVALID_KHAN_ID) 
		{
			QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager_Epoll::ProcessOutputs(): Error ! m_aConnecorID[%d] == INVALID_ID", _Idx );
			DelInvalidConnectorFromMgr(_Idx);
			_Idx--;
			uConnectorCount = GetConnectorCount();
			continue;
		}

		Connector* pConnector = GetConnector(m_aConnecorID[_Idx]);
		if( pConnector == NULL )
		{
			QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager_Epoll::ProcessOutputs(): Error ! pConnector==NULL,id=%d", m_aConnecorID[_Idx] );
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
			QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager_Epoll::ProcessOutputs(): Error ! socket==INVALID" );
			continue;
		}

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

BOOL ConnectorManager_Epoll::Handle_Exceptions()
{
	__GUARD__ 
		
	return TRUE;
	__UNGUARD__ 
	return FALSE;
}

BOOL ConnectorManager_Epoll::Handle_Commands()
{
	__GUARD__ 
		
	BOOL	ret = FALSE;
	uint32	uConnectorCount = GetConnectorCount();

	for(uint32 _Idx = 0; _Idx < uConnectorCount; _Idx++)
	{
		if(m_aConnecorID[_Idx] == INVALID_KHAN_ID) 
		{
			QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager_Epoll::Handle_Commands(): Error ! m_aConnecorID[%d] == INVALID_ID", _Idx );
			DelInvalidConnectorFromMgr(_Idx);
			_Idx--;
			uConnectorCount = GetConnectorCount();
			continue;
		}

		Connector *pConnector = GetConnector(m_aConnecorID[_Idx]);
		if( pConnector == NULL )
		{
			QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager_Epoll::Handle_Commands(): Error ! pConnector==NULL,id=%d", m_aConnecorID[_Idx] );
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
			QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorManager_Epoll::Handle_Commands(): Error ! socket==INVALID" );
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

#endif
