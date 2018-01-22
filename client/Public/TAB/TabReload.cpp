/*$T MapServer/TabReload.cpp GC */


/*$6
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

#include "stdafx.h"
#include "TabReload.h"

/*
=======================================================================================================================
=======================================================================================================================
*/
TabReloadHandle::TabReloadHandle()
{
	m_uThreadCount = 0;
	memset(m_CurrentCommnd, 0, sizeof(m_CurrentCommnd));
	m_pThreadSignal = NULL;

	m_MainThreadProcessTimer.Clear();
	m_LogicThreadProcessTimer.Clear();
	m_uCommandCount = 0;
}

/*
=======================================================================================================================
=======================================================================================================================
*/
TabReloadHandle::~TabReloadHandle()
{
	m_uThreadCount = 0;
	KSafeDelete_ARRAY(m_pThreadSignal);
}

/*
=======================================================================================================================
=======================================================================================================================
*/
BOOL TabReloadHandle::Init()
{
	__GUARD__

	m_pThreadSignal = new BOOL[m_uThreadCount];
	KCheck(m_pThreadSignal);

	m_eReloadSate = eNORMAL_STATE;

	m_bIsProcessCommand = FALSE;

	initword(m_aTabReloadCommandState, eCOMMAND_FREE);

 	ResetAllThreadSignal();
 	RegisterAllHandler();

	m_MainThreadProcessTimer.BeginTimer(1000, GET_TIME().TickCount());
	m_LogicThreadProcessTimer.BeginTimer(500, GET_TIME().TickCount());

	return TRUE;

	__UNGUARD__ return FALSE;
}

/*
=======================================================================================================================
=======================================================================================================================
*/
void TabReloadHandle::RegisterTabHandler(char* Tab, TabReloadCallBackFun reloadCallBack, TabMemcpyCallBackFun memcpyCallBack)
{
	__GUARD__

	KCheck(m_uCommandCount < TAB_COMMAND_COUNT);

	tsnprintf_s(m_CommandHandle[m_uCommandCount].m_szCommand, sizeof(m_CommandHandle[m_uCommandCount].m_szCommand), "%s", Tab);

	m_CommandHandle[m_uCommandCount].m_ReloadCallBackFun = reloadCallBack;
	m_CommandHandle[m_uCommandCount].m_MemcpyCallBackFun = memcpyCallBack;

	m_uCommandCount++;

	__UNGUARD__
}

/*
=======================================================================================================================
执行Reload命令的实现
=======================================================================================================================
*/
void TabReloadHandle::ExecReloadCommand(const char* TabCommand)
{
	__GUARD__

	for(int32 nIndex = 0; nIndex < m_uCommandCount; nIndex++ )
	{
		if (strcmp(TabCommand, m_CommandHandle[nIndex].m_szCommand) == 0)
		{
			QLogSystem::QUICK_LOG
				(
				QUICK_LOG_DEBUG, 
				"TabReloadHandle::ExecReloadCommand Start... Tab = %s",
				TabCommand
				);

			(*m_CommandHandle[nIndex].m_ReloadCallBackFun)();

			QLogSystem::QUICK_LOG
				(
				QUICK_LOG_DEBUG, 
				"TabReloadHandle::ExecReloadCommand Success!!! Tab = %s",
				TabCommand
				);
		}
	}

	__UNGUARD__
}

/*
=======================================================================================================================
执行Memcpy命令的实现
=======================================================================================================================
*/
void TabReloadHandle::ExecMemcpyCommand(const char* TabCommand)
{
	__GUARD__

	for(int32 nIndex = 0; nIndex < m_uCommandCount; nIndex++ )
	{
		if (strcmp(TabCommand, m_CommandHandle[nIndex].m_szCommand) == 0)
		{
			(*m_CommandHandle[nIndex].m_MemcpyCallBackFun)();

			QLogSystem::QUICK_LOG
				(
				QUICK_LOG_DEBUG, 
				"TabReloadHandle::ExecMemcpyCommand Success!!! Tab = %s",
				TabCommand
				);
		}
	}

	__UNGUARD__
}

