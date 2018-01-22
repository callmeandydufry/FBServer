#ifndef __PLAYER_DBPROXY_H__
#define __PLAYER_DBPROXY_H__

#include "TypeDefine.h"

class PlayerArchive;
class RegisterPlayerArchive;
class BatchRegPlayerArchive;
class DBProxyManager;

class PlayerDBProxy
{
public:
	PlayerDBProxy(DBProxyManager* mgr)
		: mDBManager(mgr)
	{}

	// 创建新玩家 [12/15/2017 yz]
	void doCreateNewPlayerArchive(RegisterPlayerArchive* archive);

	// 保存玩家全部数据到DB [12/15/2017 yz]
	BOOL doSavePlayerArchive(PlayerArchive* archive);

	// 通过SNID查询玩家数据 [12/15/2017 yz]
	BOOL doFetchPlayerArchiveBySnid(SNID_t snid, PlayerArchive* archive);

	// 查询给定SNID区间内的玩家总数 [1/2/2018 yz]
	BOOL doFetchPlayerNumInSnidRange(int32 serverID, SNID_t begin, SNID_t end, int32& playerNum);

	// 获取给定区间内多个玩家的注册玩家数据 [1/2/2018 yz]
	BOOL doFetchRegisterPlayerInSnidRange(int32 serverID, SNID_t begin, int32 limit, BatchRegPlayerArchive* batchPlayerData);

protected:
	DBProxyManager*					mDBManager;

	DBModelPlayerState				mDBModelPlayerState;
	DBModelRegisterPlayerState		mDBModelRegisterPlayerState;
};

#endif