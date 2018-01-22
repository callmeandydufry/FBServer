
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
		/*以下执行逻辑执行的时间间隔为一秒	 */
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
				// 检测是否这个服务端所有的模块都已经启动完毕 [1/13/2018 yz]
				if (mCheckAllModuleRegistered)
				{
					if (((ServerModuleManager*)g_pModuleManager)->isAllModuleRegistered())
					{
						mCheckAllModuleRegistered = FALSE;

						// 所有模块都已经启动完毕之后才开始本进程中module的tick [1/13/2018 yz]
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
	// 等待所有线程结束 [12/5/2017 yz]
	while (!m_AllThreadContainer.IsAllFinish())
	{
		SafeSleep(100);
	}

	// 通知各个模块进行退出处理 [11/25/2016 yz]
	if (g_pModuleManager)
	{
		g_pModuleManager->stopWork();
		g_pModuleManager->waitThreadExit();
		g_pModuleManager->clear();
	}
}
