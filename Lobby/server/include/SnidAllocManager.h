#ifndef __SNID__ALLOC_MANAGER_H__
#define __SNID__ALLOC_MANAGER_H__

#include "ServerModuleExportInterface.h"
#include "ServerModuleUtils.h"
#include "ServerModuleBase.h"
#include "CommonObjectPool.h"
#include "StringHashMap.h"
#include "ServerModuleManager.h"
#include <queue>

#if 0

enum EAllocStatus
{
	EAS_Init,
	EAS_Wait_Init,
	EAS_Enough,
	EAS_NotEnough,				
	EAS_Applying,				// 正在申请中 [1/11/2018 Chief]
};

class SnidAllocManager
	: public ServerModuleBase
	, public ISnidAllocManager
{
public:
	SnidAllocManager()
	{
	}

	~SnidAllocManager()
	{
	}

	virtual BOOL initServerModule(int32 moduleName, ModuleComponent* defaultComponent, int32 groupID);
	virtual void uinitServerModule();
	virtual void tickModule(uint32 uTick);
	virtual int64 getComponentId() { return MODULE_DEFAULT_ID; }

	//----------------------------------------------------------------------
	//RPC调用接口 START
	//----------------------------------------------------------------------

	// 通过account查询玩家 [1/11/2018 Chief]
	virtual uint64 rpcAllocPlayerUniqueSnid(const char* szAccount, int64 nSession);		

	// 获得当前最大有效snid [1/11/2018 Chief]
	virtual SNID_t rpcGetValidPlayerSnid();

	//----------------------------------------------------------------------
	//RPC调用接口 END
	//----------------------------------------------------------------------
protected:
	void tickFetchSnidNumFromDB();				// 获取当前数据库中SNID数量 [1/11/2018 Chief]
	void requestNewSnidsFromDB();				// 申请新的空闲snid  [1/11/2018 Chief]
	BOOL isLeftSnidEnough();					// 检测剩余空闲SNID是否够用  [1/11/2018 Chief]
	void batchFetchDataFromDB();				// 批量获取数据 [1/11/2018 Chief]
	BOOL updateSNIDAccount2DB(SNID_t n64Snid, const char* szAccount);	// 将新申请的数据入库  [1/11/2018 Chief]
protected:
	BOOL				mIsALLInited;			// 
	EAllocStatus		mEStatus;				// 

	//----------------------------------------------------------------------
	// 从DB请求数据 init
	//----------------------------------------------------------------------
	SNID_t				mStartRequestSnid;		// 开始请求snid [1/11/2018 Chief]
	int32				mAllSnidNum;			// all snid nums [1/11/2018 Chief]
	int32				mRequestFromDBNum;		// 已从DB请求的数量 [1/11/2018 Chief]
	int32				mFetchSnidNum;			// 本次请求数量 [1/11/2018 Chief]

	//----------------------------------------------------------------------
	// 申请更多SNID
	//----------------------------------------------------------------------
	SNID_t				mValidMaxSnid;			// 当前有效的最大SNID [1/11/2018 Chief]

protected:
	typedef CommonObjectPool<SnidAllocArchive>	SnidAllocArchPool;
	typedef TStringHashMap<SnidAllocArchive*>	SnidAllocArchStrMap;
	typedef std::queue<SnidAllocArchive*>		SnidAllocArchQueue;

	SnidAllocArchPool	mSnidPool;				// snid pool [1/11/2018 Chief]
	SnidAllocArchStrMap	mStrSnidMap;			// 远程调用时只有account查询 [1/11/2018 Chief]
	SnidAllocArchQueue	mFreeSnidQuene;			// todo 需要提供一个queue，替换掉std::queue 空闲SNID [1/11/2018 Chief]

};

//----------------------------------------------------------------------
// factory
//----------------------------------------------------------------------
class SnidAllocModuleFactory : public ServerModuleFactory
{
	SnidAllocManager* mPtrServerModule;
public:
	virtual EServerModuleType getType() const;
	virtual ServerModuleBase* createModule(EServerModuleType type, int32 group);
	virtual void destroyModule(ModuleBase* module);
};

#endif

#endif // __SNID__ALLOC_MANAGER_H__
