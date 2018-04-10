#ifndef _STREAM_SYSTEM_H
#define _STREAM_SYSTEM_H
#include "KwNetBasics.h"
#include "KwNetSocket.h"
#include "KwNetCompressMinilzo.h"
#include "KwNetMessageEncryptor.h"

#define INITRECSIZE						(64*1024)
#define INITWRITESIZE					(8*1024)
#define CM_HEADER_SIZE					(2)
#define CM_IN_SIZE						(1024*40)
#define CM_OUT_SIZE						(CM_IN_SIZE + CM_IN_SIZE/16 + 64 + 3 + CM_HEADER_SIZE)
#define	CM_MIN_CM_SIZE					100
// 服务器发给客户端的socket的数据，每次心跳(50ms)send的数据量，不宜过大，否则多用户同时传输时带宽峰值过大 [5/17/2017 yz]
#define SOCKET2CLINET_SIZE_PER_FRAME	(5120)

#if defined(__WINDOWS__)
#define SENDFLAG	MSG_DONTROUTE
#elif defined(IOS)
#define SENDFLAG	SO_NOSIGPIPE
#elif defined(__LINUX__)
#define SENDFLAG	MSG_NOSIGNAL
#endif

// 消息包类型 [wangxiaoyong]
enum ENCPYPT_TYPE
{
	ENCPYPT_TYPE_CLIENT				=0,		// 与客户端通信的消息包
	ENCPYPT_TYPE_PLATFORMSERVER		=1,		// 与nodejs server通信的消息包
	ENCPYPT_TYPE_LOCO				=2,		// 与乐动平台通信的消息包
	ENCPYPT_TYPE_LOCO_UNIFIEDANDROID =3,	// 与乐动平台的android大包的billing接入
	ENCPYPT_TYPE_LOCO_GM =4,				// 与乐动平台的android大包的billing接入
};

class Encrptor
{
public:
	enum { ED_DATA_LENGTH = 16};
public:
	Encrptor();
	~Encrptor();
	void* Encode(void * dst,const void * src,uint32 count);
	void* Decode(void * dst,const void * src,uint32 count);

	void  SetPdata(char* p,uint32 uMagicNum);
	char* GetPdata();
	char  enable;
private:
	char  pdata[ED_DATA_LENGTH];
	
};


class IMessage ;
class RecieveStream
{
public :
	RecieveStream( Socket* sock, 
		uint32 BufferSize = INITRECSIZE,
		uint32 MaxBufferSize = 500*1024 ) ;
	virtual ~RecieveStream( ) ;


public :
	uint32						Reci( char* buf, uint32 len ) ;
	virtual BOOL				ReciveMessage( IMessage* p ) ;
	BOOL						Peek( char* buf, uint32 len ) ;
	BOOL						SkipByte( uint32 len ) ;
	uint32						ReadFull( ) ;
	uint32						ReadFull_FromMemory(const char* pBuf,uint32 uSize);
	uint32						Fill_RAW( ) ;
	void						Initsize( ) ;
	BOOL						Resize( int32 size ) ;
	uint32						Caps( )const { return m_BufferLen; }
	uint32						Length( )const ;
	uint32						Size( )const { return Length(); }
	BOOL						IsEmpty( )const { return m_Head==m_Tail; }
	void						Clear( ) ;
	void						SetEncryptMode(uint32 mode);
	uint32						GetEncryptMode();
	uint32						GetHead() { return m_Head; }
	uint32						GetTail() { return m_Tail; }
	void						EnableDebugLog(BOOL bEnable) { m_bEnableDebugLog=bEnable; }
	BOOL						IsEnableDebugLog() { return m_bEnableDebugLog; }
	void						EnableEncrpt(BOOL bEnable);
	BOOL						IsEnableEncrpt();
	Encrptor*					GetEncrptor() { return m_pEncrptor; }
	void						Encode(void * dst,const void * src,uint32 count);
	void						Decode(void * dst,const void * src,uint32 count);
	void						SetMagicNum( uint32 uMsgHeadMagicNum, uint32 uMsgMagicNum );
	void						DeEncryptMsgHead(MSG_ID* MsgID);
	void						DeEncryptMsgHead(uint32* MsgUint);
	void						DeEncryptMessage(MSG_ID MsgID, uint32 uSize);
	// modify by faido
	void						SetFormatMode(uint32 mode);

private :
	Socket*						m_pSocket ;
	char*						m_Buffer ;
	uint32						m_BufferLen ;
	uint32						m_MaxBufferLen ;
	uint32						m_Head ;
	uint32						m_Tail ;
	uint32						m_Setting;
	BOOL						m_bEnableDebugLog;
	Encrptor*					m_pEncrptor;
	MessageEncryptor			m_MessageEncryptor;
	uint32						m_uMsgHeadMagicNum;
	// modify by faido
	uint32						m_FormatMode;
	// modify by faido
	uint32						Reci_i( char* buf, uint32 len ) ;
};

