#include "stdafx.h"
#include "udpsender.h"
#include <errno.h>


CUdpSender::CUdpSender()
{
	Clear();
}

CUdpSender::~CUdpSender()
{
	Clear();
}

void CUdpSender::Clear()
{
	m_SocketID = INVALID_SOCKET;
	m_Port = 0;
	memset( m_IP, 0, sizeof(m_IP) );
	memset( &m_SockAddr, 0, sizeof(m_SockAddr) );	
	memset( m_uSerialNo, 0, sizeof(m_uSerialNo) );
}

BOOL CUdpSender::Init( const char* ip, uint16 port )
{
	m_SocketID = socket(AF_INET,SOCK_DGRAM,0);
	if(m_SocketID == INVALID_SOCKET)
		return FALSE;

	memset(&m_SockAddr,0,sizeof(m_SockAddr));
	m_SockAddr.sin_family = AF_INET;
	m_SockAddr.sin_addr.s_addr = inet_addr(ip);
	m_SockAddr.sin_port = htons(port);

	strncpy( m_IP, ip, IP_SIZE );
	m_Port = port;

	return TRUE;
}

BOOL CUdpSender::Send( const char* pData, uint32 uSize, int32& err )
{
	unsigned int uSend=0, uTotalSend=0;
	int times = 0;
	while( uTotalSend < uSize )
	{
		uSend = sendto( m_SocketID, pData, uSize, 0, (SOCKADDR*)&m_SockAddr, sizeof(m_SockAddr));
		if(uSend<0)
		{
			err = errno;
			return FALSE;
		}
		
		uTotalSend += uSend;
		if( times++ >= 10 )
		{
			err = 10000;
			return FALSE;
		}
	}

	return TRUE;
}
