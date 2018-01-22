#pragma once

#include "GameNetBase.h"
#include "Sockets.h"
#include "GameNetMessageEncryptor.h"

#define MSGHEAD_MAGIC_NUM 0xD13DEA9B
#define MSG_MAGIC_NUM 0x9326662B
#define STREAM_MAGIC_NUM 0xD5C0A6E3
#define STREAM_ENCRPTOR_KEY 0xD7AC0001

#define INITRECSIZE						(64*1024)
#define INITWRITESIZE					(8*1024)
#define CM_HEADER_SIZE					(2)
#define CM_IN_SIZE						(1024*40)
#define CM_OUT_SIZE						(CM_IN_SIZE + CM_IN_SIZE/16 + 64 + 3 + CM_HEADER_SIZE)
#define	CM_MIN_CM_SIZE					100
// 服务器发给客户端的socket的数据，每次心跳(50ms)send的数据量，不宜过大，否则多用户同时传输时带宽峰值过大 [5/17/2017 yz]
#define SOCKET_SERVER2CLINET_SIZE_PER_FRAME	(5120)
#define NET_SOCKET_ERROR				(-1)

extern uint32 GetMsgIndex(uint32 a);
extern void	SetMsgIndex(uint32 &a, uchar index);
extern uint32 GetMsgLen(uint32 a);
extern void	SetMsgLen(uint32 &a, uint32 Len);
#define GET_MSG_INDEX(a)		GetMsgIndex(a)
#define SET_MSG_INDEX(a, index)		SetMsgIndex(a, index)
#define GET_MSG_LEN(a)			GetMsgLen(a)
#define SET_MSG_LEN(a, len)		SetMsgLen(a, len)
#define BILL_GET_MSG_INDEX(a)		((a) >> 24)
#define BILL_SET_MSG_INDEX(a, index)	((a) = (((a) & 0xffffff) + ((index) << 24)))
#define BILL_GET_MSG_LEN(a)		((a) & 0xffffff)
#define BILL_SET_MSG_LEN(a, len)	((a) = ((a) & 0xff000000) + (len))
#define MSG_HEADER_SIZE			(sizeof(MSG_ID) + sizeof(uint32))


typedef signed char code_char;

class NetEncrptor
{
public:
	enum { ED_DATA_LENGTH = 16 };
public:
	NetEncrptor();
	~NetEncrptor();
	void* Encode(void * dst, const void * src, uint32 count);
	void* Decode(void * dst, const void * src, uint32 count);

	void  SetPdata(const code_char* p, uint32 uMagicNum);
	code_char* GetPdata();
	char  enable;
private:
	code_char  pdata[ED_DATA_LENGTH];

};

class IGameMessage;

class NetRecieveStream
{
public:
	NetRecieveStream(uint32 BufferSize = INITRECSIZE,
		uint32 MaxBufferSize = 96 * 1024);
	virtual ~NetRecieveStream();


public:
	void						BindSocket(FSocket* socket) { m_pSocket = socket; }
	void						UnbindSocket() { m_pSocket = NULL; }
	uint32						Reci(char* buf, uint32 len);
	virtual bool				ReciveMessage(IGameMessage* p);
	bool						Peek(char* buf, uint32 len);
	bool						SkipByte(uint32 len);
	int32						ReadFull();
	int32						ReadFullFromMemory(const char* pBuf, uint32 uSize);
	void						Initsize();
	bool						Resize(int32 size);
	uint32						Caps()const { return m_BufferLen; }
	uint32						Length()const;
	uint32						Size()const { return Length(); }
	bool						IsEmpty()const { return m_Head == m_Tail; }
	void						Clear();
	void						SetEncryptMode(uint32 mode);
	uint32						GetEncryptMode();
	uint32						GetHead() { return m_Head; }
	uint32						GetTail() { return m_Tail; }
	void						EnableEncrpt(bool bEnable);
	bool						IsEnableEncrpt();
	NetEncrptor*				GetEncrptor() { return m_pEncrptor; }
	void						Encode(void * dst, const void * src, uint32 count);
	void						Decode(void * dst, const void * src, uint32 count);
	void						SetMagicNum(uint32 uMsgHeadMagicNum, uint32 uMsgMagicNum);
	void						DeEncryptMsgHead(MSG_ID* MsgID);
	void						DeEncryptMsgHead(uint32* MsgUint);
	void						DeEncryptMessage(MSG_ID MsgID, uint32 uSize);
	void						SetFormatMode(uint32 mode);
	
protected:
	uint32						Reci_i(char* buf, uint32 len);

private:
	FSocket*					m_pSocket;
	uint8*						m_Buffer;
	uint32						m_BufferLen;
	uint32						m_MaxBufferLen;
	uint32						m_Head;
	uint32						m_Tail;
	uint32						m_Setting;
	NetEncrptor*				m_pEncrptor;
	NetMessageEncryptor			m_MessageEncryptor;
	uint32						m_uMsgHeadMagicNum;
	uint32						m_FormatMode;
};

class NetSendStream
{
public:
	NetSendStream(uint32 BufferSize = INITWRITESIZE,
		uint32 MaxBufferSize = 100 * 1024);
	virtual ~NetSendStream();


public:
	void						BindSocket(FSocket* socket) { m_pSocket = socket; }
	void						UnbindSocket() { m_pSocket = NULL; }
	uint32						Send(const char* buf, uint32 len);
	virtual bool				SendMessage(const IGameMessage* pPacket, bool bEncrypt = false);
	void						Initsize();
	bool						Resize(int32 size);
	int32						Caps()const { return m_BufferLen; }
	uint32						Length()const;
	uint32						Size()const { return Length(); }
	uint8*						GetBuffer()const { return m_Buffer; }
	bool						IsEmpty()const { return m_Head == m_Tail; }
	void						Clear();
	void						SetEncryptMode(uint32 mode);
	uint32						GetHead() { return m_Head; }
	uint32						GetTail() { return m_Tail; }
	bool						IsStaticStream() { return m_bStatic; }
	void						SetStaticStream(bool bStatic) { m_bStatic = bStatic; }
	bool						IsServerData() { return m_bServerData; }
	void						SetServerData(bool bServerData) { m_bServerData = bServerData; }

	void						UpdateTime(uint32 nowtime);
	void						ResetQos();
	void						CalQos(uint32 nFlushbytes);

	uint32						Flush();
	uint32						FlushServerStream();
	uint32						FlushClientStream();

	bool						HasDataToSend();

	void						EnableEncrpt(bool bEnable);
	bool						IsEnableEncrpt();
	NetEncrptor*				GetEncrptor() { return m_pEncrptor; }

	void						SetMagicNum(uint32 uMsgHeadMagicNum, uint32 uMsgMagicNum);
	void						SetFormatMode(uint32 mode);

protected:
	uint32						Send_i(const char* buf, uint32 len);

protected:

	FSocket*	m_pSocket;

	uint8*		m_Buffer;
	uint32		m_BufferLen;
	uint32		m_nInitBufferLen;
	uint32		m_MaxBufferLen;

	uint32		m_Head;
	uint32		m_Tail;

	uint32		m_uSendTail;

	uint32		m_Setting;
	bool		m_bServerData;
	bool		m_bStatic;
	bool		m_bFull;

	int32 		m_QosBytes;
	uint32		m_nowTime;
	uint32		m_LastSendTime;

	NetEncrptor*	m_pEncrptor;

	NetMessageEncryptor m_MessageEncryptor;
	uint32		m_uMsgHeadMagicNum;
	uint32		m_FormatMode;

	uint16		m_uSendCount;
	uint32		m_iUintOffset;
};