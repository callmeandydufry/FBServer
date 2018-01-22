#include "KwNetPrivate.h"
#include "KwNetStreamSystem.h"
#include "KwNetMessage.h"
//#include "VMProtectSDK.h"

#ifdef __LINUX__
#include <sys/utsname.h>
#include <sys/time.h>
#endif

#define NET_ABS(x) (x)>0?(x):(-x)
void* Encrptor::Decode(void * dst,const void * src, uint32 count)
{
	//VMProtectBegin("Encrptor::Decode");

	void * ret = dst;
	while (count--) 
	{
		*(char *)dst = *(char *)src;
		if(enable)
		{
			char   low	= 0;
			char   high = 0;
    		low  = (*(char *)dst) & 0x0F;
			high = (*(char *)dst) & 0xF0;

			high = high^(pdata[low] & 0xF0);
			low  = ( ((low^0x0F)&0x0F) + pdata[0]&0x0F ) & 0x0F;
			*(char *)dst = high+low;
		}
		else
			*(char *)dst = (*(char *)dst)^0xFF;
		dst = (char *)dst + 1;
		src = (char *)src + 1;
	}
	//VMProtectEnd();
	return(ret);
}


void* Encrptor::Encode(void * dst,const void * src,uint32 count)
{
	//VMProtectBegin("Encrptor::Encode");
	void * ret = dst;
	while (count--) {
		*(char *)dst = *(char *)src;
		if(enable)
		{
			char   low	= 0;
			char   high = 0;
    		low  = (*(char *)dst) & 0x0F;
			high = (*(char *)dst) & 0xF0;
			low  = ((low - pdata[0]&0x0F) & 0x0F)^0x0F;
			high = high^(pdata[low] & 0xF0);
			*(char *)dst = high+low;
		}
		else
			*(char *)dst = (*(char *)dst)^0xFF;
		dst = (char *)dst + 1;
		src = (char *)src + 1;
	}
	//VMProtectEnd();
	return(ret);
}

void  Encrptor::SetPdata(char* p,uint32 uMagicNum)
{
	char buf[ED_DATA_LENGTH] = {0};
	memcpy( buf, p, ED_DATA_LENGTH );
	for( int32 i=0; i<ED_DATA_LENGTH; i++ )
	{
		buf[i] += (char)uMagicNum;
	}
	memcpy( pdata, buf, ED_DATA_LENGTH );
}

char* Encrptor::GetPdata()
{
	return pdata;
}

Encrptor::Encrptor()
{
	memset(pdata,0,sizeof(pdata));
	enable = 0;
};

Encrptor::~Encrptor()
{
	
}

void *  read_memcpy(void * dst,
					 const void * src,
					 uint32 count
					 )
{
	//VMProtectBegin("read_memcpy");
	void * ret = dst;
	while (count--) {
		*(char *)dst = *(char *)src;
		*(char *)dst = (*(char *)dst)^0xFF;
		dst = (char *)dst + 1;
		src = (char *)src + 1;
	}
	//VMProtectEnd();
	return(ret);
}

RecieveStream::RecieveStream( Socket* sock, uint32 BufferLen, uint32 MaxBufferLen ) 
{
	GUARD

	m_pSocket = sock ;
	m_BufferLen = BufferLen ;
	m_MaxBufferLen = MaxBufferLen ;

	m_Head = 0 ;
	m_Tail = 0 ;

	m_Buffer = new char[ m_BufferLen ];
	memset( m_Buffer, 0, m_BufferLen ) ;
	m_Setting = 0;
	m_pEncrptor = new Encrptor();
	m_bEnableDebugLog = FALSE;
	m_uMsgHeadMagicNum = 0;

	// modify by faido
	m_FormatMode = 0;

	UNGUARD
}

RecieveStream::~RecieveStream( ) 
{
	GUARD

	KSafeDelete_ARRAY(m_Buffer);
	KSafeDelete(m_pEncrptor);

	UNGUARD
}

void RecieveStream::SetMagicNum( uint32 uMsgHeadMagicNum, uint32 uMsgMagicNum )
{
	m_uMsgHeadMagicNum = uMsgHeadMagicNum;
	m_MessageEncryptor.SetMagicNum(uMsgMagicNum);
}

void RecieveStream::DeEncryptMsgHead(MSG_ID* MsgID)
{
	(*MsgID) = (*MsgID) ^ m_uMsgHeadMagicNum;
	if( ((*MsgID) & 0x8000) != 0 )
		(*MsgID) &= 0x7FFF;
}

void RecieveStream::DeEncryptMsgHead(uint32* MsgUint)
{
	(*MsgUint) = (*MsgUint) ^ m_uMsgHeadMagicNum; 
}

void RecieveStream::DeEncryptMessage(MSG_ID MsgID, uint32 uSize)
{
	m_MessageEncryptor.Decode( MsgID, (uchar*)m_Buffer, (m_Head+6)%m_BufferLen, (m_Head+uSize+6)%m_BufferLen, m_BufferLen ); 
}

uint32 RecieveStream::Length( )const
{
	GUARD

		if( m_Head<m_Tail )
			return m_Tail-m_Head;

		else if( m_Head>m_Tail ) 
			return m_BufferLen-m_Head+m_Tail ;

	return 0 ;

	UNGUARD

		return 0 ;
}
//modify by faido
uint32 RecieveStream::Reci( char* buf, uint32 len ) 
{
	GUARD

		if (m_FormatMode == 0)
		{
			return Reci_i(buf,len);
		}
		else
		{
			//first recv len
			uint16 uLen = 0;

			if (Reci_i((char*)&uLen,sizeof(uLen)) > 0)
			{
				//get the min len
				uint32 uMinLen = len > uLen ? uLen : len;

				uMinLen =  Reci_i(buf,uMinLen);
				if (uMinLen > 0)
				{
					//set left buf to 0
					uint32 uLeft = len - uMinLen;
					if (uLeft > 0)
					{
						memset(buf+uMinLen, 0 , uLeft);
					}

					return len;
				}

				return 0;
			}

			return 0;
		}


	UNGUARD

		return 0 ;
}

