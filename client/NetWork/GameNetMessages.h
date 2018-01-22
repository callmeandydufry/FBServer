#pragma once

#include "GameNetBase.h"
#include "DynaString.h"
#include "FixedString.h"
#include "GameNetMessageDefine.h"
#include "GameNetStream.h"

//------------------------------------------------------------------------------------------------
//CGHandShake
//------------------------------------------------------------------------------------------------
class CGHandShake : public IGameMessage
{
public:
	CGHandShake()
	{
		mMagicNum = 20100801;
		mQuickMark = 0;
		mCheckVer = 0;
	}

	virtual MSG_ID getMsgID() const { return MESSAGE_CG_HANDSHAKE; }

	virtual uint32 getMsgSize() const
	{
		return sizeof(mMagicNum) + sizeof(mQuickMark) + sizeof(mCheckVer);
	}

	virtual bool process(GameNetSystem* netSystem) { return false; }

	virtual bool recieve(NetRecieveStream& stream)
	{
		stream.Reci((char*)&mMagicNum, sizeof(mMagicNum));
		stream.Reci((char*)&mQuickMark, sizeof(mQuickMark));
		stream.Reci((char*)&mCheckVer, sizeof(mCheckVer));
		return true;
	}

	virtual bool send(NetSendStream& stream) const
	{
		stream.Send((char*)&mMagicNum, sizeof(mMagicNum));
		stream.Send((char*)&mQuickMark, sizeof(mQuickMark));
		stream.Send((char*)&mCheckVer, sizeof(mCheckVer));
		return true;
	}

	static IGameMessageFactory* createFactory()
	{
		class CGHandShakeFactory : public IGameMessageFactory
		{
		public:
			virtual MSG_ID getMsgID() { return MESSAGE_CG_HANDSHAKE; }
			virtual uint32 getMessageMaxSize() { return sizeof(CGHandShake) - sizeof(IGameMessage); }
			virtual IGameMessage* createMessage() { return new CGHandShake(); }
		};
		return new CGHandShakeFactory();
	}

	int32 mMagicNum;
	int8 mQuickMark;
	uint32 mCheckVer;
};

//------------------------------------------------------------------------------------------------
//GCHandShake
//------------------------------------------------------------------------------------------------
class GCHandShake : public IGameMessage
{
public:
	virtual MSG_ID getMsgID() const { return MESSAGE_GC_HANDSHAKE; }

	virtual uint32 getMsgSize() const
	{
		return sizeof(mLoginIP)
			+ sizeof(mLoginPort)
			+ sizeof(mResult)
			+ sizeof(mKey)
			+ sizeof(mEncryptArith)
			+ sizeof(mRsaIndex);
	}

	virtual bool process(GameNetSystem* netSystem) { return false; }

	virtual bool recieve(NetRecieveStream& stream)
	{
		stream.Reci((char*)&mLoginIP, sizeof(mLoginIP));
		stream.Reci((char*)&mLoginPort, sizeof(mLoginPort));
		stream.Reci((char*)&mResult, sizeof(mResult));
		stream.Reci((char*)&mKey, sizeof(mKey));
		stream.Reci((char*)&mEncryptArith, sizeof(mEncryptArith));
		stream.Reci((char*)&mRsaIndex, sizeof(mRsaIndex));
		return true;
	}

	virtual bool send(NetSendStream& stream) const
	{
		stream.Send((char*)&mLoginIP, sizeof(mLoginIP));
		stream.Send((char*)&mLoginPort, sizeof(mLoginPort));
		stream.Send((char*)&mResult, sizeof(mResult));
		stream.Send((char*)&mKey, sizeof(mKey));
		stream.Send((char*)&mEncryptArith, sizeof(mEncryptArith));
		stream.Send((char*)&mRsaIndex, sizeof(mRsaIndex));
		return true;
	}

