#ifndef __CONNECTOR_POOL_H__
#define __CONNECTOR_POOL_H__

#include "TypeDefine.h"
#include "Connector.h"

class	ConnectorPool
{
public:
	ConnectorPool();
	virtual ~ConnectorPool();

	BOOL				Init(uint32 uMaxCount);
	BOOL				InitPoolData(int32 nIndex,Connector* pConnector);

	uint32				GetMaxCount() { return m_uMaxCount; }

	int32				GetConnectorCount()
	{ 
		//uint32 uMaxPoolSize = g_Config.m_ConfigInfo.m_nGeneralMapMaxPlayer;
		//if (g_Config.m_MultiGroupConfig.m_nGroupCount > 0)
		//{
		//	uMaxPoolSize = g_Config.m_ConfigInfo.m_nMPVPBattleMapMaxPlayer;
		//}

		//return uMaxPoolSize - m_uConnectorCount;
		return m_uConnectorCount;
	}

	Connector*			GetConnector(ut_id_connector ConnectorID);
	Connector			*GetConnectorForGroupMode(ut_id_connector ConnectorID);	// 2010-9-25 by rj 以跨服方式取得连接
	virtual Connector*	NewConnector(BOOL bClear=TRUE);
	virtual BOOL		DelConnector(ut_id_connector ConnectorID);

private:
	Connector**			m_apConnectors;
	uint32				m_uConnectorCount;
	uint32				m_uMaxCount;
	uint32				m_Position;
	FastLock			m_Lock;
};

#endif