//modify by faido
uint32 RecieveStream::Reci_i( char* buf, uint32 len ) 
{
	GUARD

		if ( len == 0 )
			return 0 ;

	if ( len > Length() )
		return 0 ;

	if ( m_Head < m_Tail ) 
	{
		if(m_Setting == 0)
		{
			m_pEncrptor->Decode(buf, &m_Buffer[m_Head],len) ;
		}
		else
		{
			memcpy( buf, &m_Buffer[m_Head], len ) ;
		}
	} 
	else 
	{
		uint32 rightLen = m_BufferLen-m_Head ;
		if( len<=rightLen ) 
		{
			if(m_Setting==0)
			{
				m_pEncrptor->Decode( buf, &m_Buffer[m_Head], len ) ;
			}
			else
			{
				memcpy( buf, &m_Buffer[m_Head], len ) ;
			}
		} 
		else 
		{
			if(m_Setting==0)
			{
				m_pEncrptor->Decode( buf, &m_Buffer[m_Head], rightLen ) ;
				m_pEncrptor->Decode( &buf[rightLen], m_Buffer, len-rightLen ) ;
			}
			else
			{
				memcpy( buf, &m_Buffer[m_Head], rightLen ) ;
				memcpy( &buf[rightLen], m_Buffer, len-rightLen ) ;
			}

		}
	}

	m_Head = (m_Head+len)%m_BufferLen ;

	if( IsEnableDebugLog() )
	{
		WriteNetLog( "[Debug] Reci len=%d, head=%d, tail=%d", len, m_Head, m_Tail );
	}

	return len ;

	UNGUARD

		return 0 ;
}


BOOL RecieveStream::ReciveMessage( IMessage* pMsg ) 
{
	GUARD

		BOOL ret ;

	ret = SkipByte( MSG_HEADER_SIZE ) ;
	if( !ret )
		return FALSE ;

	return pMsg->Recieve( *this ) ;

	UNGUARD

		return FALSE ;
}


BOOL RecieveStream::Peek( char* buf, uint32 len ) 
{
	GUARD

		if( len==0 )
			return FALSE ;

	if( len>Length() )
		return FALSE ;

	if( m_Head<m_Tail ) 
	{
		if(m_Setting==0)
		{
			m_pEncrptor->Decode( buf , &m_Buffer[m_Head] , len );
		}
		else
		{
			memcpy( buf , &m_Buffer[m_Head] , len );
		}

	} 
	else 
	{
		uint32 rightLen = m_BufferLen-m_Head ;
		if( len<=rightLen ) 
		{
			if(m_Setting==0)
			{
				m_pEncrptor->Decode( &buf[0], &m_Buffer[m_Head], len ) ;
			}
			else
			{
				memcpy( &buf[0], &m_Buffer[m_Head], len ) ;
			}
		} 
		else 
		{
			if(m_Setting==0)
			{
				m_pEncrptor->Decode( &buf[0], &m_Buffer[m_Head], rightLen ) ;
				m_pEncrptor->Decode( &buf[rightLen], &m_Buffer[0], len-rightLen ) ;
			}
			else
			{
				memcpy( &buf[0], &m_Buffer[m_Head], rightLen ) ;
				memcpy( &buf[rightLen], &m_Buffer[0], len-rightLen ) ;
			}
		}
	}

	return TRUE ;

	UNGUARD

		return FALSE ;
}

BOOL RecieveStream::SkipByte( uint32 len ) 
{
	GUARD
	if( len == 0 ) return FALSE ;

	if( len>Length( ) )	return FALSE ;
	m_Head = (m_Head+len)%m_BufferLen ;
	return TRUE ;
	UNGUARD
	return FALSE ;
}

void RecieveStream::Initsize( )
{
	m_Head = 0 ;
	m_Tail = 0 ;
	KSafeDelete_ARRAY( m_Buffer ) ;
	m_Buffer = new char[INITRECSIZE] ;
	m_BufferLen = INITRECSIZE ;
	memset( m_Buffer, 0, m_BufferLen ) ;
}
uint32 RecieveStream::ReadFull( ) 
{
	GUARD

	uint32 nFilled = 0 ;
	uint32 nRec = 0 ;
	uint32 nFree = 0 ;
	if ( m_Head <= m_Tail ) 
	{
		if ( m_Head == 0 ) 
		{
			nRec = 0 ;
			nFree = m_BufferLen-m_Tail-1 ;
			if( nFree != 0 )
			{
				nRec = m_pSocket->receive( &m_Buffer[m_Tail] , nFree );
				if (nRec==SOCKET_ERROR_WOULDBLOCK) return 0 ; 
				if (nRec==SOCKET_ERROR) return SOCKET_ERROR-1 ;
				if (nRec==0) return SOCKET_ERROR-2 ;
				m_Tail += nRec;
				nFilled += nRec;
			}

			if( nRec == nFree ) 
			{
				uint32 available = m_pSocket->available();
				if ( available > 0 ) 
				{
					if( (m_BufferLen+available+1)>m_MaxBufferLen )
					{
						Initsize( ) ;
						return SOCKET_ERROR-3 ;
					}
					if( !Resize( available+1 ) )
						return 0 ;

					nRec = m_pSocket->receive( &m_Buffer[m_Tail] , available );
					if (nRec==SOCKET_ERROR_WOULDBLOCK) return 0 ; 
					if (nRec==SOCKET_ERROR) return SOCKET_ERROR-4 ;
					if (nRec==0) return SOCKET_ERROR-5;

					m_Tail += nRec;
					nFilled += nRec;
				}
			}
		} 
		else 
		{
			nFree = m_BufferLen-m_Tail ;
			nRec = m_pSocket->receive( &m_Buffer[m_Tail], nFree );
			if( nRec==SOCKET_ERROR_WOULDBLOCK ) return 0 ; 
			if( nRec==SOCKET_ERROR ) return SOCKET_ERROR-6 ;
			if( nRec==0 ) return SOCKET_ERROR-7 ;

			m_Tail = (m_Tail+nRec)%m_BufferLen ;
			nFilled += nRec ;

			if( nRec==nFree ) 
			{
				nRec = 0 ;
				nFree = m_Head-1 ;
				if( nFree!=0 )
				{
					nRec = m_pSocket->receive( &m_Buffer[0] , nFree );
					if( nRec==SOCKET_ERROR_WOULDBLOCK ) return 0 ; 
					if( nRec==SOCKET_ERROR ) return SOCKET_ERROR -8;
					if( nRec==0 ) return SOCKET_ERROR-9 ;

					m_Tail += nRec;
					nFilled += nRec;
				}

				if( nRec==nFree ) 
				{
					uint32 available = m_pSocket->available();
					if ( available > 0 ) 
					{
						if( (m_BufferLen+available+1)>m_MaxBufferLen )
						{
							Initsize( ) ;
							return SOCKET_ERROR-10 ;
						}
						if( !Resize( available+1 ) )
							return 0 ;

						nRec = m_pSocket->receive( &m_Buffer[m_Tail] , available );
						if (nRec==SOCKET_ERROR_WOULDBLOCK) return 0 ; 
						if (nRec==SOCKET_ERROR) return SOCKET_ERROR-11 ;
						if (nRec==0) return SOCKET_ERROR-12;

						m_Tail += nRec;
						nFilled += nRec;
					}
				}
			}
		}

	} 
	else 
	{	
		nRec = 0 ;
		nFree = m_Head-m_Tail-1 ;
		if( nFree!=0 )
		{
			nRec = m_pSocket->receive( &m_Buffer[m_Tail], nFree ) ;
			if( nRec==SOCKET_ERROR_WOULDBLOCK ) return 0 ; 
			if( nRec==SOCKET_ERROR ) return SOCKET_ERROR-13 ;
			if( nRec==0 ) return SOCKET_ERROR-14 ;

			m_Tail += nRec ;
			nFilled += nRec ;
		}
		if( nRec==nFree ) 
		{
			uint32 available = m_pSocket->available( ) ;
			if ( available>0 ) 
			{
				if( (m_BufferLen+available+1)>m_MaxBufferLen )
				{
					Initsize( ) ;
					return SOCKET_ERROR-15 ;
				}
				if( !Resize( available+1 ) )
					return 0 ;

				nRec = m_pSocket->receive( &m_Buffer[m_Tail], available ) ;
				if( nRec==SOCKET_ERROR_WOULDBLOCK ) return 0 ; 
				if( nRec==SOCKET_ERROR ) return SOCKET_ERROR-16 ;
				if( nRec==0 ) return SOCKET_ERROR-17 ;

				m_Tail += nRec ;
				nFilled += nRec ;
			}
		}
	}

	if( IsEnableDebugLog() )
	{
		WriteNetLog( "[Debug] ReadFull fill=%d, head=%d, tail=%d", nFilled, m_Head, m_Tail );
	}

	return nFilled ;

	UNGUARD

		return 0 ;
}

