#ifndef __SERVER_MODULE_THREAD_CONTAINER_H__
#define __SERVER_MODULE_THREAD_CONTAINER_H__

#include "TypeDefine.h"
#include "ThreadContainer.h"


class ServerModuleThreadContainer
{
public:
	ServerModuleThreadContainer();
	~ServerModuleThreadContainer();

	BOOL Init();
	void Clear();

	void Start();

	BOOL Tick();

	void Quit();

	KThreadContainer& GetThreadContainer()
	{
		return m_AllThreadContainer;
	}

private:
	CTinyTimer	m_WorkingTime;
	uint32		m_OPTimes;
	uint32		m_OPSteps;

	BOOL mCheckAllModuleRegistered;
	KThreadContainer m_AllThreadContainer;
};

extern ServerModuleThreadContainer* g_pModuleThreadContainer;

#endif