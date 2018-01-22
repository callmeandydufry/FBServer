#ifndef __SERVER_CONNECTOR_MANAGER_H__
#define __SERVER_CONNECTOR_MANAGER_H__

#ifdef __EPOLL__
#include "ConnectorManager_Epoll.h"
#else
#include "ConnectorManager_Select.h"
#endif
#include "ServerConnector.h"

struct WORLDID_HASH
{
	ut_id_connector	connID;
	ut_id			WorldID;

	WORLDID_HASH()
	{
		connID = INVALID_KHAN_ID;
		WorldID = INVALID_KHAN_ID;
	}
};

#ifdef __EPOLL__
class ServerConnectorManager : public ConnectorManager_Epoll
#else
class ServerConnectorManager : public ConnectorManager_Select
#endif
{
public:
	ServerConnectorManager();
	virtual ~ServerConnectorManager();

	virtual BOOL		RemoveConnector(Connector *pConnector);

	virtual ut_id		GetCurServerID();
	SERVER_DATA*		GetCurServerInfo();
	SERVER_DATA*		FindServerInfo(ut_id ServerID);

	ServerConnector*	GetServerConnector(ut_id ServerID);
	void				AddServerConnector(ut_id ServerID,ut_id_connector connectorID);
	void				RemoveServerConnector(ut_id ServerID);
	void				RemoveServerConnector(Connector* pConnector);

	void				BroadCast(IMessage *pKMessage);

	BOOL				ConnectToServer(const char* ip,uint16 port,ut_id serverid,ut_id_connector& connID,BOOL bCreate=TRUE,BOOL bSendSS=TRUE);
	Connector			*ConnectToServerForGroup(const char* ip,uint16 port,ut_id serverid,ut_id_connector& connID);
	
protected:
	ut_id_connector		m_aServerHash[SERVER_CAPACITY];
	SERVER_DATA*		m_p_CurServerInfo;
};

#endif
