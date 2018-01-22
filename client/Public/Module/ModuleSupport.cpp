#include "stdafx.h"
#include "ModuleSupport.h"
#include "ModuleManager.h"


ModuleBase::ModuleBase()
	: mDefaultComponent(NULL)
{

}

ModuleBase::~ModuleBase()
{
	__GUARD__
		DataPacket* packet = NULL;
	while (mRecvPacketQueue.recvPacket(packet))
	{
		KSafeDelete(packet);
	}
	mDefaultComponent = NULL;
	__UNGUARD__
}

BOOL ModuleBase::initModule(int32 moduleName, ModuleComponent* defaultComponent)
{
	__GUARD__
		mModuleName = moduleName;
	mDefaultComponent = defaultComponent;
	if (NULL != mDefaultComponent)
	{
		mDefaultComponent->setModule(this);
	}

	return TRUE;
	__UNGUARD__
		return FALSE;
}

void ModuleBase::uinitModule()
{
	__GUARD__
		if (NULL != mDefaultComponent)
		{
			mDefaultComponent->setModule(NULL);
		}
	mComponents.clear();
	__UNGUARD__
}

void ModuleBase::tickModule(uint32 uTick)
{
	__GUARD__
		processPackets();
	if (NULL != mDefaultComponent)
	{
		mDefaultComponent->tickComponent(uTick);
	}
	for (ComponentMap::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
	{
		it->second->tickComponent(uTick);
	}
	__UNGUARD__
}

void ModuleBase::recvPacket(DataPacket* packet)
{
	mRecvPacketQueue.pushPacket(packet);
}

void ModuleBase::registerModuleComponent(ModuleComponent* component)
{
	component->setModule(this);
	mComponents.insert(ComponentMap::value_type(component->getComponentId(), component));
}

void ModuleBase::unregisterModuleComponent(ModuleComponent* component)
{
	component->setModule(NULL);
	mComponents.erase(component->getComponentId());
}

ModuleComponent* ModuleBase::getComponent(int64 componentId)
{
	ComponentMap::iterator compIt = mComponents.find(componentId);
	if (compIt != mComponents.end())
	{
		return compIt->second;
	}
	return NULL;
}

void ModuleBase::processPackets()
{
	__GUARD__
		DataPacket* packet = NULL;
	while (mRecvPacketQueue.recvPacket(packet))
	{
		ModuleComponent* component = getComponent(packet->mTargetComponentId);
		if (NULL != component)
		{
			component->executePacket(packet);
		}
		else if (MODULE_DEFAULT_ID == packet->mTargetComponentId && NULL != mDefaultComponent)
		{
			mDefaultComponent->executePacket(packet);
		}
		delete packet;
	}
	__UNGUARD__
}