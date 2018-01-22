#include "stdafx.h"
#include "ModuleExportInterface.h"
#include "ModuleManager.h"


	PacketReturnCallable::PacketReturnCallable()
		: mOperateID(INVALID_ID)
	{
		mTimeStamp = g_pModuleManager->getCurrentTickCount();
		mOvertimeLength = 0;
	}

	PacketCallable::~PacketCallable()
	{
		sendPacket();

		KSafeDelete(mPacket);
		KSafeDelete(mReturnCallback);
	}

	DataPacket* PacketCallable::getDataPacket()
	{
		if (NULL == mPacket)
		{
			mPacket = new DataPacket;
		}
		return mPacket;
	}

	void PacketCallable::setTarget(int32 moduleName, int64 objID)
	{
		mTargetModuleName = moduleName;
		mTargetObjID = objID;
	}

	void PacketCallable::setSource(int32 moduleName, int64 objID)
	{
		mSourceModuleName = moduleName;
		mSourceObjID = objID;
	}

	void PacketCallable::setInvoker(ModuleComponent* invoker)
	{
		mInvoker = invoker;
	}

	void PacketCallable::setSendPacketProxy(PacketSendProxy* proxy)
	{
		mSendPacketProxy = proxy;
	}

	void PacketCallable::sendPacket()
	{
		if (mTargetModuleName > 0 && NULL != mPacket)
		{
			// 这里并未加锁，所以nUniqueOperateID可能会出现线程问题，但是没有关系，只要同一个线程中发出去的ID不同就行，不同线程的消息有模块名称去分发 [11/24/2016 yz]
			static int nUniqueOperateID = 1;
			nUniqueOperateID++;

			mPacket->mSourceModule = mSourceModuleName;
			mPacket->mSourceComponentId = mSourceObjID;
			mPacket->mTargetComponentId = mTargetObjID;
			mPacket->mIsReturn = FALSE;
			
			if (NULL != mInvoker && NULL != mReturnCallback && mReturnCallback->isValid())
			{
				mReturnCallback->setOperateID(nUniqueOperateID);
				mInvoker->addExternalReturnCallback(mReturnCallback);
				// 释放控制权 [11/24/2016 yz]
				mReturnCallback = NULL;

				// 之后需要返回消息的时候才设置opID，设置了opID调用者才会返回消息 [11/24/2016 yz]
				mPacket->mOperateID = nUniqueOperateID;
			}

			if (mSendPacketProxy)
			{
				if (!mSendPacketProxy->overrideSendPacket(mTargetModuleName, mPacket))
				{
					delete mPacket;
				}
			}
			else
			{
				g_pModuleManager->sendPacket(mTargetModuleName, mPacket);
			}
			
			// 释放控制权 [11/24/2016 yz]
			mPacket = NULL;
		}
	}
