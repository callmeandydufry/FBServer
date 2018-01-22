/*********************************************************************************************
created:	10/10/07 10:07:26
filename: 	ThreadContainer.h
author:		
modified:	yangzhen
purpose:	����Ϊ�߳�����

**********************************************************************************************/


#ifndef __THREAD_CONTAINER_H__
#define __THREAD_CONTAINER_H__

#include "TypeDefine.h"
#include "BaseTool.h"

#define MAX_THREAD	1024
class KThreadContainer
{

public:
	KThreadContainer();
	~KThreadContainer();

	//���������߳�
	BOOL	Begin();
	//ֹͣ�����߳�
	BOOL	Stop();
	//����߳�
	BOOL	InsertThread(KThread *pThread, int32 nIndex);
	//ɾ���߳�
	BOOL	RemoveThread(TID id);
	//����߳�
	KThread *GetThreadByID(TID id);
	KThread *GetThreadByIndex(uint32 index);
	//�洢�߳���Ϣ
	void	SaveThreadInfo();
	//�Ƿ������̶߳���ʼ�����
	BOOL	IsAllRun();
	//�Ƿ������̶߳�ֹͣ
	BOOL IsAllFinish();

private:
	//�̳߳�
	KThread *m_pThread[MAX_THREAD];
	//ͳ���̸߳��µĴ���
	int32	m_ThreadTickCount[MAX_THREAD];
	uint32	m_Count;
	uint32	m_Position;
};
#endif
