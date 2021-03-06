#include "stdafx.h"
#include "RobotManager.h"
#include "ModuleManager.h"
#include "Setting.h"

BOOL RobotManager::initServerModule(int32 moduleName, ModuleComponent* defaultComponent, int32 groupID)
{
	for (int32 i = 0; i < ERCMD_END; ++i)
	{
		mArrHandleRobotControlCMD[i] = NULL;
	}

	mArrHandleRobotControlCMD[ERCMD_InitAllRobot] = &RobotManager::handleCMDInitAllRobot;
	mArrHandleRobotControlCMD[ERCMD_StartLogin] = &RobotManager::handleCMDStartLogin;
	mArrHandleRobotControlCMD[ERCMD_TalkInTheWorld] = &RobotManager::handleCMDTalkInTheWorld;

	mRobotPool.initPool(g_Config.mRobotInfo.mRunRobotNum);
	mRobotStrMap.InitTable(g_Config.mRobotInfo.mRunRobotNum, MAX_ACCOUNT);

	return ServerModuleBase::initServerModule(moduleName, this, groupID);
}

void RobotManager::uinitServerModule()
{
	ServerModuleBase::uinitServerModule();
}

void RobotManager::tickModule(uint32 uTick)
{
	if (mWaittingCMD)
	{
		int32 nCmdID = 0;
		std::cout << "input cmd id:";
		std::cin >> nCmdID;
		std::cout << std::endl;

		int32 nControlNums = 0;
		std::cout << "input robot nums:";
		std::cin >> nControlNums;
		std::cout << std::endl;

		// invalid cmd
		if (nCmdID >= ERCMD_END)
			return;

		tagRobotCMDUtil tagCMD;
		tagCMD.clear();
		tagCMD.mControlNum = nControlNums;
		controlRobotCMD((ERobotCMD)nCmdID, tagCMD);

		mWaittingCMD = FALSE;
	}

	ServerModuleBase::tickModule(uTick);
}

//----------------------------------------------------------------------
// 操作机器人接口 [1/15/2018 Chief]
//----------------------------------------------------------------------
BOOL RobotManager::controlRobotCMD(ERobotCMD eCMD, tagRobotCMDUtil& stCMD)
{
	__GUARD__;

	// todo check

	BOOL bRet = FALSE;
	HandleRobotControlCMD pFuncHandler = mArrHandleRobotControlCMD[(int32)eCMD];
	if (pFuncHandler)
	{
		bRet = (this->*pFuncHandler)(&stCMD);
	}

	mWaittingCMD = TRUE;

	return TRUE;
	__UNGUARD__;
	return FALSE;
}


//----------------------------------------------------------------------
// 处理操作 [1/15/2018 Chief]
//----------------------------------------------------------------------
// 初始化机器人回调
class __RobotManager_rpcCreateRobotNewSession_Callback : public RobotManager
{
public:
	void callbackSuccess(uint64& nSessionID, DataStream& userdata)
	{
		FixedString<MAX_ACCOUNT> strName;
		userdata.getValue(strName);
		RobotData* pRobot = mRobotStrMap.Get(strName.c_str());
		if (pRobot)
		{
			pRobot->setSession(nSessionID);
		}
	}

	void callbackOvertime(DataStream& userData)
	{

	}

};

