#ifndef __REGISTER_PLAYER_STRUCT_H__
#define __REGISTER_PLAYER_STRUCT_H__

#include "PropertyPlayerState.h"

//----------------------------------------------------------------------------------------------
// 注册玩家基本属性，属性量不大，作为全部数据的摘要 [12/15/2017 yz]
class RegisterPlayerArchive
{
public:
	RegisterPlayerArchive();
	~RegisterPlayerArchive() {}

	void clear();

	BOOL serialize(DataStream &oStream) const;
	BOOL serialize(DataStream &oStream, BOOL forceAll);

	BOOL unserialize(DataStream &iStream);

	void serializeJson(JsonSerializer* serializer);

public:
	BOOL mIsIgnorePropDirtyFlag;
	DBRegisterPlayerState mDBState;
};


//----------------------------------------------------------------------------------------------
// 用于批量获取注册玩家数据 [1/2/2018 yz]
#define BATCH_REGPLAYER_NUM (372)
class BatchRegPlayerArchive
{
public:
	BatchRegPlayerArchive()
		: mBatchNum(0)
	{}
	~BatchRegPlayerArchive(){}

	void clear();

	BOOL serialize(DataStream &oStream) const;
	BOOL serialize(DataStream &oStream, BOOL forceAll);

	BOOL unserialize(DataStream &iStream);

	void serializeJson(JsonSerializer* serializer);

public:
	int32 mBatchNum;
	RegisterPlayerArchive mBatchPlayerData[BATCH_REGPLAYER_NUM];
};

#endif