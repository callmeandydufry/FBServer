#ifndef __ROOM_MGR_H__
#define __ROOM_MGR_H__

#include "TypeDefine.h"
#include "ServerModuleBase.h"
#include "RoomDefine.h"
#include "ServerModuleExportInterface.h"
#include "RoomData.h"

class RoomData;

class RoomMgr
	: public ServerModuleBase
	, public IRoomMgr
{
public:
	RoomMgr();
	~RoomMgr();
	void tick(int tickCount);
public:
	virtual void rpcTry2JoinRoom(SNID_t snid, int roomID) {}; // , PlayerData stData);
	virtual void rpcTry2LeaveRoom(SNID_t snid, int roomID) {};

private:

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
	RoomData mArrRoomData[MAX_ROOM_NUM];
private:

};

#endif // __ROOM_MGR_H__
