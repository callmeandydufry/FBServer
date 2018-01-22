#ifndef __INCOMING_CONNECTOR_MANAGER_H__
#define __INCOMING_CONNECTOR_MANAGER_H__

#ifdef __EPOLL__
#include "ConnectorManager_Epoll.h"
#else
#include "ConnectorManager_Select.h"
#endif

class ClientGateManager;

#ifdef __EPOLL__
class IncomingConnectorManager : public ConnectorManager_Epoll
#else
class IncomingConnectorManager : public ConnectorManager_Select
#endif
	, public KThread
{
public:
	IncomingConnectorManager(ClientGateManager* gateManager);
	virtual ~IncomingConnectorManager();

	virtual BOOL		InitPoolData(ConnectorPool* pool = NULL);
	virtual Connector*	RecvNewConnector();
	virtual BOOL		RemoveConnector(Connector* pConnector);
	virtual BOOL		AddConnector(Connector* pConnector);
	virtual BOOL		HeartBeat(uint32 uTime = 0);

	virtual void		run();

	// 获取这个ConnectorManager的名称，用于写日志 [yangzhen]
	virtual const char* GetManagerName() { return "IncomingConnectorManager"; }

	ClientGateManager* getGateManager() const { return mGateManager; }

	virtual void stop()
	{
		mActive = FALSE;
	};


	BOOL IsActive()
	{
		return mActive;
	};

protected:
	BOOL mActive;

	ClientGateManager* mGateManager;
};

#endif
