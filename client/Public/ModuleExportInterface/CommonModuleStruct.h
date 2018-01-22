#ifndef __COMMON_MODULE_STRUCT_H__
#define __COMMON_MODULE_STRUCT_H__

#include "TypeDefine.h"
#include "ModulePacket.h"
#include "ModuleDefine.h"

// 模块注册信息 [12/5/2017 yz]
struct ModuleRegInfo
{
	ModuleRegInfo()
	{
		mPort = 0;
	}

	int32 mModuleName;
	FixedString<MAX_MODULE_NAME_LEN> mProcName;
	FixedString<IP_SIZE> mIP;
	uint16 mPort;

	BOOL serialize(DataStream &oStream) const
	{
		oStream.pushValue(mModuleName);
		oStream.pushValue(mProcName);
		oStream.pushValue(mIP);
		oStream.pushValue(mPort);
		return TRUE;
	}

	BOOL unserialize(DataStream &iStream)
	{
		iStream.getValue(mModuleName);
		iStream.getValue(mProcName);
		iStream.getValue(mIP);
		iStream.getValue(mPort);
		return TRUE;
	}
};

// 模块注册列表 [12/5/2017 yz]
struct ModuleRegList
{
	ModuleRegList()
	{
		mModuleCount = 0;
	}

	uint32 mModuleCount;
	ModuleRegInfo mAllModules[MAX_MODULE_COUNT];

	BOOL serialize(DataStream &oStream) const
	{
		oStream.pushValue(mModuleCount);
		for (uint32 i = 0; i < mModuleCount; ++i)
		{
			oStream.pushValue(mAllModules[i]);
		}
		
		return TRUE;
	}

	BOOL unserialize(DataStream &iStream)
	{
		iStream.getValue(mModuleCount);
		for (uint32 i = 0; i < mModuleCount; ++i)
		{
			iStream.getValue(mAllModules[i]);
		}

		return TRUE;
	}

};

#endif
