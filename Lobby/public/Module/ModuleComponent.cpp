#include "stdafx.h"
#include "ModuleComponent.h"
#include "ModuleExportInterface.h"
#include "ModuleManager.h"


ModuleComponent::ModuleComponent()
	: mModule(NULL)
	, mLastCheckOvertime(0)
	, mCurrentExecutingPacket(NULL)
{

}

ModuleComponent::~ModuleComponent()
{
	removeAllReturnCallback();
	if (NULL != mModule)
	{
		mModule->unregisterModuleComponent(this);
	}
}

void ModuleComponent::registerModule(ModuleBase* module)
{
	if (NULL != module)
	{
		module->registerModuleComponent(this);
	}
}

void ModuleComponent::unregisterModule()
{
	if (NULL != mModule)
	{
		mModule->unregisterModuleComponent(this);
	}
}

BOOL ModuleComponent::executePacket(DataPacket* packet)
{
	__GUARD__;
	return executeReturnPacket(packet);
	__UNGUARD__
		return FALSE;
}

BOOL ModuleComponent::executePacketFunction(DataPacket* packet, PacketProcessFunction* processFuncInfo)
{
	__GUARD__;

	// 设置这个变量的目的是可以在rpc函数执行过程中获取其触发的消息包 [11/29/2016 yz]
	mCurrentExecutingPacket = packet;

	BOOL ret = FALSE;
	DataPacket* retPacket = NULL;
	// mOperateID不为空，说明发起者需要等待调用者的返回 [11/24/2016 yz]
	if (packet->mOperateID != INVALID_ID && packet->mSourceComponentId != INVALID_ID && packet->mSourceModule > 0)
	{
		retPacket = new DataPacket;
		retPacket->mSourceComponentId = packet->mTargetComponentId;
		retPacket->mTargetComponentId = packet->mSourceComponentId;
		retPacket->mPackMethodId = packet->mPackMethodId;
		retPacket->mOperateID = packet->mOperateID;
		retPacket->mIsReturn = TRUE;

		// 将返回值装进返回的消息包中 [11/24/2016 yz]
		ret = processFuncInfo->processPacket(this, packet->mData, &retPacket->mData);
		if (ret)
		{
			g_pModuleManager->sendPacket(packet->mSourceModule, retPacket);
		}
		else
		{
			delete retPacket;
		}
	}
	else
	{
		ret = processFuncInfo->processPacket(this, packet->mData, NULL);
	}

	mCurrentExecutingPacket = NULL;

	return ret;
	__UNGUARD__ return FALSE;
}

BOOL ModuleComponent::executeReturnPacket(DataPacket* packet)
{
	__GUARD__;
	KCheck(NULL != packet && packet->mIsReturn && INVALID_ID != packet->mOperateID);

	// 设置这个变量的目的是可以在rpc函数执行过程中获取其触发的消息包 [11/29/2016 yz]
	mCurrentExecutingPacket = packet;

	BOOL bRet = FALSE;
	PacketReturnCallable* deleteCallable = NULL;
	for (int i = 0; i < mOperateCallbacks.size(); ++i)
	{
		PacketReturnCallable* callable = mOperateCallbacks[i];
		if (callable->getOperateID() == packet->mOperateID)
		{
			callable->executeCallback(this, packet->mData);
			deleteCallable = callable;
			bRet = TRUE;
			break;
		}
	}

	for (int i = 0; i < mOperateCallbacks.size(); ++i)
	{
		PacketReturnCallable* callable = mOperateCallbacks[i];
		if (callable == deleteCallable)
		{
			delete callable;
			mOperateCallbacks.erase(mOperateCallbacks.begin() + i);
			break;
		}
	}

	mCurrentExecutingPacket = NULL;

	return bRet;

	__UNGUARD__
		return FALSE;
}

void ModuleComponent::addExternalReturnCallback(PacketReturnCallable* callback)
{
	mOperateCallbacks.push_back(callback);
}

void ModuleComponent::removeAllReturnCallback()
{
	__GUARD__
		for (int i = 0; i < mOperateCallbacks.size(); ++i)
		{
			delete mOperateCallbacks[i];
		}
	mOperateCallbacks.clear();
	__UNGUARD__
}

void ModuleComponent::tickComponent(uint32 uTick)
{
	__GUARD__
		if (uTick - mLastCheckOvertime > 200)
		{
			mLastCheckOvertime = uTick;
			std::vector<PacketReturnCallable*>::iterator it;
			for (it = mOperateCallbacks.begin(); it != mOperateCallbacks.end(); )
			{
				PacketReturnCallable* callable = *it;
				if (callable->checkOvertime(uTick))
				{
					// 执行异步调用超时的回调函数 [11/28/2016 yz]
					callable->executeOvertime(this);

					delete callable;
					it = mOperateCallbacks.erase(it);
				}
				else
				{
					++it;
				}
			}
		}
	__UNGUARD__
}