/*
=======================================================================================================================
主线程处理Reload的流程,包括验证是否有需要处理的脚本,及处理接口的调用
=======================================================================================================================
*/
void TabReloadHandle::MainThreadProcessTabReloadCommand(const uint32 uTime)
{
	__GUARD__

	if ((!m_MainThreadProcessTimer.IsSetTimer()) || (!m_MainThreadProcessTimer.CountingTimer(uTime)))
	{
		return ;
	}

	if (TRUE == IsHasCommand())	// 有要处理的命令
	{
		// 设置成正在处理指令
		if (FALSE == m_bIsProcessCommand)
		{
			m_bIsProcessCommand = TRUE;
		}

		if (eLOAD_TAB_FINISH == m_eReloadSate)	// 判定是否加载完毕
		{
			// 所有活跃子线程都已经收到了加载完成信号,开始进行表的copy
			if (TRUE == IsAllThreadReponse())
			{
				QLogSystem::QUICK_LOG
					(
					QUICK_LOG_DEBUG, 
					"MainThreadProcessTabReloadCommand()!!! TabCommand = %s",
					m_CurrentCommnd
					);

				//表copy实现
				ExecMemcpyCommand(m_CurrentCommnd);

				//当所有都执行完后
				ResetAllState();
				
			}
		}
		else if (eNORMAL_STATE == m_eReloadSate)	// 此时开始加载表
		{
			m_eReloadSate = eLOAD_TAB_STATE;

			QLogSystem::QUICK_LOG
				(
				QUICK_LOG_DEBUG, 
				"MainThreadProcessTabReloadCommand()!!! Start Loading Tab... TabCommand = %s",
				m_CurrentCommnd
				);

			//加载表的实现
			ExecReloadCommand(m_CurrentCommnd);

			// 加载完成,置完成标志
			m_eReloadSate = eLOAD_TAB_FINISH;
		}
	}

	__UNGUARD__
}

/*
=======================================================================================================================
逻辑线程处理Reload的流程
=======================================================================================================================
*/
BOOL TabReloadHandle::LogicThreadProcessTabReloadCommand(int32 nContainerPos)
{
	__GUARD__

	// 子线程判定如果有要执行的ReloadTab指令且主线程已经将表加载完成,向主线程发送确认信号
	if (eLOAD_TAB_FINISH == m_eReloadSate)
	{
		ThreadResponse(nContainerPos);

		return FALSE;
	}

	return TRUE;

	__UNGUARD__	return TRUE;
}

/*
=======================================================================================================================
逻辑线程向主线程发送ReTab的指令
=======================================================================================================================
*/
BOOL TabReloadHandle::SendReloadTabCommand(const char* cTabCommandName)
{
	__GUARD__

	if (FALSE == HasThisCommand(cTabCommandName))
	{
		QLogSystem::QUICK_LOG
			(
			QUICK_LOG_DEBUG, 
			"TabReloadHandle::SendReloadTabCommand Error!!! Not Regist This Commnd cTabCommandName = %s",
			cTabCommandName
			);

		return FALSE;
	}

	// TabReload指令正在处理
	if ( IsProcessCommand() || !ChangeReloadCommandState(eCOMMAND_FREE, eCOMMAND_READY_SEND))
	{
		return FALSE;
	}

	strncpy(m_CurrentCommnd, cTabCommandName, TAB_COMMAND_LENGTH - 1);
	
	if (!ChangeReloadCommandState(eCOMMAND_READY_SEND, eCOMMAND_SEND_FINISH))
	{
		QLogSystem::QUICK_LOG
			(
			QUICK_LOG_DEBUG, 
			"TabReloadHandle::SendReloadTabCommand Error!!! ChangeState Step2 Fail!!! m_nTabReloadCommandState = %d",
			GetReloadCommandState()
			);
	}

	return TRUE;
	__UNGUARD__ return FALSE;
}

