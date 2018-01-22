#ifndef __ModuleConnectorManager_H__
#define __ModuleConnectorManager_H__

#ifdef __EPOLL__
#include "ConnectorManager_Epoll.h"
#else
#include "ConnectorManager_Select.h"
#endif
#include "StringHashMap.h"


#ifdef __EPOLL__
class ModuleConnectorManager : public ConnectorManager_Epoll
#else
class ModuleConnectorManager : public ConnectorManager_Select
#endif

{
public:
	//网络连接部分
	ModuleConnectorManager();
	virtual ~ModuleConnectorManager();

	virtual BOOL InitPoolData(ConnectorPool* pool = NULL);
	virtual BOOL RemoveConnector(Connector* pConnector);
	virtual BOOL AddConnector(Connector* pConnector);
	virtual BOOL HeartBeat(uint32 uTime = 0);
	virtual BOOL Handle_CacheCommands();

	ut_id_connector ConnectServer(const char* ip, uint16 port);
	ut_id_connector GetConnectIDByIP(const char* ip, uint16 port);

	// 获取这个ConnectorManager的名称，用于写日志 [yangzhen]
	virtual const char* GetManagerName() { return "ModuleConnectorManager"; }

protected:
	BOOL ConnectToServer(const char* ip, uint16 port, ut_id serverid, ut_id_connector& connID, BOOL bCreate = TRUE, BOOL bSendSS = TRUE);

	TStringHashMap<ut_id_connector> mIpConnectorMap;
};

#endif
