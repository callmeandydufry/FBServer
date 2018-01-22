#include "stdafx.h"
#include "SnidAllocManager.h"
#include "ModuleManager.h"

#if 0

BOOL SnidAllocManager::initServerModule(int32 moduleName, ModuleComponent* defaultComponent, int32 groupID)
{
	mEStatus				= EAS_Enough;
	mStartRequestSnid		= SnidUtil::genPlayerSnid(ESnidType_Player, 1);
	mAllSnidNum 			= 0;
	mRequestFromDBNum 		= 0;
	mFetchSnidNum 			= 0;
	mIsALLInited			= FALSE;
	mValidMaxSnid			= mStartRequestSnid;

	mStrSnidMap.InitTable(MAX_CREATE_SNID_NUM, MAX_ACCOUNT);
	mSnidPool.initPool(MAX_SNID_POOL_NUMS);

	return ServerModuleBase::initServerModule(moduleName, this, groupID);
}

void SnidAllocManager::uinitServerModule()
{
	ServerModuleBase::uinitServerModule();
}

void SnidAllocManager::tickModule(uint32 uTick)
{
	ServerModuleBase::tickModule(uTick);
	if (!mIsALLInited)
	{
		tickFetchSnidNumFromDB();
		mEStatus = EAS_Wait_Init;
	}

	switch (mEStatus)
	{
	case EAS_Init:
		break;
	case EAS_Wait_Init:
		break;
	case EAS_Enough:
		if (!isLeftSnidEnough())
		{
			mEStatus = EAS_NotEnough;
		}
		break;
	case EAS_NotEnough:
		requestNewSnidsFromDB();
		break;
	case EAS_Applying:
		break;
	default:
		break;
	}
}

// 通过account请求snid [1/4/2018 Chief]
uint64 SnidAllocManager::rpcAllocPlayerUniqueSnid(const char* szAccount, int64 nSession)
{
	SNID_t u64Snid = INVALID_SNID;
	__GUARD__;

	if (mStrSnidMap.Get(szAccount))
	{
		SnidAllocArchive* pArchive = mStrSnidMap.Get(szAccount);
		u64Snid = pArchive->mDBState.getSnid();
	}
	else
	{
		if (mFreeSnidQuene.size() > 0)
		{
			SnidAllocArchive* pArchive = mFreeSnidQuene.front();
			if (pArchive)
			{
				// free_quene--
				mFreeSnidQuene.pop();

				KCheck(mFreeSnidQuene.size() > 0);
				// log: ERROR SnidAllocManager tick error, can't create more snids

				u64Snid = pArchive->mDBState.getSnid();

				// add to map
				pArchive->mDBState.setAccountName(szAccount);
				mStrSnidMap.Add(szAccount, pArchive);

				updateSNIDAccount2DB(u64Snid, szAccount);
			}
		}
	}

	return u64Snid;
	__UNGUARD__;
	return u64Snid;
}

SNID_t SnidAllocManager::rpcGetValidPlayerSnid()
{
	return mValidMaxSnid;
}

class __SnidAllocManager_rpcUpdateSnidData_Callback : public SnidAllocManager
{
public:
	void callbackSuccess(BOOL& bSave)
	{
		
	}

	void callbackOvertime()
	{
		
	}
};

BOOL SnidAllocManager::updateSNIDAccount2DB(SNID_t n64Snid, const char* szAccount)
{
	BOOL bSave = FALSE;
	__GUARD__;

	g_pModuleManager->getExportCallable< IDBProxyManagerExportCallable<__SnidAllocManager_rpcUpdateSnidData_Callback> >(
		ServerModuleUtil::getDBProxyModuleByPlayerSnid(n64Snid),
		MODULE_DEFAULT_ID,
		(__SnidAllocManager_rpcUpdateSnidData_Callback*)this
		)
		->rpcUpdateSnidData(n64Snid, szAccount)
		->bindCallback(
			&__SnidAllocManager_rpcUpdateSnidData_Callback::callbackSuccess,
			&__SnidAllocManager_rpcUpdateSnidData_Callback::callbackOvertime,
			10000
		);

	bSave = TRUE;

	return bSave;
	__UNGUARD__;
	return bSave;
}

//----------------------------------------------------------------------
// 从DB请求数据 init
//----------------------------------------------------------------------

class __SnidAllocManager_rpcFetchAllSnidNum_Callback : public SnidAllocManager
{
public:
	void callbackSuccess(int32& nNum)
	{
		if (nNum > 0)
		{
			mAllSnidNum = nNum;
			mFetchSnidNum = (mAllSnidNum - mRequestFromDBNum > BATCH_SNID_ALLOC_NUM) ? BATCH_SNID_ALLOC_NUM : mAllSnidNum - mRequestFromDBNum;
			batchFetchDataFromDB();
		}
		else
		{
			requestNewSnidsFromDB();
		}
		
	}

	void callbackOvertime()
	{
		tickFetchSnidNumFromDB();
	}
};

void SnidAllocManager::tickFetchSnidNumFromDB()
{
	__GUARD__;

	mIsALLInited = TRUE;

	g_pModuleManager->getExportCallable< IDBProxyManagerExportCallable<__SnidAllocManager_rpcFetchAllSnidNum_Callback> >(
		ServerModuleUtil::getDBProxyModuleByPlayerSnid(mStartRequestSnid),
		MODULE_DEFAULT_ID,
		(__SnidAllocManager_rpcFetchAllSnidNum_Callback*)this
		)
		->rpcFetchAllSnidNum()
		->bindCallback(
			&__SnidAllocManager_rpcFetchAllSnidNum_Callback::callbackSuccess,
			&__SnidAllocManager_rpcFetchAllSnidNum_Callback::callbackOvertime,
			10000
		);
	
	__UNGUARD__;
}