uint32 RecieveStream::ReadFull_FromMemory(const char* pBuf,uint32 uSize)
{
	uint32 uFree = 0;
	uint32 uFilled = 0;

	if ( m_Head <= m_Tail )
	{
		if ( m_Head == 0 ) 
		{
			uFree = m_BufferLen-m_Tail-1 ;
			uint32 uCpSize = uFree > uSize ? uSize : uFree;
			if(m_Setting == 0)
			{
				m_pEncrptor->Encode( &m_Buffer[m_Tail], pBuf, uCpSize ) ;
			}
			else
			{
				memcpy( &m_Buffer[m_Tail], pBuf, uCpSize ) ;
			}
			uFilled += uCpSize;
			m_Tail += uCpSize;
		}
		else
		{
			uFree = m_BufferLen-m_Tail;
			uint32 uCpSize1 = uFree > uSize ? uSize : uFree;
			if(m_Setting == 0)
			{
				m_pEncrptor->Encode( &m_Buffer[m_Tail], pBuf, uCpSize1 ) ;
			}
			else
			{
				memcpy( &m_Buffer[m_Tail], pBuf, uCpSize1 ) ;
			}
			uFilled += uCpSize1;
			m_Tail = (m_Tail+uCpSize1)%m_BufferLen;
			pBuf += uCpSize1;

			uFree = m_Head-1;
			uint32 uCpSize2 = uFree > (uSize-uCpSize1) ? (uSize-uCpSize1) : uFree;
			if(m_Setting == 0)
			{
				m_pEncrptor->Encode( &m_Buffer[m_Tail], pBuf, uCpSize2 ) ;
			}
			else
			{
				memcpy( &m_Buffer[m_Tail], pBuf, uCpSize2 ) ;
			}
			uFilled += uCpSize2;
			m_Tail += uCpSize2;
		}
	}
	else
	{
		uFree = m_Head-m_Tail-1;
		uint32 uCpSize = uFree > uSize ? uSize : uFree;
		if(m_Setting == 0)
		{
			m_pEncrptor->Encode( &m_Buffer[m_Tail], pBuf, uCpSize ) ;
		}
		else
		{
			memcpy( &m_Buffer[m_Tail], pBuf, uCpSize ) ;
		}
		uFilled += uCpSize;
		m_Tail += uCpSize;
	}

	return uFilled;
}

BOOL RecieveStream::Resize( int32 size )
{
	GUARD

		size = max(size, (int)(m_BufferLen>>1));
	uint32 newBufferLen = m_BufferLen + size;
	uint32 len = Length();

	if ( size < 0 ) 
	{
		if ( newBufferLen < 0 || newBufferLen < len )
			return FALSE ;		
	} 

	char * newBuffer = new char[ newBufferLen ];
	if ( m_Head < m_Tail ) 
	{
		memcpy( newBuffer , &m_Buffer[m_Head] , m_Tail - m_Head );
	} 
	else if ( m_Head > m_Tail ) 
	{
		memcpy( newBuffer , &m_Buffer[m_Head] , m_BufferLen - m_Head );
		memcpy( &newBuffer[ m_BufferLen - m_Head ] , m_Buffer , m_Tail );
	}

	delete [] m_Buffer ;

	m_Buffer = newBuffer ;
	m_BufferLen = newBufferLen ;
	m_Head = 0 ;
	m_Tail = len ;

	return TRUE ;

	UNGUARD

		return FALSE ;
}

void RecieveStream::Clear( )
{
	GUARD

	m_Head = 0 ;
	m_Tail = 0 ;

	UNGUARD
}

void RecieveStream::SetEncryptMode(uint32 mode)
{
	m_Setting = mode;
}

uint32 RecieveStream::GetEncryptMode()
{
	return m_Setting;
}

void RecieveStream::EnableEncrpt(BOOL bEnable)
{
	m_pEncrptor->enable = (char)bEnable;
}

BOOL RecieveStream::IsEnableEncrpt()
{
	return (BOOL)m_pEncrptor->enable;
}

void RecieveStream::Encode(void * dst,const void * src,uint32 count)
{
	m_pEncrptor->Encode(dst,src,count);
}

void RecieveStream::Decode(void * dst,const void * src,uint32 count)
{
	m_pEncrptor->Decode(dst,src,count);
}
// modify by faido
void RecieveStream::SetFormatMode(uint32 mode)
{
	m_FormatMode = mode;
}



void *  write_memcpy( void * dst,
					  const void * src,
					  uint32 count
					  )
{
	//VMProtectBegin("write_memcpy");
	void * ret = dst;
	while (count--) {
		*(char *)dst = *(char *)src;
		*(char *)dst = (*(char *)dst)^0xFF;
		dst = (char *)dst + 1;
		src = (char *)src + 1;
	}
	//VMProtectEnd();
	return(ret);
}

