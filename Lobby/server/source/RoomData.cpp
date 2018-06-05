#include "stdafx.h"
#include "RoomData.h"
#include "FixedString.h"
#include <cstdlib>
#include <ctime>

RoomData::RoomData()
{

}

RoomData::~RoomData()
{

}

bool RoomData::initRoomData(RoomMgr* pMgr)
{
	if (NULL == pMgr) return false;
	mPtrRoomMgr = pMgr;
	srand((int)time(0));

	mPlayerDataMap.clear();
	memset(mArrPlayerData, 0, MAX_PLAYER_NUM_IN_ONEROOM);
	mRoomPlayerNum = 0;
	mBattleNums = 0;
	memset(mVecBallePlayer, 0, MAX_PLAYER_NUM_IN_ONEROOM);
	bLock = false;

	return true;
}

bool RoomData::joinRoom(SNID_t snid, PlayerData* pData)
{
	// rpc call func
	return true;
}

void RoomData::tick(int tickCount)
{
	// battle logic
	for (int i = 0; i < MAX_PLAYER_NUM_IN_ONEROOM; ++i)
	{
		if (!mArrPlayerData[i].isValid()) continue;
		switch (mArrPlayerData[i].eStateType)
		{
		case ERSIR_Idle:
		{
			// 三种状态随机选择
			setRandomState(mArrPlayerData[i].mPlayerID);
		}
		break;
		case ERSIR_Grap:
		{
			// 获取表格物品,输出log
			getRandomItem(mArrPlayerData[i].mPlayerID);
			// battle/idle/grap 三种状态随机选择,并设置
			setRandomState(mArrPlayerData[i].mPlayerID);
		}
		break;
		case ERSIR_Battle:
		{
			// 放入 battlelist
			addToBattleList(mArrPlayerData[i].mPlayerID);
		}
		break;
		case ERSIR_AfterBattle:
		{
			// 设置状态为idle
			mArrPlayerData[i].eStateType = ERSIR_Idle;
		}
		break;
		}
		
	}

	// 遍历battlelist.
	int nRandBattle1 = 0;
	int nRandBattle2 = 0;

	if (mBattleNums > 1)
	{
		for (int i = 0; i < mBattleNums / 2 - 1; ++i)
		{
			// maybe logic bug
			if (mBattleNums > 1)
			{
				nRandBattle1 = rand() % mBattleNums;
				nRandBattle2 = rand() % mBattleNums;
				mVecBallePlayer[nRandBattle1] = mVecBallePlayer[--mBattleNums];
				mVecBallePlayer[nRandBattle2] = mVecBallePlayer[--mBattleNums];
				getBattleResult(mVecBallePlayer[nRandBattle1], mVecBallePlayer[nRandBattle2]);
			}
		}
	}

	// every step write log

}

bool RoomData::leaveRoom(SNID_t snid)
{
	return true;
}

bool RoomData::prepareForTheCommbat()
{
	lockRoom(true);
	return true;
}

void RoomData::lockRoom(bool bLock)
{
	bLock = bLock;
}

bool RoomData::addToBattleList(SNID_t snid)
{
	if (!snid) return false;
	if (mBattleNums >= MAX_PLAYER_NUM_IN_ONEROOM) return false;

	mVecBallePlayer[mBattleNums++] = snid;

	return true;
}

bool RoomData::removeFromBattleList(SNID_t snid)
{
	if (!snid) return false;
	if (mBattleNums <= 0) return false;
	int nFind = MAX_PLAYER_NUM_IN_ONEROOM;
	for (int i = 0; i < mBattleNums; ++i)
	{
		if (mVecBallePlayer[i] == snid)
		{
			nFind = i;
			break;
		}
	}

	if (MAX_PLAYER_NUM_IN_ONEROOM == nFind) return false;
	
	mVecBallePlayer[--mBattleNums] = mVecBallePlayer[nFind];

	return true;
}

void RoomData::setRandomState(SNID_t snid)
{
	int nRand = rand() % ERSIR_END;
	std::map<SNID_t, PlayerData*>::iterator it = mPlayerDataMap.find(snid);
	if (it != mPlayerDataMap.end())
	{
		it->second->eStateType = (EPlayerStateInRoom)nRand;
	}
}

void RoomData::setPlayerState(SNID_t snid, EPlayerStateInRoom eType)
{
	if (INVALID == snid) return;
	if (eType <= EPSIR_INVALID || eType >= ERSIR_END) return;
	int nRand = rand() % ERSIR_END;
	std::map<SNID_t, PlayerData*>::iterator it = mPlayerDataMap.find(snid);
	if (it != mPlayerDataMap.end())
	{
		it->second->eStateType = eType;
	}
}

// 随机获得物品
void RoomData::getRandomItem(SNID_t snid)
{
	if (INVALID == snid) return;
	// 读取表单 获取总数 随机 
	if (NULL == mPtrRoomMgr) return;
	int nTabNums = mPtrRoomMgr->mTABItemInfo.mNums;
	int nRand = rand() % (0 == nTabNums ? 1 : nTabNums);
	if (nRand >= nTabNums) return;
	FixedString<MAX_CHARACTER_NAME> strName = mPtrRoomMgr->mTABItemInfo.mItemInfo[nRand].mStrName;

	// writelog


}

// 随机取胜
void RoomData::getBattleResult(SNID_t nPlayer1, SNID_t nPlayer2)
{
	bool bWiner = rand() % 2 == 0 ? false : true;
	
	SNID_t winerID = bWiner ? nPlayer1 : nPlayer2;

	// after battle
	setPlayerState(nPlayer1, ERSIR_AfterBattle);
	setPlayerState(nPlayer2, ERSIR_AfterBattle);

	// write log
}