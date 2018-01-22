#include "stdafx.h"
#include "ServerModuleBase.h"

//-------------------------------------------------------------------------------------------------------------
ServerModuleBase::ServerModuleBase()
	: mGroupID(INVALID_ID)
	, mThreadIndex(INVALID_ID)
{

}

ServerModuleBase::~ServerModuleBase()
{

}

BOOL ServerModuleBase::initServerModule(int32 moduleName, ModuleComponent* defaultComponent, int32 groupID)
{
	mGroupID = groupID;
	return ModuleBase::initModule(moduleName, defaultComponent);
}

void ServerModuleBase::uinitServerModule()
{
	ModuleBase::uinitModule();
	mGroupID = INVALID_ID;
}

//-------------------------------------------------------------------------------------------------------------
ServerModuleThread::ServerModuleThread()
{
	mIsActive = TRUE;
}

ServerModuleThread::~ServerModuleThread()
{

}

void ServerModuleThread::stop()
{
	mIsActive = FALSE;
}

void ServerModuleThread::run()
{
	mTickTimer.BeginTimer(50, GET_TIME().TickCount());

	while (mIsActive)
	{
		if (!mTickTimer.CountingTimer(GET_TIME().TickCount()))
		{
			SafeSleep(5);
			continue;
		}

		for (int32 i = 0; i < mModules.GetSize(); ++i)
		{
			mModules[i]->tickModule(GET_TIME().TickCount());
		}
	}
}

void ServerModuleThread::insertModule(ServerModuleBase* module)
{
	// 线程已经跑起来之后就不能再加入模块了 [1/13/2018 yz]
	KCheck(getState() < KThread::THREAD_RUNNING);

	mModules.AddItem(module);
}

void ServerModuleThread::removeModule(ServerModuleBase* module)
{
	KCheck(getState() != KThread::THREAD_RUNNING);

	mModules.RemoveItem(module);
}

BOOL ServerModuleThread::isEmpty()
{
	if (mModules.GetSize() == 0)
		return TRUE;
	return FALSE;
}