SendStream::SendStream( Socket* sock, uint32 BufferLen, uint32 MaxBufferLen ) 
{
	GUARD

	m_pSocket = sock ;
	m_BufferLen = BufferLen ;
	m_nInitBufferLen = BufferLen;
	m_MaxBufferLen = MaxBufferLen ;
	m_Head = 0 ;
	m_Tail = 0 ;
	m_uSendTail = 0;

	m_Buffer = new char[ m_BufferLen ];
	memset( m_Buffer, 0, m_BufferLen ) ;
	m_Setting = 0;
	m_bStatic = FALSE;
	m_bServerData = TRUE;
	m_bFull	  = FALSE;
	m_QosBytes =		1024*3;
	m_nowTime  =		0;
	m_LastSendTime =	0;
	m_CmStream.AllocBuffer(CM_OUT_SIZE);
	m_bEnableDebugLog = FALSE;
	m_pEncrptor = new Encrptor();

	m_CmStream.SetEncrptor(m_pEncrptor);

	m_uMsgHeadMagicNum = 0;

	// modify by faido
	m_FormatMode = 0;

	UNGUARD
}

SendStream::~SendStream( ) 
{
	GUARD

	KSafeDelete(m_pEncrptor);
	KSafeDelete_ARRAY(m_Buffer);
	

	UNGUARD
}

void SendStream::SetMagicNum( uint32 uMsgHeadMagicNum, uint32 uMsgMagicNum )
{
	m_uMsgHeadMagicNum = uMsgHeadMagicNum;
	m_MessageEncryptor.SetMagicNum(uMsgMagicNum);
}


void SendStream::UpdateTime(uint32 nowtime)
{
	GUARD
	
		m_nowTime = nowtime;

	UNGUARD
}

void SendStream::ResetQos()
{
	m_QosBytes = 1024*3;
	m_LastSendTime = m_nowTime;
}

void SendStream::CalQos(uint32 nFlushbytes)
{
	m_QosBytes -= nFlushbytes;
	
	if(m_QosBytes<0)
	{
		m_QosBytes = 0;
	}
}
uint32 SendStream::Length( )const
{
	GUARD

		if( m_Head<m_Tail )
			return m_Tail-m_Head;

		else if( m_Head>m_Tail ) 
			return m_BufferLen-m_Head+m_Tail ;

	return 0 ;

	UNGUARD

		return 0 ;
}

// modify by faido
uint32 SendStream::Send( const char* buf, uint32 len )
{
	GUARD

		if (m_FormatMode == 0)
		{
			return Send_i(buf,len);
		}
		else
		{
			//first send len
			uint16 uLen = (uint16)len;

			if (Send_i((const char*)&uLen,sizeof(uLen)) > 0)
			{
				m_uSendCount += sizeof(uLen);
				return Send_i(buf,len);
			}

			return 0;
		}


	UNGUARD

		return 0 ;
}


// modify by faido
uint32 SendStream::Send_i( const char* buf, uint32 len ) 
{
	GUARD

	uint32 nFree = ( (m_Head<=m_Tail)?(m_BufferLen-m_Tail+m_Head-1):(m_Head-m_Tail-1) ) ;

	if( len>=nFree )
	{
		if( !Resize( len-nFree+1 ) )
			return 0 ;
	}

	if( m_Head<=m_Tail ) 
	{	
		if( m_Head==0 ) 
		{
			nFree = m_BufferLen - m_Tail - 1;
			if(m_Setting==0)
			{
				m_pEncrptor->Encode( &m_Buffer[m_Tail], buf, len ) ;
			}
			else
			{

				memcpy( &m_Buffer[m_Tail], buf, len ) ;
			}
		} 
		else 
		{
			nFree = m_BufferLen-m_Tail ;
			if( len<=nFree )
			{
				if(m_Setting==0)
				{
					m_pEncrptor->Encode( &m_Buffer[m_Tail], buf, len ) ;
				}
				else
				{
					memcpy( &m_Buffer[m_Tail], buf, len ) ;
				}
			}
			else 
			{
				if(m_Setting == 0 )
				{
					m_pEncrptor->Encode( &m_Buffer[m_Tail], buf, nFree ) ;
					m_pEncrptor->Encode( m_Buffer, &buf[nFree], len-nFree ) ;
				}
				else
				{
					memcpy( &m_Buffer[m_Tail], buf, nFree ) ;
					memcpy( m_Buffer, &buf[nFree], len-nFree ) ;
				}
			}
		}
	} 
	else 
	{	
		if(m_Setting == 0 )
		{
			m_pEncrptor->Encode( &m_Buffer[m_Tail], buf, len ) ;
		}
		else
		{
			memcpy( &m_Buffer[m_Tail], buf, len ) ;
		}
	}

	m_Tail = (m_Tail+len)%m_BufferLen ;

	if( IsEnableDebugLog() )
	{
		WriteNetLog( "[Debug] Send len=%d, head=%d, tail=%d", len, m_Head, m_Tail );
	}

	return len;

	UNGUARD

		return 0 ;
}

// modify by faido
BOOL SendStream::SendMessage( const IMessage* pMsg, bool bEncrypt/*=false*/ )
{
	GUARD

	MSG_ID msgID = pMsg->GetMsgID();
	if( bEncrypt )	
	{
		msgID = msgID ^ m_uMsgHeadMagicNum;
		if( (msgID & 0x8000) != 0 )
			msgID &= 0x7FFF;
	}
	uint32 w = Send_i( (char*)&msgID , sizeof(MSG_ID) ) ;
	uint32 packetUINT ;
	uint32 MsgSize = pMsg->GetMsgSize( ) ;
	uint32 MsgIndex = pMsg->GetComID( ) ;
	if(m_Setting==0)
	{
		SET_MSG_INDEX(packetUINT, MsgIndex) ;
		SET_MSG_LEN(packetUINT, MsgSize) ;
	}
	else
	{
		BILL_SET_MSG_INDEX(packetUINT, MsgIndex) ;
		BILL_SET_MSG_LEN(packetUINT, MsgSize) ;
	}

	if( bEncrypt)	packetUINT = packetUINT ^ m_uMsgHeadMagicNum;

	
	// for client format
	if (m_FormatMode > 0)
	{
		m_uSendCount = 0;
		// save offset
		m_iUintOffset = m_Tail;
	}
	

	w = Send_i( (char*)&packetUINT, sizeof(uint32) ) ;
	
	if( !bEncrypt )
	{
		if( pMsg->Send( *this ) )
		{
			if (m_FormatMode > 0)
			{
				//reset size
				uint32 uTemp = m_Tail;

				uint32 packetUINT ;
				uint32 MsgSize = pMsg->GetMsgSize( ) + m_uSendCount;
				uint32 MsgIndex = pMsg->GetComID( ) ;
				if(m_Setting==0)
				{
					SET_MSG_INDEX(packetUINT, MsgIndex) ;
					SET_MSG_LEN(packetUINT, MsgSize) ;
				}
				else
				{
					BILL_SET_MSG_INDEX(packetUINT, MsgIndex) ;
					BILL_SET_MSG_LEN(packetUINT, MsgSize) ;
				}

				if( bEncrypt)	packetUINT = packetUINT ^ m_uMsgHeadMagicNum;

				m_Tail = m_iUintOffset;
				w = Send_i( (char*)&packetUINT, sizeof(uint32) ) ;
				m_Tail = uTemp;
			}
			return TRUE;
		}
		else
			return FALSE;
	}
	else
	{
		uint32 uTailBefore = m_Tail;
		if( pMsg->Send(*this) )
		{
			if (m_FormatMode > 0)
			{
				//reset size
				uint32 uTemp = m_Tail;

				uint32 packetUINT ;
				uint32 MsgSize = pMsg->GetMsgSize( ) + m_uSendCount;
				uint32 MsgIndex = pMsg->GetComID( ) ;
				if(m_Setting==0)
				{
					SET_MSG_INDEX(packetUINT, MsgIndex) ;
					SET_MSG_LEN(packetUINT, MsgSize) ;
				}
				else
				{
					BILL_SET_MSG_INDEX(packetUINT, MsgIndex) ;
					BILL_SET_MSG_LEN(packetUINT, MsgSize) ;
				}

				if( bEncrypt)	packetUINT = packetUINT ^ m_uMsgHeadMagicNum;

				m_Tail = m_iUintOffset;
				w = Send_i( (char*)&packetUINT, sizeof(uint32) ) ;
				m_Tail = uTemp;
			}

			m_MessageEncryptor.Encode( pMsg->GetMsgID()+pMsg->GetComID(), (uchar*)m_Buffer, uTailBefore, m_Tail, m_BufferLen );
			return TRUE;
		}
		else
			return FALSE;
	}

	UNGUARD
	return FALSE ;
}