class CmStream
{
public:
	CmStream();
	~CmStream();
	void	Clear();

public:
	BOOL	AllocBuffer(uint32 uSize);
	char*	GetBuffer();
	char*	GetHeadBuffer();
	
	uint32	GetSize();
	uint32	GetMaxSize();
	uint32	GetFreeSize();

	BOOL	PushBack(uint32 uSize);
	BOOL	PopHead(uint32 uSize);
	void	SetHead(uint32 h) {m_uHead=h;}
	void	SetTail(uint32 t) {m_uTail=t;}
	void	AddMsgHeader();
	void	EncryptChange();
	void	ResetPos();
	void	SetEncrptor(Encrptor* pEncrptor);
private:
	char*	m_pBuffer;
	uint32	m_uHead;
	uint32	m_uTail;
	uint32	m_uMaxSize;
	Encrptor* m_pEncrptor;
	
};

class SendStream 
{
public :
	SendStream( Socket* sock, 
		uint32 BufferSize = INITWRITESIZE,
		uint32 MaxBufferSize = 100*1024 ) ;
	virtual ~SendStream( ) ;


public :
	uint32						Send( const char* buf, uint32 len ) ;
	virtual BOOL				SendMessage( const IMessage* pPacket, bool bEncrypt=false ) ;
	void						Initsize( ) ;
	BOOL						Resize( int32 size ) ;
	int32						Caps ()const { return m_BufferLen ; }
	uint32						Length( )const ;
	uint32						Size( )const { return Length( ) ; }
	char*						GetBuffer( )const { return m_Buffer ; }
	BOOL						IsEmpty ()const { return m_Head==m_Tail ; }
	void						Clear( ) ;
	void						SetEncryptMode(uint32 mode);
	uint32						GetHead() { return m_Head;}
	uint32						GetTail() { return m_Tail;}
	BOOL						IsStaticStream() { return m_bStatic;}
	void						SetStaticStream(BOOL bStatic) {m_bStatic = bStatic;}
	BOOL						IsServerData() { return m_bServerData; }
	void						SetServerData(BOOL bServerData) { m_bServerData = bServerData; }

	void						UpdateTime(uint32 nowtime);
	void						ResetQos();
	void						CalQos(uint32 nFlushbytes);
	
	uint32						Flush();
	uint32						FlushServerStream();
	uint32						FlushClientStream();
	uint32						FlushClientCmStream();
	BOOL						Compress2CmStream(uint32 uTail);
	uint32						FlushClientRawStream();
	void						PrepareClientStream(uint32 uTail);

	BOOL						IsRawStreamEmpty();
	uint32						GetFloorTail();
	void						EnableCompress(BOOL bEnable,TID threadID=0);
	void						EnableCompressLog(BOOL bEnable);
	void						EnableDebugLog(BOOL bEnable) { m_bEnableDebugLog = bEnable; }
	BOOL						IsEnableDebugLog() { return m_bEnableDebugLog; }
	BOOL						HasDataToSend();

	void						EnableEncrpt(BOOL bEnable);
	BOOL						IsEnableEncrpt();
	Encrptor*					GetEncrptor() { return m_pEncrptor; }

	void						SetMagicNum( uint32 uMsgHeadMagicNum, uint32 uMsgMagicNum );
	// modify by faido
	void						SetFormatMode(uint32 mode);

protected :

	Socket*		m_pSocket ;

	char*		m_Buffer ;
	uint32		m_BufferLen ;
	uint32		m_nInitBufferLen;
	uint32		m_MaxBufferLen ;

	uint32		m_Head ;
	uint32		m_Tail ;

	uint32		m_uSendTail;

	uint32		m_Setting;
	BOOL		m_bServerData;
	BOOL		m_bStatic;
	BOOL		m_bFull;

	int32 		m_QosBytes;
	uint32		m_nowTime;
	uint32		m_LastSendTime;

	BOOL		m_bEnableDebugLog;

	Compress_Assistant	m_CmAssist;
	CmStream	m_CmStream;
	Encrptor*	m_pEncrptor;

	MessageEncryptor m_MessageEncryptor;
	uint32		m_uMsgHeadMagicNum;
	// modify by faido
	uint32						m_FormatMode;

	// modify by faido
	uint32						Send_i( const char* buf, uint32 len ) ;	
	uint16		m_uSendCount;
	uint32		m_iUintOffset;
};

void 	WriteNetLog(const char *cszFormat, ...);
void *  write_memcpy ( void * dst, const void * src, uint32 count );
void *  read_memcpy ( void * dst, const void * src, uint32 count );
void *  read_bytess(char* dst,char* src,int32 ss);

// 对网络字节序进行转换 [3/20/2015 yangzhen]
template<typename T>
T RevertWordEndian(T val)
{
	T retVal = 0 ;
	uchar* destPos = (uchar*)&retVal;
	uchar* srcPos = (uchar*)&val;
	for(int32 i = 0; i < sizeof(T); i++)
	{
		destPos[i] = srcPos[sizeof(T) - 1 - i];
	}
	return retVal;
}


#endif
