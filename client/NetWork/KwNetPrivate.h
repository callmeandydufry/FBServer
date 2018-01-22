/********************************************************************
	Copyright (c) 2008 Beijing Kylin Network Information 
	Science and Technology Co, Ltd. All Rights Reserved.
	created:	2010/11/30
	author:		JiangWei
	
	purpose:	
*********************************************************************/
#ifndef KwNetPrivate_h__
#define KwNetPrivate_h__

#include "KwNetDrv.h"


#if __WINDOWS__
#elif __LINUX__
#include <sys/types.h>			
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>			
#include <netinet/in.h>
#include <errno.h>
#include <sys/types.h>	// for open()
#include <sys/stat.h>	// for open()
#include <unistd.h>		// for fcntl()
#include <fcntl.h>		// for fcntl()
#include <sys/ioctl.h>	// for ioctl()
#include <errno.h>		// for errno
#include <pthread.h>
#include <string.h>

#include <net/if.h> 

// IOS平台没有此头文件
#if !defined(IOS)
#include <net/if_arp.h>
#endif

#include <dirent.h>
#include <string.h>
#endif

#endif // KwNetPrivate_h__