void SendStream::Initsize( )
{
	m_Head = 0 ;
	m_Tail = 0 ;
	if(m_bStatic)
	{
		m_bFull = FALSE;
		return;
	}
	KSafeDelete_ARRAY( m_Buffer ) ;
	m_Buffer = new char[INITWRITESIZE] ;
	m_BufferLen = INITWRITESIZE ;
	memset( m_Buffer, 0, m_BufferLen ) ;
}

uint32 SendStream::Flush()
{
	if( IsServerData() )
	{
		return FlushServerStream();
	}
	else
	{
		if( !m_CmAssist.IsEanble() )
		{
			return FlushClientStream();
		}
		else
		{
			if( IsEnableDebugLog() )
			{
				WriteNetLog( "[DEBUG] Flush Length=%d", Length() );
			}

			uint32 uRet;
			uint32 uSend = 0;
			uint32 uTail = GetFloorTail();
			if( (int32)uTail < (int32)-1 )
				return uTail;

			if( TRUE == Compress2CmStream(uTail) )
				uTail = (uint32)-1;

			uRet = FlushClientCmStream();
			uSend += uRet;
			if( (int32)uRet <= SOCKET_ERROR )
				return uRet;

			PrepareClientStream(uTail);
			
			uRet = FlushClientRawStream();
			uSend += uRet;
			if( (int32)uRet <= SOCKET_ERROR )
				return uRet;
		
			return uSend;
		}
	}
}

uint32 SendStream::FlushServerStream()
{
	GUARD
	uint32 nFlushed = 0;
	uint32 nSent = 0;
	uint32 nLeft;

	if( m_BufferLen>m_MaxBufferLen )
	{
		Initsize( ) ;
		return SOCKET_ERROR-1 ;
	}

	_NET_TRY 
	{
		if ( m_Head < m_Tail ) 
		{
			nLeft = m_Tail - m_Head;
			while ( nLeft > 0 ) 
			{
				nSent = m_pSocket->send( &m_Buffer[m_Head] , nLeft , SENDFLAG );
				if (nSent==SOCKET_ERROR_WOULDBLOCK) return 0 ; 
				if (nSent==SOCKET_ERROR) return SOCKET_ERROR-2 ;
				if (nSent==0) return 0;

				nFlushed += nSent;
				nLeft -= nSent;
				m_Head += nSent;
			}
		} 
		else if( m_Head > m_Tail ) 
		{
			nLeft = m_BufferLen - m_Head;
			while ( nLeft > 0 ) 
			{
				nSent = m_pSocket->send( &m_Buffer[m_Head] , nLeft , SENDFLAG );
				if (nSent==SOCKET_ERROR_WOULDBLOCK) return 0 ; 
				if (nSent==SOCKET_ERROR) return SOCKET_ERROR-3 ;
				if (nSent==0) return 0;

				nFlushed += nSent;
				nLeft -= nSent;
				m_Head += nSent;
			}

			m_Head = 0;
			nLeft = m_Tail;

			while ( nLeft > 0 ) 
			{
				nSent = m_pSocket->send( &m_Buffer[m_Head] , nLeft , SENDFLAG );
				if (nSent==SOCKET_ERROR_WOULDBLOCK) return 0 ; 
				if (nSent==SOCKET_ERROR) return SOCKET_ERROR-4 ;
				if (nSent==0) return 0;

				nFlushed += nSent;
				nLeft -= nSent;
				m_Head += nSent;
			}
		}
	}
	_NET_CATCH
	{
		if( nSent>0 )
		{
			m_Head += nSent;
		}
	} 
	m_Head = m_Tail = 0 ;
	return nFlushed;
	UNGUARD
	return 0 ;	
}

