#ifndef _LOCO_STREAM_SYSTEM_H
#define _LOCO_STREAM_SYSTEM_H
#include "KwNetStreamSystem.h"

//--------------------------------------------------------------------------------------------------
// �ֶ�BILLING�����Ϣ������ [wangxiaoyong]
#define MSG_LOCO_HEADER_SIZE	(sizeof(int32)*7)
#define MSG_LOCO_SEPERATOR		(5439)

class LocoRecieveStream : public RecieveStream
{
public:
	LocoRecieveStream( Socket* sock, 
		uint32 BufferSize = INITRECSIZE,
		uint32 MaxBufferSize = 96*1024 ):RecieveStream(sock, BufferSize, MaxBufferSize)
	{

	}

	virtual ~LocoRecieveStream()
	{

	}

public:
	virtual BOOL				ReciveMessage( IMessage* p ) ;
};

class LocoSendStream : public SendStream
{
public:
	LocoSendStream( Socket* sock, 
		uint32 BufferSize = INITWRITESIZE,
		uint32 MaxBufferSize = 100*1024 ):SendStream(sock, BufferSize, MaxBufferSize)
	{

	}

	virtual ~LocoSendStream()
	{

	}

public:
	virtual BOOL				SendMessage( const IMessage* pPacket, bool bEncrypt=false ) ;
};

//--------------------------------------------------------------------------------------------------
// �����ֶ�android�����ʱ��billing�������� [1/13/2015 yangzhen]
#define MSG_LOCO_UNIFIEDANDROID_HEADER_SIZE	(sizeof(uint16)*2)

class LocoUnifiedAndroidRecieveStream : public RecieveStream
{
public:
	LocoUnifiedAndroidRecieveStream( Socket* sock, 
		uint32 BufferSize = INITRECSIZE,
		uint32 MaxBufferSize = 96*1024 ):RecieveStream(sock, BufferSize, MaxBufferSize)
	{

	}

	virtual ~LocoUnifiedAndroidRecieveStream()
	{

	}

public:
	virtual BOOL ReciveMessage( IMessage* p ) ;
};

class LocoUnifiedAndroidSendStream : public SendStream
{
public:
	LocoUnifiedAndroidSendStream( Socket* sock, 
		uint32 BufferSize = INITWRITESIZE,
		uint32 MaxBufferSize = 100*1024 ):SendStream(sock, BufferSize, MaxBufferSize)
	{

	}

	virtual ~LocoUnifiedAndroidSendStream()
	{

	}

public:
	virtual BOOL SendMessage( const IMessage* pPacket, bool bEncrypt=false ) ;
};

//--------------------------------------------------------------------------------------------------
// loco GM ������
#define MSG_LOCO_GM_HEADER_SIZE	(sizeof(int32) + sizeof(int16))

class LocoGMRecieveStream : public RecieveStream
{
public:
	LocoGMRecieveStream( Socket* sock, 
		uint32 BufferSize = INITRECSIZE,
		uint32 MaxBufferSize = 96*1024 ):RecieveStream(sock, BufferSize, MaxBufferSize)
	{

	}

	virtual ~LocoGMRecieveStream()
	{

	}

public:
	virtual BOOL ReciveMessage( IMessage* p ) ;
};

class LocoGMSendStream : public SendStream
{
public:
	LocoGMSendStream( Socket* sock, 
		uint32 BufferSize = INITWRITESIZE,
		uint32 MaxBufferSize = 100*1024 ):SendStream(sock, BufferSize, MaxBufferSize)
	{
		m_nCacheLastMsgID = INVALID_ID;
	}

	virtual ~LocoGMSendStream()
	{

	}

public:
	virtual BOOL SendMessage( const IMessage* pPacket, bool bEncrypt=false ) ;

	void SetLastMsgID(uint32 nMsgID)
	{
		m_nCacheLastMsgID = nMsgID;
	}

	uint32 GetLastMsgID()
	{
		return m_nCacheLastMsgID;
	}

protected:
	// �����ֶ�������GM����id���ڷ��ؽ����ʱ����Ҫ�ٸ����Ƿ���ȥ [3/20/2015 yangzhen]
	uint32 m_nCacheLastMsgID;
};

#endif