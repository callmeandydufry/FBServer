#include "stdafx.h"
#include "IpRegionConfig.h"
#include "IntegerHashMap.h"

IpRegionConfig g_IpRegionConfig;

IpRegionConfig::IpRegionConfig()
{
	m_mapHost.InitTable(MAX_IP_RANGE_NUM);
}

IpRegionConfig::~IpRegionConfig()
{
	Clear();
}

BOOL IpRegionConfig::Init()
{
	LoadData();
	return TRUE;
}

void IpRegionConfig::Clear()
{
	m_mapHost.Clear();
}

// 通过IP地址获取网络类型 网通/电信
char IpRegionConfig::GetNetTypeByIpAddr(const char* pszHost)
{
	__GUARD__

	NET_TYPE liType = NET_TYPE_UNKNOW;

	uint32 uAddress = inet_addr(pszHost);
	
	in_addr lmask;
	// 目前只对电信和网通进行判断，如果非电信的IP则都认为是网通线
	// 电信网通网段掩码范围一般255.0.0.0 - 255.255.255.0 之间
	char pszMask[32]={0};
	unsigned char a,b,c,d;
	for (int32 idx = 8; idx < 24; idx++)
	{
		lmask.s_addr = 0;
		
		if (idx < 16)
		{
			a = 0xff;
			b = 0xff << (16-idx);
			c = d = 0;

		}
		else if (idx < 24)
		{
			a = b = 0xff;
			c = 0xff << (24-idx);
			d = 0;
		}	

		sprintf(pszMask, "%d.%d.%d.%d",a,b,c,d);

		lmask.s_addr = inet_addr(pszMask);

		uint32 uHost = uAddress & lmask.s_addr;

		int32 nValue = (int32)(m_mapHost.Get(uHost));
		if (nValue > 0 && (nValue%100) == idx)
		{
			liType = (NET_TYPE)(nValue / 100);
			break;
		}
	}
	
	// 没有找到则默认是网通线
	if (liType == NET_TYPE_UNKNOW)
	{
		liType = NET_TYPE_CNC;
	}

	return liType;

	__UNGUARD__
}

void IpRegionConfig::LoadData()
{
	__GUARD__
	Configer	ini(FILE_IPSET);
	
	// 读取电信数据
	int32 liCount = ini.Get_Property_Int( "chinanet", "count" );
	int32 liType = ini.Get_Property_Int("chinanet", "type" );

	char lszIp[IP_SIZE + 3] = {0};
	char lszKey[32] = {0};
	for (int32 idx = 1; idx <= liCount; idx++)
	{
		sprintf(lszKey, "ip%d", idx);
		ini.GetText( "chinanet", lszKey, lszIp, IP_SIZE + 3 );

		int32 liPos = strcspn(lszIp, "/");
		lszIp[liPos] = '\0';

		const char* ip = &lszIp[0];
		int host = atoi(&lszIp[++liPos]);

		Add(ip, host, liType);
	}
	__UNGUARD__
}

void IpRegionConfig::Add(const char* pszHost, int32 nHost, char nNetType)
{
	unsigned long ulAddress = inet_addr(pszHost);
	in_addr sin;
	sin.s_addr = ulAddress;

	m_mapHost.Add(ulAddress, (void*)(nHost + (nNetType * 100) ));
}
