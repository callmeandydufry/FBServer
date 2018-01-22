#ifndef __SERVER_CONNECTOR_H__
#define __SERVER_CONNECTOR_H__

#include "TypeDefine.h"
#include "Connector.h"
#include "Setting.h"

class ServerConnector : public Connector
{
public:
	ServerConnector()								{ m_pServerData = NULL; }
	virtual ~ServerConnector()						{ }

public:

#ifdef __OS64__
	virtual void	InitSocket()
	{
		m_pSocket = new Socket;
		KCheck(m_pSocket);

		m_pRecieveStream = new RecieveStream(m_pSocket, INITRECSIZE, 128 * 1024 * 1024);
		KCheck(m_pRecieveStream);

		m_pSendStream = new SendStream(m_pSocket, INITWRITESIZE, 128 * 1024 * 1024);
		KCheck(m_pSendStream);	
	}
#endif

	virtual BOOL	IsGameConnector()				{ return FALSE; }
	virtual BOOL	IsServerConnector()				{ return TRUE; }

	SERVER_DATA*	GetServerData()					{ return m_pServerData; }
	void			SetServerData(SERVER_DATA *pData){ m_pServerData = pData; }
	
	ut_id			GetServerID()					
	{ 
		if( m_pServerData )
			return m_pServerData->m_ServerID;
		else
			return INVALID_KHAN_ID;
	}

private:
	SERVER_DATA*	m_pServerData;
};

#endif
