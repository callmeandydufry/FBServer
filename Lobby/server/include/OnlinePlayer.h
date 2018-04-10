#ifndef __ONLINE_PLAYER_H__
#define __ONLINE_PLAYER_H__

#include "TypeDefine.h"
#include "PlayerArchiveStruct.h"
#include "ServerModuleExportInterface.h"
#include "GameMail.h"
#include "MailDefine.h"

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

	// 请求DB数据 [12/15/2017 yz]
	void					requestDBData();

	// 收到DB返回的玩家完整数据 [12/16/2017 yz]
	void					receiveDBData(PlayerArchive* dbArchive);

	// 玩家上线时初始化数据 [12/13/2017 yz]
	void					enterOnlineState();

	// 请求邮件数据 [2/8/2018 Chief]
	void					trySynMailList();

	// 与客户端会话进行连接 [12/16/2017 yz]
	void					connectClientSession();

	// 准备好正式进入在线状态 [12/16/2017 yz]
	void					finishOnlineState();

	// 玩家下线流程 [12/13/2017 yz]
	void					leaveOnlineState();

	// 清理错误玩家数据 [12/18/2017 yz]
	void					dealInvalidState();

	EOnlineState			getOnlineState() { return mOnlineState; }

	DBPlayerState*			getPlayerState();

	// 玩家登录后定时检测对应clientsession状态是否一致 [1/17/2018 Chief]
	void					checkClientSessionState();
	//----------------------------------------------------------------------
	// rpc start
	//----------------------------------------------------------------------
	virtual void			rpcRequestPlayerDetialData(uint64 nSessionID);
	virtual BOOL			rpcSessionCheckPlayerIsOnline();

	// 同步邮件列表,由邮件模块发起同步 [2/8/2018 Chief]
	virtual void			rpcMgrSynMailList2Player(tagMailConfirm& stListInfo);
	// 请求全部邮件list [2/8/2018 Chief]
	virtual void			rpcRequestPlayerMailList();
	// 阅读邮件 [2/9/2018 Chief]
	virtual void			rpcRequestReadMail(int32 nMailID);
	// 玩家手动删除邮件 [2/9/2018 Chief]
	virtual void			rpcRequestDelMail(int32 nMailID);
	//----------------------------------------------------------------------
	// rpc end
	//----------------------------------------------------------------------

	//----------------------------------------------------------------------
	// client 请求数据 [1/8/2018 Chief]
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

	// 在线状态 [12/15/2017 yz]
	EOnlineState			mOnlineState;

	// onlineplayer/session心跳检测 [1/17/2018 Chief]
	CTinyTimer				mPlayerSessionSyncTimer;

	// 邮件[1/29/2018 Chief]
	tagMailIDList			mPlayerMailArr;
	int32					mNewMailsNum;
};

#endif

// worning: // todo 没有邮件就将按钮隐藏,不让玩家太过频繁的请求所有邮件信息,强占太多资源和带宽
// tips:
//	1.	数据的流转
//  2.	数据从DB获取的效率()

