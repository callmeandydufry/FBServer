#ifndef __IP_REGION_CONFIG_H__
#define __IP_REGION_CONFIG_H__


class IntegerHashMap;
class IpRegionConfig
{
	enum{MAX_IP_RANGE_NUM=512};
public:
	IpRegionConfig();
	~IpRegionConfig();

	BOOL Init();
	
	// ͨ��IP��ַ��ȡ�������� ��ͨ/����
	char GetNetTypeByIpAddr(const char* pszHost);

private:
	void Clear();
	void LoadData();
	// ���һ��Ip�� 58.44.0.0/14��pszHost=58.44.0.0 nHost=14
	void Add(const char* pszHost, int32 nHost, char nNetType);
private:
	IntegerHashMap m_mapHost;
};

extern IpRegionConfig g_IpRegionConfig;

#endif