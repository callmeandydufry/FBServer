#ifndef __ROBOT_DATA_H__
#define __ROBOT_DATA_H__

#include "ServerModuleExportInterface.h"

enum ERobotStatus
{
	ERobotStatus_INVALID,
	ERobotStatus_Inited,
	ERobotStatus_StableOnline,			// 稳定在线
	ERobotStatus_UnStableOnline,		// 不稳定在线
	ERobotStatus_StableOffline,
	ERobotStatus_UnStableOffline,		
	ERobotStatus_Talk2Others,
	ERobotStatus_TalkInTheWorld,
};

class RobotData
	: public IRobotData
	, public KThread
{
	

public:
	RobotData() {}
	~RobotData() {};

	void						setRobotCurStatus(int32 eStatus) { mStatus = (ERobotStatus)eStatus; }
	int32						getRobotCurStatus() { return mStatus; }
public:
	void						tickComponent(uint32 uTick);
	virtual int64				getComponentId();

public:
	void						Clear();

	void						setAccountName(FixedString<MAX_ACCOUNT>& strName);
	void						setToken(FixedString<MAX_ACCOUNT>& strToken);
	void						setPlayerID(SNID_t snid);
	void						setSession(uint64 nSessionID);

	FixedString<MAX_ACCOUNT>&	getAccountName();
	FixedString<MAX_ACCOUNT>&	getToken();
	SNID_t						getPlayerID();
	uint64						getSession();
private:
	FixedString<MAX_ACCOUNT>	mAccountName;
	FixedString<MAX_ACCOUNT>	mToken;
	SNID_t						m64PlayerID;
	uint64						m64Session;
	ERobotStatus				mStatus;
	CTinyTimer					mChangeTimer; // 不稳定状态下，每隔一段时间进行一次上下线请求
}; 

#endif // __ROBOT_DATA_H__
