
#include "stdafx.h"
#include "ServerModuleThreadContainer.h"
#include "BaseTool.h"
#include "ConnectorPool.h"
#include "ServerModuleManager.h"

#if defined(__LINUX__)
#include <fstream>
#endif

ServerModuleThreadContainer* g_pModuleThreadContainer = NULL;

ServerModuleThreadContainer::ServerModuleThreadContainer()
{
	m_OPTimes = 0;
	m_OPSteps = 0;
	mCheckAllModuleRegistered = TRUE;
}

ServerModuleThreadContainer::~ServerModuleThreadContainer()
{
	Clear();
}

BOOL ServerModuleThreadContainer::Init()
{
	m_OPSteps = -1;
	m_OPTimes = 0;
	Compress_Minilzo::GetInstance().EnableLog(TRUE /*g_Config.m_ConfigInfo.m_nCompressGlobalLog*/);
	return TRUE;
}

void ServerModuleThreadContainer::Clear()
{
	mCheckAllModuleRegistered = TRUE;
}

BOOL ServerModuleThreadContainer::Tick()
{
	__GUARD__

		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		BOOL	ret = FALSE;
		uint32	uTime = GET_TIME().TickCount();
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		/*����ִ���߼�ִ�е�ʱ����Ϊһ��	 */
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		if (!m_WorkingTime.IsSetTimer())
		{
			m_WorkingTime.BeginTimer(1000, uTime);
			QLogSystem::QUICK_LOG(SLOW_LOG_SERVER, "ServerModuleThreadContainer Time init OK!");
		}

		if (!gIsOnlyLocalServer)
		{
			if (m_WorkingTime.CountingTimer(uTime))
			{
				// ����Ƿ������������е�ģ�鶼�Ѿ�������� [1/13/2018 yz]
				if (mCheckAllModuleRegistered)
				{
					if (((ServerModuleManager*)g_pModuleManager)->isAllModuleRegistered())
					{
						mCheckAllModuleRegistered = FALSE;

						// ����ģ�鶼�Ѿ��������֮��ſ�ʼ��������module��tick [1/13/2018 yz]
						m_AllThreadContainer.Begin();
					}
				}
			}
		}

		m_OPTimes++;
		m_OPSteps = 1;

		return TRUE;

	__UNGUARD__ return TRUE;
}

void ServerModuleThreadContainer::Start()
{
	g_pModuleManager->startWork();

	if (gIsOnlyLocalServer)
	{
		m_AllThreadContainer.Begin();
	}
}

void ServerModuleThreadContainer::Quit()
{
	m_AllThreadContainer.Stop();
	// �ȴ������߳̽��� [12/5/2017 yz]
	while (!m_AllThreadContainer.IsAllFinish())
	{
		SafeSleep(100);
	}

	// ֪ͨ����ģ������˳����� [11/25/2016 yz]
	if (g_pModuleManager)
	{
		g_pModuleManager->stopWork();
		g_pModuleManager->waitThreadExit();
		g_pModuleManager->clear();
	}
}
