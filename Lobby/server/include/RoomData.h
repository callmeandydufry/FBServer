#ifndef __ROOM_DATA_H__
#define __ROOM_DATA_H__
#include "RoomDefine.h"
#include "RoomMgr.h"

class RoomMgr;

enum EPlayerStateInRoom
{
	EPSIR_INVALID,

	ERSIR_Idle,
	ERSIR_Grap,
	ERSIR_Battle,
	ERSIR_AfterBattle,

	ERSIR_END,
};

class PlayerData
{
public:
	bool isValid()
	{
		return (mPlayerID != INVALID);
	}
public:
	SNID_t				mPlayerID;
	EPlayerStateInRoom	eStateType;
	FixedString<MAX_CHARACTER_NAME> mName;
};

class RoomData
{
public:
	RoomData();
	~RoomData();
public:
	bool initRoomData(RoomMgr* pMgr);
	bool joinRoom(SNID_t snid, PlayerData* pData);
	bool leaveRoom(SNID_t snid);
	void tick(int tickCount);

private:
	bool prepareForTheCommbat();
	void lockRoom(bool bLock);

private:
	bool addToBattleList(SNID_t snid);
	bool removeFromBattleList(SNID_t snid);
	void setRandomState(SNID_t snid);
	void setPlayerState(SNID_t snid, EPlayerStateInRoom eType);

	// ��������Ʒ
	void getRandomItem(SNID_t snid);
	// ���ȡʤ
	void getBattleResult(SNID_t nPlayer1, SNID_t nPlayer2);
private:
	std::map<SNID_t, PlayerData*> mPlayerDataMap;
	PlayerData mArrPlayerData[MAX_PLAYER_NUM_IN_ONEROOM];
	int mRoomPlayerNum;

	int mBattleNums; // ��Ը����ս��������
	SNID_t mVecBallePlayer[MAX_PLAYER_NUM_IN_ONEROOM];

	// pMgr
	RoomMgr*	mPtrRoomMgr;

	bool bLock;
};

#endif // __ROOM_DATA_H__
