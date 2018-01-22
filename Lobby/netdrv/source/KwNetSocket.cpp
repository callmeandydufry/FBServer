#include "KwNetPrivate.h"
#include "KwNetSocket.h"

#ifdef __WINDOWS__
#else
#endif

#if __LINUX__
	#if !ANDROID
		// Android平台已经定义了此变量 [JiangWei]
		extern int32 errno;
	#endif
pthread_t tid;
void *ThreadEntrySample(void *derivedThread)
{

	int i = 0;
	errno = 300;	
	printf("other error=%d\r\n",errno);
	errno = 200;
	return NULL;
}

void LinuxThreadSample()
{
	pthread_create(&tid, 0, ThreadEntrySample, 0);

}

#endif





char ErrorMessage[_ESIZE] ;


int32 fcntl_ex ( int32 fd , int32 cmd , long arg ) 
{
	GUARD

#if __LINUX__
		int32 result = fcntl ( fd , cmd , arg );
	if ( result < 0 ) {
		switch ( errno ) {
			case EINTR : 
			case EINVAL : 
			case EBADF : 
			case EACCES : 
			case EAGAIN : 
			case EDEADLK : 
			case EMFILE  : 
			case ENOLCK : 
			default : 
				{
					break;
				}
		}
	}
	return result;
#elif __WINDOWS__
		return 0 ;
#endif

	UNGUARD
}

void ioctl_ex ( int32 fd , int32 request , void * argp )
{
	GUARD

#if __LINUX__
		if ( ioctl(fd,request,argp) < 0 ) {
			switch ( errno ) {
		case EBADF : 
		case ENOTTY : 
		case EINVAL : 
		default :
			{
				break;
			}
			}
		}
#elif __WINDOWS__
#endif

	UNGUARD
}


uint32 availablefile_ex ( int32 fd )
{
	GUARD

#if __LINUX__
		uint32 arg = 0;
	ioctl_ex(fd,FIONREAD,&arg);
	return arg;
#elif __WINDOWS__
		return 0;
#endif

	UNGUARD
}


#ifdef WIN32
#ifndef snprintf
#define snprintf _snprintf_s
#endif
#endif
#define NET_OK          (0)
#define NET_ERR         (-1)
static int createSocket(int domain) {
    int s, on = 1;
    if ((s = socket(domain, SOCK_STREAM, 0)) == -1) {
        snprintf(ErrorMessage, _ESIZE, "creating socket: %s", strerror(errno));
        return NET_ERR;
    }

    /* Make sure connection-intensive things like the redis benckmark
    * will be able to close/open sockets a zillion of times */
#ifdef WIN32
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&on, sizeof(on)) == -1) {
#else
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1) {
#endif
        snprintf(ErrorMessage, _ESIZE, "setsockopt SO_REUSEADDR: %s", strerror(errno));
        return NET_ERR;
    }
    return s;
    }

