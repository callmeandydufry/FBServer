/*$T Common/Socket.h GC 1.140 08/23/09 03:08:39 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */




#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "KwNetBasics.h"

#if defined(__WINDOWS__)
#   define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#   include <WinSock2.h>
#   include <Ws2tcpip.h>
#elif defined(__LINUX__)
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#if !defined(IOS)
#include <sys/epoll.h>
#endif
#include <sys/select.h>
#endif
#define _ESIZE	256

#if defined(__LINUX__)
typedef int32 SOCKET;
#define INVALID_SOCKET	- 1
#define SOCKET_ERROR	- 1
#endif
static const int32			SOCKET_ERROR_WOULDBLOCK = -100;
typedef struct sockaddr		SOCKADDR;
typedef struct sockaddr_in	SOCKADDR_IN;

namespace					SOCKET_BASIC
{
SOCKET	socket_ex(int32 domain, int32 type, int32 protocol);
BOOL	bind_ex(SOCKET s, const struct sockaddr *name, uint32 namelen);
BOOL	connect_ex(SOCKET s, const struct sockaddr *name, uint32 namelen);
BOOL	listen_ex(SOCKET s, uint32 backlog);
SOCKET	accept_ex(SOCKET s, struct sockaddr *addr, uint32 *addrlen);
BOOL	getsockopt_ex(SOCKET s, int32 level, int32 optname, void *optval, uint32 *optlen);
uint32	getsockopt_ex2(SOCKET s, int32 level, int32 optname, void *optval, uint32 *optlen);
BOOL	setsockopt_ex(SOCKET s, int32 level, int32 optname, const void *optval, uint32 optlen);
uint32	send_ex(SOCKET s, const void *buf, uint32 len, uint32 flags);
uint32	sendto_ex(SOCKET s, const void *buf, int32 len, uint32 flags, const struct sockaddr *to, int32 tolen);
uint32	recv_ex(SOCKET s, void *buf, uint32 len, uint32 flags);
uint32	recvfrom_ex(SOCKET s, void *buf, int32 len, uint32 flags, struct sockaddr *from, uint32 *fromlen);
BOOL	closesocket_ex(SOCKET s);
BOOL	ioctlsocket_ex(SOCKET s, long cmd, unsigned long *argp);
BOOL	getsocketnonblocking_ex(SOCKET s);
BOOL	setsocketnonblocking_ex(SOCKET s, BOOL on);
uint32	availablesocket_ex(SOCKET s);
BOOL	shutdown_ex(SOCKET s, uint32 how);
int32	select_ex(int32 maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset, struct timeval *timeout);
};
class	Socket
{
/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
public:
	Socket();
	Socket(const char *host, uint32 port);
	virtual ~Socket();

/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
public:
	uint32	send(const void *buf, uint32 len, uint32 flags = 0);
	uint32	receive(void *buf, uint32 len, uint32 flags = 0);
	uint32	available() const;
	SOCKET	accept(struct sockaddr *addr, uint32 *addrlen);
	BOOL	bind();
	BOOL	bind(uint32 port, const char *addr);
	BOOL	listen(int32 backlog);

/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
public:
	BOOL	create();
	void	close();
	BOOL	connect();
	BOOL	connect(const char *host, uint32 port);
	BOOL	reconnect(const char *host, uint32 port);

/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
public:
	uint32	getLinger() const;
	BOOL	setLinger(uint32 lingertime);
	BOOL	isReuseAddr() const;
	BOOL	setReuseAddr(BOOL on = TRUE);
	uint32	getSockError() const;
	BOOL	isNonBlocking() const;
	BOOL	setNonBlocking(BOOL on = TRUE);
	uint32	getReceiveBufferSize() const;
	BOOL	setReceiveBufferSize(uint32 size);
	uint32	getSendBufferSize() const;
	BOOL	setSendBufferSize(uint32 size);
	uint32	getPort() const;
	IP_t	getIP() const;
	BOOL	isValid() const;
	SOCKET	getSOCKET() const;
	BOOL	isSockError() const;

/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
public:
	SOCKET		m_SocketID;
	SOCKADDR_IN m_SockAddr;
	char		m_IP[IP_SIZE];
	uint32		m_Port;
};
#endif