/*
=======================================================================================================================
主线程清理ReTab的指令
=======================================================================================================================
*/
void TabReloadHandle::ClearCommand()
{
	__GUARD__

	memset(m_CurrentCommnd, 0, sizeof(m_CurrentCommnd));

	if (!ChangeReloadCommandState(eCOMMAND_SEND_FINISH, eCOMMAND_FREE))
	{
		QLogSystem::QUICK_LOG
			(
			QUICK_LOG_DEBUG, 
			"TabReloadHandle::ClearCommand() Error!!! m_nTabReloadCommandState = %d",
			GetReloadCommandState()
			);
	}

	__UNGUARD__
}

/*
=======================================================================================================================
改变当前reload指令的状态
=======================================================================================================================
*/
BOOL TabReloadHandle::ChangeReloadCommandState(int32 nExpState, int32 nDesState)
{
	__GUARD__

	BOOL	bRes = FALSE;
#ifdef __LINUX__	//Linux 稍后加入Linux实现
	int32 nChangeValue = nDesState - nExpState;

	// 如果不等于预期值,则再减回去
	if (nExpState != atomic_add_return(nChangeValue, &m_aTabReloadCommandState))
	{
		atomic_sub_and_test(nChangeValue, &m_aTabReloadCommandState);
	}
	else
	{
		bRes = TRUE;
	}

	QLogSystem::QUICK_LOG
		(
		QUICK_LOG_DEBUG, 
		"TabReloadHandle::ChangeReloadCommandState... nExpState = %d, nDesState = %d, nChangeValue = %d",
		nExpState,
		nDesState,
		nChangeValue
		);

#else	// Windows
	bRes = (InterlockedCompareExchange(const_cast<LPLONG>(&m_aTabReloadCommandState), nDesState, nExpState) == nExpState);
#endif

	return bRes;

	__UNGUARD__ return FALSE;
}

/*
=======================================================================================================================
增加线程信号计数
=======================================================================================================================
*/
void TabReloadHandle::ThreadResponse(int32 nContainerPos)
{
	__GUARD__;

	char cMsg[100] = {0};

	sprintf(cMsg, "TabReloadHandle::ThreadResponse Error !!! nContainerPos = %d", nContainerPos);
	KCheckEx(nContainerPos >= 0 && nContainerPos < m_uThreadCount, cMsg);

	m_pThreadSignal[nContainerPos] = TRUE;

	__UNGUARD__;
}

/*
=======================================================================================================================
重置所有线程的状态
=======================================================================================================================
*/
void TabReloadHandle::ResetAllThreadSignal()
{
	__GUARD__

	for (int32 nIndex = 0; nIndex < m_uThreadCount; nIndex++ )
	{
		m_pThreadSignal[nIndex] = FALSE;
	}

	__UNGUARD__
}

/*
=======================================================================================================================
当执行成功后,重置所有状态
=======================================================================================================================
*/
void TabReloadHandle::ResetAllState()
{
	__GUARD__

	m_eReloadSate = eNORMAL_STATE;
	ClearCommand();

	ResetAllThreadSignal();

	m_bIsProcessCommand = FALSE;

	__UNGUARD__
}

/*
=======================================================================================================================
判断是否所有子线程都已经做出了回应
=======================================================================================================================
*/
BOOL TabReloadHandle::IsAllThreadReponse()
{
	__GUARD__;

	for (int32 nIndex = 0; nIndex < m_uThreadCount; nIndex++ )
	{
		if (FALSE == m_pThreadSignal[nIndex])
		{
			return FALSE;
		}
	}

	return TRUE;

	__UNGUARD__ return FALSE;
}


/*
=======================================================================================================================
=======================================================================================================================
*/
BOOL TabReloadHandle::HasThisCommand(const char* TabCommand)
{
	__GUARD__

	BOOL bRes = FALSE;

	for(int32 nIndex = 0; nIndex < m_uCommandCount; nIndex++ )
	{
		if (strcmp(TabCommand, m_CommandHandle[nIndex].m_szCommand) == 0)
		{
			bRes = TRUE;	
		}
	}

	return bRes;

	__UNGUARD__ return FALSE;
}

/*
=======================================================================================================================
=======================================================================================================================
*/
void TabReloadHandle::RegisterAllHandler()
{
	__GUARD__;
	
	__UNGUARD__;
}