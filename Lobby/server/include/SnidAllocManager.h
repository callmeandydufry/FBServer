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
	EAS_Applying,				// ���������� [1/11/2018 Chief]
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
	//RPC���ýӿ� START
	//----------------------------------------------------------------------

	// ͨ��account��ѯ��� [1/11/2018 Chief]
	virtual uint64 rpcAllocPlayerUniqueSnid(const char* szAccount, int64 nSession);		

	// ��õ�ǰ�����Чsnid [1/11/2018 Chief]
	virtual SNID_t rpcGetValidPlayerSnid();

	//----------------------------------------------------------------------
	//RPC���ýӿ� END
	//----------------------------------------------------------------------
protected:
	void tickFetchSnidNumFromDB();				// ��ȡ��ǰ���ݿ���SNID���� [1/11/2018 Chief]
	void requestNewSnidsFromDB();				// �����µĿ���snid  [1/11/2018 Chief]
	BOOL isLeftSnidEnough();					// ���ʣ�����SNID�Ƿ���  [1/11/2018 Chief]
	void batchFetchDataFromDB();				// ������ȡ���� [1/11/2018 Chief]
	BOOL updateSNIDAccount2DB(SNID_t n64Snid, const char* szAccount);	// ����������������  [1/11/2018 Chief]
protected:
	BOOL				mIsALLInited;			// 
	EAllocStatus		mEStatus;				// 

	//----------------------------------------------------------------------
	// ��DB�������� init
	//----------------------------------------------------------------------
	SNID_t				mStartRequestSnid;		// ��ʼ����snid [1/11/2018 Chief]
	int32				mAllSnidNum;			// all snid nums [1/11/2018 Chief]
	int32				mRequestFromDBNum;		// �Ѵ�DB��������� [1/11/2018 Chief]
	int32				mFetchSnidNum;			// ������������ [1/11/2018 Chief]

	//----------------------------------------------------------------------
	// �������SNID
	//----------------------------------------------------------------------
	SNID_t				mValidMaxSnid;			// ��ǰ��Ч�����SNID [1/11/2018 Chief]

protected:
	typedef CommonObjectPool<SnidAllocArchive>	SnidAllocArchPool;
	typedef TStringHashMap<SnidAllocArchive*>	SnidAllocArchStrMap;
	typedef std::queue<SnidAllocArchive*>		SnidAllocArchQueue;

	SnidAllocArchPool	mSnidPool;				// snid pool [1/11/2018 Chief]
	SnidAllocArchStrMap	mStrSnidMap;			// Զ�̵���ʱֻ��account��ѯ [1/11/2018 Chief]
	SnidAllocArchQueue	mFreeSnidQuene;			// todo ��Ҫ�ṩһ��queue���滻��std::queue ����SNID [1/11/2018 Chief]

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
