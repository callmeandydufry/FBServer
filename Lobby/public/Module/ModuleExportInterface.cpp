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
			// ���ﲢδ����������nUniqueOperateID���ܻ�����߳����⣬����û�й�ϵ��ֻҪͬһ���߳��з���ȥ��ID��ͬ���У���ͬ�̵߳���Ϣ��ģ������ȥ�ַ� [11/24/2016 yz]
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
				// �ͷſ���Ȩ [11/24/2016 yz]
				mReturnCallback = NULL;

				// ֮����Ҫ������Ϣ��ʱ�������opID��������opID�����߲Ż᷵����Ϣ [11/24/2016 yz]
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
			
			// �ͷſ���Ȩ [11/24/2016 yz]
			mPacket = NULL;
		}
	}
