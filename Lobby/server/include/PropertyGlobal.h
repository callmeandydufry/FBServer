#ifndef __PROPERTY_GLOBAL_H__
#define __PROPERTY_GLOBAL_H__

#include "TypeDefine.h"
#include "Define.h"
#include "Struct.h"
#include "StructMacro.h"
#include "ModuleDataStream.h"

#define PROPERTYS_GlobalState(PROPERTY)	\
	PROPERTY(int32, GlobalIndex, 1)\
	PROPERTY(int32, GlobalVal, 1)\

struct DBGlobalPro
{
	//-------------------------------------------------------------------------
	//  Ù–‘¡–±Ì 
	STRUCT_PROPERTY_LIST(PROPERTYS_GlobalState)
	//-------------------------------------------------------------------------
};

DB_STRUCT_DATA_MODEL(DBGlobalState, DBGlobalPro, PROPERTYS_GlobalState)
{
public:
	DBGlobalState()
	{
		mCheckIndex = EPF_INVALID;
		mVal = INVALID;
	}

	virtual const char* getDBTableName()
	{
		return "t_globalval";
	}

	void setCheckType(EProFlag nType)
	{
		mCheckIndex = nType;
	}

	void setVal(int32 nVal)
	{
		mVal = nVal;
	}

protected:
	virtual String DBGlobalState::getSQLQueryConditionString()
	{
		String sqlString;

		if (EPF_INVALID != mCheckIndex)
		{
			sqlString = StringUtil::printf("GlobalIndex='%d'", (int32)mCheckIndex);
		}

		if (INVALID != mVal)
		{
			sqlString = StringUtil::printf("GlobalVal='%d'", mVal);
		}

		return sqlString;
	};

	EProFlag	mCheckIndex;
	int32		mVal;
};

#endif // __PROPERTY_GLOBAL_H__