#include "stdafx.h"
#include "IndexAllocManager.h"


BOOL IndexAllocManager::initServerModule(int32 moduleName, ModuleComponent* defaultComponent, int32 groupID)
{
	return ServerModuleBase::initServerModule(moduleName, this, groupID);
}

void IndexAllocManager::uinitServerModule()
{
	ServerModuleBase::uinitServerModule();
}

void IndexAllocManager::tickModule(uint32 uTick)
{
	ServerModuleBase::tickModule(uTick);
}

uint64 IndexAllocManager::rpcAllocPlayerSnid()
{
	return SnidUtil::genPlayerSnid(ESnidType_Player, mAllocedPlayerSnidIndex++);
}

uint64 IndexAllocManager::rpcRequestID(int32 eType, BOOL bStart)
{
	return SnidUtil::genPlayerSnid((ESnidType)eType, (bStart ? 1 : getSnidIndexByEType((ESnidType)eType)));
}

uint64 IndexAllocManager::getSnidIndexByEType(ESnidType eType)
{
	uint64 u64Sind = 1;
	switch (eType)
	{
	case ESnidType_Player:
		u64Sind = mAllocedPlayerSnidIndex;
		break;
	case ESnidType_Guild:
		u64Sind = mAllocedGuildSnidIndex;
		break;
	case ESnidType_Room:
		break;
	default:
		break;
	}
	return u64Sind;
}