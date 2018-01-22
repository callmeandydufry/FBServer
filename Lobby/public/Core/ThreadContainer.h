/*********************************************************************************************
created:	10/10/07 10:07:26
filename: 	ThreadContainer.h
author:		
modified:	yangzhen
purpose:	本类为线程容器

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

	//启动所有线程
	BOOL	Begin();
	//停止所有线程
	BOOL	Stop();
	//添加线程
	BOOL	InsertThread(KThread *pThread, int32 nIndex);
	//删除线程
	BOOL	RemoveThread(TID id);
	//获得线程
	KThread *GetThreadByID(TID id);
	KThread *GetThreadByIndex(uint32 index);
	//存储线程信息
	void	SaveThreadInfo();
	//是否所有线程都初始化完毕
	BOOL	IsAllRun();
	//是否所有线程都停止
	BOOL IsAllFinish();

private:
	//线程池
	KThread *m_pThread[MAX_THREAD];
	//统计线程更新的次数
	int32	m_ThreadTickCount[MAX_THREAD];
	uint32	m_Count;
	uint32	m_Position;
};
#endif
