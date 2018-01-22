#ifndef __UDP_SENDER_H__
#define __UDP_SENDER_H__

#include "TypeDefine.h"
#include "KwNetSocket.h"

class CUdpSender
{
public:
	CUdpSender();
	~CUdpSender();

	void		Clear();

	const char*	GetIP()	{ return m_IP; }
	uint16		GetPort() { return m_Port; }

	BOOL		Init( const char* ip, uint16 port );
	BOOL		Send( const char* pData, uint32 uSize, int32& err  );

	uint32		GetSerialNo(uint32 uIndex) { return m_uSerialNo[uIndex]; }
	void		IncSerialNo(uint32 uIndex) { m_uSerialNo[uIndex]++; }

private:
	SOCKET		m_SocketID;
	SOCKADDR_IN m_SockAddr;
	char		m_IP[IP_SIZE];
	uint32		m_Port;
	uint32		m_uSerialNo[ALL_LOG_NUMBER];
};

#endif
