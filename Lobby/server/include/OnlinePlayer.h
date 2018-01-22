#ifndef __ONLINE_PLAYER_H__
#define __ONLINE_PLAYER_H__

#include "TypeDefine.h"
#include "PlayerArchiveStruct.h"
#include "ServerModuleExportInterface.h"

class OnlinePlayerManager;

class OnlinePlayer : public IOnlinePlayer
{
public:
	OnlinePlayer();
	~OnlinePlayer();

	enum EOnlineState
	{
		EOnlineState_Uninited,
		EOnlineState_Created,
		EOnlineState_WaitDBData,
		EOnlineState_DBInited,
		EOnlineState_EnterOnline,
		EOnlineState_ConnectSession,
		EOnlineState_Online,
		EOnlineState_RequestLeaveOnline,
		EOnlineState_SavingBeforeLeave,
		EOnlineState_SavedAndOffline,
		EOnlineState_Invalid,
	};

	void init(OnlinePlayerManager* mgr, const char* account, SNID_t snid);
	void uninit();
	void clear();

	void bindSessionID(int64 sessionID);
	void unbindSession();
	int64 getSessionID() const { return mSessionID; }
	BOOL isInvalid() { return EOnlineState_Invalid == mOnlineState; }

	virtual int64			getComponentId();

	void					tickComponent(uint32 uTick);

	OnlinePlayerManager*	getPlayerManager();

	// ����DB���� [12/15/2017 yz]
	void					requestDBData();

	// �յ�DB���ص������������ [12/16/2017 yz]
	void					receiveDBData(PlayerArchive* dbArchive);

	// �������ʱ��ʼ������ [12/13/2017 yz]
	void					enterOnlineState();

	// ��ͻ��˻Ự�������� [12/16/2017 yz]
	void					connectClientSession();

	// ׼������ʽ��������״̬ [12/16/2017 yz]
	void					finishOnlineState();

	// ����������� [12/13/2017 yz]
	void					leaveOnlineState();

	// �������������� [12/18/2017 yz]
	void					dealInvalidState();

	EOnlineState			getOnlineState() { return mOnlineState; }

	DBPlayerState*			getPlayerState();

	// ��ҵ�¼��ʱ����Ӧclientsession״̬�Ƿ�һ�� [1/17/2018 Chief]
	void					checkClientSessionState();
	//----------------------------------------------------------------------
	// rpc start
	//----------------------------------------------------------------------
	virtual void			rpcRequestPlayerDetialData(uint64 nSessionID);
	virtual BOOL			rpcSessionCheckPlayerIsOnline();
	//----------------------------------------------------------------------
	// rpc end
	//----------------------------------------------------------------------

	//----------------------------------------------------------------------
	// client �������� [1/8/2018 Chief]
	//----------------------------------------------------------------------
public:
	void					requestPlayerDetialData();

	//----------------------------------------------------------------------
	// 
	//----------------------------------------------------------------------
public:
	SNID_t					getSnid();
	const char*				getAccount();

protected:
	SNID_t					mSnid;
	int64					mSessionID;

	PlayerArchive			mPlayerArchive;

	// ����״̬ [12/15/2017 yz]
	EOnlineState			mOnlineState;

	// onlineplayer/session������� [1/17/2018 Chief]
	CTinyTimer				mPlayerSessionSyncTimer;
};

#endif