uint32 SendStream::FlushClientStream()
{
	GUARD
	uint32 nFlushed = 0;
	uint32 nSent = 0;
	uint32 nLeft;

	if( m_BufferLen>m_MaxBufferLen || m_bFull )
	{
		Initsize( ) ;
		return SOCKET_ERROR-1 ;
	}

	_NET_TRY 
	{
		BOOL bTotalSend = TRUE;
		if ( m_Head < m_Tail ) 
		{
			nLeft = m_Tail - m_Head;
			if (nLeft>SOCKET2CLINET_SIZE_PER_FRAME)
			{
				nLeft = SOCKET2CLINET_SIZE_PER_FRAME;
				bTotalSend = FALSE;
			}
			while ( nLeft > 0 ) 
			{
				nSent = m_pSocket->send( &m_Buffer[m_Head] , nLeft , SENDFLAG );
				if (nSent==SOCKET_ERROR_WOULDBLOCK) return 0; 
				if (nSent==SOCKET_ERROR) return SOCKET_ERROR-2;
				if (nSent==0) return 0;
				nFlushed += nSent;
				nLeft -= nSent;
				m_Head += nSent;
			}
			if( bTotalSend == FALSE )
			{
				if( IsEnableDebugLog() )
				{
					WriteNetLog( "[Debug] FlushClientStream1 flush=%d, head=%d, tail=%d", nFlushed, m_Head, m_Tail );
				}
				return nFlushed;
			}
		} 
		else if( m_Head > m_Tail ) 
		{
			nLeft = m_BufferLen - m_Head;
			if (nLeft>SOCKET2CLINET_SIZE_PER_FRAME)
			{
				nLeft = SOCKET2CLINET_SIZE_PER_FRAME;
				bTotalSend = FALSE;
			}
			while ( nLeft > 0 ) 
			{
				nSent = m_pSocket->send( &m_Buffer[m_Head] , nLeft , SENDFLAG );
				if (nSent==SOCKET_ERROR_WOULDBLOCK) return 0 ; 
				if (nSent==SOCKET_ERROR) return SOCKET_ERROR-3 ;
				if (nSent==0) return 0;

				nFlushed += nSent;
				nLeft -= nSent;
				m_Head += nSent;
			}
			if( bTotalSend == FALSE )
			{
				if( IsEnableDebugLog() )
				{
					WriteNetLog( "[Debug] FlushClientStream2 flush=%d, head=%d, tail=%d", nFlushed, m_Head, m_Tail );
				}
				return nFlushed;
			}
			m_Head = 0;
		}
	}
	_NET_CATCH
	{
		if( nSent>0 )
		{
			m_Head += nSent;
		}
	} 

	if( m_Head == m_Tail )
		m_Head = m_Tail = 0 ;

	if( IsEnableDebugLog() )
	{
		WriteNetLog( "[Debug] FlushClientStream3 flush=%d, head=%d, tail=%d", nFlushed, m_Head, m_Tail );
	}

	return nFlushed;
	UNGUARD
	return 0 ;	
}

uint32 SendStream::FlushClientCmStream()
{
	GUARD

	if( m_CmStream.GetSize() == 0 )
		return 0;

	uint32 nFlushed = 0;
	uint32 nLeft = m_CmStream.GetSize();
	uint32 nSend = 0;

	_NET_TRY
	{
		BOOL bTotalSend = TRUE;
		if (nLeft > SOCKET2CLINET_SIZE_PER_FRAME)
		{
			nLeft = SOCKET2CLINET_SIZE_PER_FRAME;
			bTotalSend = FALSE;
		}

		while ( nLeft > 0 ) 
		{
			nSend = m_pSocket->send( m_CmStream.GetHeadBuffer(), nLeft, SENDFLAG );
			if (nSend==SOCKET_ERROR_WOULDBLOCK) return 0 ; 
			if (nSend==SOCKET_ERROR) return SOCKET_ERROR-14 ;
			if (nSend==0) return 0;

			nFlushed += nSend;
			m_CmStream.PopHead( nSend );
			nLeft -= nSend;
		}

		if( bTotalSend == FALSE )
		{
			if( IsEnableDebugLog() )
			{
				WriteNetLog( "[Debug] FlushClientCmStream1 flush=%d, head=%d, tail=%d", nFlushed, m_Head, m_Tail );
			}
			return nFlushed;
		}
	}
	_NET_CATCH
	{
		if( nSend > 0 )
		{
			m_CmStream.PopHead(nSend);
		}
	}

	m_CmStream.ResetPos();

	if( IsEnableDebugLog() )
	{
		WriteNetLog( "[Debug] FlushClientCmStream2 flush=%d, head=%d, tail=%d", nFlushed, m_Head, m_Tail );
	}

	return nFlushed;
	UNGUARD
	return 0;
}

BOOL SendStream::Compress2CmStream(uint32 uTail)
{
	GUARD

	if( uTail == (uint32)-1 || m_CmStream.GetSize()!=0 || !IsRawStreamEmpty() )
		return FALSE;

	if( m_Head > uTail )
		return FALSE;

	if( m_BufferLen>m_MaxBufferLen || m_bFull )
	{
		Initsize( ) ;
		return SOCKET_ERROR-20 ;
	}

	m_CmStream.ResetPos();

	char* pInBuffer = NULL;
	lzo_uint uInSize = uTail - m_Head;
	lzo_uint uOutSize = 0;

	m_CmAssist.IncCmFrame();

	if( uInSize < CM_MIN_CM_SIZE )
		return FALSE;

	pInBuffer = m_Buffer + m_Head;

	BOOL bRet = Compress_Minilzo::GetInstance().Compress
		(	
		(const uchar*)pInBuffer,
		uInSize,
		(uchar*)m_CmStream.GetHeadBuffer(), 
		uOutSize,
		m_CmAssist.GetWrkMem()
		);
	if( bRet == FALSE )
	{
		if( IsEnableDebugLog() )
		{
			WriteNetLog( "[Debug] Compress2CmStream FAIL!! head=%d, tail=%d", m_Head, m_Tail );
		}
		return FALSE;
	}
	m_CmAssist.IncCmSuccFrame();

	if( m_CmAssist.IsEnableLog() )
	{
		WriteNetLog("in=%u,\tout=%u,\tfr1=%d,\tfr2=%d,\trate=%.2f%%", 
					uInSize, (uOutSize+CM_HEADER_SIZE), m_CmAssist.GetCmSuccFrame(), m_CmAssist.GetCmFrame(),
					100.0*(double)(uOutSize+CM_HEADER_SIZE)/(double)uInSize );
	}

	m_CmStream.PushBack(uOutSize);
	m_CmStream.AddMsgHeader();
	if( m_Setting == 0 )
		m_CmStream.EncryptChange();

	m_Head = (m_Head+uInSize)%m_BufferLen;
	if( m_Head == m_Tail )	
		m_Head = m_Tail = 0;
	m_uSendTail = m_Head;

	if( Compress_Minilzo::GetInstance().IsEnableLog() )
	{
		Compress_Minilzo::GetInstance().AddUnCmDataSize(uInSize);
		Compress_Minilzo::GetInstance().AddCmDataSize(uOutSize+CM_HEADER_SIZE);
	}

	if( IsEnableDebugLog() )
	{
		WriteNetLog( "[Debug] Compress2CmStream head=%d, tail=%d", m_Head, m_Tail );
	}

	return TRUE;
	UNGUARD
	return FALSE;
}

