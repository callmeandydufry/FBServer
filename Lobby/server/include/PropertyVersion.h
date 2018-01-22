#ifndef __PROPERTY_VERSION_H__
#define __PROPERTY_VERSION_H__

#include "TypeDefine.h"
#include "Struct.h"
#include "StructMacro.h"
#include "ModuleDataStream.h"

#define PROPERTYS_VersionState(PROPERTY)	\
	PROPERTY(int32, verNum, 1)\

struct DBVersionPro
{
	//-------------------------------------------------------------------------
	//  Ù–‘¡–±Ì [12/12/2017 yz]
	STRUCT_PROPERTY_LIST(PROPERTYS_VersionState)
		//-------------------------------------------------------------------------
};

DB_STRUCT_DATA_MODEL(DBVersionState, DBVersionPro, PROPERTYS_VersionState)
{
public:
	DBVersionState()
	{
		nVerNum = 1;
	}

	virtual const char* getDBTableName()
	{
		return "t_version";
	}

protected:
	String DBVersionState::getSQLSelectString()
	{
		String paramNameStr;
		for (int32 i = 0; i < mAllModelProps.GetSize(); ++i)
		{
			if (i > 0)
			{
				paramNameStr += ",";
			}
			paramNameStr += mAllModelProps[i]->getPropName();
		}

		String sqlString = StringUtil::printf("select %s from %s ", paramNameStr.c_str(), getDBTableName());
		return sqlString;
	};

	int32 nVerNum;
};

#endif