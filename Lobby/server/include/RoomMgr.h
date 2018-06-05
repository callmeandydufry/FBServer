#ifndef __ROOM_MGR_H__
#define __ROOM_MGR_H__

#include "TypeDefine.h"
#include "ServerModuleBase.h"
#include "RoomDefine.h"
#include "ServerModuleExportInterface.h"
//#include "RoomData.h"

#define MAX_TAB_ROBOT_NUM (1000)
#define MAX_TAB_ITEM_NUM (1000)

struct TabRootPlayerInfo
{
	int mIndex;
	SNID_t mPlayerID;
	FixedString<MAX_CHARACTER_NAME> mStrName;
	float mHp;
	float mAttack;
	float mDef;

	TabRootPlayerInfo()
	{
		clear();
	}

	void clear()
	{
		mIndex = 0;
		mAttack = 0;
		mHp = 0;
		mDef = 0;
		mPlayerID = 0;
		mStrName.clear();
	}
};

struct STConfigRobotInfo
{
	TabRootPlayerInfo mRobotInfo[MAX_TAB_ROBOT_NUM];
	int mNums;

	STConfigRobotInfo()
	{
		clear();
	}

	void clear()
	{
		for (int i = 0; i < MAX_TAB_ROBOT_NUM; ++i)
		{
			mRobotInfo[i].clear();
		}
	}
};

struct TabItemInfo
{
	int mIndex;
	int mItemID;
	FixedString<MAX_CHARACTER_NAME> mStrName;
	float mHp;
	float mAttack;
	float mDef;

	TabItemInfo()
	{
		clear();
	}

	void clear()
	{
		mIndex = 0;
		mAttack = 0;
		mHp = 0;
		mDef = 0;
		mStrName.clear();
		mItemID = 0;
	}
};

struct STConfigItemInfo
{
	TabItemInfo mItemInfo[MAX_TAB_ITEM_NUM];
	int mNums;

	STConfigItemInfo()
	{
		clear();
	}

	void clear()
	{
		for (int i = 0; i < MAX_TAB_ITEM_NUM; ++i)
		{
			mItemInfo[i].clear();
		}
	}
};

class RoomData;
class RoomMgr
	: public ServerModuleBase
	, public IRoomMgr
{
public:
	RoomMgr();
	~RoomMgr();
	void tick(int tickCount);
	void initRoomMgr();
public:
	// todo 现在先从tab中读取玩家数据进行操作
	//virtual void rpcTry2JoinRoom(SNID_t snid, int roomID); // , PlayerData stData);
	//virtual void rpcTry2LeaveRoom(SNID_t snid, int roomID);
	
public:
	bool startBattle();

private:
	void loadRobotPlayerInfo();
	void loadItemInfo();
private:
	//void rpcCallback_rpcTry2JoinRoom(PlayerData* pData)
	//{
	// already get data
	// class callback or function callback
	// try to get room id
	// RoomData.joinRoom();
	//}


private:
	// func

private:
	// data
	RoomData*	mArrRoomData[MAX_ROOM_NUM];
	int			mRealRoomNum;

public:
	STConfigRobotInfo mTABRobotInfo;
	STConfigItemInfo  mTABItemInfo;
private:

};

#endif // __ROOM_MGR_H__