	static IGameMessageFactory* createFactory()
	{
		class GCHandShakeFactory : public IGameMessageFactory
		{
		public:
			virtual MSG_ID getMsgID() { return MESSAGE_GC_HANDSHAKE; }
			virtual uint32 getMessageMaxSize() { return sizeof(GCHandShake) - sizeof(IGameMessage); }
			virtual IGameMessage* createMessage() { return new GCHandShake(); }
		};
		return new GCHandShakeFactory();
	}

	char			mLoginIP[NET_IP_SIZE];
	uint32			mLoginPort;
	EHAND_SHAKE_RESULT_CODE mResult;
	uint32			mKey;
	uint32			mEncryptArith;
	uint32			mRsaIndex;
};

//------------------------------------------------------------------------------------------------
//CGReqeustLogin
//------------------------------------------------------------------------------------------------
class CGRequestLogin : public IGameMessage
{
public:
	virtual MSG_ID getMsgID() const { return MESSAGE_CG_REQUEST_LOGIN; }
	virtual uint32 getMsgSize() const
	{
		return mAccount.getSize() + mToken.getSize();
	}

	virtual bool process(GameNetSystem* netSystem) { return false; }

	virtual bool recieve(NetRecieveStream& stream)
	{
		mAccount.reci(stream);
		mToken.reci(stream);
		return true;
	}

	virtual bool send(NetSendStream& stream) const
	{
		mAccount.send(stream);
		mToken.send(stream);
		return true;
	}

	static IGameMessageFactory* createFactory()
	{
		class CGRequestLoginFactory : public IGameMessageFactory
		{
		public:
			virtual MSG_ID getMsgID() { return MESSAGE_CG_REQUEST_LOGIN; }
			virtual uint32 getMessageMaxSize() { return sizeof(CGRequestLogin) - sizeof(IGameMessage); }
			virtual IGameMessage* createMessage() { return new CGRequestLogin(); }
		};
		return new CGRequestLoginFactory();
	}

	FixedString<MAX_ACCOUNT> mAccount;
	FixedString<MAX_TOKEN_LEN> mToken;
};


//------------------------------------------------------------------------------------------------
//GCEncryptKey
//------------------------------------------------------------------------------------------------
class GCEncryptKey : public IGameMessage
{
public:
	virtual MSG_ID getMsgID() const { return MESSAGE_GC_ENCRYPT_KEY; }
	virtual uint32 getMsgSize() const
	{
		return mKeyData.getSize();
	}

	virtual bool process(GameNetSystem* netSystem) { return false; }

	virtual bool recieve(NetRecieveStream& stream)
	{
		mKeyData.reci(stream);
		return true;
	}

	virtual bool send(NetSendStream& stream) const
	{
		mKeyData.send(stream);
		return true;
	}

	static IGameMessageFactory* createFactory()
	{
		class GCEncryptKeyFactory : public IGameMessageFactory
		{
		public:
			virtual MSG_ID getMsgID() { return MESSAGE_GC_ENCRYPT_KEY; }
			virtual uint32 getMessageMaxSize() { return sizeof(GCEncryptKey) - sizeof(IGameMessage); }
			virtual IGameMessage* createMessage() { return new GCEncryptKey(); }
		};
		return new GCEncryptKeyFactory();
	}

	FixedString<128> mKeyData;
};


//------------------------------------------------------------------------------------------------
//CGPing
//------------------------------------------------------------------------------------------------
class CGPing : public IGameMessage
{
public:
	CGPing()
	{
		mIndex = 0;
	}
	virtual MSG_ID getMsgID() const { return MESSAGE_CG_PING; }

	virtual uint32 getMsgSize() const
	{
		return sizeof(mIndex);
	}

	virtual bool process(GameNetSystem* netSystem) { return false; }

	virtual bool recieve(NetRecieveStream& stream)
	{
		stream.Reci((char*)&mIndex, sizeof(mIndex));
		return true;
	}

	virtual bool send(NetSendStream& stream) const
	{
		stream.Send((char*)&mIndex, sizeof(mIndex));
		return true;
	}

