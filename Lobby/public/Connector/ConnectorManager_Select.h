#ifndef __CONNECTOR_MANAGER_SELECT_H__
#define __CONNECTOR_MANAGER_SELECT_H__

#include "ConnectorManager.h"

enum { SELECT_BAK = 0, SELECT_USE = 1, SELECT_MAX = 2, };

class ConnectorManager_Select : public ConnectorManager
{
public:
	ConnectorManager_Select();
	virtual ~ConnectorManager_Select();

	virtual BOOL		Init(uint32 uMaxCount,const char* ip=NULL,uint16 port=0, uint32 uBackLog = 5);

	virtual BOOL		AddConnectorSocket(SOCKET fd,ut_id_connector pid);
	virtual BOOL		DelConnectorSocket(SOCKET fd, ut_id_connector pid);

	virtual BOOL		Select();
	virtual BOOL		Handle_Inputs();
	virtual BOOL		Handle_Outputs();
	virtual BOOL		Handle_Exceptions();
	virtual BOOL		Handle_Commands();

protected:
	// ¿Õº¯Êý£¬¼æÈÝÓï·¨
	void				SetEpollMaxSize(uint32 uMaxSize) {}

protected:
	fd_set				m_ReadFDs[SELECT_MAX];
	fd_set				m_WriteFDs[SELECT_MAX];
	fd_set				m_ExceptFDs[SELECT_MAX];
	timeval				m_Timeout[SELECT_MAX];
	SOCKET				m_MinFD;
	SOCKET				m_MaxFD;
};

#endif
