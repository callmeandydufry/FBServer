#ifndef __SESSION_CONNECTOR_MANAGER_H__
#define __SESSION_CONNECTOR_MANAGER_H__

#ifdef __EPOLL__
#include "ConnectorManager_Epoll.h"
#else
#include "ConnectorManager_Select.h"
#endif

class ClientGateManager;

#ifdef __EPOLL__
class SessionConnectorManager : public ConnectorManager_Epoll
#else
class SessionConnectorManager : public ConnectorManager_Select
#endif

{
public:
	//�������Ӳ���
	SessionConnectorManager(ClientGateManager* gateManager);
	virtual ~SessionConnectorManager();

	virtual BOOL		InitPoolData(ConnectorPool* pool = NULL);
	virtual BOOL		RemoveConnector(Connector* pConnector);
	virtual BOOL		AddConnector(Connector* pConnector);
	virtual BOOL		HeartBeat(uint32 uTime = 0);
	virtual BOOL		Handle_CacheCommands();

	ClientGateManager* getGateManager() const { return mGateManager; }
public:

	// ��ȡ���ConnectorManager�����ƣ�����д��־ [yangzhen]
	virtual const char* GetManagerName() { return "SessionConnectorManager"; }

protected:
	ClientGateManager* mGateManager;
};

#endif
