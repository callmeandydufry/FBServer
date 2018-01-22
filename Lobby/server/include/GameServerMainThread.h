#ifndef __BATTLE_SERVER_MAIN_THREAD_H__
#define __BATTLE_SERVER_MAIN_THREAD_H__

#include "TypeDefine.h"
#include "ThreadContainer.h"

class GameServerMainThread
{
public:
	GameServerMainThread();
	~GameServerMainThread();

	BOOL Init();
	void Clear();

	BOOL Loop();

	void Start();

	void Stop()
	{
		m_Active = FALSE;
	}

	void Quit();

	BOOL IsActive()
	{
		return m_Active;
	};

private:
	BOOL		m_Active;
	CTinyTimer	m_WorkingTime;
	CTinyTimer	m_FlushLogTimer;
	CTinyTimer	m_TickLogTimer;
	uint32		m_OPTimes;
	uint32		m_OPSteps;
};

extern GameServerMainThread* g_pGameServerMainThread;

#endif
