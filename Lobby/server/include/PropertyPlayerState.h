#ifndef __PROPERTY_PLAYER_STATE_H__
#define __PROPERTY_PLAYER_STATE_H__

#include "TypeDefine.h"
#include "Struct.h"
#include "StructMacro.h"
#include "ModuleDataStream.h"

/*
---------------------------------------------------------------------------------------------------------
OnlinePlayer Struct
---------------------------------------------------------------------------------------------------------
*/
#define PROPERTYS_PlayerState(PROPERTY)	\
	PROPERTY(SNID_t, Snid, INVALID_ID)\
	PROPERTY(FixedString<MAX_ACCOUNT>, AccountName, " ")\
	PROPERTY(FixedString<MAX_CHARACTER_NAME>, CharacterName, "")\
 	PROPERTY(uint32, OnlineTime, 0)\
	PROPERTY(uint32, OnlineAllTime, 0)\
	PROPERTY(uint32, OnlineAllTimeCurrDay, 0)\
	PROPERTY(uint32, LastLoginTime, 0)\
	PROPERTY(uint32, LastLogoutTime, 0)\
	PROPERTY(FixedString<IP_SIZE>, LastLoginIP, "")\
	PROPERTY(int32, Gold, 0)\
	PROPERTY(int32, Diamond, 0)\
	PROPERTY(int32, GiftDiamond, 0)\
	PROPERTY(int32, BuyDiamond, 0)\
	PROPERTY(int32, Level, 0)\
	PROPERTY(uint64, Exp, 0)\
	PROPERTY(FixedString<MAX_MAILID_LIST>, MailIDList, " ")\
	PROPERTY(uint64, MailStatus, 0)\

struct DBPlayerState
{
	//-------------------------------------------------------------------------
	// 属性列表 [12/12/2017 yz]
	STRUCT_PROPERTY_LIST(PROPERTYS_PlayerState);
	//-------------------------------------------------------------------------
};

DB_STRUCT_DATA_MODEL(DBModelPlayerState, DBPlayerState, PROPERTYS_PlayerState)
{
public:
	DBModelPlayerState()
	{
		clear();
	}

	void clear()
	{
		DBModelBase::clear();
		mSnid = INVALID_ID;
		mAccount.clear();
	}

	virtual const char* getDBTableName()
	{
		return "t_player";
	}

	void setSnid(SNID_t snid)
	{
		mSnid = snid;
	}

	void setAccount(const char* account)
	{
		mAccount = account;
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
		return condition;
	}

	SNID_t mSnid;
	String mAccount;
};

/*
---------------------------------------------------------------------------------------------------------
RegisterPlayer Struct
---------------------------------------------------------------------------------------------------------
*/
struct DBRegisterPlayerState
{
	//-------------------------------------------------------------------------
	// 属性列表 [12/12/2017 yz]
#define PROPERTYS_RegisterPlayer(PROPERTY)	\
	PROPERTY(SNID_t, Snid, INVALID_ID)\
	PROPERTY(FixedString<MAX_ACCOUNT>, AccountName, "")\
	PROPERTY(FixedString<MAX_CHARACTER_NAME>, CharacterName, "")\
	PROPERTY(int32, Gold, 0)\
	PROPERTY(int32, Diamond, 0)\
	PROPERTY(int32, Level, 0)\
	PROPERTY(uint64, Exp, 0)

	STRUCT_PROPERTY_LIST(PROPERTYS_RegisterPlayer)
	//-------------------------------------------------------------------------
};


DB_STRUCT_DATA_MODEL(DBModelRegisterPlayerState, DBRegisterPlayerState, PROPERTYS_RegisterPlayer)
{
public:
	DBModelRegisterPlayerState()
	{
		clear();
	}

	void clear()
	{
		DBModelBase::clear();
		mSnid = INVALID_ID;
		mSnidRangeBegin = INVALID_ID;
		mCountLimit = 0;
	}

	virtual const char* getDBTableName()
	{
		return "t_player";
	}

	void setSnid(SNID_t snid)
	{
		mSnid = snid;
	}

	void setAccount(const char* account)
	{
		mAccount = account;
	}

	void setSnidRange(SNID_t begin, int32 limit)
	{
		mSnidRangeBegin = begin;
		mCountLimit = limit;
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

		if (mSnidRangeBegin != INVALID_ID)
		{
			if (!condition.empty())
				condition += " and ";
			condition += StringUtil::printf("Snid >= %llu", mSnidRangeBegin);
		}

		if (mCountLimit > 0)
		{
			condition += StringUtil::printf(" limit %d", mCountLimit);
		}

		return condition;
	}

	SNID_t mSnid;
	String mAccount;
	SNID_t mSnidRangeBegin;
	int32 mCountLimit;
};


//-------------------------------------------------------------------------------------------------------- -
//  [2/8/2018 Chief]
//-------------------------------------------------------------------------------------------------------- -

#endif