#include "stdafx.h"
#include "RoomMgr.h"
#include "TAB.h"
#include "FileConfigConst.h"
#include "RoomData.h"

RoomMgr::RoomMgr()
{

}

RoomMgr::~RoomMgr()
{
	delete[] mArrRoomData;
}

void RoomMgr::initRoomMgr()
{
	// read tab robot info 
	loadRobotPlayerInfo();
	// read tab item info
	loadItemInfo();
	// init roomData from mgr
	for (int i = 0; i < MAX_ROOM_NUM; ++i)
	{
		mArrRoomData[i] = new RoomData;
		mArrRoomData[i]->initRoomData(this);
	}
}

bool RoomMgr::startBattle()
{
	// all players join the room

	// check timer && start battle tick

	return true;
}

void RoomMgr::tick(int tickCount)
{
	if (0 == mRealRoomNum) return;
	for (int i = 0; i < mRealRoomNum; ++i)
	{
		mArrRoomData[i]->tick(tickCount);
	}
}

void RoomMgr::loadRobotPlayerInfo()
{
	enum
	{
		eIndex = 0,
		ePlayerID = 1,
		eName = 2,
		eHP = 3,
		eAttack = 4,
		eDef = 5,
	};

	TAB::TABFile  tabFile(0);
 	BOOL	ret = tabFile.OpenFile_From_TEXT(FILE_ROBOTINFO_TAB);
 	KCheckEx(ret, FILE_ROBOTINFO_TAB);
 
	int32 iTableCount = tabFile.GetRowsNum();		//行
	int32 iTableFields = tabFile.GetFieldsNum();	//列
	KCheck(iTableCount <= 1000);

	mTABRobotInfo.mNums = iTableCount;

	for (uint32 i = 0; i < iTableCount; i++)
	{
		int32 nIndex = tabFile.SearchField(i, eIndex)->iValue;

		mTABRobotInfo.mRobotInfo[i].mIndex = i;
		mTABRobotInfo.mRobotInfo[i].mPlayerID = tabFile.SearchField(i, ePlayerID)->iValue;
		mTABRobotInfo.mRobotInfo[i].mStrName = tabFile.SearchField(i, eName)->pString;
		mTABRobotInfo.mRobotInfo[i].mHp = tabFile.SearchField(i, eHP)->fValue;
		mTABRobotInfo.mRobotInfo[i].mAttack = tabFile.SearchField(i, eAttack)->fValue;
		mTABRobotInfo.mRobotInfo[i].mDef = tabFile.SearchField(i, eDef)->fValue;
	}
}

void RoomMgr::loadItemInfo()
{
	enum
	{
		eIndex = 0,
		ePlayerID = 1,
		eName = 2,
		eHP = 3,
		eAttack = 4,
		eDef = 5,
	};

	TAB::TABFile  tabFile(0);
	BOOL	ret = tabFile.OpenFile_From_TEXT(FILE_ITEMINFO_TAB);
	KCheckEx(ret, FILE_ITEMINFO_TAB);

	int32 iTableCount = tabFile.GetRowsNum();		//行
	int32 iTableFields = tabFile.GetFieldsNum();	//列
	KCheck(iTableCount <= 1000);

	mTABItemInfo.mNums = iTableCount;

	for (uint32 i = 0; i < iTableCount; i++)
	{
		int32 nIndex = tabFile.SearchField(i, eIndex)->iValue;

		mTABItemInfo.mItemInfo[i].mIndex = i;
		mTABItemInfo.mItemInfo[i].mItemID = tabFile.SearchField(i, ePlayerID)->iValue;
		mTABItemInfo.mItemInfo[i].mStrName = tabFile.SearchField(i, eName)->pString;
		mTABItemInfo.mItemInfo[i].mHp = tabFile.SearchField(i, eHP)->fValue;
		mTABItemInfo.mItemInfo[i].mAttack = tabFile.SearchField(i, eAttack)->fValue;
		mTABItemInfo.mItemInfo[i].mDef = tabFile.SearchField(i, eDef)->fValue;
	}
}