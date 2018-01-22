/*********************************************************************************************
filename: 	ConnectThread.cpp
purpose:	本线程是负责与GL进行通信的 
			ThreadSingleton(ThreadManager.h)负责创建本与启动本线程
**********************************************************************************************/


#include "stdafx.h"
#include "ConnectThread.h"

ConnectThread::ConnectThread(ConnectorManager* pConnectorManager)
{
	__GUARD__	m_Active = TRUE;

	m_pConnectorManager = pConnectorManager;

	__UNGUARD__
}

ConnectThread::~ConnectThread()
{
	__GUARD__ __UNGUARD__
}

void ConnectThread::run()
{
	__GUARD__	_NET_TRY
	{
		QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectThread::run() Thread ID:%d...",getTID());

		while(IsActive())
		{
			if(m_pConnectorManager)
			{
				m_TickCount++;
				SafeSleep(1);

				BOOL	ret = TRUE;
				_NET_TRY
				{
					ret = m_pConnectorManager->CheckAndReconnectToServer();
					KCheck(ret);
				}
				_NET_CATCH
				{
					QLOGGER_CODEHOLDER();
				}
				 
				m_TickCount++;
			}
			else
			{
				KCheck(FALSE);
			}
		}
	}

	_NET_CATCH
	{
		QLOGGER_CODEHOLDER();
	}

	QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "ConnectThread Exit!!!! Thread ID:%d...",getTID());

	__UNGUARD__
}
