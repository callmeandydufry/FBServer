#ifndef __DEFINE_PROPERTY_STRUCT_H__
#define __DEFINE_PROPERTY_STRUCT_H__

#define DefinePropertyArchive(NAME)  \
class NAME##Archive \
{\
public:\
	NAME##Archive()\
	{\
		mDBState.clear();\
		mDBState.initIgnoreDirtyFlagPointer(&mIsIgnorePropDirtyFlag);\
	}\
	~NAME##Archive() {}\
	void clear() { mDBState.clear(); }\
	BOOL serialize(DataStream &oStream) const\
	{\
		mDBState.serialize(oStream);\
		return TRUE;\
	}\
	BOOL serialize(DataStream &oStream, BOOL forceAll)\
	{\
		BOOL temp = mIsIgnorePropDirtyFlag;\
		mIsIgnorePropDirtyFlag = forceAll;\
		mDBState.serialize(oStream);\
		mIsIgnorePropDirtyFlag = temp;\
		return TRUE;\
	}\
	BOOL unserialize(DataStream &iStream)\
	{\
		iStream.getValue(mDBState);\
		return TRUE;\
	}\
	void serializeJson(JsonSerializer* serializer);\
public:\
	BOOL mIsIgnorePropDirtyFlag;\
	DB##NAME##State mDBState;\
};\
class Batch##NAME##Archive\
{\
public:\
	Batch##NAME##Archive(): mBatchNum(0){}\
	~Batch##NAME##Archive() {}\
	void clear()\
	{\
		mBatchNum = 0;\
		for (int32 i = 0; i < BATCH_##NAME##_NUM; ++i)\
		{\
			mBatch##NAME##Data[i].clear();\
		}\
	}\
	BOOL serialize(DataStream &oStream) const\
	{\
		oStream.pushValue(mBatchNum);\
		for (int32 i = 0; i < mBatchNum; ++i)\
		{\
			oStream.pushValue(mBatch##NAME##Data[i]);\
		}\
		return TRUE;\
	}\
	BOOL serialize(DataStream &oStream, BOOL forceAll)\
	{\
		oStream.pushValue(mBatchNum);\
		for (int32 i = 0; i < mBatchNum; ++i)\
		{\
			mBatch##NAME##Data[i].serialize(oStream, forceAll);\
		}\
		return TRUE; \
	}\
	BOOL unserialize(DataStream &iStream)\
	{\
		iStream.getValue(mBatchNum);\
		for (int32 i = 0; i < mBatchNum; ++i)\
		{\
			iStream.getValue(mBatch##NAME##Data[i]);\
		}\
		return TRUE;\
	}\
	void serializeJson(JsonSerializer* serializer);\
public:\
	int32 mBatchNum;\
	NAME##Archive mBatch##NAME##Data[BATCH_##NAME##_NUM];\
};

#endif // __DEFINE_PROPERTY_STRUCT_H__
