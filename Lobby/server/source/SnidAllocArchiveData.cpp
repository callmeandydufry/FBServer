#include "stdafx.h"
#include "SnidAllocArchiveData.h"

#if 0

//----------------------------------------------------------------------------------------------
SnidAllocArchive::SnidAllocArchive()
	: mIsIgnorePropDirtyFlag(FALSE)
{
	mDBState.clear();
	mDBState.initIgnoreDirtyFlagPointer(&mIsIgnorePropDirtyFlag);
}

void SnidAllocArchive::clear()
{
	mDBState.clear();
	mIsIgnorePropDirtyFlag = TRUE;
}

BOOL SnidAllocArchive::serialize(DataStream &oStream) const
{
	mDBState.serialize(oStream);
	return TRUE;
}

BOOL SnidAllocArchive::serialize(DataStream &oStream, BOOL forceAll)
{
	BOOL temp = mIsIgnorePropDirtyFlag;
	mIsIgnorePropDirtyFlag = forceAll;
	mDBState.serialize(oStream);
	mIsIgnorePropDirtyFlag = temp;
	return TRUE;
}

BOOL SnidAllocArchive::unserialize(DataStream &iStream)
{
	iStream.getValue(mDBState);
	return TRUE;
}

void SnidAllocArchive::serializeJson(JsonSerializer* serializer)
{
	serializer->serialize("DBPlayerState", mDBState, (int32)sizeof(mDBState));
}

//----------------------------------------------------------------------------------------------
void BatchSnidAllocArchive::clear()
{
	mBatchNum = 0;
	for (int32 i = 0; i < BATCH_SNID_ALLOC_NUM; ++i)
	{
		mBatchSnidData[i].clear();
	}
}

BOOL BatchSnidAllocArchive::serialize(DataStream &oStream) const
{
	oStream.pushValue(mBatchNum);
	for (int32 i = 0; i < mBatchNum; ++i)
	{
		oStream.pushValue(mBatchSnidData[i]);
	}
	return TRUE;
}

BOOL BatchSnidAllocArchive::serialize(DataStream &oStream, BOOL forceAll)
{
	oStream.pushValue(mBatchNum);
	for (int32 i = 0; i < mBatchNum; ++i)
	{
		mBatchSnidData[i].serialize(oStream, forceAll);
	}
	return TRUE;
}

BOOL BatchSnidAllocArchive::unserialize(DataStream &iStream)
{
	iStream.getValue(mBatchNum);
	for (int32 i = 0; i < mBatchNum; ++i)
	{
		iStream.getValue(mBatchSnidData[i]);
	}
	return TRUE;
}

void BatchSnidAllocArchive::serializeJson(JsonSerializer* serializer)
{
	serializer->serialize("BatchNum", mBatchNum, (int32)sizeof(mBatchNum));
	serializer->serialize("BatchPlayerData", mBatchSnidData, (int32)sizeof(mBatchSnidData));
}

#endif