#include "stdafx.h"
#include "RegisterPlayerStruct.h"

//----------------------------------------------------------------------------------------------
RegisterPlayerArchive::RegisterPlayerArchive()
	: mIsIgnorePropDirtyFlag(FALSE)
{
	mDBState.clear();
	mDBState.initIgnoreDirtyFlagPointer(&mIsIgnorePropDirtyFlag);
}

void RegisterPlayerArchive::clear()
{
	mDBState.clear();
}

BOOL RegisterPlayerArchive::serialize(DataStream &oStream) const
{
	mDBState.serialize(oStream);
	return TRUE;
}

BOOL RegisterPlayerArchive::serialize(DataStream &oStream, BOOL forceAll)
{
	BOOL temp = mIsIgnorePropDirtyFlag;
	mIsIgnorePropDirtyFlag = forceAll;
	mDBState.serialize(oStream);
	mIsIgnorePropDirtyFlag = temp;
	return TRUE;
}

BOOL RegisterPlayerArchive::unserialize(DataStream &iStream)
{
	iStream.getValue(mDBState);
	return TRUE;
}

void RegisterPlayerArchive::serializeJson(JsonSerializer* serializer)
{
	serializer->serialize("DBPlayerState", mDBState, (int32)sizeof(mDBState));
}

//----------------------------------------------------------------------------------------------
void BatchRegPlayerArchive::clear()
{
	mBatchNum = 0;
	for (int32 i = 0; i < BATCH_REGPLAYER_NUM; ++i)
	{
		mBatchPlayerData[i].clear();
	}
}

BOOL BatchRegPlayerArchive::serialize(DataStream &oStream) const
{
	oStream.pushValue(mBatchNum);
	for (int32 i = 0; i < mBatchNum; ++i)
	{
		oStream.pushValue(mBatchPlayerData[i]);
	}
	return TRUE;
}

BOOL BatchRegPlayerArchive::serialize(DataStream &oStream, BOOL forceAll)
{
	oStream.pushValue(mBatchNum);
	for (int32 i = 0; i < mBatchNum; ++i)
	{
		mBatchPlayerData[i].serialize(oStream, forceAll);
	}
	return TRUE;
}

BOOL BatchRegPlayerArchive::unserialize(DataStream &iStream)
{
	iStream.getValue(mBatchNum);
	for (int32 i = 0; i < mBatchNum; ++i)
	{
		iStream.getValue(mBatchPlayerData[i]);
	}
	return TRUE;
}

void BatchRegPlayerArchive::serializeJson(JsonSerializer* serializer)
{
	serializer->serialize("BatchNum", mBatchNum, (int32)sizeof(mBatchNum));
	serializer->serialize("BatchPlayerData", mBatchPlayerData, (int32)sizeof(mBatchPlayerData));
}