	static IGameMessageFactory* createFactory()
	{
		class CGPingFactory : public IGameMessageFactory
		{
		public:
			virtual MSG_ID getMsgID() { return MESSAGE_CG_PING; }
			virtual uint32 getMessageMaxSize() { return sizeof(CGPing) - sizeof(IGameMessage); }
			virtual IGameMessage* createMessage() { return new CGPing(); }
		};
		return new CGPingFactory();
	}

	int16 mIndex;
};


//------------------------------------------------------------------------------------------------
//GCPing
//------------------------------------------------------------------------------------------------
class GCPing : public IGameMessage
{
public:
	GCPing()
	{
		mIndex = 0;
	}
	virtual MSG_ID getMsgID() const { return MESSAGE_GC_PING; }

	virtual uint32 getMsgSize() const
	{
		return sizeof(mIndex);
	}

	virtual bool process(GameNetSystem* netSystem) { return false; }

	virtual bool recieve(NetRecieveStream& stream)
	{
		stream.Reci((char*)&mIndex, sizeof(mIndex));
		return true;
	}

	virtual bool send(NetSendStream& stream) const
	{
		stream.Send((char*)&mIndex, sizeof(mIndex));
		return true;
	}

	static IGameMessageFactory* createFactory()
	{
		class GCPingFactory : public IGameMessageFactory
		{
		public:
			virtual MSG_ID getMsgID() { return MESSAGE_GC_PING; }
			virtual uint32 getMessageMaxSize() { return sizeof(GCPing) - sizeof(IGameMessage); }
			virtual IGameMessage* createMessage() { return new GCPing(); }
		};
		return new GCPingFactory();
	}

	int16 mIndex;
};


//------------------------------------------------------------------------------------------------
//CGRpcInvokeMsg
//------------------------------------------------------------------------------------------------
class CGRpcInvokeMsg : public IGameMessage
{
public:
	virtual MSG_ID getMsgID() const { return MESSAGE_CG_RPC_CALL; }

	virtual uint32 getMsgSize() const
	{
		return mData.getSize();
	}

	virtual bool process(GameNetSystem* netSystem) { return false; }

	virtual bool recieve(NetRecieveStream& stream)
	{
		mData.reci(stream);
		return true;
	}

	virtual bool send(NetSendStream& stream) const
	{
		mData.send(stream);
		return true;
	}

	static IGameMessageFactory* createFactory()
	{
		class CGRpcInvokeMsgFactory : public IGameMessageFactory
		{
		public:
			virtual MSG_ID getMsgID() { return MESSAGE_CG_RPC_CALL; }
			virtual uint32 getMessageMaxSize() { return CM_IN_SIZE; }
			virtual IGameMessage* createMessage() { return new CGRpcInvokeMsg(); }
		};
		return new CGRpcInvokeMsgFactory();
	}

	DataStream mData;
};

//------------------------------------------------------------------------------------------------
//GCRpcInvokeMsg
//------------------------------------------------------------------------------------------------
class GCRpcInvokeMsg : public IGameMessage
{
public:
	virtual MSG_ID getMsgID() const { return MESSAGE_GC_RPC_CALL; }

	virtual uint32 getMsgSize() const
	{
		return mData.getSize();
	}

	virtual bool process(GameNetSystem* netSystem) { return false; }

	virtual bool recieve(NetRecieveStream& stream)
	{
		mData.reci(stream);
		return true;
	}

	virtual bool send(NetSendStream& stream) const
	{
		mData.send(stream);
		return true;
	}

	static IGameMessageFactory* createFactory()
	{
		class GCRpcInvokeMsgFactory : public IGameMessageFactory
		{
		public:
			virtual MSG_ID getMsgID() { return MESSAGE_GC_RPC_CALL; }
			virtual uint32 getMessageMaxSize() { return CM_IN_SIZE; }
			virtual IGameMessage* createMessage() { return new GCRpcInvokeMsg(); }
		};
		return new GCRpcInvokeMsgFactory();
	}

	DataStream mData;
};