#include "stdafx.h"
#include "PlayerArchiveStruct.h"

PlayerArchive::PlayerArchive()
	: mIsIgnorePropDirtyFlag(FALSE)
{
	mDBState.clear();
	mDBState.initIgnoreDirtyFlagPointer(&mIsIgnorePropDirtyFlag);
}

void PlayerArchive::clear()
{
	mDBState.clear();
}

BOOL PlayerArchive::serialize(DataStream &oStream) const
{
	mDBState.serialize(oStream);
	return TRUE;
}

BOOL PlayerArchive::serialize(DataStream &oStream, BOOL forceAll)
{
	BOOL temp = mIsIgnorePropDirtyFlag;
	mIsIgnorePropDirtyFlag = forceAll;
	mDBState.serialize(oStream);
	mIsIgnorePropDirtyFlag = temp;
	return TRUE;
}

BOOL PlayerArchive::unserialize(DataStream &iStream)
{
	iStream.getValue(mDBState);
	return TRUE;
}

void PlayerArchive::serializeJson(JsonSerializer* serializer)
{
	serializer->serialize("DBPlayerState", mDBState, (int32)sizeof(mDBState));
}