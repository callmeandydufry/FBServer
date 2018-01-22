
#ifndef __STRUCT_MACRO_H__
#define __STRUCT_MACRO_H__
#pragma warning (disable:4996)

#include "TypeDefine.h"
#include "Define.h"
#include "KwNetMessage.h"
#include "KwNetStreamSystem.h"
#include "BaseTool.h"
#include "Array_Base.h"
#include "DBCore.h"
#include "ShareStruct.h"

//------------------------------------------------------------------------------------------
// 按这个格式定义属性列表 [12/12/2017 yz]
// #define STRUCT_PROPERTYS(PROPERTY)	\
// 	PROPERTY(BOOL, TestA)\
// 	PROPERTY(int32, TestB)

#define STRUCT_PROPERTY_ENUM(TYPE, NAME, DEFAULTVAL)	\
	EPROPERTY_##NAME,

#define STRUCT_PROPERTY_MEMBER(TYPE, NAME, DEFAULTVAL)	\
	TYPE m##NAME;

#define STRUCT_PROPERTY_RESETDEFAULT(TYPE, NAME, DEFAULTVAL)	\
	m##NAME = DEFAULTVAL;

#define STRUCT_PROPERTY_SET_GET(TYPE, NAME, DEFAULTVAL)	\
	void set##NAME(const TYPE& val)	\
	{\
		m##NAME = val;\
		mPropertyFlags.MarkFlagByIndex(EPROPERTY_##NAME);\
	}\
	const TYPE& get##NAME() const\
	{\
		return m##NAME;\
	}\
	void set##NAME##Dirty(BOOL dirty) { if(dirty) {mPropertyFlags.MarkFlagByIndex(EPROPERTY_##NAME);} else { mPropertyFlags.Clear_FlagByIndex(EPROPERTY_##NAME); } }	\
	BOOL get##NAME##Dirty() const { if(mIsIgnoreDirtyFlags && *mIsIgnoreDirtyFlags == TRUE) { return TRUE; } else { return mPropertyFlags.GetFlagByIndex(EPROPERTY_##NAME);} }	\


#define STRUCT_PROPERTY_SERIALIZE(TYPE, NAME, DEFAULTVAL)	\
	if (get##NAME##Dirty())	\
	{	\
		oStream.pushValue(m##NAME);	\
	}

#define STRUCT_PROPERTY_UNSERIALIZE(TYPE, NAME, DEFAULTVAL)	\
	if (get##NAME##Dirty())	\
	{\
		iStream.getValue(m##NAME);\
	}

#define STRUCT_PROPERTY_JSON_SERIALIZE(TYPE, NAME, DEFAULTVAL)	\
		serializer->serialize(#NAME, m##NAME, (int32)sizeof(m##NAME));

#define STRUCT_PROPERTY_COPY_PROP_POINTER(TYPE, NAME, DEFAULTVAL)	\
		set##NAME(other->get##NAME());

#define STRUCT_PROPERTY_LIST(PROP_LIST)	\
	public:\
	enum PropFlag	\
	{\
		PROP_LIST(STRUCT_PROPERTY_ENUM)\
		EPROPERTY_MAX,\
	};\
	protected:\
	BOOL* mIsIgnoreDirtyFlags;	\
	BitFlags<EPROPERTY_MAX> mPropertyFlags;\
	public:\
	void initIgnoreDirtyFlagPointer(BOOL* ptr){ mIsIgnoreDirtyFlags = ptr; }	\
	PROP_LIST(STRUCT_PROPERTY_MEMBER)\
	PROP_LIST(STRUCT_PROPERTY_SET_GET)\
	void clearDirtyFlags()\
	{\
		mPropertyFlags.ClearAllFlags(); \
	}\
	void markDirtyFlags()\
	{\
		mPropertyFlags.MarkAllFlags(); \
	}\
	void clear()\
	{\
		mPropertyFlags.ClearAllFlags(); \
		PROP_LIST(STRUCT_PROPERTY_RESETDEFAULT)\
	}\
	BOOL serialize(DataStream &oStream) const\
	{\
		if(mIsIgnoreDirtyFlags && *mIsIgnoreDirtyFlags)\
		{\
			BitFlags<EPROPERTY_MAX> temp;\
			temp.MarkAllFlags();\
			oStream.pushValue(temp);\
		}\
		else\
		{\
			oStream.pushValue(mPropertyFlags); \
		}\
		PROP_LIST(STRUCT_PROPERTY_SERIALIZE)\
		return TRUE;\
	}\
	BOOL unserialize(DataStream &iStream)\
	{\
		iStream.getValue(mPropertyFlags);\
		PROP_LIST(STRUCT_PROPERTY_UNSERIALIZE)\
		return TRUE;\
	}\
	void serializeJson(JsonSerializer* serializer)\
	{\
		PROP_LIST(STRUCT_PROPERTY_JSON_SERIALIZE)\
	}\
	template<typename T>	\
	void copyProperty(T* other)\
	{\
		PROP_LIST(STRUCT_PROPERTY_COPY_PROP_POINTER)\
	}

//------------------------------------------------------------------------------------------
class DBCore;

class DBModelProperty
{
public:
	enum PropType
	{
		EPT_NONE = 0,
		EPT_INT8,
		EPT_INT16,
		EPT_INT32,
		EPT_INT64,
		EPT_UINT8,
		EPT_UINT16,
		EPT_UINT32,
		EPT_UINT64,
		EPT_FLOAT32,
		EPT_FLOAT64,
		EPT_STRING,
		EPT_FIXEDSTRING,
		EPT_BINARY,
	};

	virtual const char* getPropName()
	{
		return "";
	}

	virtual const char* getPropValString()
	{
		return mValStr.c_str();
	}

	virtual PropType getPropType()
	{
		return EPT_NONE;
	}

	virtual void readFromProp(void* source)
	{
		
	}

	virtual void writeToProp(void* source)
	{
		
	}

	virtual void fetchFromDB(DBCore* dbCore)
	{

	}

	void clear()
	{
		mValStr.clear();
	}

	bool isValid()
	{
		return (!mValStr.empty());
	}

protected:
	String mValStr;
};



template<typename T>
class TDBModelPropertyBase : public DBModelProperty
{
public:
	virtual PropType getPropType()
	{
		return EPT_BINARY;
	}

protected:
	void translateToString(const T& val)
	{
		// 所有自定义类型统一序列化为json格式 [12/14/2017 yz]
		JsonWriteSerializer jsonWritter;
		val.serializeJson(&jsonWritter);
		jsonWritter.toString(mValStr);
	}

	void stringToValue(T& val)
	{
		// 所有自定义类型统一序列化为json格式 [12/14/2017 yz]
		JsonReadSerializer jsonReader;
		if (jsonReader.fromString(mValStr))
		{
			val.serializeJson(&jsonReader);
		}
	}
};

// int8 specialization [12/14/2017 yz]
template<>
class TDBModelPropertyBase<int8> : public DBModelProperty
{
public:
	virtual PropType getPropType()
	{
		return EPT_INT8;
	}

protected:
	void translateToString(const int8& val)
	{
		mValStr = StringUtil::printf("%d", (int32)val);
	}

	void stringToValue(int8& val)
	{
		val = (int8)StringUtil::parseInt(mValStr);
	}
};

// int16 specialization [12/14/2017 yz]
template<>
class TDBModelPropertyBase<int16> : public DBModelProperty
{
public:
	virtual PropType getPropType()
	{
		return EPT_INT16;
	}

protected:
	void translateToString(const int16& val)
	{
		mValStr = StringUtil::printf("%d", (int32)val);
	}

	void stringToValue(int16& val)
	{
		val = (int16)StringUtil::parseInt(mValStr);
	}
};

// int32 specialization [12/14/2017 yz]
template<>
class TDBModelPropertyBase<int32> : public DBModelProperty
{
public:
	virtual PropType getPropType()
	{
		return EPT_INT32;
	}

protected:
	void translateToString(const int32& val)
	{
		mValStr = StringUtil::printf("%d", val);
	}

	void stringToValue(int32& val)
	{
		val = StringUtil::parseInt(mValStr);
	}
};

// int64 specialization [12/14/2017 yz]
template<>
class TDBModelPropertyBase<int64> : public DBModelProperty
{
public:
	virtual PropType getPropType()
	{
		return EPT_INT64;
	}

protected:
	void translateToString(const int64& val)
	{
		mValStr = StringUtil::printf("%lld", val);
	}

	void stringToValue(int64& val)
	{
		val = StringUtil::parseInt64(mValStr);
	}
};

// uint8 specialization [12/14/2017 yz]
template<>
class TDBModelPropertyBase<uint8> : public DBModelProperty
{
public:
	virtual PropType getPropType()
	{
		return EPT_UINT8;
	}

protected:
	void translateToString(const uint8& val)
	{
		mValStr = StringUtil::printf("%u", (uint32)val);
	}

	void stringToValue(uint8& val)
	{
		val = (uint8)StringUtil::parseUnsignedInt(mValStr);
	}
};

// uint16 specialization [12/14/2017 yz]
template<>
class TDBModelPropertyBase<uint16> : public DBModelProperty
{
public:
	virtual PropType getPropType()
	{
		return EPT_UINT16;
	}

protected:
	void translateToString(const uint16& val)
	{
		mValStr = StringUtil::printf("%u", (uint32)val);
	}

	void stringToValue(uint16& val)
	{
		val = (uint16)StringUtil::parseUnsignedInt(mValStr);
	}
};

// uint32 specialization [12/14/2017 yz]
template<>
class TDBModelPropertyBase<uint32> : public DBModelProperty
{
public:
	virtual PropType getPropType()
	{
		return EPT_UINT32;
	}

protected:
	void translateToString(const uint32& val)
	{
		mValStr = StringUtil::printf("%u", val);
	}

	void stringToValue(uint32& val)
	{
		val = StringUtil::parseUnsignedInt(mValStr);
	}
};

// uint64 specialization [12/14/2017 yz]
template<>
class TDBModelPropertyBase<uint64> : public DBModelProperty
{
public:
	virtual PropType getPropType()
	{
		return EPT_UINT64;
	}

protected:
	void translateToString(const uint64& val)
	{
		mValStr = StringUtil::printf("%llu", val);
	}

	void stringToValue(uint64& val)
	{
		val = StringUtil::parseUnsignedInt64(mValStr);
	}
};

// float32 specialization [12/14/2017 yz]
template<>
class TDBModelPropertyBase<float> : public DBModelProperty
{
public:
	virtual PropType getPropType()
	{
		return EPT_FLOAT32;
	}

protected:
	void translateToString(const float& val)
	{
		mValStr = StringUtil::printf("%f", val);
	}

	void stringToValue(float& val)
	{
		val = StringUtil::parseFloat(mValStr);
	}
};

// double specialization [12/14/2017 yz]
template<>
class TDBModelPropertyBase<double> : public DBModelProperty
{
public:
	virtual PropType getPropType()
	{
		return EPT_FLOAT64;
	}

protected:
	void translateToString(const double& val)
	{
		// print内部float与double都会被提升为double，统一使用%f作为占位符 [12/14/2017 yz]
		mValStr = StringUtil::printf("%f", val);
	}

	void stringToValue(double& val)
	{
		val = StringUtil::parseDouble(mValStr);
	}
};

// String specialization [12/14/2017 yz]
template<>
class TDBModelPropertyBase<String> : public DBModelProperty
{
public:
	virtual PropType getPropType()
	{
		return EPT_STRING;
	}

protected:
	void translateToString(const String& val)
	{
		mValStr = val;
	}

	void stringToValue(String& val)
	{
		val = mValStr;
	}
};

// FixedString specialization [12/14/2017 yz]
template<int TMAXLEN, typename TSIZE_TYPE>
class TDBModelPropertyBase<FixedString<TMAXLEN, TSIZE_TYPE> > : public DBModelProperty
{
public:
	virtual PropType getPropType()
	{
		return EPT_FIXEDSTRING;
	}

protected:
	void translateToString(const FixedString<TMAXLEN, TSIZE_TYPE>& val)
	{
		mValStr = val.c_str();
	}

	void stringToValue(FixedString<TMAXLEN, TSIZE_TYPE>& val)
	{
		val = mValStr;
	}
};

//------------------------------------------------------------------------------------
// 生成DB存档时用到的数据原型 [12/14/2017 yz]
//------------------------------------------------------------------------------------

class DBCore;

class DBModelBase
{
public:
	DBModelBase()
		: m_AffectedRowCount(0)
	{

	}

	virtual ~DBModelBase()
	{
	}

	virtual const char* getDBTableName()
	{
		return "";
	}

	int32 getAffectedRowCount()
	{
		return m_AffectedRowCount;
	}

	void clear()
	{
		m_AffectedRowCount = 0;
		for (int32 i = 0; i < mAllModelProps.GetSize(); ++i)
		{
			if(mAllModelProps[i] != NULL)
				mAllModelProps[i]->clear(); 
		}
	}

	virtual BOOL doSQLInsert(DBCore* dbCore);
	virtual BOOL doSQLUpdate(DBCore* dbCore);
	virtual BOOL doSQLDelete(DBCore* dbCore);
	virtual BOOL doSQLVallidate(DBCore* dbCore);
	virtual BOOL doSQLInvallidate(DBCore* dbCore);
	virtual BOOL doSQLSelect(DBCore* dbCore);
	virtual BOOL doSQLSelectCount(DBCore* dbCore);
	virtual BOOL doSQLFetch(DBCore* dbCore, void* structPtr);
	virtual BOOL doSQLBatchInsert(DBCore* dbCore);

protected:
	virtual String getSQLSelectString();
	virtual String getSQLSelectCountString();
	virtual String getSQLInsertString();
	virtual String getSQLUpdateString();
	virtual String getSQLDeleteString();
	virtual String getSQLValidateString();
	virtual String getSQLInvalidateString();
	virtual String getSQLBatchInsertString();

	virtual String getSQLQueryConditionString()
	{
		return StringUtil::BLANK;
	}

protected:
	TDynaArray<DBModelProperty*> mAllModelProps;
	int32 m_AffectedRowCount;
};


#define DB_STRUCT_PROPERTY_ENUM(TYPE, NAME, DEFAULTVAL)	\
	EDBPROPERTY_##NAME,

#define DB_STRUCT_PROPERTY_CTOR(TYPE, NAME, DEFAULTVAL)	\
	mAllModelProps.AddItem(&mDBProp##NAME);

#define DB_STRUCT_MODEL_PROPERTY(TYPE, NAME, DEFAULTVAL)	\
public:\
	class DBProp##NAME : public TDBModelPropertyBase<TYPE>	\
	{\
		public:\
		virtual const char* getPropName()\
		{\
			return #NAME; \
		}\
		virtual void readFromProp(void* source)\
		{\
			translateToString(((SourceStructType*)source)->get##NAME()); \
		}\
		virtual void writeToProp(void* source)\
		{\
			TYPE val;\
			stringToValue(val);\
			((SourceStructType*)source)->set##NAME(val); \
		}\
		virtual void fetchFromDB(DBCore* dbCore)\
		{\
			mValStr.clear(); \
			const char* str = dbCore->GetColData((int32)EDBPROPERTY_##NAME); \
			if(str) { mValStr = str; } \
		}\
	};\
	DBProp##NAME mDBProp##NAME;

#define DB_STRUCT_READ_FROM_PROPERTY(TYPE, NAME, DEFAULTVAL)	\
	if(source->get##NAME##Dirty())\
	{\
		mDBProp##NAME.readFromProp(source); \
	}\

// DB数据库中一行数据的数据模型 [12/14/2017 yz]
#define DB_STRUCT_DATA_MODEL(MODELCLSNAME, SOURCE_STRUCT_TYPE, PROP_LIST)	\
class __DBModel##SOURCE_STRUCT_TYPE##Base : public DBModelBase	\
{ \
public:	\
	typedef SOURCE_STRUCT_TYPE SourceStructType; \
	enum DBPropType	\
	{\
		PROP_LIST(DB_STRUCT_PROPERTY_ENUM)\
		EDBPROPERTY_MAX,\
	}; \
	PROP_LIST(DB_STRUCT_MODEL_PROPERTY)\
	__DBModel##SOURCE_STRUCT_TYPE##Base()\
		{\
			PROP_LIST(DB_STRUCT_PROPERTY_CTOR)\
		}\
	void readFromProps(SourceStructType* source)\
	{\
		PROP_LIST(DB_STRUCT_READ_FROM_PROPERTY)\
	}\
};\
class MODELCLSNAME : public __DBModel##SOURCE_STRUCT_TYPE##Base \


#endif
