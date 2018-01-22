#ifndef __SERVER_MODULE_BASE_H__
#define __SERVER_MODULE_BASE_H__

#include "ModuleSupport.h"

class ServerModuleBase : public ModuleBase
{
public:
	ServerModuleBase();
	~ServerModuleBase();

	virtual BOOL initServerModule(int32 moduleName, ModuleComponent* defaultComponent, int32 groupID);
	virtual void uinitServerModule();

	int32 getGroupID() const { return mGroupID; }

	void setThreadIndex(int32 threadIndex) { mThreadIndex = threadIndex; }
	int32 getThreadIndex() const { return mThreadIndex; }
protected:
	int32 mGroupID;
	int32 mThreadIndex;
};


class ServerModuleThread : public KThread
{
public:
	ServerModuleThread();
	~ServerModuleThread();

	virtual void stop();
	virtual void run();

	void insertModule(ServerModuleBase* module);
	void removeModule(ServerModuleBase* module);
	BOOL isEmpty();

protected:
	BOOL mIsActive;
	CTinyTimer mTickTimer;
	TStaticArray<ServerModuleBase*, MAX_MODULE_COUNT> mModules;
};
#endif