uint32 SendStream::FlushClientRawStream()
{
	GUARD

	if( m_CmStream.GetSize() != 0 || IsRawStreamEmpty() )
		return 0;

	uint32 nFlushed = 0;
	uint32 nSent = 0;
	uint32 nLeft;

	if( m_BufferLen>m_MaxBufferLen || m_bFull )
	{
		Initsize() ;
		return SOCKET_ERROR-11 ;
	}

	_NET_TRY 
	{
		BOOL bTotalSend = TRUE;
		if ( m_Head < m_uSendTail ) 
		{
			nLeft = m_uSendTail - m_Head;
			if (nLeft>SOCKET2CLINET_SIZE_PER_FRAME)
			{
				nLeft = SOCKET2CLINET_SIZE_PER_FRAME;
				bTotalSend = FALSE;
			}

			while ( nLeft > 0 ) 
			{
				nSent = m_pSocket->send( &m_Buffer[m_Head] , nLeft , SENDFLAG );
				if (nSent==SOCKET_ERROR_WOULDBLOCK) return 0 ; 
				if (nSent==SOCKET_ERROR) return SOCKET_ERROR-12 ;
				if (nSent==0) return 0;
				nFlushed += nSent;
				nLeft -= nSent;
				m_Head += nSent;
			}

			if( bTotalSend == FALSE )
			{
				if( IsEnableDebugLog() )
				{
					WriteNetLog( "[Debug] FlushClientRawStream1 flush=%d, head=%d, tail=%d", nFlushed, m_Head, m_Tail );
				}
				return nFlushed;
			}
		}
		else if( m_Head > m_uSendTail )
		{
			nLeft = m_BufferLen - m_Head;
			if (nLeft>SOCKET2CLINET_SIZE_PER_FRAME)
			{
				nLeft = SOCKET2CLINET_SIZE_PER_FRAME;
				bTotalSend = FALSE;
			}

			while ( nLeft > 0 ) 
			{
				nSent = m_pSocket->send( &m_Buffer[m_Head] , nLeft , SENDFLAG );
				if (nSent==SOCKET_ERROR_WOULDBLOCK) return 0 ; 
				if (nSent==SOCKET_ERROR) return SOCKET_ERROR-13 ;
				if (nSent==0) return 0;

				nFlushed += nSent;
				nLeft -= nSent;
				m_Head += nSent;
			}

			if( bTotalSend == FALSE )
			{
				if( IsEnableDebugLog() )
				{
					WriteNetLog( "[Debug] FlushClientRawStream2 flush=%d, head=%d, tail=%d", nFlushed, m_Head, m_Tail );
				}				
				return nFlushed;
			}
			else
				m_Head = 0;
		}
	}
	_NET_CATCH
	{
		if( nSent>0 )
		{
			m_Head += nSent;
		}
	} 

	if( m_Head == m_Tail )
	{
		m_Head = m_Tail = m_uSendTail = 0;
	}

	if( IsEnableDebugLog() )
	{
		WriteNetLog( "[Debug] FlushClientRawStream3 flush=%d, head=%d, tail=%d", nFlushed, m_Head, m_Tail );
	}
	return nFlushed;
	UNGUARD
	return 0 ;
}

void SendStream::PrepareClientStream(uint32 uTail)
{
	if( m_CmStream.GetSize() == 0
		&& IsRawStreamEmpty()
		&& uTail != (uint32)-1 )
	{
		m_uSendTail = uTail;
	}
}

BOOL SendStream::IsRawStreamEmpty()
{
	if( m_uSendTail == m_Head )
		return TRUE;
	return FALSE;
}

uint32 SendStream::GetFloorTail()
{
	if( m_BufferLen>m_MaxBufferLen || m_bFull )
	{
		Initsize( ) ;
		return SOCKET_ERROR-21 ;
	}

	if( m_CmStream.GetSize() != 0 || !IsRawStreamEmpty() || Length() == 0 )
		return (uint32)-1;

	uint32 uPos = m_Head;
	uint32 uSize = 0;
	char* pBuf = m_Buffer + uPos;
	uint32 uMaxSize = CM_IN_SIZE;

	if( Length() <= uMaxSize )
		return m_Tail;

//	char szTmp[1024] = {0};
//	sprintf(szTmp, "%s%d,", szTmp, uPos);

	uint16 uLastMsgID = (uint16)-1;
	while(TRUE)
	{
		uint32 packetUINT, uMsgSize;
		uint16 uMsgID;

		if( uSize + 4 > Length() )
		{
			//WriteNetLog("Track: %d,%d,%d,%d,[%s]",m_Head,m_Tail,m_uSendTail,uPos, szTmp);
			//FILE* fp = fopen("./Log/netdump.log","wb");
			//if( fp )
			//{
			//	fwrite( m_Buffer, 1, m_BufferLen, fp );
			//	fclose(fp);
			//}
			return SOCKET_ERROR-17;
		}

		if( m_Setting == 0 )
		{
			m_pEncrptor->Decode( &uMsgID, pBuf, sizeof(uMsgID) );
			m_pEncrptor->Decode( &packetUINT, pBuf+2, sizeof(uint32) );
		}
		else
		{
			memcpy( &uMsgID, pBuf, sizeof(uMsgID) );
			memcpy( &packetUINT, pBuf+2, sizeof(uint32) );
		}

		uMsgID = uMsgID ^ m_uMsgHeadMagicNum;
		if( (uMsgID & 0x8000) != 0 )
			uMsgID &= 0x7FFF;
		packetUINT = packetUINT ^ m_uMsgHeadMagicNum;

		uMsgSize = GET_MSG_LEN(packetUINT);

		if( uMsgSize > uMaxSize ) // too large msg size or wrong stream data
		{
			//WriteNetLog("Track: %d,%d,%d,%d,[%s]",m_Head,m_Tail,m_uSendTail,uPos, szTmp);
			//FILE* fp = fopen("./Log/netdump.log","wb");
			//if( fp )
			//{
			//	fwrite( m_Buffer, 1, m_BufferLen, fp );
			//	fclose(fp);
			//}
			WriteNetLog("Net Error -15 : uMsgID=%d, uMsgSize=%d, uLastMsgID=%d", uMsgID, uMsgSize, uLastMsgID);
			return SOCKET_ERROR-15;
		}

		if( uSize + uMsgSize + 6 > m_BufferLen )	//wrong stream data
		{
			//WriteNetLog("Track: %d,%d,%d,%d,[%s]",m_Head,m_Tail,m_uSendTail,uPos, szTmp);
			//FILE* fp = fopen("./Log/netdump.log","wb");
			//if( fp )
			//{
			//	fwrite( m_Buffer, 1, m_BufferLen, fp );
			//	fclose(fp);
			//}
			WriteNetLog("Net Error -16 : uMsgID=%d, uMsgSize=%d, uLastMsgID=%d", uMsgID, uMsgSize, uLastMsgID);
			return SOCKET_ERROR-16;
		}

		if( uSize + uMsgSize + 6 > uMaxSize )
			break;

		uSize += uMsgSize + 6;
		uPos = (uPos+uMsgSize+6) % m_BufferLen;
	//	sprintf(szTmp, "%s%d,", szTmp, uPos);

		pBuf = m_Buffer + uPos;
		uLastMsgID = uMsgID;
	}

	return uPos;
}

