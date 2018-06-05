#ifndef __ROOM_DATA_H__
#define __ROOM_DATA_H__
#include "RoomDefine.h"

class PlayerData
{

};

class RoomData
{
public:
	RoomData();
	~RoomData();
public:
	bool initRoomData();
	bool joinRoom(SNID_t snid); //, PlayerData* pData);
	bool leaveRoom(SNID_t snid);
	void tick(int tickCount);

private:
	bool prepareForTheCommbat();
	void lockRoom(bool bLock);


private:
	std::map<SNID_t, PlayerData*> mPlayerDataMap;
	PlayerData mArrPlayerData[MAX_PLAYER_NUM_IN_ONEROOM];
	bool bLock;
};

#endif // __ROOM_DATA_H__
