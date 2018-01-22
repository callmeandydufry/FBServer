#include "stdafx.h"
#include "ConnectorPool.h"


ConnectorPool::ConnectorPool()
{
	m_apConnectors		= NULL;
	m_uConnectorCount	= 0;
	m_Position			= 0;
	m_uMaxCount			= 0;
}

ConnectorPool::~ConnectorPool()
{
	if( m_apConnectors )
	{
		for( int32 _Idx=0; _Idx<m_uMaxCount; _Idx++ )
		{
			KSafeDelete(m_apConnectors[_Idx]);
		}
	}
	KSafeDelete_ARRAY(m_apConnectors);
}

BOOL ConnectorPool::Init(uint32 uMaxCount)
{
	__GUARD__	

	m_Position = 0;
	m_uMaxCount = uMaxCount;
	m_uConnectorCount = uMaxCount;
	
	m_apConnectors = new Connector*[uMaxCount];
	KCheck(m_apConnectors);
	for(int32 _Idx = 0; _Idx < uMaxCount; _Idx++)
	{
		m_apConnectors[_Idx] = NULL;
	}

	return TRUE;
	__UNGUARD__
	return FALSE;
}

BOOL ConnectorPool::InitPoolData(int32 nIndex,Connector* pConnector)
{
	__GUARD__

	KCheck(pConnector);
	KCheck(nIndex>=0 && nIndex<m_uMaxCount);

	m_apConnectors[nIndex] = pConnector;
	m_apConnectors[nIndex]->SetConnectorID((ut_id_connector)nIndex);
	m_apConnectors[nIndex]->SetEmpty(TRUE);
	m_apConnectors[nIndex]->InitSocket();		

	return TRUE;
	__UNGUARD__
	return FALSE;
}


Connector* ConnectorPool::GetConnector(ut_id_connector ConnectorID)
{
	if(ConnectorID >= m_uMaxCount || ConnectorID < 0) 
		return NULL;
	if( m_apConnectors[ConnectorID]->IsEmpty() )
		return NULL;
	return m_apConnectors[ConnectorID];
}

// 以跨服方式取得连接
Connector *ConnectorPool::GetConnectorForGroupMode(ut_id_connector ConnectorID)
{
	if(ConnectorID >= m_uMaxCount || ConnectorID < 0) 
		return NULL;
	return m_apConnectors[ConnectorID];
}

Connector* ConnectorPool::NewConnector(BOOL bClear)
{
	__GUARD__ 
		
	lock_guard<FastLock>	autolock(m_Lock);
	int32					iRet = 0;

	for(int32 _Idx = 0; _Idx < m_uMaxCount; _Idx++)
	{
		if(m_apConnectors[m_Position]->IsEmpty())
		{
			iRet = m_Position;
			if( bClear )
				m_apConnectors[iRet]->Clear();
			m_apConnectors[m_Position]->SetEmpty(FALSE);

			m_Position++;
			if(m_Position >= m_uMaxCount)
				m_Position = 0;

			m_uConnectorCount--;

			return m_apConnectors[iRet];
		}

		m_Position++;
		if(m_Position >= m_uMaxCount) 
			m_Position = 0;
	}

	return NULL;
	__UNGUARD__ 
	return NULL;
}


BOOL ConnectorPool::DelConnector(ut_id_connector ConnectorID)
{
	__GUARD__ 
	
	lock_guard<FastLock>	autolock(m_Lock);

	if( ConnectorID<0 || ConnectorID>=m_uMaxCount )
	{
		QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectorPool::DelConnector(): Error ! ConnectorID=%d", ConnectorID );
		return FALSE;
	}

	m_apConnectors[ConnectorID]->Clear();

	m_uConnectorCount++;

	return TRUE;
	__UNGUARD__
	return FALSE;
}