static int tcpGenericConnect(char *addr, int port)
{
    int s;
    struct sockaddr_in* sa_v4;
    //////////////////////////////////////////////////////////////////////////
    //判定网络类型    指定特定接口信息
    struct addrinfo *result;
    struct addrinfo *res;

    struct addrinfo _addrinfo;
    memset(&_addrinfo, 0, sizeof(_addrinfo));
    _addrinfo.ai_family = AF_UNSPEC;
    _addrinfo.ai_socktype = SOCK_STREAM;
    _addrinfo.ai_protocol = IPPROTO_TCP;

    char service_port[25] = { 0 };
    //itoa(port, service_port, 10);
    snprintf(service_port, sizeof(service_port), "%d", port);
    int error = getaddrinfo(addr, service_port, &_addrinfo, &result);
    if (0 != error)
    {
        snprintf(ErrorMessage, _ESIZE, "getaddrinfo failed");
        return NET_ERR;
    }

    for (res = result; res != NULL; res = res->ai_next)
    {
        if (AF_INET6 == res->ai_addr->sa_family)
        {
            char buf[128] = { 0 };
            sa_v4 = (struct sockaddr_in*)res->ai_addr;
            inet_ntop(AF_INET6, &((reinterpret_cast<struct sockaddr_in6*>(sa_v4))->sin6_addr), buf, 128);

            if ((s = createSocket(res->ai_family)) == NET_ERR)
                goto genericerr;

            struct sockaddr_in6 svraddr;
            memset(&svraddr, 0, sizeof(svraddr)); //注意初始化  
            svraddr.sin6_family = AF_INET6;
            svraddr.sin6_port = htons(port);
            if (inet_pton(AF_INET6, buf, &svraddr.sin6_addr) < 0)
            {
                snprintf(ErrorMessage, _ESIZE, "inet_pton failed: %s", addr);
                goto genericerr;
            }

            if (connect(s, (struct sockaddr*)&svraddr, sizeof(svraddr)) == -1) {
                if (errno == EINPROGRESS)
                    break;

                snprintf(ErrorMessage, _ESIZE, "connect: %s", strerror(errno));
#ifdef WIN32
                closesocket(s);
#else
                close(s);
#endif
                goto genericerr;
            }
            break;
        }
        else if (AF_INET == res->ai_addr->sa_family)
        {
            char buf[32] = {};
            sa_v4 = (struct sockaddr_in*)res->ai_addr;
            inet_ntop(AF_INET, &sa_v4->sin_addr, buf, 32);
            if ((s = createSocket(res->ai_family)) == NET_ERR)
                goto genericerr;

            struct sockaddr_in svraddr;
            svraddr.sin_family = AF_INET;
            svraddr.sin_port = htons(port);
#ifdef WIN32
            svraddr.sin_addr.s_addr = inet_addr(buf); if (0) {
#else
            if (inet_aton(buf, &svraddr.sin_addr) == 0) {
#endif
                struct hostent *he;
                he = gethostbyname(addr);
                if (he == NULL) {
                    snprintf(ErrorMessage, _ESIZE, "can't resolve: %s", addr);
#ifdef WIN32
                    closesocket(s);
#else
                    close(s);
#endif
                    return NET_ERR;
                }
                memcpy(&svraddr.sin_addr, he->h_addr, sizeof(struct in_addr));
            }
            if (connect(s, (struct sockaddr*)&svraddr, sizeof(svraddr)) == -1) {
                if (errno == EINPROGRESS)
                    break;

                snprintf(ErrorMessage, _ESIZE, "connect: %s", strerror(errno));
#ifdef WIN32
                closesocket(s);
#else
                close(s);
#endif
                goto genericerr;
            }
            break;
        }
    }
    freeaddrinfo(res);
    return s;
genericerr:
    freeaddrinfo(res);
    return NET_ERR;
}

SOCKET SOCKET_BASIC::socket_ex ( int32 domain , int32 type , int32 protocol ) 
{
	GUARD

	SOCKET s = ::socket(domain,type,protocol);

	if ( s == INVALID_SOCKET ) 
	{
#if __LINUX__
		switch ( errno ) 
		{
		case EPROTONOSUPPORT :
			strncpy( ErrorMessage, "EPROTONOSUPPORT", _ESIZE ) ;
			break;
		case EMFILE : 
			strncpy( ErrorMessage, "EMFILE", _ESIZE ) ;
			break;
		case ENFILE : 
			strncpy( ErrorMessage, "ENFILE", _ESIZE ) ;
			break;
		case EACCES : 
			strncpy( ErrorMessage, "EACCES", _ESIZE ) ;
			break;
		case ENOBUFS : 
			strncpy( ErrorMessage, "ENOBUFS", _ESIZE ) ;
			break;
		default : 
			{
				strncpy( ErrorMessage, "UNKNOWN", _ESIZE ) ;
				break;
			}
		}
#elif __WINDOWS__
		int32 iErr = WSAGetLastError() ;
		switch ( iErr ) 
		{
		case WSANOTINITIALISED : 
			strncpy( ErrorMessage, "WSANOTINITIALISED", _ESIZE ) ;
			break ;
		case WSAENETDOWN : 
			strncpy( ErrorMessage, "WSAENETDOWN", _ESIZE ) ;
			break ;
		case WSAEAFNOSUPPORT : 
			strncpy( ErrorMessage, "WSAEAFNOSUPPORT", _ESIZE ) ;
			break ;
		case WSAEINPROGRESS : 
			strncpy( ErrorMessage, "WSAEINPROGRESS", _ESIZE ) ;
			break ;
		case WSAEMFILE : 
			strncpy( ErrorMessage, "WSAEMFILE", _ESIZE ) ;
			break ;
		case WSAENOBUFS : 
			strncpy( ErrorMessage, "WSAENOBUFS", _ESIZE ) ;
			break ;
		case WSAEPROTONOSUPPORT : 
			strncpy( ErrorMessage, "WSAEPROTONOSUPPORT", _ESIZE ) ;
			break ;
		case WSAEPROTOTYPE : 
			strncpy( ErrorMessage, "WSAEPROTOTYPE", _ESIZE ) ;
			break ;
		case WSAESOCKTNOSUPPORT : 
			strncpy( ErrorMessage, "WSAESOCKTNOSUPPORT", _ESIZE ) ;
			break ;
		default : 
			{
			strncpy( ErrorMessage, "UNKNOWN", _ESIZE ) ;
			break ;
			};
		};
#endif
	}

	return s;
	
	UNGUARD

	return INVALID_SOCKET ;
}

BOOL SOCKET_BASIC::bind_ex ( SOCKET s , const struct sockaddr * addr , uint32 addrlen ) 
{
	GUARD

	if ( ::bind ( s , addr , addrlen ) == SOCKET_ERROR ) 
	{
#if __LINUX__
		switch ( errno ) 
		{
		case EADDRINUSE :
			strncpy( ErrorMessage, "EADDRINUSE", _ESIZE ) ;
			break;
		case EINVAL : 
			strncpy( ErrorMessage, "EINVAL", _ESIZE ) ;
			break;
		case EACCES : 
			strncpy( ErrorMessage, "EACCES", _ESIZE ) ;
			break;
		case ENOTSOCK : 
			strncpy( ErrorMessage, "ENOTSOCK", _ESIZE ) ;
			break;
		case EBADF : 
			strncpy( ErrorMessage, "EBADF", _ESIZE ) ;
			break;
		case EROFS : 
			strncpy( ErrorMessage, "EROFS", _ESIZE ) ;
			break;
		case EFAULT : 
			strncpy( ErrorMessage, "EFAULT", _ESIZE ) ;
			break;
		case ENAMETOOLONG : 
			strncpy( ErrorMessage, "ENAMETOOLONG", _ESIZE ) ;
			break;
		case ENOENT : 
			strncpy( ErrorMessage, "ENOENT", _ESIZE ) ;
			break;
		case ENOMEM : 
			strncpy( ErrorMessage, "ENOMEM", _ESIZE ) ;
			break;
		case ENOTDIR : 
			strncpy( ErrorMessage, "ENOTDIR", _ESIZE ) ;
			break;
		case ELOOP : 
			strncpy( ErrorMessage, "ELOOP", _ESIZE ) ;
			break;
		default :
			{
				strncpy( ErrorMessage, "UNKNOWN", _ESIZE ) ;
				break;
			}
		}
#elif __WINDOWS__
		int32 iErr = WSAGetLastError() ;
		switch ( iErr ) 
		{
		case WSANOTINITIALISED : 
			strncpy( ErrorMessage, "WSAESOCKTNOSUPPORT", _ESIZE ) ;
			break ;
		case WSAENETDOWN : 
			strncpy( ErrorMessage, "WSAENETDOWN", _ESIZE ) ;
			break ;
		case WSAEADDRINUSE : 
			strncpy( ErrorMessage, "WSAEADDRINUSE", _ESIZE ) ;
			break ;
		case WSAEADDRNOTAVAIL : 
			strncpy( ErrorMessage, "WSAEADDRNOTAVAIL", _ESIZE ) ;
			break ;
		case WSAEFAULT : 
			strncpy( ErrorMessage, "WSAEFAULT", _ESIZE ) ;
			break ;
		case WSAEINPROGRESS : 
			strncpy( ErrorMessage, "WSAEINPROGRESS", _ESIZE ) ;
			break ;
		case WSAEINVAL : 
			strncpy( ErrorMessage, "WSAEINVAL", _ESIZE ) ;
			break ;
		case WSAENOBUFS : 
			strncpy( ErrorMessage, "WSAENOBUFS", _ESIZE ) ;
			break ;
		case WSAENOTSOCK : 
			strncpy( ErrorMessage, "WSAENOTSOCK", _ESIZE ) ;
			break ;
		default :
			{
			strncpy( ErrorMessage, "UNKNOWN", _ESIZE ) ;
			break ;
			};
		};
#endif

		return FALSE ;
	}
	
	return TRUE ;

	UNGUARD

	return FALSE ;
}
BOOL SOCKET_BASIC::connect_ex ( SOCKET s , const struct sockaddr * addr , uint32 addrlen )
{
	GUARD

	if ( ::connect(s,addr,addrlen) == SOCKET_ERROR ) 
	{
#if __LINUX__
		switch ( errno ) {
		case EALREADY : 
			strncpy( ErrorMessage, "EALREADY", _ESIZE ) ;
			break;
		case EINPROGRESS : 
			strncpy( ErrorMessage, "EINPROGRESS", _ESIZE ) ;
			break;
		case ECONNREFUSED : 
			strncpy( ErrorMessage, "ECONNREFUSED", _ESIZE ) ;
			break;
		case EISCONN : 
			strncpy( ErrorMessage, "EISCONN", _ESIZE ) ;
			break;
		case ETIMEDOUT :
			strncpy( ErrorMessage, "ETIMEDOUT", _ESIZE ) ;
			break;
		case ENETUNREACH :
			strncpy( ErrorMessage, "ENETUNREACH", _ESIZE ) ;
			break;
		case EADDRINUSE : 
			strncpy( ErrorMessage, "EADDRINUSE", _ESIZE ) ;
			break;
		case EBADF : 
			strncpy( ErrorMessage, "EBADF", _ESIZE ) ;
			break;
		case EFAULT : 
			strncpy( ErrorMessage, "EFAULT", _ESIZE ) ;
			break;
		case ENOTSOCK :
			strncpy( ErrorMessage, "ENOTSOCK", _ESIZE ) ;
			break;
		default :
			{
				strncpy( ErrorMessage, "UNKNOWN", _ESIZE ) ;
				break;
			}
		}
#elif __WINDOWS__
		int32 iErr = WSAGetLastError() ;
		switch ( iErr ) 
		{
		case WSANOTINITIALISED : 
			strncpy( ErrorMessage, "WSANOTINITIALISED", _ESIZE ) ;
			break ;
		case WSAENETDOWN : 
			strncpy( ErrorMessage, "WSAENETDOWN", _ESIZE ) ;
			break ;
		case WSAEADDRINUSE : 
			strncpy( ErrorMessage, "WSAEADDRINUSE", _ESIZE ) ;
			break ;
		case WSAEINTR : 
			strncpy( ErrorMessage, "WSAEINTR", _ESIZE ) ;
			break ;
		case WSAEINPROGRESS : 
			strncpy( ErrorMessage, "WSAEINPROGRESS", _ESIZE ) ;
			break ;
		case WSAEALREADY : 
			strncpy( ErrorMessage, "WSAEALREADY", _ESIZE ) ;
			break ;
		case WSAEADDRNOTAVAIL : 
			strncpy( ErrorMessage, "WSAEADDRNOTAVAIL", _ESIZE ) ;
			break ;
		case WSAEAFNOSUPPORT : 
			strncpy( ErrorMessage, "WSAEAFNOSUPPORT", _ESIZE ) ;
			break ;
		case WSAECONNREFUSED : 
			strncpy( ErrorMessage, "WSAECONNREFUSED", _ESIZE ) ;
			break ;
		case WSAEFAULT : 
			strncpy( ErrorMessage, "WSAEFAULT", _ESIZE ) ;
			break ;
		case WSAEINVAL : 
			strncpy( ErrorMessage, "WSAEINVAL", _ESIZE ) ;
			break ;
		case WSAEISCONN : 
			strncpy( ErrorMessage, "WSAEISCONN", _ESIZE ) ;
			break ;
		case WSAENETUNREACH : 
			strncpy( ErrorMessage, "WSAENETUNREACH", _ESIZE ) ;
			break ;
		case WSAENOBUFS : 
			strncpy( ErrorMessage, "WSAENOBUFS", _ESIZE ) ;
			break ;
		case WSAENOTSOCK : 
			strncpy( ErrorMessage, "WSAENOTSOCK", _ESIZE ) ;
			break ;
		case WSAETIMEDOUT : 
			strncpy( ErrorMessage, "WSAETIMEDOUT", _ESIZE ) ;
			break ;
		case WSAEWOULDBLOCK  : 
			strncpy( ErrorMessage, "WSAEWOULDBLOCK", _ESIZE ) ;
			break ;
		default :
			{
			strncpy( ErrorMessage, "UNKNOWN", _ESIZE ) ;
			break ;
			};
		};
#endif
		return FALSE ;
	}

	return TRUE ;
	
	UNGUARD

	return FALSE ;
}

BOOL SOCKET_BASIC::listen_ex ( SOCKET s , uint32 backlog ) 
{
	GUARD

	if ( ::listen( s , backlog ) == SOCKET_ERROR ) 
	{
#if __LINUX__
		switch ( errno ) 
		{
		case EBADF : 
			strncpy( ErrorMessage, "EBADF", _ESIZE ) ;
			break;
		case ENOTSOCK :
			strncpy( ErrorMessage, "ENOTSOCK", _ESIZE ) ;
			break;
		case EOPNOTSUPP :
			strncpy( ErrorMessage, "EOPNOTSUPP", _ESIZE ) ;
			break;
		default :
			{
				strncpy( ErrorMessage, "UNKNOWN", _ESIZE ) ;
				break;
			}
		}
#elif __WINDOWS__
		int32 iErr = WSAGetLastError() ;
		switch ( iErr ) 
		{
		case WSANOTINITIALISED : 
			strncpy( ErrorMessage, "WSANOTINITIALISED", _ESIZE ) ;
			break ;
		case WSAENETDOWN : 
			strncpy( ErrorMessage, "WSAENETDOWN", _ESIZE ) ;
			break ;
		case WSAEADDRINUSE : 
			strncpy( ErrorMessage, "WSAEADDRINUSE", _ESIZE ) ;
			break ;
		case WSAEINPROGRESS : 
			strncpy( ErrorMessage, "WSAEINPROGRESS", _ESIZE ) ;
			break ;
		case WSAEINVAL : 
			strncpy( ErrorMessage, "WSAEINVAL", _ESIZE ) ;
			break ;
		case WSAEISCONN : 
			strncpy( ErrorMessage, "WSAEISCONN", _ESIZE ) ;
			break ;
		case WSAEMFILE : 
			strncpy( ErrorMessage, "WSAEMFILE", _ESIZE ) ;
			break ;
		case WSAENOBUFS : 
			strncpy( ErrorMessage, "WSAENOBUFS", _ESIZE ) ;
			break ;
		case WSAENOTSOCK : 
			strncpy( ErrorMessage, "WSAENOTSOCK", _ESIZE ) ;
			break ;
		case WSAEOPNOTSUPP : 
			strncpy( ErrorMessage, "WSAEOPNOTSUPP", _ESIZE ) ;
			break ;
		default :
			{
			strncpy( ErrorMessage, "UNKNOWN", _ESIZE ) ;
			break ;
			};
		};
#endif

		return FALSE ;
	}

	return TRUE ;
	
	UNGUARD

	return FALSE ;
}

SOCKET SOCKET_BASIC::accept_ex ( SOCKET s , struct sockaddr * addr , uint32 * addrlen )
{
	GUARD

#if __LINUX__
	SOCKET client = ::accept( s , addr , addrlen );
#elif __WINDOWS__
	SOCKET client = ::accept( s , addr , (int*)addrlen );
#endif
	
	if ( client == INVALID_SOCKET ) 
	{
#if __LINUX__
		switch ( errno ) 
		{

		case EWOULDBLOCK : 
			strncpy( ErrorMessage, "EWOULDBLOCK", _ESIZE ) ;
			break;
		case ECONNRESET :
			strncpy( ErrorMessage, "ECONNRESET", _ESIZE ) ;
			break;
		case ECONNABORTED :
			strncpy( ErrorMessage, "ECONNABORTED", _ESIZE ) ;
			break;	
		case EPROTO :
			strncpy( ErrorMessage, "EPROTO", _ESIZE ) ;
			break;	
		case EINTR :
			strncpy( ErrorMessage, "EINTR", _ESIZE ) ;
			break;	
		case EBADF :
			strncpy( ErrorMessage, "EBADF", _ESIZE ) ;
			break;	
		case ENOTSOCK : 
			strncpy( ErrorMessage, "ENOTSOCK", _ESIZE ) ;
			break;	
		case EOPNOTSUPP : 
			strncpy( ErrorMessage, "EOPNOTSUPP", _ESIZE ) ;
			break;	
		case EFAULT : 
			strncpy( ErrorMessage, "EFAULT", _ESIZE ) ;
			break;	
		default :
			{
				strncpy( ErrorMessage, "UNKNOWN", _ESIZE ) ;
				break;	
			}
		}
#elif __WINDOWS__
		int32 iErr = WSAGetLastError() ;
		switch ( iErr ) 
		{
		case WSANOTINITIALISED : 
			strncpy( ErrorMessage, "WSANOTINITIALISED", _ESIZE ) ;
			break ;
		case WSAENETDOWN : 
			strncpy( ErrorMessage, "WSAENETDOWN", _ESIZE ) ;
			break ;
		case WSAEFAULT : 
			strncpy( ErrorMessage, "WSAEFAULT", _ESIZE ) ;
			break ;
		case WSAEINTR : 
			strncpy( ErrorMessage, "WSAEINTR", _ESIZE ) ;
			break ;
		case WSAEINPROGRESS : 
			strncpy( ErrorMessage, "WSAEINPROGRESS", _ESIZE ) ;
			break ;
		case WSAEINVAL : 
			strncpy( ErrorMessage, "WSAEINVAL", _ESIZE ) ;
			break ;
		case WSAEMFILE : 
			strncpy( ErrorMessage, "WSAEMFILE", _ESIZE ) ;
			break ;
		case WSAENOBUFS : 
			strncpy( ErrorMessage, "WSAENOBUFS", _ESIZE ) ;
			break ;
		case WSAENOTSOCK : 
			strncpy( ErrorMessage, "WSAENOTSOCK", _ESIZE ) ;
			break ;
		case WSAEOPNOTSUPP : 
			strncpy( ErrorMessage, "WSAEOPNOTSUPP", _ESIZE ) ;
			break ;
		case WSAEWOULDBLOCK : 
			strncpy( ErrorMessage, "WSAEWOULDBLOCK", _ESIZE ) ;
			break ;
		default :
			{
			strncpy( ErrorMessage, "UNKNOWN", _ESIZE ) ;
			break ;
			};
		};
#endif
	} else {
	}

	return client;

	UNGUARD

	return INVALID_SOCKET ;
}
BOOL SOCKET_BASIC::getsockopt_ex ( SOCKET s , int32 level , int32 optname , void * optval , uint32 * optlen )
{
	GUARD

#if __LINUX__
	if ( ::getsockopt( s , level , optname , optval , optlen ) == SOCKET_ERROR ) 
	{
		switch ( errno ) 
		{
		case EBADF : 
		case ENOTSOCK : 
		case ENOPROTOOPT : 
		case EFAULT : 
		default :
			{
				break;
			}
		}
		return FALSE ;
	}
#elif __WINDOWS__
	if ( ::getsockopt( s , level , optname , (char*)optval , (int*)optlen ) == SOCKET_ERROR ) 
	{
		int32 iErr = WSAGetLastError() ;
		switch ( iErr ) 
		{
		case WSANOTINITIALISED : 
			strncpy( ErrorMessage, "WSANOTINITIALISED", _ESIZE ) ;
			break ;
		case WSAENETDOWN : 
			strncpy( ErrorMessage, "WSAENETDOWN", _ESIZE ) ;
			break ;
		case WSAEFAULT : 
			strncpy( ErrorMessage, "WSAEFAULT", _ESIZE ) ;
			break ;
		case WSAEINPROGRESS : 
			strncpy( ErrorMessage, "WSAEINPROGRESS", _ESIZE ) ;
			break ;
		case WSAEINVAL : 
			strncpy( ErrorMessage, "WSAEINVAL", _ESIZE ) ;
			break ;
		case WSAENOPROTOOPT : 
			strncpy( ErrorMessage, "WSAENOPROTOOPT", _ESIZE ) ;
			break ;
		case WSAENOTSOCK : 
			strncpy( ErrorMessage, "WSAENOTSOCK", _ESIZE ) ;
			break ;
		default : 
			{
			strncpy( ErrorMessage, "UNKNOWN", _ESIZE ) ;
			break ;
			};
		};
		return FALSE ;
	}
#endif

	return TRUE ;
	
	UNGUARD

	return FALSE ;
}

uint32 SOCKET_BASIC::getsockopt_ex2 ( SOCKET s , int32 level , int32 optname , void * optval , uint32 * optlen )
{
	GUARD

#if __LINUX__
	if ( ::getsockopt( s , level , optname , optval , optlen ) == SOCKET_ERROR ) 
	{
		switch ( errno ) 
		{
		case EBADF : 
			return 1;
		case ENOTSOCK : 
			return 2;
		case ENOPROTOOPT : 
			return 3;
		case EFAULT : 
			return 4;
		default :
			return 5;
		}
	}
	return 0;

#elif __WINDOWS__
	if ( getsockopt( s , level , optname , (char*)optval , (int*)optlen ) == SOCKET_ERROR ) 
	{
		int32 iErr = WSAGetLastError() ;
		switch ( iErr ) 
		{
		case WSANOTINITIALISED:
			strncpy( ErrorMessage, "WSANOTINITIALISED", _ESIZE ) ;
			break ;
		case WSAENETDOWN:
			strncpy( ErrorMessage, "WSAENETDOWN", _ESIZE ) ;
			break ;
		case WSAEFAULT:
			strncpy( ErrorMessage, "WSAEFAULT", _ESIZE ) ;
			break ;
		case WSAEINPROGRESS:
			strncpy( ErrorMessage, "WSAEINPROGRESS", _ESIZE ) ;
			break ;
		case WSAEINVAL:
			strncpy( ErrorMessage, "WSAEINVAL", _ESIZE ) ;
			break ;
		case WSAENOPROTOOPT:
			strncpy( ErrorMessage, "WSAENOPROTOOPT", _ESIZE ) ;
			break ;
		case WSAENOTSOCK:
			strncpy( ErrorMessage, "WSAENOTSOCK", _ESIZE ) ;
			break ;
		default : 
			{
			strncpy( ErrorMessage, "UNKNOWN", _ESIZE ) ;
			break ;
			};
		}
	}
#endif

	return 0;

	UNGUARD

	return 5;
}

BOOL SOCKET_BASIC::setsockopt_ex ( SOCKET s , int32 level , int32 optname , const void * optval , uint32 optlen )
{
	GUARD

#if __LINUX__
	if ( ::setsockopt( s , level , optname , optval , optlen ) == SOCKET_ERROR ) 
	{
		switch ( errno ) 
		{
			case EBADF : 
			case ENOTSOCK : 
			case ENOPROTOOPT : 
			case EFAULT : 
			default :
				{
					break;
				}
		}
		return FALSE ;
	}
#elif __WINDOWS__
	if ( ::setsockopt( s , level , optname , (char*)optval , optlen ) == SOCKET_ERROR ) 
	{
		int32 iErr = WSAGetLastError() ;
		switch ( iErr ) 
		{
		case WSANOTINITIALISED : 
			strncpy( ErrorMessage, "WSANOTINITIALISED", _ESIZE ) ;
			break ;
		case WSAENETDOWN : 
			strncpy( ErrorMessage, "WSAENETDOWN", _ESIZE ) ;
			break ;
		case WSAEFAULT : 
			strncpy( ErrorMessage, "WSAEFAULT", _ESIZE ) ;
			break ;
		case WSAEINPROGRESS : 
			strncpy( ErrorMessage, "WSAEINPROGRESS", _ESIZE ) ;
			break ;
		case WSAEINVAL : 
			strncpy( ErrorMessage, "WSAEINVAL", _ESIZE ) ;
			break ;
		case WSAENETRESET : 
			strncpy( ErrorMessage, "WSAENETRESET", _ESIZE ) ;
			break ;
		case WSAENOPROTOOPT : 
			strncpy( ErrorMessage, "WSAENOPROTOOPT", _ESIZE ) ;
			break ;
		case WSAENOTCONN : 
			strncpy( ErrorMessage, "WSAENOTCONN", _ESIZE ) ;
			break ;
		case WSAENOTSOCK : 
			strncpy( ErrorMessage, "WSAENOTSOCK", _ESIZE ) ;
			break ;
		default :
			{
			strncpy( ErrorMessage, "UNKNOWN", _ESIZE ) ;
			break ;
			};
		};
		return FALSE ;
	}
#endif

	return TRUE ;
	
	UNGUARD

	return FALSE ;
}

uint32 SOCKET_BASIC::send_ex ( SOCKET s , const void * buf , uint32 len , uint32 flags )
{
	GUARD

	int32 nSent;

	_NET_TRY
	{

#if __LINUX__
	nSent = ::send(s,buf,len,flags);
#elif __WINDOWS__
	nSent = ::send(s,(const char *)buf,len,flags);
#endif

	if ( nSent == SOCKET_ERROR ) 
	{
#if __LINUX__
		switch ( errno ) 
		{

		case EWOULDBLOCK : 
			return SOCKET_ERROR_WOULDBLOCK;
		case ECONNRESET :
			strncpy( ErrorMessage, "ECONNRESET", _ESIZE ) ;
			break ;
		case EPIPE :
			strncpy( ErrorMessage, "EPIPE", _ESIZE ) ;
			break ;
		case EBADF : 
			strncpy( ErrorMessage, "EBADF", _ESIZE ) ;
			break ;
		case ENOTSOCK : 
			strncpy( ErrorMessage, "ENOTSOCK", _ESIZE ) ;
			break ;
		case EFAULT : 
			strncpy( ErrorMessage, "EFAULT", _ESIZE ) ;
			break ;
		case EMSGSIZE : 
			strncpy( ErrorMessage, "EMSGSIZE", _ESIZE ) ;
			break ;
		case ENOBUFS : 
			strncpy( ErrorMessage, "ENOBUFS", _ESIZE ) ;
			break ;
		default : 
			{
				strncpy( ErrorMessage, "UNKNOWN", _ESIZE ) ;
				break ;
				break;
			}
		}
#elif __WINDOWS__
		int32 iErr = WSAGetLastError() ;
		switch ( iErr ) 
		{
		case WSANOTINITIALISED : 
			strncpy( ErrorMessage, "WSANOTINITIALISED", _ESIZE ) ;
			break ;
		case WSAENETDOWN : 
			strncpy( ErrorMessage, "WSAENETDOWN", _ESIZE ) ;
			break ;
		case WSAEACCES : 
			strncpy( ErrorMessage, "WSAEACCES", _ESIZE ) ;
			break ;
		case WSAEINTR : 
			strncpy( ErrorMessage, "WSAEINTR", _ESIZE ) ;
			break ;
		case WSAEINPROGRESS : 
			strncpy( ErrorMessage, "WSAEINPROGRESS", _ESIZE ) ;
			break ;
		case WSAEFAULT : 
			strncpy( ErrorMessage, "WSAEFAULT", _ESIZE ) ;
			break ;
		case WSAENETRESET : 
			strncpy( ErrorMessage, "WSAENETRESET", _ESIZE ) ;
			break ;
		case WSAENOBUFS : 
			strncpy( ErrorMessage, "WSAENOBUFS", _ESIZE ) ;
			break ;
		case WSAENOTCONN : 
			strncpy( ErrorMessage, "WSAENOTCONN", _ESIZE ) ;
			break ;
		case WSAENOTSOCK : 
			strncpy( ErrorMessage, "WSAENOTSOCK", _ESIZE ) ;
			break ;
		case WSAEOPNOTSUPP : 
			strncpy( ErrorMessage, "WSAEOPNOTSUPP", _ESIZE ) ;
			break ;
		case WSAESHUTDOWN : 
			strncpy( ErrorMessage, "WSAESHUTDOWN", _ESIZE ) ;
			break ;
		case WSAEWOULDBLOCK : 
			return SOCKET_ERROR_WOULDBLOCK ;
			break ;
		case WSAEMSGSIZE : 
			strncpy( ErrorMessage, "WSAEMSGSIZE", _ESIZE ) ;
			break ;
		case WSAEHOSTUNREACH : 
			strncpy( ErrorMessage, "WSAEHOSTUNREACH", _ESIZE ) ;
			break ;
		case WSAEINVAL : 
			strncpy( ErrorMessage, "WSAEINVAL", _ESIZE ) ;
			break ;
		case WSAECONNABORTED : 
			strncpy( ErrorMessage, "WSAECONNABORTED", _ESIZE ) ;
			break ;
		case WSAECONNRESET : 
			strncpy( ErrorMessage, "WSAECONNRESET", _ESIZE ) ;
			break ;
		case WSAETIMEDOUT : 
			strncpy( ErrorMessage, "WSAETIMEDOUT", _ESIZE ) ;
			break ;
		default :
			{
			strncpy( ErrorMessage, "UNKNOWN", _ESIZE ) ;
			break ;
			};
		};
#endif
	} 
	else if ( nSent == 0 )
	{
	}

	}
	_NET_CATCH
	{
	}

	return nSent;
	
	UNGUARD

	return 0 ;
}

uint32 SOCKET_BASIC::sendto_ex ( SOCKET s , const void * buf , int32 len , uint32 flags , const struct sockaddr * to , int32 tolen )
{
	GUARD

#if __LINUX__
	int32 nSent = ::sendto(s,buf,len,flags,to,tolen);
#elif __WINDOWS__
	int32 nSent = ::sendto(s,(const char *)buf,len,flags,to,tolen);
#endif

	if ( nSent == SOCKET_ERROR ) 
	{
#if __LINUX__
		switch ( errno ) 
		{

		case EWOULDBLOCK : 
			return 0;

		case ECONNRESET :
		case EPIPE :

		case EBADF : 
		case ENOTSOCK : 
		case EFAULT : 
		case EMSGSIZE : 
		case ENOBUFS : 

		default : 
			{
				break;
			}
		}	
#elif __WINDOWS__
#endif
	}

	return nSent;

	UNGUARD

	return 0 ;
}


uint32 SOCKET_BASIC::recv_ex ( SOCKET s , void * buf , uint32 len , uint32 flags )
{
	GUARD

#if __LINUX__
	int32 nrecv = ::recv(s,buf,len,flags);
#elif __WINDOWS__
	int32 nrecv = ::recv(s,(char*)buf,len,flags);
#endif

	if ( nrecv == SOCKET_ERROR ) 
	{
#if __LINUX__
		switch ( errno ) 
		{

		case EWOULDBLOCK : 
			return SOCKET_ERROR_WOULDBLOCK;

		case ECONNRESET :
			strncpy( ErrorMessage, "ECONNRESET", _ESIZE ) ;
			break ;
		case EPIPE :
			strncpy( ErrorMessage, "EPIPE", _ESIZE ) ;
			break ;
		case EBADF : 
			strncpy( ErrorMessage, "EBADF", _ESIZE ) ;
			break ;
		case ENOTCONN : 
			strncpy( ErrorMessage, "ENOTCONN", _ESIZE ) ;
			break ;
		case ENOTSOCK : 
			strncpy( ErrorMessage, "ENOTSOCK", _ESIZE ) ;
			break ;
		case EINTR : 
			strncpy( ErrorMessage, "EINTR", _ESIZE ) ;
			break ;
		case EFAULT : 
			strncpy( ErrorMessage, "EFAULT", _ESIZE ) ;
			break ;
		default : 
			{
				strncpy( ErrorMessage, "UNKNOWN", _ESIZE ) ;
				break ;
				break;
			}
		}

#elif __WINDOWS__
		int32 iErr = WSAGetLastError() ;
		switch ( iErr ) 
		{
		case WSANOTINITIALISED : 
			strncpy( ErrorMessage, "WSANOTINITIALISED", _ESIZE ) ;
			break ;
		case WSAENETDOWN : 
			strncpy( ErrorMessage, "WSAENETDOWN", _ESIZE ) ;
			break ;
		case WSAEFAULT : 
			strncpy( ErrorMessage, "WSAEFAULT", _ESIZE ) ;
			break ;
		case WSAENOTCONN : 
			strncpy( ErrorMessage, "WSAENOTCONN", _ESIZE ) ;
			break ;
		case WSAEINTR : 
			strncpy( ErrorMessage, "WSAEINTR", _ESIZE ) ;
			break ;
		case WSAEINPROGRESS : 
			strncpy( ErrorMessage, "WSAEINPROGRESS", _ESIZE ) ;
			break ;
		case WSAENETRESET : 
			strncpy( ErrorMessage, "WSAENETRESET", _ESIZE ) ;
			break ;
		case WSAENOTSOCK : 
			strncpy( ErrorMessage, "WSAENOTSOCK", _ESIZE ) ;
			break ;
		case WSAEOPNOTSUPP : 
			strncpy( ErrorMessage, "WSAEOPNOTSUPP", _ESIZE ) ;
			break ;
		case WSAESHUTDOWN : 
			strncpy( ErrorMessage, "WSAESHUTDOWN", _ESIZE ) ;
			break ;
		case WSAEWOULDBLOCK : 
			return SOCKET_ERROR_WOULDBLOCK ;
			break ;
		case WSAEMSGSIZE : 
			strncpy( ErrorMessage, "WSAEMSGSIZE", _ESIZE ) ;
			break ;
		case WSAEINVAL : 
			strncpy( ErrorMessage, "WSAEINVAL", _ESIZE ) ;
			break ;
		case WSAECONNABORTED : 
			strncpy( ErrorMessage, "WSAECONNABORTED", _ESIZE ) ;
			break ;
		case WSAETIMEDOUT : 
			strncpy( ErrorMessage, "WSAETIMEDOUT", _ESIZE ) ;
			break ;
		case WSAECONNRESET : 
			strncpy( ErrorMessage, "WSAECONNRESET", _ESIZE ) ;
			break ;
		default :
			{
			strncpy( ErrorMessage, "UNKNOWN", _ESIZE ) ;
			break ;
			};
		};
#endif
	} 
	else if ( nrecv == 0 )
	{
	}

	return nrecv;
	
	UNGUARD

	return 0 ;
}

uint32 SOCKET_BASIC::recvfrom_ex ( SOCKET s , void * buf , int32 len , uint32 flags , struct sockaddr * from , uint32 * fromlen )
{
	GUARD

#if __LINUX__
	int32 nRec = ::recvfrom(s,buf,len,flags,from,fromlen);

#elif __WINDOWS__
	int32 nRec = ::recvfrom(s,(char*)buf,len,flags,from,(int*)fromlen);
#endif

	if ( nRec == SOCKET_ERROR ) 
	{
#if __LINUX__
		switch ( errno ) 
		{

		case EWOULDBLOCK : 
			return SOCKET_ERROR_WOULDBLOCK;

		case ECONNRESET :
		case EPIPE :

		case EBADF : 
		case ENOTCONN : 
		case ENOTSOCK : 
		case EINTR : 
		case EFAULT : 

		default : 
			{
				break;
			}
		}
#elif __WINDOWS__
#endif
	}

	return nRec;

	UNGUARD

	return 0 ;
}
BOOL SOCKET_BASIC::closesocket_ex ( SOCKET s )
{
	GUARD

#if __LINUX__
	 ::close(s);
#elif __WINDOWS__
	if ( ::closesocket(s) == SOCKET_ERROR ) 
	{
		int32 iErr = WSAGetLastError() ;
		switch ( iErr ) 
		{
		case WSANOTINITIALISED : 
			strncpy( ErrorMessage, "WSANOTINITIALISED", _ESIZE ) ;
			break ;
		case WSAENETDOWN : 
			strncpy( ErrorMessage, "WSAENETDOWN", _ESIZE ) ;
			break ;
		case WSAENOTSOCK : 
			strncpy( ErrorMessage, "WSAENOTSOCK", _ESIZE ) ;
			break ;
		case WSAEINPROGRESS : 
			strncpy( ErrorMessage, "WSAEINPROGRESS", _ESIZE ) ;
			break ;
		case WSAEINTR : 
			strncpy( ErrorMessage, "WSAEINTR", _ESIZE ) ;
			break ;
		case WSAEWOULDBLOCK : 
			strncpy( ErrorMessage, "WSAEWOULDBLOCK", _ESIZE ) ;
			break ;
		default : 
			{
			strncpy( ErrorMessage, "UNKNOWN", _ESIZE ) ;
			break ;
			};
		};
		return FALSE ;
	}
#endif

	return TRUE ;
	
	UNGUARD

	return FALSE ;
}

BOOL SOCKET_BASIC::ioctlsocket_ex ( SOCKET s , long cmd , unsigned long * argp )
{
	GUARD

#if __LINUX__
#elif __WINDOWS__
	if ( ::ioctlsocket(s,cmd,argp) == SOCKET_ERROR ) 
	{
		int32 iErr = WSAGetLastError() ;
		switch ( iErr ) 
		{
		case WSANOTINITIALISED : 
			strncpy( ErrorMessage, "WSANOTINITIALISED", _ESIZE ) ;
			break ;
		case WSAENETDOWN : 
			strncpy( ErrorMessage, "WSAENETDOWN", _ESIZE ) ;
			break ;
		case WSAEINPROGRESS : 
			strncpy( ErrorMessage, "WSAEINPROGRESS", _ESIZE ) ;
			break ;
		case WSAENOTSOCK : 
			strncpy( ErrorMessage, "WSAENOTSOCK", _ESIZE ) ;
			break ;
		case WSAEFAULT : 
			strncpy( ErrorMessage, "WSAEFAULT", _ESIZE ) ;
			break ;
		default :
			{
			strncpy( ErrorMessage, "UNKNOWN", _ESIZE ) ;
			break ;
			};
		};

		return FALSE ;
	}
#endif

	return TRUE ;
	
	UNGUARD

	return FALSE ;
}

BOOL SOCKET_BASIC::getsocketnonblocking_ex ( SOCKET s )
{
	GUARD

#if __LINUX__
	int32 flags = fcntl_ex( s , F_GETFL , 0 );
	return flags | O_NONBLOCK;
#elif __WINDOWS__
	return FALSE ;
#endif
	
	UNGUARD

	return FALSE ;
}

BOOL SOCKET_BASIC::setsocketnonblocking_ex ( SOCKET s , BOOL on )
{
	GUARD

#if __LINUX__
	int32 flags = fcntl_ex( s , F_GETFL , 0 );
	if ( on )
		flags |= O_NONBLOCK;
	else
		flags &= ~O_NONBLOCK;
	fcntl_ex( s , F_SETFL , flags );
	return TRUE;
#elif __WINDOWS__

	ULONG argp = ( on == TRUE ) ? 1 : 0;
	return ioctlsocket_ex( s,FIONBIO,&argp);

#endif
	
	UNGUARD

	return FALSE ;
}

uint32 SOCKET_BASIC::availablesocket_ex ( SOCKET s )
{
	GUARD

#if __LINUX__
	return availablefile_ex(s);
#elif __WINDOWS__
	ULONG argp = 0;
	ioctlsocket_ex(s,FIONREAD,&argp);
	return argp;
#endif

	
	UNGUARD
	return 0 ;
}

BOOL SOCKET_BASIC::shutdown_ex ( SOCKET s , uint32 how )
{
	GUARD

	if ( ::shutdown(s,how) < 0 ) 
	{
#if __LINUX__
		switch ( errno ) {
		case EBADF : 
		case ENOTSOCK : 
		case ENOTCONN : 
		default : 
			{
				break;
			}
		}
#elif __WINDOWS__
		int32 iErr = WSAGetLastError() ;
		switch ( iErr ) 
		{
		case WSANOTINITIALISED : 
			strncpy( ErrorMessage, "WSANOTINITIALISED", _ESIZE ) ;
			break ;
		case WSAENETDOWN :
			strncpy( ErrorMessage, "WSAENETDOWN", _ESIZE ) ;
			break ;
		case WSAEINVAL : 
			strncpy( ErrorMessage, "WSAEINVAL", _ESIZE ) ;
			break ;
		case WSAEINPROGRESS : 
			strncpy( ErrorMessage, "WSAEINPROGRESS", _ESIZE ) ;
			break ;
		case WSAENOTCONN : 
			strncpy( ErrorMessage, "WSAENOTCONN", _ESIZE ) ;
			break ;
		case WSAENOTSOCK : 
			strncpy( ErrorMessage, "WSAENOTSOCK", _ESIZE ) ;
			break ;
		default :
			{
			strncpy( ErrorMessage, "UNKNOWN", _ESIZE ) ;
			break ;
			};
		};
#endif

		return FALSE ;
	}

	return TRUE ;
	
	UNGUARD

	return FALSE ;
}


int32 SOCKET_BASIC::select_ex ( int32 maxfdp1 , fd_set * readset , fd_set * writeset , fd_set * exceptset , struct timeval * timeout )
{
	GUARD

	int32 result;

	_NET_TRY 
	{
		result = ::select( maxfdp1 , readset , writeset , exceptset , timeout );
		if( result == SOCKET_ERROR )
		{
#if __LINUX__

#elif __WINDOWS__
		int32 iErr = WSAGetLastError() ;
		switch ( iErr ) 
		{
		case WSANOTINITIALISED : 
			strncpy( ErrorMessage, "WSANOTINITIALISED", _ESIZE ) ;
			break ;
		case WSAEFAULT:
			strncpy( ErrorMessage, "WSAEFAULT", _ESIZE ) ;
			break ;
		case WSAENETDOWN:
			strncpy( ErrorMessage, "WSAENETDOWN", _ESIZE ) ;
			break ;
		case WSAEINVAL:
			strncpy( ErrorMessage, "WSAEINVAL", _ESIZE ) ;
			break ;
		case WSAEINTR:
			strncpy( ErrorMessage, "WSAEINTR", _ESIZE ) ;
			break ;
		case WSAEINPROGRESS:
			strncpy( ErrorMessage, "WSAEINPROGRESS", _ESIZE ) ;
			break ;
		case WSAENOTSOCK:
			strncpy( ErrorMessage, "WSAENOTSOCK", _ESIZE ) ;
			break ;
		default :
			{
			strncpy( ErrorMessage, "UNKNOWN", _ESIZE ) ;
			break ;
			};
		};
#endif
		}
	} 
	_NET_CATCH
	{
	}

	return result;

	UNGUARD

	return 0 ;
}

Socket::Socket () 
{ 
	GUARD

	m_SocketID = INVALID_SOCKET ;
	memset( &m_SockAddr, 0, sizeof(SOCKADDR_IN) ) ;
	memset( m_IP, 0, IP_SIZE ) ;
	m_Port = 0 ;
	
	UNGUARD
}

Socket::Socket ( const char* host , uint32 port ) 
{ 
	GUARD

	strncpy( m_IP, host, IP_SIZE-1 ) ;
	m_Port = port ;

	//create() ;	

	UNGUARD
}

Socket::~Socket () 
{ 
	GUARD

	close() ;
	
	UNGUARD
}

BOOL Socket::create( )
{
	GUARD
	
#ifdef __WINDOWS__
#else
#endif
	m_SocketID = SOCKET_BASIC::socket_ex( AF_INET , SOCK_STREAM , 0 );
    
	memset( &m_SockAddr , 0 , sizeof(m_SockAddr) );
	
	m_SockAddr.sin_family = AF_INET;

	if( isValid() )
		return TRUE ;
	else 
		return FALSE ;

	UNGUARD

	return FALSE ;
}

BOOL Socket::reconnect ( const char* host , uint32 port )
{
	GUARD
	close();
	strncpy( m_IP, host, IP_SIZE-1 ) ;
	m_Port = port ;
	
    //create() ;    // modified by xiaotao 2016-8-9
	//return connect(); 
    m_SocketID = tcpGenericConnect(m_IP, m_Port);
    if (NET_ERR == m_SocketID)
        return FALSE;
    return TRUE;

	UNGUARD

	return FALSE ;
}

void Socket::close () 
{ 
	GUARD
	if( isValid() && !isSockError() ) 
	{
		_NET_TRY 
		{
			SOCKET_BASIC::closesocket_ex( m_SocketID );
		} 
		_NET_CATCH
		{
		}
	}
	m_SocketID = INVALID_SOCKET ;
	memset( &m_SockAddr, 0, sizeof(SOCKADDR_IN) ) ;
	m_Port = 0 ;
	memset( m_IP, 0, IP_SIZE ) ;

	UNGUARD
}
	
BOOL Socket::connect () 
{ 
	GUARD
	m_SockAddr.sin_addr.s_addr = inet_addr( m_IP );
	m_SockAddr.sin_port = htons(m_Port);
	BOOL result = SOCKET_BASIC::connect_ex( m_SocketID , (const struct sockaddr *)&m_SockAddr , sizeof(m_SockAddr) );
	if( result)
		return TRUE ;
	else
		return FALSE ;

	UNGUARD
	return 0 ;
}

BOOL Socket::connect (const char* host, uint32 port) 
{ 
	GUARD

	strncpy( m_IP, host, IP_SIZE-1 ) ;
	m_Port = port ;

	//return connect() ; //modified by xiaotao 2016-8-9
    m_SocketID = tcpGenericConnect(m_IP, m_Port);
    if (NET_ERR == m_SocketID)
        return FALSE;
    return TRUE;
	UNGUARD

	return 0 ;
}

uint32 Socket::send (const void* buf, uint32 len, uint32 flags) 
{ 
	GUARD

	return SOCKET_BASIC::send_ex( m_SocketID , buf , len , flags );

	UNGUARD

	return 0 ;
}

uint32 Socket::receive (void* buf, uint32 len, uint32 flags) 
{ 
	GUARD

	return SOCKET_BASIC::recv_ex( m_SocketID , buf , len , flags );

	UNGUARD

	return 0 ;
}

uint32 Socket::available ()const
{ 
	GUARD

	return SOCKET_BASIC::availablesocket_ex( m_SocketID );

	UNGUARD

	return 0 ;
}

uint32 Socket::getLinger ()const 
{ 
	GUARD

	struct linger ling;
	uint32 len = sizeof(ling);
	
	SOCKET_BASIC::getsockopt_ex( m_SocketID , SOL_SOCKET , SO_LINGER , &ling , &len );
	
	return ling.l_linger;

	UNGUARD

	return 0 ;
}

SOCKET Socket::accept( struct sockaddr* addr, uint32* addrlen )
{
	GUARD

	return SOCKET_BASIC::accept_ex( m_SocketID, addr, addrlen ) ;

	UNGUARD

	return INVALID_SOCKET ;
}

BOOL Socket::setLinger (uint32 lingertime) 
{
	GUARD

	struct linger ling;
	
	ling.l_onoff = lingertime > 0 ? 1 : 0;
	ling.l_linger = lingertime;
	
	return SOCKET_BASIC::setsockopt_ex( m_SocketID , SOL_SOCKET , SO_LINGER , &ling , sizeof(ling) );

	UNGUARD

	return FALSE ;
}

uint32 Socket::getSockError()const 
{ 
	GUARD

	return isSockError(); 

	UNGUARD

	return 0 ;
}

BOOL Socket::isNonBlocking ()const 
{ 
	GUARD

	return SOCKET_BASIC::getsocketnonblocking_ex( m_SocketID );

	UNGUARD

	return FALSE ;
}

BOOL Socket::setNonBlocking (BOOL on ) 
{ 
	GUARD

	return SOCKET_BASIC::setsocketnonblocking_ex( m_SocketID , on );

	UNGUARD

	return FALSE ;
}

uint32 Socket::getReceiveBufferSize ()const 
{ 
	GUARD

	uint32 ReceiveBufferSize;
	uint32 size = sizeof(ReceiveBufferSize);

	SOCKET_BASIC::getsockopt_ex( m_SocketID , SOL_SOCKET , SO_RCVBUF , &ReceiveBufferSize, &size );
	
	return ReceiveBufferSize;

	UNGUARD

	return 0 ;
}

BOOL Socket::setReceiveBufferSize (uint32 size)
{ 
	GUARD

	return (BOOL)(SOCKET_BASIC::setsockopt_ex( m_SocketID , SOL_SOCKET , SO_RCVBUF , &size, sizeof(uint32) ) ) ;

	UNGUARD

	return FALSE ;
}

uint32 Socket::getSendBufferSize ()const 
{ 
	GUARD

	uint32 SendBufferSize;
	uint32 size = sizeof(SendBufferSize);

	SOCKET_BASIC::getsockopt_ex( m_SocketID , SOL_SOCKET , SO_SNDBUF , &SendBufferSize, &size );
	
	return SendBufferSize;

	UNGUARD

	return 0 ;
}

BOOL Socket::setSendBufferSize (uint32 size)
{ 
	GUARD

	return (BOOL)(SOCKET_BASIC::setsockopt_ex( m_SocketID , SOL_SOCKET , SO_SNDBUF , &size, sizeof(uint32) ) );

	UNGUARD

	return FALSE ;
}

uint32 Socket::getPort ()const 
{ 
	GUARD

	return m_Port; 

	UNGUARD
}

IP_t Socket::getIP ()const 
{ 
	GUARD

	return (IP_t)(m_SockAddr.sin_addr.s_addr);; 

	UNGUARD
}

BOOL Socket::isValid ()const 
{
	GUARD

	return m_SocketID!=INVALID_SOCKET; 

	UNGUARD
}

SOCKET Socket::getSOCKET ()const 
{ 
	GUARD

	return m_SocketID; 

	UNGUARD
}

BOOL Socket::isSockError()const
{
	GUARD

	int32 error;
	uint32 len = sizeof(error);
	
	int32 Result = SOCKET_BASIC::getsockopt_ex2( m_SocketID , SOL_SOCKET , SO_ERROR , &error, &len );

	if( Result == 0 ) 
		return FALSE;
	else 			  
		return TRUE;

	UNGUARD

	return FALSE ;
}

BOOL Socket::bind( )
{
	GUARD
	
	m_SockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    m_SockAddr.sin_port        = htons(m_Port);

	BOOL result = SOCKET_BASIC::bind_ex( m_SocketID , (const struct sockaddr *)&m_SockAddr , sizeof(m_SockAddr) ) ;
	if( result )
		return TRUE ;
	else
		return FALSE ;

	UNGUARD

	return 0 ;
}

BOOL Socket::bind( uint32 port,const char* addr)
{
	GUARD

	m_Port = port ;
	if(strcmp(addr,"127.0.0.1")==0)
	{
		m_SockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
		m_SockAddr.sin_addr.s_addr = inet_addr(addr);

    m_SockAddr.sin_port        = htons(m_Port);

	BOOL result = SOCKET_BASIC::bind_ex( m_SocketID , (const struct sockaddr *)&m_SockAddr , sizeof(m_SockAddr) );
	if( result )
		return TRUE ;
	else
		return FALSE ;

	UNGUARD

	return 0 ;
}

BOOL Socket::listen( int32 backlog )
{
	GUARD
		
	return SOCKET_BASIC::listen_ex( m_SocketID , backlog );

	UNGUARD

	return FALSE ;
}

BOOL Socket::isReuseAddr ()const
{
	GUARD 
		
	int32 reuse;
	uint32 len = sizeof(reuse);
	
	SOCKET_BASIC::getsockopt_ex( m_SocketID , SOL_SOCKET , SO_REUSEADDR , &reuse , &len );
	
	return reuse == 1;
	
	UNGUARD

	return 0 ;
}

BOOL Socket::setReuseAddr ( BOOL on )
{
	GUARD 

	int32 opt = on == TRUE ? 1 : 0;
	
	return SOCKET_BASIC::setsockopt_ex( m_SocketID , SOL_SOCKET , SO_REUSEADDR , &opt , sizeof(opt) );
	
	UNGUARD

	return TRUE ;
}

