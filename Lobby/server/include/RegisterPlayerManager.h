#ifndef __REGISTER_PLAYER_MANAGER_H__
#define __REGISTER_PLAYER_MANAGER_H__

#include "TypeDefine.h"
#include "ServerModuleBase.h"
#include "StringHashMap.h"
#include "ServerModuleExportInterface.h"
#include "CommonObjectPool.h"
#include "ServerModuleUtils.h"
#include "ServerModuleManager.h"

class RegisterPlayer;

class RegisterPlayerManager
	: public ServerModuleBase
	, public IRegisterPlayerManager
{
public:
	RegisterPlayerManager();
	~RegisterPlayerManager();

	virtual BOOL initServerModule(int32 moduleName, ModuleComponent* defaultComponent, int32 groupID);
	virtual void uinitServerModule();
	virtual void tickModule(uint32 uTick);

	RegisterPlayer* getPlayerByAccount(const char* account);
	RegisterPlayer* getPlayerBySnid(SNID_t snid);

	// ����DB���ݳ�ʼ��ע����Ҷ��� [12/19/2017 yz]
	void initRegisterPlayer(RegisterPlayerArchive& dbArchive);

	void removeReigsterPlayer(RegisterPlayer* player);

	// ����ĳ��������ߣ�Ȼ�������ݵ�OnlinePlayerManagerȥ����OnlinePlayer [12/13/2017 yz]
	void requestPlayerCreate(BOOL isFromLogin, const char* account, int64 sessionID, SNID_t u64PlayerID);
	
	//----------------------------------------------------------------------
	//RPC���ýӿ� START
	//----------------------------------------------------------------------
	virtual BOOL rpcRequestPlayerOnline(const char* account, int64 sessionID);
	virtual BOOL rpcUpdatePlayerOnlineState(SNID_t snid, BOOL isOnline);

	//----------------------------------------------------------------------
	//RPC���ýӿ� END
	//----------------------------------------------------------------------
	virtual int64 getComponentId() { return MODULE_DEFAULT_ID; }
protected:
	void initNewPlayerDB(RegisterPlayerArchive& dbArchive);

	// ����ʱ�ֶ����DB��ȡע��������� [12/19/2017 yz]
	void tickFetchDataFromDB();

	// һ���Դ�DBProxy��ȡһ��SNID����Ķ�����ע������ [1/2/2018 yz]
	void fetchRegisterPlayerByRange();

	// ����һ���µ�δʹ�õ�SNID����ע����� [1/16/2018 yz]
	SNID_t allocNewSnid();

protected:
	CommonObjectPool<RegisterPlayer> mPlayerPool;
	TStaticArray<RegisterPlayer*, MODULE_REGISTERPLAYER_NUM> mPlayerList;

	// �Ƿ��Ѿ���DB�л�ȡ��ȫ����ע����� [12/19/2017 yz]
	BOOL mIsALLPlayerInited;
	// �Ƿ�������DBProxy�������� [1/16/2018 yz]
	BOOL mIsReqeustingDataFromDB;

	// ��һ���ɷ����Snid [1/16/2018 yz]
	SNID_t mNextValidSnid;

	// ֮ǰע������Snid���в������ģ�����������Snid���ռ������������ʱ���ȷ�����Щ©����Snid [1/16/2018 yz]
	TDynaArray<SNID_t> mLeakedValidSnids;

protected:
	typedef TStringHashMap<RegisterPlayer*>		RegPlayerStrHashMap;
	typedef TIntegerHashMap<RegisterPlayer*>	RegPlayerIntHashMap;

	RegPlayerStrHashMap	mAccountPlayerMap;			// ͨ���˺Ų��ҽ�ɫ [12/13/2017 yz]
	RegPlayerIntHashMap	mSnidPlayerMap;				// ͨ��SNID���ҽ�ɫ [12/13/2017 yz]
	RegPlayerStrHashMap	mNamePlayerMap;				// ͨ�����ֲ������ [1/4/2018 Chief]
};

//----------------------------------------------------------------------
//factory
//----------------------------------------------------------------------

class RegisterModuleFactory : public ServerModuleFactory
{
	RegisterPlayerManager* mPtrServerModule;
public:
	virtual EServerModuleType getType() const;
	virtual ServerModuleBase* createModule(EServerModuleType type, int32 group);
	virtual void destroyModule(ModuleBase* module);
};

#endif