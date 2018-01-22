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

	// 获取DB中有多少条SNID/Account数据 [1/5/2018 Chief] 
	int32 doFetchAllSnidNum();

	// 批量获取 [1/5/2018 Chief] 
	BOOL doFetchSnidDataByNum(SNID_t u64Snid, int32 nFetchNum, BatchSnidAllocArchive& stArchive);

	// 批量插入,保证成功插入即可,不用返回数据 [1/5/2018 Chief] 
	BOOL doBatchInsertData(int32 nInsertNum, SNID_t n64Snid);

	// 保存(snid,account) [1/5/2018 Chief]
	BOOL doUpdateSnidData(SNID_t u64Snid, const char* szAccountName);

private:
	DBProxyManager*			mDBManager;

	DBSnidAllocState		mDBSnidAllocState;
};
#endif

#endif // __SNID_ALLOCATEE_DBPROXY_H__

