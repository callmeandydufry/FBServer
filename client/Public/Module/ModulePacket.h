
#ifndef __ModulePacket_H__
#define __ModulePacket_H__

#include "TypeDefine.h"
#include "ModuleDataStream.h"

// ģ��֮��ͨ����Ϣ���� [11/22/2016 yz]
struct DataPacket
{
	DataPacket()
	{
		mTargetModule = INVALID_ID;
		mSourceModule = INVALID_ID;
		mSourceComponentId = INVALID_ID;
		mTargetComponentId = INVALID_ID;
		mPackMethodId = INVALID_ID;
		mOperateID = INVALID_ID;
		mIsReturn = FALSE;
	}

	DataPacket(DataPacket* pPkt)
	{
		mTargetModule = pPkt->mTargetModule;
		mSourceModule = pPkt->mSourceModule;
		mSourceComponentId = pPkt->mSourceComponentId;
		mTargetComponentId = pPkt->mTargetComponentId;
		mPackMethodId = pPkt->mPackMethodId;
		mOperateID = pPkt->mOperateID;
		mIsReturn = pPkt->mIsReturn;
		mData = pPkt->mData;
	}

	void reci(RecieveStream &iStream)
	{
		iStream.Reci((char *)(&mTargetModule), sizeof(mTargetModule));
		iStream.Reci((char *)(&mSourceModule), sizeof(mSourceModule));
		iStream.Reci((char *)(&mSourceComponentId), sizeof(mSourceComponentId));
		iStream.Reci((char *)(&mTargetComponentId), sizeof(mTargetComponentId));
		iStream.Reci((char *)(&mPackMethodId), sizeof(mPackMethodId));
		iStream.Reci((char *)(&mOperateID), sizeof(mOperateID));
		iStream.Reci((char *)(&mIsReturn), sizeof(mIsReturn));
		mData.reci(iStream);
	}

	void send(SendStream &oStream) const
	{
		oStream.Send((char *)(&mTargetModule), sizeof(mTargetModule));
		oStream.Send((char *)(&mSourceModule), sizeof(mSourceModule));
		oStream.Send((char *)(&mSourceComponentId), sizeof(mSourceComponentId));
		oStream.Send((char *)(&mTargetComponentId), sizeof(mTargetComponentId));
		oStream.Send((char *)(&mPackMethodId), sizeof(mPackMethodId));
		oStream.Send((char *)(&mOperateID), sizeof(mOperateID));
		oStream.Send((char *)(&mIsReturn), sizeof(mIsReturn));
		mData.send(oStream);
	}

	uint32 getSize() const
	{
		return sizeof(mTargetModule)
			+ sizeof(mSourceModule)
			+ sizeof(mSourceComponentId)
			+ sizeof(mTargetComponentId)
			+ sizeof(mPackMethodId)
			+ sizeof(mOperateID)
			+ sizeof(mIsReturn)
			+ mData.getSize();
	}

	int32 mTargetModule;
	int32 mSourceModule;
	int64 mSourceComponentId;
	int64 mTargetComponentId;
	uint16 mPackMethodId;
	uint32 mOperateID;	// �ɷ����߼�¼һ�ε��õ�Ψһid����������Զ�˵��÷��ؽ�� [11/24/2016 yz]
	uint8 mIsReturn;
	DataStream mData;
};

#define MAX_CACHE_SIZE		1024
class DataPacketQueue
{
public:
	enum PACKET_FLAG { PF_NONE = 0, PF_USED, PF_REMOVE };


	struct ASYNC_PACKET
	{
		DataPacket* mPacket;
		uint32 m_Flag;

		ASYNC_PACKET()
		{
			mPacket = NULL;
			m_Flag = PF_NONE;
		};
		~ASYNC_PACKET()
		{
			KSafeDelete(mPacket);
			m_Flag = PF_NONE;
		};
	};

	DataPacketQueue();
	~DataPacketQueue();

	BOOL pushPacket(DataPacket *pKMessage);
	BOOL recvPacket(DataPacket * &pKMessage);
	BOOL resizeCache();
	BOOL removeMessage(uint32 targetId);
	void clear();

protected:
	FastLock		mLock;
	ASYNC_PACKET	*mPacketQueue;				// ��Ϣ����
	uint32			mQueueSize;					// ��Ϣ���д�С
	uint32			mHead;						// ��Ϣ����Head���
	uint32			mTail;						// ��Ϣ����Tail���
};

#endif
