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
ִ��Reload�����ʵ��
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
ִ��Memcpy�����ʵ��
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
���̴߳���Reload������,������֤�Ƿ�����Ҫ����Ľű�,������ӿڵĵ���
=======================================================================================================================
*/
void TabReloadHandle::MainThreadProcessTabReloadCommand(const uint32 uTime)
{
	__GUARD__

	if ((!m_MainThreadProcessTimer.IsSetTimer()) || (!m_MainThreadProcessTimer.CountingTimer(uTime)))
	{
		return ;
	}

	if (TRUE == IsHasCommand())	// ��Ҫ���������
	{
		// ���ó����ڴ���ָ��
		if (FALSE == m_bIsProcessCommand)
		{
			m_bIsProcessCommand = TRUE;
		}

		if (eLOAD_TAB_FINISH == m_eReloadSate)	// �ж��Ƿ�������
		{
			// ���л�Ծ���̶߳��Ѿ��յ��˼�������ź�,��ʼ���б��copy
			if (TRUE == IsAllThreadReponse())
			{
				QLogSystem::QUICK_LOG
					(
					QUICK_LOG_DEBUG, 
					"MainThreadProcessTabReloadCommand()!!! TabCommand = %s",
					m_CurrentCommnd
					);

				//��copyʵ��
				ExecMemcpyCommand(m_CurrentCommnd);

				//�����ж�ִ�����
				ResetAllState();
				
			}
		}
		else if (eNORMAL_STATE == m_eReloadSate)	// ��ʱ��ʼ���ر�
		{
			m_eReloadSate = eLOAD_TAB_STATE;

			QLogSystem::QUICK_LOG
				(
				QUICK_LOG_DEBUG, 
				"MainThreadProcessTabReloadCommand()!!! Start Loading Tab... TabCommand = %s",
				m_CurrentCommnd
				);

			//���ر��ʵ��
			ExecReloadCommand(m_CurrentCommnd);

			// �������,����ɱ�־
			m_eReloadSate = eLOAD_TAB_FINISH;
		}
	}

	__UNGUARD__
}

/*
=======================================================================================================================
�߼��̴߳���Reload������
=======================================================================================================================
*/
BOOL TabReloadHandle::LogicThreadProcessTabReloadCommand(int32 nContainerPos)
{
	__GUARD__

	// ���߳��ж������Ҫִ�е�ReloadTabָ�������߳��Ѿ�����������,�����̷߳���ȷ���ź�
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
�߼��߳������̷߳���ReTab��ָ��
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

	// TabReloadָ�����ڴ���
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
���߳�����ReTab��ָ��
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
�ı䵱ǰreloadָ���״̬
=======================================================================================================================
*/
BOOL TabReloadHandle::ChangeReloadCommandState(int32 nExpState, int32 nDesState)
{
	__GUARD__

	BOOL	bRes = FALSE;
#ifdef __LINUX__	//Linux �Ժ����Linuxʵ��
	int32 nChangeValue = nDesState - nExpState;

	// ���������Ԥ��ֵ,���ټ���ȥ
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
�����߳��źż���
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
���������̵߳�״̬
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
��ִ�гɹ���,��������״̬
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
�ж��Ƿ��������̶߳��Ѿ������˻�Ӧ
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