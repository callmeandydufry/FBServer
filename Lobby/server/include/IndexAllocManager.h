#ifndef __INDEX__ALLOC_MANAGER_H__
#define __INDEX__ALLOC_MANAGER_H__

#include "ServerModuleExportInterface.h"
#include "ServerModuleUtils.h"
#include "ServerModuleBase.h"

class IndexAllocManager
	: public ServerModuleBase
	, public IIndexAllocManager
{
public:
	IndexAllocManager()
		: mAllocedPlayerSnidIndex(1)
		, mAllocedGuildSnidIndex(1)
	{
	}

	~IndexAllocManager()
	{
	}

	virtual BOOL initServerModule(int32 moduleName, ModuleComponent* defaultComponent, int32 groupID);
	virtual void uinitServerModule();
	virtual void tickModule(uint32 uTick);

	virtual int64 getComponentId() { return MODULE_DEFAULT_ID; }

	//----------------------------------------------------------------------
	//RPC调用接口 START
	//----------------------------------------------------------------------
	virtual uint64 rpcAllocPlayerSnid();

	virtual uint64 rpcRequestID(int32 eType, BOOL bStart);  // 根据类型获取当前起始和当前最大id
	//----------------------------------------------------------------------
	//RPC调用接口 END
	//----------------------------------------------------------------------
protected:
	uint64 getSnidIndexByEType(ESnidType eType);

protected:
	uint64 mAllocedPlayerSnidIndex;
	uint64 mAllocedGuildSnidIndex;
};


#endif