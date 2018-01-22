#ifndef __DBPROXY_MANAGER_H__
#define __DBPROXY_MANAGER_H__

#include "TypeDefine.h"
#include "ServerModuleBase.h"
#include "DBConst.h"
#include "PlayerArchiveStruct.h"
#include "ServerModuleExportInterface.h"
#include "ServerModuleManager.h"
#include "PropertyVersion.h"

class DBOper;
class PlayerDBProxy;
class SnidAllocateeDBProxy;

struct DBConnectionInfo
{
	FixedString<IP_SIZE> mIP;
	int32 mPort;
	FixedString<DB_CONNECTION_NAME_LEN> mDBName;
	FixedString<MAX_ACCOUNT> mDBUser;
	FixedString<MAX_ACCOUNT> mDBPwd;
};

// DB数据分类 [12/15/2017 yz]
enum EDBDataCategory
{
	EDBCategory_UNKNOWN = -1,
	EDBCategory_Version,
	EDBCategory_Snid,
	EDBCategory_Player,
	EDBCategory_Friend,
	EDBCategory_Guild,
	EDBCategory_MAX,
};

struct DBCategoryAddress
{
	EDBDataCategory mDataCategory;
	int32 mGroupIndex;
	int32 mDBIndex;
};

class DBProxyManager
	: public ServerModuleBase
	, public IDBProxyManager
{
public:
	DBProxyManager();
	~DBProxyManager();

	virtual BOOL initServerModule(int32 moduleName, ModuleComponent* defaultComponent, int32 groupID);
	virtual void uinitServerModule();
	virtual void tickModule(uint32 uTick);

	DBOper* getDBConnByCategory(EDBDataCategory dataCategory, int32 groupIndex);

	//---------------------------------------------------------------------------------------
	// RPC远程接口定义 [12/16/2017 yz]
	//---------------------------------------------------------------------------------------
	virtual PlayerArchive& rpcFetchOnlinePlayerArchive(SNID_t snid);

	// 创建出新的玩家数据并插入到数据库 [12/15/2017 yz]
	virtual void rpcCreateNewPlayerArchive(RegisterPlayerArchive& archive);

	// 保存玩家全部数据到DB [12/15/2017 yz]
	virtual BOOL rpcSavePlayerArchive(PlayerArchive& archive);

	// 查询给定SNID区间内的玩家总数 [1/2/2018 yz]
	virtual int32 rpcFetchPlayerNumInSnidRange(int32 serverID, SNID_t begin, SNID_t end);

	// 获取给定区间内多个玩家的注册玩家数据 [1/2/2018 yz]
	virtual BatchRegPlayerArchive& rpcFetchRegisterPlayerInSnidRange(int32 serverID, SNID_t begin, int32 limit);

	//---------------------------------------------------------------------------------------
	// RPC远程接口定义 END [12/16/2017 yz]
	//---------------------------------------------------------------------------------------
	virtual int64 getComponentId() { return MODULE_DEFAULT_ID; }
protected:
	void loadDBConfig();
	BOOL DecodeDBPassword(const char* pIn, char* pOut);
	BOOL CheckVersion(int32 nConnNum);

protected:
	int32						mVersionNum;
	int32						mDBConnNum;
	DBConnectionInfo			mDBConnectionInfos[DB_MAX_CONNECTION];
	int32						mCategoryConnMap[EDBCategory_MAX][DB_MAX_CATEGORY_GROUP];  // todo 需要配置

	DBOper*						mDBConnections[DB_MAX_CONNECTION];

	PlayerDBProxy*				mPlayerDBProxy;
 
	//---------------------------------------------------------------------------------------
	// 用来做函数返回值用的，因为要返回引用，所以不能用临时变量，这里使用一个成员变量返回引用 [12/16/2017 yz]
	//---------------------------------------------------------------------------------------
	PlayerArchive				mReturnPlayerArchive;
	DBVersionState				mDBVersionState;
	BatchRegPlayerArchive		mBatchPlayerArchive;
};


//---------------------------------------------------------------------------------------
// factory
//---------------------------------------------------------------------------------------
class DBProxyModuleFactory : public ServerModuleFactory
{
	DBProxyManager* mPtrServerModule;
public:
	virtual EServerModuleType getType() const;
	virtual ServerModuleBase* createModule(EServerModuleType type, int32 group);
	virtual void destroyModule(ModuleBase* module);
};

#endif