class __SnidAllocManager_rpcFetchSnidDataByNum_Callback : public SnidAllocManager
{
public:
	void callbackSuccess(BatchSnidAllocArchive& stArchive)
	{
		if (mFetchSnidNum == stArchive.mBatchNum)  // fetch sucess
		{
			mRequestFromDBNum += stArchive.mBatchNum;
			mStartRequestSnid += stArchive.mBatchNum;

			for (int32 i = 0; i < stArchive.mBatchNum; ++i)
			{
				SnidAllocArchive* pSnidArchive = mSnidPool.allocObject();

				if (pSnidArchive)
				{
					pSnidArchive->mDBState.copyProperty(&stArchive.mBatchSnidData[i].mDBState);
					const char* szOldAccount = pSnidArchive->mDBState.getAccountName().c_str();

					if (!strncmp(szOldAccount, " ", sizeof szOldAccount))  // equal
					{
						mFreeSnidQuene.push(pSnidArchive);
						mValidMaxSnid = pSnidArchive->mDBState.getSnid();
					}
					else
					{
						mStrSnidMap.Add(pSnidArchive->mDBState.getAccountName().c_str(), pSnidArchive);
					}
				}
			}
			
			// 全部获取完毕后 
			if (mRequestFromDBNum == mAllSnidNum)
			{
				mEStatus = EAS_Enough;
				mIsALLInited = TRUE;
				return;
			}

			mFetchSnidNum = (mAllSnidNum - mRequestFromDBNum > BATCH_SNID_ALLOC_NUM) ? BATCH_SNID_ALLOC_NUM : mAllSnidNum - mRequestFromDBNum;

			batchFetchDataFromDB();
		}
		else  // fetch fail
		{
			batchFetchDataFromDB();
		}
	}

	void callbackOvertime()
	{
		//mIsALLInited = FALSE;
	}
};

void SnidAllocManager::batchFetchDataFromDB()
{
	__GUARD__;

	g_pModuleManager->getExportCallable< IDBProxyManagerExportCallable<__SnidAllocManager_rpcFetchSnidDataByNum_Callback> >(
		ServerModuleUtil::getDBProxyModuleByPlayerSnid(mStartRequestSnid),
		MODULE_DEFAULT_ID,
		(__SnidAllocManager_rpcFetchSnidDataByNum_Callback*)this
		)
		->rpcFetchSnidDataByNum(mStartRequestSnid, mFetchSnidNum)
		->bindCallback(
			&__SnidAllocManager_rpcFetchSnidDataByNum_Callback::callbackSuccess,
			&__SnidAllocManager_rpcFetchSnidDataByNum_Callback::callbackOvertime,
			10000
		);


	__UNGUARD__;
}

//----------------------------------------------------------------------
// 申请更多SNID
//----------------------------------------------------------------------
class __SnidAllocManager_rpcBatchInsertData_Callback : public SnidAllocManager
{
public:
	void callbackSuccess(BOOL& bInsert)
	{
		if (bInsert)
		{
			mRequestFromDBNum += MAX_CREATE_SNID_NUM;			// 扩充当前申请数量

			for (int32 i = 1; i <= MAX_CREATE_SNID_NUM; ++i)	// 扩充 mFreeSnidQuene
			{
				SnidAllocArchive* pSnidArchive = mSnidPool.allocObject();
				pSnidArchive->mDBState.setSnid(mValidMaxSnid++);
				pSnidArchive->mDBState.setAccountName(" ");
				mFreeSnidQuene.push(pSnidArchive);
			}

			mEStatus = EAS_Enough;								// 更改状态
		}
		else
		{
			//requestNewSnidsFromDB();
		}
		
	}
	void callbackOvertime()
	{
	}
};

void SnidAllocManager::requestNewSnidsFromDB()
{
	__GUARD__;

	mEStatus = EAS_Applying;

	g_pModuleManager->getExportCallable< IDBProxyManagerExportCallable<__SnidAllocManager_rpcBatchInsertData_Callback> >(
		ServerModuleUtil::getDBProxyModuleByPlayerSnid(mValidMaxSnid),
		MODULE_DEFAULT_ID,
		(__SnidAllocManager_rpcBatchInsertData_Callback*)this
		)
		->rpcBatchInsertData(MAX_CREATE_SNID_NUM, mStartRequestSnid)
		->bindCallback(
			&__SnidAllocManager_rpcBatchInsertData_Callback::callbackSuccess,
			&__SnidAllocManager_rpcBatchInsertData_Callback::callbackOvertime,
			10000
		);

	__UNGUARD__;
}

//----------------------------------------------------------------------
// isLeftSnidEnough
//----------------------------------------------------------------------
BOOL SnidAllocManager::isLeftSnidEnough()
{
	__GUARD__;

	// 获得当前DB中有效的SNID空余数量
	int32 nFreeSnidNum = mFreeSnidQuene.size();
	// 是否够 1/10
	return (nFreeSnidNum > MAX_CREATE_SNID_NUM / 10);

	__UNGUARD__;
	return FALSE;
}

//----------------------------------------------------------------------
// factory
//----------------------------------------------------------------------

EServerModuleType SnidAllocModuleFactory::getType() const
{
	return EServerModuleType_SnidAlloc;
}

ServerModuleBase* SnidAllocModuleFactory::createModule(EServerModuleType type, int32 group)
{
	if (mPtrServerModule)
	{
		delete(mPtrServerModule);
		mPtrServerModule;
	}

	mPtrServerModule = new SnidAllocManager;
	return mPtrServerModule;
}

void SnidAllocModuleFactory::destroyModule(ModuleBase* module)
{
	if (mPtrServerModule)
	{
		delete(mPtrServerModule);
		mPtrServerModule;
	}
}

#endif