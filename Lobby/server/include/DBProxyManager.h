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

// DB���ݷ��� [12/15/2017 yz]
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
	// RPCԶ�̽ӿڶ��� [12/16/2017 yz]
	//---------------------------------------------------------------------------------------
	virtual PlayerArchive& rpcFetchOnlinePlayerArchive(SNID_t snid);

	// �������µ�������ݲ����뵽���ݿ� [12/15/2017 yz]
	virtual void rpcCreateNewPlayerArchive(RegisterPlayerArchive& archive);

	// �������ȫ�����ݵ�DB [12/15/2017 yz]
	virtual BOOL rpcSavePlayerArchive(PlayerArchive& archive);

	// ��ѯ����SNID�����ڵ�������� [1/2/2018 yz]
	virtual int32 rpcFetchPlayerNumInSnidRange(int32 serverID, SNID_t begin, SNID_t end);

	// ��ȡ���������ڶ����ҵ�ע��������� [1/2/2018 yz]
	virtual BatchRegPlayerArchive& rpcFetchRegisterPlayerInSnidRange(int32 serverID, SNID_t begin, int32 limit);

	//---------------------------------------------------------------------------------------
	// RPCԶ�̽ӿڶ��� END [12/16/2017 yz]
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
	int32						mCategoryConnMap[EDBCategory_MAX][DB_MAX_CATEGORY_GROUP];  // todo ��Ҫ����

	DBOper*						mDBConnections[DB_MAX_CONNECTION];

	PlayerDBProxy*				mPlayerDBProxy;
 
	//---------------------------------------------------------------------------------------
	// ��������������ֵ�õģ���ΪҪ�������ã����Բ�������ʱ����������ʹ��һ����Ա������������ [12/16/2017 yz]
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