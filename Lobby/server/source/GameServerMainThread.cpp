
#include "stdafx.h"
#include "GameServerMainThread.h"
#include "GameServer.h"
#include "BaseTool.h"
#include "MKManager.h"
#include "ConnectorPool.h"
#include "ModuleManager.h"
#include "ServerModuleThreadContainer.h"

#include "BaseTool.h"
#include "CGReqeustLogin.h"
#include "ClientConnector.h"
#include "ClientGateManager.h"
#include "CGReqeustLogin.h"
#include "ClientSession.h"
	
#if defined(__LINUX__)
#include <fstream>
#endif

GameServerMainThread* g_pGameServerMainThread = NULL;

BOOL CheckExitFile();

GameServerMainThread::GameServerMainThread()
{
}

GameServerMainThread::~GameServerMainThread()
{
}

BOOL GameServerMainThread::Init()
{
	m_Active = TRUE;
	m_OPSteps = -1;
	m_OPTimes = 0;
	Compress_Minilzo::GetInstance().EnableLog(TRUE /*g_Config.m_ConfigInfo.m_nCompressGlobalLog*/);
	return TRUE;
}

void GameServerMainThread::Clear()
{
}

BOOL GameServerMainThread::Loop()
{
	__GUARD__

	Start();

	while (IsActive())
	{

		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		BOOL	ret = FALSE;
		uint32	uTime = GET_TIME().TickCount();
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		_NET_TRY
		{
			ret = g_pModuleThreadContainer->Tick();
			KCheck(ret);
			m_OPSteps = 0;
		}
		_NET_CATCH
		{
			QLOGGER_CODEHOLDER();
		}

		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		/*以下执行逻辑执行的时间间隔为一秒	 */
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		if (!m_WorkingTime.IsSetTimer())
		{
			m_WorkingTime.BeginTimer(1000, uTime);
			m_FlushLogTimer.BeginTimer(3000, uTime);
			m_TickLogTimer.BeginTimer(30000, uTime);
			QLogSystem::QUICK_LOG(SLOW_LOG_SERVER, "BattleServerMainThread Time init OK!");
		}

		if (!m_WorkingTime.CountingTimer(uTime))
		{
			SafeSleep(100);
			m_OPSteps = 1;
			//QLogSystem::QUICK_LOG(SLOW_LOG_SERVER, "m_WorkingTime.CountingTimer(uTime)%ld", uTime);
			continue;
		}

		//更新日志存档
		_NET_TRY
		{
			if (m_FlushLogTimer.CountingTimer(uTime))
			{
				
				//QLogSystem::QUICK_LOG(SLOW_LOG_SERVER, "m_FlushLogTimer.CountingTimer(uTime)%ld", uTime);
				QLOGGER_UPDATE();
			}

			m_OPSteps = 2;
		}
			_NET_CATCH
		{
			QLOGGER_CODEHOLDER();
		}

		m_OPTimes++;
		//检查线程是否死锁 
		if (m_TickLogTimer.CountingTimer(uTime))
		{
			if (Compress_Minilzo::GetInstance().IsEnableLog())
			{
				uint64 uUnCmSize = Compress_Minilzo::GetInstance().GetUnCmDataSize();
				uint64 uCmSize = Compress_Minilzo::GetInstance().GetCmDataSize();
				if (uUnCmSize != 0)
				{
#if defined(__WINDOWS__)
					WriteNetLog("Global Cm : UnCmSize=%I64u,\t\tCmSize=%I64u,\t\trate=%.2f%%", uUnCmSize, uCmSize, 100.0*(double)uCmSize / (double)uUnCmSize);
#elif defined(__LINUX__)
					WriteNetLog("Global Cm : UnCmSize=%llu,\t\tCmSize=%llu,\t\trate=%.2f%%", uUnCmSize, uCmSize, 100.0*(double)uCmSize / (double)uUnCmSize);
#endif
				}
			}
		}

		if (CheckExitFile() == TRUE)
		{
			// 检测到关服文件，停止服务器tick [12/5/2017 yz]
			Stop();
		}

		m_OPSteps = 3;

	};

	Quit();

	__UNGUARD__ return TRUE;
}

void GameServerMainThread::Start()
{
	g_pModuleThreadContainer->Start();
}

void GameServerMainThread::Quit()
{
	g_pModuleThreadContainer->Quit();
}

BOOL CheckExitFile()
{
	__GUARD__ if (remove("exit.cmd") == -1)
	{
		return FALSE;
	}

	remove("gameserver.pid");
	return TRUE;
	__UNGUARD__ return FALSE;
}
