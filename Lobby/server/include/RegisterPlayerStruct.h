#ifndef __REGISTER_PLAYER_STRUCT_H__
#define __REGISTER_PLAYER_STRUCT_H__

#include "PropertyPlayerState.h"

//----------------------------------------------------------------------------------------------
// ע����һ������ԣ�������������Ϊȫ�����ݵ�ժҪ [12/15/2017 yz]
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
// ����������ȡע��������� [1/2/2018 yz]
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