#ifndef __CONNECTOR_MANAGER_EPOLL_H__
#define __CONNECTOR_MANAGER_EPOLL_H__

#include "ConnectorManager.h"

#ifdef __EPOLL__


class ConnectorManager_Epoll : public ConnectorManager
{
public:
	ConnectorManager_Epoll();
	virtual ~ConnectorManager_Epoll();

	virtual BOOL		Init(uint32 uMaxCount,const char* ip=NULL,uint16 port=0, uint32 uBackLog = 5);

	virtual BOOL		AddConnectorSocket(SOCKET fd,ut_id_connector pid);
	virtual BOOL		DelConnectorSocket(SOCKET fd, ut_id_connector pid);

	virtual BOOL		Select();
	virtual BOOL		Handle_Inputs();
	virtual BOOL		Handle_Outputs();
	virtual BOOL		Handle_Exceptions();
	virtual BOOL		Handle_Commands();

protected:
	void				SetEpollMaxSize(uint32 uMaxSize);

protected:
	int32				m_Epollfd;
	uint32				m_uEventsMaxSize;
	struct epoll_event*	events;
	int32				m_nEvents;
};

#endif

#endif
