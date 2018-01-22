#pragma once
#include "CoreBase.h"
#include "PacketDataStream.h"
#include "KwNetStreamSystem.h"

/**
* 线程安全的队列，用于传输网络消息包和控制命令
*/
template <class Type>
struct TNetAsyncQueue
{
private:
	FastLock mLock;
	TQueue<Type> mItems;
	uint32 mSize = 0;
public:

	inline const uint32 GetSize() const { return mSize; }

	void enqueue(const Type& Item)
	{
		FastLock ScopeLock(&mLock);
		mItems.Enqueue(Item);
		mSize++;
	}

	bool dequeue(Type& Result)
	{
		FastLock ScopeLock(&mLock);
		mSize--;
		return mItems.Dequeue(Result);
	}

	bool batchDequeue(TQueue<Type>* Result, uint32 MaxItems)
	{
		FastLock ScopeLock(&mLock);

		uint32 i = 0;
		Type Item;
		while (i <= MaxItems)
		{
			mItems.Dequeue(Item);
			mSize--;
			Result->Enqueue(Item);

			i++;
			if (mSize == 0)
			{
				break;
			}
		}

		return i > 0;
	}

	bool peek(Type& Result)
	{
		FastLock ScopeLock(&mLock);
		return mItems.Peek(Result);
	}

	bool isEmpty()
	{
		FastLock ScopeLock(&mLock);
		return mItems.IsEmpty();
	}

	void empty()
	{
		FastLock ScopeLock(&mLock);

		Type Result;
		while (mItems.Dequeue(Result)) {}
	}
};

class GameNetSystem;
class IGameMessageFactory;
class NetRecieveStream;
class NetSendStream;

class IGameMessage
{
public:
	IGameMessage()
		: mMsgIndex(0)
	{}
	virtual ~IGameMessage(){}

	virtual MSG_ID getMsgID() const = 0;
	virtual uint32 getMsgSize() const = 0;
	virtual bool process(GameNetSystem* netSystem) = 0;
	virtual bool recieve(NetRecieveStream& stream) = 0;
	virtual bool send(NetSendStream& stream) const = 0;

	uint32 getMsgIndex() const
	{
		return mMsgIndex;
	}

	void setMsgIndex(uint32 msgIndex)
	{
		mMsgIndex = msgIndex;
	}

protected:
	uint32 mMsgIndex;
};

class IGameMessageFactory
{
public:

	virtual ~IGameMessageFactory() { }

	virtual MSG_ID getMsgID() = 0;
	virtual uint32 getMessageMaxSize() = 0;
	virtual IGameMessage* createMessage() = 0;
};
