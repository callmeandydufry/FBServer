#ifndef __SNID_ALLOCATEE_DBPROXY_H__
#define __SNID_ALLOCATEE_DBPROXY_H__

#include "TypeDefine.h"
#include "SnidAllocArchiveData.h"
#include "DBProxyManager.h"

#if 0

class DBProxyManager;
class BatchSnidAllocArchive;
class SnidAllocArchive;

class SnidAllocateeDBProxy
{
public:
	SnidAllocateeDBProxy(DBProxyManager* mgr)
		: mDBManager(mgr)
	{}

	~SnidAllocateeDBProxy() 
	{
	}

	// ��ȡDB���ж�����SNID/Account���� [1/5/2018 Chief] 
	int32 doFetchAllSnidNum();

	// ������ȡ [1/5/2018 Chief] 
	BOOL doFetchSnidDataByNum(SNID_t u64Snid, int32 nFetchNum, BatchSnidAllocArchive& stArchive);

	// ��������,��֤�ɹ����뼴��,���÷������� [1/5/2018 Chief] 
	BOOL doBatchInsertData(int32 nInsertNum, SNID_t n64Snid);

	// ����(snid,account) [1/5/2018 Chief]
	BOOL doUpdateSnidData(SNID_t u64Snid, const char* szAccountName);

private:
	DBProxyManager*			mDBManager;

	DBSnidAllocState		mDBSnidAllocState;
};
#endif

#endif // __SNID_ALLOCATEE_DBPROXY_H__

