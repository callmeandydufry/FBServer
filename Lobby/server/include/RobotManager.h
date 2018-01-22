#ifndef __ROBOT_MANAGER_H__
#define __ROBOT_MANAGER_H__

#include "TypeDefine.h"
#include "ServerModuleBase.h"
#include "ServerModuleExportInterface.h"
#include "RobotData.h"
#include "CommonObjectPool.h"
#include "StringHashMap.h"

enum ERobotCMD
{
	ERCMD_START = 0,
	ERCMD_InitAllRobot,
	ERCMD_StartLogin,	
	ERCMD_TalkInTheWorld,

	ERCMD_END,
};

struct tagRobotCMDUtil
{		
	// todo 

};

class RobotManager
	: public ServerModuleBase
	, public IRobotManager
{
	typedef BOOL(RobotManager::*HandleRobotControlCMD)(const tagRobotCMDUtil* stCMD);
public:
	RobotManager() : mWaittingCMD(TRUE) {};
	~RobotManager() {};

	virtual BOOL				initServerModule(int32 moduleName, ModuleComponent* defaultComponent, int32 groupID);
	virtual void				uinitServerModule();
	virtual void				tickModule(uint32 uTick);
	virtual int64				getComponentId() { return MODULE_DEFAULT_ID; }
public:
	BOOL						controlRobotCMD(ERobotCMD eCMD, tagRobotCMDUtil& stCMD);

public:

protected:
	BOOL						handleCMDInitAllRobot(const tagRobotCMDUtil* stCMD);
	BOOL						handleCMDStartLogin(const tagRobotCMDUtil* stCMD);
	BOOL						handleCMDTalkInTheWorld(const tagRobotCMDUtil* stCMD);
	BOOL						handleCMDTalkToSomebody(const tagRobotCMDUtil* stCMD);


	BOOL						handleCMDClearAllRobot(const tagRobotCMDUtil* stCMD);

protected:
	HandleRobotControlCMD		mArrHandleRobotControlCMD[ERCMD_END];

protected:
	CommonObjectPool<RobotData>	mRobotPool;
	TStringHashMap<RobotData*>  mRobotStrMap;
	BOOL						mWaittingCMD;
};


#endif // __ROBOT_MANAGER_H__
