#include "stdafx.h"
#include "ServerConnectorManager.h"
#include "SSHandShake.h"

ServerConnectorManager::ServerConnectorManager()
{
	m_p_CurServerInfo = NULL;

	for(int32 _Idx = 0; _Idx < SERVER_CAPACITY; _Idx++)
	{
		m_aServerHash[_Idx] = INVALID_KHAN_ID;
	}
}

ServerConnectorManager::~ServerConnectorManager()
{}


ut_id ServerConnectorManager::GetCurServerID()
{
	return g_Config.mConfigInfo.m_ServerID;
}

SERVER_DATA *ServerConnectorManager::GetCurServerInfo()
{
	__GUARD__ 
	
	if( m_p_CurServerInfo == NULL )
	{
		int32 iIndex = g_Config.mServerInfo.m_HashServer[GetCurServerID()];
		KCheck(iIndex >= 0 && iIndex < MAX_SERVER);
		m_p_CurServerInfo = &(g_Config.mServerInfo.m_pServer[iIndex]);
	}

	KCheck(m_p_CurServerInfo);
	return m_p_CurServerInfo;

	__UNGUARD__
	return NULL;
}

SERVER_DATA *ServerConnectorManager::FindServerInfo(ut_id ServerID)
{
	__GUARD__	
		
	KCheck(ServerID < SERVER_CAPACITY && ServerID >= 0);
	int32 iIndex = g_Config.mServerInfo.m_HashServer[ServerID];
	KCheck(iIndex >= 0 && iIndex < MAX_SERVER);
	
	return &(g_Config.mServerInfo.m_pServer[iIndex]);
	__UNGUARD__ 
	return NULL;
}

ServerConnector* ServerConnectorManager::GetServerConnector(ut_id ServerID)
{
	__GUARD__

	KCheck(ServerID < SERVER_CAPACITY);
	if (INVALID_KHAN_ID == ServerID)
		return NULL;

	ut_id_connector pid = m_aServerHash[ServerID];

	if(pid == INVALID_KHAN_ID) 
		return NULL;

	ServerConnector* pConnector = (ServerConnector*)GetConnector(pid);
	KCheck(pConnector);
	return pConnector;

	__UNGUARD__
	return NULL;
}

void ServerConnectorManager::AddServerConnector(ut_id ServerID,ut_id_connector connectorID)
{
	KCheck(ServerID >= 0 && ServerID < SERVER_CAPACITY);

	QLogSystem::QUICK_LOG
		(
			QUICK_LOG_CONNECTOR,
			"ServerConnectorManager::AddServerConnector(), ServerID=%d, connectorID=%d, m_aServerHash[ServerID]=%d",
			ServerID,
			connectorID,
			m_aServerHash[ServerID]
		);
	
	//KCheck( m_aServerHash[ServerID] == INVALID_KHAN_ID );
	m_aServerHash[ServerID] = connectorID;
}

void ServerConnectorManager::RemoveServerConnector(ut_id ServerID)
{
	KCheck(ServerID >= 0 && ServerID < SERVER_CAPACITY);
	
	QLogSystem::QUICK_LOG
		(
			QUICK_LOG_CONNECTOR,
			"ServerConnectorManager::RemoveServerConnector(), ServerID=%d, m_aServerHash[ServerID]=%d",
			ServerID,
			m_aServerHash[ServerID]
		);
	
	//KCheck( m_aServerHash[ServerID] != INVALID_KHAN_ID );
	m_aServerHash[ServerID] = INVALID_KHAN_ID;
}

void ServerConnectorManager::RemoveServerConnector(Connector* pConnector)
{
	ServerConnector* pServerConnector = (ServerConnector*)pConnector;
	KCheck(pServerConnector);
	SERVER_DATA	*pServerData = pServerConnector->GetServerData();
	if( pServerData )
	{
		ut_id ServerID = pServerConnector->GetServerData()->m_ServerID;
		RemoveServerConnector(ServerID);
	}
}


BOOL ServerConnectorManager::RemoveConnector(Connector *pConnector)
{
	__GUARD__	
		
	RemoveServerConnector(pConnector);

	return ConnectorManager::RemoveConnector(pConnector);

	__UNGUARD__ 
	return FALSE;
}


void ServerConnectorManager::BroadCast(IMessage *pKMessage)
{
	__GUARD__ 
		
	uint32 uConnectorCount = GetConnectorCount();
	for(uint32 _Idx=0; _Idx < uConnectorCount; _Idx++)
	{
		if(m_aConnecorID[_Idx] == INVALID_KHAN_ID) 
			continue;
		Connector *pConnector = GetConnector(m_aConnecorID[_Idx]);
		if(pConnector == NULL)
		{
			KCheck(FALSE);
			continue;
		}

		pConnector->SendMessage(pKMessage);
	}

	__UNGUARD__
}
	
// 2010-10-9 by rj 跨服线程连接方式
Connector *ServerConnectorManager::ConnectToServerForGroup(const char* ip,uint16 port,ut_id serverid,ut_id_connector& connID)
{
	__GUARD__

	int32				step = 0;
	BOOL				ret;
	uint32				_Idx = 0;
	BOOL				bRemove = FALSE;
	Connector*			pConnector = m_pConnectorPool->NewConnector(TRUE);

	if( pConnector == NULL )
	{
		return NULL;
	}

	Socket*				pSocket = pConnector->GetSocket();

	_NET_TRY
	{
		ret = pSocket->create();
		if (!ret)
		{
			step = 1;
			goto EXCEPTION;
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

	if( !ret)
	{
		step = 7;
		goto EXCEPTION;
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

	return pConnector;

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

	return NULL;
	__UNGUARD__
	return NULL;	
}

BOOL ServerConnectorManager::ConnectToServer(const char* ip,uint16 port,ut_id serverid,ut_id_connector& connID,BOOL bCreate/*=TRUE*/,BOOL bSendSS/*=TRUE*/)
{
	__GUARD__

	int32				step = 0;
	BOOL				ret;
	uint32				_Idx = 0;
	BOOL				bRemove = FALSE;
	SSHandShake*			pMsg = NULL;
	Connector*			pConnector = m_pConnectorPool->NewConnector(bCreate);
	if( pConnector == NULL )
		return FALSE;
	Socket*				pSocket = pConnector->GetSocket();

	_NET_TRY
	{
		if( bCreate )
		{
			ret = pSocket->create();
			if(!ret)
			{
				step = 1;
				goto EXCEPTION;
			}
		}

		ret = pSocket->connect(ip, port);
		if(!ret)
		{
			step = 2;
			goto EXCEPTION;
		}

		ret = pSocket->setNonBlocking();
		if(!ret)
		{
			step = 3;
			goto EXCEPTION;
		}

		ret = pSocket->setLinger(0);
		if(!ret)
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
	if(!ret)
	{
		step = 6;
		goto EXCEPTION;
	}
	bRemove = TRUE;

	if( bSendSS )
	{
		pMsg = new SSHandShake;
		pMsg->SetMapID(serverid);
		ret = pConnector->SendMessage(pMsg);
		KSafeDelete(pMsg);
		if(!ret)
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

	if( bRemove )
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
