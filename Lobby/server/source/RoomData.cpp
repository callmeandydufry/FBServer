#include "stdafx.h"
#include "RoomData.h"

RoomData::RoomData()
{

}

RoomData::~RoomData()
{

}

bool RoomData::initRoomData()
{
	return true;
}

bool RoomData::joinRoom(SNID_t snid) // , PlayerData* pData)
{
	return true;
}

void RoomData::tick(int tickCount)
{

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