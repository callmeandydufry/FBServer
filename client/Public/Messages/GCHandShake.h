#ifndef __GC_HAND_SHAKE_H__
#define __GC_HAND_SHAKE_H__

#include "TypeDefine.h"
#include "KwNetMessage.h"
#include "MessageFactory.h"
#include "MessageDefine.h"

enum EHAND_SHAKE_RESULT_CODE
{
	EHAND_SHAKE_SUCCESS,
	EHAND_SHAKE_FULL,
	EHAND_SHAKE_STOP_SERVICE,
	EHAND_SHAKE_VERSION_FAILED,
};

DECLARE_NET_MESSAGE(GCHandShake, MESSAGE_GC_HANDSHAKE)

GCHandShake()
{

};

virtual ~GCHandShake()
{
};

virtual uint32 GetMsgSize() const
{
	return sizeof(LoginIP) +
		sizeof(LoginPort) +
		sizeof(Result) +
		sizeof(Key) +
		sizeof(EncryptArith) +
		sizeof(RsaIndex);
}

public:

	EHAND_SHAKE_RESULT_CODE GetResult()
	{
		return Result;
	}

	void SetResult(EHAND_SHAKE_RESULT_CODE LResult)
	{
		Result = LResult;
	}

	uint32 GetLoginPort()
	{
		return LoginPort;
	}

	void SetLoginPort(uint32 LPort)
	{
		LoginPort = LPort;
	}

	char *GetLoginIP()
	{
		return LoginIP;
	}

	void SetLoginIP(const char *pLoginIP)
	{
		strncpy(LoginIP, pLoginIP, IP_SIZE);
		LoginIP[IP_SIZE] = '\0';
	}

	uint32 GetUserKey()
	{
		return Key;
	}

	void SetUserKey(uint32 key)
	{
		Key = key;
	}

	uint32 GetEncryptArith()
	{
		return EncryptArith;
	}

	void SetEncryptArith(uint32 Arith)
	{
		EncryptArith = Arith;
	}

	uint32 GetRsaIndex()
	{
		return RsaIndex;
	}

	void SetRsaIndex(uint32 index)
	{
		RsaIndex = index;
	}

private:
	char			LoginIP[IP_SIZE];
	uint32			LoginPort;
	EHAND_SHAKE_RESULT_CODE	Result;
	uint32			Key;
	uint32			EncryptArith;
	uint32			RsaIndex;

	DECLARE_NET_MESSAGE_FACTORY(GCHandShake, MESSAGE_GC_HANDSHAKE)

	/*
	=======================================================================================================================
	=======================================================================================================================
	*/
	uint32 GetMessageMaxSize() const
	{
		return sizeof(GCHandShake) - sizeof(IMessage);
	}

	DECLARE_NET_MESSAGE_HANDLE(GCHandShake, MESSAGE_GC_HANDSHAKE)
#endif
