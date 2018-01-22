#ifndef __PROPERTY_SNID_ACCOUNT_RELATION_H__
#define __PROPERTY_SNID_ACCOUNT_RELATION_H__

#include "TypeDefine.h"
#include "Struct.h"
#include "StructMacro.h"
#include "ModuleDataStream.h"

#define PROPERTYS_SnidAllocState(PROPERTY)	\
	PROPERTY(SNID_t, Snid, INVALID_ID)\
	PROPERTY(FixedString<MAX_ACCOUNT>, AccountName, " ")

struct DBSnidAllocPro
{
	//-------------------------------------------------------------------------
	// ÊôÐÔÁÐ±í
	STRUCT_PROPERTY_LIST(PROPERTYS_SnidAllocState)
};

DB_STRUCT_DATA_MODEL(DBSnidAllocState, DBSnidAllocPro, PROPERTYS_SnidAllocState)
{
	SNID_t mSnid;
	String mAccount;
	SNID_t mSnidRangeBegin, mSnidRangeEnd;
	int32 mCountLimit;
public:
	DBSnidAllocState()
	{
		clear();
	}

	virtual const char* getDBTableName()
	{
		return "t_snidalloc";
	}

	void setSnid(SNID_t snid)
	{
		mSnid = snid;
	}

	void setAccount(const char* account)
	{
		mAccount = account;
	}

	void setSnidRange(SNID_t begin, SNID_t end)
	{
		mSnidRangeBegin = begin;
		mSnidRangeEnd = end;
	}

	void setCountLimit(int32 count)
	{
		mCountLimit = count;
	}

	void clear()
	{
		DBModelBase::clear();
		mSnid = INVALID_ID;
		mSnidRangeBegin = INVALID_ID;
		mSnidRangeEnd = INVALID_ID;
		mCountLimit = 0;
	}

protected:
	virtual String getSQLQueryConditionString()
	{
		String condition;
		if (!mAccount.empty())
		{
			condition += StringUtil::printf("AccountName='%s'", mAccount.c_str());
		}

		if (mSnid != INVALID_ID)
		{
			if (!condition.empty())
				condition += " and ";
			condition += StringUtil::printf("Snid=%llu", mSnid);
		}

		if (mSnidRangeBegin != INVALID_ID && mSnidRangeEnd != INVALID_ID)
		{
			if (!condition.empty())
				condition += " and ";
			condition += StringUtil::printf("Snid >= %llu and Snid < %llu", mSnidRangeBegin, mSnidRangeEnd);
		}

		if (mCountLimit > 0)
		{
			condition += StringUtil::printf(" limit %d", mCountLimit);
		}

		return condition;
	}
};

#endif __PROPERTY_SNID_ACCOUNT_RELATION_H__
