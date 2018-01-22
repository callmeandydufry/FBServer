#ifndef __SNID_ALLOC_ARCHIVE_DATA_H__
#define __SNID_ALLOC_ARCHIVE_DATA_H__

#include "SnidAllocArchiveData.h"
#include "PropertySnidAccountRelation.h"

#if 0

class SnidAllocArchive
{
public:
	SnidAllocArchive();
	~SnidAllocArchive() {};

	void clear();

	BOOL serialize(DataStream &oStream) const;
	BOOL serialize(DataStream &oStream, BOOL forceAll);

	BOOL unserialize(DataStream &iStream);

	void serializeJson(JsonSerializer* serializer);

public:
 	BOOL mIsIgnorePropDirtyFlag;
	DBSnidAllocPro mDBState;
};

//----------------------------------------------------------------------------------------------
#define BATCH_SNID_ALLOC_NUM (600)
class BatchSnidAllocArchive
{
public:
	BatchSnidAllocArchive()
		: mBatchNum(0)
	{}
	~BatchSnidAllocArchive() {}

	void clear();

	BOOL serialize(DataStream &oStream) const;
	BOOL serialize(DataStream &oStream, BOOL forceAll);

	BOOL unserialize(DataStream &iStream);

	void serializeJson(JsonSerializer* serializer);

public:
	int32 mBatchNum;
	SnidAllocArchive mBatchSnidData[BATCH_SNID_ALLOC_NUM];
};

#endif

#endif // __SNID_ALLOC_ARCHIVE_DATA_H__