BOOL SendStream::Resize( int32 size )
{
	GUARD

		if(IsStaticStream())
		{
			m_bFull = TRUE;
			return FALSE;
		}

		int32 orgSize = size;

		size = max( size, (int)(m_BufferLen>>1) ) ;
		uint32 newBufferLen = m_BufferLen+size ;
		uint32 len = Length( ) ;

		if( size<0 ) 
		{
			if( newBufferLen<0 || newBufferLen<len )
				return FALSE ;
		} 

		char * newBuffer = new char[ newBufferLen ] ;
		if( newBuffer==NULL )
			return FALSE ;

		if( m_Head<m_Tail ) 
		{
			memcpy( newBuffer, &m_Buffer[m_Head], m_Tail-m_Head ) ;
		} 
		else if( m_Head>m_Tail ) 
		{
			memcpy( newBuffer, &m_Buffer[m_Head] , m_BufferLen-m_Head );
			memcpy( &newBuffer[m_BufferLen-m_Head], m_Buffer, m_Tail );
		}

		delete [] m_Buffer;

		m_Buffer = newBuffer;
		m_BufferLen = newBufferLen;
		m_Head = 0;
		m_Tail = len;	

		return TRUE ;

		UNGUARD

			return FALSE ;
}

void SendStream::Clear( )
{
	GUARD

	m_Head		=	0;
	m_Tail		=	0;
	m_bFull		=	FALSE;
	m_uSendTail	=	0;
	m_CmStream.SetHead(CM_HEADER_SIZE);
	m_CmStream.SetTail(CM_HEADER_SIZE);

	// 如果之前分配的buffer过大，这里清理一下，防止connector持有的内存膨胀 [4/28/2017 yz]
	if (m_BufferLen > m_nInitBufferLen)
	{
		delete[] m_Buffer;
		m_Buffer = new char[m_nInitBufferLen];
		m_BufferLen = m_nInitBufferLen;
	}

	UNGUARD
}

void SendStream::SetEncryptMode(uint32 mode)
{
	m_Setting = mode;
}

void SendStream::EnableCompress(BOOL bEnable,TID threadID)
{
	m_CmAssist.Enable(bEnable,threadID);
}

void SendStream::EnableCompressLog(BOOL bEnable)
{
	m_CmAssist.EnableLog(bEnable);
}

BOOL SendStream::HasDataToSend()
{
	if( IsServerData() )
	{
		return (Length() != 0);
	}
	else
	{
		if( m_CmStream.GetSize() != 0 )
			return TRUE;
		if( Length() != 0 )
			return TRUE;
		return FALSE;
	}
}

void SendStream::EnableEncrpt(BOOL bEnable)
{
	m_pEncrptor->enable = (char)bEnable;
}

BOOL SendStream::IsEnableEncrpt()
{
	return (BOOL)m_pEncrptor->enable;
}
// modify by faido
void SendStream::SetFormatMode(uint32 mode)
{
	m_FormatMode = mode;
}

//===============================================================================

CmStream::CmStream()
{
	Clear();
}

CmStream::~CmStream()
{
	KSafeDelete_ARRAY(m_pBuffer);
	Clear();
}

void CmStream::Clear()
{
	m_uHead		= CM_HEADER_SIZE;
	m_uTail		= CM_HEADER_SIZE;
	m_uMaxSize	= 0;
	m_pBuffer	= NULL;
	m_pEncrptor	= NULL;
}

BOOL CmStream::AllocBuffer(uint32 uSize)
{
	m_pBuffer = new char[uSize];
	if( m_pBuffer == NULL )
		return FALSE;
	m_uMaxSize = uSize;
	return TRUE;
}

char* CmStream::GetBuffer()
{
	return m_pBuffer;
}

char* CmStream::GetHeadBuffer()
{
	return (m_pBuffer+m_uHead);
}

uint32 CmStream::GetSize()
{
	return m_uTail - m_uHead;
}

uint32 CmStream::GetMaxSize()
{
	return m_uMaxSize;
}

uint32 CmStream::GetFreeSize()
{
	return m_uMaxSize - m_uTail;
}

BOOL CmStream::PushBack(uint32 uSize)
{
	m_uTail += uSize;
	if( m_uTail >= m_uMaxSize )
	{
		m_uTail = m_uMaxSize - 1;
		return FALSE;
	}
	return TRUE;
}

BOOL CmStream::PopHead(uint32 uSize)
{
	m_uHead += uSize;
	if( m_uHead > m_uTail )
	{
		return FALSE;
	}
	return TRUE;
}

void CmStream::AddMsgHeader()
{
	uint16 uSize = (uint16)GetSize();
	uSize |= 0x8000;
	
	*((uint16*)m_pBuffer) = uSize;
	
	m_uHead = 0;
}

void CmStream::EncryptChange()
{
	m_pEncrptor->Encode( m_pBuffer, m_pBuffer, GetSize() );
}

void CmStream::ResetPos()
{
	m_uHead = m_uTail = CM_HEADER_SIZE;
}

void CmStream::SetEncrptor(Encrptor* pEn)
{
	m_pEncrptor = pEn;
}

//===============================================================================
void WriteNetLog(const char *cszFormat, ...)
{
	time_t	timeT;
	tm		curTime;

	time(&timeT);

#if defined(__WINDOWS__)
	tm	*ptm = localtime(&timeT);
	curTime = *ptm;
#else
	tm	newtm;
	tm	*ptm = localtime_r(&timeT, &newtm);
	if(ptm) 
		curTime = newtm;
#endif

	int32 year = curTime.tm_year + 1900;
	int32 month = curTime.tm_mon + 1;
	int32 day = curTime.tm_mday;
	int32 hour = curTime.tm_hour;
	int32 minute = curTime.tm_min;
	int32 second = curTime.tm_sec;

	char szFileName[260] = {0};
	sprintf( szFileName, "%s.log", "./Log/NetDebug");

	char szBuf[1024] = {0};
	char szLogBuf[1024] = {0};
	va_list argptr;
	va_start(argptr, cszFormat);
#ifdef __WINDOWS__
	_vsnprintf(szBuf, 1023, cszFormat, argptr);
#else
	vsnprintf(szBuf, 1023, cszFormat, argptr);
#endif
	va_end(argptr);

	sprintf( szLogBuf, "(%.2d:%.2d:%.2d) %s\n", hour, minute, second, szBuf );

	int nSize = (int)strlen( szLogBuf );

	FILE* file = fopen( szFileName, "a+" );
	if( file != NULL )
	{
		fwrite( szLogBuf, 1, nSize, file );
		fclose( file );
	}
}
