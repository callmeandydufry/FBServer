#include "stdafx.h"
#include "ModulePacket.h"

DataPacketQueue::DataPacketQueue()
{
	mHead = 0;
	mTail = 0;
	mPacketQueue = new ASYNC_PACKET[MAX_CACHE_SIZE];
	KCheck(mPacketQueue);
	mQueueSize = MAX_CACHE_SIZE;
}

DataPacketQueue::~DataPacketQueue()
{
	KSafeDelete_ARRAY(mPacketQueue);
}

BOOL DataPacketQueue::pushPacket(DataPacket *pKMessage)
{
	__GUARD__

		lock_guard<FastLock> autolock(mLock);

	if (NULL != mPacketQueue[mTail].mPacket)
	{
		BOOL ret = resizeCache();
		KCheck(ret);
	}

	mPacketQueue[mTail].mPacket = pKMessage;
	mPacketQueue[mTail].m_Flag = PF_USED;

	mTail++;
	if (mTail >= mQueueSize)
		mTail = 0;

	return TRUE;
	__UNGUARD__
		return FALSE;
}

BOOL DataPacketQueue::recvPacket(DataPacket * &pKMessage)
{
	__GUARD__

		lock_guard<FastLock> autolock(mLock);

	while (mPacketQueue[mHead].m_Flag == PF_REMOVE)
	{
		if (mPacketQueue[mHead].mPacket)
			delete mPacketQueue[mHead].mPacket;
		mPacketQueue[mHead].mPacket = NULL;
		mPacketQueue[mHead].m_Flag = PF_NONE;

		mHead++;
		if (mHead >= mQueueSize) mHead = 0;
	}

	if (mPacketQueue[mHead].mPacket == NULL)
	{
		return FALSE;
	}

	pKMessage = mPacketQueue[mHead].mPacket;

	mPacketQueue[mHead].mPacket = NULL;
	mPacketQueue[mHead].m_Flag = PF_NONE;

	mHead++;
	if (mHead >= mQueueSize) mHead = 0;

	return TRUE;
	__UNGUARD__
		return FALSE;
}

BOOL DataPacketQueue::resizeCache()
{
	__GUARD__

		ASYNC_PACKET* pNew = new ASYNC_PACKET[mQueueSize + MAX_CACHE_SIZE];
	if (pNew == NULL)
		return FALSE;

	if (mHead < mTail)
		memcpy(pNew, &(mPacketQueue[mHead]), sizeof(ASYNC_PACKET) * (mTail - mHead));
	else if (mHead >= mTail)
	{
		memcpy(pNew, &(mPacketQueue[mHead]), sizeof(ASYNC_PACKET) * (mQueueSize - mHead));
		memcpy(&pNew[mQueueSize - mHead], mPacketQueue, sizeof(ASYNC_PACKET) * mTail);
	}

	memset(mPacketQueue, 0, sizeof(ASYNC_PACKET) * mQueueSize);
	KSafeDelete_ARRAY(mPacketQueue);
	mPacketQueue = pNew;

	mHead = 0;
	mTail = mQueueSize;
	mQueueSize = mQueueSize + MAX_CACHE_SIZE;

	return TRUE;
	__UNGUARD__
		return FALSE;
}

BOOL DataPacketQueue::removeMessage(uint32 targetId)
{
	__GUARD__

		lock_guard<FastLock>	autolock(mLock);
	uint32					Cur = mHead;

	for (uint32 _Idx = 0; _Idx < mQueueSize; _Idx++)
	{
		if (mPacketQueue[Cur].mPacket == NULL)
			break;

		if (mPacketQueue[Cur].mPacket->mTargetComponentId == targetId)
		{
			mPacketQueue[Cur].m_Flag = PF_REMOVE;
		}

		Cur++;
		if (Cur >= mQueueSize)
			Cur = 0;
	}

	return TRUE;
	__UNGUARD__
		return FALSE;
}

void DataPacketQueue::clear()
{
	__GUARD__

		lock_guard<FastLock>	autolock(mLock);

	for (uint32 _Idx = 0; _Idx < mQueueSize; _Idx++)
	{
		if (mPacketQueue[_Idx].mPacket != NULL)
		{
			mPacketQueue[_Idx].m_Flag = PF_NONE;
			delete mPacketQueue[_Idx].mPacket;
			mPacketQueue[_Idx].mPacket = NULL;
		}
	}

	mHead = 0;
	mTail = 0;

	__UNGUARD__
}