//初始化机器人
BOOL RobotManager::handleCMDInitAllRobot(const tagRobotCMDUtil* stCMD)
{
	__GUARD__;

	// create all robots' name && token
	FixedString<MAX_ACCOUNT> strAccount("handleCMDInitAllRobot_IAMROBOT");

	for (int32 i = 0; i < g_Config.mRobotInfo.mRunRobotNum; ++i)
	{
		char szNum[DB_MAX_PASSWD_LEN] = { 0 };
		sprintf_s(szNum, "%d_", i);
		FixedString<MAX_ACCOUNT> tempStr(szNum);
		tempStr += strAccount;
		RobotData* pRobot = mRobotPool.allocObject();
		if (pRobot)
		{
			pRobot->setAccountName(tempStr);
			pRobot->setToken(tempStr);
			mRobotStrMap.Add(tempStr.c_str(), pRobot);

			DataStream userData;
			userData.pushValue(tempStr);
			// todo 获取某个GateMod需要可配置
			g_pModuleManager->getExportCallable < IClientGateManagerExportCallable < __RobotManager_rpcCreateRobotNewSession_Callback > >(
				ServerModuleUtil::getModuleNameByTypeAndGroup(EServerModuleType_ClientGate, 0),
				MODULE_DEFAULT_ID,
				(__RobotManager_rpcCreateRobotNewSession_Callback*)this
				)
				->rpcCreateRobotNewSession(tempStr.c_str(), tempStr.c_str())
				->bindCallback(
					&__RobotManager_rpcCreateRobotNewSession_Callback::callbackSuccess,
					&__RobotManager_rpcCreateRobotNewSession_Callback::callbackOvertime,
					userData,
					10000
				);

			pRobot->setRobotCurStatus(ERobotStatus_Inited);
		}
	}

	return TRUE;
	__UNGUARD__;
	return FALSE;
}

// 登录操作
BOOL RobotManager::handleCMDStartLogin(const tagRobotCMDUtil* stCMD)
{
	__GUARD__;

	// only login
	FixedString<MAX_ACCOUNT> strAccount("IAMROBOT");
	for (int32 i = 0; i < g_Config.mRobotInfo.mRunRobotNum; ++i)
	{
		char szNum[DB_MAX_PASSWD_LEN] = { 0 };
		sprintf_s(szNum, "%d", i);
		FixedString<MAX_ACCOUNT> tempStr(szNum);
		tempStr += strAccount;
		RobotData* pRobot = mRobotPool.allocObject();
		if (pRobot)
		{
			// todo 获取某个GateMod需要可配置
			g_pModuleManager->getExportCallable < IClientSessionExportCallable < RobotManager > >(
				ServerModuleUtil::getSessionModuleBySessionID(pRobot->getSession()),
				pRobot->getSession(),
				this
				)
				->rpcRobotRequestLogon();
			pRobot->setRobotCurStatus(ERobotStatus_StableOnline);
		}
	}

	return TRUE;
	__UNGUARD__;
	return FALSE;
}

// 设置机器人不稳定操作，该操作要基于clientconnector，即机器人正常走所有的登录流程的情况下才需要该压测
// 基于session的机器人不需要进行不稳定测试
BOOL RobotManager::handleCMDRobotUnstable(const tagRobotCMDUtil* stCMD)
{
	__GUARD__;

	// only login
	FixedString<MAX_ACCOUNT> strAccount("IAMROBOT");
	for (int32 i = 0; i < stCMD->mControlNum; ++i)
	{
		char szNum[DB_MAX_PASSWD_LEN] = { 0 };
		sprintf_s(szNum, "%d", i);
		FixedString<MAX_ACCOUNT> tempStr(szNum);
		tempStr += strAccount;
		RobotData* pRobot = mRobotPool.allocObject();
		if (pRobot)
		{
			pRobot->setRobotCurStatus(ERobotStatus_UnStableOnline);
		}
	}

	return TRUE;
	__UNGUARD__;
	return FALSE;
}


// 世界发言
BOOL RobotManager::handleCMDTalkInTheWorld(const tagRobotCMDUtil* stCMD)
{
	__GUARD__;



	return TRUE;
	__UNGUARD__;
	return FALSE;
}

// 与单人聊天
BOOL RobotManager::handleCMDTalkToSomebody(const tagRobotCMDUtil* stCMD)
{
	__GUARD__;



	return TRUE;
	__UNGUARD__;
	return FALSE;
}

// 清理所有机器人 DB、Reg、OnlineMgr、OnlinePlayer、SnidAlloc
BOOL RobotManager::handleCMDClearAllRobot(const tagRobotCMDUtil* stCMD)
{
	__GUARD__;



	return TRUE;
	__UNGUARD__;
	return FALSE;
}