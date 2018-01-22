/*********************************************************************************************
created:	10/10/07 10:07:26
filename: 	ThreadContainer.cpp
author:		
modified:	yangzhen
purpose:	����Ϊ�߳�����

**********************************************************************************************/


#include "stdafx.h"
#include "ThreadContainer.h"

KThreadContainer::KThreadContainer()
{
	__GUARD__ for(int32 _Idx = 0; _Idx < MAX_THREAD; _Idx++)
	{
		m_pThread[_Idx] = NULL;
		m_ThreadTickCount[_Idx] = 0;
	}

	m_Count = 0;
	m_Position = 0;

	__UNGUARD__
}

KThreadContainer::~KThreadContainer()
{
	__GUARD__ for(int32 _Idx = 0; _Idx < MAX_THREAD; _Idx++)
	{
		KSafeDelete(m_pThread[_Idx]);
	}

	m_Count = 0;
	m_Position = 0;

	__UNGUARD__
}

//����߳�
BOOL KThreadContainer::InsertThread(KThread *pThread, int32 nIndex)
{
	__GUARD__ if(nIndex < 0 || nIndex >= MAX_THREAD)
	{
		KCheck(FALSE);
		return FALSE;
	}

	KCheck(pThread);

	KCheck(m_pThread[nIndex] == NULL);

	m_pThread[nIndex] = pThread;

	pThread->setContainerPos(nIndex);

	m_Count++;
	m_Position++;
	KCheck(m_Position <= MAX_THREAD);

	return TRUE;

	__UNGUARD__ return FALSE;
}

//ɾ���߳�
BOOL KThreadContainer::RemoveThread(TID id)
{
	__GUARD__ for(int32 _Idx = 0; _Idx < MAX_THREAD; _Idx++)
	{
		if(m_pThread[_Idx] && m_pThread[_Idx]->getTID() == id)
		{
			m_Position = _Idx;
			m_pThread[_Idx] = NULL;
			m_Count--;
			KCheck(m_Count >= 0);

			return TRUE;
		}
	}

	__UNGUARD__ return FALSE;
}

//����߳�
KThread *KThreadContainer::GetThreadByID(TID id)
{
	__GUARD__ for(int32 _Idx = 0; _Idx < MAX_THREAD; _Idx++)
	{
		if(m_pThread[_Idx] && m_pThread[_Idx]->getTID() == id)
		{
			return m_pThread[_Idx];
		}
	}

	__UNGUARD__ return NULL;
}

KThread *KThreadContainer::GetThreadByIndex(uint32 index)
{
	__GUARD__ if(index >= MAX_THREAD) return NULL;

	return m_pThread[index];

	__UNGUARD__ return NULL;
}

//���������߳�
BOOL KThreadContainer::Begin()
{
	__GUARD__ for(int32 _Idx = 0; _Idx < MAX_THREAD; _Idx++)
	{
		if(m_pThread[_Idx]) m_pThread[_Idx]->start();
	}

	return TRUE;

	__UNGUARD__ return FALSE;
}

//ֹͣ�����߳�
BOOL KThreadContainer::Stop()
{
	__GUARD__ for(int32 _Idx = 0; _Idx < MAX_THREAD; _Idx++)
	{
		if(m_pThread[_Idx]) m_pThread[_Idx]->stop(); 
	}

	return TRUE;

	__UNGUARD__ return FALSE;
}

//�洢�߳���Ϣ
void KThreadContainer::SaveThreadInfo()
{
	__GUARD__ for(int32 _Idx = 0; _Idx < MAX_THREAD; _Idx++)
	{
		if(m_pThread[_Idx])
		{
			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
			int32	TickCount = m_pThread[_Idx]->getTickCount();
			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

			if(m_ThreadTickCount[_Idx] != TickCount)
			{
				m_ThreadTickCount[_Idx] = TickCount;
			}
			else
			{
				QLogSystem::QUICK_LOG
					(
						SLOW_LOG_SERVER,
						"Thread tid = %d,TickCount = %d,Steps =%d,Status=%d",
						m_pThread[_Idx]->getTID(),
						m_pThread[_Idx]->getTickCount(),
						m_pThread[_Idx]->getSteps(),
						m_pThread[_Idx]->getState()
					);
			}
		}
	}

	__UNGUARD__
}

//�Ƿ������̶߳���ʼ�����
BOOL KThreadContainer::IsAllRun()
{
	__GUARD__ for(int32 _Idx = 0; _Idx < MAX_THREAD; _Idx++)
	{
		if(m_pThread[_Idx])
		{
			if(m_pThread[_Idx]->getInit() == FALSE) return FALSE;
		}
	}

	return TRUE;

	__UNGUARD__ return FALSE;
}
//�Ƿ������̶߳�ֹͣ
BOOL KThreadContainer::IsAllFinish()
{
	__GUARD__ for(int32 i = 0; i < MAX_THREAD; i++)
	{
		if(m_pThread[i])
		{
			if(m_pThread[i]->getState() != KThread::THREAD_FINISH) return FALSE;
		}
	}

	return TRUE;

	__UNGUARD__ return FALSE;
}
